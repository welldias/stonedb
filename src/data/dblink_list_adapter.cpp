#include <cstring>

#include "dblink_list_adapter.h"
#include "../utils/utils.h"
#include "../oci20/oci20.h"

namespace Data {

    using namespace Utils;
    using namespace Oci20;

    DbLinkEntry::DbLinkEntry()
		: deleted(false) {
	}

    DbLinkListAdapter::DbLinkListAdapter(Oci20::Connect& connect)
        : m_connect(connect) {

	}

	size_t DbLinkListAdapter::Query() {

        if (!m_connect.IsOpen())
            return -1;

        const int cn_owner = 0;
        const int cn_db_link = 1;
        const int cn_username = 2;
        const int cn_host = 3;

        static const char* csz_dblink_sttm = "SELECT <RULE> owner, db_link, username, host FROM sys.all_db_links WHERE owner = :owner";

        Utils::Substitutor substitutor;
        Connect::ServerVersion serverVersion = m_connect.GetVersion();

        substitutor.AddPair("<RULE>", (serverVersion < Connect::ServerVersion::Server10X) ? "/*+RULE*/" : "");

        substitutor << csz_dblink_sttm;

        BuffCursor cursor(m_connect, 50, 196);
        cursor.Prepare(substitutor.GetResult());
        cursor.Bind(":owner", m_schema);
        cursor.Execute();
        while (cursor.Fetch()) {

            DbLinkEntry entry;

            entry.owner    = cursor.ToString(cn_owner   );
            entry.db_link  = cursor.ToString(cn_db_link );
            entry.username = cursor.ToString(cn_username);
            entry.host     = cursor.ToString(cn_host    );

            m_entries.push_back(entry);
        }

        return m_entries.size();
	}
};

