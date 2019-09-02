#ifndef MYTABLEVIEW_H
#define MYTABLEVIEW_H

#include "./toolsclass/tabelview/mytableviewbase.h"

/**
* @fileName
* @brief         以下是设备发现用的表格类
* @author        Wxyang
* @date          2019-08-22
*/

class MyTableViewForDevice : public MyTableViewBase
{
    Q_OBJECT
public:
    explicit MyTableViewForDevice(QWidget *parent = 0);
    ~MyTableViewForDevice();

private:
    /* 以下为表格内容 */
    //QStandardItemModel *m_model;
    //QList<QString> m_strListHeadData;

private:
    void initTableView();
public:
    void initMyTableView();

};


/**
* @fileName
* @brief         以下时人脸管理：分组管理的tableview
* @author        Wxyang
* @date          2019-08-22
*/

class MyTableViewForGroupManger : public MyTableViewBase
{
    Q_OBJECT
public:
    explicit MyTableViewForGroupManger(QWidget *parent = 0);
    ~MyTableViewForGroupManger();

private:
    /* 以下为表格内容 */
    //QStandardItemModel *m_model;
    //QList<QString> m_strListHeadData;

private:
    void initTableView();
public:
    void initMyTableView();

};

#endif // MYTABLEVIEW_H
