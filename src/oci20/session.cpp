#include <cstring>

#include "session.h"
#include "statement.h"
#include "buff_cursor.h"
#include "string_var.h"
#include "error_util.h"

#include "../utils/utils.h"

namespace Oci20 {

    Session::Session() {
        m_GetSIDFailed = false;
        m_databaseOpen = false;
        m_OutputEnable = false;
        m_OutputSize = 20000L;
        m_UnlimitedOutputSize = false;
	}

    Session::~Session() {
	}

    void Session::Open(const std::string& uid, const std::string& pswd, const std::string& alias, Connect::Mode mode, Connect::Safety safety) {
        
        m_GetSIDFailed = false;
        m_sessionSid.clear();
        m_strVersion.clear();
        m_strGlobalName.clear();

        m_connect.Open(uid, pswd, alias, mode, safety);

        try {
            LoadSessionNlsParameters();
            m_databaseOpen = true;
        }
        catch (const OciException) {
            throw;
        }

        AlterSessionNlsParams();
        EnableOutput(m_OutputEnable, m_OutputSize, true);
        GetSessionSid();
        if (m_connect.IsOpenShadow())
            SetShadowClientInfo();
    }

    void Session::Open(const std::string& uid, const std::string& pswd, const std::string& host, const std::string& port, const std::string& sid, bool serviceInsteadOfSid, Connect::Mode mode, Connect::Safety safety) {
        m_GetSIDFailed = false;
        m_sessionSid.clear();
        m_strVersion.clear();
        m_strGlobalName.clear();

        m_connect.Open(uid, pswd, host, port, sid, serviceInsteadOfSid, mode, safety);

        try {
            LoadSessionNlsParameters();
            m_databaseOpen = true;
        }
        catch (const OciException) {
            throw;
        }

        AlterSessionNlsParams();
        EnableOutput(m_OutputEnable, m_OutputSize, true);
        GetSessionSid();
        if (m_connect.IsOpenShadow())
            SetShadowClientInfo();
    }

    void Session::Reconnect() {
        m_sessionSid.clear();
        m_strVersion.clear();
        m_strGlobalName.clear();

        m_connect.Reconnect();

        try {
            LoadSessionNlsParameters();
            m_databaseOpen = true;
        }
        catch (const OciException) {
            throw;
        }

        AlterSessionNlsParams();
        EnableOutput(m_OutputEnable, m_OutputSize, true);
        GetSessionSid();
        if (m_connect.IsOpenShadow())
            SetShadowClientInfo();
    }

    void Session::Close(bool purge) {
        m_connect.Close(purge);
        
        m_strVersion.clear();
        m_sessionSid.clear();
        m_strGlobalName.clear();
        m_databaseOpen = false;
    }

    void Session::ExecuteStatement(const std::string& statementString, bool guaranteedSafe) {
        Statement cursor(m_connect);
        cursor.Prepare(statementString);
        cursor.Execute(1, guaranteedSafe);
    }

    void Session::ExecuteShadowStatement(const std::string& statementString, bool guaranteedSafe) {
        Statement cursor(m_connect);
        cursor.Prepare(statementString);
        cursor.ExecuteShadow(1, guaranteedSafe);
    }

    void Session::CheckShadowSession(const bool bForceConnectShadow) {

        m_connect.CheckShadowSession(bForceConnectShadow);

        if (m_connect.IsOpenShadow())
            SetShadowClientInfo();
    }

    void Session::SetShadowClientInfo() {
        if (!m_connect.IsOpen())
            return;

        std::string buff = "BEGIN DBMS_APPLICATION_INFO.SET_CLIENT_INFO('SQLTools_pp background session'); END;";
        ExecuteShadowStatement(buff.c_str(), true);
    }

    void Session::EnableOutput(bool enable, unsigned long size, bool connectInit) {

        if (m_OutputEnable != enable
            || m_OutputEnable && connectInit
            || m_OutputSize != size
            || m_UnlimitedOutputSize != SETTINGS_PROPERTY_BOOL(UnlimitedOutputSize)) {

            if (size != ULONG_MAX)
                m_OutputSize = size;

            if (m_connect.IsOpen()) {
                if (enable) {
                    std::string strStatement;

                    // Version 10 and above support unlimited output buffer size
                    if ((GetVersion() >= Connect::ServerVersion::Server10X)
                        && ((m_OutputSize > 1000000) || SETTINGS_PROPERTY_BOOL(UnlimitedOutputSize)))
                        strStatement = "BEGIN dbms_output.enable(NULL); END;";
                    else {
                        strStatement = "BEGIN dbms_output.enable(";
                        strStatement += std::to_string(m_OutputSize);
                        strStatement += "); END;";
                    }
                    ExecuteStatement(strStatement, true);
                }
                else if (!connectInit) { // avoid server call because it's default
                    ExecuteStatement("BEGIN dbms_output.disable; END;", true);
                }
            }

            m_OutputEnable = enable;
            m_UnlimitedOutputSize = SETTINGS_PROPERTY_BOOL(UnlimitedOutputSize);
        }
    }

    void Session::ResetOutput() {
        EnableOutput(m_OutputEnable);
    }

    std::string Session::GetSessionSid()  {
        if (!m_sessionSid.size() && (!m_GetSIDFailed)) {
            try {
                BuffCursor cursor(m_connect);
                cursor.Prepare("SELECT sid FROM v$mystat where rownum <= 1");
                cursor.Execute();
                cursor.Fetch();
                cursor.GetString(0, m_sessionSid);
            }
            catch (const OciException) {
                //TODO:
                //Global::SetStatusText((string("Error: ") + x.what() + string(" reading sid from v$mystat.")).c_str());

                m_GetSIDFailed = true;
            }
        }

        return m_sessionSid;
    }

    void Session::LoadSessionNlsParameters() {

        m_dbNlsLanguage.clear();
        m_dbNlsNumericCharacters.clear();
        m_dbNlsDateFormat.clear();
        m_dbNlsDateLanguage.clear();
        m_dbNlsTimeFormat.clear();
        m_dbNlsTimestampFormat.clear();
        m_dbNlsTimeTzFormat.clear();
        m_dbNlsTimestampTzFormat.clear();
        m_dbNlsLengthSemantics.clear();

        BuffCursor cursor(m_connect);
        cursor.Prepare("SELECT parameter, value FROM sys.nls_session_parameters");
        cursor.Execute();

        while (cursor.Fetch()) {
            std::string parameter, value;
            cursor.GetString(0, parameter);
            cursor.GetString(1, value);

            if (parameter == "NLS_LANGUAGE")
                m_dbNlsLanguage = value;
            else if (parameter == "NLS_NUMERIC_CHARACTERS")
                m_dbNlsNumericCharacters = value;
            else if (parameter == "NLS_DATE_FORMAT")
                m_dbNlsDateFormat = value;
            else if (parameter == "NLS_DATE_LANGUAGE")
                m_dbNlsDateLanguage = value;
            else if (parameter == "NLS_TIME_FORMAT")
                m_dbNlsTimeFormat = value;
            else if (parameter == "NLS_TIMESTAMP_FORMAT")
                m_dbNlsTimestampFormat = value;
            else if (parameter == "NLS_TIME_TZ_FORMAT")
                m_dbNlsTimeTzFormat = value;
            else if (parameter == "NLS_TIMESTAMP_TZ_FORMAT")
                m_dbNlsTimestampTzFormat = value;
            else if (parameter == "NLS_LENGTH_SEMANTICS")
                m_dbNlsLengthSemantics = value;
        }
    }

    void Session::AlterSessionNlsParams() {
        if (m_connect.IsOpen() && !m_nlsDateFormat.empty() && m_nlsDateFormat != m_dbNlsDateFormat) {

            std::string buff = "ALTER SESSION SET nls_date_format = \'" + m_nlsDateFormat + "\'";

            ExecuteStatement(buff, true);

            m_dbNlsDateFormat = m_nlsDateFormat;
        }
    }

    std::string Session::GetGlobalName() {
        if (!m_strGlobalName.size()) {
            BuffCursor cursor(m_connect);
            cursor.Prepare("SELECT global_name FROM sys.global_name");
            cursor.Execute();
            cursor.Fetch();
            cursor.GetString(0, m_strGlobalName);
        }
        return m_strGlobalName;
    }

    std::string Session::GetVersionStr()
    {
        if (!m_strVersion.length()) {
            try {
                Statement cursor(m_connect);
                // 20.11.2004 bug fix, some servers may return very long strings due to oracle bug
                StringVar version(1024), compatibility(1024);
                cursor.Prepare("BEGIN dbms_utility.db_version(:version, :compatibility); END;");
                cursor.Bind(":version", version);
                cursor.Bind(":compatibility", compatibility);
                cursor.Execute(1, true);
                version.GetString(m_strVersion);
            }
            catch (const Exception & e) {
                // if server is 7 then use v$version view
                if (e == 6550) {
                    BuffCursor cursor(m_connect);
#if 1
                    cursor.Prepare(
                        "SELECT " // baseed on PRODUCT_COMPONENT_VERSION view which is also not avalable if db is not open
                        "SubStr(banner, InStr(banner,'Release')+8, "
                        "InStr(banner,' - ')-(InStr(banner,'Release')+8)) "
                        "FROM v$version "
                        "WHERE InStr(banner,'Release') > 0 "
                        "AND InStr(banner,'Release') <   InStr(banner,' - ') "
                        "AND Upper(banner) LIKE '%ORACLE%'"
                    );
#else
                    cursor.Prepare("SELECT SubStr(banner, InStr(banner,'.')-1,"
                        "InStr(banner,'.',1,3)-InStr(banner,'.')+1) version "
                        "FROM v$version WHERE Upper(banner) LIKE '%ORACLE%'");
#endif
                    cursor.Execute();
                    cursor.Fetch();
                    cursor.GetString(0, m_strVersion);
                }
                else
                    throw;
            }
        }
        return m_strVersion;
    }

    Connect::ServerVersion Session::GetVersion() {

        Connect::ServerVersion version = m_connect.GetVersion();
        if (version != Connect::ServerVersion::ServerUnknown) {
            return version;
        }

        if (m_strVersion.empty())
            GetVersionStr();

        if (!m_strVersion.empty()) {
            if (!StringUtil::Compare(m_strVersion.c_str(), "11", 2))       version = Connect::ServerVersion::Server11X;
            else if (!StringUtil::Compare(m_strVersion.c_str(), "10", 2))  version = Connect::ServerVersion::Server10X;
            else if (!StringUtil::Compare(m_strVersion.c_str(), "9", 1))   version = Connect::ServerVersion::Server9X;
            else if (!StringUtil::Compare(m_strVersion.c_str(), "8.1", 3)) version = Connect::ServerVersion::Server81X;
            else if (!StringUtil::Compare(m_strVersion.c_str(), "8.0", 3)) version = Connect::ServerVersion::Server80X;
            else if (!StringUtil::Compare(m_strVersion.c_str(), "7.3", 3)) version = Connect::ServerVersion::Server73X;
            else if (!StringUtil::Compare(m_strVersion.c_str(), "7", 1))   version = Connect::ServerVersion::Server7X;

            m_connect.SetVersion(version);
        }

        return version;
    }

    void Session::GetCurrentUserAndSchema(std::string& user, std::string& schema) {

        std::string strSelect;

        BuffCursor cursor(m_connect);

        if (GetVersion() < Connect::ServerVersion::Server81X)
            cursor.Prepare("SELECT USER, USER FROM dual");
        else
            cursor.Prepare("SELECT SYS_CONTEXT('USERENV', 'CURRENT_USER'), SYS_CONTEXT('USERENV', 'CURRENT_SCHEMA') FROM dual");

        cursor.Execute();
        cursor.Fetch();

        user = cursor.ToString(0);
        schema = cursor.ToString(1);
    }

    void Session::RetrieveCurrentSqlInfo()
    {
        m_CurrentSqlAddress.clear();
        m_CurrentSqlHashValue.clear();
        m_CurrentSqlChildNumber.clear();
        m_CurrentSqlID.clear();

        try {
            BuffCursor cursor(m_connect);
            if (GetVersion() < Connect::ServerVersion::Server10X)
                cursor.Prepare("SELECT rawtohex(sql_address) as sql_address, sql_hash_value, rawtohex(prev_sql_addr) as prev_sql_addr, prev_hash_value, sql_id, prev_sql_id, sql_child_number, prev_child_number from v$session where sid = :sid");
            else
                cursor.Prepare("SELECT rawtohex(sql_address) as sql_address, sql_hash_value, rawtohex(prev_sql_addr) as prev_sql_addr, prev_hash_value from v$session where sid = :sid");

            cursor.Bind(":sid", GetSessionSid());
            cursor.ExecuteShadow();

            while (cursor.Fetch()) {
                std::string sql_address, sql_hash_value, prev_sql_addr, prev_hash_value;
                std::string sql_id, prev_sql_id, sql_child_number, prev_child_number;
                cursor.GetString(0, sql_address);
                cursor.GetString(1, sql_hash_value);
                cursor.GetString(2, prev_sql_addr);
                cursor.GetString(3, prev_hash_value);
                if (GetVersion() < Connect::ServerVersion::Server10X) {
                    cursor.GetString(4, sql_id);
                    cursor.GetString(5, prev_sql_id);
                    cursor.GetString(6, sql_child_number);
                    cursor.GetString(7, prev_child_number);
                }

                if (sql_address == "00") {
                    m_CurrentSqlAddress = prev_sql_addr;
                    m_CurrentSqlHashValue = prev_hash_value;
                    if (GetVersion() >= Connect::ServerVersion::Server10X) {
                        m_CurrentSqlID = prev_sql_id;
                        m_CurrentSqlChildNumber = prev_child_number;
                    }
                }
                else {
                    m_CurrentSqlAddress = sql_address;
                    m_CurrentSqlHashValue = sql_hash_value;
                    if (GetVersion() >= Connect::ServerVersion::Server10X)
                    {
                        m_CurrentSqlID = sql_id;
                        m_CurrentSqlChildNumber = sql_child_number;
                    }
                }
            }

            cursor.Close();
        }
        catch (const Exception) {
            //TODO: Inform to UI
            //Global::SetStatusText(string("Error: ") + x.what() + string(" retrieving current sql address from v$session..."), TRUE);
        }

        m_connect.SetSession();
    }

}