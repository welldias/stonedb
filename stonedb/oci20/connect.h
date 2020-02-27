#ifndef __PROJECT_STONE_OCI20_CONNECT_H__
#define __PROJECT_STONE_OCI20_CONNECT_H__

#include <oci.h>

#include <set>
#include <map>
#include <string>

#include"object.h"
#include "break_handler.h"

namespace Oci20 {

	class Connect {
	public:
		enum class Mode : unsigned int {
			Default = OCI_DEFAULT,
			Migrate = OCI_MIGRATE,
			SysDba = OCI_SYSDBA,
			SysOper = OCI_SYSOPER,
			PreAuth = OCI_PRELIM_AUTH
		};

		enum class Safety {
			None,
			Warnings,
			ReadOnly,
		};

		enum class ServerVersion {
			ServerUnknown,
			Server7X,
			Server73X,
			Server80X,
			Server81X,
			Server9X,
			Server10X,
			Server11X,
			Server112,
			Server12X,
		};

		enum class ClientVersion {
			ClientUnknown,
			Client80X,
			Client81X,
			Client9X,
			Client10X,
			Client11X,
			Client12X,
		};

	private:
		std::string m_uid;
		std::string m_password;
		std::string m_alias;

		ServerVersion m_version;
		std::string m_strHost;
		std::string m_strPort;
		std::string m_strSid;
		bool m_bypassTns;

		std::set<Object*> m_dependencies;

		// contains data with session scope
		std::map<std::string, std::string> m_sessionCookies;

		bool m_open;
		bool m_interrupted;
		bool m_openShadow;
		bool m_ext_auth;
		bool m_isClosing;

		Mode m_mode;
		Safety         m_safety;
		ClientVersion  m_clientVersion;
		Clock64 m_lastExecutionClockTime;

	private:
		OCIEnv*     m_envhp;
		OCIServer*  m_srvhp;
		OCISession* m_authp;
		OCISession* m_auth_shadowp;
		OCISvcCtx*  m_svchp;
		OCIError*   m_errhp;

	public:
		//friend class ErrorUtil;
		Connect(unsigned mode = OCI_THREADED | OCI_DEFAULT | OCI_OBJECT);
		~Connect();
		void Open(const std::string& uid, const std::string& pswd, const std::string& alias, Mode mode, Safety safety);
		void Open(const std::string& uid, const std::string& pswd, const std::string& host, const std::string& port, const std::string& sid, bool serviceInsteadOfSid, Mode mode, Safety safety);
		void Close(bool purge = false);
		void Reconnect();

		std::string CreateTNSString(const std::string& host, const std::string& port, const std::string& sid, bool serviceInsteadOfSid);

	private:
		void DoOpen();
		void ProcAuthenticationInfo(const std::string& uid, const std::string& pswd, const std::string& alias, Mode mode, Safety safety);

		// copy-constraction & assign-operation is not supported
		Connect(const Connect&);
		void operator = (const Connect&);

	public:
		void Commit(bool guaranteedSafe = false);
		void Rollback();
		void Break(bool purge);

		BreakHandler* CreateBreakHandler();

		void CheckShadowSession(const bool bForceConnectShadow = false);
		void SetSession();
		void SetShadowSession();

		bool IsOpen()        const { return m_open; }
		bool IsOpenShadow()  const { return m_openShadow; }
		bool IsClosing()     const { return m_isClosing; }
		bool IsInterrupted() const { return m_interrupted; }

		ServerVersion GetVersion() { return m_version; }
		void SetVersion(ServerVersion version) { m_version = version; }

		std::string   GetUID()           const { return m_uid; }
		std::string   GetPassword()      const { return m_password; }
		std::string   GetAlias()         const { return m_alias; }
		Mode          GetMode()          const { return m_mode; }
		Safety        GetSafety()        const { return m_safety; }
		OCIEnv*       GetOCIEnv()        const { return m_envhp; }
		OCISvcCtx*    GetOCISvcCtx()     const { return m_svchp; }
		OCIError*     GetOCIError()      const { return m_errhp; }
		ClientVersion GetClientVersion() const { return m_clientVersion; }

		Clock64 GetLastExecutionClockTime() const { return m_lastExecutionClockTime; }
		void    SetLastExecutionClockTime() { m_lastExecutionClockTime = SystemClock::StartCount(); }

		void Interrupted(bool interrupted) { m_interrupted = interrupted; }

		void Attach(Object* obj);
		void Detach(Object* obj);

		std::string GetConnectString() const { return GetAlias(); }
		std::string GetDisplayString(bool mode = false) const;

		void SetSessionCookie(const std::string& key, const std::string& val);
		bool GetSessionCookie(const std::string& key, std::string& val) const;
	};
}

#endif // __PROJECT_STONE_OCI20_CONNECT_H__
