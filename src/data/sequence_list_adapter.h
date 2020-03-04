#ifndef __PROJECT_STONE_DATA_SEQUENCELISTADAPTER_H__
#define __PROJECT_STONE_DATA_SEQUENCELISTADAPTER_H__

#include <string>
#include <vector>
#include <cstdint>

#include "list_data_provider.h"
#include "list_data_provider_helper.h"

#include "../oci20/oci20.h"

namespace Data {

    struct SequenceEntry {
        std::string owner        ; // HIDDEN
        std::string sequence_name; // "Name"
        std::string last_number  ; // "Last Number"
        std::string min_value    ; // "Min Value"
        std::string max_value    ; // "Max Value"
        std::string increment_by ; // "Interval"
        std::string cache_size   ; // "Cache"
        std::string cycle_flag   ; // "Cycle"
        std::string order_flag   ; // "Order"
        std::string generated    ; // Generated
        tm created               ; // "Created"   
        tm last_ddl_time         ; // "Modified" 

        SequenceEntry();
        bool deleted;
    };

    class SequenceListAdapter : public ListDataProvider, ListDataProviderHelper {
	protected:
        std::vector<SequenceEntry> m_entries;
        Oci20::Connect& m_connect;

    public:
        SequenceListAdapter(Oci20::Connect& connect);

        const SequenceEntry& Data(int row) const { return m_entries.at(row); }

        virtual int GetRowCount() const { return (int)m_entries.size(); }

        virtual int GetColCount() const { return 11; }

        virtual ListDataProvider::ColumnType GetColumnType(int col) const {
            
            switch (col) {
            case  0:  return ColumnType::String;
            case  1:  return ColumnType::String;
            case  2:  return ColumnType::String;
            case  3:  return ColumnType::String;
            case  4:  return ColumnType::String;
            case  5:  return ColumnType::String;
            case  6:  return ColumnType::String;
            case  7:  return ColumnType::String;
            case  8:  return ColumnType::String;
            case  9:  return ColumnType::Date;
            case 10:  return ColumnType::Date;
            }

            return ColumnType::String;
        }

        virtual void GetColHeader(int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case  0: value = "Sequence Name"; return;
            case  1: value = "Last Number";   return;
            case  2: value = "Min Value";     return;
            case  3: value = "Max Value";     return;
            case  4: value = "Interval";      return;
            case  5: value = "Cache";         return;
            case  6: value = "Cycle";         return;
            case  7: value = "Order";         return;
            case  8: value = "Generated";     return;
            case  9: value = "Created";       return;
            case 10: value = "Modified";      return;
            }
        }

        virtual void GetString(int row, int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case  0: ToString(Data(row).sequence_name, value); return;
            case  1: ToString(Data(row).last_number, value);   return;
            case  2: ToString(Data(row).min_value, value);     return;
            case  3: ToString(Data(row).max_value, value);     return;
            case  4: ToString(Data(row).increment_by, value);  return;
            case  5: ToString(Data(row).cache_size, value);    return;
            case  6: ToString(Data(row).cycle_flag, value);    return;
            case  7: ToString(Data(row).order_flag, value);    return;
            case  8: ToString(Data(row).generated, value);     return;
            case  9: ToString(Data(row).created, value);       return;
            case 10: ToString(Data(row).last_ddl_time, value); return;
            }
        }

        bool IsVisibleRow(int row) const {
            return !Data(row).deleted;
        }

        virtual int Compare(int row1, int row2, int col) const {
            switch (col) {
            case  0: return Comp(Data(row1).sequence_name, Data(row2).sequence_name);
            case  1: return Comp(Data(row1).last_number,   Data(row2).last_number);
            case  2: return Comp(Data(row1).min_value,     Data(row2).min_value);
            case  3: return Comp(Data(row1).max_value,     Data(row2).max_value);
            case  4: return Comp(Data(row1).increment_by,  Data(row2).increment_by);
            case  5: return Comp(Data(row1).cache_size,    Data(row2).cache_size);
            case  6: return Comp(Data(row1).cycle_flag,    Data(row2).cycle_flag);
            case  7: return Comp(Data(row1).order_flag,    Data(row2).order_flag);
            case  8: return Comp(Data(row1).generated,     Data(row2).generated);
            case  9: return Comp(Data(row1).created,       Data(row2).created);
            case 10: return Comp(Data(row1).last_ddl_time, Data(row2).last_ddl_time);
            }
            return 0;
        }

        virtual int GetMinDefColWidth(int col) const {
            return !col ? 200 : ListDataProvider::GetMinDefColWidth(col);
        }

        virtual size_t Query();

        virtual InfoType GetInfoType() const { return InfoType::Sequence; }
    };
}
#endif /*__PROJECT_STONE_DATA_SEQUENCELISTADAPTER_H__*/

