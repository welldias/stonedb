#include "settings.h"
#include "exception.h"

namespace Utils {

	using namespace Stone;

	std::set<Property*> Settings::m_properties;

	Settings::Settings() {
	}

	Property& Settings::GetProperty(const std::string& name)  {
		std::set<Property*>::iterator i = m_properties.begin();
		while (i != m_properties.end()) {
			if ((*i)->GetName() == name)
				return (**i);
			i++;
		}

		throw Exception(0, "Settings::GetProperty: property " + name + " doesn't existe!");
	}

	void Settings::AddProperty(const std::string& name, std::string value) {
		try {
			GetProperty(name).SetValue(value);
		}
		catch (const Exception&) {
			m_properties.insert(new Property(name, value));
		}
	}

	void Settings::AddProperty(const std::string& name, bool value) {
		std::string strValue = (value ? "true" : "false");
		AddProperty(name, strValue);
	}

	void  Settings::AddProperty(const std::string& name, int value) {
		AddProperty(name, std::to_string(value));
	}

	void  Settings::AddProperty(const std::string& name, double value) {
		AddProperty(name, std::to_string(value));
	}
}