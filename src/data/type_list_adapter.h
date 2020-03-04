#ifndef __PROJECT_STONE_DATA_TYPELISTADAPTER_H__
#define __PROJECT_STONE_DATA_TYPELISTADAPTER_H__

#include <string>
#include <vector>
#include <cstdint>

#include "list_data_provider.h"
#include "list_data_provider_helper.h"

#include "../oci20/oci20.h"

namespace Data {

    struct TypeCodeEntry {
        std::string owner        ;
        std::string object_name  ;
        std::string typecode     ;
        int         attributes   ;
        int         methods      ;
        std::string incomplete   ;
        tm          created      ;
        tm          last_ddl_time;
        std::string status       ;

        TypeCodeEntry();
        bool deleted;
    };

    class TypeCodeListAdapter : public ListDataProvider, ListDataProviderHelper {
    public:
        enum class MonoType {
            Type,
            Bodies,
        };
    
    protected:
        std::vector<TypeCodeEntry> m_entries;
        Oci20::Connect& m_connect;
        MonoType m_monoType;

    public:
        TypeCodeListAdapter(Oci20::Connect& connect, MonoType monoType);

        const TypeCodeEntry& Data(int row) const { return m_entries.at(row); }

        virtual int GetRowCount() const { return (int)m_entries.size(); }

        virtual int GetColCount() const { return 8; }

        virtual ListDataProvider::ColumnType GetColumnType(int col) const {
            switch (col) {
            case 0: return ColumnType::String;  //object_name
            case 1: return ColumnType::String;  //typecode
            case 2: return ColumnType::Number;  //attributes
            case 3: return ColumnType::Number;  //methods
            case 4: return ColumnType::String;  //incomplete
            case 5: return ColumnType::Date;    //created
            case 6: return ColumnType::Date;    //last_ddl_time
            case 7: return ColumnType::String;  //status
            }

            return ColumnType::String;
        }

        virtual void GetColHeader(int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case 0: value = "Name";       return;
            case 1: value = "Type";       return;
            case 2: value = "Attrs";      return;
            case 3: value = "Methods";    return;
            case 4: value = "Incomplete"; return;
            case 5: value = "Created";    return;
            case 6: value = "Modified";   return;
            case 7: value = "Status";     return;
            }
        }

        virtual void GetString(int row, int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case 0: ToString(Data(row).object_name  ,value); return;
            case 1: ToString(Data(row).typecode     ,value); return;
            case 2: ToString(Data(row).attributes   ,value); return;
            case 3: ToString(Data(row).methods      ,value); return;
            case 4: ToString(Data(row).incomplete   ,value); return;
            case 5: ToString(Data(row).created      ,value); return;
            case 6: ToString(Data(row).last_ddl_time,value); return;
            case 7: ToString(Data(row).status       ,value); return;
            }
        }

        bool IsVisibleRow(int row) const {
            return !Data(row).deleted;
        }

        virtual int Compare(int row1, int row2, int col) const {
            switch (col) {
            case 0: return Comp(Data(row1).object_name   ,Data(row2).object_name);
            case 1: return Comp(Data(row1).typecode      ,Data(row2).typecode);
            case 2: return Comp(Data(row1).attributes    ,Data(row2).attributes);
            case 3: return Comp(Data(row1).methods       ,Data(row2).methods);
            case 4: return Comp(Data(row1).incomplete    ,Data(row2).incomplete);
            case 5: return Comp(Data(row1).created       ,Data(row2).created);
            case 6: return Comp(Data(row1).last_ddl_time ,Data(row2).last_ddl_time);
            case 7: return Comp(Data(row1).status        ,Data(row2).status);
            }
            return 0;
        }

        virtual int GetMinDefColWidth(int col) const {
            return !col ? 200 : ListDataProvider::GetMinDefColWidth(col);
        }

        virtual size_t Query();

        virtual InfoType GetInfoType() const { return InfoType::Type; }
    };
}
#endif /*__PROJECT_STONE_DATA_TYPELISTADAPTER_H__*/