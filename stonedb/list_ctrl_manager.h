#pragma once
#ifndef __PROJECT_STONE_DATA_LISTCTRLMANAGER_H__
#define __PROJECT_STONE_DATA_LISTCTRLMANAGER_H__

#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <string>
#include <vector>
#include <set>

#include "list_ctrl_data_provider.h"

namespace Data {

	class ListCtrlManager
	{
    public:
        enum class SortDir { 
            Asc = 1, 
            Desc = -1 
        };

        enum class FilterOperation { 
            Contain = 0, 
            StartWith = 1, 
            ExactMatch = 2 
        };

        struct FilterComponent {
            FilterOperation operation;
            std::string value;

            FilterComponent() : operation(FilterOperation::Contain) {}
            FilterComponent(const char* v, FilterOperation op = FilterOperation::Contain) : operation(op), value(v) {}
            FilterComponent(const std::string& v, FilterOperation op = FilterOperation::Contain) : operation(op), value(v) {}
        };

        typedef std::vector<FilterComponent> FilterCollection;

    private:
        CListCtrl& m_list;
        ListCtrlDataProvider& m_dataAdapter;
        int m_sortColumn;
        FilterCollection m_filter;
        bool m_filterEmpty;
        bool m_initalized;
        enum SortDir m_sortDir;

    public:
        ListCtrlManager(CListCtrl&, ListCtrlDataProvider&);

        CListCtrl& GetListCtrl() { return m_list; }
        const CListCtrl& GetListCtrl() const { return m_list; }

        void SetSortColumn(int col, SortDir sortDir = SortDir::Asc);
        void GetSortColumn(int& col, SortDir& sortDir) const;
        void SetFilter(const FilterCollection&);
        void GetFilter(FilterCollection&) const;

        void GetColumnHeaders(std::vector<std::string>&) const;
        void GetColumnValues(int col, std::set<std::string>&) const;

        const char* GetString(int row, int col) const { return m_dataAdapter.getString(row, col); }
        int GetStateImageIndex(int row) const { return m_dataAdapter.getStateImageIndex(row); }

        void OnCreate();
        void OnRefresh(bool autosizeColumns = false);
        void OnFilterChange(int col);
        void OnFilterBtnClick(int col);
        void OnSort(int col);
        void Resort() { doSort(); }

        void OnAppendEntry();
        void OnUpdateEntry(int entry);
        void OnDeleteEntry(int entry, bool moveLastSelected = true);

        void SelectEntry(int entry);

    protected:
        void setItemFilter(int col, const char* str);
        void refreshFilter();
        bool isMatchedToFilter(int row);
        void doSort();
        static int CALLBACK CompProc(LPARAM lparam1, LPARAM lparam2, LPARAM lparam3);
	};
}
#endif /*__PROJECT_STONE_DATA_LISTCTRLMANAGER_H__*/

