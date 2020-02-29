#ifndef __PROJECT_STONE_DATA_CONSTRAINTLISTADAPTER_H__
#define __PROJECT_STONE_DATA_CONSTRAINTLISTADAPTER_H__

#include <string>
#include <vector>

#include "list_data_provider.h"
#include "list_data_provider_helper.h"

#include "../oci20/oci20.h"

namespace Data {

    struct ConstraintEntry {
        // HIDDEN
        std::string owner; // "owner" 
        // ALL
        std::string constraint_name; // "Constraint"
        std::string table_name; // "Table"
        std::string deferrable; // "Deferrable"
        std::string deferred; // "Deferred"
        std::string status; // "Status"
        // CHECK
        std::string search_condition; // "Condition" 
        // FK
        std::string r_owner; // "Ref Owner", 
        std::string r_constraint_name; // "Ref Constraint",
        std::string delete_rule; // "Delete Rule",
        std::string column_list;

        ConstraintEntry();
        bool deleted;
    };

    class ConstraintListAdapter : public ListDataProvider, ListDataProviderHelper {
	protected:
        std::vector<ConstraintEntry> m_entries;
        Oci20::Connect& m_connect;
        std::string m_constraintType;


    public:
        enum class ColumnName {
            Constraint = 0,
            Table,
            Columns,
            Deferrable,
            Deferred,
            Status,
            Count,
        };

        ConstraintListAdapter(Oci20::Connect& connect, const std::string& constraintType);

        const ConstraintEntry& Data(int row) const { return m_entries.at(row); }

        virtual int GetRowCount() const { return (int)m_entries.size(); }

        virtual int GetColCount() const { return static_cast<int>(ColumnName::Count); }

        virtual ListDataProvider::ColumnType GetColumnType(int col) const {
            
            switch (static_cast<ColumnName>(col)) {
            case ColumnName::Constraint: return ColumnType::String;
            case ColumnName::Table:      return ColumnType::String;
            case ColumnName::Columns:    return ColumnType::String;
            case ColumnName::Deferrable: return ColumnType::String;
            case ColumnName::Deferred:   return ColumnType::String;
            case ColumnName::Status:     return ColumnType::String;
            }

            return ListDataProvider::ColumnType::String;
        }

        virtual void GetColHeader(int col, std::string& value) const {

            value = "Unknown";

            switch (static_cast<ColumnName>(col)) {
            case ColumnName::Constraint: value = "Constraint";
            case ColumnName::Table:      value = "Table";
            case ColumnName::Columns:    value = "Columns";
            case ColumnName::Deferrable: value = "Deferrable";
            case ColumnName::Deferred:   value = "Deferred";
            case ColumnName::Status:     value = "Status";
            }                
        }

        virtual void GetString(int row, int col, std::string& value) const {

            value = "Unknown";

            switch (static_cast<ColumnName>(col)) {
            case ColumnName::Constraint: ToString(Data(row).constraint_name, value);
            case ColumnName::Table:      ToString(Data(row).table_name, value);
            case ColumnName::Columns:    ToString(Data(row).column_list, value);
            case ColumnName::Deferrable: ToString(Data(row).deferrable, value);
            case ColumnName::Deferred:   ToString(Data(row).deferred, value);
            case ColumnName::Status:     ToString(Data(row).status, value);
            }
        }

        bool IsVisibleRow(int row) const {
            return !Data(row).deleted;
        }

        virtual int Compare(int row1, int row2, int col) const {
            switch (static_cast<ColumnName>(col)) {
            case ColumnName::Constraint: Comp(Data(row1).constraint_name, Data(row2).constraint_name);
            case ColumnName::Table:      Comp(Data(row1).table_name     , Data(row2).table_name     );
            case ColumnName::Columns:    Comp(Data(row1).column_list    , Data(row2).column_list    );
            case ColumnName::Deferrable: Comp(Data(row1).deferrable     , Data(row2).deferrable     );
            case ColumnName::Deferred:   Comp(Data(row1).deferred       , Data(row2).deferred       );
            case ColumnName::Status:     Comp(Data(row1).status         , Data(row2).status         );
            }
            return 0;
        }

        virtual int GetMinDefColWidth(int col) const {
            return !col ? 200 : ListDataProvider::GetMinDefColWidth(col);
        }

        size_t Query();

        virtual InfoType GetInfoType() const { return InfoType::Undefined; }
    };
}
#endif /*__PROJECT_STONE_DATA_CONSTRAINTLISTADAPTER_H__*/

