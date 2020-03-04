#include <cstring>

#include "invalid_object_list_adapter.h"
#include "../utils/utils.h"
#include "../oci20/oci20.h"

namespace Data {

    using namespace Utils;
    using namespace Oci20;

    InvalidObjectEntry::InvalidObjectEntry()
		: deleted(false), info_type(ListDataProvider::InfoType::Undefined) {

        memset(&created      , 0, sizeof(tm));
        memset(&last_ddl_time, 0, sizeof(tm));
	}

    InvalidObjectListAdapter::InvalidObjectListAdapter(Oci20::Connect& connect)
        : m_connect(connect) {

	}

	size_t InvalidObjectListAdapter::Query() {

        if (!m_connect.IsOpen())
            return -1;

        if (m_connect.GetVersion() < Connect::ServerVersion::Server10X)
            return -1;

        const int cn_owner = 0;
        const int cn_object_name = 1;
        const int cn_object_type = 2;
        const int cn_status = 3;
        const int cn_created = 4;
        const int cn_last_ddl_time = 5;

        static const char* csz_invalidobj_sttm =
            "SELECT"
            " owner,"
            " object_name,"
            " object_type,"
            " status,"
            " created,"
            " last_ddl_time"
            " FROM sys.all_objects"
            " WHERE owner = :owner"
            " AND status = 'INVALID'";

        BuffCursor cursor(m_connect, 50, 196);
        cursor.Prepare(csz_invalidobj_sttm);
        cursor.Bind(":owner", m_schema);
        cursor.Execute();
        while (cursor.Fetch()) {
            InvalidObjectEntry entry;

            entry.owner = cursor.ToString(cn_owner);
            entry.object_name = cursor.ToString(cn_object_name);
            entry.object_type = cursor.ToString(cn_object_type);
            entry.status = cursor.ToString(cn_status);

            cursor.GetTime(cn_created, entry.created);
            cursor.GetTime(cn_last_ddl_time, entry.last_ddl_time);

            if (entry.object_type == "FUNCTION")          entry.info_type = InfoType::Function;
            else if (entry.object_type == "PROCEDURE")    entry.info_type = InfoType::Procedure;
            else if (entry.object_type == "PACKAGE")      entry.info_type = InfoType::Package;
            else if (entry.object_type == "PACKAGE BODY") entry.info_type = InfoType::PackageBody;
            else if (entry.object_type == "TRIGGER")      entry.info_type = InfoType::Trigger;
            else if (entry.object_type == "VIEW")         entry.info_type = InfoType::View;
            else if (entry.object_type == "SYNONYM")      entry.info_type = InfoType::Synonym;
            else if (entry.object_type == "TYPE")         entry.info_type = InfoType::Type;
            else if (entry.object_type == "TYPE_BODY")    entry.info_type = InfoType::TypeBody;
            else if (entry.object_type == "JAVA SOURCE")  entry.info_type = InfoType::Java;
            else if (entry.object_type == "JAVA CLASS")   entry.info_type = InfoType::Java;
            else entry.info_type = InfoType::Undefined;

            m_entries.push_back(entry);
        }

        return m_entries.size();
	}
};

