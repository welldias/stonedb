#include <map>

#include "function_codes.h"

namespace Oci20 {

	struct {
		FunctionCode code;
		const char* name;
	} static FUNCTION_CODE_NAME[] = {
		{ FunctionCode::CreateTable ,"Create table",},
		{ FunctionCode::SetRole ,"Set role",},
		{ FunctionCode::Insert ,"Insert",},
		{ FunctionCode::Select ,"Select",},
		{ FunctionCode::Update ,"Update",},
		{ FunctionCode::Merge ,"Merge",},
		{ FunctionCode::DropRole ,"Drop role",},
		{ FunctionCode::DropView ,"Drop view",},
		{ FunctionCode::DropTable ,"Drop table",},
		{ FunctionCode::Delete ,"Delete",},
		{ FunctionCode::CreateView ,"Create view",},
		{ FunctionCode::DropUser ,"Drop user",},
		{ FunctionCode::CreateRole ,"Create role",},
		{ FunctionCode::CreateSequence ,"Create sequence",},
		{ FunctionCode::AlterSequence ,"Alter sequence",},
		{ FunctionCode::DropSequence ,"Drop sequence",},
		{ FunctionCode::CreateSchema ,"Create schema",},
		{ FunctionCode::CreateCluster ,"Create cluster",},
		{ FunctionCode::CreateUser ,"Create user",},
		{ FunctionCode::CreateIndex ,"Create index",},
		{ FunctionCode::DropIndex ,"Drop index",},
		{ FunctionCode::DropCluster ,"Drop cluster",},
		{ FunctionCode::ValidateIndex ,"Validate index",},
		{ FunctionCode::CreateProcedure ,"Create procedure",},
		{ FunctionCode::AlterProcedure ,"Alter procedure",},
		{ FunctionCode::AlterTable ,"Alter table",},
		{ FunctionCode::Explain ,"Explain",},
		{ FunctionCode::Grant ,"Grant",},
		{ FunctionCode::Revoke ,"Revoke",},
		{ FunctionCode::CreateSynonym ,"Create synonym",},
		{ FunctionCode::DropSynonym ,"Drop synonym",},
		{ FunctionCode::AlterSystem ,"Alter system",},
		{ FunctionCode::SetTransaction ,"Set transaction",},
		{ FunctionCode::PlSqlExecute ,"PL/SQL block",},
		{ FunctionCode::LockTable ,"Lock table",},
		{ FunctionCode::Rename ,"Rename",},
		{ FunctionCode::Comment ,"Comment",},
		{ FunctionCode::Audit ,"Audit",},
		{ FunctionCode::Noaudit ,"Noaudit",},
		{ FunctionCode::AlterIndex ,"Alter index",},
		{ FunctionCode::CreateExternalDatabase ,"Create external database",},
		{ FunctionCode::DropExternalDatabase ,"Drop external database",},
		{ FunctionCode::CreateDatabase ,"Create database",},
		{ FunctionCode::AlterDatabase ,"Alter database",},
		{ FunctionCode::CreateRollbackSegment ,"Create rollback segment",},
		{ FunctionCode::AlterRollbackSegment ,"Alter rollback segment",},
		{ FunctionCode::DropRollbackSegment ,"Drop rollback segment",},
		{ FunctionCode::CreateTablespace ,"Create tablespace",},
		{ FunctionCode::AlterTablespace ,"Alter tablespace",},
		{ FunctionCode::DropTablespace ,"Drop tablespace",},
		{ FunctionCode::AlterSession ,"Alter session",},
		{ FunctionCode::AlterUser ,"Alter user",},
		{ FunctionCode::Commit ,"Commit",},
		{ FunctionCode::Rollback ,"Rollback",},
		{ FunctionCode::Savepoint ,"Savepoint",},
		{ FunctionCode::CreateControlFile ,"Create control file",},
		{ FunctionCode::AlterTracing ,"Alter tracing",},
		{ FunctionCode::CreateTrigger ,"Create trigger",},
		{ FunctionCode::AlterTrigger ,"Alter trigger",},
		{ FunctionCode::DropTrigger ,"Drop trigger",},
		{ FunctionCode::AnalyzeTable ,"Analyze table",},
		{ FunctionCode::AnalyzeIndex ,"Analyze index",},
		{ FunctionCode::AnalyzeCluster ,"Analyze cluster",},
		{ FunctionCode::CreateProfile ,"Create profile",},
		{ FunctionCode::DropProfile ,"Drop profile",},
		{ FunctionCode::AlterProfile ,"Alter profile",},
		{ FunctionCode::DropProcedure ,"Drop procedure",},
		{ FunctionCode::AlterResourceCost ,"Alter resource cost",},
		{ FunctionCode::CreateSnapshotLog ,"Create snapshot log",},
		{ FunctionCode::AlterSnapshotLog ,"Alter snapshot log",},
		{ FunctionCode::DropSnapshotLog ,"Drop snapshot log",},
		{ FunctionCode::CreateSnapshot ,"Create snapshot",},
		{ FunctionCode::AlterSnapshot ,"Alter snap shot",},
		{ FunctionCode::DropSnapshot ,"Drop snapshot",},
		{ FunctionCode::CreateType ,"Alter Role",},
		{ FunctionCode::DropType ,"Truncate table",},
		{ FunctionCode::AlterRole ,"Truncate cluster",},
		{ FunctionCode::AlterType ,"Alter view",},
		{ FunctionCode::CreateTypeBody ,"Create function",},
		{ FunctionCode::AlterTypeBody ,"Alter function",},
		{ FunctionCode::DropTypeBody ,"Drop function",},
		{ FunctionCode::TruncateTable ,"Create package",},
		{ FunctionCode::TruncateCluster ,"Alter package",},
		{ FunctionCode::AlterView ,"Drop package",},
		{ FunctionCode::CreateFunction ,"Create package body",},
		{ FunctionCode::AlterFunction ,"Alter package body",},
		{ FunctionCode::DropFunction ,"Drop package body",},
		{ FunctionCode::CreatePackage ,"Create type",},
		{ FunctionCode::AlterPackage ,"Alter type",},
		{ FunctionCode::DropPackage ,"Drop type",},
		{ FunctionCode::CreatePackageBody ,"Create type body",},
		{ FunctionCode::AlterPackageBody ,"Alter type body",},
		{ FunctionCode::DropPackageBody ,"Drop type body",},
		{ FunctionCode::Noop ,"Noop",},
		{ FunctionCode::DropLibrary ,"Drop library",},
		{ FunctionCode::CreateBitmapfile ,"Create bitmapfile",},
		{ FunctionCode::DropBitmapfile ,"Drop bitmapfile",},
		{ FunctionCode::SetConstraints ,"Set constraints",},
		{ FunctionCode::CreateDirectory ,"Create directory",},
		{ FunctionCode::DropDirectory ,"Drop directory",},
		{ FunctionCode::CreateLibrary ,"Create library",},
		{ FunctionCode::CreateJava ,"Create java",},
		{ FunctionCode::AlterJava ,"Alter java",},
		{ FunctionCode::DropJava ,"Drop java",},
		{ FunctionCode::CreateOperator ,"Create operator",},
		{ FunctionCode::CreateIndextype ,"Create indextype",},
		{ FunctionCode::DropIndextype ,"Drop indextype",},
		{ FunctionCode::AlterIndextype ,"Alter indextype",},
		{ FunctionCode::DropOperator ,"Drop operator",},
		{ FunctionCode::AssociateStatistics ,"Associate statistics",},
		{ FunctionCode::DisassociateStatistics ,"Disassociate statistics",},
		{ FunctionCode::CallMethod ,"Call method",},
		{ FunctionCode::CreateSummary ,"Create summary",},
		{ FunctionCode::AlterSummary ,"Alter summary",},
		{ FunctionCode::DropSummary ,"Drop summary",},
		{ FunctionCode::CreateDimension ,"Create dimension",},
		{ FunctionCode::AlterDimension ,"Alter dimension",},
		{ FunctionCode::DropDimension ,"Drop dimension",},
		{ FunctionCode::CreateContext ,"Create context",},
		{ FunctionCode::DropContext ,"Drop context",},
		{ FunctionCode::AlterOutline ,"Alter outline",},
		{ FunctionCode::CreateOutline ,"Create outline",},
		{ FunctionCode::DropOutline ,"Drop outline",},
		{ FunctionCode::UpdateIndexes ,"Update indexes",},
		{ FunctionCode::AlterOperator ,"Alter operator",},
	};

	std::string FunctionNames::Get(FunctionCode code) {
		
		size_t size = (sizeof(FUNCTION_CODE_NAME) / sizeof(FUNCTION_CODE_NAME[0]));

		for (size_t i = 0; i < size; i++) {
			if(FUNCTION_CODE_NAME[i].code == code)
				return FUNCTION_CODE_NAME[i].name;
		}

		return "Statement processed";
	}


	std::string FunctionNames::Get(ub2 code) {
		return Get(static_cast<FunctionCode>(code));
	}

	bool FunctionNames::PossibleCompilationErrors(FunctionCode code) {
		switch (code) {
		case FunctionCode::CreateProcedure:
		case FunctionCode::CreateTrigger:
		case FunctionCode::CreateFunction:
		case FunctionCode::CreatePackage:
		case FunctionCode::CreatePackageBody:
		case FunctionCode::CreateType:
		case FunctionCode::CreateTypeBody:
			return true;
		}
		return false;
	}

	bool FunctionNames::PossibleCompilationErrors(ub2 code) {
		return PossibleCompilationErrors(static_cast<FunctionCode>(code));
	}

}