#include "property.h"
#include "string.h"

namespace Utils {

	Property::Property() {
	}

	Property::Property(const std::string& name, const std::string& value)
		: m_name(name), m_value(value) {
	}

	Property::~Property() {
	}

	bool Property::ToBool() const {
		return String::ToBool(m_value);
	}

	int Property::ToInt() const {
		return String::ToInt(m_value);
	}

	double Property::ToDouble() const {
		return String::ToDouble(m_value);
	}

	std::string Property::ToString() const {
		return m_value;
	}

	const std::string& Property::GetName() const {
		return m_name;
	}

	const std::string& Property::GetValue() const {
		return m_value;
	}

	void Property::SetValue(const std::string& value) {
		m_value = value;
	}
}