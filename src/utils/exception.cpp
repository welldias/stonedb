#include "exception.h"

namespace Utils {
	Exception::Exception()
		: m_errcode(0) {
	}

	NotFound::NotFound(const std::string& objName)
		: Exception(0, std::string("Object '" + objName + "' is not found in the dictionary.")) {
	}

	AlreadyExists::AlreadyExists(const std::string& objName)
		: Exception(0, std::string("Object '" + objName + "' already exists in the dictionary.")) {
	}

	FormatError::FormatError(const std::string& format)
		: Exception(0, std::string("Format error! format: '" + format + "'.")) {
	}

	ParserError::ParserError(const std::string& format)
		: Exception(0, std::string("Parser error! '" + format + "'.")) {
	}

}