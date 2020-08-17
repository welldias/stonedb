#ifndef __PROJECT_STONE_META_DICTIONARY_TABLECOLUMN_H__
#define __PROJECT_STONE_META_DICTIONARY_TABLECOLUMN_H__

#include <string>

#include "column.h"

namespace MetaDictionary {

    class TableColumn : public Column {
    public:
        TableColumn() : m_virtual(false), m_identity(false) {}

        inline const std::string& GetDataDefault() const { return m_dataDefault; };
        inline const std::string& GetComments() const { return m_comments; };
        inline bool GetVirtual() const { return m_virtual; }
        inline bool GetIdentity() const { return m_identity; }
        inline const std::string& GetSequenceName() const { return m_sequenceName; };
        inline const std::string& GetIndentityGenerationType() const { return m_indentityGenerationType; };

    protected:
        std::string m_dataDefault;
        std::string m_comments;
        bool m_virtual;
        bool m_identity;
        std::string m_sequenceName;
        std::string m_indentityGenerationType;

    private:
        // copy-constraction & assign-operation is not supported
        TableColumn(const TableColumn&);
        void operator = (const TableColumn&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_TABLECOLUMN_H__*/