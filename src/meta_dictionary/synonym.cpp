#include <string> 
#include <algorithm>
#include <sstream>

#include "synonym.h"
#include "parser_util.h"

namespace MetaDictionary {

    int Synonym::Write(MetaStream& out, const MetaSettings& settings) const {

        if (m_owner == "PUBLIC") {
            if (settings.GeneratePrompts) {
                out.PutIndent();
                out.Put("PROMPT CREATE PUBLIC SYNONYM ");
                out.SafeWriteDBName(m_name);
                out.NewLine();
            }

            out.PutIndent();
            out.Put("CREATE PUBLIC SYNONYM ");
            out.SafeWriteDBName(m_name);
        }
        else {
            if (settings.GeneratePrompts) {
                out.PutIndent();
                out.Put("PROMPT CREATE SYNONYM ");
                out.PutOwnerAndName(m_owner, m_name, settings.SchemaName);
                out.NewLine();
            }

            out.PutIndent();
            out.Put("CREATE SYNONYM ");
            out.PutOwnerAndName(m_owner, m_name, settings.SchemaName);
        }

        out.Put(" FOR ");
        out.PutOwnerAndName(m_refOwner, m_refName, settings.SchemaName || m_owner != m_refOwner);
        
        if (!m_refDBLink.empty()) {
            out.Put("@");
            out.SafeWriteDBName(m_refDBLink);
        }

        out.Put(settings.EndOfShortStatement);
        out.NewLine();

        return 0;
    }
}
