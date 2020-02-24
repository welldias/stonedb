#ifndef __PROJECT_STONE_OCI20_INTERVALVAR_H__
#define __PROJECT_STONE_OCI20_INTERVALVAR_H__

#include "native_oci_variable.h"

namespace Oci20 {

	class IntervalVar : public NativeOciVariable {
    public:
        enum class Type {
            Ym = SQLT_INTERVAL_YM,
            Ds = SQLT_INTERVAL_DS,
        };

        IntervalVar(OCIEnv* ociEnv, Type type);

        virtual sword GetString(std::string& strBuff, const std::string& null = m_null) const;

        virtual bool IsDatetime() const { return true; }
        virtual std::string Backcast(const std::string& val);

    protected:
        static ub2 SqltDtypeMap(ub2 dtype);
	};
}

#endif /* __PROJECT_STONE_OCI20_INTERVALVAR_H__ */
