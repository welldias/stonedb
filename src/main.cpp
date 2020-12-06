#ifdef _WIN32
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif MACOS
#include <sys/param.h>
#include <sys/sysctl.h>
#else
#include <unistd.h>
#endif

#include <utility>
#include <string>
#include <iostream>
#include <sstream>
#include <thread>

#include "utils/utils.h"
#include "oci20/oci20.h"
#include "data/data.h"
#include "meta_dictionary/meta_dictionary.h"

using namespace Utils;
using namespace Oci20;
using namespace Data;
using namespace MetaDictionary;

void FormatString() {
    std::stringstream ss;

    ss << 4.5 << ", " << 4 << " whatever" << std::endl;

    std::string str = ss.str();
}

unsigned int GetNumCores() {
    
    unsigned int nthreads = std::thread::hardware_concurrency();
    if (nthreads > 0)
        return nthreads;

#ifdef WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#elif MACOS
    int nm[2];
    size_t len = 4;
    uint32_t count;

    nm[0] = CTL_HW; nm[1] = HW_AVAILCPU;
    sysctl(nm, 2, &count, &len, NULL, 0);

    if (count < 1) {
        nm[1] = HW_NCPU;
        sysctl(nm, 2, &count, &len, NULL, 0);
        if (count < 1) { count = 1; }
    }
    return count;
#else
    return sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

//#define LINX_ENV

ListDataProvider* loadDataProvider(ListDataProvider* dataProvider, const std::string& schema) {
    try {
        dataProvider->SetSchema(schema);
        dataProvider->Query();
    }
    catch (const Exception & e) {
        std::cout << e.what() << std::endl;
    }

    return dataProvider;
}

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

    std::string currentUser;
    std::string currentSchema;

    auto startTime = SystemClock::StartCount();
    auto mode = Connect::Mode::Default;
    auto safety = Connect::Safety::None;

#ifdef LINX_ENV
    std::string user = "system";
    std::string password = "passwd";
    std::string tnsAlias = "XEPDB1";
#else
    std::string user = "user";
    std::string password = "password";
    std::string tnsAlias;
    std::string host = "localhost";
    std::string port = "9999";
    std::string sid = "SID";
    bool serviceInsteadOfSid = true;
#endif 

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

    // Common settings
    MetaSettings metaSettings;
    metaSettings.Comments = true;
    metaSettings.Grants = true;
    metaSettings.LowerNames = true;
    metaSettings.SchemaName = false;
    metaSettings.SQLPlusCompatibility = true;
    metaSettings.GeneratePrompts = true;
    // Table Specific
    metaSettings.CommentsAfterColumn = false;
    metaSettings.CommentsPos = 48;
    metaSettings.Constraints = true;
    metaSettings.Indexes = true;
    metaSettings.NoStorageForConstraint = false;
    metaSettings.StorageClause = 1;
    metaSettings.AlwaysPutTablespace = false;
    metaSettings.TableDefinition = true;
    metaSettings.Triggers = true;
    // Othes
    metaSettings.SequnceWithStart = false;
    metaSettings.ViewWithTriggers = true;
    metaSettings.ViewWithForce = false;
    // Hidden
    metaSettings.EndOfShortStatement = ";";
    metaSettings.StorageSubstitutedClause = false;
    metaSettings.AlwaysWriteColumnLengthSematics = false;

    try {
#ifdef LINX_ENV
        ociSession.Open(user, password, tnsAlias, mode, safety);
#else
        ociSession.Open(user, password, host, port, sid, serviceInsteadOfSid, mode, safety);
#endif
    }
    catch (const OciException& e) {
        std::cout << e.what() << std::endl;
    }

    auto serverVersion = ociSession.GetVersion();
    ociSession.GetCurrentUserAndSchema(currentUser, currentSchema);

    Settings::SetSynonymWithoutObjectInvalid(false);
    Settings::SetCurrentDBUser(currentUser);
    Settings::SetCurrentDBSchema(currentSchema);

    ListDataProvider* listaDataprovider[] = {
        new UserListAdapter(ociSession.getConnect()),
        new DbLinkListAdapter(ociSession.getConnect()),
        new GranteeListAdapter(ociSession.getConnect()),
        new ClusterListAdapter(ociSession.getConnect()),
        new CodeListAdapter(ociSession.getConnect(), CodeListAdapter::MonoType::Procedure),
        new CodeListAdapter(ociSession.getConnect(), CodeListAdapter::MonoType::Function),
        new CodeListAdapter(ociSession.getConnect(), CodeListAdapter::MonoType::Java),
        new CodeListAdapter(ociSession.getConnect(), CodeListAdapter::MonoType::Package),
        new CodeListAdapter(ociSession.getConnect(), CodeListAdapter::MonoType::PackageBody),
        new TypeCodeListAdapter(ociSession.getConnect(), TypeCodeListAdapter::MonoType::Type),
        new TypeCodeListAdapter(ociSession.getConnect(), TypeCodeListAdapter::MonoType::Bodies),
        new InvalidObjectListAdapter(ociSession.getConnect()),
        new RecyclebinListAdapter(ociSession.getConnect()),
        new SynonymListAdapter(ociSession.getConnect()),
        new SequenceListAdapter(ociSession.getConnect()),
        new TriggerListAdapter(ociSession.getConnect()),
        new IndexListAdapter(ociSession.getConnect()),
        new TableListAdapter(ociSession.getConnect()),
        new ViewListAdapter(ociSession.getConnect()),
        new ConstraintListAdapter(ociSession.getConnect(), ConstraintListAdapter::Type::PrimaryKey),
        new ConstraintListAdapter(ociSession.getConnect(), ConstraintListAdapter::Type::ForeignKey),
        new ConstraintListAdapter(ociSession.getConnect(), ConstraintListAdapter::Type::UniqueKey),
        new ConstraintListAdapter(ociSession.getConnect(), ConstraintListAdapter::Type::Check),
    };

    std::string colName;
    std::string colValue;
    for (auto & i : listaDataprovider) {
        try {
            auto result = i->Query();    
        } catch (...) {
            std::cout << "Query error : " << std::to_string((int)i->GetInfoType()) << std::endl;
            continue;
        }
        
        for(size_t y=0; y<i->GetColCount(); y++) {
            i->GetColHeader(y, colName);
            std::cout << colName << " | ";
        }
        std::cout << std::endl;

        for(size_t x=0; x<i->GetRowCount(); x++) {
            for(size_t y=0; y<i->GetColCount(); y++) {
                i->GetString(x, y, colValue);
                std::cout << colValue << " | ";
            }
            std::cout << std::endl;
        }
    }

    for (auto & i : listaDataprovider)
        delete std::exchange(i, nullptr);

    ociSession.Close(false);

    auto milliseconds = SystemClock::StopCount(startTime);

    return 0;
}
