#include "exception.h"

namespace Utils {
	Exception::Exception()
		: std::exception("OCI20: Cannot allocate OCI handle."),
		m_errcode(0)
	{
	}
}