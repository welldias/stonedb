#ifndef __PROJECT_STONE_OCI20_BUFFCURSOR_H__
#define __PROJECT_STONE_OCI20_BUFFCURSOR_H__

#include <string>
#include <vector>
#include <map>

#include "statement.h"


namespace Oci20 {

	class BuffCursor : protected Statement {
	public:
		BuffCursor(Connect& connect, const std::string& statementString, int prefetch = 100, int strLimit = 4000);
		BuffCursor(Connect& connect, int prefetch = 100, int strLimit = 4000);
		virtual ~BuffCursor();

        using Statement::Close;
        using Statement::Prepare;
        using Statement::GetConnect;
        using Statement::GetType;
        using Statement::GetRowCount;
        using Statement::GetSQLFunctionCode;
        using Statement::GetSQLFunctionDescription;
        using Statement::PossibleCompilationErrors;
        using Statement::GetParseErrorOffset;

        void Execute();
        void ExecuteShadow();
        bool Fetch();

        void SetStringLimit(int);
        void SetDateFormat(const std::string&);
        void SetStringNull(const std::string&);
        void SetIntNull(int);
        void SetDoubleNull(double);

        const std::string& GetStringNull() const;

        int GetFieldCount() const;

        const std::string& GetFieldName(int) const;

        int    GetStringLength(int) const;
        void   GetString(int, char*, int) const;
        void   GetString(int, std::string&) const;

        void   GetTime(int, struct tm&, struct tm* null = 0) const;

        std::string ToString(int) const;
        std::string ToString(int, const std::string&) const;
        
        int ToInt(int) const;
        int ToInt(int, int) const;
        
        int64_t ToInt64(int) const;
        int64_t ToInt64(int, int64_t) const;

        double ToDouble(int) const;
        double ToDouble(int, double) const;

        bool IsGood(int) const;
        bool IsNull(int) const;
        bool IsTruncated(int) const;
        bool IsStringField(int) const;
        bool IsNumberField(int) const;

        bool IsRecordGood() const;
        bool IsRecordTruncated() const;

        using Statement::Bind;

        void Bind(const std::string& name, const std::string& value);
        void Bind(const std::string& name, char value);
        void Bind(const std::string& name, int value);
        void Bind(const std::string& name, double value);

    protected:
        void Define();

        std::vector<std::string> m_fieldNames;
        std::vector<HostArray*> m_fields;
        std::map<std::string, Variable*> m_boundFields;
        ub4 m_curRecord, m_buffRecords, m_retRecords;
        bool m_endOfFetch, m_restore_lda;

        int         m_strLimit;
        std::string m_dateFormat;
        std::string m_null;
        int         m_intNull;
        double      m_doubleNull;

    private:
        static std::string m_defaultDateFormat;
        static std::string m_defaultNull;
        static int         m_defaultIntNull;
        static double      m_defaultDoubleNull;

        // copy-constraction & assign-operation is not supported
        BuffCursor(const BuffCursor&);
        void operator = (const BuffCursor&);
	};

}
#endif // __PROJECT_STONE_OCI20_BUFFCURSOR_H__
