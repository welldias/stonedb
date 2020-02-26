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

#include "list_data_provider.h"

namespace Data {

    struct FilterComponent {
        enum class Operation {
            Contain = 0,
            StartWith = 1,
            ExactMatch = 2
        };

        Operation operation;
        std::string value;

        FilterComponent() : operation(Operation::Contain) {}
        FilterComponent(const char* v, Operation op = Operation::Contain) : operation(op), value(v) {}
        FilterComponent(const std::string& v, Operation op = Operation::Contain) : operation(op), value(v) {}
    };

    typedef std::vector<FilterComponent> FilterCollection;
    
    class ListCtrlManager
	{
    public:
        enum class SortDir { 
            Asc = 1, 
            Desc = -1 
        };

    private:
        //CListCtrl& m_list;
        //ListDataProvider& m_dataAdapter;
        int m_sortColumn;
        FilterCollection m_filter;
        bool m_filterEmpty;
        bool m_initalized;
        enum SortDir m_sortDir;

    public:
        ListCtrlManager(/*CListCtrl&, ListCtrlDataProvider&*/);

        //CListCtrl& GetListCtrl() { return m_list; }
        //const CListCtrl& GetListCtrl() const { return m_list; }

        void SetSortColumn(int col, SortDir sortDir = SortDir::Asc);
        void GetSortColumn(int& col, SortDir& sortDir) const;
        void SetFilter(const FilterCollection&);
        void GetFilter(FilterCollection&) const;

        void GetColumnHeaders(std::vector<std::string>&) const;
        void GetColumnValues(int col, std::set<std::string>&) const;

        //const char* GetString(int row, int col) const { return m_dataAdapter.getString(row, col); }
        //int GetStateImageIndex(int row) const { return m_dataAdapter.getStateImageIndex(row); }

        void OnCreate();
        void OnRefresh(bool autosizeColumns = false);
        void OnFilterChange(int col);
        void OnFilterBtnClick(int col);
        void OnSort(int col);
        void Resort() { DoSort(); }

        void OnAppendEntry();
        void OnUpdateEntry(int entry);
        void OnDeleteEntry(int entry, bool moveLastSelected = true);

        void SelectEntry(int entry);

    protected:
        void SetItemFilter(int col, const std::string& str);
        void RefreshFilter();
        bool IsMatchedToFilter(int row);
        void DoSort();
        static int CALLBACK CompProc(LPARAM lparam1, LPARAM lparam2, LPARAM lparam3);
	};
}
#endif /*__PROJECT_STONE_DATA_LISTCTRLMANAGER_H__*/

