#ifndef __PROJECT_STONE_DATA_USERLISTADAPTER_H__
#define __PROJECT_STONE_DATA_USERLISTADAPTER_H__

#include <string>
#include <vector>
#include <cstdint>

#include "list_data_provider.h"
#include "list_data_provider_helper.h"

#include "../oci20/oci20.h"

namespace Data {

    struct UserEntry {
        std::string user;

        UserEntry() : deleted(false) {}
        bool deleted;
    };

    class UserListAdapter : public ListDataProvider, ListDataProviderHelper {
	protected:
        std::vector<UserEntry> m_entries;
        Oci20::Connect& m_connect;

    public:

        UserListAdapter(Oci20::Connect& connect);

        const UserEntry& Data(int row) const { return m_entries.at(row); }

        virtual int GetRowCount() const { return (int)m_entries.size(); }

        virtual int GetColCount() const { return 1; }

        virtual ListDataProvider::ColumnType GetColumnType(int col) const {

            switch (col) {
            case  0: return ColumnType::String;
            }
            return ColumnType::String;
        }

        virtual void GetColHeader(int col, std::string& value) const {
            value = "Unknown";

            switch (col) {
            case  0: value = "User Name"; return;
            }
        }

        virtual void GetString(int row, int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case 0: ToString(Data(row).user, value); return;
            }
        }
        
        bool IsVisibleRow(int row) const {
            return !Data(row).deleted;
        }

        virtual int Compare(int row1, int row2, int col) const {
            switch (col) {
            case  0: return Comp(Data(row1).user, Data(row2).user);
            }
            return 0;
        }

        virtual int GetMinDefColWidth(int col) const {
            return !col ? 200 : ListDataProvider::GetMinDefColWidth(col);
        }

        virtual size_t Query();

        virtual InfoType GetInfoType() const { return InfoType::Undefined; }
    };
}
#endif /*__PROJECT_STONE_DATA_USERLISTADAPTER_H__*/

