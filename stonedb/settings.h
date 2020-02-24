#ifndef __PROJECT_STONE_UTILS_SETTINGS_H__
#define __PROJECT_STONE_UTILS_SETTINGS_H__

#include <oci.h>

#include <set>
#include <string>

#include "property.h"

namespace Utils {

#define UTILS_DECLARE_PROPERTY(T,N) \
        Property&  Get##N () const { return GetProperty(#N); }; \
        void  Set##N (const T value) { AddProperty(std::string(#N), value); }

#define UTILS_SETTINGS_BOOL_PROPERTY(N) (Settings::GetInstance().Get##N().ToBool())


    class Settings  {
    private:
        std::set<Property*> m_properties;
        Settings();

    public:
        static Settings& GetInstance() {
            static Settings INSTANCE;
            return INSTANCE;
        }

        Property& GetProperty(const std::string& name) const;

        void  AddProperty(const std::string& name, std::string value);
        void  AddProperty(const std::string& name, bool value);
        void  AddProperty(const std::string& name, int  value);
        void  AddProperty(const std::string& name, double value);

    public:
        UTILS_DECLARE_PROPERTY(std::string, DateFormat);
        UTILS_DECLARE_PROPERTY(bool, Autocommit);
        UTILS_DECLARE_PROPERTY(int, CommitOnDisconnect); // 0-rollback,1-commit,2-confirm
        UTILS_DECLARE_PROPERTY(bool, SavePassword);
        UTILS_DECLARE_PROPERTY(bool, OutputEnable);
        UTILS_DECLARE_PROPERTY(int, OutputSize);
        UTILS_DECLARE_PROPERTY(bool, SessionStatistics);
        UTILS_DECLARE_PROPERTY(std::string, SessionStatisticsMode);
        UTILS_DECLARE_PROPERTY(bool, ScanForSubstitution);
        UTILS_DECLARE_PROPERTY(std::string, PlanTable);
        UTILS_DECLARE_PROPERTY(int, CancelQueryDelay);
        UTILS_DECLARE_PROPERTY(bool, TopmostCancelQuery);
        UTILS_DECLARE_PROPERTY(bool, DbmsXplanDisplayCursor);
        UTILS_DECLARE_PROPERTY(bool, WhitespaceLineDelim);
        UTILS_DECLARE_PROPERTY(bool, EmptyLineDelim);
        UTILS_DECLARE_PROPERTY(bool, UnlimitedOutputSize);
        UTILS_DECLARE_PROPERTY(std::string, ExternalToolCommand);
        UTILS_DECLARE_PROPERTY(std::string, ExternalToolParameters);
        UTILS_DECLARE_PROPERTY(bool, HaltOnErrors);
        UTILS_DECLARE_PROPERTY(bool, UseDbmsMetaData);
        UTILS_DECLARE_PROPERTY(bool, SaveFilesBeforeExecute);
        UTILS_DECLARE_PROPERTY(bool, ColumnOrderByName);
        UTILS_DECLARE_PROPERTY(bool, EnhancedVisuals);
        UTILS_DECLARE_PROPERTY(int, MaxIdentLength);
        UTILS_DECLARE_PROPERTY(bool, CacheKnownDBObjects);

        UTILS_DECLARE_PROPERTY(bool, TimestampSupported);
        UTILS_DECLARE_PROPERTY(bool, IntervalToTextSupported);
    };
}

#endif // __PROJECT_STONE_UTILS_SETTINGS_H__
