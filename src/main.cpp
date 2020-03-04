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

int loadDataProvider(ListDataProvider& dataProvider, const std::string& schema) {
    try {
        dataProvider.SetSchema(schema);
        dataProvider.Query();
    }
    catch (const Exception & e) {
        std::cout << e.what() << std::endl;
    }

    return dataProvider.GetRowCount();
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

    Connect::Mode mode = Connect::Mode::Default;
    Connect::Safety safety = Connect::Safety::None;

#ifdef LINX_ENV
    std::string user = "lasadb01_safe_fai";
    std::string password = "lasadb01_safe_fai";
    std::string tnsAlias = "ORA11GD";
#else
    std::string user = "system";
    std::string password = "custonil";
    std::string tnsAlias = "";
    std::string host = "192.168.15.2";
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

    Connect::ServerVersion serverVersion = ociSession.GetVersion();
    ociSession.GetCurrentUserAndSchema(currentUser, currentSchema);

    Settings::SetSynonymWithoutObjectInvalid(false);
    Settings::SetCurrentDBUser(currentUser);
    Settings::SetCurrentDBSchema(currentSchema);

#if 0

    // Create the thread pool with the initial number of threads (2 here).
    Task::Module::init(4);

    // Create a task manager with two worker.
    auto manager = Task::Module::makeManager(2);

    std::vector<std::future<int>> futureList;

    ViewListAdapter viewList(ociSession.getConnect());
    //loadDataProvider(viewList, currentSchema);
    futureList.push_back(manager.get()->push(loadDataProvider, viewList, currentSchema));
    
    
    //int ddd = future.get();

    std::chrono::milliseconds span(50);
    while (futureList[0].wait_for(span) == std::future_status::timeout)
        std::cout << '.' << std::flush;

    // Add a new task and get its future.
    //auto future = manager.get()->push([] { return 42; });

    // Get the result from the future and print it.
    //std::cout << future.get() << std::endl; // Prints 42

    // Not necessary here, but the stop method ensures that all launched tasks have been executed.
    manager.get()->stop().get();
#endif

    UserListAdapter userList(ociSession.getConnect());
    loadDataProvider(userList, "");

    DbLinkListAdapter dblinkList(ociSession.getConnect());
    loadDataProvider(dblinkList, currentSchema);

    GranteeListAdapter granteeList(ociSession.getConnect());
    loadDataProvider(granteeList, currentSchema);

    ClusterListAdapter clusterList(ociSession.getConnect());
    loadDataProvider(clusterList, currentSchema);

    CodeListAdapter procedureList(ociSession.getConnect(), CodeListAdapter::MonoType::Procedure);
    loadDataProvider(procedureList, currentSchema);

    CodeListAdapter functionList(ociSession.getConnect(), CodeListAdapter::MonoType::Function);
    loadDataProvider(functionList, currentSchema);

    CodeListAdapter javaList(ociSession.getConnect(), CodeListAdapter::MonoType::Java);
    loadDataProvider(javaList, currentSchema);

    CodeListAdapter packageList(ociSession.getConnect(), CodeListAdapter::MonoType::Package);
    loadDataProvider(packageList, currentSchema);

    CodeListAdapter packageBodyList(ociSession.getConnect(), CodeListAdapter::MonoType::PackageBody);
    loadDataProvider(packageBodyList, currentSchema);

    TypeCodeListAdapter typecodeList(ociSession.getConnect(), TypeCodeListAdapter::MonoType::Type);
    loadDataProvider(typecodeList, currentSchema);

    TypeCodeListAdapter typecodeBodiesList(ociSession.getConnect(), TypeCodeListAdapter::MonoType::Bodies);
    loadDataProvider(typecodeBodiesList, currentSchema);

    InvalidObjectListAdapter invalidobjectList(ociSession.getConnect());
    loadDataProvider(invalidobjectList, currentSchema);

    RecyclebinListAdapter recyclebinList(ociSession.getConnect());
    loadDataProvider(recyclebinList, currentSchema);

    SynonymListAdapter synonymList(ociSession.getConnect());
    loadDataProvider(synonymList, currentSchema);

    SequenceListAdapter sequenceList(ociSession.getConnect());
    loadDataProvider(sequenceList, currentSchema);

    TriggerListAdapter triggerList(ociSession.getConnect());
    loadDataProvider(triggerList, currentSchema);

    IndexListAdapter indexList(ociSession.getConnect());
    loadDataProvider(indexList, currentSchema);

    TableListAdapter tableList(ociSession.getConnect());
    loadDataProvider(tableList, currentSchema);

    ViewListAdapter viewList(ociSession.getConnect());
    loadDataProvider(viewList, currentSchema);

    ConstraintListAdapter pkListAdapter(ociSession.getConnect(), ConstraintListAdapter::Type::PrimaryKey);
    loadDataProvider(pkListAdapter, currentSchema);

    ConstraintListAdapter fkListAdapter(ociSession.getConnect(), ConstraintListAdapter::Type::ForeignKey);
    loadDataProvider(fkListAdapter, currentSchema);

    ConstraintListAdapter ukListAdapter(ociSession.getConnect(), ConstraintListAdapter::Type::UniqueKey);
    loadDataProvider(ukListAdapter, currentSchema);

    ConstraintListAdapter chkListAdapter(ociSession.getConnect(), ConstraintListAdapter::Type::Check);
    loadDataProvider(chkListAdapter, currentSchema);

    ociSession.Close(false);

    return 0;
}
