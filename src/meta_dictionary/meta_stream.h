#ifndef __PROJECT_STONE_META_DICTIONARY_METASTREAM_H__
#define __PROJECT_STONE_META_DICTIONARY_METASTREAM_H__

#include <iostream>

#include <string>
#include <map>
#include <list>
#include <vector>

namespace MetaDictionary {

	class MetaStream {
	protected:
		bool m_lowerDBName;
		bool m_enableLineCounter;
		unsigned int  m_indent;
        unsigned int  m_lineCounter;
		std::string m_strIndent;

	public:
        MetaStream(bool lowerDBName = false);
		virtual ~MetaStream() {}

        void SetLike(const MetaStream& other);

        unsigned int  GetIndent() const;
        unsigned int  SetIndent(unsigned int lenght);
        unsigned int  MoveIndent(int offset);

        virtual size_t  GetPosition() = 0;
        virtual size_t  GetLength() = 0;
        virtual void    PutIndent() = 0;
        virtual void    NewLine() = 0;
        virtual void    Put(const std::string& buffer) = 0;

        bool IsLowerDBName() const { return m_lowerDBName; }
        void SetLowerDBName(bool lower) { m_lowerDBName = lower; }
        int  GetLineCounter() const { return m_lineCounter; }
        void EnableLineCounter(bool enable);

        void PutLine(const std::string& buffer);
        void Fill(char chr, unsigned int count);
        void PutOwnerAndName(const std::string& owner, const std::string& name, bool putOwner);
        void WriteColumns(const std::vector<std::string>& columns, int indent, bool safeWriteDBName = true);
        void WriteColumns(const std::list<std::string>& columns, int indent, bool safeWriteDBName = true);
        void SafeWriteDBName(const std::string& strName);
        void WriteSQLString(const std::string& str);

    private:
        template <class Iterable>
        void WriteColumns(Iterable begin, Iterable end, int indent, bool safeWriteDBName);

	};

}

#endif/*__PROJECT_STONE_META_DICTIONARY_METASTREAM_H__*/