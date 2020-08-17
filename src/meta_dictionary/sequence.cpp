#include "sequence.h"

namespace MetaDictionary {

    Sequence::Sequence() {
        m_cycleFlag = false;
        m_orderFlag = false;
    }

    int Sequence::Write(MetaStream& out, const MetaSettings& settings) const {

        if (settings.GeneratePrompts) {
            out.PutIndent();
            out.Put("PROMPT CREATE SEQUENCE ");
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

        out.Put("SEQUENCE ");
        out.PutOwnerAndName(m_owner, m_name, settings.SchemaName);
        out.NewLine();

        out.MoveIndent(2);

        out.PutIndent();
        out.Put("MINVALUE ");
        out.Put(m_minValue);
        out.NewLine();

        out.PutIndent();
        out.Put("MAXVALUE ");
        out.Put(m_maxValue);
        out.NewLine();

        out.PutIndent();
        out.Put("INCREMENT BY ");
        out.Put(m_incrementBy);
        out.NewLine();

        if (settings.SequnceWithStart) {
            out.PutIndent();
            out.Put("START WITH ");
            out.Put(m_lastNumber);
            out.NewLine();
        }

        out.PutIndent();
        out.Put(m_cycleFlag ? "CYCLE" : "NOCYCLE");
        out.NewLine();

        out.PutIndent();
        out.Put(m_orderFlag ? "ORDER" : "NOORDER");
        out.NewLine();

        out.PutIndent();
        if (m_cacheSize.compare("0")) {
            out.Put("CACHE ");
            out.Put(m_cacheSize);
        }
        else {
            out.Put("NOCACHE");
        }
        out.NewLine();

        out.MoveIndent(-2);
        out.PutLine("/");
        out.NewLine();

        return 0;
    }

    std::string Sequence::GetIdentityOptions(const MetaSettings& settings) const
    {
        std::string buff;

        if (m_minValue != "1") 
            buff += "MINVALUE " + m_minValue;

        if (m_maxValue != "9999999999999999999999999999") {
            if (!buff.empty()) buff += ' ';
            buff += "MAXVALUE " + m_maxValue;
        }

        if (m_incrementBy != "1") {
            if (!buff.empty()) buff += ' ';
            buff += "INCREMENT BY " + m_incrementBy;
        }

        if (settings.SequnceWithStart)
        {
            if (m_lastNumber != "1")
            {
                if (!buff.empty()) buff += ' ';
                buff += "START WITH " + m_lastNumber;
            }
        }

        if (m_cycleFlag)
        {
            if (!buff.empty()) buff += ' ';
            buff += "CYCLE";
        }

        if (m_orderFlag)
        {
            if (!buff.empty()) buff += ' ';
            buff += "ORDER";
        }

        if (m_cacheSize == "0")
        {
            if (!buff.empty()) buff += ' ';
            buff += "NOCACHE";
        }
        else if (m_cacheSize != "20")
        {
            if (!buff.empty()) buff += ' ';
            buff += "CACHE " + m_cacheSize;
        }

        return buff;
    }
}
