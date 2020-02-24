#ifndef __PROJECT_STONE_OCI20_DATEVAR_H__
#define __PROJECT_STONE_OCI20_DATEVAR_H__

#include <oci.h>

#include <string>

#include "variable.h"

namespace Oci20 {

    class DateVar : public Variable {
    public:
        DateVar(OCIError* ociError, const std::string& nlsLanguage);
        DateVar(OCIError* ociError, const std::string& nlsLanguage, const std::string& dateFormat);

        virtual sword GetString(std::string& strbuff, const std::string& null) const;

        virtual bool IsDatetime() const { return true; }
        virtual std::string Backcast(const std::string& val) { return "To_Date('" + val + "', '" + m_dateFormat + "')"; }

    protected:
        std::string m_dateFormat;
        std::string m_nlsLanguage;

        OCIDate   m_value;
        OCIError* m_ociError;
    };
}

#endif // __PROJECT_STONE_OCI20_DATEVAR_H__
