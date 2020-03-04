#include <cstring>

#include "user_list_adapter.h"
#include "../utils/utils.h"
#include "../oci20/oci20.h"

namespace Data {

    using namespace Utils;
    using namespace Oci20;

    UserListAdapter::UserListAdapter(Oci20::Connect& connect)
        : m_connect(connect) {

	}

	size_t UserListAdapter::Query() {

        if (!m_connect.IsOpen())
            return -1;

        BuffCursor cursor(m_connect, "select username from all_users a_u where username <> user");
        cursor.Execute();
        while (cursor.Fetch()) {
            UserEntry entry;
            
            entry.user = cursor.ToString(0);

            m_entries.push_back(entry);
        }

        return m_entries.size();
	}
};

