#ifndef __PROJECT_STONE_DATA_INVALIDOBJECTLISTADAPTER_H__
#define __PROJECT_STONE_DATA_INVALIDOBJECTLISTADAPTER_H__

#include <string>
#include <vector>
#include <cstdint>

#include "list_data_provider.h"
#include "list_data_provider_helper.h"

#include "../oci20/oci20.h"

namespace Data {

    struct InvalidObjectEntry {
        std::string owner        ;
        std::string object_name  ;
        std::string object_type  ;
        tm          created      ;
        tm          last_ddl_time;
        std::string status       ;
        
        ListDataProvider::InfoType info_type;

        InvalidObjectEntry();
        bool deleted;
    };

    class InvalidObjectListAdapter : public ListDataProvider, ListDataProviderHelper {
	protected:
        std::vector<InvalidObjectEntry> m_entries;
        Oci20::Connect& m_connect;

    public:
        
        InvalidObjectListAdapter(Oci20::Connect& connect);

        const InvalidObjectEntry& Data(int row) const { return m_entries.at(row); }

        virtual int GetRowCount() const { return (int)m_entries.size(); }

        virtual int GetColCount() const { return 5; }

        virtual ListDataProvider::ColumnType GetColumnType(int col) const {
            
            switch (col) {
            case 0: return ColumnType::String;
            case 1: return ColumnType::String;
            case 2: return ColumnType::Date;
            case 3: return ColumnType::Date;
            case 4: return ColumnType::String;
            }

            return ColumnType::String;
        }

        virtual void GetColHeader(int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case 0: value = "Name";     return;
            case 1: value = "Type";     return;
            case 2: value = "Created";  return;
            case 3: value = "Modifies"; return;
            case 4: value = "Status";   return;
            }
        }

        virtual void GetString(int row, int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case 0: ToString(Data(row).object_name  ,value); return;
            case 1: ToString(Data(row).object_type  ,value); return;
            case 2: ToString(Data(row).created      ,value); return;
            case 3: ToString(Data(row).last_ddl_time,value); return;
            case 4: ToString(Data(row).status       ,value); return;
            }
        }

        bool IsVisibleRow(int row) const {
            return !Data(row).deleted;
        }

        virtual int Compare(int row1, int row2, int col) const {
            switch (col) {
            case 0: return Comp(Data(row1).object_name  ,Data(row2).object_name  );
            case 1: return Comp(Data(row1).object_type  ,Data(row2).object_type  );
            case 2: return Comp(Data(row1).created      ,Data(row2).created      );
            case 3: return Comp(Data(row1).last_ddl_time,Data(row2).last_ddl_time);
            case 4: return Comp(Data(row1).status       ,Data(row2).status       );
            }
            return 0;
        }

        virtual int GetMinDefColWidth(int col) const {
            return !col ? 200 : ListDataProvider::GetMinDefColWidth(col);
        }

        virtual size_t Query();

        virtual InfoType GetInfoType() const { return InfoType::InvalidObject; }
    };
}
#endif /*__PROJECT_STONE_DATA_INVALIDOBJECTLISTADAPTER_H__*/

