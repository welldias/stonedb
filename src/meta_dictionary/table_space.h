#ifndef __PROJECT_STONE_META_DICTIONARY_TABLESPACE_H__
#define __PROJECT_STONE_META_DICTIONARY_TABLESPACE_H__

#include <string>

#include "db_object.h"
#include "storage.h"
#include "meta_settings.h"

namespace MetaDictionary {

    using namespace Utils;

    class Tablespace : public DbObject, public Storage {
    public:
        Tablespace() {};

        virtual const std::string GetTypeStr() const { return "TABLESPACE"; };
        virtual int Write(MetaStream&, const MetaSettings&) const { return 0; };

    private:
        // copy-constraction & assign-operation is not supported
        Tablespace(const Tablespace&);
        void operator = (const Tablespace&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_TABLESPACE_H__*/