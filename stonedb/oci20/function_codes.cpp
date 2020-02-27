#include <map>

#include "function_codes.h"

namespace Oci20 {

	const FunctionNames::FucntionName FunctionNames::m_fucntionName[] = {
		{ Code::CreateTable ,"Create table",},
		{ Code::SetRole ,"Set role",},
		{ Code::Insert ,"Insert",},
		{ Code::Select ,"Select",},
		{ Code::Update ,"Update",},
		{ Code::Merge ,"Merge",},
		{ Code::DropRole ,"Drop role",},
		{ Code::DropView ,"Drop view",},
		{ Code::DropTable ,"Drop table",},
		{ Code::Delete ,"Delete",},
		{ Code::CreateView ,"Create view",},
		{ Code::DropUser ,"Drop user",},
		{ Code::CreateRole ,"Create role",},
		{ Code::CreateSequence ,"Create sequence",},
		{ Code::AlterSequence ,"Alter sequence",},
		{ Code::DropSequence ,"Drop sequence",},
		{ Code::CreateSchema ,"Create schema",},
		{ Code::CreateCluster ,"Create cluster",},
		{ Code::CreateUser ,"Create user",},
		{ Code::CreateIndex ,"Create index",},
		{ Code::DropIndex ,"Drop index",},
		{ Code::DropCluster ,"Drop cluster",},
		{ Code::ValidateIndex ,"Validate index",},
		{ Code::CreateProcedure ,"Create procedure",},
		{ Code::AlterProcedure ,"Alter procedure",},
		{ Code::AlterTable ,"Alter table",},
		{ Code::Explain ,"Explain",},
		{ Code::Grant ,"Grant",},
		{ Code::Revoke ,"Revoke",},
		{ Code::CreateSynonym ,"Create synonym",},
		{ Code::DropSynonym ,"Drop synonym",},
		{ Code::AlterSystem ,"Alter system",},
		{ Code::SetTransaction ,"Set transaction",},
		{ Code::PlSqlExecute ,"PL/SQL block",},
		{ Code::LockTable ,"Lock table",},
		{ Code::Rename ,"Rename",},
		{ Code::Comment ,"Comment",},
		{ Code::Audit ,"Audit",},
		{ Code::Noaudit ,"Noaudit",},
		{ Code::AlterIndex ,"Alter index",},
		{ Code::CreateExternalDatabase ,"Create external database",},
		{ Code::DropExternalDatabase ,"Drop external database",},
		{ Code::CreateDatabase ,"Create database",},
		{ Code::AlterDatabase ,"Alter database",},
		{ Code::CreateRollbackSegment ,"Create rollback segment",},
		{ Code::AlterRollbackSegment ,"Alter rollback segment",},
		{ Code::DropRollbackSegment ,"Drop rollback segment",},
		{ Code::CreateTablespace ,"Create tablespace",},
		{ Code::AlterTablespace ,"Alter tablespace",},
		{ Code::DropTablespace ,"Drop tablespace",},
		{ Code::AlterSession ,"Alter session",},
		{ Code::AlterUser ,"Alter user",},
		{ Code::Commit ,"Commit",},
		{ Code::Rollback ,"Rollback",},
		{ Code::Savepoint ,"Savepoint",},
		{ Code::CreateControlFile ,"Create control file",},
		{ Code::AlterTracing ,"Alter tracing",},
		{ Code::CreateTrigger ,"Create trigger",},
		{ Code::AlterTrigger ,"Alter trigger",},
		{ Code::DropTrigger ,"Drop trigger",},
		{ Code::AnalyzeTable ,"Analyze table",},
		{ Code::AnalyzeIndex ,"Analyze index",},
		{ Code::AnalyzeCluster ,"Analyze cluster",},
		{ Code::CreateProfile ,"Create profile",},
		{ Code::DropProfile ,"Drop profile",},
		{ Code::AlterProfile ,"Alter profile",},
		{ Code::DropProcedure ,"Drop procedure",},
		{ Code::AlterResourceCost ,"Alter resource cost",},
		{ Code::CreateSnapshotLog ,"Create snapshot log",},
		{ Code::AlterSnapshotLog ,"Alter snapshot log",},
		{ Code::DropSnapshotLog ,"Drop snapshot log",},
		{ Code::CreateSnapshot ,"Create snapshot",},
		{ Code::AlterSnapshot ,"Alter snap shot",},
		{ Code::DropSnapshot ,"Drop snapshot",},
		{ Code::CreateType ,"Alter Role",},
		{ Code::DropType ,"Truncate table",},
		{ Code::AlterRole ,"Truncate cluster",},
		{ Code::AlterType ,"Alter view",},
		{ Code::CreateTypeBody ,"Create function",},
		{ Code::AlterTypeBody ,"Alter function",},
		{ Code::DropTypeBody ,"Drop function",},
		{ Code::TruncateTable ,"Create package",},
		{ Code::TruncateCluster ,"Alter package",},
		{ Code::AlterView ,"Drop package",},
		{ Code::CreateFunction ,"Create package body",},
		{ Code::AlterFunction ,"Alter package body",},
		{ Code::DropFunction ,"Drop package body",},
		{ Code::CreatePackage ,"Create type",},
		{ Code::AlterPackage ,"Alter type",},
		{ Code::DropPackage ,"Drop type",},
		{ Code::CreatePackageBody ,"Create type body",},
		{ Code::AlterPackageBody ,"Alter type body",},
		{ Code::DropPackageBody ,"Drop type body",},
		{ Code::Noop ,"Noop",},
		{ Code::DropLibrary ,"Drop library",},
		{ Code::CreateBitmapfile ,"Create bitmapfile",},
		{ Code::DropBitmapfile ,"Drop bitmapfile",},
		{ Code::SetConstraints ,"Set constraints",},
		{ Code::CreateDirectory ,"Create directory",},
		{ Code::DropDirectory ,"Drop directory",},
		{ Code::CreateLibrary ,"Create library",},
		{ Code::CreateJava ,"Create java",},
		{ Code::AlterJava ,"Alter java",},
		{ Code::DropJava ,"Drop java",},
		{ Code::CreateOperator ,"Create operator",},
		{ Code::CreateIndextype ,"Create indextype",},
		{ Code::DropIndextype ,"Drop indextype",},
		{ Code::AlterIndextype ,"Alter indextype",},
		{ Code::DropOperator ,"Drop operator",},
		{ Code::AssociateStatistics ,"Associate statistics",},
		{ Code::DisassociateStatistics ,"Disassociate statistics",},
		{ Code::CallMethod ,"Call method",},
		{ Code::CreateSummary ,"Create summary",},
		{ Code::AlterSummary ,"Alter summary",},
		{ Code::DropSummary ,"Drop summary",},
		{ Code::CreateDimension ,"Create dimension",},
		{ Code::AlterDimension ,"Alter dimension",},
		{ Code::DropDimension ,"Drop dimension",},
		{ Code::CreateContext ,"Create context",},
		{ Code::DropContext ,"Drop context",},
		{ Code::AlterOutline ,"Alter outline",},
		{ Code::CreateOutline ,"Create outline",},
		{ Code::DropOutline ,"Drop outline",},
		{ Code::UpdateIndexes ,"Update indexes",},
		{ Code::AlterOperator ,"Alter operator",},
	};

	std::string FunctionNames::Get(Code code) {
		
		size_t size = (sizeof(m_fucntionName) / sizeof(m_fucntionName[0]));

		for (size_t i = 0; i < size; i++) {
			if(m_fucntionName[i].code == code)
				return m_fucntionName[i].name;
		}

		return "Statement processed";
	}


	std::string FunctionNames::Get(ub2 code) {
		return Get(static_cast<Code>(code));
	}

	bool FunctionNames::PossibleCompilationErrors(Code code) {
		switch (code) {
		case Code::CreateProcedure:
		case Code::CreateTrigger:
		case Code::CreateFunction:
		case Code::CreatePackage:
		case Code::CreatePackageBody:
		case Code::CreateType:
		case Code::CreateTypeBody:
			return true;
		}
		return false;
	}

	bool FunctionNames::PossibleCompilationErrors(ub2 code) {
		return PossibleCompilationErrors(static_cast<Code>(code));
	}

}