#include <string> 
#include <algorithm>
#include <sstream>

#include "java_source.h"
#include "parser_util.h"

namespace MetaDictionary {

    int JavaSource::Write(MetaStream& out, const MetaSettings& settings) const {

        int nHeaderLines = 0;

        if (settings.GeneratePrompts) {
            out.PutIndent();
            out.Put("PROMPT CREATE OR REPLACE JAVA SOURCE ");
            out.Put(m_type);
            out.Put(" ");
            out.PutOwnerAndName(m_owner, m_name, settings.SchemaName);
            out.NewLine();
            nHeaderLines++;
        }

        out.PutIndent();
        out.Put("CREATE OR REPLACE AND COMPILE");

        if (settings.SQLPlusCompatibility) {
            out.Put(" ");
        } else {
            out.NewLine();
            out.PutIndent();
            nHeaderLines++;
        }

        out.Put("JAVA SOURCE NAMED ");
        out.PutOwnerAndName(m_owner, m_name, settings.SchemaName);
        out.Put(" AS ");
        out.NewLine();

        ParserUtil::WriteTextBlock(out, m_text, settings.SQLPlusCompatibility, false);

        out.PutLine("/");
        out.NewLine();

        return nHeaderLines;
    }
}
