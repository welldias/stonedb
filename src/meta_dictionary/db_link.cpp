#include <string> 
#include <algorithm>
#include <sstream>

#include "db_link.h"

namespace MetaDictionary {

    int DbLink::Write(MetaStream& out, const MetaSettings& settings) const {

        if (settings.GeneratePrompts) {
            out.PutIndent();
            if (m_owner == "PUBLIC")
                out.Put("PROMPT CREATE PUBLIC DATABASE LINK ");
            else
                out.Put("PROMPT CREATE DATABASE LINK ");
            out.PutOwnerAndName(m_owner, m_name, settings.SchemaName);
            out.NewLine();
        }

        out.PutIndent();

        if (m_owner == "PUBLIC")
            out.Put("CREATE PUBLIC DATABASE LINK ");
        else
            out.Put("CREATE DATABASE LINK ");

        out.SafeWriteDBName(m_name);

        if (!m_user.empty() || !m_host.empty()) {

            out.NewLine();
            out.MoveIndent(2);
            out.PutIndent();
            if (!m_user.empty()) {
                out.Put("CONNECT TO ");
                out.SafeWriteDBName(m_user);
                out.Put(" IDENTIFIED BY ");
                if (!m_password.empty())
                    out.Put(m_password);
                else
                    out.Put("\'NOT ACCESSIBLE\'");
            }

            if (!m_host.empty()) {
                out.Put(" USING \'");
                out.Put(m_host);
                out.Put("\'");
                out.MoveIndent(-2);
            }
        }

        out.Put(settings.EndOfShortStatement);
        out.NewLine();
        out.NewLine();

        return 0;
    }
}