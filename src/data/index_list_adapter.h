#ifndef __PROJECT_STONE_DATA_INDEXLISTADAPTER_H__
#define __PROJECT_STONE_DATA_INDEXLISTADAPTER_H__

#include <string>
#include <vector>
#include <cstdint>

#include "list_data_provider.h"
#include "list_data_provider_helper.h"

#include "../oci20/oci20.h"

namespace Data {

    struct IndexEntry {
        std::string table_owner;     //HIDDEN
        std::string owner;           //HIDDEN
        std::string index_name;      //"Index"
        std::string table_name;      //"Table"
        std::string index_type;      //"Type"
        std::string uniqueness;      //"Unique"
        std::string tablespace_name; //"Tablespace"
        std::string partitioned;
        tm  created;
        tm  last_ddl_time;
        tm  last_analyzed;
        std::string column_list;

        IndexEntry();
        bool deleted;
    };

    class IndexListAdapter : public ListDataProvider, ListDataProviderHelper {
	protected:
        std::vector<IndexEntry> m_entries;
        Oci20::Connect& m_connect;

    public:
        IndexListAdapter(Oci20::Connect& connect);

        const IndexEntry& Data(int row) const { return m_entries.at(row); }

        virtual int GetRowCount() const { return (int)m_entries.size(); }

        virtual int GetColCount() const { return 10; }

        virtual ListDataProvider::ColumnType GetColumnType(int col) const {
            
            switch (col) {
            case  0: return ColumnType::String;
            case  1: return ColumnType::String;
            case  2: return ColumnType::String;
            case  3: return ColumnType::String;
            case  4: return ColumnType::String;
            case  5: return ColumnType::String;
            case  6: return ColumnType::Date;
            case  7: return ColumnType::Date;
            case  8: return ColumnType::Date;
            case  9: return ColumnType::String;
            }

            return ColumnType::String;
        }

        virtual void GetColHeader(int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case  0: value = "Index";         return;
            case  1: value = "Table";         return;
            case  2: value = "Type";          return;
            case  3: value = "Unique";        return;
            case  4: value = "Tablespace";    return;
            case  5: value = "Partitioned";   return;
            case  6: value = "Created";       return;
            case  7: value = "Modified";      return;
            case  8: value = "Last analyzed"; return;
            case  9: value = "Columns";       return;
            }
        }

        virtual void GetString(int row, int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case  0: ToString(Data(row).index_name       ,value); return;
            case  1: ToString(Data(row).table_name       ,value); return;
            case  2: ToString(Data(row).index_type       ,value); return;
            case  3: ToString(Data(row).uniqueness       ,value); return;
            case  4: ToString(Data(row).tablespace_name  ,value); return;
            case  5: ToString(Data(row).partitioned      ,value); return;
            case  6: ToString(Data(row).created          ,value); return;
            case  7: ToString(Data(row).last_ddl_time    ,value); return;
            case  8: ToString(Data(row).last_analyzed    ,value); return;
            case  9: ToString(Data(row).column_list      ,value); return;
            }
        }

        bool IsVisibleRow(int row) const {
            return !Data(row).deleted;
        }

        virtual int Compare(int row1, int row2, int col) const {
            switch (col) {
            case  0: return Comp(Data(row1).index_name,       Data(row2).index_name);
            case  1: return Comp(Data(row1).table_name,       Data(row2).table_name);
            case  2: return Comp(Data(row1).index_type,       Data(row2).index_type);
            case  3: return Comp(Data(row1).uniqueness,       Data(row2).uniqueness);
            case  4: return Comp(Data(row1).tablespace_name,  Data(row2).tablespace_name);
            case  5: return Comp(Data(row1).partitioned,      Data(row2).partitioned);
            case  6: return Comp(Data(row1).created,          Data(row2).created);
            case  7: return Comp(Data(row1).last_ddl_time,    Data(row2).last_ddl_time);
            case  8: return Comp(Data(row1).last_analyzed,    Data(row2).last_analyzed);
            case  9: return Comp(Data(row1).column_list,      Data(row2).column_list);
            }
            return 0;
        }

        virtual int GetMinDefColWidth(int col) const {
            return !col ? 200 : ListDataProvider::GetMinDefColWidth(col);
        }

        virtual size_t Query();

        virtual InfoType GetInfoType() const { return InfoType::Index; }
    };
}
#endif /*__PROJECT_STONE_DATA_INDEXLISTADAPTER_H__*/

