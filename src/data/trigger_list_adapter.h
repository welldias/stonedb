#ifndef __PROJECT_STONE_DATA_TRIGGERLISTADAPTER_H__
#define __PROJECT_STONE_DATA_TRIGGERLISTADAPTER_H__

#include <string>
#include <vector>
#include <cstdint>

#include "list_data_provider.h"
#include "list_data_provider_helper.h"

#include "../oci20/oci20.h"

namespace Data {

    struct TriggerEntry {
        std::string owner            ; // HIDDEN
        std::string trigger_name     ; // "Name"      
        std::string table_owner      ; // "Table Owner"
        std::string table_name       ; // "Table"     
        std::string trigger_type     ; // "Type"      
        std::string triggering_event ; // "Event"     
        std::string when_clause      ; // "When"     
        tm created                   ; // "Created"   
        tm last_ddl_time             ; // "Modified" 
        std::string enabled          ; // "Enabled"
        std::string status           ; // "Status"

        TriggerEntry();
        bool deleted;
    };

    class TriggerListAdapter : public ListDataProvider, ListDataProviderHelper {
	protected:
        std::vector<TriggerEntry> m_entries;
        Oci20::Connect& m_connect;

    public:
        enum class ColumnName {
            TriggerName = 0,
            TableOwner,
            Table,
            Type,
            Event,
            When,
            Created,
            Modified,
            Enabled,
            Status,
            Count
        };

        TriggerListAdapter(Oci20::Connect& connect);

        const TriggerEntry& Data(int row) const { return m_entries.at(row); }

        virtual int GetRowCount() const { return (int)m_entries.size(); }

        virtual int GetColCount() const { return static_cast<int>(ColumnName::Count); }

        virtual ListDataProvider::ColumnType GetColumnType(int col) const {
            
            switch (static_cast<ColumnName>(col)) {
            case ColumnName::TriggerName: return ColumnType::String;
            case ColumnName::TableOwner:  return ColumnType::String;
            case ColumnName::Table:       return ColumnType::String;
            case ColumnName::Type:        return ColumnType::String;
            case ColumnName::Event:       return ColumnType::String;
            case ColumnName::When:        return ColumnType::String;
            case ColumnName::Created:     return ColumnType::Date;
            case ColumnName::Modified:    return ColumnType::Date;
            case ColumnName::Enabled:     return ColumnType::String;
            case ColumnName::Status:      return ColumnType::String;
            }

            return ColumnType::String;
        }

        virtual void GetColHeader(int col, std::string& value) const {

            value = "Unknown";

            switch (static_cast<ColumnName>(col)) {
            case ColumnName::TriggerName: value = "Trigger Name";
            case ColumnName::TableOwner:  value = "Table Owner";
            case ColumnName::Table:       value = "Table";
            case ColumnName::Type:        value = "Type";
            case ColumnName::Event:       value = "Event";
            case ColumnName::When:        value = "When";
            case ColumnName::Created:     value = "Created";
            case ColumnName::Modified:    value = "Modified";
            case ColumnName::Enabled:     value = "Enabled";
            case ColumnName::Status:      value = "Status";
            }
        }

        virtual void GetString(int row, int col, std::string& value) const {

            value = "Unknown";

            switch (static_cast<ColumnName>(col)) {
            case ColumnName::TriggerName: ToString(Data(row).trigger_name      ,value);
            case ColumnName::TableOwner:  ToString(Data(row).table_owner       ,value);
            case ColumnName::Table:       ToString(Data(row).table_name        ,value);
            case ColumnName::Type:        ToString(Data(row).trigger_type      ,value);
            case ColumnName::Event:       ToString(Data(row).triggering_event  ,value);
            case ColumnName::When:        ToString(Data(row).when_clause       ,value);
            case ColumnName::Created:     ToString(Data(row).created           ,value);
            case ColumnName::Modified:    ToString(Data(row).last_ddl_time     ,value);
            case ColumnName::Enabled:     ToString(Data(row).enabled           ,value);
            case ColumnName::Status:      ToString(Data(row).status            ,value);
            }
        }

        bool IsVisibleRow(int row) const {
            return !Data(row).deleted;
        }

        virtual int Compare(int row1, int row2, int col) const {
            switch (static_cast<ColumnName>(col)) {
            case ColumnName::TriggerName: return Comp(Data(row1).trigger_name     ,Data(row2).trigger_name);
            case ColumnName::TableOwner:  return Comp(Data(row1).table_owner      ,Data(row2).table_owner);
            case ColumnName::Table:       return Comp(Data(row1).table_name       ,Data(row2).table_name);
            case ColumnName::Type:        return Comp(Data(row1).trigger_type     ,Data(row2).trigger_type);
            case ColumnName::Event:       return Comp(Data(row1).triggering_event ,Data(row2).triggering_event);
            case ColumnName::When:        return Comp(Data(row1).when_clause      ,Data(row2).when_clause);
            case ColumnName::Created:     return Comp(Data(row1).created          ,Data(row2).created);
            case ColumnName::Modified:    return Comp(Data(row1).last_ddl_time    ,Data(row2).last_ddl_time);
            case ColumnName::Enabled:     return Comp(Data(row1).enabled          ,Data(row2).enabled);
            case ColumnName::Status:      return Comp(Data(row1).status           ,Data(row2).status);
            }
            return 0;
        }

        virtual int GetMinDefColWidth(int col) const {
            return !col ? 200 : ListDataProvider::GetMinDefColWidth(col);
        }

        virtual size_t Query();

        virtual InfoType GetInfoType() const { return InfoType::Trigger; }
    };
}
#endif /*__PROJECT_STONE_DATA_TRIGGERLISTADAPTER_H__*/

