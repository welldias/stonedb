#ifndef __PROJECT_STONE_DATA_VIEWLISTADAPTER_H__
#define __PROJECT_STONE_DATA_VIEWLISTADAPTER_H__

#include <string>
#include <vector>

#include "list_data_provider.h"
#include "list_data_provider_helper.h"

#include "../oci20/oci20.h"

namespace Data {

    struct ViewEntry {
        std::string owner;         // HIDDEN
        std::string view_name;     // "Name"
        int         text_length;   // "Text Length"
        tm          created;       // "Created"
        tm          last_ddl_time; // "Modified"
        std::string status;        // "Status"

        ViewEntry();
        bool deleted;
    };

    class ViewListAdapter : public ListDataProvider, ListDataProviderHelper {
	protected:
        std::vector<ViewEntry> m_entries;
        Oci20::Connect& m_connect;

    public:
        enum class ColumnName {
            ViewName = 0,
            TextLength,
            Created,
            LastDdlTime,
            Status,
            Count,
        };

        ViewListAdapter(Oci20::Connect& connect);

        const ViewEntry& Data(int row) const { return m_entries.at(row); }

        virtual int GetRowCount() const { return (int)m_entries.size(); }

        virtual int GetColCount() const { return static_cast<int>(ColumnName::Count); }

        virtual ListDataProvider::ColumnType GetColumnType(int col) const {
            
            switch (static_cast<ColumnName>(col)) {
            case ColumnName::ViewName:    return ColumnType::String;
            case ColumnName::TextLength:  return ColumnType::Number;
            case ColumnName::Created:     return ColumnType::Date;
            case ColumnName::LastDdlTime: return ColumnType::Date;
            case ColumnName::Status:      return ColumnType::String;
            }

            return ListDataProvider::ColumnType::String;
        }

        virtual void GetColHeader(int col, std::string& value) const {

            value = "Unknown";

            switch (static_cast<ColumnName>(col)) {
            case ColumnName::ViewName:    value = "Name";
            case ColumnName::TextLength:  value = "Text Length";
            case ColumnName::Created:     value = "Created";
            case ColumnName::LastDdlTime: value = "Modified";
            case ColumnName::Status:      value = "Status";
            }
        }

        virtual void GetString(int row, int col, std::string& value) const {

            value = "Unknown";

            switch (static_cast<ColumnName>(col)) {
            case ColumnName::ViewName:    ToString(Data(row).view_name, value);
            case ColumnName::TextLength:  ToString(Data(row).text_length, value);
            case ColumnName::Created:     ToString(Data(row).created, value);
            case ColumnName::LastDdlTime: ToString(Data(row).last_ddl_time, value);
            case ColumnName::Status:      ToString(Data(row).status, value);
            }
        }

        bool IsVisibleRow(int row) const {
            return !Data(row).deleted;
        }

        virtual int Compare(int row1, int row2, int col) const {
            switch (static_cast<ColumnName>(col)) {
            case ColumnName::ViewName:    return Comp(Data(row1).view_name,     Data(row2).view_name);
            case ColumnName::TextLength:  return Comp(Data(row1).text_length,   Data(row2).text_length);
            case ColumnName::Created:     return Comp(Data(row1).created,       Data(row2).created);
            case ColumnName::LastDdlTime: return Comp(Data(row1).last_ddl_time, Data(row2).last_ddl_time);
            case ColumnName::Status:      return Comp(Data(row1).status,        Data(row2).status);
            }
            return 0;
        }

        virtual int GetMinDefColWidth(int col) const {
            return !col ? 200 : ListDataProvider::GetMinDefColWidth(col);
        }

        size_t Query();

        virtual InfoType GetInfoType() const { return InfoType::View; }
    };
}
#endif /*__PROJECT_STONE_DATA_VIEWLISTADAPTER_H__*/

