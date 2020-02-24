#ifndef __PROJECT_STONE_OCI20_SESSION_H__
#define __PROJECT_STONE_OCI20_SESSION_H__

#include <string>

#include  "connect.h"

namespace Oci20 {

	class Session {
    public:
        Session();
        ~Session();
    
    private:
        Connect m_connect;
        bool m_databaseOpen;
    public:
        void Open(const std::string& uid, const std::string& pswd, const std::string& alias, Connect::Mode mode, Connect::Safety safety);
        void Open(const std::string& uid, const std::string& pswd, const std::string& host, const std::string& port, const std::string& sid, bool serviceInsteadOfSid, Connect::Mode mode, Connect::Safety safety);
        void Reconnect();
        void Close(bool purge);
        bool IsDatabaseOpen() const { return m_databaseOpen; }

        void ExecuteStatement(const std::string& statementString, bool guaranteedSafe = false);
        void ExecuteShadowStatement(const std::string& statementString, bool guaranteedSafe = false);

    public:
        void CheckShadowSession(const bool bForceConnectShadow = false);
        void SetShadowClientInfo();

    private:
        bool m_OutputEnable;
        unsigned long m_OutputSize;
        bool m_UnlimitedOutputSize;
    public:
        void EnableOutput(bool enable = true, unsigned long size = ULONG_MAX, bool = false);
        void ResetOutput();
        bool IsOutputEnabled() const { return m_OutputEnable; }

    private:
        std::string m_sessionSid;
        bool m_GetSIDFailed;
    public:
        std::string GetSessionSid();
        void GetCurrentUserAndSchema(std::string& user, std::string& schema);

    private:
        std::string m_strGlobalName;
        std::string m_strVersion;
    public:
        std::string   GetGlobalName();
        std::string   GetVersionStr();
        Connect::ServerVersion GetVersion();

    private:
        std::string m_CurrentSqlAddress;
        std::string m_CurrentSqlHashValue;
        std::string m_CurrentSqlChildNumber;
        std::string m_CurrentSqlID;
    public:
        void RetrieveCurrentSqlInfo();
        const std::string& GetCurrentSqlAddress() const { return m_CurrentSqlAddress; }
        const std::string& GetCurrentSqlHashValue() const { return m_CurrentSqlHashValue; }
        const std::string& GetCurrentSqlChildNumber() const { return m_CurrentSqlChildNumber; }
        const std::string& GetCurrentSqlID() const { return m_CurrentSqlID; }

    /******* DataBase NLS parameters *******/
    protected:
        std::string m_nlsLanguage, m_dbNlsLanguage;                   //NLS_LANGUAGE
        std::string m_nlsNumericCharacters, m_dbNlsNumericCharacters; //NLS_NUMERIC_CHARACTERS
        std::string m_nlsDateFormat, m_dbNlsDateFormat;               //NLS_DATE_FORMAT
        std::string m_nlsDateLanguage, m_dbNlsDateLanguage;           //NLS_DATE_LANGUAGE
        std::string m_nlsTimeFormat, m_dbNlsTimeFormat;               //NLS_TIME_FORMAT
        std::string m_nlsTimestampFormat, m_dbNlsTimestampFormat;     //NLS_TIMESTAMP_FORMAT
        std::string m_nlsTimeTzFormat, m_dbNlsTimeTzFormat;           //NLS_TIME_TZ_FORMAT
        std::string m_nlsTimestampTzFormat, m_dbNlsTimestampTzFormat; //NLS_TIMESTAMP_TZ_FORMAT
        std::string m_nlsLengthSemantics, m_dbNlsLengthSemantics;     //NLS_LENGTH_SEMANTICS
    public:
        std::string GetNlsLanguage() { return !m_nlsLanguage.empty() ? m_nlsLanguage : m_dbNlsLanguage; }
        std::string GetNlsNumericCharacters() { return !m_nlsNumericCharacters.empty() ? m_nlsNumericCharacters : m_dbNlsNumericCharacters; }
        std::string GetNlsDateFormat() { return !m_nlsDateFormat.empty() ? m_nlsDateFormat : m_dbNlsDateFormat; }
        std::string GetNlsDateLanguage() { return !m_nlsDateLanguage.empty() ? m_nlsDateLanguage : m_dbNlsDateLanguage; }
        std::string GetNlsTimeFormat() { return !m_nlsTimeFormat.empty() ? m_nlsTimeFormat : m_dbNlsTimeFormat; }
        std::string GetNlsTimestampFormat() { return !m_nlsTimestampFormat.empty() ? m_nlsTimestampFormat : m_dbNlsTimestampFormat; }
        std::string GetNlsTimeTzFormat() { return !m_nlsTimeTzFormat.empty() ? m_nlsTimeTzFormat : m_dbNlsTimeTzFormat; }
        std::string GetNlsTimestampTzFormat() { return !m_nlsTimestampTzFormat.empty() ? m_nlsTimestampTzFormat : m_dbNlsTimestampTzFormat; }
        std::string GetNlsLengthSemantics() { return !m_nlsLengthSemantics.empty() ? m_nlsLengthSemantics : m_dbNlsLengthSemantics; }

        void SetNlsLanguage(const std::string& nlsLanguage) { m_nlsLanguage = nlsLanguage; }
        void SetNlsNumericCharacters(const std::string& nlsNumericCharacters) { m_nlsNumericCharacters = nlsNumericCharacters; }
        void SetNlsDateFormat(const std::string& nlsDateFormat) { m_nlsDateFormat = nlsDateFormat; }
        void SetNlsDateLanguage(const std::string& nlsDateLanguage) { m_nlsDateLanguage = nlsDateLanguage; }
        void SetNlsTimeFormat(const std::string& nlsTimeFormat) { m_nlsTimeFormat = nlsTimeFormat; }
        void SetNlsTimestampFormat(const std::string& nlsTimestampFormat) { m_nlsTimestampFormat = nlsTimestampFormat; }
        void SetNlsTimeTzFormat(const std::string& nlsTimeTzFormat) { m_nlsTimeTzFormat = nlsTimeTzFormat; }
        void SetNlsTimestampTzFormat(const std::string& nlsTimestampTzFormat) { m_nlsTimestampTzFormat = nlsTimestampTzFormat; }
        void SetNlsLengthSemantics(const std::string& nlsLengthSemantics) { m_nlsLengthSemantics = nlsLengthSemantics; }

        void LoadSessionNlsParameters();
        void AlterSessionNlsParams(); // changes session NLS_DATE_FORMAT only

	};
}
#endif // __PROJECT_STONE_OCI20_SESSION_H__

