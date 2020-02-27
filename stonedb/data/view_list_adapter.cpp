#include <cstring>

#include "view_list_adapter.h"

#include "../utils/utils.h"
#include "../oci20/oci20.h"

namespace Data {

    using namespace Utils;
    using namespace Oci20;

    ViewEntry::ViewEntry()
		: deleted(false) {

        memset(&created, 0, sizeof(tm));
        memset(&last_ddl_time, 0, sizeof(tm));
	}

    ViewListAdapter::ViewListAdapter(Oci20::Connect& connect)
        : m_connect(connect) {

	}

    size_t ViewListAdapter::Query() {

        const int cn_owner = 0;
        const int cn_view_name = 1;
        const int cn_text_length = 2;
        const int cn_created = 3;
        const int cn_last_ddl_time = 4;
        const int cn_status = 5;

        static const char* csz_sttm =
            "SELECT <RULE> "
            "v.owner,"
            "v.view_name,"
            "v.text_length,"
            "o.created,"
            "o.last_ddl_time,"
            "o.status"
            " FROM sys.all_objects o, sys.all_views v"
            " WHERE o.owner = :owner"
            " AND v.owner = :owner"
            " AND o.owner = v.owner"
            " AND object_name = view_name"
            " AND object_type = 'VIEW'";

        Utils::Substitutor substitutor;
        Connect::ServerVersion serverVersion = m_connect.GetVersion();

        substitutor.AddPair("<RULE>", (serverVersion < Connect::ServerVersion::Server10X) ? "/*+RULE*/" : "");
        substitutor << csz_sttm;

        //TODO: not hard code pls
        std::string m_schema = "SYSTEM";

        BuffCursor cursor(m_connect, 50, 196);
        cursor.Prepare(substitutor.GetResult());
        cursor.Bind(":owner", m_schema);
        cursor.Execute();
        while (cursor.Fetch()) {
            ViewEntry view;

            view.owner = cursor.ToString(cn_owner);
            view.view_name = cursor.ToString(cn_view_name);
            view.text_length = cursor.ToInt(cn_text_length);
            view.status = cursor.ToString(cn_status);

            cursor.GetTime(cn_created, view.created);
            cursor.GetTime(cn_last_ddl_time, view.last_ddl_time);

            m_entries.push_back(view);
        }

		return m_entries.size();
	}
};

