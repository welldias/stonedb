#include <cstring>

#include "synonym_list_adapter.h"
#include "../utils/utils.h"
#include "../oci20/oci20.h"

namespace Data {

    using namespace Utils;
    using namespace Oci20;

    SynonymEntry::SynonymEntry()
		: deleted(false) {
	}

    SynonymListAdapter::SynonymListAdapter(Oci20::Connect& connect)
        : m_connect(connect) {

	}

	size_t SynonymListAdapter::Query() {

        if (!m_connect.IsOpen())
            return -1;

        const int cn_owner = 0;
        const int cn_synonym_name = 1;
        const int cn_table_owner = 2;
        const int cn_table_name = 3;
        const int cn_db_link = 4;
        const int cn_status = 5;

        static const char* csz_synonym_sttm =
            "SELECT <RULE> "
            "s.owner,"
            "s.synonym_name,"
            "s.table_owner,"
            "s.table_name,"
            "s.db_link,"
            "'VALID' status "
            "FROM sys.all_synonyms s "
            "WHERE owner = :owner "
            "AND EXISTS ("
            "SELECT 1 FROM sys.all_objects o "
            "WHERE s.table_name  = o.object_name "
            "AND s.table_owner = o.owner "
            ")"
            "UNION ALL"
            "("
            "SELECT "
            "s.owner,"
            "s.synonym_name,"
            "s.table_owner,"
            "s.table_name,"
            "s.db_link,"
            "'INVALID' status "
            "FROM ("
            "SELECT * FROM sys.all_synonyms WHERE owner = :owner "
            "MINUS "
            "SELECT * FROM sys.all_synonyms s "
            "WHERE owner = :owner "
            "AND EXISTS ("
            "SELECT 1 FROM sys.all_objects o "
            "WHERE s.table_name  = o.object_name "
            "AND s.table_owner = o.owner "
            ")"
            ") s"
            ")";

        static const char* csz_synonym_sttm_10g =
            "WITH valid AS"
            "("
            "SELECT "
            "s.owner,"
            "s.synonym_name,"
            "s.table_owner,"
            "s.table_name,"
            "s.db_link,"
            "'VALID' status "
            "FROM sys.all_synonyms s "
            "WHERE owner = :owner "
            "AND ("
            "db_link IS NOT NULL "
            "OR EXISTS "
            "("
            "SELECT 1 FROM sys.all_objects o "
            "WHERE s.table_name  = o.object_name "
            "AND s.table_owner = o.owner "
            ")"
            ")"
            ")"
            "SELECT * FROM valid "
            "UNION ALL "
            "SELECT "
            "s.owner,"
            "s.synonym_name,"
            "s.table_owner,"
            "s.table_name,"
            "s.db_link,"
            "'INVALID' status "
            "FROM sys.all_synonyms s "
            "WHERE owner = :owner "
            "AND (s.owner, s.synonym_name)"
            "NOT IN (SELECT owner, synonym_name FROM valid)"
            ;

        static const char* csz_synonym_user_fast_sttm =
            "SELECT <RULE> "
            ":owner,"
            "s.synonym_name,"
            "s.table_owner,"
            "s.table_name,"
            "s.db_link,"
            "o.status "
            "FROM sys.user_synonyms s, sys.user_objects o "
            "WHERE :owner= :owner "
            "AND o.object_type = 'SYNONYM' "
            "AND s.synonym_name = o.object_name "
            ;

        static const char* csz_synonym_fast_sttm =
            "SELECT <RULE> "
            "s.owner,"
            "s.synonym_name,"
            "s.table_owner,"
            "s.table_name,"
            "s.db_link,"
            "'NA' status "
            "FROM sys.all_synonyms s "
            "WHERE s.owner = :owner "
            ;

        Utils::Substitutor substitutor;
        Connect::ServerVersion serverVersion = m_connect.GetVersion();

        substitutor.AddPair("<RULE>", (serverVersion < Connect::ServerVersion::Server10X) ? "/*+RULE*/" : "");

        if (Settings::GetSynonymWithoutObjectInvalid()) {
            substitutor << ((serverVersion < Connect::ServerVersion::Server10X) ? csz_synonym_sttm : csz_synonym_sttm_10g);
        }
        else if (serverVersion < Connect::ServerVersion::Server81X) {
            substitutor << csz_synonym_fast_sttm;
        }
        else {
            std::string currentUser = Settings::GetCurrentDBUser();
            std::string currentSchema = Settings::GetCurrentDBSchema();

            if (m_schema == currentUser && m_schema == currentSchema)
                substitutor << csz_synonym_user_fast_sttm;
            else
                substitutor << csz_synonym_fast_sttm;
        }

        BuffCursor cursor(m_connect, 50, 196);
        cursor.Prepare(substitutor.GetResult());
        cursor.Bind(":owner", m_schema);
        cursor.Execute();
        while (cursor.Fetch()) {
            SynonymEntry entry;

            entry.owner = cursor.ToString(cn_owner);
            entry.synonym_name = cursor.ToString(cn_synonym_name);
            entry.table_owner = cursor.ToString(cn_table_owner);
            entry.table_name = cursor.ToString(cn_table_name);
            entry.db_link = cursor.ToString(cn_db_link);
            entry.status = cursor.ToString(cn_status);

            m_entries.push_back(entry);
        }

        return m_entries.size();
	}
};

