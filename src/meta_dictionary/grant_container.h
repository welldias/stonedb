#ifndef __PROJECT_STONE_META_DICTIONARY_GRANTCONTAINER_H__
#define __PROJECT_STONE_META_DICTIONARY_GRANTCONTAINER_H__

#include <map>
#include <set>
#include <string>
#include <functional>

#include "db_object.h"

#include "grant.h"
#include "meta_stream.h"
#include "meta_settings.h"

namespace MetaDictionary {

    using GrantMap = std::map<std::string, Grant*>;
    using GrantVector = std::vector<const Grant*>;
    using FuncEnum = std::function<void(const Grant&)>;

    class GrantContainer : public DbObject {
    public:
        GrantContainer() : DbObject() {};

        // If it's grantor then the arguments are (grantee_name, object_name)
        // If it's grantee then the arguments are (grantor_name, object_name)
        Grant& CreateGrant(const std::string& grantxx, const std::string& object);
        Grant& LookupGrant(const std::string&, const std::string&);
        void   DestroyGrant(const std::string&, const std::string&);

        size_t GetGrants(GrantVector& grantVector, const std::string& name, bool sort = false) const;
        void EnumGrants(FuncEnum funcEnum, bool sort = false) const;

        virtual int Write(MetaStream& out, const MetaSettings& settings) const;

    protected:
        GrantMap m_mapGrants;

    private:
        // copy-constraction & assign-operation is not supported
        GrantContainer(const GrantContainer&);
        void operator = (const GrantContainer&);
    };

    class Grantor : public GrantContainer {
    public:
        Grantor() {}
        virtual const std::string GetTypeStr() const { return "GRANTOR"; };
        void WriteObjectGrants(const std::string& name, MetaStream& out, const MetaSettings& settings) const;
    };

    class Grantee : public GrantContainer {
    public:
        Grantee() {}
        virtual const std::string GetTypeStr() const { return "GRANTEE"; };
    };

}

#endif/*__PROJECT_STONE_META_DICTIONARY_GRANT_H__*/