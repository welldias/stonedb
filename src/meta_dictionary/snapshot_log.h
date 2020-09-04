#ifndef __PROJECT_STONE_META_DICTIONARY_SNAPSHOT_LOG_H__
#define __PROJECT_STONE_META_DICTIONARY_SNAPSHOT_LOG_H__

#include <map>

#include "db_object.h"
#include "table_storage.h"

namespace MetaDictionary {

    class SnapshotLog : public DbObject, public TableStorage {

    public:
        SnapshotLog() : DbObject() {};

        virtual const std::string GetTypeStr() const { return "SNAPSHOT LOG"; };
        virtual int Write(MetaStream& out, const MetaSettings& settings) const;

    private:
        std::string m_user;
        std::string m_password;
        std::string m_host;

    private:
        // copy-constraction & assign-operation is not supported
        SnapshotLog(const SnapshotLog&);
        void operator = (const SnapshotLog&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_SNAPSHOT_LOG_H__*/