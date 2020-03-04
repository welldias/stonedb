#include <algorithm>

#include "auto_cursor.h"
#include "error_util.h"
#include "variable.h"
#include "string_var.h"
#include "number_var.h"
#include "date_var.h"
#include "timestamp_var.h"
#include "dummy_var.h"
#include "lob_var.h"
#include "interval_var.h"

#include "../utils/utils.h"

namespace Oci20 {

	using namespace Utils;

	AutoCursor::AutoCursor(Connect& connect, const std::string& statementString, int prefetch, int strLimit, int blobHexRowLength)
		: Statement(connect, statementString, prefetch),
		m_IntNull(0),
		m_DoubleNull(0.0) {
		m_StringLimit = strLimit;
		m_BlobHexRowLength = blobHexRowLength;
		m_endOfFetch = false;
		m_SkipLobs = false;
	}

	AutoCursor::AutoCursor(Connect& connect, int prefetch, int strLimit, int blobHexRowLength)
		: Statement(connect, prefetch),
		m_IntNull(0),
		m_DoubleNull(0.0) {
		m_StringLimit = strLimit,
		m_BlobHexRowLength = blobHexRowLength;
		m_endOfFetch = false;
		m_SkipLobs = false;
	}

	AutoCursor::~AutoCursor() {
	}

	void AutoCursor::Attach(RefCursorVariable& refCur) {
		if (IsOpen()) 
			Close();

		RefCursorVariable tmpCur(m_connect, m_prefetch);

		m_sttmp = refCur.GetOCIStmt();
		refCur.SetOCIStmt(tmpCur.GetOCIStmt());

		Define();
	}

	void AutoCursor::Execute() {

		Clock64 startTime = SystemClock::StartCount();

		if (GetType() == Statement::Type::Select) {
			Statement::Execute(0);
			Define();
		}
		else {
			Statement::Execute(1);
		}

		SetExecutionTime(SystemClock::StopCount(startTime));

		m_endOfFetch = false;
	}

	void AutoCursor::ExecuteShadow() {

		Clock64 startTime = SystemClock::StartCount();

		if (GetType() == Statement::Type::Select) {
			Statement::ExecuteShadow(0);
			Define();
		}
		else {
			Statement::ExecuteShadow(1);
		}

		SetExecutionTime(SystemClock::StopCount(startTime));

		m_endOfFetch = false;
	}


	bool AutoCursor::Fetch() {
		if (!m_endOfFetch) {
			std::vector<Variable*>::const_iterator it = m_fields.begin();
			for (; it != m_fields.end(); ++it)
				(*it)->BeforeFetch();

			ub4 retRecords = 0;
			m_endOfFetch = Statement::Fetch(1, retRecords) ? false : true;
		}

		return !m_endOfFetch;
	}

	void AutoCursor::Bind(Variable* var, const std::string& name) {
		if (var == NULL)
			return;

		ERROR_UTIL_CHECKTHROW(var->Bind(GetOCIStmt(), GetOCIError(), name), m_connect, m_sttmText);
	}

	void AutoCursor::Bind(const std::string& name, const std::string& value) {
		
		Variable* fld = m_boundFields[name] = new StringVar(value);

		Statement::Bind(name, *fld);
	}

	void AutoCursor::Bind(const std::string& name, int value) {
		
		Variable* fld = m_boundFields[name] = new NumberVar(m_connect.GetOCIError(), value);

		Statement::Bind(name, *fld);
	}

	void AutoCursor::Bind(const std::string& name, double value) {

		Variable * fld = m_boundFields[name] = new NumberVar(m_connect.GetOCIError(), value);

		Statement::Bind(name, *fld);
	}

	void AutoCursor::Define() {

		m_fields.clear();
		m_fieldNames.clear();

		sb4 cols = 0;

		ERROR_UTIL_CHECKTHROW(OCIAttrGet(m_sttmp, OCI_HTYPE_STMT, (dvoid*)&cols, (ub4*)0, OCI_ATTR_PARAM_COUNT, GetOCIError()), m_connect, m_sttmText);

		int defined = 0;
		for (int col = 0; col < cols; col++) {
			ub1 char_form;
			ub2 type, size;
			OCIParam* paramd;
			Variable* fld = NULL;
			char* col_name = 0;
			ub4   col_name_len = 0;

			ERROR_UTIL_CHECKTHROW(OCIParamGet(m_sttmp, OCI_HTYPE_STMT, GetOCIError(), (dvoid**)&paramd, col + 1), m_connect, m_sttmText);
			ERROR_UTIL_CHECKTHROW(OCIAttrGet((dvoid*)paramd, OCI_DTYPE_PARAM, (dvoid*)&type, 0, OCI_ATTR_DATA_TYPE, GetOCIError()), m_connect, m_sttmText);
			ERROR_UTIL_CHECKTHROW(OCIAttrGet((dvoid*)paramd, OCI_DTYPE_PARAM, (dvoid*)&size, 0, OCI_ATTR_DATA_SIZE, GetOCIError()), m_connect, m_sttmText);
			ERROR_UTIL_CHECKTHROW(OCIAttrGet((dvoid*)paramd, OCI_DTYPE_PARAM, (dvoid*)&col_name, (ub4*)&col_name_len, OCI_ATTR_NAME, GetOCIError()), m_connect, m_sttmText);
			ERROR_UTIL_CHECKTHROW(OCIAttrGet((dvoid*)paramd, OCI_DTYPE_PARAM, (dvoid*)&char_form, 0, OCI_ATTR_CHARSET_FORM, GetOCIError()), m_connect, m_sttmText);

			switch (type) {
			case SQLT_CLOB:
				if (m_SkipLobs)
					fld = new DummyVar(true);
				else
					fld = new CLobVar(m_connect.GetOCIEnv(), m_connect.GetOCISvcCtx(), m_connect.GetOCIError(), m_StringLimit, static_cast<LobVar::CharForm>(char_form));
				break;
			case SQLT_BLOB:
				if (m_SkipLobs)
					fld = new DummyVar(true);
				else
					fld = new BLobVar(m_connect.GetOCIEnv(), m_connect.GetOCISvcCtx(), m_connect.GetOCIError(), m_StringLimit, m_BlobHexRowLength / 2);
				break;
			case SQLT_BFILE:
				if (m_SkipLobs)
					fld = new DummyVar(true);
				else
					fld = new BFileVar(m_connect.GetOCIEnv(), m_connect.GetOCISvcCtx(), m_connect.GetOCIError(), m_StringLimit, m_BlobHexRowLength / 2);
				break;
			case SQLT_RID:
			case SQLT_RDD:
				fld = new StringVar(20 + 1);
				break;
			case SQLT_BIN:
				size *= 2;
			case SQLT_LBI:
				fld = new StringVar((size && size < m_StringLimit) ? size : m_StringLimit);
				break;
			case SQLT_CHR:
			case SQLT_LNG:
			case SQLT_AFC:
				fld = new StringVar((size && size < m_StringLimit) ? size : m_StringLimit);
				break;
			case SQLT_INT:
			case SQLT_FLT:
			case SQLT_NUM:
			case SQLT_IBFLOAT:
			case SQLT_IBDOUBLE:
				if (m_NumberFormat.empty())
					fld = new NumStrVar();
				else
					fld = new NumberVar(m_connect.GetOCIError(), m_NumberFormat);
				break;

			case SQLT_DAT:
			case SQLT_DATE:
				//TODO: m_DateFormat pode vir vazio, neste caso recuperar de GetNlsDateFormat() para passar como parametro
				fld = new DateVar(m_connect.GetOCIError(), m_DateFormat);
				break;
			case SQLT_TIME:
			case SQLT_TIMESTAMP:
			case SQLT_TIMESTAMP_TZ:
			case SQLT_TIMESTAMP_LTZ:
				if (SETTINGS_PROPERTY_BOOL(TimestampSupported)) {
					TimestampVar::Type timeStampeType = static_cast<TimestampVar::Type>(type);
					fld = new TimestampVar(m_connect.GetOCIEnv(), timeStampeType, m_TimestampFormat);
				}
				break;
			case SQLT_INTERVAL_YM:
			case SQLT_INTERVAL_DS:
				if (SETTINGS_PROPERTY_BOOL(IntervalToTextSupported)) {
					IntervalVar::Type intervalType = static_cast<IntervalVar::Type>(type);
					fld = new IntervalVar(m_connect.GetOCIEnv(), intervalType);
				}
				break;
			default:
				fld = new DummyVar();
				break;
			}

			m_fields.push_back(fld);
			m_fieldNames.push_back(std::string(col_name, col_name_len));

			if (fld != NULL && !dynamic_cast<DummyVar*>(fld)) {
				Statement::Define(col + 1, *fld);
				defined++;
			}
		}

		if (!defined)
			throw OciException(-1, "Coi20::AutoCursor::Define: The query fails because all columns types are currently not supported.");
	}

	int AutoCursor::GetFieldCount() const {
		return static_cast<int>(m_fieldNames.size());
	}

	const std::string& AutoCursor::GetFieldName(int col) const {
		return m_fieldNames.at(col);
	}

	bool AutoCursor::IsNumberField(int col) const {
		return m_fields.at(col)->IsNumber();
	}

	bool AutoCursor::IsDatetimeField(int col) const {
		return m_fields.at(col)->IsDatetime();
	}

	std::string AutoCursor::BackcastField(int col, const std::string& val) const {
		return m_fields.at(col)->Backcast(val);
	}

	void AutoCursor::GetString(int col, std::string& buffstr) const {
		ERROR_UTIL_CHECKTHROW(m_fields.at(col)->GetString(buffstr, m_StringNull), m_connect, m_sttmText);
	}

	bool AutoCursor::IsNull(int col) const {
		return m_fields.at(col)->IsNull();
	}
}