#ifdef _WINDOWS
#include <SDKDDKVer.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <utility>
#include <string>
#include <iostream>

#include "utils/utils.h"
#include "oci20/oci20.h"
#include "data/data.h"

#include "task/manager.h"
#include "task/thread_pool.h"
#include "task/module.h"
#include "task/priority_queue.h"
#include "task/scheduler.h"
#include "task/task.h"

using namespace Utils;
using namespace Oci20;
using namespace Data;

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

    auto mode = Connect::Mode::Default;
    auto safety = Connect::Safety::None;

#ifdef LINX_ENV
    std::string user = "lasadb01_safe_fai";
    std::string password = "lasadb01_safe_fai";
    std::string tnsAlias = "ORA11GD";
#else
    std::string user = "system";
    std::string password = "custonil";
    std::string tnsAlias = "";
    std::string host = "localhost";
    std::string port = "1521";
    std::string sid = "XEPDB1";
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

    Clock64 startTime = SystemClock::StartCount();

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


    Task::Module::init(2);
    std::vector<std::future<ListDataProvider*>> futureList;
    auto manager = Task::Module::makeManager(1);

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

    for (int i = 0; i < (sizeof(listaDataprovider) / sizeof(ListDataProvider*)); i++) {
        futureList.push_back(manager.get()->push(loadDataProvider, listaDataprovider[i], currentSchema));
    }

    std::chrono::milliseconds span(100);
    std::vector<std::future<ListDataProvider*>>::iterator it;

    while (!futureList.empty()) {
        it = futureList.begin();
        while (it != futureList.end()) {
            if ((*it).wait_for(span) == std::future_status::ready) {
                auto infoType = (*it).get();
                it = futureList.erase(it);
            }
        }
    }

    manager.get()->stop().get();

    for (int i = 0; i < (sizeof(listaDataprovider) / sizeof(ListDataProvider*)); i++)
        delete std::exchange(listaDataprovider[i], nullptr);

    ociSession.Close(false);

    auto milliseconds = SystemClock::StopCount(startTime);

    return 0;
}
