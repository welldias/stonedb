#ifndef __PROJECT_STONE_DATA_RECYCLEBINLISTADAPTER_H__
#define __PROJECT_STONE_DATA_RECYCLEBINLISTADAPTER_H__

#include <string>
#include <vector>
#include <cstdint>

#include "list_data_provider.h"
#include "list_data_provider_helper.h"

#include "../oci20/oci20.h"

namespace Data {

    struct RecyclebinEntry {
        std::string  original_name ; // "Name"
        std::string  operation     ; // "Oper"
        std::string  type          ; // "Type"
        std::string  partition_name; // "Partition"
        std::string  ts_name       ; // "Tablespace"
        std::string  createtime    ; // "Created" it is varchar2 column for some reason
        std::string  droptime      ; // "Dropped" it is varchar2 column for some reason
        std::string  can_undrop    ; // "Can Undrop"
        std::string  can_purge     ; // "Can Purge"
        int          space         ; // "Space (blk)"
        
        // HIDDEN
        std::string  object_name; 
        std::string  dropscn;
        std::string  related; 
        std::string  base_object;
        std::string  purge_object; 

        ListDataProvider::InfoType info_type;

        RecyclebinEntry();
        bool deleted;
    };

    class RecyclebinListAdapter : public ListDataProvider, ListDataProviderHelper {
	protected:
        std::vector<RecyclebinEntry> m_entries;
        Oci20::Connect& m_connect;

    public:
        RecyclebinListAdapter(Oci20::Connect& connect);

        const RecyclebinEntry& Data(int row) const { return m_entries.at(row); }

        virtual int GetRowCount() const { return (int)m_entries.size(); }

        virtual int GetColCount() const { return 10; }

        virtual ListDataProvider::ColumnType GetColumnType(int col) const {
            
            switch (col) {
            case 0: return ColumnType::String;
            case 1: return ColumnType::String;
            case 2: return ColumnType::String;
            case 3: return ColumnType::String;
            case 4: return ColumnType::String;
            case 5: return ColumnType::String;
            case 6: return ColumnType::String;
            case 7: return ColumnType::String;
            case 8: return ColumnType::String;
            case 9: return ColumnType::Number;
            }

            return ColumnType::String;
        }

        virtual void GetColHeader(int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case 0: value = "Name";        return;
            case 1: value = "Oper";        return;
            case 2: value = "Type";        return;
            case 3: value = "Partition";   return;
            case 4: value = "Tablespace";  return;
            case 5: value = "Created";     return;
            case 6: value = "Dropped";     return;
            case 7: value = "Can Undrop";  return;
            case 8: value = "Can Purge";   return;
            case 9: value = "Space (blk)"; return;
            }
        }

        virtual void GetString(int row, int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case 0: ToString(Data(row).original_name,  value); return;
            case 1: ToString(Data(row).operation,      value); return;
            case 2: ToString(Data(row).type,           value); return;
            case 3: ToString(Data(row).partition_name, value); return;
            case 4: ToString(Data(row).ts_name,        value); return;
            case 5: ToString(Data(row).createtime,     value); return;
            case 6: ToString(Data(row).droptime,       value); return;
            case 7: ToString(Data(row).can_undrop,     value); return;
            case 8: ToString(Data(row).can_purge,      value); return;
            case 9: ToString(Data(row).space,          value); return;
            }
        }

        bool IsVisibleRow(int row) const {
            return !Data(row).deleted;
        }

        virtual int Compare(int row1, int row2, int col) const {
            switch (col) {
            case 0: return Comp(Data(row1).original_name,  Data(row2).original_name);
            case 1: return Comp(Data(row1).operation,      Data(row2).operation);
            case 2: return Comp(Data(row1).type,           Data(row2).type);
            case 3: return Comp(Data(row1).partition_name, Data(row2).partition_name);
            case 4: return Comp(Data(row1).ts_name,        Data(row2).ts_name);
            case 5: return Comp(Data(row1).createtime,     Data(row2).createtime);
            case 6: return Comp(Data(row1).droptime,       Data(row2).droptime);
            case 7: return Comp(Data(row1).can_undrop,     Data(row2).can_undrop);
            case 8: return Comp(Data(row1).can_purge,      Data(row2).can_purge);
            case 9: return Comp(Data(row1).space,          Data(row2).space);
            }
            return 0;
        }

        virtual int GetMinDefColWidth(int col) const {
            return !col ? 200 : ListDataProvider::GetMinDefColWidth(col);
        }

        virtual size_t Query();

        virtual InfoType GetInfoType() const { return InfoType::Recyclebin; }
    };
}
#endif /*__PROJECT_STONE_DATA_RECYCLEBINLISTADAPTER_H__*/

