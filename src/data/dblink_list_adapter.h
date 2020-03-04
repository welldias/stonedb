#ifndef __PROJECT_STONE_DATA_DBLINKLISTADAPTER_H__
#define __PROJECT_STONE_DATA_DBLINKLISTADAPTER_H__

#include <string>
#include <vector>
#include <cstdint>

#include "list_data_provider.h"
#include "list_data_provider_helper.h"

#include "../oci20/oci20.h"

namespace Data {

    struct DbLinkEntry {
        std::string owner; // HIDDEN
        std::string db_link; // "Db Link" 
        std::string username; // "Username"
        std::string host; // "Host"

        DbLinkEntry();
        bool deleted;
    };

    class DbLinkListAdapter : public ListDataProvider, ListDataProviderHelper {
	protected:
        std::vector<DbLinkEntry> m_entries;
        Oci20::Connect& m_connect;

    public:
        DbLinkListAdapter(Oci20::Connect& connect);

        const DbLinkEntry& Data(int row) const { return m_entries.at(row); }

        virtual int GetRowCount() const { return (int)m_entries.size(); }

        virtual int GetColCount() const { return 3; }

        virtual ListDataProvider::ColumnType GetColumnType(int col) const {
            
            switch (col) {
            case  0: return ColumnType::String; // string db_link ; 
            case  1: return ColumnType::String; // string username; 
            case  2: return ColumnType::String; // string host    ; 
            }

            return ColumnType::String;
        }

        virtual void GetColHeader(int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case  0: value = "Db Link";  return;
            case  1: value = "Username"; return;
            case  2: value = "Host";     return;
            }
        }

        virtual void GetString(int row, int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case  0: ToString(Data(row).db_link    ,value); return;
            case  1: ToString(Data(row).username   ,value); return;
            case  2: ToString(Data(row).host       ,value); return;
            }
        }

        bool IsVisibleRow(int row) const {
            return !Data(row).deleted;
        }

        virtual int Compare(int row1, int row2, int col) const {
            switch (col) {
            case  0: return Comp(Data(row1).db_link    ,Data(row2).db_link  );
            case  1: return Comp(Data(row1).username   ,Data(row2).username );
            case  2: return Comp(Data(row1).host       ,Data(row2).host     );
            }
            return 0;
        }

        virtual int GetMinDefColWidth(int col) const {
            return !col ? 200 : ListDataProvider::GetMinDefColWidth(col);
        }

        virtual size_t Query();

        virtual InfoType GetInfoType() const { return InfoType::Dblink; }
    };
}
#endif /*__PROJECT_STONE_DATA_DBLINKLISTADAPTER_H__*/

