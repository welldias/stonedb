#ifndef __PROJECT_STONE_META_DICTIONARY_COLUMN_H__
#define __PROJECT_STONE_META_DICTIONARY_COLUMN_H__

#include <map>

#include "meta_settings.h"

namespace MetaDictionary {

    class Column
    {
    public:
        enum class CharLengthSemantics { 
            UseByte, 
            UseChar 
        };

        Column();

        CharLengthSemantics m_charLengthSemantics;
        CharLengthSemantics m_defCharLengthSemantics;

        void GetSpecString(std::string& strBuff, const MetaSettings& settings) const;


    private:
        class SubtypeMap : public std::map<std::string, int> {
        public:
            SubtypeMap();
        };
        static SubtypeMap m_mapSubtypes;

    protected:
        std::string m_strColumnName;
        std::string m_strDataType;
        int m_nDataPrecision;
        int m_nDataScale;
        int m_nDataLength;
        bool m_bNullable;

    private:
        // copy-constraction & assign-operation is not supported
        Column(const Column&);
        void operator = (const Column&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_COLUMN_H__*/