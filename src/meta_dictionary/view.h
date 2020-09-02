#ifndef __PROJECT_STONE_META_DICTIONARY_VIEW_H__
#define __PROJECT_STONE_META_DICTIONARY_VIEW_H__

#include <map>

#include "db_object.h"

namespace MetaDictionary {

    class View : public DbObject {
    private:
        using ColumnContainer      = std::map<int, std::string>;
        using TriggerContainer     = std::set<std::string>;
        using CommentContainer     = std::map<int, std::string>;
        using ConstraintsContainer = std::set<std::string>;

    public:
        View() {};

        virtual bool IsCode()     const { return true; };
        virtual bool IsGrantable() const { return true; }
        virtual const std::string GetTypeStr() const { return "VIEW"; };
        virtual int Write(MetaStream& out, const MetaSettings& settings) const;

        void WriteConstraints(MetaStream& out, const MetaSettings& settings, char chType) const;
        void WriteTriggers(MetaStream& out, const MetaSettings& settings) const;
        void WriteComments(MetaStream& out, const MetaSettings& settings) const;

    private:
        std::string m_text;
        std::string m_comments;
        std::string m_viewConstraint;

        ColumnContainer  m_columns;
        TriggerContainer m_triggers;
        CommentContainer m_columnsComments;
        ConstraintsContainer m_constraints;

    private:
        // copy-constraction & assign-operation is not supported
        View(const View&);
        void operator = (const View&);
    };
}

#endif/*__PROJECT_STONE_META_DICTIONARY_VIEW_H__*/