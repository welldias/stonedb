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
        enum class ColumnName {
            Synonym = 0,
            Owner,
            Object,
            DBLink,
            Status,
            Count,
        };

        SynonymListAdapter(Oci20::Connect& connect);

        const SynonymEntry& Data(int row) const { return m_entries.at(row); }

        virtual int GetRowCount() const { return (int)m_entries.size(); }

        virtual int GetColCount() const { return static_cast<int>(ColumnName::Count); }

        virtual ListDataProvider::ColumnType GetColumnType(int col) const {

            switch (static_cast<ColumnName>(col)) {
            case ColumnName::Synonym: return ColumnType::String;
            case ColumnName::Owner:   return ColumnType::String;
            case ColumnName::Object:  return ColumnType::String;
            case ColumnName::DBLink:  return ColumnType::String;
            case ColumnName::Status:  return ColumnType::String;
            }

            return ColumnType::String;
        }

        virtual void GetColHeader(int col, std::string& value) const {

            value = "Unknown";

            switch (static_cast<ColumnName>(col)) {
            case ColumnName::Synonym: value = "Synonym";
            case ColumnName::Owner:   value = "Owner";
            case ColumnName::Object:  value = "Object";
            case ColumnName::DBLink:  value = "DB Link";
            case ColumnName::Status:  value = "Status";
            }
        }

        virtual void GetString(int row, int col, std::string& value) const {

            value = "Unknown";

            switch (static_cast<ColumnName>(col)) {
            case ColumnName::Synonym: ToString(Data(row).synonym_name, value);
            case ColumnName::Owner:   ToString(Data(row).table_owner, value);
            case ColumnName::Object:  ToString(Data(row).table_name, value);
            case ColumnName::DBLink:  ToString(Data(row).db_link, value);
            case ColumnName::Status:  ToString(Data(row).status, value);
            }
        }

        bool IsVisibleRow(int row) const {
            return !Data(row).deleted;
        }

        virtual int Compare(int row1, int row2, int col) const {
            switch (static_cast<ColumnName>(col)) {
            case ColumnName::Synonym: return Comp(Data(row1).synonym_name, Data(row2).synonym_name);
            case ColumnName::Owner:   return Comp(Data(row1).table_owner, Data(row2).table_owner);
            case ColumnName::Object:  return Comp(Data(row1).table_name, Data(row2).table_name);
            case ColumnName::DBLink:  return Comp(Data(row1).db_link, Data(row2).db_link);
            case ColumnName::Status:  return Comp(Data(row1).status, Data(row2).status);
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

