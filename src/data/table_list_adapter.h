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
        enum class ColumnName {
            TableName = 0,
            TableType,
            TablespaceName,
            Compression,
            Created,
            LastDdlTime,
            NumRows,
            Blocks,
            LastAnalyzed,
            Count,
        };

        TableListAdapter(Oci20::Connect& connect);

        const TableEntry& Data(int row) const { return m_entries.at(row); }

        virtual int GetRowCount() const { return (int)m_entries.size(); }

        virtual int GetColCount() const { return static_cast<int>(ColumnName::Count); }

        virtual ListDataProvider::ColumnType GetColumnType(int col) const {
            
            switch (static_cast<ColumnName>(col)) {
            case ColumnName::TableName:      return ColumnType::String; //table_name       
            case ColumnName::TableType:      return ColumnType::String; //table_type       
            case ColumnName::Compression:    return ColumnType::String; //compression         
            case ColumnName::TablespaceName: return ColumnType::Number; //tablespace_name  
            case ColumnName::Created:        return ColumnType::Date; //created     
            case ColumnName::LastDdlTime:    return ColumnType::Date; //last_ddl_time
            case ColumnName::LastAnalyzed:   return ColumnType::Date; //last_analyzed    
            case ColumnName::NumRows:        return ColumnType::Number;
            case ColumnName::Blocks:         return ColumnType::Number;
            }

            return ColumnType::String;
        }

        virtual void GetColHeader(int col, std::string& value) const {

            value = "Unknown";

            switch (static_cast<ColumnName>(col)) {
            case ColumnName::TableName:      value = "Name"; //table_name       
            case ColumnName::TableType:      value = "Type"; //table_type       
            case ColumnName::Compression:    value = "Compression"; //compression         
            case ColumnName::TablespaceName: value = "Tablespace"; //tablespace_name  
            case ColumnName::Created:        value = "Created"; //created
            case ColumnName::LastDdlTime:    value = "Modified"; //last_ddl_time
            case ColumnName::LastAnalyzed:   value = "Last Analyzed"; //last_analyzed    
            case ColumnName::NumRows:        value = "Rows";
            case ColumnName::Blocks:         value = "Blocks";
            }
        }

        virtual void GetString(int row, int col, std::string& value) const {

            value = "Unknown";

            switch (static_cast<ColumnName>(col)) {
            case ColumnName::TableName:      ToString(Data(row).table_name, value);
            case ColumnName::TableType:      ToString(Data(row).table_type, value);
            case ColumnName::Compression:    ToString(Data(row).compression, value);
            case ColumnName::TablespaceName: ToString(Data(row).tablespace_name, value);
            case ColumnName::Created:        ToString(Data(row).created, value);
            case ColumnName::LastDdlTime:    ToString(Data(row).last_ddl_time, value);
            case ColumnName::LastAnalyzed:   ToString(Data(row).last_analyzed, value);
            case ColumnName::NumRows:        ToString(Data(row).num_rows, value);
            case ColumnName::Blocks:         ToString(Data(row).blocks, value);
            }
        }

        bool IsVisibleRow(int row) const {
            return !Data(row).deleted;
        }

        virtual int Compare(int row1, int row2, int col) const {
            switch (static_cast<ColumnName>(col)) {
            case ColumnName::TableName:      return Comp(Data(row1).table_name,      Data(row2).table_name);
            case ColumnName::TableType:      return Comp(Data(row1).table_type,      Data(row2).table_type);
            case ColumnName::Compression:    return Comp(Data(row1).compression,     Data(row2).compression);
            case ColumnName::TablespaceName: return Comp(Data(row1).tablespace_name, Data(row2).tablespace_name);
            case ColumnName::Created:        return Comp(Data(row1).created,         Data(row2).created);
            case ColumnName::LastDdlTime:    return Comp(Data(row1).last_ddl_time,   Data(row2).last_ddl_time);
            case ColumnName::LastAnalyzed:   return Comp(Data(row1).last_analyzed,   Data(row2).last_analyzed);
            case ColumnName::NumRows:        return Comp(Data(row1).num_rows,        Data(row2).num_rows);
            case ColumnName::Blocks:         return Comp(Data(row1).blocks,          Data(row2).blocks);
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

