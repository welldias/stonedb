#include <cstring>

#include "type_list_adapter.h"
#include "../utils/utils.h"
#include "../oci20/oci20.h"

namespace Data {

    using namespace Utils;
    using namespace Oci20;

    TypeCodeEntry::TypeCodeEntry()
		: deleted(false), attributes(0), methods(0) {

        memset(&created      , 0, sizeof(tm));
        memset(&last_ddl_time, 0, sizeof(tm));
	}

    TypeCodeListAdapter::TypeCodeListAdapter(Oci20::Connect& connect, MonoType monoType)
        : m_connect(connect), m_monoType(monoType){

	}

	size_t TypeCodeListAdapter::Query() {

        if (!m_connect.IsOpen())
            return -1;

        const int cn_owner = 0;
        const int cn_object_name = 1;
        const int cn_typecode = 2;
        const int cn_attributes = 3;
        const int cn_methods = 4;
        const int cn_incomplete = 5;
        const int cn_status = 6;
        const int cn_created = 7;
        const int cn_last_ddl_time = 8;

        static const char* csz_typeobject_sttm =
            "SELECT * FROM ("
            "SELECT "
            "v1.owner,"
            "v1.type_name,"
            "v1.typecode,"
            "v1.attributes,"
            "v1.methods,"
            "v1.incomplete,"
            "v2.status,"
            "v2.created,"
            "v2.last_ddl_time,"
            "Row_Number() OVER (PARTITION BY v2.owner, v2.object_name, v2.object_type ORDER BY v2.last_ddl_time DESC) rn "
            "FROM sys.all_types v1, sys.all_objects v2 "
            "WHERE v2.owner = :owner "
            "AND v2.object_type = :type "
            "AND v2.owner = v1.owner "
            "AND v1.type_name = v2.object_name"
            ") WHERE rn = 1";

        BuffCursor cursor(m_connect, 50, 196);
        cursor.Prepare(csz_typeobject_sttm);
        cursor.Bind(":owner", m_schema);
        cursor.Bind(":type", (m_monoType == MonoType::Bodies ? "TYPE BODY" : "TYPE"));

        cursor.Execute();
        while (cursor.Fetch()) {
            TypeCodeEntry entry;

            entry.owner      = cursor.ToString(cn_owner      );
            entry.object_name= cursor.ToString(cn_object_name);
            entry.status     = cursor.ToString(cn_status     );

            entry.typecode   = cursor.ToString(cn_typecode  );
            entry.attributes = cursor.ToInt   (cn_attributes);
            entry.methods    = cursor.ToInt   (cn_methods   );
            entry.incomplete = cursor.ToString(cn_incomplete);

            cursor.GetTime(cn_created,       entry.created);
            cursor.GetTime(cn_last_ddl_time, entry.last_ddl_time);

            m_entries.push_back(entry);
        }

        return m_entries.size();
	}
};

