#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <iostream>

#include "connect.h"
#include "exception.h"
#include "settings.h"

using namespace Stone;
using namespace Utils;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    Oci20::Connect connect;
    Oci20::ConnectionMode mode = Oci20::ConnectionMode::Default;
    Oci20::Safety safety = Oci20::Safety::ReadOnly;

    std::string user = "lasadb01_safe_fai";
    std::string password = "lasadb01_safe_fai";
    std::string tnsAlias = "ORA11GD";

    Settings::GetInstance().SetDateFormat("dd.mm.yy");
    Settings::GetInstance().SetAutocommit(false);
    Settings::GetInstance().SetCommitOnDisconnect(2);
    Settings::GetInstance().SetSavePassword(true);
    Settings::GetInstance().SetOutputEnable(true);
    Settings::GetInstance().SetOutputSize(100000);
    Settings::GetInstance().SetSessionStatistics(false);
    Settings::GetInstance().SetSessionStatisticsMode("Auto");
    Settings::GetInstance().SetScanForSubstitution(false);
    Settings::GetInstance().SetPlanTable("PLAN_TABLE");
    Settings::GetInstance().SetCancelQueryDelay(1);
    Settings::GetInstance().SetTopmostCancelQuery(true);
    Settings::GetInstance().SetDbmsXplanDisplayCursor(false);
    Settings::GetInstance().SetWhitespaceLineDelim(true);
    Settings::GetInstance().SetEmptyLineDelim(true);
    Settings::GetInstance().SetUnlimitedOutputSize(false);
    Settings::GetInstance().SetExternalToolCommand("");
    Settings::GetInstance().SetExternalToolParameters("");

    /* new properties */
    Settings::GetInstance().SetTimestampSupported(true);
    Settings::GetInstance().SetIntervalToTextSupported(true);

    try {
		connect.Open(user, password, tnsAlias, mode, safety);
    }
    catch (const Exception& e) {
        std::cout << e.what() << std::endl;
    }


    connect.Close();

    return 0;
}
