#ifndef __PROJECT_STONE_META_FUNCTION_VIEW_H__
#define __PROJECT_STONE_META_FUNCTION_VIEW_H__

#include <map>

#include "db_object.h"

namespace MetaDictionary {

    class PlsCode : public DbObject {
    public:
        PlsCode() {};

        virtual bool IsGrantable() const { return true; }
        virtual bool IsCode() const { return true; };
        virtual const std::string GetTypeStr() const { return "sql"; };
        virtual int Write(MetaStream& out, const MetaSettings& settings) const;

    public:
        std::string m_type;
        std::string m_text;

    private:
        // copy-constraction & assign-operation is not supported
        PlsCode(const PlsCode&);
        void operator = (const PlsCode&);
    };

    class Function : public PlsCode {
    public:
        Function() : PlsCode() {};

        virtual const std::string GetTypeStr() const { return "FUNCTION"; };
        virtual const std::string GetDefExt()  const { return "pls"; };

    private:
        // copy-constraction & assign-operation is not supported
        Function(const Function&);
        void operator = (const Function&);
    };
}

#endif/*__PROJECT_STONE_META_FUNCTION_VIEW_H__*/
