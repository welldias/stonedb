#include "meta_stream.h"

#include  "..\utils\utils.h"

namespace MetaDictionary {

	using namespace Utils;

	MetaStream::MetaStream(bool lowerDBName)
		: m_lowerDBName(lowerDBName), m_enableLineCounter(false), m_indent(4), m_lineCounter(0) {
	}

	void MetaStream::SetLike(const MetaStream& other) {
		
		m_indent = other.m_indent;
		m_lowerDBName = other.m_lowerDBName;
		m_strIndent = other.m_strIndent;
	}

	unsigned int MetaStream::SetIndent(unsigned int lenght) {
		
		size_t oldLenght = m_strIndent.size();
		m_strIndent.resize(lenght, ' ');
		return static_cast<unsigned int>(oldLenght);
	}

	unsigned int MetaStream::MoveIndent(int offset) {

		size_t oldLenght = m_strIndent.size();

		if ((oldLenght + offset) < 0)
			return static_cast<unsigned int>(oldLenght);

		m_strIndent.resize(oldLenght + offset, ' ');
		return static_cast<unsigned int>(oldLenght);
	}

	void MetaStream::PutLine(const std::string& buffer) {

		PutIndent();
		Put(buffer);
		NewLine();
	}

	void MetaStream::Fill(char chr, unsigned int count) {

		Put(std::string(count, chr));
	}

	void MetaStream::PutOwnerAndName(const std::string& owner, const std::string& name, bool putOwner) {

		if (putOwner && !owner.empty()) {
			SafeWriteDBName(owner);
			Put(".");
		}
		SafeWriteDBName(name);
	}

	template <class Iterable>
	void MetaStream::WriteColumns(Iterable begin, Iterable end, int indent, bool safeWriteDBName) {

		auto it = begin;
		while (it != end) {
			PutIndent();

			if (safeWriteDBName)
				SafeWriteDBName(*it);
			else
				Put(*it);

			it++;

			if (it != end)
				Put(",");

			NewLine();
		}
	}

	void MetaStream::WriteColumns(const std::vector<std::string>& columns, int indent, bool safeWriteDBName) {
		MoveIndent(indent);
		WriteColumns(columns.begin(), columns.end(), indent, safeWriteDBName);
		MoveIndent(-indent);
	}

	void MetaStream::WriteColumns(const std::list<std::string>& columns, int indent, bool safeWriteDBName) {
		MoveIndent(indent);
		WriteColumns(columns.begin(), columns.end(), indent, safeWriteDBName);
		MoveIndent(-indent);
	}

	void MetaStream::SafeWriteDBName(const std::string& name) {

		bool regular = true;

		for (auto& c : name) {
			if (!((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '$' || c == '#')) {
				regular = false;
				break;
			}
		}

		if (regular) {
			Put((m_lowerDBName ? StringUtil::ToLower(name) : name));
		}
		else {
			Put("\""); Put(name); Put("\"");
		}
	}

	void MetaStream::WriteSQLString(const std::string& str) {

		std::string buffer;
		
		buffer.reserve(str.length() + 10);
		buffer += '\'';

		if (str.find('\'') == std::string::npos) {
			buffer += str;
		}
		else {
			for (auto it = str.begin(); it != str.end(); ++it)
			{
				if (*it == '\'')
					buffer += *it;
				buffer += *it;
			}
		}

		buffer += '\'';
		Put(buffer);
	}

	void MetaStream::EnableLineCounter(bool enable) {
		m_enableLineCounter = enable;
		m_lineCounter = 0;
	}

	unsigned int MetaStream::GetIndent() const {
		return static_cast<unsigned int>(m_strIndent.size());
	}
}
