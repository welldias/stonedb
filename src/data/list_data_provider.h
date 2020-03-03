#ifndef __PROJECT_STONE_DATA_LISTDATAPROVIDER_H__
#define __PROJECT_STONE_DATA_LISTDATAPROVIDER_H__

namespace Data {

    class ListDataProvider
	{
    protected:
        std::string m_schema;

    public:
        enum class InfoType {
            Undefined,
            Function,
            Procedure,
            Package,
            PackageBody,
            Trigger,
            View,
            Table,
            Sequence,
            Synonym,
            Grantee,
            Cluster,
            Dblink,
            Snapshot,
            SnapshotLog,
            Type,
            TypeBody,
            Index,
            ChkConstraint,
            PkConstraint,
            UkConstraint,
            FkConstraint,
            Recyclebin,
            InvalidObject,
            Java,
        };

        enum class ColumnType {
            String,
            Number,
            Date
        };

        static const int MIN_DEF_COL_WIDTH = 80;
        static const int MAX_DEF_COL_WIDTH = 200;

        virtual ~ListDataProvider() {}

        virtual int GetRowCount() const = 0;
        virtual int GetColCount() const = 0;
        virtual ColumnType GetColumnType(int col) const = 0;
        virtual void GetColHeader(int col, std::string& value) const = 0;
        virtual void GetString(int row, int col, std::string& value) const = 0;
        virtual bool IsVisibleRow(int row) const = 0;
        virtual int Compare(int row1, int row2, int col) const = 0;

        virtual int GetMinDefColWidth(int col) const { return MIN_DEF_COL_WIDTH; }
        virtual int GetMaxDefColWidth(int col) const { return MAX_DEF_COL_WIDTH; }

        virtual InfoType GetInfoType() const = 0;

        virtual size_t Query() = 0;

        void SetSchema(const std::string& schema) { m_schema = schema; }
        std::string GetSchema() { return m_schema; }
	};
}

#endif /*__PROJECT_STONE_DATA_LISTDATAPROVIDER_H__*/
