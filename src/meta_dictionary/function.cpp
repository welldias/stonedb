#include <string> 
#include <algorithm>
#include <sstream>

#include "function.h"
#include "parser_util.h"

namespace MetaDictionary {

    int PlsCode::Write(MetaStream& out, const MetaSettings& settings) const {

        int nHeaderLines = 0;
        if (settings.GeneratePrompts) {
            out.PutIndent();
            out.Put("PROMPT CREATE OR REPLACE ");
            out.Put(m_type);
            out.Put(" ");
            out.PutOwnerAndName(m_owner, m_name, settings.SchemaName);
            out.NewLine();
            nHeaderLines++;
        }

        out.PutIndent();
        out.Put("CREATE OR REPLACE");

        if (settings.SQLPlusCompatibility) {
            out.Put(" ");
        }
        else {
            out.NewLine();
            out.PutIndent();
            nHeaderLines++;
        }

        int nLen;
        auto pszText = m_text.c_str();
        pszText += ParserUtil::SkipSpaceAndComments(pszText);
        ParserUtil::GetWordSP(pszText, pszText, nLen);

        if (m_type.compare(pszText)) {
            int nSecond;
            const char* pszSecond;
            ParserUtil::GetWordSP(pszText + nLen, pszSecond, nSecond);

            if (!(m_type == "PACKAGE BODY" || m_type == "TYPE BODY") 
                || !(!_strnicmp("PACKAGE", pszText, sizeof("PACKAGE") - 1)  || !_strnicmp("TYPE", pszText, sizeof("TYPE") - 1)) 
                || _strnicmp("BODY", pszSecond, sizeof("BODY") - 1)) {

                throw Exception("Loading Error: a body header is invalid!");
            }

            nLen = static_cast<int>(pszSecond + nSecond - pszText);
            out.Put(pszText);
            pszText += nLen;
        }
        else {
            out.Put(pszText);
            pszText += m_type.size();
        }

        out.Put(" ");
        out.PutOwnerAndName(m_owner, m_name, settings.SchemaName);

        pszText += ParserUtil::SkipDbName(pszText);

        ParserUtil::WriteTextBlock(out, pszText, settings.SQLPlusCompatibility, false);

        out.PutLine("/");
        out.NewLine();

        return nHeaderLines;
    }
}
