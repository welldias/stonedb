#include <cstring>

#include "recycled_bin_list_adapter.h"
#include "../utils/utils.h"
#include "../oci20/oci20.h"

namespace Data {

    using namespace Utils;
    using namespace Oci20;

    RecyclebinEntry::RecyclebinEntry()
		: deleted(false), space(0), info_type(ListDataProvider::InfoType::Undefined) {

	}

    RecyclebinListAdapter::RecyclebinListAdapter(Oci20::Connect& connect)
        : m_connect(connect) {

	}

	size_t RecyclebinListAdapter::Query() {

        if (!m_connect.IsOpen())
            return -1;

        if (m_connect.GetVersion() < Connect::ServerVersion::Server10X)
            return -1;

        const int cn_object_name = 0;
        const int cn_original_name = 1;
        const int cn_operation = 2;
        const int cn_type = 3;
        const int cn_ts_name = 4;
        const int cn_createtime = 5;
        const int cn_droptime = 6;
        const int cn_dropscn = 7;
        const int cn_partition_name = 8;
        const int cn_can_undrop = 9;
        const int cn_can_purge = 10;
        const int cn_related = 11;
        const int cn_base_object = 12;
        const int cn_purge_object = 13;
        const int cn_space = 14;

        static const char* csz_recyclebin_sttm =
            "SELECT"
            " object_name,"
            " original_name,"
            " operation,"
            " type,"
            " ts_name,"
            " createtime,"
            " droptime,"
            " dropscn,"
            " partition_name,"
            " can_undrop,"
            " can_purge,"
            " related,"
            " base_object,"
            " purge_object,"
            " space"
            " FROM user_recyclebin"
            " WHERE USER = :owner";

        BuffCursor cursor(m_connect, 50, 196);
        cursor.Prepare(csz_recyclebin_sttm);
        cursor.Bind(":owner", m_schema);
        cursor.Execute();
        while (cursor.Fetch()) {
            RecyclebinEntry entry;

            entry.original_name  = cursor.ToString(cn_original_name );
            entry.operation      = cursor.ToString(cn_operation     );
            entry.type           = cursor.ToString(cn_type          );
            entry.partition_name = cursor.ToString(cn_partition_name);
            entry.ts_name        = cursor.ToString(cn_ts_name       );
            entry.createtime     = cursor.ToString(cn_createtime    );
            entry.droptime       = cursor.ToString(cn_droptime      );
            entry.can_undrop     = cursor.ToString(cn_can_undrop    );
            entry.can_purge      = cursor.ToString(cn_can_purge     );
            entry.space          = cursor.ToInt   (cn_space         );
            // HIDDEN
            entry.object_name    = cursor.ToString(cn_object_name   );
            entry.dropscn        = cursor.ToString(cn_dropscn       );
            entry.related        = cursor.ToString(cn_related       );
            entry.base_object    = cursor.ToString(cn_base_object   );
            entry.purge_object   = cursor.ToString(cn_purge_object  );

            std::string buff =  StringUtil::ToUpper(entry.type);
            entry.info_type = InfoType::Recyclebin;
            if (buff.find("TABLE") != std::string::npos || buff.find("LOB") != std::string::npos)
                entry.info_type = InfoType::Table;
            else if (buff.find("INDEX") != std::string::npos)
                entry.info_type = InfoType::Index;

            m_entries.push_back(entry);
        }

        return m_entries.size();
	}
};

