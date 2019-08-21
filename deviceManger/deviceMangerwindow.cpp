#include "deviceMangerwindow.h"
#include "ui_deviceMangerwindow.h"

#include "./uilib/iconhelper.h"
#include "./deviceManger/devinfosys.h"
#include <QMessageBox>

#define iconSize    50
#define iconWidth   50
#define iconHeight  45
#define navBtnWidth 45
#define navIcoWidth 18

#define ColorBg "#34495E"
#define ColorFontText "#FEFEFE"
#define ColorChar 0xf085
#define ColorlistText tr("设备管理")
static const QList<QString> g_mapKey= {"DeviceID","DeviceName","DeviceIP","FtpServerIP","FtpServerCapPath",\
                                   "FtpServerRecoPath","HostRealTimeEventPort","HostAlarmEventPort"};


DeviceMangerWindow::DeviceMangerWindow(QWidget *parent):
    QDialog(parent),
    ui(new Ui::DeviceMangerWindow),
    m_myTableView(NULL),
    m_reply(NULL)
{
    m_SendThread = NULL;
    m_SendObjThread = NULL;
    m_strRegisterPath = "";
    m_strUnRegisterPath = "";
    ui->setupUi(this);
    m_isUpdate = true;
    m_isAdd = false;
    m_isModified = false;
    this->initForm();
    this->initNav();
    this->initDevDiscovery();
    qRegisterMetaType<QList<QList<QString>>>("QList<QList<QString>>");

}


DeviceMangerWindow::~DeviceMangerWindow()
{
    stopDevSendThread();
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

    btns << ui->btnDevice << ui->btnDevFind << ui->btnAdd << ui->btnDelete << ui->btnClear;                /*buttons*/
    listChar << 0xf085 << 0xf002 << 0xf055 << 0xf056 << 0xf1f8;
    listText << tr("设备管理") << tr("设备探测") << tr("添加") << tr("删除") << tr("清空");

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
    connect(btns.at(4), SIGNAL(clicked(bool)), this, SLOT(btnRemoveAllClicked()));
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
    //ui->btnAdd->setDisabled(true);
    //ui->btnDelete->setDisabled(true);
    QString strTemp = QDir::currentPath();
    m_strRegisterPath = strTemp + "/DevInfoRegister.xml";
    m_strUnRegisterPath = strTemp + "/DevInfoUnRegister.xml";
    //displayRegisterInfo();
    //displayUnRegisterInfo();

}
/*
 * 初始化tableview
 */
void DeviceMangerWindow::initTableView()
{
    m_myTableView = new  MyTableView(this);
    m_myTableView->resize(930,500);
    m_myTableView->setTableColWidth(930);
    m_myTableView->move(60,220);
    MyTableviewDelegate *mytableDelegate = new MyTableviewDelegate(this);
    m_myTableView->setItemDelegate(mytableDelegate);
    connect(m_myTableView,SIGNAL(entered(QModelIndex)),this, SLOT(showToolTip(QModelIndex)));
    connect(mytableDelegate,SIGNAL(addRegisterSingal(const QModelIndex)),this, SLOT(addRegisterSlot(const QModelIndex)));
    connect(mytableDelegate,SIGNAL(editRegisterSignal(const QModelIndex)),this, SLOT(editRegisterSlot(const QModelIndex)));


}

/*
 * 增加一行
 */
void DeviceMangerWindow::addTabelViewRow(QList<QString> listText)
{
    m_myTableView->doTable("add",listText);
}
/*
 * 选中行删除
 */
void DeviceMangerWindow::removeTableRow()
{
    QList<QString> listText;
    m_myTableView->doTable("remove",listText);
}

void DeviceMangerWindow::removeAllRow()
{
    QList<QString> listText;
    m_myTableView->doTable("removeAll",listText);
}
/*
 * 获取(row,col) 单元格的值
 */
QString DeviceMangerWindow::getDataFromRowAndCol(int row, int col)
{
    QString str;
    str = m_myTableView->getValueFromRowAndCol(row,col);
    return str;
}
/**
 * @funcname  getDataFromRow
 * @brief     获取一行的值
 * @param     row
 * @return    无
 */
QList<QString> DeviceMangerWindow::getDataFromRow(int row)
{
    QList<QString> strList;
    strList = m_myTableView->getValueFromRow(row);
    return strList;
}
/**
 * @funcname  displayRegisterInfo
 * @brief     显示注册信息
 * @param     无
 * @param     无
 * @return    无
 */
void DeviceMangerWindow::displayRegisterInfo()
{
    removeAllRow();
    //1. 获取所有ID
    QList<QString> strIdList;
    DomXmlAnalysisForRegister regXml(m_strRegisterPath);
    regXml.getDevIdList(strIdList);
    int count = strIdList.count();
    //2. 显示
    for(int i = 0; i < count;i++){
        QList<QString> strList;
        QString strId = strIdList.at(i);
        bool flag = regXml.getDevInfo(strId,strList);
        if(flag){
            //strList.prepend(strId);
            strList.append(tr("已注册"));
            addTabelViewRow(strList);
        }
    }


}
/**
 * @funcname  displayUnRegisterInfo
 * @brief     显示未注册表信息（未注册表存的临时信息，ID从0开始，假的）
 * @return    无
 */
void DeviceMangerWindow::displayUnRegisterInfo()
{
    //1. 获取所有ID
    QList<QString> strIdList;
    DomXmlAnalysisForUnRegister regXml(m_strUnRegisterPath);
    regXml.getDevIdList(strIdList);
    int count = strIdList.count();
    //2. 显示
    for(int i = 0; i < count;i++){
        QList<QString> strList;
        QString strId = strIdList.at(i);
        bool flag = regXml.getDevInfo(strId,strList);
        if(flag){
            //strList.prepend(strId);
            strList.append(tr("未注册"));
            //strList.replace(0,"-1");
            if(strList.at(5)=="NULL")
                strList.replace(5,tr("暂无"));
            addTabelViewRow(strList);
        }
    }

}
/**
 * @funcname  isInRegisterXml
 * @brief     判断id是否在注册表里
 * @param     strId 需要添加的ID
 * @param     无
 * @return    true 在注册表里
 */
bool DeviceMangerWindow::isInRegisterXml(QString strId)
{
    QList<QString> strIdList;
    DomXmlAnalysisForRegister regXml(m_strRegisterPath);
    return regXml.isDomXmlstrIdexist(strId);
}
/**
 * @funcname  deleteUnRegisterXml
 * @brief     根据id删除一条信息
 * @param     strId
 * @return    no
 */
void DeviceMangerWindow::deleteUnRegisterXml(QString strId)
{
    DomXmlAnalysisForUnRegister regXml(m_strUnRegisterPath);
    QList<QString> strIdList;
    regXml.doXml("delete",strId,strIdList);
}
/**
 * @funcname  deleteRegisterXml
 * @brief     删除设备ID为strId的注册信息
 * @param     strId
 * @return    no
 */
void DeviceMangerWindow::deleteRegisterXml(QString strId)
{
    DomXmlAnalysisForRegister regXml(m_strRegisterPath);
    QList<QString> strIdList;
    regXml.doXml("delete",strId,strIdList);
}

void DeviceMangerWindow::removeAllRegisterXml()
{
    DomXmlAnalysisForRegister regXml(m_strRegisterPath);
    regXml.removeAllDoxXml();
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
    m_SendObjThread = new DevDiscoveryThread;
    m_SendObjThread->moveToThread(m_SendThread);
    connect(m_SendThread,&QThread::finished,m_SendThread,&QObject::deleteLater);
    connect(m_SendThread,&QThread::finished,m_SendObjThread,&QObject::deleteLater);
    //绑定设备请求
    //connect(this,&DeviceMangerWindow::stopDevSendThreadSignal,m_SendObjThread,&DevSendThread::stop);
    connect(this,&DeviceMangerWindow::startUdpSendData,m_SendObjThread,&DevDiscoveryThread::startSending);
    connect(m_SendObjThread,&DevDiscoveryThread::startUpdateDevInfo,this,&DeviceMangerWindow::updateDevInfoDisplay);


    m_SendThread->start();

}


void DeviceMangerWindow::stopDevSendThread()
{
    qDebug() << "stop m_SendThread";

    if(m_SendThread)
    {
        if(m_SendObjThread){
            m_SendObjThread->enableThread(false);
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
    m_isUpdate = false;                     /*添加修改的时候不更新tabel*/
    //addTabelViewRow(listText);
    int row= m_myTableView->currentIndex().row();   /*从0开始*/
    QAbstractItemModel *model = m_myTableView->model ();
    QModelIndex index = model->index(row,0);
    if(!index.isValid()){
        QMessageBox::information(NULL,tr("添加"),tr("请选中有限ID!"));
        return;
    }
    addRegisterSlot(index);
}
/*
 * 选中删除
 */
void DeviceMangerWindow::btnDeleteClicked()
{
    //0.是否删除

    //1.获取id值
    int row= m_myTableView->currentIndex().row();   /*从0开始*/
    QString strId;
    strId = getDataFromRowAndCol(row+1,2);
    if(strId.isEmpty())
        return;
    int ret = QMessageBox::question(NULL,tr("删除"),QString(tr("确认从注册表中删除(%1)项!")).arg(strId),\
                                    QMessageBox::Yes,QMessageBox::No);
    //2.从注册表删除
    if(ret == QMessageBox::Yes)
    {
        deleteRegisterXml(strId);   /*从注册表删除strId项目*/
        removeTableRow();           /*移除当前选中的行*/
        qDebug() << "删除!";
    }

}

void DeviceMangerWindow::btnRemoveAllClicked()
{

    //1.清空注册表
    removeAllRegisterXml();

    //2.清空表格
    removeAllRow();
    //m_isUpdate = true;
    QMessageBox::information(NULL,tr("清空"),tr("注册表已清空!"),QMessageBox::Ok);



}

void DeviceMangerWindow::updateDevInfoDisplay()
{
    /*1.先清空 */
    removeAllRow();
    /*2.更新注册表显示*/
    displayRegisterInfo();
    /*3.更新未注册表显示*/
    displayUnRegisterInfo();

    ui->btnAdd->setEnabled(true);
    ui->btnDelete->setEnabled(true);
}
/**
 * @funcname  showToolTip
 * @brief     鼠标移动显示单元格提示
 * @param     param1
 * @param     param2
 * @return    ret
 */
void DeviceMangerWindow::showToolTip(QModelIndex index)
{
    if (!index.isValid()) {
        qDebug() << "Invalid index";
        return;
    }
    int row = index.row();
    int col = index.column();

    if(col == 1){
        //显示全部
        QString strId = "Id: " + m_myTableView->getValueFromRowAndCol(row+1,2) + "\n";
        QString strName ="Name: " + m_myTableView->getValueFromRowAndCol(row+1,2)+ "\n";
        QString strModel = "Model: " + m_myTableView->getValueFromRowAndCol(row+1,4)+ "\n";
        QString strSn = "Sn: " + m_myTableView->getValueFromRowAndCol(row+1,5)+ "\n";
        QString strMac = "Mac: " + m_myTableView->getValueFromRowAndCol(row+1,6)+ "\n";
        QString strIp = "IP: " + m_myTableView->getValueFromRowAndCol(row+1,8)+ "\n";
        QString str = strId + strName + strModel+ strSn + strMac + strIp;
        QToolTip::showText(QCursor::pos(), str);
    }
    else
    {
        QToolTip::showText(QCursor::pos(), index.data().toString());
    }
}

/**
 * @funcname  addRegisterSlot
 * @brief     添加注册信息
 * @param     无
 * @param     无
 * @return    无
 */
void DeviceMangerWindow::addRegisterSlot(const QModelIndex &index)
{
    if(!index.isValid())
        return;
    m_isUpdate = false;
    int row = index.row();                  /*行从0开始*/
    //1.获取表格值
    QString strId,strName,strModel,strSn,strMac,strPri,strDevIP,strSatus;
    strId = getDataFromRowAndCol(row+1,2);  /*第2列ID值*/
    if(isInRegisterXml(strId)){
        QMessageBox::information(NULL,tr("注册"),tr("该设备ID已注册!"));
        return;
    }
    strName = getDataFromRowAndCol(row+1,3);/*Dev Name*/
    strModel = getDataFromRowAndCol(row+1,4);/*Dev Model*/
    strSn = getDataFromRowAndCol(row+1,5);  /*Dev Sn*/
    strMac = getDataFromRowAndCol(row+1,6); /*Dev Mac*/
    strPri = getDataFromRowAndCol(row+1,7); /*Dev Pri*/
    strDevIP = getDataFromRowAndCol(row+1,8); /*Dev Pri*/
    strSatus = tr("在线");
    QList<QString> strList;
    QMap<QString,QString> mapSysSetInfo;
    QList<QString> mapKey;
    strList.append(strId);
    strList.append(strName);
    strList.append(strDevIP);
    strList << "" <<""<<""<<""<<"";
    //mapKey << "Dev ID" << "Dev Name" << "Dev IP" << "FTP IP" << "FTP Capture" << "FTP Recog" << "RTC Port" << "WARN Port";
    //2. 得到syssetInfo
    for(int i = 0;i < g_mapKey.count(); i++) {
        mapSysSetInfo.insert(g_mapKey.at(i),strList.at(i));
    }
    //3. 获取注册信息
    QList<QString> strReisterInfo;
    strReisterInfo << strId << strName << strModel << strSn << strMac << strPri \
                   << strDevIP << strSatus << "" << "" <<"" << "" <<"";
    //3.显示默认值
    DevInfoSys dlg(mapSysSetInfo,strReisterInfo,isRegister);;
    dlg.setFilePath(m_strRegisterPath);                     /*设置注册路径*/
    int ret = dlg.exec();
    if(!m_strListRecv.isEmpty())
        m_strListRecv.clear();
    m_strListRecv = dlg.m_strListRecv;
    if(ret == DevInfoSys::Accepted)
    {

        //注册完成后，从未注册表中删除添加的这项
        deleteUnRegisterXml(strId);

    }
    else
    {
        QMessageBox::information(NULL,tr("注册"),tr("取消注册!"));
    }
    displayRegisterInfo();      //刷新显示
    displayUnRegisterInfo();

}
/**
 * @funcname  editRegisterSlot
 * @brief     修改注册信息
 * @param     无
 * @param     无
 * @return    no
 */
void DeviceMangerWindow::editRegisterSlot(const QModelIndex &index)
{
    if(!index.isValid())
        return;

    int row = index.row();                  /*行从0开始*/
    //1.获取设备ID
    QString strId;
    strId = getDataFromRowAndCol(row+1,2);  /*第3列ID值*/
    //2.读取注册表
    DomXmlAnalysisForRegister regXml(m_strRegisterPath);
    QList<QString> strList;
    regXml.getSysSetInfo(strId,strList);
    //3.保存默认信息
    QMap<QString,QString> mapSysSetInfo;
    for(int i = 0;i < g_mapKey.count(); i++) {
        mapSysSetInfo.insert(g_mapKey.at(i),strList.at(i));
    }

    //qDebug() << mapSysSetInfo;
    //3.显示默认值
    //4.
    DevInfoSys dlg(mapSysSetInfo,isEdit);
    dlg.setWindowTitle(tr("修改设备信息"));
    dlg.setFilePath(m_strRegisterPath);
    int ret = dlg.exec();
    if(ret == DevInfoSys::Accepted)
    {
        //QMessageBox::information(NULL,tr("修改"),tr("修改成功!"));
    }
    else
    {
        QMessageBox::information(NULL,tr("修改"),tr("取消修改!"));
    }
    displayRegisterInfo();      //刷新显示
}




