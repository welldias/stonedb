#include "exception.h"

namespace Stone {
	Exception::Exception()
		: std::exception("OCI20: Cannot allocate OCI handle."),
		m_errcode(0)
	{
	}
}