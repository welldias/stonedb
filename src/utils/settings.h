#ifndef __PROJECT_STONE_UTILS_SETTINGS_H__
#define __PROJECT_STONE_UTILS_SETTINGS_H__

#include <set>
#include <string>

#include "variable.h"

namespace Utils {

    class Settings {
    private:
        Settings();

#define SETTINGS_PROPERTY(T,N) \
        private: \
        static Variable<T> m_##N; \
        public: \
        static const T& Get##N () { return m_##N.GetValue(); }; \
        static void  Set##N (const T& value) { m_##N.Set(value); }

        SETTINGS_PROPERTY(std::string, DateFormat);
        SETTINGS_PROPERTY(bool, Autocommit);
        SETTINGS_PROPERTY(int, CommitOnDisconnect); // 0-rollback,1-commit,2-confirm
        SETTINGS_PROPERTY(bool, SavePassword);
        SETTINGS_PROPERTY(bool, OutputEnable);
        SETTINGS_PROPERTY(int, OutputSize);
        SETTINGS_PROPERTY(bool, SessionStatistics);
        SETTINGS_PROPERTY(std::string, SessionStatisticsMode);
        SETTINGS_PROPERTY(bool, ScanForSubstitution);
        SETTINGS_PROPERTY(std::string, PlanTable);
        SETTINGS_PROPERTY(int, CancelQueryDelay);
        SETTINGS_PROPERTY(bool, TopmostCancelQuery);
        SETTINGS_PROPERTY(bool, DbmsXplanDisplayCursor);
        SETTINGS_PROPERTY(bool, WhitespaceLineDelim);
        SETTINGS_PROPERTY(bool, EmptyLineDelim);
        SETTINGS_PROPERTY(bool, UnlimitedOutputSize);
        SETTINGS_PROPERTY(std::string, ExternalToolCommand);
        SETTINGS_PROPERTY(std::string, ExternalToolParameters);
        SETTINGS_PROPERTY(bool, HaltOnErrors);
        SETTINGS_PROPERTY(bool, UseDbmsMetaData);
        SETTINGS_PROPERTY(bool, SaveFilesBeforeExecute);
        SETTINGS_PROPERTY(bool, ColumnOrderByName);
        SETTINGS_PROPERTY(bool, EnhancedVisuals);
        SETTINGS_PROPERTY(int, MaxIdentLength);
        SETTINGS_PROPERTY(bool, CacheKnownDBObjects);

        SETTINGS_PROPERTY(bool, TimestampSupported);
        SETTINGS_PROPERTY(bool, IntervalToTextSupported);

        SETTINGS_PROPERTY(bool, SynonymWithoutObjectInvalid);
        SETTINGS_PROPERTY(std::string, CurrentDBUser);
        SETTINGS_PROPERTY(std::string, CurrentDBSchema);
    };
}

#endif // __PROJECT_STONE_UTILS_SETTINGS_H__
