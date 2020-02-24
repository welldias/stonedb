#include "list_ctrl_manager.h"

namespace Data {

    void ListCtrlManager::SetSortColumn(int col, SortDir sortDir) {
        m_sortColumn = col;
        m_sortDir = sortDir;
    }

    void ListCtrlManager::GetSortColumn(int& col, SortDir& sortDir) const {
        col = m_sortColumn;
        sortDir = m_sortDir;
    }

    void ListCtrlManager::GetFilter(FilterCollection& filter) const {
        filter = m_filter;
    }
}