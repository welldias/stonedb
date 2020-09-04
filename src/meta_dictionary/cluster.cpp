#include <string> 
#include <algorithm>
#include <sstream>

#include "cluster.h"
#include "parser_util.h"

namespace MetaDictionary {

    Cluster::Cluster() {
        m_isHash = false;
        m_cache = false;
    }

    int Cluster::Write(MetaStream& out, const MetaSettings& settings) const {

        if (settings.GeneratePrompts) {
            out.PutIndent();
            out.Put("PROMPT CREATE CLUSTER ");
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

        out.Put("CLUSTER ");
        out.PutOwnerAndName(m_owner, m_name, settings.SchemaName);
        out.Put(" (");
        out.NewLine();

        out.MoveIndent(2);

        std::string type;
        
        int i = static_cast<int>(m_columns.size() - 1);
        for (const auto& [key, value] : m_columns) {
            out.PutIndent();
            out.SafeWriteDBName(value->GetColumnName());
            out.Put(" ");
            value->GetSpecString(type, settings);
            out.Put(type);
            if (i) out.Put(",");
            out.NewLine();

            i--;
        }

        out.MoveIndent(-2);
        out.PutLine(")");
        out.MoveIndent(2);

        if (!m_size.empty()) {
            out.PutIndent();
            out.Put("SIZE ");
            out.Put(m_size);
            out.NewLine();
        }

        if (m_isHash) {
            out.PutIndent();
            out.Put("HASHKEYS ");
            out.Put(m_hashKeys);
            out.NewLine();

            if (!m_hashExpression.empty()) {
                out.PutIndent();
                out.Put("HASH IS ");
                out.Put(m_hashExpression);
                out.NewLine();
            }
        }

        TableStorage defStorage;

        //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
        //BuildDefault(defStorage, m_Dictionary, m_strOwner, m_strTablespaceName.GetValue());

        defStorage.m_iniTrans.SetDefault(2);

        WriteStorage(out, defStorage, settings);

        if (m_cache) 
            out.PutLine("CACHE");

        out.MoveIndent(-2);
        out.PutLine("/");
        out.NewLine();

        for (auto indice : m_indexes) {
            //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
            //m_dictionary.LookupIndex(it->c_str()).Write(out, settings);
        }

        return 0;
    }
}
