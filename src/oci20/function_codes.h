#ifndef __PROJECT_STONE_OCI20_FUNCTION_CODES_H__
#define __PROJECT_STONE_OCI20_FUNCTION_CODES_H__

#include <oci.h>

#include <string>

namespace Oci20 {

    class FunctionNames {
    public:
        enum class Code : ub2 {
            CreateTable = 1,
            SetRole = 2,
            Insert = 3,
            Select = 4,
            Update = 5,
            DropRole = 6,
            DropView = 7,
            DropTable = 8,
            Delete = 9,
            CreateView = 10,
            DropUser = 11,
            CreateRole = 12,
            CreateSequence = 13,
            AlterSequence = 14,
            DropSequence = 16,
            CreateSchema = 17,
            CreateCluster = 18,
            CreateUser = 19,
            CreateIndex = 20,
            DropIndex = 21,
            DropCluster = 22,
            ValidateIndex = 23,
            CreateProcedure = 24,
            AlterProcedure = 25,
            AlterTable = 26,
            Explain = 27,
            Grant = 28,
            Revoke = 29,
            CreateSynonym = 30,
            DropSynonym = 31,
            AlterSystem = 32,
            SetTransaction = 33,
            PlSqlExecute = 34,
            LockTable = 35,
            Rename = 37,
            Comment = 38,
            Audit = 39,
            Noaudit = 40,
            AlterIndex = 41,
            CreateExternalDatabase = 42,
            DropExternalDatabase = 43,
            CreateDatabase = 44,
            AlterDatabase = 45,
            CreateRollbackSegment = 46,
            AlterRollbackSegment = 47,
            DropRollbackSegment = 48,
            CreateTablespace = 49,
            AlterTablespace = 50,
            DropTablespace = 51,
            AlterSession = 52,
            AlterUser = 53,
            Commit = 54,
            Rollback = 55,
            Savepoint = 56,
            CreateControlFile = 57,
            AlterTracing = 58,
            CreateTrigger = 59,
            AlterTrigger = 60,
            DropTrigger = 61,
            AnalyzeTable = 62,
            AnalyzeIndex = 63,
            AnalyzeCluster = 64,
            CreateProfile = 65,
            DropProfile = 66,
            AlterProfile = 67,
            DropProcedure = 68,
            AlterResourceCost = 70,
            CreateSnapshotLog = 71,
            AlterSnapshotLog = 72,
            DropSnapshotLog = 73,
            CreateSnapshot = 74,
            AlterSnapshot = 75,
            DropSnapshot = 76,
            CreateType = 77,
            DropType = 78,
            AlterRole = 79,
            AlterType = 80,
            CreateTypeBody = 81,
            AlterTypeBody = 82,
            DropTypeBody = 83,
            TruncateTable = 85,
            TruncateCluster = 86,
            AlterView = 88,
            CreateFunction = 91,
            AlterFunction = 92,
            DropFunction = 93,
            CreatePackage = 94,
            AlterPackage = 95,
            DropPackage = 96,
            CreatePackageBody = 97,
            AlterPackageBody = 98,
            DropPackageBody = 99,
            Noop = 36,
            DropLibrary = 84,
            CreateBitmapfile = 87,
            DropBitmapfile = 89,
            SetConstraints = 90,
            CreateDirectory = 157,
            DropDirectory = 158,
            CreateLibrary = 159,
            CreateJava = 160,
            AlterJava = 161,
            DropJava = 162,
            CreateOperator = 163,
            CreateIndextype = 164,
            DropIndextype = 165,
            AlterIndextype = 166,
            DropOperator = 167,
            AssociateStatistics = 168,
            DisassociateStatistics = 169,
            CallMethod = 170,
            CreateSummary = 171,
            AlterSummary = 172,
            DropSummary = 173,
            CreateDimension = 174,
            AlterDimension = 175,
            DropDimension = 176,
            CreateContext = 177,
            DropContext = 178,
            AlterOutline = 179,
            CreateOutline = 180,
            DropOutline = 181,
            UpdateIndexes = 182,
            AlterOperator = 183,
            Merge = 189,
        };

        static std::string Get(Code code);
        static std::string Get(ub2 code);

        static bool PossibleCompilationErrors(Code code);
        static bool PossibleCompilationErrors(ub2 code);
        
        private:
            struct FucntionName { Code code; const char* name; };

            static const FucntionName m_fucntionName[];
    };
}

#endif // __PROJECT_STONE_OCI20_FUNCTION_CODES_H__