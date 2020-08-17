#include "constraint.h"
#include "index.h"
#include "string_stream.h"

#include <iomanip>
#include <iostream>
#include <sstream>

namespace MetaDictionary {

    Constraint::Constraint() {
        m_deferrable = false;
        m_deferred = false;
        m_generated = false;
        m_onView = false;
        m_rely = false;
        m_noValidate = false;
    };

    bool Constraint::IsNotNullCheck() const {
        if (m_type[0] == 'C' && m_columns.size() == 1) {
            
            auto notNullCheck = m_columns.begin()->second + " IS NOT NULL";
            if (m_searchCondition == notNullCheck)
                return true;

            notNullCheck = "\"" + m_columns.begin()->second + "\" IS NOT NULL";
            if (m_searchCondition == notNullCheck)
                return true;
        }
        return false;
    }

    int Constraint::Write(MetaStream& out, const MetaSettings& settings) const {

        if (settings.GeneratePrompts) {

            out.PutIndent();
            out.Put(!m_onView ? "PROMPT ALTER TABLE " : "PROMPT ALTER VIEW ");
            out.PutOwnerAndName(m_owner, m_tableName, settings.SchemaName);
            out.Put(" ADD ");

            if (!IsGenerated()) {
                out.Put("CONSTRAINT ");
                out.SafeWriteDBName(m_name);
                out.Put(" ");
            }

            switch (m_type[0]) {
            case 'C': out.Put("CHECK");       break;
            case 'P': out.Put("PRIMARY KEY"); break;
            case 'U': out.Put("UNIQUE");      break;
            case 'R': out.Put("FOREIGN KEY"); break;
            }
            out.NewLine();
        }

        out.PutIndent();
        out.Put(!m_onView ? "ALTER TABLE " : "ALTER VIEW ");
        out.PutOwnerAndName(m_owner, m_tableName, settings.SchemaName);
        out.NewLine();

        out.MoveIndent(2);
        out.PutIndent();
        out.Put("ADD ");
        if (!IsGenerated()) {
            out.Put("CONSTRAINT ");
            out.SafeWriteDBName(m_name);
            out.Put(" ");
        }

        switch (m_type[0]) {
        case 'C': out.Put("CHECK (");       break;
        case 'P': out.Put("PRIMARY KEY ("); break;
        case 'U': out.Put("UNIQUE (");      break;
        case 'R': out.Put("FOREIGN KEY ("); break;
        }

        out.NewLine();

        if (m_type[0] == 'C') {
            out.MoveIndent(2);
            out.PutLine(m_searchCondition);
            out.MoveIndent(-2);
        } else {
            out.WriteColumns(m_columns, 2);
        }

        if (m_type[0] == 'R') {
            Constraint refKey;
            //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
            //Constraint& refKey = m_dictionary.LookupConstraint(m_rOwner.c_str(), m_rConstraintName.c_str());
            out.PutIndent();
            out.Put(") REFERENCES ");
            out.PutOwnerAndName(refKey.m_owner, refKey.m_tableName, settings.SchemaName || m_owner != refKey.m_owner);
            out.Put(" (");
            out.NewLine();
            out.WriteColumns(refKey.m_columns, 2);
            out.PutIndent();
            out.Put(")");
            if (m_deleteRule == "CASCADE")
                out.Put(" ON DELETE CASCADE");
            else if (m_deleteRule == "SET NULL")
                out.Put(" ON DELETE SET NULL");
            out.NewLine();
        } else {
            out.PutLine(")");
        }

        if (m_deferrable) {
            out.PutIndent();

            if (m_deferred)
                out.Put("INITIALLY DEFERRED ");

            out.Put("DEFERRABLE");
            out.NewLine();
        }

        if (m_rely)
            out.PutLine("RELY");

        if (m_status == "DISABLED")
            out.PutLine("DISABLE");

        if (m_noValidate)
            out.PutLine("NOVALIDATE");

        if (!settings.NoStorageForConstraint && (m_type[0] == 'P' || m_type[0] == 'U')) {
            try {
                Index index;
                //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
                //Index& index = m_Dictionary.LookupIndex(m_strOwner.c_str(), m_strName.c_str());

                if (!index.GetTemporary()) {
                    StringStream out2(true);
                    out2.SetLike(out);

                    out2.MoveIndent(2);
                    index.WriteIndexParams(out2, settings, /*skip_parallel =*/ true);
                    out2.MoveIndent(-2);

                    if (out2.GetLength() > 0) {
                        out.PutLine("USING INDEX");
                        out.Put(out2.GetString());
                    }
                }
            }
            catch (const NotFound&) {}
        }

        out.MoveIndent(-2);
        out.PutLine("/");
        out.NewLine();

        return 0;
    }

    void Constraint::WritePrimaryKeyForIOT(MetaStream& out, const MetaSettings& settings) const {

        if(m_type[0] == 'P' )
         throw Exception("IOT error:\nit is not primary key!");

        out.PutIndent();

        if (!IsGenerated()) {
            out.Put("CONSTRAINT ");
            out.SafeWriteDBName(m_name);
            out.Put(" ");
        }

        out.Put("PRIMARY KEY (");
        out.NewLine();
        out.WriteColumns(m_columns, 2);
        out.PutLine(")");
    }
}
