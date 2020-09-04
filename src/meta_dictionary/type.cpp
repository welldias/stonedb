#include <string> 
#include <algorithm>
#include <sstream>

#include "type.h"
#include "parser_util.h"

namespace MetaDictionary {

    void Type::WriteAsIncopmlete(MetaStream& out, const MetaSettings& settings) const {

        bool parseError = false;

        const char* pszBuff = m_text.c_str();

        pszBuff += ParserUtil::SkipSpaceAndComments(pszBuff);
        parseError |= !StringUtil::Compare(pszBuff, "TYPE", sizeof("TYPE") - 1) ? false : true;
        pszBuff += sizeof("TYPE") - 1;
        parseError |= isspace(*pszBuff) ? false : true;
        pszBuff += ParserUtil::SkipSpaceAndComments(pszBuff);
        pszBuff += ParserUtil::SkipDbName(pszBuff);
        parseError |= isspace(*pszBuff) ? false : true;
        pszBuff += ParserUtil::SkipSpaceAndComments(pszBuff);
        parseError |= (!StringUtil::Compare(pszBuff, "AS", sizeof("AS") - 1) || !StringUtil::Compare(pszBuff, "IS", sizeof("IS") - 1)) ? false : true;
        pszBuff += sizeof("AS") - 1;
        parseError |= isspace(*pszBuff) ? false : true;
        pszBuff += ParserUtil::SkipSpaceAndComments(pszBuff);

        if(parseError)
            throw ParserError("Invalid header!");

        if (!(!StringUtil::Compare(pszBuff, "TABLE", sizeof(pszBuff) - 1) && isspace(pszBuff[sizeof("TABLE") - 1]))) {
            out.PutIndent();
            out.Put("CREATE OR REPLACE TYPE ");
            out.PutOwnerAndName(m_owner, m_name, settings.SchemaName);
            out.Put(";");
            out.NewLine();
            out.PutLine("/");
        }
    }
}
