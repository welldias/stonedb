#ifndef __PROJECT_STONE_DATA_SYNONYMLISTADAPTER_H__
#define __PROJECT_STONE_DATA_SYNONYMLISTADAPTER_H__

#include <string>
#include <vector>
#include <cstdint>

#include "list_data_provider.h"
#include "list_data_provider_helper.h"

#include "../oci20/oci20.h"

namespace Data {

    struct SynonymEntry {
        std::string owner; // HIDDEN
        std::string synonym_name; // "Synonym"
        std::string table_owner; // "Owner", 
        std::string table_name; // "Object",
        std::string db_link; // "DB Link"
        std::string status; // "Status"

        SynonymEntry();
        bool deleted;
    };

    class SynonymListAdapter : public ListDataProvider, ListDataProviderHelper {
    protected:
        std::vector<SynonymEntry> m_entries;
        Oci20::Connect& m_connect;

    public:
        SynonymListAdapter(Oci20::Connect& connect);

        const SynonymEntry& Data(int row) const { return m_entries.at(row); }

        virtual int GetRowCount() const { return (int)m_entries.size(); }

        virtual int GetColCount() const { return 5; }

        virtual ListDataProvider::ColumnType GetColumnType(int col) const {

            switch (col) {
            case 0: return ColumnType::String;
            case 1: return ColumnType::String;
            case 2: return ColumnType::String;
            case 3: return ColumnType::String;
            case 4: return ColumnType::String;
            }

            return ColumnType::String;
        }

        virtual void GetColHeader(int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case 0: value = "Synonym";  return;
            case 1: value = "Owner";    return;
            case 2: value = "Object";   return;
            case 3: value = "DB Link";  return;
            case 4: value = "Status";   return;
            }
        }

        virtual void GetString(int row, int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case 0: ToString(Data(row).synonym_name, value); return;
            case 1: ToString(Data(row).table_owner,  value); return;
            case 2: ToString(Data(row).table_name,   value); return;
            case 3: ToString(Data(row).db_link,      value); return;
            case 4: ToString(Data(row).status,       value); return;
            }
        }

        bool IsVisibleRow(int row) const {
            return !Data(row).deleted;
        }

        virtual int Compare(int row1, int row2, int col) const {
            switch (col) {
            case 0: return Comp(Data(row1).synonym_name, Data(row2).synonym_name);
            case 1: return Comp(Data(row1).table_owner,  Data(row2).table_owner);
            case 2: return Comp(Data(row1).table_name,   Data(row2).table_name);
            case 3: return Comp(Data(row1).db_link,      Data(row2).db_link);
            case 4: return Comp(Data(row1).status,       Data(row2).status);
            }
            return 0;
        }

        virtual int GetMinDefColWidth(int col) const {
            return !col ? 200 : ListDataProvider::GetMinDefColWidth(col);
        }

        virtual size_t Query();

        virtual InfoType GetInfoType() const { return InfoType::Sequence; }
    };
}
#endif /*__PROJECT_STONE_DATA_SYNONYMLISTADAPTER_H__*/

