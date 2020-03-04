#ifndef __PROJECT_STONE_DATA_GRANTEELISTADAPTER_H__
#define __PROJECT_STONE_DATA_GRANTEELISTADAPTER_H__

#include <string>
#include <vector>
#include <cstdint>

#include "list_data_provider.h"
#include "list_data_provider_helper.h"

#include "../oci20/oci20.h"

namespace Data {

    struct GranteeEntry
    {
        std::string grantee;

        GranteeEntry() : deleted(false) {}
        bool deleted;
    };

    class GranteeListAdapter : public ListDataProvider, ListDataProviderHelper {
    protected:
        std::vector<GranteeEntry> m_entries;
        Oci20::Connect& m_connect;

    public:

        GranteeListAdapter(Oci20::Connect& connect);

        const GranteeEntry& Data(int row) const { return m_entries.at(row); }

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
            case  0: value = "Grantee"; return;
            }
        }

        virtual void GetString(int row, int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case 0: ToString(Data(row).grantee, value); return;
            }
        }

        bool IsVisibleRow(int row) const {
            return !Data(row).deleted;
        }

        virtual int Compare(int row1, int row2, int col) const {
            switch (col) {
            case  0: return Comp(Data(row1).grantee, Data(row2).grantee);
            }
            return 0;
        }

        virtual int GetMinDefColWidth(int col) const {
            return !col ? 200 : ListDataProvider::GetMinDefColWidth(col);
        }

        virtual size_t Query();

        virtual InfoType GetInfoType() const { return InfoType::Grantee; }
    };
}
#endif /*__PROJECT_STONE_DATA_GRANTEELISTADAPTER_H__*/

