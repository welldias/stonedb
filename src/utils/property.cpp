#include "property.h"

#include "string_util.h"

namespace Utils {

	Property::Property(const std::string& name)
		: m_name(name) {
	}

	Property::Property(const std::string& name, const std::string& value)
		: m_name(name), m_value(value) {
	}

	Property::~Property() {
	}

	bool Property::ToBool() const {
		return StringUtil::ToBool(m_value);
	}

	int Property::ToInt() const {
		return StringUtil::ToInt(m_value);
	}

	double Property::ToDouble() const {
		return StringUtil::ToDouble(m_value);
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