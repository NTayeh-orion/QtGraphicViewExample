#ifndef CHILDFILTERPROXYMODEL_H
#define CHILDFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

class ChildFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    using QSortFilterProxyModel::QSortFilterProxyModel;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
};

#endif // CHILDFILTERPROXYMODEL_H
