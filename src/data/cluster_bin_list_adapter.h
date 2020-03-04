#ifndef __PROJECT_STONE_DATA_CLUSTERLISTADAPTER_H__
#define __PROJECT_STONE_DATA_CLUSTERLISTADAPTER_H__

#include <string>
#include <vector>
#include <cstdint>

#include "list_data_provider.h"
#include "list_data_provider_helper.h"

#include "../oci20/oci20.h"

namespace Data {

    struct ClusterEntry {
        std::string owner          ;  //HIDDEN
        std::string cluster_name   ;  //"Name"
        std::string cluster_type   ;  //"Type"
        std::string function       ;  //"Function"
        int         hashkeys       ;  //"Hashkeys"
        int         key_size       ;  //"Key size"
        std::string tablespace_name;  //"Tablespace"
        int         initial_extent ;  //"Initial"
        int         next_extent    ;  //"Next"
        int         pct_increase   ;  //"Increase"
        int         max_extents    ;  //"Max Ext"
        int         pct_free       ;  //"Pct Free"
        int         pct_used       ;  //"Pct Used"

        ClusterEntry();
        bool deleted;
    };

    class ClusterListAdapter : public ListDataProvider, ListDataProviderHelper {
	protected:
        std::vector<ClusterEntry> m_entries;
        Oci20::Connect& m_connect;

    public:
        ClusterListAdapter(Oci20::Connect& connect);

        const ClusterEntry& Data(int row) const { return m_entries.at(row); }

        virtual int GetRowCount() const { return (int)m_entries.size(); }

        virtual int GetColCount() const { return 12; }

        virtual ListDataProvider::ColumnType GetColumnType(int col) const {
            
            switch (col) {
            case  0: return ColumnType::String; // string cluster_name   
            case  1: return ColumnType::String; // string cluster_type   
            case  2: return ColumnType::String; // string function       
            case  3: return ColumnType::Number; // int    hashkeys       
            case  4: return ColumnType::Number; // int    key_size       
            case  5: return ColumnType::String; // string tablespace_name
            case  6: return ColumnType::Number; // int    initial_extent 
            case  7: return ColumnType::Number; // int    next_extent    
            case  8: return ColumnType::Number; // int    pct_increase   
            case  9: return ColumnType::Number; // int    max_extents    
            case 10: return ColumnType::Number; // int    pct_free       
            case 11: return ColumnType::Number; // int    pct_used       
            }

            return ColumnType::String;
        }

        virtual void GetColHeader(int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case  0: value = "Name";       return;
            case  1: value = "Type";       return;
            case  2: value = "Function";   return;
            case  3: value = "Hashkeys";   return;
            case  4: value = "Key size";   return;
            case  5: value = "Tablespace"; return;
            case  6: value = "Initial";    return;
            case  7: value = "Next";       return;
            case  8: value = "Increase";   return;
            case  9: value = "Max Ext";    return;
            case 10: value = "Pct Free";   return;
            case 11: value = "Pct Used";   return;
            }
        }

        virtual void GetString(int row, int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case  0: ToString(Data(row).cluster_name    ,value); return;
            case  1: ToString(Data(row).cluster_type    ,value); return;
            case  2: ToString(Data(row).function        ,value); return;
            case  3: ToString(Data(row).hashkeys        ,value); return;
            case  4: ToString(Data(row).key_size        ,value); return;
            case  5: ToString(Data(row).tablespace_name ,value); return;
            case  6: ToString(Data(row).initial_extent  ,value); return;
            case  7: ToString(Data(row).next_extent     ,value); return;
            case  8: ToString(Data(row).pct_increase    ,value); return;
            case  9: ToString(Data(row).max_extents     ,value); return;
            case 10: ToString(Data(row).pct_free        ,value); return;
            case 11: ToString(Data(row).pct_used        ,value); return;
            }
        }

        bool IsVisibleRow(int row) const {
            return !Data(row).deleted;
        }

        virtual int Compare(int row1, int row2, int col) const {
            switch (col) {
            case  0: return Comp(Data(row1).cluster_name   ,Data(row2).cluster_name   );
            case  1: return Comp(Data(row1).cluster_type   ,Data(row2).cluster_type   );
            case  2: return Comp(Data(row1).function       ,Data(row2).function       );
            case  3: return Comp(Data(row1).hashkeys       ,Data(row2).hashkeys       );
            case  4: return Comp(Data(row1).key_size       ,Data(row2).key_size       );
            case  5: return Comp(Data(row1).tablespace_name,Data(row2).tablespace_name);
            case  6: return Comp(Data(row1).initial_extent ,Data(row2).initial_extent );
            case  7: return Comp(Data(row1).next_extent    ,Data(row2).next_extent    );
            case  8: return Comp(Data(row1).pct_increase   ,Data(row2).pct_increase   );
            case  9: return Comp(Data(row1).max_extents    ,Data(row2).max_extents    );
            case 10: return Comp(Data(row1).pct_free       ,Data(row2).pct_free       );
            case 11: return Comp(Data(row1).pct_used       ,Data(row2).pct_used       );
            }
            return 0;
        }

        virtual int GetMinDefColWidth(int col) const {
            return !col ? 200 : ListDataProvider::GetMinDefColWidth(col);
        }

        virtual size_t Query();

        virtual InfoType GetInfoType() const { return InfoType::Cluster; }
    };
}
#endif /*__PROJECT_STONE_DATA_CLUSTERLISTADAPTER_H__*/

