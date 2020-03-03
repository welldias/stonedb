#include <cstring>

#include "index_list_adapter.h"
#include "../utils/utils.h"
#include "../oci20/oci20.h"

namespace Data {

    using namespace Utils;
    using namespace Oci20;

    IndexEntry::IndexEntry()
		: deleted(false) {

        memset(&created      , 0, sizeof(tm));
        memset(&last_ddl_time, 0, sizeof(tm));
        memset(&last_analyzed, 0, sizeof(tm));
	}

    IndexListAdapter::IndexListAdapter(Oci20::Connect& connect)
        : m_connect(connect) {

	}

	size_t IndexListAdapter::Query() {

        if (!m_connect.IsOpen())
            return -1;

        const int cn_table_owner = 0;
        const int cn_owner = 1;
        const int cn_index_name = 2;
        const int cn_table_name = 3;
        const int cn_index_type = 4;
        const int cn_uniqueness = 5;
        const int cn_tablespace_name = 6;
        const int cn_partitioned = 7;
        const int cn_created = 8;
        const int cn_last_ddl_time = 9;
        const int cn_last_analyzed = 10;

        static const char* csz_index_sttm =
            "SELECT <RULE> "
            "t1.table_owner,"
            "t1.owner,"
            "t1.index_name,"
            "t1.table_name,"
            "<INDEX_TYPE>,"
            "t1.uniqueness,"
            "t1.tablespace_name,"
            "t1.partitioned,"
            "t2.created,"
            "t2.last_ddl_time,"
            "t1.last_analyzed"
            " FROM sys.all_indexes t1, sys.all_objects t2"
            " WHERE t1.owner = :owner"
            " AND t2.owner(+) = :owner AND t2.object_type(+) = 'INDEX'"
            " AND t1.owner = t2.owner(+) AND t1.index_name = t2.object_name(+)"
            " <IOT_FILTER>";

        Utils::Substitutor substitutor;
        Connect::ServerVersion serverVersion = m_connect.GetVersion();

        substitutor.AddPair("<RULE>", (serverVersion < Connect::ServerVersion::Server10X) ? "/*+RULE*/" : "");
        substitutor.AddPair("<INDEX_TYPE>", (serverVersion > Connect::ServerVersion::Server73X) ? "t1.index_type" : "NULL");
        substitutor.AddPair("<IOT_FILTER>", (serverVersion > Connect::ServerVersion::Server73X) ? "AND t1.index_type != \'IOT - TOP\'" : "");

        substitutor << csz_index_sttm;

        BuffCursor cursor(m_connect, 50, 196);
        cursor.Prepare(substitutor.GetResult());
        cursor.Bind(":owner", m_schema);
        cursor.Execute();
        while (cursor.Fetch()) {
            IndexEntry index;

            index.table_owner = cursor.ToString(cn_table_owner);
            index.owner = cursor.ToString(cn_owner);
            index.index_name = cursor.ToString(cn_index_name);
            index.table_name = cursor.ToString(cn_table_name);
            index.index_type = cursor.ToString(cn_index_type);
            index.uniqueness = cursor.ToString(cn_uniqueness);
            index.tablespace_name = cursor.ToString(cn_tablespace_name);
            index.partitioned = cursor.ToString(cn_partitioned);

            cursor.GetTime(cn_last_analyzed, index.last_analyzed);
            cursor.GetTime(cn_created, index.created);
            cursor.GetTime(cn_last_ddl_time, index.last_ddl_time);

            m_entries.push_back(index);
        }

        return m_entries.size();
	}
};

