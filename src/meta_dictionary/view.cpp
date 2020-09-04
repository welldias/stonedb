#include <string> 
#include <algorithm>
#include <sstream>

#include "view.h"
#include "constraint.h"
#include "parser_util.h"

namespace MetaDictionary {

    void View::WriteConstraints(MetaStream& out, const MetaSettings& settings, char chType) const {

        std::vector<const Constraint*> vector;

        for (auto it : m_constraints) {

            Constraint* constraint = NULL;
            //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
            //Constraint& constraint = m_Dictionary.LookupConstraint(m_strOwner.c_str(), it->c_str());
            if (constraint->GetType() == chType && !constraint->IsNotNullCheck())
                vector.push_back(constraint);
        }

        std::stable_sort(vector.begin(), vector.end(), [](const Constraint* c1, const Constraint* c2) {

            if (c1->GetType() != c2->GetType())
                return (c1->GetType() < c2->GetType());

            bool first_is_null = c1->IsGenerated();
            bool second_is_null = c2->IsGenerated();

            if (!first_is_null || !second_is_null)
                return (first_is_null ? std::string() : c1->m_name) < (second_is_null ? std::string() : c2->m_name);

            if (c1->GetType() == 'C')
                return c1->GetSearchCondition() < c2->GetSearchCondition();

            return c1->GetColumns() < c2->GetColumns();
        });

        for (auto it : vector) { it->Write(out, settings); }
    }

    void View::WriteTriggers(MetaStream& out, const MetaSettings& settings) const {

        for (auto it : m_triggers) {
            //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
            //  m_Dictionary.LookupTrigger(it.c_str()).Write(out, settings);
        }
    }

    void View::WriteComments(MetaStream& out, const MetaSettings& settings) const {
        
        if (!m_comments.empty()) {
            out.PutIndent();
            out.Put("COMMENT ON TABLE ");
            out.PutOwnerAndName(m_owner, m_name, settings.SchemaName);
            out.Put(" IS ");
            out.WriteSQLString(m_comments);
            out.Put(settings.EndOfShortStatement);
            out.NewLine();
            out.NewLine();
        }

        int i = 0;
        for (auto it : m_columns) {
            auto comIt = m_columnsComments.find(i);
            if (comIt != m_columnsComments.end()) {
                out.PutIndent();
                out.Put("COMMENT ON COLUMN ");
                out.PutOwnerAndName(m_owner, m_name, settings.SchemaName);
                out.Put(".");
                out.SafeWriteDBName(it.second);
                out.Put(" IS ");
                out.WriteSQLString((*comIt).second);
                out.Put(settings.EndOfShortStatement);
                out.NewLine();
            }

            i++;
        }

        if (m_columnsComments.size())
            out.NewLine();
    }

    int View::Write(MetaStream& out, const MetaSettings& settings) const {
        
        int nHeaderLines = 0;

        if (settings.GeneratePrompts) {
            out.PutIndent();
            out.Put("PROMPT CREATE OR REPLACE VIEW ");
            out.PutOwnerAndName(m_owner, m_name, settings.SchemaName);
            out.NewLine();
            nHeaderLines++;
        }

        if (!settings.ViewWithTriggers && m_triggers.size()) {
            out.PutLine("PROMPT This view has instead triggers");
            nHeaderLines++;
        }

        out.PutIndent();
        out.Put("CREATE OR REPLACE");
        if (settings.ViewWithForce) 
            out.Put(" FORCE");

        if (settings.SQLPlusCompatibility) {
            out.Put(" ");
        }
        else {
            nHeaderLines++;
            out.NewLine();
            out.PutIndent();
        }

        out.Put("VIEW ");
        out.PutOwnerAndName(m_owner, m_name, settings.SchemaName);
        out.Put(" (");

        if (settings.CommentsAfterColumn && !m_comments.empty()) {
            out.Put(" -- ");
            out.Put(m_comments);
        }
        out.NewLine();
        nHeaderLines++;

        int nMaxColLength = 0;
        int nSize = static_cast<int>(m_columns.size());
        for (auto it : m_columns)
            nMaxColLength = std::max<int>(nMaxColLength, static_cast<int>(it.second.size()));

        nMaxColLength += 2;
        nMaxColLength = std::max<int>(nMaxColLength, settings.CommentsPos);

        out.MoveIndent(2);
        int i = 0;
        for (auto it : m_columns) {

            out.PutIndent();
            int nPos = static_cast<int>(out.GetPosition());
            out.SafeWriteDBName(it.second);

            if (i < (nSize - 1)) out.Put(",");

            if (settings.CommentsAfterColumn) {
                auto comIt = m_columnsComments.find(i);
                if (comIt != m_columnsComments.end()) {
                    int j = static_cast<int>(out.GetPosition()) - nPos;
                    while (j++ < nMaxColLength)
                        out.Put(" ");
                    out.Put("-- ");
                    out.Put((*comIt).second);
                }
            }
            out.NewLine();
            nHeaderLines++;
            i++;
        }

        out.MoveIndent(-2);
        out.PutLine(") AS");
        nHeaderLines++;

        ParserUtil::WriteTextBlock(out, m_text, settings.SQLPlusCompatibility, settings.SQLPlusCompatibility);

        if (!m_viewConstraint.empty()) {
            out.MoveIndent(1);
            out.PutIndent();
            out.Put("CONSTRAINT ");
            out.SafeWriteDBName(m_name);
            out.NewLine();
            out.MoveIndent(-1);
        }

        out.PutLine("/");
        out.NewLine();

        if (settings.Constraints) {
            WriteConstraints(out, settings, 'C');
            WriteConstraints(out, settings, 'P');
            WriteConstraints(out, settings, 'U');
            WriteConstraints(out, settings, 'R');
        }

        if (settings.ViewWithTriggers)
            WriteTriggers(out, settings);

        if (settings.Comments)
            WriteComments(out, settings);

        return nHeaderLines;
    }
}
