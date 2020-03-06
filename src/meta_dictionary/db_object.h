#ifndef __PROJECT_STONE_META_DICTIONARY_DBOBJECT_H__
#define __PROJECT_STONE_META_DICTIONARY_DBOBJECT_H__

#include <string>

#include "meta_stream.h"
#include "meta_settings.h"

namespace MetaDictionary {

    class DbObject {

    public:
        DbObject() {};
        virtual ~DbObject() {};

        const std::string& GetOwner() const { return m_owner; }
        const std::string& GetName()  const { return m_name; }

        virtual bool IsCode()      const { return false; }
        virtual bool IsLight()     const { return false; }
        virtual bool IsGrantable() const { return false; }
        virtual bool IsGenerated() const { return false; }

        virtual const std::string& GetDefExt()  const { return "sql"; };
        virtual const std::string& GetTypeStr() const = 0;

        // return code offset for functions,procedures,packages & views
        virtual int  Write(MetaStream& out, const MetaSettings& settings) const = 0;
        
        //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
        //virtual void WriteGrants(MetaStream& out, const MetaSettings& settings) const;

    public:
        std::string m_owner;
        std::string m_name;

    private:
        // copy-constraction & assign-operation is not supported
        DbObject(const DbObject&);
        void operator = (const DbObject&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_DBOBJECT_H__*/