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
        enum class ColumnName {
            SequenceName = 0,
            LastNumber,
            MinValue,
            MaxValue,
            Interval,
            Cache,
            Cycle,
            Order,
            Generated,
            Created,
            Modified,
            Count,
        };

        SequenceListAdapter(Oci20::Connect& connect);

        const SequenceEntry& Data(int row) const { return m_entries.at(row); }

        virtual int GetRowCount() const { return (int)m_entries.size(); }

        virtual int GetColCount() const { return static_cast<int>(ColumnName::Count); }

        virtual ListDataProvider::ColumnType GetColumnType(int col) const {
            
            switch (static_cast<ColumnName>(col)) {
            case ColumnName::SequenceName:  return ColumnType::String;
            case ColumnName::LastNumber:    return ColumnType::String;
            case ColumnName::MinValue:      return ColumnType::String;
            case ColumnName::MaxValue:      return ColumnType::String;
            case ColumnName::Interval:      return ColumnType::String;
            case ColumnName::Cache:         return ColumnType::String;
            case ColumnName::Cycle:         return ColumnType::String;
            case ColumnName::Order:         return ColumnType::String;
            case ColumnName::Generated:     return ColumnType::String;
            case ColumnName::Created:       return ColumnType::Date;
            case ColumnName::Modified:      return ColumnType::Date;
            }

            return ColumnType::String;
        }

        virtual void GetColHeader(int col, std::string& value) const {

            value = "Unknown";

            switch (static_cast<ColumnName>(col)) {
            case ColumnName::SequenceName:  value = "Sequence Name";
            case ColumnName::LastNumber:    value = "Last Number";
            case ColumnName::MinValue:      value = "Min Value";
            case ColumnName::MaxValue:      value = "Max Value";
            case ColumnName::Interval:      value = "Interval";
            case ColumnName::Cache:         value = "Cache";
            case ColumnName::Cycle:         value = "Cycle";
            case ColumnName::Order:         value = "Order";
            case ColumnName::Generated:     value = "Generated";
            case ColumnName::Created:       value = "Created";
            case ColumnName::Modified:      value = "Modified";
            }
        }

        virtual void GetString(int row, int col, std::string& value) const {

            value = "Unknown";

            switch (static_cast<ColumnName>(col)) {
            case ColumnName::SequenceName:  ToString(Data(row).sequence_name     ,value);
            case ColumnName::LastNumber:    ToString(Data(row).last_number       ,value);
            case ColumnName::MinValue:      ToString(Data(row).min_value         ,value);
            case ColumnName::MaxValue:      ToString(Data(row).max_value         ,value);
            case ColumnName::Interval:      ToString(Data(row).increment_by      ,value);
            case ColumnName::Cache:         ToString(Data(row).cache_size        ,value);
            case ColumnName::Cycle:         ToString(Data(row).cycle_flag        ,value);
            case ColumnName::Order:         ToString(Data(row).order_flag        ,value);
            case ColumnName::Generated:     ToString(Data(row).generated         ,value);
            case ColumnName::Created:       ToString(Data(row).created           ,value);
            case ColumnName::Modified:      ToString(Data(row).last_ddl_time     ,value);
            }
        }

        bool IsVisibleRow(int row) const {
            return !Data(row).deleted;
        }

        virtual int Compare(int row1, int row2, int col) const {
            switch (static_cast<ColumnName>(col)) {
            case ColumnName::SequenceName:  return Comp(Data(row1).sequence_name     ,Data(row2).sequence_name);
            case ColumnName::LastNumber:    return Comp(Data(row1).last_number       ,Data(row2).last_number);
            case ColumnName::MinValue:      return Comp(Data(row1).min_value         ,Data(row2).min_value);
            case ColumnName::MaxValue:      return Comp(Data(row1).max_value         ,Data(row2).max_value);
            case ColumnName::Interval:      return Comp(Data(row1).increment_by      ,Data(row2).increment_by);
            case ColumnName::Cache:         return Comp(Data(row1).cache_size        ,Data(row2).cache_size);
            case ColumnName::Cycle:         return Comp(Data(row1).cycle_flag        ,Data(row2).cycle_flag);
            case ColumnName::Order:         return Comp(Data(row1).order_flag        ,Data(row2).order_flag);
            case ColumnName::Generated:     return Comp(Data(row1).generated         ,Data(row2).generated);
            case ColumnName::Created:       return Comp(Data(row1).created           ,Data(row2).created);
            case ColumnName::Modified:      return Comp(Data(row1).last_ddl_time     ,Data(row2).last_ddl_time);
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

