#include <typeinfo>
#include <cstdint>


#include "buff_cursor.h"
#include "string_var.h"
#include "number_var.h"
#include "error_util.h"

namespace Oci20 {

	std::string BuffCursor::m_defaultDateFormat = "%Y.%m.%d %H:%M:%S";
	std::string BuffCursor::m_defaultNull;
	int         BuffCursor::m_defaultIntNull = 0;
	double      BuffCursor::m_defaultDoubleNull = 0.0;


    BuffCursor::BuffCursor(Connect& connect, const std::string& statementString, int prefetch, int strLimit) 
		: Statement(connect, statementString, prefetch),
		m_dateFormat(m_defaultDateFormat),
		m_null(m_defaultNull),
		m_intNull(m_defaultIntNull),
		m_doubleNull(m_defaultDoubleNull) {

		m_strLimit = strLimit;
		m_curRecord = static_cast<ub4>(-1);
		m_retRecords = m_buffRecords = 0;
		m_endOfFetch = false;
    }

    BuffCursor::BuffCursor(Connect& connect, int prefetch, int strLimit) 
		: Statement(connect, prefetch),
		m_dateFormat(m_defaultDateFormat),
		m_null(m_defaultNull),
		m_intNull(m_defaultIntNull),
		m_doubleNull(m_defaultDoubleNull) {
		
		m_strLimit = strLimit;
		m_curRecord = static_cast<ub4>(-1);
		m_retRecords = m_buffRecords = 0;
		m_endOfFetch = false;
    }

    BuffCursor::~BuffCursor() {

    }

	void BuffCursor::Execute() {
		Statement::Execute(0);

		Define();

		m_curRecord = static_cast<ub4>(-1);
		m_retRecords = m_buffRecords = 0;
		m_endOfFetch = false;
	}

	void BuffCursor::ExecuteShadow() {
		Statement::ExecuteShadow(0);

		Define();

		m_curRecord = static_cast<ub4>(-1);
		m_retRecords = m_buffRecords = 0;
		m_endOfFetch = false;
	}

	bool BuffCursor::Fetch() {
		bool retVal = false;

		if (m_curRecord < m_buffRecords - 1) {
			m_curRecord++;
			retVal = true;
		}
		else if (!m_endOfFetch)
		{
			ub4 retRecords = 0;
			m_endOfFetch = Statement::Fetch((ub4)m_prefetch, retRecords) ? false : true;
			m_buffRecords = retRecords - m_retRecords;
			m_retRecords = retRecords;
			m_curRecord = 0;

			retVal = m_buffRecords > 0 ? true : false;

			std::vector<HostArray*>::iterator it = m_fields.begin();
			for (; it != m_fields.end(); ++it)
				(*it)->SetCurCoutn(m_retRecords);
		}

		return retVal;
	}

	void BuffCursor::Define() {
		m_fields.clear();
		m_fieldNames.clear();

		ub4 rowSize = 0;
		sb4 cols = 0;
		
		ERROR_UTIL_CHECKTHROW(OCIAttrGet(m_sttmp, OCI_HTYPE_STMT, (dvoid*)&cols, (ub4*)0, OCI_ATTR_PARAM_COUNT, GetOCIError()), m_connect, m_sttmText);

		for (int col = 0; col < cols; col++) {
			ub2 type, size;
			OCIParam* paramd;
			HostArray* fld;
			char* col_name = 0;
			ub4   col_name_len = 0;

			ERROR_UTIL_CHECKTHROW(OCIParamGet(m_sttmp, OCI_HTYPE_STMT, GetOCIError(), (dvoid**)&paramd, col + 1), m_connect, m_sttmText);
			ERROR_UTIL_CHECKTHROW(OCIAttrGet((dvoid*)paramd, OCI_DTYPE_PARAM, (dvoid*)&type, 0, OCI_ATTR_DATA_TYPE, GetOCIError()), m_connect, m_sttmText);
			ERROR_UTIL_CHECKTHROW(OCIAttrGet((dvoid*)paramd, OCI_DTYPE_PARAM, (dvoid*)&size, 0, OCI_ATTR_DATA_SIZE, GetOCIError()), m_connect, m_sttmText);
			ERROR_UTIL_CHECKTHROW(OCIAttrGet((dvoid*)paramd, OCI_DTYPE_PARAM, (dvoid*)&col_name, (ub4*)&col_name_len, OCI_ATTR_NAME, GetOCIError()), m_connect, m_sttmText);

			rowSize += ((size && size < m_strLimit) ? size : m_strLimit);

			switch (type) {
			case SQLT_CHR:
			case SQLT_LNG:
			case SQLT_CLOB:
			case SQLT_AFC:
				fld = new StringArray((size && size < m_strLimit) ? size : m_strLimit, m_prefetch);
				break;
			case SQLT_INT:
			case SQLT_FLT:
			case SQLT_NUM:
				fld = new NumberArray(m_prefetch);
				break;
			case SQLT_DAT:
				fld = new DateArray(m_prefetch, m_dateFormat);
				break;
			default:
				throw OciException(0, "Oci20::BuffCursor::Define(): Unsupported data type!");
				break;
			}

			m_fields.push_back(fld);
			m_fieldNames.push_back(std::string(col_name, col_name_len));

			Statement::Define(col + 1, *fld);
		}
	}

	void BuffCursor::Bind(const std::string& name, const std::string& value) {

		Variable* fld = m_boundFields[name] = new StringVar(value);

		Statement::Bind(name, *fld);
	}

	void BuffCursor::Bind(const std::string& name, char value) {

		Variable* fld = m_boundFields[name] = new StringVar(std::string(1, value));

		Statement::Bind(name, *fld);
	}

	void BuffCursor::Bind(const std::string& name, int value) {

		Variable* fld = m_boundFields[name] = new NumberVar(m_connect.GetOCIError(), value);

		Statement::Bind(name, *fld);
	}

	void BuffCursor::Bind(const std::string& name, double value) {

		Variable* fld = m_boundFields[name] = new NumberVar(m_connect.GetOCIError(), value);

		Statement::Bind(name, *fld);
	}

	bool BuffCursor::IsRecordGood() const {
		
		std::vector<HostArray*>::const_iterator it = m_fields.begin();
		for (; it != m_fields.end(); ++it)
			if (!(*it)->IsGood(m_curRecord))
				return false;

		return true;
	}

	bool BuffCursor::IsRecordTruncated() const {
		std::vector<HostArray*>::const_iterator it = m_fields.begin();
		for (; it != m_fields.end(); ++it)
			if ((*it)->IsTruncated(m_curRecord))
				return true;

		return false;
	}

	void BuffCursor::SetStringLimit(int lim) {
		m_strLimit = lim;
	}

	void BuffCursor::SetDateFormat(const std::string& dateFormat) {
		m_dateFormat = dateFormat;
	}

	void BuffCursor::SetStringNull(const std::string& null) {
		m_null = null;
	}
	
	const std::string& BuffCursor::GetStringNull() const {
		return m_null;
	}

	void BuffCursor::SetIntNull(int null) {
		m_intNull = null;
	}

	void BuffCursor::SetDoubleNull(double null) {
		m_doubleNull = null;
	}

	int BuffCursor::GetFieldCount() const {
		return static_cast<int>(m_fieldNames.size());
	}

	const std::string& BuffCursor::GetFieldName(int col) const {
		return m_fieldNames.at(col);
	}

	void BuffCursor::GetString(int col, std::string& buffstr) const {
		ERROR_UTIL_CHECKTHROW(m_fields.at(col)->GetString(m_curRecord, buffstr, m_null), m_connect, m_sttmText);
	}

	std::string BuffCursor::ToString(int col) const {
		std::string buffstr;
		GetString(col, buffstr);
		return buffstr;
	}

	std::string BuffCursor::ToString(int col, const std::string& null) const {
		std::string buffstr;
		ERROR_UTIL_CHECKTHROW(m_fields.at(col)->GetString(m_curRecord, buffstr, null), m_connect, m_sttmText);
		return buffstr;
	}

	void BuffCursor::GetTime(int col, struct tm& time, struct tm* null) const {
		m_fields.at(col)->GetTime(m_curRecord, time, null);
	}

	int BuffCursor::ToInt(int col) const {
		return m_fields.at(col)->ToInt(m_curRecord, m_intNull);
	}

	int BuffCursor::ToInt(int col, int null) const {
		return m_fields.at(col)->ToInt(m_curRecord, null);
	}

	int64_t BuffCursor::ToInt64(int col) const {
		return m_fields.at(col)->ToInt64(m_curRecord, m_intNull);
	}

	int64_t BuffCursor::ToInt64(int col, int64_t null) const {
		return m_fields.at(col)->ToInt64(m_curRecord, null);
	}

	double BuffCursor::ToDouble(int col) const {
		return m_fields.at(col)->ToDouble(m_curRecord, m_doubleNull);
	}

	double BuffCursor::ToDouble(int col, double null) const {
		return m_fields.at(col)->ToDouble(m_curRecord, null);
	}

	bool BuffCursor::IsGood(int col) const {
		return m_fields.at(col)->IsGood(m_curRecord);
	}

	bool BuffCursor::IsNull(int col) const {
		return m_fields.at(col)->IsNull(m_curRecord);
	}

	bool BuffCursor::IsTruncated(int col) const {
		return m_fields.at(col)->IsTruncated(m_curRecord);
	}

	bool BuffCursor::IsStringField(int col) const {
		return typeid(*m_fields.at(col)) == typeid(StringArray);
	}

	bool BuffCursor::IsNumberField(int col) const {
		return typeid(*m_fields.at(col)) == typeid(NumberArray);
	}
}