#ifndef __PROJECT_STONE_META_DICTIONARY_TABLECOLUMN_H__
#define __PROJECT_STONE_META_DICTIONARY_TABLECOLUMN_H__

#include <string>

#include "column.h"

namespace MetaDictionary {

    class TableColumn : public Column {
    private:
        std::string m_strDataDefault;
        std::string m_strComments;
        bool m_bVirtual;
        bool m_bIdentity;
        std::string m_strSequenceName;
        std::string m_strIndentityGenerationType;

    public:
        TableColumn() : m_bVirtual(false), m_bIdentity(false) {}

    private:
        // copy-constraction & assign-operation is not supported
        TableColumn(const TableColumn&);
        void operator = (const TableColumn&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_TABLECOLUMN_H__*/