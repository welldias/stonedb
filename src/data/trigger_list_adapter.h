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
        TriggerListAdapter(Oci20::Connect& connect);

        const TriggerEntry& Data(int row) const { return m_entries.at(row); }

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
            case 6: return ColumnType::Date;
            case 7: return ColumnType::Date;
            case 8: return ColumnType::String;
            case 9: return ColumnType::String;
            }

            return ColumnType::String;
        }

        virtual void GetColHeader(int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case 0: value = "Trigger Name"; return;
            case 1: value = "Table Owner";  return;
            case 2: value = "Table";        return;
            case 3: value = "Type";         return;
            case 4: value = "Event";        return;
            case 5: value = "When";         return;
            case 6: value = "Created";      return;
            case 7: value = "Modified";     return;
            case 8: value = "Enabled";      return;
            case 9: value = "Status";       return;
            }
        }

        virtual void GetString(int row, int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case 0: ToString(Data(row).trigger_name      ,value); return;
            case 1: ToString(Data(row).table_owner       ,value); return;
            case 2: ToString(Data(row).table_name        ,value); return;
            case 3: ToString(Data(row).trigger_type      ,value); return;
            case 4: ToString(Data(row).triggering_event  ,value); return;
            case 5: ToString(Data(row).when_clause       ,value); return;
            case 6: ToString(Data(row).created           ,value); return;
            case 7: ToString(Data(row).last_ddl_time     ,value); return;
            case 8: ToString(Data(row).enabled           ,value); return;
            case 9: ToString(Data(row).status            ,value); return;
            }
        }

        bool IsVisibleRow(int row) const {
            return !Data(row).deleted;
        }

        virtual int Compare(int row1, int row2, int col) const {
            switch (col) {
            case 0: return Comp(Data(row1).trigger_name     ,Data(row2).trigger_name);
            case 1: return Comp(Data(row1).table_owner      ,Data(row2).table_owner);
            case 2: return Comp(Data(row1).table_name       ,Data(row2).table_name);
            case 3: return Comp(Data(row1).trigger_type     ,Data(row2).trigger_type);
            case 4: return Comp(Data(row1).triggering_event ,Data(row2).triggering_event);
            case 5: return Comp(Data(row1).when_clause      ,Data(row2).when_clause);
            case 6: return Comp(Data(row1).created          ,Data(row2).created);
            case 7: return Comp(Data(row1).last_ddl_time    ,Data(row2).last_ddl_time);
            case 8: return Comp(Data(row1).enabled          ,Data(row2).enabled);
            case 9: return Comp(Data(row1).status           ,Data(row2).status);
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

