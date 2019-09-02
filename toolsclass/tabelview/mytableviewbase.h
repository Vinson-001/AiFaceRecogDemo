/**
* @fileName      MyTableViewBase.h
* @brief         自定义tabelview表基类
*                支持以下功能：
*                           0.设置model()
*                           1.初始化表格(initTableView)，包括表头格式
*                           2.添加、删除表格
*                           3.排序
*                           4.设置表格宽度
*                           5.获取单元格值
*                           6.注意：首先需要初始设置表头(m_strListHeadData,子类还需要初始化model)，然后初始化表格(initTableView)
*                           7.基本用法
*                                   1）setTableModel()
*                                   2) initTableView()
* @author        Wxyang
* @date          2019-08-08
*/
#ifndef MYTABLEVIEWBASE_H
#define MYTABLEVIEWBASE_H
#include <QTableView>
#include <QHeaderView>
#include <QWidget>
#include <QStandardItemModel>
#include <QDebug>
class MyTableViewBase : public QTableView
{
    Q_OBJECT
public:
    explicit MyTableViewBase(QWidget *parent = 0);
    ~MyTableViewBase();

private:
    /* 以下为表头 */

    /* 以下为表格内容 */

public:
        QList<QString> m_strListHeadData;
        QStandardItemModel *m_model; /*模型为公用的*/

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
    void setTableModel(QStandardItemModel *model);

public slots:

signals:

};

#endif // MyTableViewBase_H
