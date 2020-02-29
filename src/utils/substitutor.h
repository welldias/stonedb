#ifndef __PROJECT_STONE_UTILS_SUBSTITUTOR_H__
#define __PROJECT_STONE_UTILS_SUBSTITUTOR_H__

#include <string>
#include <vector>

namespace Utils {
	class Substitutor
	{
	public:
		Substitutor(bool casesensitive = true) { m_casesensitive = casesensitive; }

		void ResetContent();
		inline void ResetResult() { m_result.erase(); }

		void AddPair(const char*, const char*);
		void AddPair(const std::string&, const std::string&);
		Substitutor& operator << (const char*);
		inline const char* GetResult() const { return m_result.c_str(); }
		inline const std::string& GetResultStr() const { return m_result; }

	private:
		std::string m_result;
		std::vector<std::string> m_whatFind;
		std::vector<std::string> m_withReplace;
		bool m_casesensitive;
	};
}

#endif /*__PROJECT_STONE_UTILS_SUBSTITUTOR_H__*/