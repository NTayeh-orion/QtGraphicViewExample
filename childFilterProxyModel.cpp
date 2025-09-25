#include "childFilterProxyModel.h"

// Returns true → keep the row visible in the proxy.
// Returns false → hide the row in the proxy.
// sourceRow: the row number inside sourceParent.
bool ChildFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, filterKeyColumn(), sourceParent);

    // 🔹 Case 1: Leaf (child item)
    // rowCount : # of the subrows that the row have (childs count), if it value was 0 it means that it is a child else its a parent
    // if it a child node then check it text if it's match to the search text, if yes return true else false
    if (sourceModel()->rowCount(index) == 0) {
        return sourceModel()->data(index).toString().contains(filterRegularExpression());
    }

    // 🔹 Case 2: Parent node → keep only if it has a matching child
    // rowCount : # of the subrows that the row have (childs count), if it value was 0 it means that it is a child else its a parent
    // travers on the whole tree(all of the parents and childs)
    for (int i = 0; i < sourceModel()->rowCount(index); ++i) {
        if (filterAcceptsRow(i, index))
            return true;
    }

    return false; // parent doesn't match and no child matches
}
