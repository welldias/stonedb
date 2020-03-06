#ifndef __PROJECT_STONE_META_DICTIONARY_GRANT_H__
#define __PROJECT_STONE_META_DICTIONARY_GRANT_H__

#include <map>
#include <set>
#include <string>

#include "meta_stream.h"
#include "meta_settings.h"

namespace MetaDictionary {

    struct Grant
    {
        std::string m_owner;
        std::string m_name;
        std::string m_grantee;

        std::set<std::string> m_privileges;
        std::set<std::string> m_privilegesWithAdminOption;

        std::map<std::string, std::set<std::string> > m_collectionPrivileges;
        std::map<std::string, std::set<std::string> > m_collectionPrivilegesWithAdminOption;

        Grant() {}

        virtual int Write(MetaStream& out, const MetaSettings& settings) const;

    private:
        // copy-constraction & assign-operation is not supported
        Grant(const Grant&);
        void operator = (const Grant&);
    };

}

#endif/*__PROJECT_STONE_META_DICTIONARY_GRANT_H__*/