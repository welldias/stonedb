#include "db_object.h"

#include "..\utils\utils.h"

namespace MetaDictionary {

    void DbObject::WriteSubstitutedClause(MetaStream& out, const std::string& text, const std::string& suffix) {
        
        auto clause = StringUtil::ToLower(text);

        if ((clause.size() + suffix.size()) > 30)
            clause.resize(30 - suffix.size());

        for (auto it = clause.begin(); it != clause.end(); it++)
            if (!isalnum(*it)) *it = '_';

        out.PutIndent();
        out.Put("&");
        out.Put(clause);
        out.Put(suffix);
        out.NewLine();
    }
}
