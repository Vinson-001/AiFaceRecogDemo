#ifndef READONLYDELEGATEFORTABLEVIEW_H
#define READONLYDELEGATEFORTABLEVIEW_H
#include <QtGui>
#include <QItemDelegate>

//编号列，只读委托
class ReadOnlyDelegateforTableView : public QItemDelegate
{
    Q_OBJECT
public:
      ReadOnlyDelegateforTableView(QObject *parent = 0): QItemDelegate(parent) { }
      QWidget *createEditor(QWidget*parent, const QStyleOptionViewItem &option,
          const QModelIndex &index) const
      {
          return NULL;
      }

};

#endif // READONLYDELEGATEFORTABLEVIEW_H
