#include "realeventwindow.h"
#include "ui_realeventwindow.h"

#include "./uilib/iconhelper.h"

#define iconSize    50
#define iconWidth   50
#define iconHeight  45
#define navBtnWidth 45
#define navIcoWidth 18
#define ColorFontText "#FEFEFE"
RealEventWindow::RealEventWindow(QWidget *parent):
    QDialog(parent),
    ui(new Ui::RealEventWindow),
    m_TcpServerRealEvent(NULL)
{
    ui->setupUi(this);
    this->initForm();
    this->initNav();
    qRegisterMetaType<rtime_event_head_t>("rtime_event_head_t");    /*注册自定义类*/
   // startTcpRealEventThread();
}


RealEventWindow::~RealEventWindow()
{
    stopRtimeEventThread();
    if(m_TcpServerRealEvent)
    {
        delete m_TcpServerRealEvent;
    }
    delete ui;
}

void RealEventWindow::initForm()
{
    QList<QToolButton *> btns;
    QList<QChar> listChar;          /* icon */
    QList<QString> listText;        /* name*/

    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);

    IconHelper::Instance()->setIcon(ui->btnReTurnMenu, 0xf122, navIcoWidth);

    btns << ui->btnRealEvent ;
    listChar << 0xf109;
    listText << tr("实时事件");
    initBtnSytle(btns,listChar,listText);       /*初始化btn样式*/

    initRealEventGroupControl();
}

void RealEventWindow::initNav()
{

#if 1
    QStringList qss;
    QFile qssFile(":/qrc/qss/realeventwindow.qss");

    qssFile.open(QFile::ReadOnly);

   if(qssFile.isOpen())

    {

        this->setStyleSheet(tr(qssFile.readAll()));
        qssFile.close();
    }
#endif

}
/**
 * @funcname  initBtnSytlename
 * @brief     初始化button样式，同时连接信号与槽函数
 * @param     btns 需要设置的信号
 * @param     listChar 需要设置btn 的图标
 * @param     listText 需要设置btn文本
 * @return    no
 */
void RealEventWindow::initBtnSytle(QList<QToolButton *> btns, QList<QChar> listChar, QList<QString> listText)
{
    for(int i = 0; i < btns.count(); i++) {
        QToolButton *btn = btns.at(i);
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        btn->setIconSize(QSize(iconWidth, iconHeight));

        QPixmap pix = IconHelper::Instance()->getPixmap(ColorFontText, listChar.at(i), iconSize/2, iconWidth, iconHeight);

        btn->setIcon(QIcon(pix));
        btn->setText(listText.at(i));
    }

}

void RealEventWindow::initBtnSytle(QList<QToolButton *> btns, QList<QChar> listChar, QList<QString> listText, QList<QString> listTextProperty)
{
    for(int i = 0; i < btns.count(); i++) {
        QToolButton *btn = btns.at(i);
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        btn->setIconSize(QSize(iconWidth, iconHeight));

        QPixmap pix = IconHelper::Instance()->getPixmap(ColorFontText, listChar.at(i), iconSize/2, iconWidth, iconHeight);

        btn->setIcon(QIcon(pix));
        btn->setText(listText.at(i));
        btn->setProperty("btnType",listTextProperty.at(i));           /*设置属性，为了区分qss设置*/
    }

}
/**
 * @funcname  initRealEventGroupControl
 * @brief     初始化实时事件控件
 * @return    ret
 */
void RealEventWindow::initRealEventGroupControl()
{
    QList<QToolButton *> btns;
    QList<QChar> listChar;          /* icon */
    QList<QString> listText;        /* name*/

    btns << ui->startBtn << ui->stopBtn ;
    listChar << 0xf144 << 0xf28d;
    listText << tr("开始") <<tr("停止");
    initBtnSytle(btns,listChar,listText);       /*初始化btn样式*/


    /*绑定信号与槽函数*/
    connect(ui->startBtn, SIGNAL(clicked(bool)), this, SLOT(btnStartClicked()));
    connect(ui->stopBtn, SIGNAL(clicked(bool)), this, SLOT(btnStopClicked()));
}
/**
 * @funcname  startTcpRealEventThread
 * @brief     开始实施时间接收线程
 * @param     no
 * @param     no
 * @return    no
 */
void RealEventWindow::startTcpRealEventThread()
{
    if(m_TcpServerRealEvent == NULL)
        m_TcpServerRealEvent = new MyTcpServerForRealEvent("",8010,this);
#if 0
    if(m_RealEventQThread)
    {
        return;
    }
    m_RealEventQThread= new QThread();
    m_RealEventQObjThread = new TCPClenttThreadForRealEnent;
    m_RealEventQObjThread->initTcp("");
    m_RealEventQObjThread->moveToThread(m_RealEventQThread);
    connect(m_RealEventQThread,&QThread::finished,m_RealEventQThread,&QObject::deleteLater);
    connect(m_RealEventQThread,&QThread::finished,m_RealEventQObjThread,&QObject::deleteLater);
    //绑定设备请求
    //connect(this,&DeviceMangerWindow::stopDevSendThreadSignal,m_SendObjThread,&DevSendThread::stop);
    /*开始接收*/
    connect(this,&RealEventWindow::startRecvDataFromServer,m_RealEventQObjThread,&TCPClenttThreadForRealEnent::onRecvDataFromServer);
    connect(m_RealEventQObjThread,SIGNAL(isRealTimeRecvFinshed(char*)),this,SLOT(onIsRealTimeRecvFinshed(char*)));

    /*创建tcp 连接*/

    /*启动线程*/
    m_RealEventQThread->start();
#endif
}

void RealEventWindow::stopTcpRealEventThread()
{
    qDebug("(%s:%s:%d) stop real event thread ", __FILE__, __FUNCTION__, __LINE__);
    emit stopRtimeEventThread();
    qDebug("(%s:%s:%d) end stop real event thread ", __FILE__, __FUNCTION__, __LINE__);
    delete m_TcpServerRealEvent;
    m_TcpServerRealEvent = NULL;
#if 0
    qDebug("(%s:%s:%d) stop real event thread ", __FILE__, __FUNCTION__, __LINE__);
    /*if(m_RealEventQThread)
    {
        if(m_RealEventQObjThread){
            m_RealEventQObjThread->enableThread(false);
        }
        m_RealEventQThread->quit();
    }
    m_RealEventQThread->wait();*/
    emit stopRtimeEventThread();    //停止
    qDebug("(%s:%s:%d) end stop real event thread ", __FILE__, __FUNCTION__, __LINE__);
#endif

}
/**
 * @funcname  printfRtimeEventInfo
 * @brief     debug RetimeEventInfo
 * @return    ret
 */
void RealEventWindow::printfRtimeEventInfo()
{
    qDebug("###############print Rtime Event Info ################");
    qDebug("m_rtime_event_head.magic: = %s",m_rtime_event_head.magic);
    qDebug("m_rtime_event_head.type: = %d",m_rtime_event_head.type);
    qDebug("m_rtime_event_head.direction: = %d",m_rtime_event_head.direction);
    qDebug("m_rtime_event_head.timestamp: = %lld",m_rtime_event_head.timestamp);
    qDebug("m_rtime_event_head.device_id: = %d",m_rtime_event_head.device_id);
    qDebug("m_rtime_event_head.gid: = %d",m_rtime_event_head.gid);
    qDebug("m_rtime_event_head.id: = %d",m_rtime_event_head.id);
    qDebug("m_rtime_event_head.name: = %s",m_rtime_event_head.name);
    qDebug("m_rtime_event_head.gender: = %d",m_rtime_event_head.gender);
    qDebug("m_rtime_event_head.age: = %d",m_rtime_event_head.age);
    qDebug("m_rtime_event_head.data_type: = %d",m_rtime_event_head.data_type);
    qDebug("m_rtime_event_head.data_len: = %d",m_rtime_event_head.data_len);
    qDebug("###############end print Rtime Event info ############");
}
/**
 * @funcname  showRecogPic
 * @brief     显示识别的图片信息
 * @param     strPic 图片名
 * @return    ret
 */
void RealEventWindow::showRecogPic(QString strPic)
{
   // QPixmap picture = QPixmap(strPic);
   // ui->picLab->setPixmap(picture);
     QImage image;
     if( image.load(strPic) )
     {
         ui->picLabel->setPixmap(QPixmap::fromImage(image));
     }
     //ui->picLabel->resize(icon.width(),icon.height());
}
/**
 * @funcname  showRecogPicInfo
 * @brief     显示识别信息
 *                          事件类型:
 *                          姓名:
 *                          性别:
 *                          年龄：
 *                          gid:
 *                          pid:
 *                          方向：
 * @param     no
 * @param     no
 * @return    no
 */
void RealEventWindow::showRecogPicInfo()
{
    QString strName;
    QString strText;
    quint32 gid = m_rtime_event_head.gid;




    strName = QString(QLatin1String((char *)m_rtime_event_head.name));
    quint8 type = m_rtime_event_head.type;
    if(type == 0){
        strText += QString(tr("事件类型: 人脸识别\n"));
    }
    else if(type == 1){
        strText += QString(tr("事件类型: 刷卡识别\n"));
    }
    strText += QString(tr("姓名: %1\n")).arg(strName);
    quint8 gender;
    gender = m_rtime_event_head.gender;

    if(gid == 0) {
        strText += QString(tr("性别: ?\n"));
        strText += QString(tr("年龄: ?\n"));
        strText += QString(tr("gid: ?\n"));
        strText += QString(tr("pid: ?\n"));
    } else {
        if(gender == 0){
              strText += QString(tr("性别: 男\n"));
        }else if(gender == 1){
            strText += QString(tr("性别: 女\n"));
        }
        else{
            strText += QString(tr("性别: ?\n"));
        }
        strText += QString(tr("年龄: %1\n")).arg(m_rtime_event_head.age);
        strText += QString(tr("gid: group_%1\n")).arg(gid);
        strText += QString(tr("pid: person_Id_%1\n")).arg(m_rtime_event_head.id);

    }
    quint8 direction =  m_rtime_event_head.direction;
    if(direction == 0) {
        strText += QString(tr("方向: 进\n"));
    }
    else{
        strText += QString(tr("方向: 出\n"));
    }
    strText += QString(tr("时间戳: \n"));
    if(gid == 0){
        strText += QString(tr("黑白名单: 陌生人\n"));
    }
    else{
        strText += QString(tr("黑白名单: 白名单\n"));
    }
    ui->RecogInfolabel->setText(strText);
   // ui->RecogInfolabel->show();

}
/**
 * @funcname  isDirExist
 * @brief     判断文件夹是否存在
 * @param     fullPath 全路径,如果不存在则创建
 * @return    true 存在
 */
bool RealEventWindow::isDirExist(QString fullPath)
{
    QDir dir(fullPath);
    if(dir.exists())
    {
      return true;
    }
    else {

        bool ok = dir.mkpath(fullPath);//创建多级目录
        return ok;
    }
}

bool RealEventWindow::isFileExist(QString fullFilePath)
{
    QFileInfo fileInfo(fullFilePath);
    if(fileInfo.exists())
    {
        return true;
    }
    return false;
}


void RealEventWindow::mouseMoveEvent(QMouseEvent *event)
{

    int dx = event->globalX() - lastPos.x();
    int dy = event->globalY() - lastPos.y();
    lastPos = event->globalPos();
    move(x()+dx, y()+dy);

}

void RealEventWindow::mousePressEvent(QMouseEvent *event)
{

    lastPos = event->globalPos();

}

void RealEventWindow::mouseReleaseEvent(QMouseEvent *event)
{

    int dx = event->globalX() - lastPos.x();
    int dy = event->globalY() - lastPos.y();
    move(x()+dx, y()+dy);
}

void RealEventWindow::on_btnReTurnMenu_clicked()
{
    //accept();
    this->hide();               /*隐藏当前对话框 */
    emit showwnd();             /*显示主窗口 */
}

void RealEventWindow::getshowdlg()
{
    this->show();               /*显示窗口 */
}
/**
 * @funcname  onIsRecvFinshed
 * @brief     realtime 接收完成
 * @param     ip 客户端ip
 * @param     rtime_event_head 信息头
 * @param     tcpRecvBlock 图片信息
 * @return    ret
 */
void RealEventWindow::onIsRealTimeRecvFinshed(const QString& ip, const rtime_event_head_t&rtime_event_head, const QByteArray& tcpRecvBlock)
{
    qDebug() << "ip : " << ip;
    static int jcount = 0;
    memset(&m_rtime_event_head, 0, sizeof(rtime_event_head));
    m_rtime_event_head = rtime_event_head;
#ifdef DEBUG
    printfRtimeEventInfo();
#endif

#if 1
            //const char *picData = (tcpRecvBlock.data()); // 获取图片数据
            QString filePath,gidPath,fileName;
            QString strName;
            int gid,pid,age,gender;
            gid = m_rtime_event_head.gid;
            pid = m_rtime_event_head.id;
            age = m_rtime_event_head.age;
            gender = m_rtime_event_head.gender;
            strName = QString(QLatin1String((char *)m_rtime_event_head.name));
            QString current = QDir::currentPath();
            current += "/recog";
            if(gid == 0)
            {
                gidPath = QString("%1/unknow").arg(current);
                fileName = QString("unknow%1.jpeg").arg(jcount);
                jcount++;
            }
            else
            {
                gidPath = QString("%1/group_%2").arg(current).arg(gid); /*文件目录*/
                if(gender == 0)
                    fileName = QString("group%1_pid%2_%3_man_age%4.jpeg").arg(gid).arg(pid).arg(strName).arg(age);
                else{
                    fileName = QString("group%1_pid%2_%3_woman_age%4.jpeg").arg(gid).arg(pid).arg(strName).arg(age);
                }
            }

            isDirExist(gidPath);                                        /*如果不存在目录则创建，则创建*/
            filePath = gidPath + "/" + fileName;


           // filePath = QString("%1/%2_%3_%4.jpeg").arg(current).arg(strName).arg(m_rtime_event_head.id).arg(jcount);
           // filePath = QString("%1/%2_recog.jpeg").arg(current).arg(jcount);

             QFile file(filePath);
             if(!file.open(QIODevice::WriteOnly)){
                 qDebug() << "open file error";
                 //return;
                 file.close();
             }else{
                 //file.write(picData,tcpRecvBlock.size());
                 file.write(tcpRecvBlock);
                 file.close();
             }
             showRecogPic(filePath);
             showRecogPicInfo();

             //jcount++;
#endif

}
/**
 * @funcname  btnStartClicked
 * @brief     开始接收
 * @param     param1
 * @param     param2
 * @return    ret
 */
void RealEventWindow::btnStartClicked()
{

   //emit startRecvDataFromServer();
    startTcpRealEventThread();
}

void RealEventWindow::btnStopClicked()
{
    stopTcpRealEventThread();
}


