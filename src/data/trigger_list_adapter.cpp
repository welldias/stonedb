#include <cstring>

#include "trigger_list_adapter.h"
#include "../utils/utils.h"
#include "../oci20/oci20.h"

namespace Data {

    using namespace Utils;
    using namespace Oci20;

    TriggerEntry::TriggerEntry()
		: deleted(false) {

        memset(&created      , 0, sizeof(tm));
        memset(&last_ddl_time, 0, sizeof(tm));
	}

    TriggerListAdapter::TriggerListAdapter(Oci20::Connect& connect)
        : m_connect(connect) {

	}

	size_t TriggerListAdapter::Query() {

        if (!m_connect.IsOpen())
            return -1;

        const int cn_owner = 0;
        const int cn_trigger_name = 1;
        const int cn_enabled = 2;
        const int cn_table_owner = 3;
        const int cn_table_name = 4;
        const int cn_trigger_type = 5;
        const int cn_triggering_event = 6;
        const int cn_when_clause = 7;
        const int cn_created = 8;
        const int cn_last_ddl_time = 9;
        const int cn_status = 10;

        static const char* csz_trigger_sttm =
            "SELECT <RULE> "
            " t.owner,"
            " t.trigger_name,"
            " t.status enabled,"
            " t.table_owner,"
            " t.table_name,"
            " t.trigger_type,"
            " t.triggering_event,"
            " t.when_clause,"
            " o.created,"
            " o.last_ddl_time,"
            " o.status "
            " FROM sys.all_objects o, sys.all_triggers t"
            " WHERE o.owner(+) = :owner"
            " AND t.owner = :owner"
            " AND o.owner(+) = t.owner"
            " AND object_name(+) = trigger_name"
            " AND object_type(+) = 'TRIGGER'";

        Utils::Substitutor substitutor;
        Connect::ServerVersion serverVersion = m_connect.GetVersion();

        substitutor.AddPair("<RULE>", (serverVersion < Connect::ServerVersion::Server10X) ? "/*+RULE*/" : "");

        substitutor << csz_trigger_sttm;

        BuffCursor cursor(m_connect, 50, 196);
        cursor.Prepare(substitutor.GetResult());
        cursor.Bind(":owner", m_schema);
        cursor.Execute();
        while (cursor.Fetch()) {
            TriggerEntry trigger;

            trigger.owner = cursor.ToString(cn_owner);
            trigger.trigger_name = cursor.ToString(cn_trigger_name);
            trigger.enabled = cursor.ToString(cn_enabled);
            trigger.table_owner = cursor.ToString(cn_table_owner);
            trigger.table_name = cursor.ToString(cn_table_name);
            trigger.trigger_type = cursor.ToString(cn_trigger_type);
            trigger.triggering_event = cursor.ToString(cn_triggering_event);
            trigger.when_clause = cursor.ToString(cn_when_clause);
            trigger.status = cursor.ToString(cn_status);

            cursor.GetTime(cn_created, trigger.created);
            cursor.GetTime(cn_last_ddl_time, trigger.last_ddl_time);

            m_entries.push_back(trigger);
        }

        return m_entries.size();
	}
};

