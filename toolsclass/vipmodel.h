#ifndef VIPMODEL_H
#define VIPMODEL_H
#include <QStandardItemModel>

class VipModel : public QStandardItemModel
{
    Q_OBJECT
public:
    VipModel(QObject *parent=NULL) : QStandardItemModel(parent) { }
    VipModel(int row, int column, QObject *parent=NULL)
        : QStandardItemModel(row, column, parent) { }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
    {
        if( Qt::TextAlignmentRole == role )
            return Qt::AlignCenter;
        return QStandardItemModel::data(index, role);
    }
};

#endif // VIPMODEL_H
