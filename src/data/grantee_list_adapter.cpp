#include <cstring>

#include "grantee_list_adapter.h"
#include "../utils/utils.h"
#include "../oci20/oci20.h"

namespace Data {

    using namespace Utils;
    using namespace Oci20;

    GranteeListAdapter::GranteeListAdapter(Oci20::Connect& connect)
        : m_connect(connect) {

	}

	size_t GranteeListAdapter::Query() {

        if (!m_connect.IsOpen())
            return -1;

        const int cn_grantee = 0;

        static const char* csz_grantee_sttm =
            " SELECT /*+NO_MERGE*/"
            " DISTINCT grantee"
            " FROM sys.all_tab_privs"
            " WHERE table_schema = :owner"
            " UNION"
            " SELECT /*+NO_MERGE*/"
            " DISTINCT grantee"
            " FROM sys.all_col_privs"
            " WHERE table_schema = :owner";

        BuffCursor cursor(m_connect, 50, 196); 
        cursor.Prepare(csz_grantee_sttm);
        cursor.Bind(":owner", m_schema);
        cursor.Execute();

        while (cursor.Fetch()) {
            GranteeEntry entry;

            entry.grantee = cursor.ToString(cn_grantee);

            m_entries.push_back(entry);
        }

        return m_entries.size();
	}
};

