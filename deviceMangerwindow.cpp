#include "deviceMangerwindow.h"
#include "ui_deviceMangerwindow.h"

#include "./uilib/iconhelper.h"


#define iconSize    50
#define iconWidth   50
#define iconHeight  45
#define navBtnWidth 45
#define navIcoWidth 18

#define ColorBg "#34495E"
#define ColorFontText "#FEFEFE"
#define ColorChar 0xf085
#define ColorlistText tr("设备管理")





DeviceMangerWindow::DeviceMangerWindow(QWidget *parent):
    QDialog(parent),
    ui(new Ui::DeviceMangerWindow)
{
    m_tableviewModel = NULL;
    m_SendThread = NULL;
    m_SendObjThread = NULL;
    m_strfilePath = "";
    ui->setupUi(this);

    this->initForm();
    this->initNav();
    this->initDevDiscovery();
    qRegisterMetaType<QList<QList<QString>>>("QList<QList<QString>>");

}


DeviceMangerWindow::~DeviceMangerWindow()
{
    stopDevSendThread();
   //stopDevRecvThread();
    delete ui;
}

void DeviceMangerWindow::initForm()
{
    QList<QToolButton *> btns;
    QList<QChar> listChar;          /* icon */
    QList<QString> listText;        /* name*/
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    this->setProperty("canMove", true);


    IconHelper::Instance()->setIcon(ui->btnReTurnMenu, 0xf122, navIcoWidth);
    //IconHelper::Instance()->setIcon(ui->toolButtonFind, 0xf002, navIcoWidth);

    btns << ui->btnDevice << ui->btnDevFind << ui->btnAdd << ui->btnDelete;                /*buttons*/
    listChar << 0xf085 << 0xf002 << 0xf055 << 0xf056;
    listText << tr("设备管理") << tr("设备发现") << tr("添加") << tr("删除");

    for(int i = 0; i < btns.count(); i++) {
        QToolButton *btn = btns.at(i);
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        btn->setIconSize(QSize(iconWidth, iconHeight));

        QPixmap pix = IconHelper::Instance()->getPixmap(ColorFontText, listChar.at(i), iconSize/2, iconWidth, iconHeight);

        btn->setIcon(QIcon(pix));
        btn->setText(listText.at(i));

    }
    //connect(btns.at(0), SIGNAL(clicked(bool)), this, SLOT(btnViewPeopleClicked()));
    connect(btns.at(1), SIGNAL(clicked(bool)), this, SLOT(btnDevFindClicked()),Qt::QueuedConnection);
    connect(btns.at(2), SIGNAL(clicked(bool)), this, SLOT(btnAddClicked()));
    connect(btns.at(3), SIGNAL(clicked(bool)), this, SLOT(btnDeleteClicked()));
    //connect(btns.at(4), SIGNAL(clicked(bool)), this, SLOT(btnDeviceMangerClicked()));
    //connect(btns.at(5), SIGNAL(clicked(bool)), this, SLOT(btnConfigClicked()));
    initTableView();


}

void DeviceMangerWindow::initNav()
{

#if 1
    QStringList qss;
    QFile qssFile(":/qrc/qss/devicemangerwindow.qss");

    qssFile.open(QFile::ReadOnly);

   if(qssFile.isOpen())

    {
        this->setStyleSheet(tr(qssFile.readAll()));
        qssFile.close();
    }
#endif

   m_strfilePath = QDir::currentPath();
   m_strfilePath += "/DevInfo.xml";

}
/*
 * 初始化tableview
 */
void DeviceMangerWindow::initTableView()
{
    m_tableviewModel = new QStandardItemModel(this);
    m_tableviewModel->setColumnCount(7);
   // m_tableviewModel->setHeaderData(0,Qt::Horizontal,tr("序号"));
    m_tableviewModel->setHeaderData(0,Qt::Horizontal,tr("序号"));
    m_tableviewModel->setHeaderData(1,Qt::Horizontal,tr("设备ID"));
    m_tableviewModel->setHeaderData(2,Qt::Horizontal,tr("设备名称"));
    m_tableviewModel->setHeaderData(3,Qt::Horizontal,tr("设备序列号"));
    m_tableviewModel->setHeaderData(4,Qt::Horizontal,tr("私有数据"));
    m_tableviewModel->setHeaderData(5,Qt::Horizontal,tr("IP地址"));
    m_tableviewModel->setHeaderData(6,Qt::Horizontal,tr("状态"));

    //单元格格式
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignVCenter |Qt::AlignHCenter );    //表头信息居中
    ui->tableView->horizontalHeader()->setObjectName("hHeader");
    ui->tableView->verticalHeader()->setObjectName("vHeader");
    ui->tableView->setAlternatingRowColors(true);                                                 /* 奇偶行颜色区分 */
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch );                  /*设置表格自适应宽度*/

    ui->tableView->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    ui->tableView->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    //ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch); /*自适应行高*/

    ui->tableView->setModel(m_tableviewModel);
    //QList<QString> listText;
    //listText << "1" << "id1" << "device1" << "sn1" << "193.169.3.111" << "在线" << tr("");
    //addTabelViewRowText(m_tableviewModel,0,listText);
    //addTabelViewRow();

}
/*
 * 添加一行数据到表头
 */
void DeviceMangerWindow::addTabelViewRowText(QStandardItemModel *model, int row, QList<QString> listText)
{
    QString strNum;
    strNum = QString("%1").arg(row);
    listText.prepend(strNum);
    for(int i = 0; i < listText.count(); i++) {
        model->setItem(row, i, new QStandardItem(listText[i]));
        model->item(row,i)->setTextAlignment(Qt::AlignCenter);
    }
}
/*
 * 增加一行
 */
void DeviceMangerWindow::addTabelViewRow(QList<QString> listText)
{
    int rowIndex = m_tableviewModel->rowCount();
    m_tableviewModel->insertRow(rowIndex);
    addTabelViewRowText(m_tableviewModel,rowIndex,listText);
}

void DeviceMangerWindow::removeTableRow()
{
    int rowIndex = ui->tableView->currentIndex().row();
     if (rowIndex != -1)
         m_tableviewModel->removeRow(rowIndex);
}

void DeviceMangerWindow::removeAllRow()
{
    m_tableviewModel->removeRows(0,m_tableviewModel->rowCount());
}

void DeviceMangerWindow::initDevDiscovery()
{
    startDevSendThread();
}

/*
 * 开始发送线程
 */
void DeviceMangerWindow::startDevSendThread()
{
    if(m_SendThread)
    {
        return;
    }
    m_SendThread= new QThread();
    m_SendObjThread = new DevSendThread();
    m_SendObjThread->moveToThread(m_SendThread);
    connect(m_SendThread,&QThread::finished,m_SendThread,&QObject::deleteLater);
    connect(m_SendThread,&QThread::finished,m_SendObjThread,&QObject::deleteLater);
    //绑定设备请求
    //connect(this,&DeviceMangerWindow::startUdpRecvData,m_SendObjThread,&DevSendThread::udpRecvData);
    connect(this,&DeviceMangerWindow::startUdpSendData,m_SendObjThread,&DevSendThread::startSending);
    connect(m_SendObjThread,&DevSendThread::startUpdateDevInfo,this,&DeviceMangerWindow::updateDevInfoDisplay);

    m_SendThread->start();

}


void DeviceMangerWindow::stopDevSendThread()
{
    qDebug() << "stop m_SendThread";

    if(m_SendThread)
    {
        if(m_SendObjThread){
            m_SendObjThread->enableSendThread(false);
        }
        m_SendThread->quit();
    }
    m_SendThread->wait();
    qDebug() << "end m_SendThread";
}


void DeviceMangerWindow::mouseMoveEvent(QMouseEvent *event)
{

    int dx = event->globalX() - lastPos.x();
    int dy = event->globalY() - lastPos.y();
    lastPos = event->globalPos();
    move(x()+dx, y()+dy);

}

void DeviceMangerWindow::mousePressEvent(QMouseEvent *event)
{

    lastPos = event->globalPos();

}

void DeviceMangerWindow::mouseReleaseEvent(QMouseEvent *event)
{

    int dx = event->globalX() - lastPos.x();
    int dy = event->globalY() - lastPos.y();
    move(x()+dx, y()+dy);
}
/*
 * 返回主菜单
 */
void DeviceMangerWindow::on_btnReTurnMenu_clicked()
{

    this->hide();               /*隐藏当前对话框 */
    emit showwnd();             /*显示主窗口 */

}

void DeviceMangerWindow::getshowdlg()
{
    this->show();               /*显示对话框*/
}
/*
 * 开始设备搜索
 */
void DeviceMangerWindow::btnDevFindClicked()
{

    //QThread::sleep(1);
    //emit startUdpRecvData();

    emit startUdpSendData();
}

void DeviceMangerWindow::btnAddClicked()
{
    QList<QString> listText;
    listText << "1" << "id1" << "device1" << "sn1" << "prviate" << "193.169.3.111" << tr("在线");
    addTabelViewRow(listText);
}

void DeviceMangerWindow::btnDeleteClicked()
{
    removeTableRow();
}

void DeviceMangerWindow::updateDevInfoDisplay(bool isUpdate)
{
    //QList<QString> listText;
    //listText << "1" << "id" << "dev" << "sn" << "" << "" << tr("在线");
   // for(int i = 0 ; i < strListText.count(); i++)
    {
        //listText[i+1] = strListText[i];
    }
    //addTabelViewRow(listText);
    //qDebug() << "str " << strListText;
    QList<QList<QString>> strListListText;
    CDomXmlAnalysis domxml(m_strfilePath,"");
    domxml.ReadXml(DevInfoUdpType);
    removeAllRow();                                     /*先清空*/
    for(int i = 0 ; i < domxml.m_strListListText.count(); i++)
    {
        strListListText.append(domxml.m_strListListText.at(i));
        addTabelViewRow(strListListText.at(i));
    }
    qDebug() << strListListText;
}




