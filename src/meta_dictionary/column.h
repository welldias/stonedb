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

        inline const std::string& GetColumnName() const { return m_columnName; };
        inline const std::string& GetDataType() const { return m_dataType; };

        inline bool GetNullable() const { return m_nullable; }

    private:
        class SubtypeMap : public std::map<std::string, int> {
        public:
            SubtypeMap();
        };

        static SubtypeMap m_mapSubtypes;

    protected:
        std::string m_columnName;
        std::string m_dataType;
        int m_dataPrecision;
        int m_dataScale;
        int m_dataLength;
        bool m_nullable;

    private:
        // copy-constraction & assign-operation is not supported
        Column(const Column&);
        void operator = (const Column&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_COLUMN_H__*/