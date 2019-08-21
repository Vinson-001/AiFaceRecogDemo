#include "mytableview.h"
#include <QDebug>
#include "mytableviewdelegate.h"
#include "./deviceManger/devinfosys.h"
MyTableView::MyTableView(QWidget *parent)
    :QTableView(parent),m_model(NULL)
{
    m_strListHeadData << tr("序号") << tr("设备ID") << tr("设备名称") << tr("设备类型") << tr("设备序列号") \
              << tr("Mac地址") << tr("Private") << tr("IP地址") << tr("连接状态") << tr("注册状态") << tr("注册");
    initTableView();
}

MyTableView::~MyTableView()
{
    if(m_model){
        delete m_model;
        m_model = NULL;
    }
}
/**
 * @funcname  initTabelView
 * @brief     初始化表格
 * @param     colCount
 * @param     strListHeadData
 * @return    no
 */

void MyTableView::initTableView()
{
    int colCount = m_strListHeadData.count();
    if(m_strListHeadData.isEmpty())
    {
        qDebug() << "initTabelView failed!";
        return;
    }
    m_model = new QStandardItemModel(this);
    m_model->setColumnCount(colCount);
    for(int i = 0; i < colCount; i++)
    {
        m_model->setHeaderData(i,Qt::Horizontal,m_strListHeadData.at(i));
    }
 #if 0
    m_tableviewModel->setHeaderData(0,Qt::Horizontal,tr("序号"));
    m_tableviewModel->setHeaderData(1,Qt::Horizontal,tr("设备ID"));
    m_tableviewModel->setHeaderData(2,Qt::Horizontal,tr("设备名称"));
    m_tableviewModel->setHeaderData(3,Qt::Horizontal,tr("设备序列号"));
    m_tableviewModel->setHeaderData(4,Qt::Horizontal,tr("私有数据"));
    m_tableviewModel->setHeaderData(5,Qt::Horizontal,tr("IP地址"));
    m_tableviewModel->setHeaderData(6,Qt::Horizontal,tr("状态"));
#endif
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
    connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slotRowDoubleClicked(const QModelIndex &)));
    //connect(mytableDelegate,SIGNAL(addRegisterSingal()),this, SLOT(addRegisterSlot()));
    //connect(mytableDelegate,SIGNAL(editRegisterSignal()),this, SLOT(editRegisterSlot()));
}
/**
 * @funcname  addRowTextForTable
 * @brief     添加一行显示
 * @param     rowConut
 * @param     strTextList
 * @return    no
 */

void MyTableView::addRowTextForTable(int rowConut, QList<QString> strTextList)
{
    QString strNum;
    strNum = QString("%1").arg(rowConut);
    strTextList.prepend(strNum);
    for(int i = 0; i < strTextList.count(); i++) {
        m_model->setItem(rowConut, i, new QStandardItem(strTextList[i]));
        m_model->item(rowConut,i)->setTextAlignment(Qt::AlignCenter);
    }
}
/**
 * @funcname  addRowForTable
 * @brief     新增一行
 * @param     strTexList(不需要插入序号值)
 * @return    no
 */

void MyTableView::addRowForTable(QList<QString> strTextList)
{
    int rowIndex = m_model->rowCount();
    if(m_model == NULL)
        return;
    m_model->insertRow(rowIndex);
    addRowTextForTable(rowIndex,strTextList);
    //SetTabViewColumnSpace();
}
/**
 * @funcname  removeRowForTable
 * @brief     删除选中的行
 * @return    no
 */

void MyTableView::removeRowForTable()
{
    int rowIndex = this->currentIndex().row();
    if(m_model == NULL)
        return;
    if (rowIndex != -1)
        m_model->removeRow(rowIndex);
    sortColForTable();
}
/**
 * @funcname  removeAllRowForTable
 * @brief     删除所有的行
 * @return    ret
 */

void MyTableView::removeAllRowForTable()
{
    if(m_model == NULL)
        return;
    m_model->removeRows(0,m_model->rowCount());
}
/**
 * @funcname  sortColForTable
 * @brief     排序
 * @return    no
 */

void MyTableView::sortColForTable()
{

    int rowcount = m_model->rowCount();
    for (int i = 0; i < rowcount; ++i) {
       QModelIndex index= m_model->index(i, 0);
       m_model->setData(index, QVariant::fromValue(i));
    }

}
/**
 * @funcname  SetTabViewColumnSpace
 * @brief     根据内容调整宽度
 * @return    no
 */
void MyTableView::SetTabViewColumnSpace()
{
    //根据内容来确定列宽度
       this->resizeColumnsToContents();
       this->horizontalHeader();
       //获取表头列数
       for(int i = 0; i < this->horizontalHeader()->count(); i++)
       {
           this->setColumnWidth(i, this->columnWidth(i) + 10);  //多一些空余控件，不然每列内容很挤
       }
       //m_tableView->horizontalHeader()->setStretchLastSection(true);        //最后一列补全所有空白位置
}
/**
 * @funcname  doTable
 * @brief     gaiyao
 * @param     operate
 * @param     strTextList
 * @return    no
 */

void MyTableView::doTable(QString operate, QList<QString> strTextList)
{
    if(operate == "add") {
        addRowForTable(strTextList);
    } else if(operate == "remove")
    {
        removeRowForTable();
    } else if(operate == "removeAll")
    {
        removeAllRowForTable();
    }
}
/**
 * @funcname  getValueFromRowAndCol
 * @brief     获取单元格的值(row,col)
 * @param     row 从1开始
 * @param     col
 * @return    QString
 */

QString MyTableView::getValueFromRowAndCol(int row, int col)
{
    QModelIndex index=m_model->index(row-1,col-1,QModelIndex());//rowNum,columnNum为行列号 从0开始
    QString str= index.data().toString();
    //qDebug() << str;
    return str;
}
/**
 * @funcname  getValueFromRow
 * @brief     获取一行的值，去掉序号
 * @param     param1
 * @param     param2
 * @return    ret
 */
QList<QString> MyTableView::getValueFromRow(int row)
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
/**
 * @funcname  setTableColWidth
 * @brief     设置表格宽度
 * @param     param1
 * @param     param2
 * @return    no
 */
void MyTableView::setTableColWidth(int width)
{
    this->setColumnWidth(0, (1 * width/31));
    this->setColumnWidth(1, (2 * width/31));    /*0~65535*/
    this->setColumnWidth(2, (3 * width/31));
    this->setColumnWidth(3, (3 * width/31));
    this->setColumnWidth(4, (4 * width/31));
    this->setColumnWidth(5, (5 * width/31));
    this->setColumnWidth(6, (3 * width/31));
    this->setColumnWidth(7, (4 * width/31));
    this->setColumnWidth(8, (2 * width/31));
    this->setColumnWidth(9, (2 * width/31));
    this->setColumnWidth(10, (2 * width/31));
}
/**
 * @funcname  slotRowDoubleClicked
 * @brief     双击
 * @param     index
 */

void MyTableView::slotRowDoubleClicked(const QModelIndex &index)
{
    //QModelIndex index = this->currentIndex();
    int row = index.row();
    if (!index.isValid())
    {
        return;
    }
    //qDebug() << row;
    //DevInfoSys dlg(this);
    //dlg.exec();

}
