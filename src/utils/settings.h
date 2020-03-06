#ifndef __PROJECT_STONE_UTILS_SETTINGS_H__
#define __PROJECT_STONE_UTILS_SETTINGS_H__

#include <set>
#include <string>

#include "variable.h"

namespace Utils {

    class Settings {
    private:
        std::string m_fileName;
        bool m_updated;
    public:
        Settings(const std::string& fileName);

    private: 
        static Variable<std::string> m_DateFormat;
        static Variable<bool> m_Autocommit;
        static Variable<int> m_CommitOnDisconnect;
        static Variable<bool> m_SavePassword;
        static Variable<bool> m_OutputEnable;
        static Variable<int> m_OutputSize;
        static Variable<bool> m_SessionStatistics;
        static Variable<std::string> m_SessionStatisticsMode;
        static Variable<bool> m_ScanForSubstitution;
        static Variable<std::string> m_PlanTable;
        static Variable<int> m_CancelQueryDelay;
        static Variable<bool> m_TopmostCancelQuery;
        static Variable<bool> m_DbmsXplanDisplayCursor;
        static Variable<bool> m_WhitespaceLineDelim;
        static Variable<bool> m_EmptyLineDelim;
        static Variable<bool> m_UnlimitedOutputSize;
        static Variable<std::string> m_ExternalToolCommand;
        static Variable<std::string> m_ExternalToolParameters;
        static Variable<bool> m_HaltOnErrors;
        static Variable<bool> m_UseDbmsMetaData;
        static Variable<bool> m_SaveFilesBeforeExecute;
        static Variable<bool> m_ColumnOrderByName;
        static Variable<bool> m_EnhancedVisuals;
        static Variable<int> m_MaxIdentLength;
        static Variable<bool> m_CacheKnownDBObjects;
        static Variable<bool> m_TimestampSupported;
        static Variable<bool> m_IntervalToTextSupported;
        static Variable<bool> m_SynonymWithoutObjectInvalid;
        static Variable<std::string> m_CurrentDBUser;
        static Variable<std::string> m_CurrentDBSchema;

    public:
        static const std::string&   GetDateFormat()                  { return m_DateFormat.GetValue(); }
        static const bool&          GetAutocommit()                  { return m_Autocommit.GetValue(); }
        static const int&           GetCommitOnDisconnect()          { return m_CommitOnDisconnect.GetValue(); }
        static const bool&          GetSavePassword()                { return m_SavePassword.GetValue(); }
        static const bool&          GetOutputEnable()                { return m_OutputEnable.GetValue(); }
        static const int&           GetOutputSize()                  { return m_OutputSize.GetValue(); }
        static const bool&          GetSessionStatistics()           { return m_SessionStatistics.GetValue(); }
        static const std::string&   GetSessionStatisticsMode()       { return m_SessionStatisticsMode.GetValue(); }
        static const bool&          GetScanForSubstitution()         { return m_ScanForSubstitution.GetValue(); }
        static const std::string&   GetPlanTable()                   { return m_PlanTable.GetValue(); }
        static const int&           GetCancelQueryDelay()            { return m_CancelQueryDelay.GetValue(); }
        static const bool&          GetTopmostCancelQuery()          { return m_TopmostCancelQuery.GetValue(); }
        static const bool&          GetDbmsXplanDisplayCursor()      { return m_DbmsXplanDisplayCursor.GetValue(); }
        static const bool&          GetWhitespaceLineDelim()         { return m_WhitespaceLineDelim.GetValue(); }
        static const bool&          GetEmptyLineDelim()              { return m_EmptyLineDelim.GetValue(); }
        static const bool&          GetUnlimitedOutputSize()         { return m_UnlimitedOutputSize.GetValue(); }
        static const std::string&   GetExternalToolCommand()         { return m_ExternalToolCommand.GetValue(); }
        static const std::string&   GetExternalToolParameters()      { return m_ExternalToolParameters.GetValue(); }
        static const bool&          GetHaltOnErrors()                { return m_HaltOnErrors.GetValue(); }
        static const bool&          GetUseDbmsMetaData()             { return m_UseDbmsMetaData.GetValue(); }
        static const bool&          GetSaveFilesBeforeExecute()      { return m_SaveFilesBeforeExecute.GetValue(); }
        static const bool&          GetColumnOrderByName()           { return m_ColumnOrderByName.GetValue(); }
        static const bool&          GetEnhancedVisuals()             { return m_EnhancedVisuals.GetValue(); }
        static const int&           GetMaxIdentLength()              { return m_MaxIdentLength.GetValue(); }
        static const bool&          GetCacheKnownDBObjects()         { return m_CacheKnownDBObjects.GetValue(); }
        static const bool&          GetTimestampSupported()          { return m_TimestampSupported.GetValue(); }
        static const bool&          GetIntervalToTextSupported()     { return m_IntervalToTextSupported.GetValue(); }
        static const bool&          GetSynonymWithoutObjectInvalid() { return m_SynonymWithoutObjectInvalid.GetValue(); }
        static const std::string&   GetCurrentDBUser()               { return m_CurrentDBUser.GetValue(); }
        static const std::string&   GetCurrentDBSchema()             { return m_CurrentDBSchema.GetValue(); }































                        


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
