#ifndef __PROJECT_STONE_UTILS_METASETTINGS_H__
#define __PROJECT_STONE_UTILS_METASETTINGS_H__

#include <set>
#include <string>

#include "..\utils\utils.h"

namespace MetaDictionary {

    using namespace Utils;

    class MetaSettings {
    public:
        MetaSettings() {};

        bool IsStorageEnabled() const { return (m_StorageClause.GetValue() != 0); };
        bool IsStorageAlways()  const { return (m_StorageClause.GetValue() == 2); };

    public:
#define META_PROPERTY(T,N) \
        private:\
        Variable<T> m_##N; \
        public: \
        const T& Get##N () const { return m_##N.GetValue(); }; \
        void  Set##N (const T& value) { m_##N.Set(value); }

        // Common settings
        META_PROPERTY(bool, Comments);               // table, view & column comments
        META_PROPERTY(bool, Grants);                 // grants
        META_PROPERTY(bool, LowerNames);             // safe lowercase object names
        META_PROPERTY(bool, ShemaName);              // schema as object names prefix
        META_PROPERTY(bool, SQLPlusCompatibility);   // view do not have no white space lines, truncate end-line white space 
        META_PROPERTY(bool, GeneratePrompts);        // table, view & column comments
        // Table Specific
        META_PROPERTY(bool, CommentsAfterColumn);    // column comments as end line remark
        META_PROPERTY(int,  CommentsPos);            // end line comments position
        META_PROPERTY(bool, Constraints);            // constraints (table writing)
        META_PROPERTY(bool, Indexes);                // indexes (table writing)
        META_PROPERTY(bool, NoStorageForConstraint); // no storage for primary key & unique key
        META_PROPERTY(int,  StorageClause);          // table & index storage clause
        META_PROPERTY(bool, AlwaysPutTablespace);    // always put tablespace in storage clause (independent of previous flag)
        META_PROPERTY(bool, TableDefinition);        // short table definition (table writing)
        META_PROPERTY(bool, Triggers);               // table triggers (table writing)
        // Othes
        META_PROPERTY(bool, SequnceWithStart);       // sequence with START clause
        META_PROPERTY(bool, ViewWithTriggers);       // view triggers (view writing)
        META_PROPERTY(bool, ViewWithForce);          // use FORCE for view statement
        // Hidden
        META_PROPERTY(std::string, EndOfShortStatement);  // what put after comments and grants 
        META_PROPERTY(bool, StorageSubstitutedClause);    // generate Sql*Plus substitution instead of table/index storage

        META_PROPERTY(bool, AlwaysWriteColumnLengthSematics);
    };
}

#endif // __PROJECT_STONE_UTILS_METASETTINGS_H__