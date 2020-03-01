#include <cstring>

#include "connect.h"
#include "error_util.h"

#include "../utils/utils.h"

namespace Oci20 {

	using namespace Utils;

	Connect::Connect(unsigned mode) {
		m_open = false;
		m_openShadow = false;
		m_interrupted = false;
		m_isClosing = false;
		m_ext_auth = false;
		m_lastExecutionClockTime = 0;

		m_mode = Mode::Default;
		m_safety = Safety::None;
		m_clientVersion = ClientVersion::ClientUnknown;

		m_version = ServerVersion::ServerUnknown;
		m_bypassTns = false;

		m_envhp = 0;
		m_srvhp = 0;
		m_authp = 0;
		m_svchp = 0;
		m_errhp = 0;
		m_auth_shadowp = 0;
#if 0
		std::string version = Common::getDllVersionProperty("OCI.DLL", "FileVersion");

		if (!version.empty())
		{
			if (!StringUtil::Compare(version.c_str(), "11", 2))       m_clientVersoon = ecvClient11X;
			else if (!StringUtil::Compare(version.c_str(), "10", 2))  m_clientVersoon = ecvClient10X;
			else if (!StringUtil::Compare(version.c_str(), "9", 1))   m_clientVersoon = ecvClient9X;
			else if (!StringUtil::Compare(version.c_str(), "8.1", 3)) m_clientVersoon = ecvClient81X;
			else if (!StringUtil::Compare(version.c_str(), "8.0", 3)) m_clientVersoon = ecvClient80X;
		}
#endif

#if 0
		status = OCIEnvCreate((OCIEnv**)&m_envhp, mode, 0, 0, 0, 0, 0, 0));
#endif

		ERROR_UTIL_CHECKTHROW(OCIInitialize(mode, 0, 0, 0, 0), *this, "");
		ERROR_UTIL_CHECKTHROW(OCIEnvInit((OCIEnv**)&m_envhp, OCI_DEFAULT, 0, 0), *this, "");
	}

	Connect::~Connect() {
		Close();
		OCIHandleFree(m_envhp, OCI_HTYPE_ENV);
	}

	void Connect::DoOpen() {

		ERROR_UTIL_CHECKALLOC(OCIHandleAlloc(m_envhp, (dvoid**)&m_errhp, OCI_HTYPE_ERROR, 0, 0));
		ERROR_UTIL_CHECKALLOC(OCIHandleAlloc(m_envhp, (dvoid**)&m_srvhp, OCI_HTYPE_SERVER, 0, 0));
		ERROR_UTIL_CHECKTHROW(OCIServerAttach(m_srvhp, m_errhp, (OraText*)m_alias.c_str(), (sb4)m_alias.length(), (ub4)0), *this, "");
		ERROR_UTIL_CHECKALLOC(OCIHandleAlloc(m_envhp, (dvoid**)&m_authp, (ub4)OCI_HTYPE_SESSION, 0, 0));
		ERROR_UTIL_CHECKALLOC(OCIHandleAlloc(m_envhp, (dvoid**)&m_auth_shadowp, (ub4)OCI_HTYPE_SESSION, 0, 0));

		if (!m_uid.empty())
			ERROR_UTIL_CHECKTHROW(OCIAttrSet(m_authp, OCI_HTYPE_SESSION, (OraText*)m_uid.c_str(), (sb4)m_uid.length(), OCI_ATTR_USERNAME, m_errhp), *this, "");

		ERROR_UTIL_CHECKTHROW(OCIAttrSet(m_auth_shadowp, OCI_HTYPE_SESSION, (OraText*)m_uid.c_str(), (sb4)m_uid.length(), OCI_ATTR_USERNAME, m_errhp), *this, "");

		if (!m_password.empty())
			ERROR_UTIL_CHECKTHROW(OCIAttrSet(m_authp, OCI_HTYPE_SESSION, (OraText*)m_password.c_str(), (sb4)m_password.length(), OCI_ATTR_PASSWORD, m_errhp), *this, "");

		ERROR_UTIL_CHECKTHROW(OCIAttrSet(m_auth_shadowp, OCI_HTYPE_SESSION, (OraText*)m_password.c_str(), (sb4)m_password.length(), OCI_ATTR_PASSWORD, m_errhp), *this, "");
		ERROR_UTIL_CHECKALLOC(OCIHandleAlloc(m_envhp, (dvoid**)&m_svchp, OCI_HTYPE_SVCCTX, 0, 0));
		ERROR_UTIL_CHECKTHROW(OCIAttrSet(m_svchp, OCI_HTYPE_SVCCTX, m_srvhp, 0, OCI_ATTR_SERVER, m_errhp), *this, "");

		try {
			ERROR_UTIL_CHECKTHROW(OCISessionBegin(m_svchp, m_errhp, m_authp, m_ext_auth ? OCI_CRED_EXT : OCI_CRED_RDBMS, static_cast<unsigned int>(m_mode)), *this, "");
		}
		catch (const OciException & e) {
			switch (e) {
			case 28001: //ORA-28001: the password has expired
			case 28002: //ORA-28002: The password will expire within %s days 
			case 28011: //ORA-28011: the account will expire soon; change your password now
				throw OciException((int)e, e.what());
				break;
			case 604:
				if (const char* not_open_err = strstr((const char*)e.what(), "ORA-01219"))
					throw OciException(1219, not_open_err);
			default:
				throw;
			}
		}

		ERROR_UTIL_CHECKTHROW(OCIAttrSet(m_svchp, OCI_HTYPE_SVCCTX, m_authp, 0, OCI_ATTR_SESSION, m_errhp), *this, "");

		m_open = true;

		if (SETTINGS_PROPERTY_BOOL(DbmsXplanDisplayCursor) || SETTINGS_PROPERTY_BOOL(SessionStatistics)) {
			ERROR_UTIL_CHECKTHROW(OCISessionBegin(m_svchp, m_errhp, m_auth_shadowp, m_ext_auth ? OCI_CRED_EXT : OCI_CRED_RDBMS, static_cast<unsigned int>(m_mode)), *this, "");
			m_openShadow = true;
		}
		else
			m_openShadow = false;
	}

	void Connect::ProcAuthenticationInfo(const std::string& uid, const std::string& pswd, const std::string& alias, Mode mode, Safety safety) {
		m_version = ServerVersion::ServerUnknown;
		m_sessionCookies.clear();

		m_uid.erase();
		m_password.erase();
		m_alias.erase();

		m_mode = mode;
		m_safety = safety;

		m_ext_auth = false;

		if (!uid.length() && !pswd.length() && !alias.length()) {
			m_alias = alias;
			m_ext_auth = true;
		}
		else if (!pswd.length() || !alias.length()) {
			const char* ptr = uid.c_str();

			for (; *ptr && *ptr != '/' && *ptr != '@'; ptr++)
				m_uid += *ptr;

			if (*ptr == '/') {
				++ptr;
				for (; *ptr && *ptr != '@'; ptr++)
					m_password += *ptr;
			}

			if (*ptr == '@') {
				++ptr;
				for (; *ptr && *ptr != '@'; ptr++)
					m_alias += *ptr;
			}

			if (!alias.empty()) {
				if (!m_alias.empty())
					throw Exception(0, "Oci::Connect::Open: alias cannot be defined twice!");
				m_alias = alias;
			}
		}
		else {
			m_uid = uid;
			m_password = pswd;
			m_alias = alias;
		}
	}

	void Connect::Open(const std::string& uid, const std::string& pswd, const std::string& alias, Mode mode, Safety safety) {

		m_bypassTns = false;
		m_strHost.clear();
		m_strPort.clear();
		m_strSid.clear();

		ProcAuthenticationInfo(uid, pswd, alias, mode, safety);
		DoOpen();
	}

	void Connect::Open(const std::string& uid, const std::string& pswd, const std::string& host, const std::string& port, const std::string& sid, bool serviceInsteadOfSid, Mode mode, Safety safety) {

		m_bypassTns = true;
		m_strHost = host;
		m_strPort = port;
		m_strSid = sid;

		std::string alias = CreateTNSString(host, port, sid, serviceInsteadOfSid);

		ProcAuthenticationInfo(uid, pswd, alias, mode, safety);
		DoOpen();
	}

	void Connect::Reconnect() {
		m_version = ServerVersion::ServerUnknown;
		m_sessionCookies.clear();

		DoOpen();
	}

	void Connect::Close(bool purge) {

		if (!m_open)
			return;

		m_isClosing = true;

		std::set<Object*>::iterator i = m_dependencies.begin();
		while (i != m_dependencies.end())
			(*i++)->Close(purge);

		m_version = ServerVersion::ServerUnknown;
		m_sessionCookies.clear();

		if (m_openShadow) {
			try {
				ERROR_UTIL_CHECKTHROW(OCISessionEnd(m_svchp, m_errhp, m_auth_shadowp, OCI_DEFAULT), *this, "");
			}
			catch (const Exception&) {
				if (!purge) {
					m_isClosing = false;
					throw;
				}
			}
			m_openShadow = false;
		}

		try {
			ERROR_UTIL_CHECKTHROW(OCISessionEnd(m_svchp, m_errhp, m_authp, OCI_DEFAULT), *this, "");
		}
		catch (const OciException&) {
			if (!purge) {
				m_isClosing = false;
				throw;
			}
		}

		try {
			ERROR_UTIL_CHECKTHROW(OCIServerDetach(m_srvhp, m_errhp, OCI_DEFAULT), *this, "");
		}
		catch (const OciException&) {
			if (!purge) {
				m_isClosing = false;
				throw;
			}
		}
		m_isClosing = false;

		OCIHandleFree(m_svchp, OCI_HTYPE_SVCCTX);
		OCIHandleFree(m_authp, OCI_HTYPE_SESSION);
		OCIHandleFree(m_auth_shadowp, OCI_HTYPE_SESSION);
		OCIHandleFree(m_srvhp, OCI_HTYPE_SERVER);
		OCIHandleFree(m_errhp, OCI_HTYPE_ERROR);

		m_open = false;
	}

	void Connect::Commit(bool guaranteedSafe) {
		if (!guaranteedSafe && m_safety == Safety::ReadOnly)
			throw OciException(0, "Oci::Connect::Commit: the operation is not allowed for Read-Only connection!");

		ERROR_UTIL_CHECKINTERRUPT(*this);
		ERROR_UTIL_CHECKTHROW(OCITransCommit(m_svchp, m_errhp, (ub4)0), *this, "");
	}

	void Connect::Rollback() {
		ERROR_UTIL_CHECKINTERRUPT(*this);
		ERROR_UTIL_CHECKTHROW(OCITransRollback(m_svchp, m_errhp, (ub4)0), *this, "");
	}

	BreakHandler* Connect::CreateBreakHandler() {
		OCIError* errhp = 0;
		ERROR_UTIL_CHECKALLOC(OCIHandleAlloc(m_envhp, (dvoid**)&errhp, OCI_HTYPE_ERROR, 0, 0));
		return new BreakHandler(m_svchp, errhp);
	}

	void Connect::CheckShadowSession(const bool bForceConnectShadow) {
		if (bForceConnectShadow) {
			if ((!m_openShadow) && m_open) {
				ERROR_UTIL_CHECKTHROW(OCISessionBegin(m_svchp, m_errhp, m_auth_shadowp, m_ext_auth ? OCI_CRED_EXT : OCI_CRED_RDBMS, static_cast<unsigned int>(m_mode)), *this, "");
				m_openShadow = true;
			}
		}
		else if (m_openShadow) {
			ERROR_UTIL_CHECKTHROW(OCISessionEnd(m_svchp, m_errhp, m_auth_shadowp, OCI_DEFAULT), *this, "");
			m_openShadow = false;
		}
	}

	void Connect::SetSession() {
		if (m_open)
			ERROR_UTIL_CHECKTHROW(OCIAttrSet(m_svchp, OCI_HTYPE_SVCCTX, m_authp, 0, OCI_ATTR_SESSION, m_errhp), *this, "");
	}

	void Connect::SetShadowSession() {
		if (m_openShadow)
			ERROR_UTIL_CHECKTHROW(OCIAttrSet(m_svchp, OCI_HTYPE_SVCCTX, m_auth_shadowp, 0, OCI_ATTR_SESSION, m_errhp), *this, "");
	}

	void Connect::Break(bool purge) {
		if (!m_open)
			return;

		m_interrupted = true;

		if (purge)
			OCIBreak(m_svchp, m_errhp);
		else
			ERROR_UTIL_CHECKTHROW(OCIBreak(m_svchp, m_errhp), *this, "");
	}

	void Connect::Attach(Object* obj) {
		m_dependencies.insert(obj);
	}

	void Connect::Detach(Object* obj) {
		m_dependencies.erase(obj);
	}

	std::string Connect::CreateTNSString(const std::string& host, const std::string& port, const std::string& sid, bool serviceInsteadOfSid)
	{
		return std::string("(DESCRIPTION=(ADDRESS_LIST=(ADDRESS=(PROTOCOL=TCP)(Host=") + host + ")(Port=" + port + ")))"
			"(CONNECT_DATA=(" + (serviceInsteadOfSid ? "SERVICE_NAME" : "SID") + "=" + sid + ")))";
	}

	std::string Connect::GetDisplayString(bool mode) const
	{
		std::string displayStr = "@" + GetUID();

		if (!m_bypassTns) {
			displayStr += GetAlias();
		}
		else {
			displayStr += m_strSid;
			displayStr += ':';
			displayStr += m_strHost;
		}

		if (mode) {
			switch (GetMode()) {
			case Mode::SysDba:  displayStr += " as sysdba"; break;
			case Mode::SysOper: displayStr += " as sysoper"; break;
			}
		}

		return displayStr;
	}

	void Connect::SetSessionCookie(const std::string& key, const std::string& val) {
		m_sessionCookies[key] = val;
	}

	bool Connect::GetSessionCookie(const std::string& key, std::string& val) const {

		std::map<std::string, std::string>::const_iterator it = m_sessionCookies.find(key);
		if (it != m_sessionCookies.end()) {
			val = it->second;
			return true;
		}
		return false;
	}
}