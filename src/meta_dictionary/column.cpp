#include "column.h"

namespace MetaDictionary {

    Column::SubtypeMap::SubtypeMap() {
        insert(value_type("CHAR",      1));
        insert(value_type("NCHAR",     1));
        insert(value_type("RAW",       1));
        insert(value_type("VARCHAR2",  1));
        insert(value_type("NVARCHAR2", 1));
        insert(value_type("NUMBER",    2));
        insert(value_type("FLOAT",     2));
        insert(value_type("LONG",      0));
        insert(value_type("DATE",      0));
        insert(value_type("LONG RAW",  0));
        insert(value_type("ROWID",     0));
        insert(value_type("MLSLABEL",  0));
        insert(value_type("CLOB",      0));
        insert(value_type("NCLOB",     0));
        insert(value_type("BLOB",      0));
        insert(value_type("BFILE",     0));
    }

    Column::SubtypeMap Column::m_mapSubtypes;

    Column::Column() {

		m_nDataPrecision = 0;
		m_nDataScale = 0;
		m_nDataLength = 0;
		m_bNullable = false;
		m_charLengthSemantics = CharLengthSemantics::UseByte;
		m_defCharLengthSemantics = CharLengthSemantics::UseByte;
    }

    void Column::GetSpecString(std::string& strBuff, const MetaSettings& settings) const {

        strBuff = m_strDataType;
        switch (m_mapSubtypes[m_strDataType]) {
        case 0:
            break;
        case 1:
            strBuff += '(';
            strBuff += std::to_string(m_nDataLength);
            if ((m_strDataType == "VARCHAR2" || m_strDataType == "CHAR") && (m_charLengthSemantics != m_defCharLengthSemantics || settings.GetAlwaysWriteColumnLengthSematics()))
                strBuff += (m_charLengthSemantics == CharLengthSemantics::UseChar) ? " CHAR" : " BYTE";
            strBuff += ')';
            break;
        case 2:
            if (m_nDataPrecision != -1 && m_nDataScale != -1) {
                strBuff += '(';
                strBuff += std::to_string(m_nDataPrecision);
                strBuff += ',';
                strBuff += std::to_string(m_nDataScale);
                strBuff += ')';
            }
            else if (m_nDataPrecision != -1) {
                strBuff += '(';
                strBuff += std::to_string(m_nDataPrecision);
                strBuff += ')';
            }
            else if (m_nDataScale != -1) {
                strBuff = "INTEGER";
            }
            break;
        }
    }
}
