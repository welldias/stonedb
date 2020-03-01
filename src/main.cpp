#ifdef _WINDOWS
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <string>
#include <iostream>

#include "utils/utils.h"
#include "oci20/oci20.h"
#include "data/data.h"

using namespace Utils;
using namespace Oci20;
using namespace Data;

#ifdef _WINDOWS
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
#else
int main(int argc, char *argv[])
#endif
{
    Session ociSession;

    //ConnectionMode mode = ConnectionMode::Default;
    //Safety safety = Safety::ReadOnly;
    //std::string user = "lasadb01_safe_fai";
    //std::string password = "lasadb01_safe_fai";
    //std::string tnsAlias = "ORA11GD";

    std::string user = "system";
    std::string password = "custonil";
    std::string tnsAlias = "";
    std::string host = "localhost";
    std::string port = "1521";
    std::string sid = "XEPDB1";
    bool serviceInsteadOfSid = true;
    Connect::Mode mode = Connect::Mode::Default;
    Connect::Safety safety = Connect::Safety::None;

    Settings::SetDateFormat("dd.mm.yy");
    Settings::SetAutocommit(false);
    Settings::SetCommitOnDisconnect(2);
    Settings::SetSavePassword(true);
    Settings::SetOutputEnable(true);
    Settings::SetOutputSize(100000);
    Settings::SetSessionStatistics(false);
    Settings::SetSessionStatisticsMode("Auto");
    Settings::SetScanForSubstitution(false);
    Settings::SetPlanTable("PLAN_TABLE");
    Settings::SetCancelQueryDelay(1);
    Settings::SetTopmostCancelQuery(true);
    Settings::SetDbmsXplanDisplayCursor(false);
    Settings::SetWhitespaceLineDelim(true);
    Settings::SetEmptyLineDelim(true);
    Settings::SetUnlimitedOutputSize(false);
    Settings::SetExternalToolCommand("");
    Settings::SetExternalToolParameters("");

    /* new properties */
    Settings::SetTimestampSupported(true);
    Settings::SetIntervalToTextSupported(true);

    try {
        //ociSession.Open(user, password, tnsAlias, mode, safety);
        ociSession.Open(user, password, host, port, sid, serviceInsteadOfSid, mode, safety);
        ociSession.GetVersion();
    }
    catch (const OciException& e) {
        std::cout << e.what() << std::endl;
    }

    TableListAdapter tableList(ociSession.getConnect());
    if (ociSession.IsDatabaseOpen()) {
        try {
            tableList.Query();
        }
        catch (const Exception & e) {
            std::cout << e.what() << std::endl;
        }
    }

    ViewListAdapter viewList(ociSession.getConnect());
    if (ociSession.IsDatabaseOpen()) {
        try {
            viewList.Query();
        }
        catch (const Exception & e) {
            std::cout << e.what() << std::endl;
        }
    }

    ConstraintListAdapter pkListAdapter(ociSession.getConnect(), "P");
    if (ociSession.IsDatabaseOpen()) {
        try {
            pkListAdapter.Query();
        }
        catch (const Exception & e) {
            std::cout << e.what() << std::endl;
        }
    }

    ConstraintListAdapter fkListAdapter(ociSession.getConnect(), "R");
    if (ociSession.IsDatabaseOpen()) {
        try {
            fkListAdapter.Query();
        }
        catch (const Exception & e) {
            std::cout << e.what() << std::endl;
        }
    }

    ConstraintListAdapter ukListAdapter(ociSession.getConnect(), "U");
    if (ociSession.IsDatabaseOpen()) {
        try {
            ukListAdapter.Query();
        }
        catch (const Exception & e) {
            std::cout << e.what() << std::endl;
        }
    }

    ConstraintListAdapter chkListAdapter(ociSession.getConnect(), "C");
    if (ociSession.IsDatabaseOpen()) {
        try {
            chkListAdapter.Query();
        }
        catch (const Exception & e) {
            std::cout << e.what() << std::endl;
        }
    }

    ociSession.Close(false);

    return 0;
}
