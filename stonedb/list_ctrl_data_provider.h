#ifndef __PROJECT_STONE_DATA_LISTCTRLDATAPROVIDER_H__
#define __PROJECT_STONE_DATA_LISTCTRLDATAPROVIDER_H__

namespace Data {
	class ListCtrlDataProvider
	{
    public:
        enum class Type {
            String,
            Number,
            Date
        };

        static const int MIN_DEF_COL_WIDTH = 80;
        static const int MAX_DEF_COL_WIDTH = 200;

        virtual ~ListCtrlDataProvider() {}

        virtual int getRowCount() const = 0;
        virtual int getColCount() const = 0;
        virtual Type getColType(int col) const = 0;
        virtual const char* getColHeader(int col) const = 0;
        virtual const char* getString(int row, int col) const = 0;
        virtual bool IsVisibleRow(int row) const = 0;
        virtual int compare(int row1, int row2, int col) const = 0;

        virtual int GetMinDefColWidth(int col) const { return MIN_DEF_COL_WIDTH; }
        virtual int GetMaxDefColWidth(int col) const { return MAX_DEF_COL_WIDTH; }

        // optional icons support
        virtual int getImageIndex(int row) const { return -1; }
        virtual int getStateImageIndex(int row) const { return -1; }
	};
}

#endif /*__PROJECT_STONE_DATA_LISTCTRLDATAPROVIDER_H__*/
