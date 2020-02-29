#ifndef __PROJECT_STONE_OCI20_STRINGVAR_H__
#define __PROJECT_STONE_OCI20_STRINGVAR_H__

#include <oci.h>

#include <string>

#include "variable.h"

namespace Oci20 {

    class StringVar : public Variable {
    public:
        StringVar(int size);
        StringVar(const std::string& str);

        void Assign(const std::string& str);

        virtual sword GetString(std::string& strbuff, const std::string& null = m_null) const;

        sb4 GetStringBufSize() const { return GetSize() - static_cast<sb4>(sizeof(int)); }
    };
}

#endif // __PROJECT_STONE_OCI20_STRINGVAR_H__
