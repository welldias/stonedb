#include <string> 
#include <algorithm>
#include <sstream>

#include "snapshot_log.h"
#include "parser_util.h"

namespace MetaDictionary {

    int SnapshotLog::Write(MetaStream& out, const MetaSettings& settings) const {

        if (settings.GeneratePrompts) {
            out.PutIndent();
            out.Put("PROMPT CREATE SNAPSHOT LOG ON ");
            out.PutOwnerAndName(m_owner, m_name, settings.SchemaName);
            out.NewLine();
        }

        out.PutIndent();
        out.Put("CREATE");

        if (settings.SQLPlusCompatibility) {
            out.Put(" ");
        }
        else {
            out.NewLine();
            out.PutIndent();
        }

        out.Put("SNAPSHOT LOG ON ");
        out.PutOwnerAndName(m_owner, m_name, settings.SchemaName);
        out.NewLine();

        out.MoveIndent(2);

        TableStorage defStorage;
        
        //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
        //BuildDefault(defStorage, m_Dictionary, m_strOwner, m_strTablespaceName.GetValue());

        WriteStorage(out, defStorage, settings);

        if (settings.StorageSubstitutedClause)
            ParserUtil::WriteSubstitutedClause(out, m_name, "_st");

        out.MoveIndent(-2);

        out.PutLine("/");
        out.NewLine();

        return 0;
    }
}