#ifndef __PROJECT_STONE_OCI20_NUMBERVAR_H__
#define __PROJECT_STONE_OCI20_NUMBERVAR_H__

#include <oci.h>

#include <string>

#include "string_var.h"
#include "variable.h"

namespace Oci20 {

    class NumStrVar : public StringVar {
    public:
        NumStrVar() : StringVar(41) {}

        virtual bool IsNumber() const { return true; }
    };

    class NumberVar : public Variable {
    public:
        NumberVar(OCIError* ociError);
        NumberVar(OCIError* ociError, const std::string& format);
        NumberVar(OCIError* ociError, int value);
        NumberVar(OCIError* ociError, double value);

        sword Assign(int);
        sword Assign(double);

        virtual sword GetString(std::string& strbuff, const std::string& numericCharacters, const std::string& null) const;

        virtual sword  ToInt(int* value, int null) const;
        virtual sword  ToDouble(double* value, double null) const;

        virtual bool IsNumber() const { return true; }

    protected:
        std::string m_numberFormat;
        OCINumber m_value;
        OCIError* m_ociError;
    };
}

#endif // __PROJECT_STONE_OCI20_NUMBERVAR_H__
