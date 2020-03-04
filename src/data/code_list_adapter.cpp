#include <cstring>

#include "code_list_adapter.h"
#include "../utils/utils.h"
#include "../oci20/oci20.h"

namespace Data {

    using namespace Utils;
    using namespace Oci20;

    CodeEntry::CodeEntry()
		: deleted(false) {

        memset(&created      , 0, sizeof(tm));
        memset(&last_ddl_time, 0, sizeof(tm));
	}

    CodeListAdapter::CodeListAdapter(Oci20::Connect& connect, MonoType monoType)
        : m_connect(connect), m_monoType(monoType){

	}

	size_t CodeListAdapter::Query() {

        if (!m_connect.IsOpen())
            return -1;

        const int cn_owner = 0;
        const int cn_object_name = 1;
        const int cn_status = 2;
        const int cn_created = 3;
        const int cn_last_ddl_time = 4;

        static const char* csz_code_sttm =
            "SELECT * FROM ("
            "  SELECT"
            "    owner,"
            "    object_name,"
            "    status,"
            "    created,"
            "    last_ddl_time,"
            "    Row_Number() OVER (PARTITION BY owner, object_name, object_type ORDER BY last_ddl_time DESC) rn"
            "  FROM sys.all_objects"
            "    WHERE owner = :owner"
            "      AND object_type = :type"
            ") WHERE rn = 1";

        std::string strNameType;

        switch (m_monoType) {
        case MonoType::Function:    strNameType = "FUNCTION";     break;
        case MonoType::Package:     strNameType = "PACKAGE";      break;
        case MonoType::PackageBody: strNameType = "PACKAGE BODY"; break;
        case MonoType::Java:        strNameType = "JAVA SOURCE";  break;
        case MonoType::Procedure:   
        default:
            strNameType = "PROCEDURE";
            break;
        }

        BuffCursor cursor(m_connect, 50, 196);
        cursor.Prepare(csz_code_sttm);
        cursor.Bind(":owner", m_schema);
        cursor.Bind(":type", strNameType);

        cursor.Execute();
        while (cursor.Fetch()) {
            CodeEntry entry;

            entry.owner = cursor.ToString(cn_owner);
            entry.object_name = cursor.ToString(cn_object_name);
            entry.status = cursor.ToString(cn_status);

            cursor.GetTime(cn_created, entry.created);
            cursor.GetTime(cn_last_ddl_time, entry.last_ddl_time);

            m_entries.push_back(entry);
        }

        return m_entries.size();
	}
};

