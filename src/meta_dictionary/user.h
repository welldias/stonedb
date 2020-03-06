#ifndef __PROJECT_STONE_META_DICTIONARY_USER_H__
#define __PROJECT_STONE_META_DICTIONARY_USER_H__

#include <string>

#include "db_object.h"

namespace MetaDictionary {

    class User : public DbObject {
    protected:
        std::string m_defTablespace;

    public:
        User() : DbObject(){};
        virtual ~User() {};

        virtual std::string GetTypeStr() const { return "USER"; };
        virtual int Write(MetaStream&, const MetaSettings&) const { return 0; };

    private:
        // copy-constraction & assign-operation is not supported
        User(const User&);
        void operator = (const User&);
    };
}

#endif /*__PROJECT_STONE_META_DICTIONARY_USER_H__*/