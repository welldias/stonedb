#include <cstring>

#include "constraint_list_adapter.h"

#include "../utils/utils.h"
#include "../oci20/oci20.h"

namespace Data {

    using namespace Utils;
    using namespace Oci20;

    ConstraintEntry::ConstraintEntry()
		: deleted(false) {
	}

    ConstraintListAdapter::ConstraintListAdapter(Oci20::Connect& connect, const std::string& constraintType)
        : m_connect(connect), m_constraintType(constraintType) {

	}

    size_t ConstraintListAdapter::Query() {

        const int cn_owner = 0;
        const int cn_table_name = 1;
        const int cn_constraint_name = 2;
        const int cn_status = 3;
        const int cn_deferrable = 4;
        const int cn_deferred = 5;
        const int cn_search_condition = 6;
        const int cn_r_owner = 7;
        const int cn_r_constraint_name = 8;
        const int cn_delete_rule = 9;

        static const char* csz_sttm =
            "SELECT <RULE>"
            " owner, table_name, constraint_name, status,"
            " <DEFERRABLE>,"
            " <DEFERRED>,"
            " search_condition,"
            " decode(owner, r_owner, null, r_owner) r_owner, r_constraint_name,"
            " decode(delete_rule,'NO ACTION', null, delete_rule) delete_rule"
            " FROM sys.all_constraints"
            " WHERE owner = :owner"
            " AND constraint_type = :type"
            " <NOT_RECYCLED>";

        Utils::Substitutor substitutor;
        Connect::ServerVersion serverVersion = m_connect.GetVersion();

        substitutor.AddPair("<RULE>", (serverVersion < Connect::ServerVersion::Server10X) ? "/*+RULE*/" : "");
        substitutor.AddPair("<RULE>", (serverVersion < Connect::ServerVersion::Server10X) ? "/*+RULE*/" : "");
        substitutor.AddPair("<DEFERRABLE>", (serverVersion > Connect::ServerVersion::Server73X) ? "decode(deferrable,'DEFERRABLE','Y')" : "NULL");
        substitutor.AddPair("<DEFERRED>", (serverVersion > Connect::ServerVersion::Server73X) ? "decode(deferred,'DEFERRED','Y')" : "NULL");
        substitutor.AddPair("<NOT_RECYCLED>", (serverVersion >= Connect::ServerVersion::Server10X) ? "AND (owner, table_name) NOT IN (SELECT user, object_name FROM user_recyclebin)" : "");
        
        substitutor << csz_sttm;

        //TODO: not hard code pls
        std::string m_schema = "SYSTEM";

        BuffCursor cursor(m_connect, 50, 196);
        cursor.Prepare(substitutor.GetResult());
        cursor.Bind(":owner", m_schema.c_str());
        cursor.Bind(":type", m_constraintType.c_str());

        cursor.Execute();
        while (cursor.Fetch()) {
            ConstraintEntry code;

            code.owner = cursor.ToString(cn_owner);
            code.constraint_name = cursor.ToString(cn_constraint_name);
            code.table_name = cursor.ToString(cn_table_name);
            code.deferrable = cursor.ToString(cn_deferrable);
            code.deferred = cursor.ToString(cn_deferred);
            code.status = cursor.ToString(cn_status);
            code.search_condition = cursor.ToString(cn_search_condition);
            code.r_owner = cursor.ToString(cn_r_owner);
            code.r_constraint_name = cursor.ToString(cn_r_constraint_name);
            code.delete_rule = cursor.ToString(cn_delete_rule);

            if (!code.search_condition.empty() && *code.search_condition.begin() == '\"') {
                size_t second_quote_pos = code.search_condition.find('\"', 1);
                if (second_quote_pos != std::string::npos && code.search_condition.substr(second_quote_pos) == "\" IS NOT NULL")
                    continue;
            }

            code.search_condition = Utils::StringUtil::Trim(code.search_condition);

            m_entries.push_back(code);
        }

		return m_entries.size();
	}
};

