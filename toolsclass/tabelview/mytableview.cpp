#include "mytableview.h"

/**
* @fileName
* @brief         以下是设备发现用的表格类
* @author        Wxyang
* @date          2019-08-22
*/

MyTableViewForDevice::MyTableViewForDevice(QWidget *parent):
        MyTableViewBase(parent)
{
    m_strListHeadData << tr("序号") << tr("设备ID") << tr("设备名称") << tr("设备类型") << tr("设备序列号") \
              << tr("Mac地址") << tr("Private") << tr("IP地址") << tr("连接状态") << tr("注册状态") << tr("注册");
    //initTableView();
}

MyTableViewForDevice::~MyTableViewForDevice()
{
    if(m_model){
        delete m_model;
        m_model = NULL;
    }
}
/**
 * @funcname  initTableView
 * @brief     初始化表格样式，重载
 * @return    no
 */
void MyTableViewForDevice::initTableView()
{
    int colCount = m_strListHeadData.count();
    if(m_strListHeadData.isEmpty())
    {
        qDebug() << "initTabelView failed!";
        return;
    }
    //m_model = new QStandardItemModel(this);
    if(m_model == NULL)
        return;
    m_model->setColumnCount(colCount);
    for(int i = 0; i < colCount; i++)
    {
        m_model->setHeaderData(i,Qt::Horizontal,m_strListHeadData.at(i));
    }

    //单元格格式
    this->horizontalHeader()->setDefaultAlignment(Qt::AlignVCenter |Qt::AlignHCenter );    //表头信息居中
    this->horizontalHeader()->setObjectName("hHeader");
    this->verticalHeader()->setObjectName("vHeader");
    this->setAlternatingRowColors(true);                                                 /* 奇偶行颜色区分 */
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed );                  /*设置表格自适应等宽度*/

    this->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    this->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    //ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch); /*自适应行高*/
    this->setEditTriggers(QAbstractItemView::NoEditTriggers); /*禁止编辑*/
    this->setMouseTracking(true);
    this->setModel(m_model);
}
/**
 * @funcname  initMyTableView
 * @brief     初始化表格（前提是先设置model）
 * @return    no
 */
void MyTableViewForDevice::initMyTableView()
{
    initTableView();
}
/**
 * @funcname  getValueFromRow
 * @brief     重载，获取特定的值
 * @param     去掉序号
 * @param     param2
 * @return    ret
 */
QList<QString> MyTableViewForDevice::getValueFromRow(int row)
{
    QList<QString> strList;
    int count = m_strListHeadData.count();
    for(int i = 1; i < count-2; i++) {
        QString str;
        str = getValueFromRowAndCol(row,i+1);   /*col从1开始*/
        strList.append(str);
    }
    return strList;
}

//----------------------------------------------------------------------------------------------------//
/**
* @fileName
* @brief         以下时人脸管理：分组管理的tableview
* @author        Wxyang
* @date          2019-08-22
*/


MyTableViewForGroupManger::MyTableViewForGroupManger(QWidget *parent):
    MyTableViewBase(parent)
{
    m_strListHeadData  << tr("序号") << tr("分组ID") << tr("成员数") << tr("组信息");
}

MyTableViewForGroupManger::~MyTableViewForGroupManger()
{
    if(m_model){
        delete m_model;
        m_model = NULL;
    }
}
/**
 * @funcname  initTableView
 * @brief     初始化表格样式，重载
 * @return    no
 */
void MyTableViewForGroupManger::initTableView()
{
    int colCount = m_strListHeadData.count();
    if(m_strListHeadData.isEmpty())
    {
        qDebug() << "initTabelView failed!";
        return;
    }
    //m_model = new QStandardItemModel(this);
    if(m_model == NULL)
        return;
    m_model->setColumnCount(colCount);
    for(int i = 0; i < colCount; i++)
    {
        m_model->setHeaderData(i,Qt::Horizontal,m_strListHeadData.at(i));
    }

    //单元格格式
    this->horizontalHeader()->setDefaultAlignment(Qt::AlignVCenter |Qt::AlignHCenter );    //表头信息居中
    this->horizontalHeader()->setObjectName("hHeader");
    this->verticalHeader()->setObjectName("vHeader");
    this->setAlternatingRowColors(true);                                                 /* 奇偶行颜色区分 */
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);                  /*设置表格自适应等宽度*/

    this->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    this->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    //ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch); /*自适应行高*/
    this->verticalHeader()->hide();                             /*隐藏列序号*/
    //this->setEditTriggers(QAbstractItemView::NoEditTriggers); /*禁止编辑*/
    this->setMouseTracking(true);
    this->setModel(m_model);
}

/**
 * @funcname  initMyTableView
 * @brief     初始化表格（前提是先设置model）
 * @return    no
 */
void MyTableViewForGroupManger::initMyTableView()
{
    initTableView();
}

//-----------------------------------------------------------------------------------------------


MyTableViewForFaceQuery::MyTableViewForFaceQuery(QWidget *parent)
    :MyTableViewBase(parent)
{
     m_strListHeadData  << tr("序号") << tr("索引") << tr("ID") << \
                           tr("姓名") << tr("性别") << tr("年龄") << tr("其他");
}

MyTableViewForFaceQuery::~MyTableViewForFaceQuery()
{
    if(m_model){
        delete m_model;
        m_model = NULL;
    }
}
/**
 * @funcname  initTableView
 * @brief     初始化表格样式，重载
 * @return    no
 */
void MyTableViewForFaceQuery::initTableView()
{
    int colCount = m_strListHeadData.count();
    if(m_strListHeadData.isEmpty())
    {
        qDebug() << "initTabelView failed!";
        return;
    }
    //m_model = new QStandardItemModel(this);
    if(m_model == NULL)
        return;
    m_model->setColumnCount(colCount);
    for(int i = 0; i < colCount; i++)
    {
        m_model->setHeaderData(i,Qt::Horizontal,m_strListHeadData.at(i));
    }

    //单元格格式
    this->horizontalHeader()->setDefaultAlignment(Qt::AlignVCenter |Qt::AlignHCenter );    //表头信息居中
    this->horizontalHeader()->setObjectName("hHeader");
    this->verticalHeader()->setObjectName("vHeader");
    this->setAlternatingRowColors(true);                                                 /* 奇偶行颜色区分 */
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);                  /*设置表格自适应等宽度*/

    this->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    this->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    //ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch); /*自适应行高*/
    this->verticalHeader()->hide();                             /*隐藏列序号*/
    //this->setEditTriggers(QAbstractItemView::NoEditTriggers); /*禁止编辑*/
    this->setMouseTracking(true);
    this->setModel(m_model);
}
/**
 * @funcname  initMyTableView
 * @brief     初始化表格（前提是先设置model）
 * @return    no
 */
void MyTableViewForFaceQuery::initMyTableView()
{
    initTableView();
}
//---------------------------------------------------------------------------------------------------------

MyTableViewForImportMulPic::MyTableViewForImportMulPic(QWidget *parent)
    :MyTableViewBase(parent)
{
    m_strListHeadData  << tr("序号") << tr("姓名") << tr("性别") << tr("年龄") \
                       << tr("组ID") << tr("人ID") << tr("图片路径") << tr("特征值路径")\
                       << tr("音频支持") << tr("音频路径");
}

MyTableViewForImportMulPic::~MyTableViewForImportMulPic()
{
    if(m_model){
        delete m_model;
        m_model = NULL;
    }
}
/**
 * @funcname  initTableView
 * @brief     初始化表格样式，重载
 * @return    no
 */
void MyTableViewForImportMulPic::initTableView()
{
    int colCount = m_strListHeadData.count();
    if(m_strListHeadData.isEmpty())
    {
        qDebug() << "initTabelView failed!";
        return;
    }
    //m_model = new QStandardItemModel(this);
    if(m_model == NULL)
        return;
    m_model->setColumnCount(colCount);
    for(int i = 0; i < colCount; i++)
    {
        m_model->setHeaderData(i,Qt::Horizontal,m_strListHeadData.at(i));
    }

    //单元格格式
    this->horizontalHeader()->setDefaultAlignment(Qt::AlignVCenter |Qt::AlignHCenter );    //表头信息居中
    this->horizontalHeader()->setObjectName("hHeader");
    this->verticalHeader()->setObjectName("vHeader");
    this->setAlternatingRowColors(true);                                                 /* 奇偶行颜色区分 */
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);                  /*设置表格自适应等宽度*/

    this->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    this->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    //ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch); /*自适应行高*/
    this->verticalHeader()->hide();                             /*隐藏列序号*/
    //this->setEditTriggers(QAbstractItemView::NoEditTriggers); /*禁止编辑*/
    this->setMouseTracking(true);
    this->setModel(m_model);
}
/**
 * @funcname  initTableView
 * @brief     初始化表格样式，重载
 * @return    no
 */
void MyTableViewForImportMulPic::initMyTableView()
{
    initTableView();
}
/*设置表格宽度*/
void MyTableViewForImportMulPic::setTableColWidth(int width)
{
    this->setColumnWidth(0, (2 * width/30));    /*序号*/
    this->setColumnWidth(1, (3 * width/30));    /*姓名*/
    this->setColumnWidth(2, (2 * width/30));    /*性别*/
    this->setColumnWidth(3, (2 * width/30));    /*年龄*/
    this->setColumnWidth(4, (2 * width/30));    /*gid*/
    this->setColumnWidth(5, (3 * width/30));    /*pid*/
    this->setColumnWidth(6, (4 * width/30));    /*图片大小*/
    this->setColumnWidth(7, (4 * width/30));    /*特征值大小*/
    this->setColumnWidth(8, (4 * width/30));    /*音频支持*/
    this->setColumnWidth(9, (4 * width/30));    /*音频长度*/
}
