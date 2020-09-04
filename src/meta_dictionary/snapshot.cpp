#include <string> 
#include <algorithm>
#include <sstream>

#include "snapshot.h"
#include "parser_util.h"

namespace MetaDictionary {

    int Snapshot::Write(MetaStream& out, const MetaSettings& settings) const {

        if (settings.GeneratePrompts) {
            out.PutIndent();
            out.Put("PROMPT CREATE SNAPSHOT ");
            out.PutOwnerAndName(m_owner, m_name, settings.SchemaName);
            out.NewLine();
        }

        out.PutIndent();
        out.Put("CREATE");

        if (settings.SQLPlusCompatibility) {
            out.Put(" ");
        } else {
            out.NewLine();
            out.PutIndent();
        }

        out.Put("SNAPSHOT ");
        out.PutOwnerAndName(m_owner, m_name, settings.SchemaName);

        if (settings.CommentsAfterColumn && !m_comments.empty()) {
            out.Put(" -- ");
            out.Put(m_comments);
        }
        out.NewLine();


        out.MoveIndent(2);

        if (m_clusterName.empty()) {
            TableStorage defStorage;

            //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
            //BuildDefault(defStorage, m_Dictionary, m_strOwner, m_strTablespaceName.GetValue());

            WriteStorage(out, defStorage, settings);

            if (settings.StorageSubstitutedClause)
                ParserUtil::WriteSubstitutedClause(out, m_name, "_st");
        } else {
            out.PutIndent();
            out.Put("CLUSTER ");
            out.SafeWriteDBName(m_clusterName);
            out.Put(" (");
            out.NewLine();

            out.WriteColumns(m_clusterColumns, 2);

            out.PutLine(")");
        }

        out.MoveIndent(-2);

        out.PutLine("REFRESH");

        out.MoveIndent(2);
        out.PutIndent();
        out.Put(m_refreshType);

        if (!m_startWith.empty()) {
            out.Put(" START WITH ");
            out.Put(m_startWith);
        }

        if (!m_nextTime.empty()) {
            out.Put(" NEXT ");
            out.Put(m_nextTime);
        }

        out.MoveIndent(-2);

        out.PutLine("AS");
        ParserUtil::WriteTextBlock(out, m_query, settings.SQLPlusCompatibility, settings.SQLPlusCompatibility);

        out.PutLine("/");
        out.NewLine();

        return 0;
    }
}