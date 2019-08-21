/**
* @fileName      MyTableView.h
* @brief         自定义tabelview表
* @author        Wxyang
* @date          2019-08-08
*/
#ifndef MYTABLEVIEW_H
#define MYTABLEVIEW_H
#include <QTableView>
#include <QHeaderView>
#include <QWidget>
#include <QStandardItemModel>
class MyTableView : public QTableView
{
    Q_OBJECT
public:
    explicit MyTableView(QWidget *parent = 0);
    ~MyTableView();

private:
    /* 以下为表头 */

    /* 以下为表格内容 */
    QStandardItemModel *m_model;
    QList<QString> m_strListHeadData;

private:
    /* 以下为表格内容 */
    void initTableView();
    void addRowTextForTable(int rowConut,QList<QString> strTextList);
    void addRowForTable(QList<QString> strTextList);
    void removeRowForTable();
    void removeAllRowForTable();
    void sortColForTable();
    void SetTabViewColumnSpace();


public:
    /* 以下为表格内容 */
    void doTable(QString operate, QList<QString> strTextList);
    QString getValueFromRowAndCol(int row,int col);
    QList<QString> getValueFromRow(int row);
    void setTableColWidth(int width);

public slots:
    void slotRowDoubleClicked(const QModelIndex &index);

signals:

};

#endif // MYTABLEVIEW_H
