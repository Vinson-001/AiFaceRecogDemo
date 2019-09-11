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
    QList<QString> getValueFromRow(int row);

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


/**
* @fileName      MyTableViewForFaceQuery
* @brief         以下是人脸管理：人脸查询的tableview
* @author        Wxyang
* @date          2019-09-05
*/

class MyTableViewForFaceQuery : public MyTableViewBase
{
    Q_OBJECT
public:
    explicit MyTableViewForFaceQuery(QWidget *parent = 0);
    ~MyTableViewForFaceQuery();

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
* @brief         以下是人脸管理中，批量管理表格
* @author        Wxyang
* @date          2019-09-06
*/

class MyTableViewForImportMulPic : public MyTableViewBase
{
    Q_OBJECT
public:
    explicit MyTableViewForImportMulPic(QWidget *parent = 0);
    ~MyTableViewForImportMulPic();

private:
    /* 以下为表格内容 */
    //QStandardItemModel *m_model;
    //QList<QString> m_strListHeadData;

private:
    void initTableView();
public:
    void initMyTableView();
    void setTableColWidth(int width);

};
#endif // MYTABLEVIEW_H
