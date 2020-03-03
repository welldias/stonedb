#include <cstring>

#include "sequence_list_adapter.h"
#include "../utils/utils.h"
#include "../oci20/oci20.h"

namespace Data {

    using namespace Utils;
    using namespace Oci20;

    SequenceEntry::SequenceEntry()
		: deleted(false) {

        memset(&created      , 0, sizeof(tm));
        memset(&last_ddl_time, 0, sizeof(tm));
	}

    SequenceListAdapter::SequenceListAdapter(Oci20::Connect& connect)
        : m_connect(connect) {

	}

	size_t SequenceListAdapter::Query() {

        if (!m_connect.IsOpen())
            return -1;

        const int cn_owner = 0;
        const int cn_sequence_name = 1;
        const int cn_last_number = 2;
        const int cn_min_value = 3;
        const int cn_max_value = 4;
        const int cn_increment_by = 5;
        const int cn_cache_size = 6;
        const int cn_cycle_flag = 7;
        const int cn_order_flag = 8;
        const int cn_generated = 9;
        const int cn_created = 10;
        const int cn_last_ddl_time = 11;

        static const char* csz_sequence_sttm =
            "SELECT <RULE> "
            "seq.sequence_owner,"
            "seq.sequence_name,"
            "seq.last_number,"
            "seq.min_value,"
            "seq.max_value,"
            "seq.increment_by,"
            "seq.cache_size, "
            "seq.cycle_flag,"
            "seq.order_flag,"
            "obj.generated,"
            "obj.created,"
            "obj.last_ddl_time "
            "FROM sys.all_sequences seq, sys.all_objects obj "
            "WHERE seq.sequence_owner = :owner "
            "AND obj.owner = :owner AND obj.object_type = 'SEQUENCE' "
            "AND seq.sequence_owner = obj.owner AND seq.sequence_name = obj.object_name" ;

        Utils::Substitutor substitutor;
        Connect::ServerVersion serverVersion = m_connect.GetVersion();

        substitutor.AddPair("<RULE>", (serverVersion < Connect::ServerVersion::Server10X) ? "/*+RULE*/" : "");

        substitutor << csz_sequence_sttm;

        BuffCursor cursor(m_connect, 50, 196);
        cursor.Prepare(substitutor.GetResult());
        cursor.Bind(":owner", m_schema);
        cursor.Execute();
        while (cursor.Fetch()) {
            SequenceEntry entry;

            entry.owner         = cursor.ToString(cn_owner        );
            entry.sequence_name = cursor.ToString(cn_sequence_name);
            entry.last_number   = cursor.ToString(cn_last_number  );
            entry.min_value     = cursor.ToString(cn_min_value    );
            entry.max_value     = cursor.ToString(cn_max_value    );
            entry.increment_by  = cursor.ToString(cn_increment_by );
            entry.cache_size    = cursor.ToString(cn_cache_size   );
            entry.cycle_flag    = cursor.ToString(cn_cycle_flag   );
            entry.order_flag    = cursor.ToString(cn_order_flag   );
            entry.generated     = cursor.ToString(cn_generated    );

            cursor.GetTime(cn_created, entry.created);
            cursor.GetTime(cn_last_ddl_time, entry.last_ddl_time);

            m_entries.push_back(entry);
        }

        return m_entries.size();
	}
};

