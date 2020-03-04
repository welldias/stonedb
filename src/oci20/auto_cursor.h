#ifndef __PROJECT_STONE_OCI20_AUTOCURSOR_H__
#define __PROJECT_STONE_OCI20_AUTOCURSOR_H__

#include <string>
#include <map>
#include <vector>
#include <memory>

#include "statement.h"
#include "connect.h"
#include "variable.h"
#include "ref_cursor_variable.h"

namespace Oci20 {

	class AutoCursor : protected Statement {
	public:
		AutoCursor(Connect& connect, const std::string& sttm, int prefetch = 100, int strLimit = 4000, int blobHexRowLength = 32);
		AutoCursor(Connect& connect, int prefetch = 100, int strLimit = 4000, int blobHexRowLength = 32);

		~AutoCursor();

        void Attach(RefCursorVariable&);

        using Statement::IsOpen;
        using Statement::Close;

        using Statement::Prepare;
        void Execute();
        void ExecuteShadow();
        bool Fetch();
        void Bind(Variable*, const std::string&);
        void Bind(const std::string& name, const std::string& value);
        void Bind(const std::string& name, int value);
        void Bind(const std::string& name, double value);

        int GetFieldCount() const;
        const std::string& GetFieldName(int) const;
        bool IsNumberField(int) const;
        bool IsDatetimeField(int) const;
        std::string BackcastField(int, const std::string&) const;

        void GetString(int, std::string&) const;
        bool IsNull(int) const;


        using Statement::GetConnect;
        using Statement::GetType;
        using Statement::GetRowCount;
        using Statement::GetSQLFunctionCode;
        using Statement::GetSQLFunctionDescription;
        using Statement::PossibleCompilationErrors;
        using Statement::GetParseErrorOffset;
        using Statement::EnableAutocommit;
        using Statement::IsStateInitialized;
        using Statement::IsStateExecuted;
        using Statement::IsStateEndOfFetch;

        std::string  GetNumberFormat() { return m_NumberFormat; }
        std::string  GetDateFormat() { return m_DateFormat; }
        std::string  GetTimeFormat() { return m_TimeFormat; }
        std::string  GetTimestampFormat() { return m_TimestampFormat; }
        std::string  GetTimestampTzFormat() { return m_TimestampTzFormat; }
        std::string  GetStringNull() { return m_StringNull; }
        int          GetIntNull() { return m_IntNull; }
        double       GetDoubleNull() { return m_DoubleNull; }
        int          GetStringLimit() { return m_StringLimit; }
        int          GetBlobHexRowLength() { return m_BlobHexRowLength; }
        bool         GetSkipLobs() { return m_SkipLobs; }
        long long    GetExecutionTime() { return m_ExecutionTime; }

        void SetNumberFormat(const std::string& value) { m_NumberFormat = value; }
        void SetDateFormat(const std::string& value) { m_DateFormat = value; }
        void SetTimeFormat(const std::string& value) { m_TimeFormat = value; }
        void SetTimestampFormat(const std::string& value) { m_TimestampFormat = value; }
        void SetTimestampTzFormat(const std::string& value) { m_TimestampTzFormat = value; }
        void SetStringNull(const std::string& value) { m_StringNull = value; }
        void SetIntNull(int value) { m_IntNull = value; }
        void SetDoubleNull(double value) { m_DoubleNull = value; }
        void SetStringLimit(int value) { m_StringLimit = value; }
        void SetBlobHexRowLength(int value) { m_BlobHexRowLength = value; }
        void SetSkipLobs(bool value) { m_SkipLobs = value; }
        void SetExecutionTime(long long value) { m_ExecutionTime = value; }
	
    protected:
        std::string m_NumberFormat;
        std::string m_DateFormat;
        std::string m_TimeFormat;
        std::string m_TimestampFormat;
        std::string m_TimestampTzFormat;
        std::string m_StringNull;
        int m_IntNull;
        double m_DoubleNull;
        int m_StringLimit;
        int m_BlobHexRowLength;
        bool m_SkipLobs;
        long long m_ExecutionTime;

		std::vector<std::string> m_fieldNames;
		std::vector<Variable*> m_fields;
		std::map<std::string, Variable*> m_boundFields;
        bool m_endOfFetch;

        void Define();

    private:
        // copy-constraction & assign-operation is not supported
        AutoCursor(const AutoCursor&);
        void operator = (const AutoCursor&);
	};
}

#endif /* __PROJECT_STONE_OCI20_AUTOCURSOR_H__ */


