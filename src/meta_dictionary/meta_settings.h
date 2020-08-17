#ifndef __PROJECT_STONE_UTILS_METASETTINGS_H__
#define __PROJECT_STONE_UTILS_METASETTINGS_H__

#include <set>
#include <string>

#include "..\utils\utils.h"

namespace MetaDictionary {

    using namespace Utils;

    class MetaSettings {
    public:
        MetaSettings() :
            Comments(&m_comments), 
            Grants(&m_grants), 
            LowerNames(&m_lowerNames), 
            SchemaName(&m_schemaName),
            SQLPlusCompatibility(&m_sqlPlusCompatibility),
            GeneratePrompts(&m_generatePrompts),
            CommentsAfterColumn(&m_commentsAfterColumn),
            CommentsPos(&m_commentsPos),
            Constraints(&m_constraints),
            Indexes(&m_indexes),
            NoStorageForConstraint(&m_noStorageForConstraint),
            StorageClause(&m_storageClause),
            AlwaysPutTablespace(&m_alwaysPutTablespace),
            TableDefinition(&m_tableDefinition),
            Triggers(&m_triggers),
            SequnceWithStart(&m_sequnceWithStart),
            ViewWithTriggers(&m_viewWithTriggers),
            ViewWithForce(&m_viewWithForce),
            EndOfShortStatement(&m_endOfShortStatement),
            StorageSubstitutedClause(&m_storageSubstitutedClause),
            AlwaysWriteColumnLengthSematics(&m_alwaysWriteColumnLengthSematics)
        {};

        bool IsStorageEnabled() const { return (m_storageClause != 0); };
        bool IsStorageAlways()  const { return (m_storageClause == 2); };

        GetSet<bool> Comments;               // table, view & column comments
        GetSet<bool> Grants;                 // grants
        GetSet<bool> LowerNames;             // safe lowercase object names
        GetSet<bool> SchemaName;              // schema as object names prefix
        GetSet<bool> SQLPlusCompatibility;   // view do not have no white space lines, truncate end-line white space 
        GetSet<bool> GeneratePrompts;        // table, view & column comments
        // Table Specific
        GetSet<bool> CommentsAfterColumn;    // column comments as end line remark
        GetSet<int>  CommentsPos;            // end line comments position
        GetSet<bool> Constraints;            // constraints (table writing)
        GetSet<bool> Indexes;                // indexes (table writing)
        GetSet<bool> NoStorageForConstraint; // no storage for primary key & unique key
        GetSet<int> StorageClause;          // table & index storage clause
        GetSet<bool> AlwaysPutTablespace;    // always put tablespace in storage clause (independent of previous flag)
        GetSet<bool> TableDefinition;        // short table definition (table writing)
        GetSet<bool> Triggers;               // table triggers (table writing)
        // Othes
        GetSet<bool> SequnceWithStart;       // sequence with START clause
        GetSet<bool> ViewWithTriggers;       // view triggers (view writing)
        GetSet<bool> ViewWithForce;          // use FORCE for view statement
        // Hidden
        GetSet<std::string> EndOfShortStatement;  // what put after comments and grants 
        GetSet<bool> StorageSubstitutedClause;    // generate Sql*Plus substitution instead of table/index storage

        GetSet<bool> AlwaysWriteColumnLengthSematics;

    private:
        // Common settings
        bool m_comments;               // table, view & column comments
        bool m_grants;                 // grants
        bool m_lowerNames;             // safe lowercase object names
        bool m_schemaName;             // schema as object names prefix
        bool m_sqlPlusCompatibility;   // view do not have no white space lines, truncate end-line white space 
        bool m_generatePrompts;        // table, view & column comments
        // Table Specific
        bool m_commentsAfterColumn;    // column comments as end line remark
        int  m_commentsPos;            // end line comments position
        bool m_constraints;            // constraints (table writing
        bool m_indexes;                // indexes (table writing
        bool m_noStorageForConstraint; // no storage for primary key & unique key
        int  m_storageClause;          // table & index storage clause
        bool m_alwaysPutTablespace;    // always put tablespace in storage clause (independent of previous flag
        bool m_tableDefinition;        // short table definition (table writing
        bool m_triggers;               // table triggers (table writing
        // Othes
        bool m_sequnceWithStart;       // sequence with START clause
        bool m_viewWithTriggers;       // view triggers (view writing
        bool m_viewWithForce;          // use FORCE for view statement
        // Hidden
        std::string m_endOfShortStatement;  // what put after comments and grants 
        bool m_storageSubstitutedClause;    // generate Sql*Plus substitution instead of table/index storage

        bool m_alwaysWriteColumnLengthSematics;
    };
}

#endif // __PROJECT_STONE_UTILS_METASETTINGS_H__