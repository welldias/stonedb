
#include "grant.h"
#include "string_stream.h"

namespace MetaDictionary {

    int Grant::Write(MetaStream& out, const MetaSettings& settings) const {
        
        int i = 0;

        // Write GRANTs with global privileges. Example:
        // GRANT UPDATE ON table TO user;
        for (i = 0; i < 2; i++) {

            const std::set<std::string>& privileges = (!i) ? m_privileges : m_privilegesWithAdminOption;

            auto it  = privileges.begin();
            auto end = privileges.end();

            if (it != end) {
                StringStream out2(true);
                out2.SetLike(out);

                bool commaInList = false;

                for (; it != end; ++it) {
                    if (commaInList) out2.Put(",");
                    commaInList = true;
                    out2.Put(*it);
                }

                if (out2.GetLength() > 0) {
                    out.Put("GRANT ");

                    out.Put(out2.GetString());

                    out.Put(" ON ");
                    out.PutOwnerAndName(m_owner, m_name, settings.GetShemaName());
                    out.Put(" TO ");
                    out.SafeWriteDBName(m_grantee);

                    if (i) out.Put(" WITH GRANT OPTION");

                    out.Put(settings.GetEndOfShortStatement());
                    out.NewLine();
                }
            }

            if (privileges.find("ENQUEUE") != end) {
                out.Put("exec Dbms_Aqadm.Grant_Queue_Privilege(\'enqueue', \'");
                out.PutOwnerAndName(m_owner, m_name, settings.GetShemaName());

                out.Put("\',\'");
                out.SafeWriteDBName(m_grantee);
                out.Put("\',\'");

                if (!i) out.Put("\', FALSE);");
                else   out.Put("\', TRUE);");
            }

            if (privileges.find("DEQUEUE") != end) {
                out.Put("exec Dbms_Aqadm.Grant_Queue_Privilege(\'dequeue', \'");
                out.PutOwnerAndName(m_owner, m_name, settings.GetShemaName());
                out.Put("\',\'");
                out.SafeWriteDBName(m_grantee);

                if (!i) out.Put("\', FALSE);");
                else   out.Put("\', TRUE);");

                out.NewLine();
            }
        }

        // Write GRANTs with column privileges. Example:
        // GRANT UPDATE(colx) ON table TO user;
        for (i = 0; i < 2; i++) {

            const std::map<std::string, std::set<std::string> >& colPrivileges = (!i) ? m_collectionPrivileges : m_collectionPrivilegesWithAdminOption;

            auto it = colPrivileges.begin();
            auto end = colPrivileges.end();

            if (it != end) {
                StringStream out2(true);
                out2.SetLike(out);

                bool commaInList = false;

                for (; it != end; ++it) {
                    if (commaInList) out2.Put(", ");
                    commaInList = true;
                    out2.Put(it->first);
                    out2.Put("(");

                    bool commaInColList = false;
                    std::set<std::string>::const_iterator colIt(it->second.begin());
                    for (; colIt != it->second.end(); ++colIt) {
                        if (commaInColList) out2.Put(", ");
                        commaInColList = true;
                        out2.SafeWriteDBName(*colIt);
                    }

                    out2.Put(")");
                }

                if (out2.GetLength() > 0) {
                    out.Put("GRANT ");

                    out.Put(out2.GetString());

                    out.Put(" ON ");
                    out.PutOwnerAndName(m_owner, m_name, settings.GetShemaName());
                    out.Put(" TO ");
                    out.SafeWriteDBName(m_grantee);

                    if (i) out.Put(" WITH GRANT OPTION");

                    out.Put(settings.GetEndOfShortStatement());
                    out.NewLine();
                }
            }
        }

        return 0;
    }
}