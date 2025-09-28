#include "TreeViewItemWithPath.h"

TreeViewItemWithPath::TreeViewItemWithPath(const QString &text, const QString &path)
    : QStandardItem(text) // initialize with display text
{
    setData(path, Qt::UserRole); // store path in user role
}

QString TreeViewItemWithPath::getPath()
{
    return data(Qt::UserRole).toString();
}
