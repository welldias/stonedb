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
    public:
        enum class Type : char {
            PrimaryKey = 'P',
            ForeignKey = 'R',
            UniqueKey = 'U',
            Check = 'C',
        };
    
    protected:
        std::vector<ConstraintEntry> m_entries;
        Oci20::Connect& m_connect;
        Type m_type;

    public:

        ConstraintListAdapter(Oci20::Connect& connect, Type type);

        const ConstraintEntry& Data(int row) const { return m_entries.at(row); }

        virtual int GetRowCount() const { return (int)m_entries.size(); }

        virtual int GetColCount() const { return 6; }

        virtual ListDataProvider::ColumnType GetColumnType(int col) const {
            
            switch (col) {
            case 0: return ColumnType::String;
            case 1: return ColumnType::String;
            case 2: return ColumnType::String;
            case 3: return ColumnType::String;
            case 4: return ColumnType::String;
            case 5: return ColumnType::String;
            }

            return ListDataProvider::ColumnType::String;
        }

        virtual void GetColHeader(int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case 0: value = "Constraint";  return;
            case 1: value = "Table";       return;
            case 2: value = "Columns";     return;
            case 3: value = "Deferrable";  return;
            case 4: value = "Deferred";    return;
            case 5: value = "Status";      return;
            }                
        }

        virtual void GetString(int row, int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case 0: ToString(Data(row).constraint_name, value);  return;
            case 1: ToString(Data(row).table_name,      value);  return;
            case 2: ToString(Data(row).column_list,     value);  return;
            case 3: ToString(Data(row).deferrable,      value);  return;
            case 4: ToString(Data(row).deferred,        value);  return;
            case 5: ToString(Data(row).status,          value);  return;
            }
        }

        bool IsVisibleRow(int row) const {
            return !Data(row).deleted;
        }

        virtual int Compare(int row1, int row2, int col) const {
            switch (col) {
            case 0: return Comp(Data(row1).constraint_name ,Data(row2).constraint_name); 
            case 1: return Comp(Data(row1).table_name      ,Data(row2).table_name);      
            case 2: return Comp(Data(row1).column_list     ,Data(row2).column_list);     
            case 3: return Comp(Data(row1).deferrable      ,Data(row2).deferrable);      
            case 4: return Comp(Data(row1).deferred        ,Data(row2).deferred);        
            case 5: return Comp(Data(row1).status          ,Data(row2).status );         
            }
            return 0;
        }

        virtual int GetMinDefColWidth(int col) const {
            return !col ? 200 : ListDataProvider::GetMinDefColWidth(col);
        }

        virtual size_t Query();

        virtual InfoType GetInfoType() const { 
            switch (m_type) {
            case Type::PrimaryKey: return InfoType::PkConstraint;
            case Type::ForeignKey: return InfoType::FkConstraint;
            case Type::UniqueKey:  return InfoType::UkConstraint;
            case Type::Check:      return InfoType::ChkConstraint;
            }
            return InfoType::Undefined;
        }
    };
}
#endif /*__PROJECT_STONE_DATA_CONSTRAINTLISTADAPTER_H__*/

