#ifndef __PROJECT_STONE_DATA_CODELISTADAPTER_H__
#define __PROJECT_STONE_DATA_CODELISTADAPTER_H__

#include <string>
#include <vector>
#include <cstdint>

#include "list_data_provider.h"
#include "list_data_provider_helper.h"

#include "../oci20/oci20.h"

namespace Data {

    struct CodeEntry {
        std::string owner        ;   //"owner"         HIDDEN
        std::string object_name  ;   //"object_name"   "Name"
        tm          created      ;   //"created"       "Created"
        tm          last_ddl_time;   //"last_ddl_time" "Modified"
        std::string status       ;   //"status"        "Status"

        CodeEntry();
        bool deleted;
    };

    class CodeListAdapter : public ListDataProvider, ListDataProviderHelper {
    public:
        enum class MonoType {
            Procedure,
            Function,
            Package,
            PackageBody, 
            Java,
        };
    
    protected:
        std::vector<CodeEntry> m_entries;
        Oci20::Connect& m_connect;
        MonoType m_monoType;

    public:
        CodeListAdapter(Oci20::Connect& connect, MonoType monoType);

        const CodeEntry& Data(int row) const { return m_entries.at(row); }

        virtual int GetRowCount() const { return (int)m_entries.size(); }

        virtual int GetColCount() const { return 4; }

        virtual ListDataProvider::ColumnType GetColumnType(int col) const {
            switch (col) {
            case 0: return ColumnType::String;
            case 1: return ColumnType::Date;  
            case 2: return ColumnType::Date;  
            case 3: return ColumnType::String;
            }

            return ColumnType::String;
        }

        virtual void GetColHeader(int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case 0: value = "Name"      ; return;
            case 1: value = "Created"   ; return;
            case 2: value = "Modifies"  ; return;
            case 3: value = "Status"    ; return;
            }
        }

        virtual void GetString(int row, int col, std::string& value) const {

            value = "Unknown";

            switch (col) {
            case 0: ToString(Data(row).object_name  ,value); return;
            case 1: ToString(Data(row).created      ,value); return;
            case 2: ToString(Data(row).last_ddl_time,value); return;
            case 3: ToString(Data(row).status       ,value); return;
            }
        }

        bool IsVisibleRow(int row) const {
            return !Data(row).deleted;
        }

        virtual int Compare(int row1, int row2, int col) const {
            switch (col) {
            case 0: return Comp(Data(row1).object_name  ,Data(row2).object_name  );
            case 1: return Comp(Data(row1).created      ,Data(row2).created);
            case 2: return Comp(Data(row1).last_ddl_time,Data(row2).last_ddl_time);
            case 3: return Comp(Data(row1).status       ,Data(row2).status);
            }
            return 0;
        }

        virtual int GetMinDefColWidth(int col) const {
            return !col ? 200 : ListDataProvider::GetMinDefColWidth(col);
        }

        virtual size_t Query();

        virtual InfoType GetInfoType() const {
            switch (m_monoType) {
            case MonoType::Function:    return InfoType::Function;
            case MonoType::Package:     return InfoType::Package;
            case MonoType::PackageBody: return InfoType::PackageBody;
            case MonoType::Procedure:
            default:
                return InfoType::Procedure;
            }

        }
    };
}
#endif /*__PROJECT_STONE_DATA_CODELISTADAPTER_H__*/