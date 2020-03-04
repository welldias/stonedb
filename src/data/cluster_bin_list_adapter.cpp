#include <cstring>

#include "cluster_bin_list_adapter.h"
#include "../utils/utils.h"
#include "../oci20/oci20.h"

namespace Data {

    using namespace Utils;
    using namespace Oci20;

    ClusterEntry::ClusterEntry()
		: deleted(false), hashkeys(0), key_size(0), initial_extent(0), 
        next_extent(0), pct_increase(0), max_extents(0), pct_free(0), pct_used(0) {
	}

    ClusterListAdapter::ClusterListAdapter(Oci20::Connect& connect)
        : m_connect(connect) {

	}

	size_t ClusterListAdapter::Query() {

        if (!m_connect.IsOpen())
            return -1;

        const int cn_owner = 0;
        const int cn_cluster_name = 1;
        const int cn_cluster_type = 2;
        const int cn_function = 3;
        const int cn_hashkeys = 4;
        const int cn_key_size = 5;
        const int cn_tablespace_name = 6;
        const int cn_initial_extent = 7;
        const int cn_next_extent = 8;
        const int cn_pct_increase = 9;
        const int cn_max_extents = 10;
        const int cn_pct_free = 11;
        const int cn_pct_used = 12;


        static const char* csz_cluster_sttm =
            "SELECT <RULE>"
            " owner,"
            " cluster_name,"
            " cluster_type,"
            " function,"
            " hashkeys,"
            " key_size,"
            " tablespace_name,"
            " initial_extent,"
            " next_extent,"
            " pct_increase,"
            " max_extents,"
            " pct_free,"
            " pct_used"
            " FROM sys.all_clusters WHERE owner = :owner";

        Utils::Substitutor substitutor;
        Connect::ServerVersion serverVersion = m_connect.GetVersion();

        substitutor.AddPair("<RULE>", (serverVersion < Connect::ServerVersion::Server10X) ? "/*+RULE*/" : "");

        substitutor << csz_cluster_sttm;

        BuffCursor cursor(m_connect, 50, 196);
        cursor.Prepare(substitutor.GetResult());
        cursor.Bind(":owner", m_schema);
        cursor.Execute();
        while (cursor.Fetch()) {

            ClusterEntry entry;

            entry.owner           = cursor.ToString(cn_owner          );
            entry.cluster_name    = cursor.ToString(cn_cluster_name   );
            entry.cluster_type    = cursor.ToString(cn_cluster_type   );
            entry.function        = cursor.ToString(cn_function       );
            entry.hashkeys        = cursor.ToInt   (cn_hashkeys       );
            entry.key_size        = cursor.ToInt   (cn_key_size       );
            entry.tablespace_name = cursor.ToString(cn_tablespace_name);
            entry.initial_extent  = cursor.ToInt   (cn_initial_extent );
            entry.next_extent     = cursor.ToInt   (cn_next_extent    );
            entry.pct_increase    = cursor.ToInt   (cn_pct_increase   );
            entry.max_extents     = cursor.ToInt   (cn_max_extents    );
            entry.pct_free        = cursor.ToInt   (cn_pct_free       );
            entry.pct_used        = cursor.ToInt   (cn_pct_used       );

            m_entries.push_back(entry);
        }

        return m_entries.size();
	}
};

