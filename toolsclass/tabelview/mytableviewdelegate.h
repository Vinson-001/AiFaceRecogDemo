/**
* @fileName      MyTableViewDelegate.h
* @brief         自定义委托,添加button
* @author        Wxyang
* @date          2019-08-08
*/
#ifndef MYTABLEVIEWDELEGATE_H
#define MYTABLEVIEWDELEGATE_H
#include <QStyledItemDelegate>
#include <QScopedPointer>
#include <QPushButton>
#include <QApplication>
#include <QMouseEvent>
#include <QToolTip>
class MyTableviewDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit MyTableviewDelegate(QWidget *parent = 0);

    ~MyTableviewDelegate();
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index);

signals:
    void addRegisterSingal(const QModelIndex &index);
    void editRegisterSignal(const QModelIndex &index);

private:
    QPoint m_mousePoint;  // 鼠标位置
    QScopedPointer<QPushButton> m_pAddButton;
    QScopedPointer<QPushButton> m_pEditButton;
    QStringList m_list;
    int m_nSpacing;      // 按钮之间的间距
    int m_nWidth;        // 按钮宽度
    int m_nHeight;       // 按钮高度
    int m_nType;         // 按钮状态-1：划过 2：按下

};

#endif // MYTABLEVIEWDELEGATE_H
