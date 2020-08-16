#include "table_base.h"
#include "index.h"
#include "constraint.h"
#include "string_stream.h"

#include <algorithm>

namespace MetaDictionary {

    TableBase::TableBase() {
    }

    void TableBase::WriteIndexes(MetaStream& out, const MetaSettings& settings) const {
        auto it(m_indexes.begin());
        auto end(m_indexes.end());

        for(auto it : m_indexes) {
            Index index;
            //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
            //Index& index = m_Dictionary.LookupIndex(it->c_str());

            if (index.GetType() != Index::Type::IoitTop) {
                try {
                    //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
                    //m_dictionary.LookupConstraint(it->c_str());
                    // all indexes for constraints
                    if (settings.GetNoStorageForConstraint())
                        index.Write(out, settings);
                }
                catch (const NotFound&) {
                    // or index without constraints
                    index.Write(out, settings);
                }
            }
        }
    }

    void TableBase::WriteConstraints(MetaStream& out, const MetaSettings& settings, char chType) const {

        std::vector<const Constraint*> vector;

        for(auto it : m_constraints) {
            Constraint constraint;
            //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
            //Constraint& constraint = m_Dictionary.LookupConstraint(m_strOwner.c_str(), it->c_str());
            if (chType == constraint.GetType() && !constraint.IsNotNullCheck())
                vector.push_back(&constraint);
        }

        std::stable_sort(vector.begin(), vector.end(),
            [](const Constraint* c1, const Constraint* c2) {
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

    void TableBase::WriteTriggers(MetaStream& out, const MetaSettings& settings) const {

        for (auto it : m_triggers) {
            //TODO: Isto esta horrivel, verificar forma mais elegante para resolver essa amarra.
            //m_Dictionary.LookupTrigger(it->c_str()).Write(out, settings);
        }
    }
}
