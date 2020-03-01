#include <cstring>

#include "table_list_adapter.h"
#include "../utils/utils.h"
#include "../oci20/oci20.h"

namespace Data {

    using namespace Utils;
    using namespace Oci20;

	TableEntry::TableEntry()
		: deleted(false) , num_rows(0), blocks(0) {

        memset(&created, 0, sizeof(tm));
        memset(&last_ddl_time, 0, sizeof(tm));
        memset(&last_analyzed, 0, sizeof(tm));
	}

	TableListAdapter::TableListAdapter(Oci20::Connect& connect)
        : m_connect(connect) {

	}

	size_t TableListAdapter::Query() {

        const int cn_owner = 0;
        const int cn_table_name = 1;
        const int cn_compression = 2;
        const int cn_tablespace_name = 3;
        const int cn_last_analyzed = 4;
        const int cn_created = 5;
        const int cn_last_ddl_time = 6;
        const int cn_partitioning_type = 7;
        const int cn_subpartitioning_type = 8;
        const int cn_def_tablespace_name = 9;
        const int cn_def_compression = 10;
        const int cn_duration = 11;
        const int cn_compress_for = 12;
        const int cn_def_compress_for = 13;
        const int cn_temporary = 14;
        const int cn_iot_type = 15;
        const int cn_partitioned = 16;
        const int cn_num_rows = 17;
        const int cn_blocks = 18;

        static const char* csz_table_sttm =
            "SELECT <RULE> "
            "t1.owner,"
            "t1.table_name,"
            "<COMPRESSION> compression,"
            "t1.tablespace_name,"
            "t1.last_analyzed,"
            "t2.created,"
            "t2.last_ddl_time,"
            "t3.partitioning_type,"
            "t3.subpartitioning_type,"
            "t3.def_tablespace_name,"
            "<DEF_COMPRESSION> def_compression,"
            "t1.duration,"
            "<COMPRESS_FOR> compress_for,"
            "<DEF_COMPRESS_FOR> def_compress_for,"
            "t1.temporary,"
            "t1.iot_type,"
            "t1.partitioned,"
            "t1.num_rows,"
            "t1.blocks"
            " FROM sys.all_tables t1, sys.all_objects t2, sys.all_part_tables t3"
            " WHERE t1.owner = :owner"
            " AND t2.owner = :owner AND t2.object_type = 'TABLE'"
            " AND t1.owner = t2.owner AND t1.table_name = t2.object_name"
            " AND t3.owner(+) = :owner"
            " AND t1.owner = t3.owner(+) AND t1.table_name = t3.table_name(+)"
            " <IOT_FILTER>"; // iot_name is null because of overflow tables

        Utils::Substitutor substitutor;
        Connect::ServerVersion serverVersion = m_connect.GetVersion();

        substitutor.AddPair("<RULE>", (serverVersion < Connect::ServerVersion::Server10X) ? "/*+RULE*/" : "");
        substitutor.AddPair("<COMPRESSION>", (serverVersion >= Connect::ServerVersion::Server10X) ? "t1.compression" : "NULL");
        substitutor.AddPair("<COMPRESS_FOR>", (serverVersion >= Connect::ServerVersion::Server11X) ? "t1.compress_for" : "NULL");
        substitutor.AddPair("<DEF_COMPRESS_FOR>", (serverVersion >= Connect::ServerVersion::Server11X) ? "t3.def_compress_for" : "NULL");
        substitutor.AddPair("<DEF_COMPRESSION>", (serverVersion >= Connect::ServerVersion::Server9X) ? "t3.def_compression" : "NULL");
        substitutor.AddPair("<IOT_FILTER>", (serverVersion >= Connect::ServerVersion::Server81X) ? "AND t1.iot_name IS NULL" : "");

        substitutor << csz_table_sttm;

        //TODO: not hard code pls
        std::string m_schema = "SYSTEM";

        BuffCursor cursor(m_connect, 50, 196);
        cursor.Prepare(substitutor.GetResult());
        cursor.Bind(":owner", m_schema);
        cursor.Execute();
        while (cursor.Fetch()) {
            TableEntry table;

            table.owner = cursor.ToString(cn_owner);
            table.table_name = cursor.ToString(cn_table_name);

            if (cursor.ToString(cn_temporary) == "Y") {
                table.table_type = "TEMP";
                std::string duration = cursor.ToString(cn_duration);

                if(!StringUtil::NoCaseCompare(duration, "SYS$", 4)) {
                    table.table_type += " / ";
                    table.table_type += duration.substr(4);
                }
            }
            else if (cursor.ToString(cn_partitioned) == "YES") {
                table.table_type = "PART";
                std::string partitioning_type = cursor.ToString(cn_partitioning_type);
                std::string subpartitioning_type = cursor.ToString(cn_subpartitioning_type);

                if (!cursor.IsNull(cn_iot_type)) {
                    table.table_type += " by ";
                    table.table_type += cursor.ToString(cn_iot_type);
                }

                table.table_type += " - ";
                table.table_type += partitioning_type;

                if (subpartitioning_type != "NONE") {
                    table.table_type += " / ";
                    table.table_type += subpartitioning_type;
                }
            }
            else
            {
                if (!cursor.IsNull(cn_iot_type))
                    table.table_type = cursor.ToString(cn_iot_type);
                else
                    table.table_type = ".";
            }

            table.compression = !cursor.IsNull(cn_compression) ? cursor.ToString(cn_compression) : cursor.ToString(cn_def_compression);
            table.tablespace_name = !cursor.IsNull(cn_tablespace_name) ? cursor.ToString(cn_tablespace_name) : cursor.ToString(cn_def_tablespace_name);

            cursor.GetTime(cn_last_analyzed, table.last_analyzed);
            cursor.GetTime(cn_created, table.created);
            cursor.GetTime(cn_last_ddl_time, table.last_ddl_time);

            if (table.compression == "ENABLED") {
                if (!cursor.IsNull(cn_def_compress_for))
                    table.compression = cursor.ToString(cn_def_compress_for);
                else if (!cursor.IsNull(cn_compress_for))
                    table.compression = cursor.ToString(cn_compress_for);
                else
                    table.compression = "BASIC";
            }
            else if (table.compression == "DISABLED")
                table.compression = ".";
            else if (table.compression == "NONE")
                table.compression = ".";
            else if (table.compression.empty())
                table.compression = ".";

            table.num_rows = cursor.ToInt64(cn_num_rows);
            table.blocks = cursor.ToInt64(cn_blocks);

            m_entries.push_back(table);
        }

        return m_entries.size();
	}
};

