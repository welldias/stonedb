#include "list_manager.h"

namespace Data {

    ListCtrlManager::ListCtrlManager(/*CListCtrl&, ListCtrlDataProvider&*/) {

    }

    void ListCtrlManager::SetSortColumn(int col, SortDir sortDir) {
        m_sortColumn = col;
        m_sortDir = sortDir;
    }

    void ListCtrlManager::SetFilter(const FilterCollection& filterCollection) {

    }

    void ListCtrlManager::GetColumnHeaders(std::vector<std::string>&) const {
    }

    void ListCtrlManager::GetColumnValues(int col, std::set<std::string>&) const {

    }

    void ListCtrlManager::OnCreate() {

    }

    void ListCtrlManager::OnRefresh(bool autosizeColumns) {

    }

    void ListCtrlManager::OnFilterChange(int col) {

    }

    void ListCtrlManager::OnFilterBtnClick(int col) {

    }

    void ListCtrlManager::OnSort(int col) {

    }

    void ListCtrlManager::OnAppendEntry() {

    }

    void ListCtrlManager::OnUpdateEntry(int entry) {

    }

    void ListCtrlManager::OnDeleteEntry(int entry, bool moveLastSelected) {

    }

    void ListCtrlManager::SelectEntry(int entry) {

    }

    void ListCtrlManager::GetSortColumn(int& col, SortDir& sortDir) const {
        col = m_sortColumn;
        sortDir = m_sortDir;
    }

    void ListCtrlManager::GetFilter(FilterCollection& filter) const {
        filter = m_filter;
    }

    void ListCtrlManager::SetItemFilter(int col, const std::string& str) {

    }

    void ListCtrlManager::RefreshFilter() {

    }

    bool ListCtrlManager::IsMatchedToFilter(int row) {
        return false;
    }

    void ListCtrlManager::DoSort() {

    }

    int CALLBACK ListCtrlManager::CompProc(LPARAM lparam1, LPARAM lparam2, LPARAM lparam3) {
        return 0;
    }

}