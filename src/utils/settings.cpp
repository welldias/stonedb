#include "settings.h"
#include "exception.h"

namespace Utils {

	Settings::Settings(const std::string& fileName) : m_updated(false) {
		m_fileName = fileName;
	}
}