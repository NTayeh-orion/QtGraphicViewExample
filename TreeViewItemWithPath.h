#ifndef TREEVIEWITEMWITHPATH_H
#define TREEVIEWITEMWITHPATH_H

#include <QStandardItem>

class TreeViewItemWithPath : public QStandardItem
{
public:
    TreeViewItemWithPath(const QString &text, const QString &path);
    QString dirPAth;

    QString getPath();
};

#endif // TREEVIEWITEMWITHPATH_H
