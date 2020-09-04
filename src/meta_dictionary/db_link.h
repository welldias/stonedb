#ifndef __PROJECT_STONE_META_DICTIONARY_DB_LINK_H__
#define __PROJECT_STONE_META_DICTIONARY_DB_LINK_H__

#include <map>

#include "db_object.h"

namespace MetaDictionary {

    class DbLink : public DbObject {

    public:
        DbLink() : DbObject() {};

        virtual bool IsLight()     const { return true; };
        virtual const std::string GetTypeStr() const { return "DATABASE LINK"; };
        virtual int Write(MetaStream& out, const MetaSettings& settings) const;

    private:
        std::string m_user;
        std::string m_password;
        std::string m_host;

    private:
        // copy-constraction & assign-operation is not supported
        DbLink(const DbLink&);
        void operator = (const DbLink&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_DB_LINK_H__*/