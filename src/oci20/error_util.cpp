#include <oci.h>

#include <cstring>

#include "error_util.h"

#include "../utils/utils.h"

namespace Oci20 {

	using namespace Utils;

	void ErrorUtil::CheckErrorAndThrow(sword status, Connect& connect, const std::string& exceptionMsg) {

		if (status == OCI_SUCCESS)
			return;

		int errcode = 0;
		oratext message[1024];
		memset(message, 0, sizeof(message));

		OCIErrorGet(connect.GetOCIError(), 1, 0, &errcode, message, sizeof(message), OCI_HTYPE_ERROR);

		std::string strMessage((char*)message);

		bool bIsRemote = (strMessage.find("ORA-02063") != std::string::npos);

		strMessage = "Oci20: " + strMessage;


		switch (errcode) {
		case 22: // invalid session ID
			if (!bIsRemote && !connect.IsClosing() && connect.IsOpen())
				connect.Close(true); // connect losed, not logged on
			break;
		case 28: // your session has been killed
		case 41: // RESOURCE MANAGER: active time limit exceeded
		case 1012: // not logged on
		case 3113: // end-of-file communication channel
		case 3114: // not connected to Oracle
		case 2396: // exceeded maximum idle time
		case 2397: // exceeded PRIVATE_SGA limit
		case 2399: // exceeded maximum connect time
			if (!bIsRemote && connect.IsOpen())
				connect.Close(true); // connect losed, not logged on
			break;
		}

		switch (errcode) {
		default:
			if (exceptionMsg.length() > 0) {
				strMessage += "\n";
				strMessage += exceptionMsg;
			} 
			throw OciException(errcode, strMessage);

		case 1013:
			// 26.10.2003 bug fix, any sql statement which is executed after cancelation will be also canceled
			connect.Interrupted(false);
			throw UserCancel(errcode, strMessage);
		case 1406: // ORA-01406: fetched column value was truncated
			break;
		case 28002: // ORA-28002: the password will expire within <x> days
			// if encountered while connection is open, report it as usual
			if (connect.IsOpen()
				&& (connect.IsOpenShadow() || !(SETTINGS_GET_BOOL(DbmsXplanDisplayCursor) || SETTINGS_GET_BOOL(SessionStatistics))))
				throw OciException(errcode, strMessage);
			// if encountered while opening the connection, report it but continue
			else {
				if (!connect.IsOpen())
					throw OciException(errcode, strMessage);
				//TODO: Send  Status Text
			}
			break;
		}
	}

	void ErrorUtil::CheckAllocErrorAndThrow(sword status) {
		if (status == OCI_SUCCESS)
			return;

		throw OciException(status, "Oci20: Handle alloc error");
	}

	void ErrorUtil::CheckInterruptAndThrow(Connect& connect) {
		if (!connect.IsInterrupted())
			return;

		connect.Interrupted(false);
		throw UserCancel(0, "Oci20: user requested cancel of current operation");
	}

}