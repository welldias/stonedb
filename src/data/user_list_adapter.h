#ifndef __PROJECT_STONE_DATA_USERLISTADAPTER_H__
#define __PROJECT_STONE_DATA_USERLISTADAPTER_H__

#include <string>
#include <vector>
#include <cstdint>

#include "list_data_provider.h"
#include "list_data_provider_helper.h"

#include "../oci20/oci20.h"

namespace Data {

    class UserListAdapter : public ListDataProvider, ListDataProviderHelper {
	protected:
        std::vector<std::string> m_entries;
        Oci20::Connect& m_connect;

    public:

        UserListAdapter(Oci20::Connect& connect);

        const std::string& Data(int row) const { return m_entries.at(row); }

        virtual int GetRowCount() const { return (int)m_entries.size(); }

        virtual int GetColCount() const { return 1; }

        virtual ListDataProvider::ColumnType GetColumnType(int col) const {
            return ColumnType::String;
        }

        virtual void GetColHeader(int col, std::string& value) const {

            value = "User Name";
        }

        virtual void GetString(int row, int col, std::string& value) const {

            ToString(Data(row), value);
        }
        
        bool IsVisibleRow(int row) const {
            return true;
        }

        virtual int Compare(int row1, int row2, int col) const {
            return Comp(Data(row1), Data(row2));
        }

        virtual int GetMinDefColWidth(int col) const {
            return !col ? 200 : ListDataProvider::GetMinDefColWidth(col);
        }

        virtual size_t Query();

        virtual InfoType GetInfoType() const { return InfoType::Undefined; }
    };
}
#endif /*__PROJECT_STONE_DATA_USERLISTADAPTER_H__*/

