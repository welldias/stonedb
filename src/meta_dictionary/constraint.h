#ifndef __PROJECT_STONE_META_DICTIONARY_CONSTRAINT_H__
#define __PROJECT_STONE_META_DICTIONARY_CONSTRAINT_H__

#include <map>

#include "db_object.h"
#include "index_storage.h"
#include "table_storage.h"
#include "partition.h"
#include "index_partition.h"
#include "meta_stream.h"
#include "meta_settings.h"

namespace MetaDictionary {

    class Constraint : public DbObject {
    public:
        Constraint();

        bool IsNotNullCheck() const;

        virtual const std::string GetTypeStr()  const { return std::string("CONSTRAINT"); };
        virtual bool IsGenerated() const { return m_generated; }
        virtual int Write(MetaStream& out, const MetaSettings& settings) const;

        void WritePrimaryKeyForIOT(MetaStream& out, const MetaSettings& settings) const;

        char GetType() const { return m_type[0]; };
        const std::string& GetSearchCondition() const { return m_searchCondition; };
        const std::map<int, std::string>& GetColumns() const { return m_columns; };

    private:
        std::string m_tableName;
        std::string m_type;
        std::string m_rOwner;
        std::string m_rConstraintName;
        std::string m_deleteRule;
        std::string m_status;
        std::string m_searchCondition;

        std::map<int, std::string> m_columns;

        bool m_deferrable;
        bool m_deferred;
        bool m_generated;
        bool m_onView;
        bool m_rely;
        bool m_noValidate;
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_CONSTRAINT_H__*/