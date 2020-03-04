#ifndef __PROJECT_STONE_DATA_TABLELISTADAPTER_H__
#define __PROJECT_STONE_DATA_TABLELISTADAPTER_H__

#include <string>
#include <vector>
#include <cstdint>

#include "list_data_provider.h"
#include "list_data_provider_helper.h"

#include "../oci20/oci20.h"

namespace Data {

    struct TableEntry {
        std::string owner; //HIDDEN
        std::string table_name;
        std::string table_type;
        std::string part_key;
        std::string compression;
        std::string tablespace_name;
        tm created; //"created" "Created"
        tm last_ddl_time; //"last_ddl_time" "Modified"
        tm last_analyzed;
        int64_t num_rows;
        int64_t blocks;

        TableEntry();
        bool deleted;
    };

    class TableListAdapter : public ListDataProvider, ListDataProviderHelper {
	protected:
        std::vector<TableEntry> m_entries;
        Oci20::Connect& m_connect;

    public:
        TableListAdapter(Oci20::Connect& connect);

        const TableEntry& Data(int row) const { return m_entries.at(row); }

        virtual int GetRowCount() const { return (int)m_entries.size(); }

        virtual int GetColCount() const { return 9; }

        virtual ListDataProvider::ColumnType GetColumnType(int col) const {
            
            switch (col) {
            case 0: return ColumnType::String;  
            case 1: return ColumnType::String;  
            case 2: return ColumnType::String;     
            case 3: return ColumnType::Number;  
            case 4: return ColumnType::Date;
            case 5: return ColumnType::Date;
            case 6: return ColumnType::Date;
            case 7: return ColumnType::Number;
            case 8: return ColumnType::Number;
            }

            return ColumnType::String;
        }

        virtual void GetColHeader(int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case 0: value = "Name";          return;
            case 1: value = "Type";          return;
            case 2: value = "Compression";   return;
            case 3: value = "Tablespace";    return;
            case 4: value = "Created";       return;
            case 5: value = "Modified";      return;
            case 6: value = "Last Analyzed"; return;
            case 7: value = "Rows";          return;
            case 8: value = "Blocks";        return;
            }
        }

        virtual void GetString(int row, int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case 0: ToString(Data(row).table_name,      value);  return;
            case 1: ToString(Data(row).table_type,      value);  return;
            case 2: ToString(Data(row).compression,     value);  return;
            case 3: ToString(Data(row).tablespace_name, value);  return;
            case 4: ToString(Data(row).created,         value);  return;
            case 5: ToString(Data(row).last_ddl_time,   value);  return;
            case 6: ToString(Data(row).last_analyzed,   value);  return;
            case 7: ToString(Data(row).num_rows,        value);  return;
            case 8: ToString(Data(row).blocks,          value);  return;
            }
        }                                                       

        bool IsVisibleRow(int row) const {
            return !Data(row).deleted;
        }

        virtual int Compare(int row1, int row2, int col) const {
            switch (col) {
            case 0: return Comp(Data(row1).table_name,      Data(row2).table_name);
            case 1: return Comp(Data(row1).table_type,      Data(row2).table_type);
            case 2: return Comp(Data(row1).compression,     Data(row2).compression);
            case 3: return Comp(Data(row1).tablespace_name, Data(row2).tablespace_name);
            case 4: return Comp(Data(row1).created,         Data(row2).created);
            case 5: return Comp(Data(row1).last_ddl_time,   Data(row2).last_ddl_time);
            case 6: return Comp(Data(row1).last_analyzed,   Data(row2).last_analyzed);
            case 7: return Comp(Data(row1).num_rows,        Data(row2).num_rows);
            case 8: return Comp(Data(row1).blocks,          Data(row2).blocks);
            }
            return 0;
        }

        virtual int GetMinDefColWidth(int col) const {
            return !col ? 200 : ListDataProvider::GetMinDefColWidth(col);
        }

        virtual size_t Query();

        virtual InfoType GetInfoType() const { return InfoType::Table; }
    };
}
#endif /*__PROJECT_STONE_DATA_TABLELISTADAPTER_H__*/

