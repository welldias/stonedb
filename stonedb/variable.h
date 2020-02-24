#ifndef __PROJECT_STONE_OCI20_VARIABLE_H__
#define __PROJECT_STONE_OCI20_VARIABLE_H__

#include <oci.h>

#include <string>

namespace Oci20 {

    class Variable {
    protected:
        Variable(ub2 type, sb4 size);
        Variable(dvoid* buffer, ub2 type, sb4 size);

        sb4 GetSize()         const { return m_size; }
        sb4 GetBufferSize()   const { return m_buffer_size; }
        ub2 GetReturnedSize() const { return m_ret_size; }

    public:
        virtual ~Variable();

        virtual sword GetString(std::string&, const std::string& null = m_null) const /*= 0*/;

        virtual sword Define(OCIStmt* ociStmt, OCIError* ociStmError, int pos);
        virtual sword Bind(OCIStmt* ociStmt, OCIError* ociStmError, int pos);
        virtual sword Bind(OCIStmt* ociStmt, OCIError* ociStmError, const std::string& name);

        virtual bool IsNull() const { return m_indicator == OCI_IND_NULL ? true : false; }
        virtual bool IsNumber() const { return false; }
        virtual bool IsDatetime() const { return false; }

        virtual std::string Backcast(const std::string& val) { return val; }

        virtual void BeforeFetch() {}

    protected:
        ub2 m_type;
        sb2 m_indicator;
        sb4 m_size;
        sb4 m_buffer_size;
        ub2 m_ret_size;

        dvoid* m_buffer;
        bool   m_owner;

    public:
        static const std::string m_null;
    };
}

#endif // __PROJECT_STONE_OCI20_VARIABLE_H__
