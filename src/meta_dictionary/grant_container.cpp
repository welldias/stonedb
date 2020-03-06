#include <algorithm>

#include "grant_container.h"
#include "string_stream.h"
#include "..\utils\utils.h"

namespace MetaDictionary {

    using namespace Utils;


    Grant& GrantContainer::CreateGrant(const std::string& grantxx, const std::string& object) {
        
        std::string key = grantxx + "." + object;

        Grant* ptrGrant = new Grant();

        if (m_mapGrants.find(key) == m_mapGrants.end())
            m_mapGrants.insert(GrantMap::value_type(key, ptrGrant));
        else
            throw AlreadyExists(key);

        return *(ptrGrant);
    }

    Grant& GrantContainer::LookupGrant(const std::string& grantxx, const std::string& object) {

        std::string key = grantxx + "." + object;

        auto it = m_mapGrants.find(key);

        if (it == m_mapGrants.end())
            throw NotFound(key);

        return *(it->second);
    }

    void GrantContainer::DestroyGrant(const std::string& grantxx, const std::string& object) {

        std::string key = grantxx + "." + object;

        auto it = m_mapGrants.find(key);

        if (it == m_mapGrants.end())
            throw NotFound(key);

        delete it->second;

        m_mapGrants.erase(it);
    }

    int GrantContainer::Write(MetaStream& out, const MetaSettings& settings) const {
        GrantVector grantVector;

        for (auto it : m_mapGrants)
            grantVector.push_back(it.second);

        std::stable_sort(grantVector.begin(), grantVector.end(), [](const Grant* a, const Grant* b) {
                return (a->m_grantee < b->m_grantee)
                    && (a->m_owner   < b->m_owner)
                    && (a->m_name    < b->m_name);
            });

        for (auto it : grantVector)
            if (it != NULL) it->Write(out, settings);

        return 0;
    }

    size_t GrantContainer::GetGrants(GrantVector& grantVector, const std::string& name, bool sort) const {
        size_t result = 0;
        for (auto it : m_mapGrants) {
            if ((it.second)->m_name == name) {
                grantVector.push_back(it.second);
                result++;
            }
        }

        if (sort) {
            std::stable_sort(grantVector.begin(), grantVector.end(), [](const Grant* a, const Grant* b) {
                return (a->m_grantee < b->m_grantee)
                    && (a->m_owner < b->m_owner)
                    && (a->m_name < b->m_name);
                });
        }

        return result;
    }

    void GrantContainer::EnumGrants(FuncEnum funcEnum, bool sort) const {

        if (!sort) {
            for (auto it : m_mapGrants)
                funcEnum(*(it.second));
        }
        else {
            GrantVector grantVector;

            for (auto it : m_mapGrants)
                grantVector.push_back(it.second);

            std::stable_sort(grantVector.begin(), grantVector.end(), [](const Grant* a, const Grant* b) {
                return (a->m_grantee < b->m_grantee)
                    && (a->m_owner < b->m_owner)
                    && (a->m_name < b->m_name);
                });

            for (auto it : grantVector)
                funcEnum(*(it));
        }
    }

    void Grantor::WriteObjectGrants(const std::string& name, MetaStream& out, const MetaSettings& settings) const {

        GrantVector grantVector;

        GetGrants(grantVector, name, true);

        for (auto it : grantVector)
                it->Write(out, settings);
    }
}