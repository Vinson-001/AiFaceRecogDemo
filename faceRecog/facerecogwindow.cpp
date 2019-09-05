#include "facerecogwindow.h"
#include "ui_facerecogwindow.h"

#include "./uilib/iconhelper.h"

#define iconSize    50
#define iconWidth   50
#define iconHeight  45
#define navBtnWidth 45
#define navIcoWidth 18

#define ColorBg "#34495E"
#define ColorFontText "#FEFEFE"
#define ColorChar 0xf085
#define ColorlistText tr("人脸管理")
FaceRecogWindow::FaceRecogWindow(QWidget *parent):
    QDialog(parent),
    ui(new Ui::FaceRecogWindow),
    m_myTableForGroupManger(NULL),
    m_reply(NULL),
    m_strRegisterPath(""),
    m_strPicPath(""),
    m_strAudioPath(""),
    m_devIp("")

{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    ui->setupUi(this);
    clearTcpData();
    m_tcp_face_request = new TcpClentObj(this);
    this->initForm();
    this->initNav();

}


FaceRecogWindow::~FaceRecogWindow()
{
    if(m_myTableForGroupManger){
        delete m_myTableForGroupManger;
        m_myTableForGroupManger = NULL;
    }
    if(m_tcp_face_request) {
        delete m_tcp_face_request;
        m_tcp_face_request = NULL;
    }
    delete ui;
}

void FaceRecogWindow::initForm()
{
    QList<QToolButton *> btns;
    QList<QChar> listChar;          /* icon */
    QList<QString> listText;        /* name*/

    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    this->setProperty("canMove", true);

    //IconHelper::Instance()->setIcon(ui->btnPeople, 0xf0c0, navIcoWidth);
    IconHelper::Instance()->setIcon(ui->btnReTurnMenu, 0xf122, navIcoWidth);

    btns << ui->btnPeople ;
    listChar << 0xf0c0;
    listText << tr("人脸管理");
    initBtnSytle(btns,listChar,listText);       /*初始化btn样式*/

    //connect(btns.at(0), SIGNAL(clicked(bool)), this, SLOT(btnViewPeopleClicked()));
    //connect(btns.at(1), SIGNAL(clicked(bool)), this, SLOT(btnDevFindClicked()),Qt::QueuedConnection);
    //connect(btns.at(2), SIGNAL(clicked(bool)), this, SLOT(btnAddClicked()));
    //connect(btns.at(3), SIGNAL(clicked(bool)), this, SLOT(btnDeleteClicked()));
    //connect(btns.at(4), SIGNAL(clicked(bool)), this, SLOT(btnRemoveAllClicked()));
    //connect(btns.at(5), SIGNAL(clicked(bool)), this, SLOT(btnConfigClicked()));

    QString strTemp = QDir::currentPath();
    m_strRegisterPath = strTemp + "/DevInfoRegister.xml";
    initGroupMangerControl();
    initDataMangerControl();
    initFaceMangerControl();
}

void FaceRecogWindow::initNav()
{

#if 1
    QStringList qss;
    QFile qssFile(":/qrc/qss/facerecogwindow.qss");

    qssFile.open(QFile::ReadOnly);

   if(qssFile.isOpen())

    {

        this->setStyleSheet(tr(qssFile.readAll()));
        qssFile.close();
    }
#endif


}

void FaceRecogWindow::clearTcpData()
{
    memset(&m_request_head,0,sizeof(m_request_head));
    memset(&m_reply_head,0,sizeof(m_reply_head));
    memset(&m_item_head,0,sizeof(m_item_head));
    memset(&m_reply_item_head,0,sizeof(m_reply_item_head));
}

void FaceRecogWindow::mallocBuffer(char *buffer, quint32 size)
{
    if(buffer == NULL)
    {
        buffer = (char *)malloc(size);
    }
}
/**
 * @funcname  initBtnSytlename
 * @brief     初始化button样式，同时连接信号与槽函数
 * @param     btns 需要设置的信号
 * @param     listChar 需要设置btn 的图标
 * @param     listText 需要设置btn文本
 * @return    no
 */
void FaceRecogWindow::initBtnSytle(QList<QToolButton *> btns,QList<QChar> listChar, QList<QString> listText)
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

void FaceRecogWindow::initBtnSytle(QList<QToolButton *> btns, QList<QChar> listChar, QList<QString> listText, QList<QString> listTextProperty)
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
 * @funcname  initGroupMangerControl
 * @brief     初始化分组管理控件
 *                  1.创建group
 *                  2.添加 [创建]/[删除]/[查询] button 水平布局
 *                  3.创建 tableview 垂直布局
 *                  4.调整位置
 * @param     param1
 * @param     param2
 * @return    ret
 */
void FaceRecogWindow::initGroupMangerControl()
{
    //1.创建group
    QGroupBox *groupBox = new QGroupBox(this);
    groupBox->setProperty("groupType","groupManger");     /*设置属性*/
    groupBox->setTitle(tr("分组管理"));                     /*设置标题*/
    groupBox->setGeometry(30,270,300,450);
    //groupBox->setAlignment(Qt::AlignHCenter);               /*调整位置*/

    //2. 新增button [创建]/[删除]/[保存]/[查询]
    QToolButton *createGroupBtn = new QToolButton(this);
    QToolButton *deteleGroupBtn = new QToolButton(this);
    QToolButton *saveGroupBtn = new QToolButton(this);
    QToolButton *serachGroupBtn = new QToolButton(this);
    QList<QToolButton *> btns;
    QList<QChar> listChar;          /* icon */
    QList<QString> listText;        /* name*/
    QList<QString> listBtnProterty; /* btn 属性*/

    btns << createGroupBtn << deteleGroupBtn << saveGroupBtn << serachGroupBtn;
    listChar << 0xf055 << 0xf056 << 0xf0c7 << 0xf002;
    listText << tr("创建") << tr("删除") << tr("保存") << tr("查询");
    listBtnProterty << "createGroupBtn" << "deteleGroupBtn" << "saveGroupBtn" << "serachGroupBtn";
    initBtnSytle(btns,listChar,listText,listBtnProterty);       /*初始化btn样式*/

    QHBoxLayout *hbox = new QHBoxLayout;//横向布局管理器
    hbox->addWidget(createGroupBtn);
    hbox->addWidget(deteleGroupBtn);
    hbox->addWidget(saveGroupBtn);
    hbox->addWidget(serachGroupBtn);
    // spacer项目（或者叫“stretch”），它占满按钮下方的空间，
    //确保这些按钮处在他们所在布局的上方。
    hbox->addStretch(1);

    //3.新增tableview控件
    m_myTableForGroupManger = new  MyTableViewForGroupManger(this);
    QStandardItemModel *groupMangerModel = new QStandardItemModel(m_myTableForGroupManger);
    /*3.1 设置model*/
    m_myTableForGroupManger->setTableModel(groupMangerModel);
    /*3.2 创建表格，并初始化 */
    m_myTableForGroupManger->initMyTableView();
    /*3.3 调整位置*/
    QVBoxLayout *vbox = new QVBoxLayout;//纵向向布局管理器
    vbox->addLayout(hbox);
    vbox->addWidget(m_myTableForGroupManger);

    /*4. 显示控件*/
    groupBox->setLayout(vbox);

    /*5. 绑定信号与槽函数*/
    /*5.1 [创建] 、[删除]、[保存]、[查询] */
    connect(createGroupBtn, SIGNAL(clicked(bool)), this, SLOT(onCreateGroupBtnClicked()));
    connect(deteleGroupBtn, SIGNAL(clicked(bool)), this, SLOT(onDeteleGroupBtnClicked()));
    connect(saveGroupBtn, SIGNAL(clicked(bool)), this, SLOT(onSaveGroupBtnClicked()));
    connect(serachGroupBtn, SIGNAL(clicked(bool)), this, SLOT(onSerachGroupBtnClicked()));
    /*5.2 tableview 组ID改变信号与槽函数 */
    /*5.2 tableview 组ID改变信号与槽函数 */
    connect(m_myTableForGroupManger, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(slotGroupMangerDoubleClicked(const QModelIndex &)));

    connect(m_myTableForGroupManger,SIGNAL(clicked(const QModelIndex &)),this,SLOT(myTableForGroupMangerClicked(const QModelIndex &)));
}
/**
 * @funcname  initDataMangerControl
 * @brief     初始化数据库管理控件
 * @param     param1
 * @param     param2
 * @return    ret
 */
void FaceRecogWindow::initDataMangerControl()
{
    //1.创建group
    QGroupBox *groupBox = new QGroupBox(this);
    groupBox->setProperty("groupType","DataManger");     /*设置属性*/
    groupBox->setTitle(tr("数据库管理"));                     /*设置标题*/
    groupBox->setGeometry(30,100,300,100);

    //2. 新增button [删除]
    QToolButton *deleteDataMangerBtn = new QToolButton(this);
    QList<QToolButton *> btns;
    QList<QChar> listChar;          /* icon */
    QList<QString> listText;        /* name*/
    QList<QString> listBtnProterty; /* btn 属性*/

    btns << deleteDataMangerBtn;
    listChar << 0xf1f8;
    listText << tr("删除数据库");
    listBtnProterty << "deleteDataMangerBtn";
    initBtnSytle(btns,listChar,listText,listBtnProterty);       /*初始化btn样式*/

    QHBoxLayout *hbox = new QHBoxLayout;//横向布局管理器

    hbox->addWidget(deleteDataMangerBtn);
    // spacer项目（或者叫“stretch”），它占满按钮下方的空间，
    //确保这些按钮处在他们所在布局的上方。
    hbox->addStretch(1);


    /*3. 显示控件*/
    groupBox->setLayout(hbox);

    connect(deleteDataMangerBtn, SIGNAL(clicked(bool)), this, SLOT(onDeleteDataMangerBtnClicked()));
}
/**
 * @funcname  initFaceMangerControl
 * @brief     初始化人脸管理控件
 *                  1. 创建group
 *                  2. 添加基本信息group、人脸信息group、其他信息group
 *                  3. 添加编辑框。button
 * @return    no
 */
void FaceRecogWindow::initFaceMangerControl()
{
    ui->topGroupBox->setProperty("groupType","faceManger");     /*设置属性*/

    /*1. 初始化控件*/
    ui->namelineEdit->setPlaceholderText(tr("入库时必填"));
    ui->gidlineEdit->setPlaceholderText(tr("入库时必填"));
    ui->agelineEdit->setPlaceholderText(tr("选填"));
    ui->pidlineEdit->setPlaceholderText(tr("需指定"));
    ui->gendercomboBox->addItem(tr("男"),"man");
    ui->gendercomboBox->addItem(tr("女"),"woman");
    ui->gendercomboBox->addItem(tr("不选"),"no");
    //ui->gidcomboBox->addItem(tr("0"),"0");

    ui->opscomboBox->addItem(tr("入库"),"0");
    ui->opscomboBox->addItem(tr("抽feature"),"1");
    //IconHelper::Instance()->setIcon(ui->audiotoolButton, 0xf093, 20);
    QList<QToolButton*> btns;
    QList<QChar> listChar;
    QList<QString> listText;
    btns << ui->audiotoolButton;
    listChar << 0xf093;
    listText <<"";
    initBtnSytle(btns,listChar,listText);
    QList<QString> strListIp;
    QList<QString> strListId;
    getOnlineDevIDAndDevIP(strListId,strListIp);
    m_mapdevIp.clear();
    for(int i = 0; i < strListId.count(); i++)
    {
        m_mapdevIp.insert(strListId.at(i),strListIp.at(i));
    }

    QMap<QString,QString>::const_iterator imap;

    for (imap = m_mapdevIp.constBegin(); imap != m_mapdevIp.constEnd(); ++imap) {
        ui->devIdcomboBox->addItem(imap.key(),imap.value());
    }

    /*导入图片*/
    connect(ui->importPicBtn, SIGNAL(clicked(bool)), this, SLOT(onImportPicBtnClicked()));
    /*音频支持*/
    connect(ui->audiocheckBox, SIGNAL(stateChanged(int)), this, SLOT(onAudiocheckBoxStateChanged(int)));
    /*导入音频*/
    connect(ui->audiotoolButton, SIGNAL(clicked(bool)), this, SLOT(onAudioBtnClicked()));
    /* 最后确认按钮 */
    connect(ui->sendpushButton,SIGNAL(clicked(bool)),this,SLOT(onOkBtnClicked()));
    connect(ui->saveFeatureBtn,SIGNAL(clicked(bool)),this,SLOT(onSaveFilesBtnClicked()));
    connect(ui->faceQueryBtn,SIGNAL(clicked(bool)),this,SLOT(onFaceQueryBtnClicked()));

    /*检查ops状态*/
    connect(ui->opscomboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(onOpsComboxIndexChanged(QString)));
    /* 检查年龄 */
    connect(ui->gendercomboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(onGenderComboxIndexChanged(QString)));
    /* 检查name*/
    connect(ui->namelineEdit,SIGNAL(textChanged(const QString &)),this, SLOT(onNameLineEditChanged(const QString &)));

    connect(m_tcp_face_request,SIGNAL(recvFinshed(int,char *)),this,SLOT(onRecvFinsed(int,char *)));
    ui->info1label->hide();
    ui->info2label->hide();
}
/**
 * @funcname  startHttpRequest
 * @brief     http 请求
 * @param     url
 * @return    true 成功
 */
bool FaceRecogWindow::startHttpRequest(QUrl url)
{
    QEventLoop eventLoop;
    m_reply = CHttpClient::getInstance()->startRequest(url);
    connect(m_reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();   //block

    if(m_reply->error() == QNetworkReply::NoError){
        qDebug() << "request http NoError";
    } else{
        qDebug() << "request http handle errors here";
        QVariant statusCodeV = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);//http服务器响应码
        qDebug("request http found error ... code :%d %d \n",statusCodeV.toInt(),(int)m_reply->error());//reply->error ,QT 错误码
        qDebug(qPrintable(m_reply->errorString()));
        return false;

    }
    //请求返回的结果
    QByteArray responseByte = m_reply->readAll();

    //保存结果
    QString strFile;
    strFile = QDir::currentPath();
    strFile += "/HttpTemp.xml";
    if(!m_strListRecv.isEmpty())
        m_strListRecv.clear();
    QFileInfo fileInfo(strFile);
    if(!fileInfo.exists()){
        DomXmlAnalysisForHttpTemp httpTemp(strFile);
        httpTemp.createDomXml();
    }
    QFile file(strFile);
    int ret = file.open(QIODevice::WriteOnly| QIODevice::Text);
    if (!ret) {
        qDebug() << file.errorString();
        return false;
    }
    //QMutexLocker locker(&m_lockForXml);
    qDebug() << responseByte;
    file.write(responseByte);
    file.close();
    /*保存值*/
    /*保存命令状态*/
    readDomXmlForReturnCmd();
    return true;
}
/**
 * @funcname  startHttpRequest
 * @brief     http 请求
 * @param     url
 * @param     strType 类型(RETURN_GROUP_INFO | RETURN_HUAMN_INFO) 返回的是信息，不是cmd
 * @return    true 成功
 */
bool FaceRecogWindow::startHttpRequest(QUrl url, QString strType)
{
    QEventLoop eventLoop;
    m_reply = CHttpClient::getInstance()->startRequest(url);
    connect(m_reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();   //block

    if(m_reply->error() == QNetworkReply::NoError){
        qDebug() << "request http NoError";
    } else{
        qDebug() << "request http handle errors here";
        QVariant statusCodeV = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);//http服务器响应码
        qDebug("request http found error ... code :%d %d \n",statusCodeV.toInt(),(int)m_reply->error());//reply->error ,QT 错误码
        qDebug(qPrintable(m_reply->errorString()));
        return false;

    }
    //请求返回的结果
    QByteArray responseByte = m_reply->readAll();

    //保存结果
    QString strFile;
    strFile = QDir::currentPath();
    strFile += "/HttpTemp.xml";
    if(!m_strListRecv.isEmpty())
        m_strListRecv.clear();
    QFileInfo fileInfo(strFile);
    if(!fileInfo.exists()){
        DomXmlAnalysisForHttpTemp httpTemp(strFile);
        httpTemp.createDomXml();
    }
    QFile file(strFile);
    int ret = file.open(QIODevice::WriteOnly| QIODevice::Text);
    if (!ret) {
        qDebug() << file.errorString();
        return false;
    }
    //QMutexLocker locker(&m_lockForXml);
    //qDebug() << responseByte;
    file.write(responseByte);
    file.close();
    /*保存值*/
    /*保存命令状态*/
    if(strType == RETURN_GROUP_INFO || strType == RETURN_HUAMN_INFO)
    {
        readDomXmlForReturnValue();
    }
    else {
            readDomXmlForReturnCmd();
    }
    return true;
}
/**
 * @funcname  readDomXmlForReturnCmd
 * @brief     返回命令状态，结果保存在m_strListRecv中(m_strListRecv[0] 命令字、m_strListRecv[1] 发送状态)
 * @param     no
 * @param     no
 * @return    ret
 */
void FaceRecogWindow::readDomXmlForReturnCmd()
{
    QString strFile;
    strFile = QDir::currentPath();
    strFile += "/HttpTemp.xml";
    if(!m_strListRecv.isEmpty())
        m_strListRecv.clear();
    DomXmlAnalysisForHttpTemp httptemp(strFile);
    httptemp.readDomXmlTempForReturnCmd(m_strListRecv);
}
/**
 * @funcname  readDomXmlForReturnValue
 * @brief     返回接收的数值，结果保存在m_mapElement中
 *                          key: 区分分组
 *                          value:每组的值 value[0] 组ID value[1] members （查询分组）
 * @param     no
 * @param     no
 * @return    ret
 */
void FaceRecogWindow::readDomXmlForReturnValue()
{
    QString strFile;
    strFile = QDir::currentPath();
    strFile += "/HttpTemp.xml";
    if(!m_mapElement.isEmpty())
        m_mapElement.clear();
    DomXmlAnalysisForHttpTemp httptemp(strFile);
    httptemp.readDomXmlTempForReturnValue(m_mapElement);

}
/**
 * @funcname  sendOneCmd
 * @brief     发送单条命令
 * @param     strDevIP 设备IP
 * @param     strCmdCode 命令码
 * @param     par 参数
 * @return    ret
 */
bool FaceRecogWindow::sendOneCmd(QString strDevIP, QString strCmdCode, QString par)
{
    //1. 获取命令码
    HttpCmd cmd(this);
    QString str = cmd.CombineHttpCmd(strDevIP,strCmdCode,par);
    QUrl url = str;
    //2. 发送请求,接收数据阻塞状态
    QString strType = "";
    if(strCmdCode == STR_SERACH_GROUP){
        strType = RETURN_GROUP_INFO;
    }
    else if(strCmdCode == STR_FACE_QUERY){
        strType = RETURN_HUAMN_INFO;
    }
    startHttpRequest(url,strType);

    //4. 判断结果
    if(strType.isEmpty())   /*此时返回的是命令*/
    {
        if(m_strListRecv.isEmpty())
            return false;
        else if(m_strListRecv.at(0) == strCmdCode&&  m_strListRecv.at(1)== RETURN_SUCESS)
            return true;
        else
            return false;
    }else                   /*此时返回的是数值*/
    {
        if(m_mapElement.isEmpty())
            return false;
        else
            return true;
    }
}
/**
 * @funcname  sendCmdForCreateGroup
 * @brief     创建分组，发给单一设备
 * @param     strDevIP 设备IP
 * @param     par 设备参数
 * @return    no
 */
void FaceRecogWindow::sendCmdForCreateGroup(QString strDevIP,QString par)
{
    bool flag = false;
    flag = sendOneCmd(strDevIP,STR_CREATE_GROUP,par);
    if(flag)
    {
        qDebug() << "ok";
    }
    else
    {
        qDebug() << "fail";
    }
}
/**
 * @funcname  sendCmdForCreateGroup
 * @brief     给所有在线设备创建分组
 * @param     groupName 分组名
 * @return    no
 */
void FaceRecogWindow::sendCmdForCreateGroup(QString groupName)
{
    int count;

    /*1. 根据设备ID 获取设备IP值 （仅仅获取在线的设备）*/
    QList<QString> strListDevIP;
    QList<QString> strListOnlineDevID;
    QList<QString> strInfoText;
    QMap<QString,QString> mapList;
    getOnlineDevIDAndDevIP(strListOnlineDevID,strListDevIP);
    if (strListDevIP.isEmpty())
    {
        qDebug() << "[sendCmdForCreateGroup]: strDevID isEmpty";
        return;
    }
    qDebug() << "strListOnlineDevID" <<strListOnlineDevID;
    qDebug() << "strListDevIP" <<strListDevIP;
    count = strListDevIP.count();
    /*2. 依次向所有设备发送创建分组命令命令 */
    for (int i = 0; i < count; i++)
    {
        bool flag = false;
        QString strIp = strListDevIP.at(i);
        QString strDevID = strListOnlineDevID.at(i);
        flag = sendOneCmd(strIp,STR_CREATE_GROUP,groupName); /*是否根据返回码确认状态*/
        if (flag)
        {
            QString strText = QString(tr("设置 groupID = [%1] 成功")).arg(groupName);
            mapList.insert(strDevID,strText);
        }
        else
        {
            QString strText = QString(tr("设置 groupID = [%1] 失败")).arg(groupName);
            mapList.insert(strDevID,strText);
        }

    }
    /*3.获取发送状态*/
    QString strText;
    QMap<QString,QString>::const_iterator i;
    for (i = mapList.constBegin(); i != mapList.constEnd(); ++i) {
        QString strTemp;
        strTemp = i.key() + ": " + i.value() + "\n";
        strText+=strTemp;
    }
    //qDebug() << strText;
    /*4.提示发送状态*/
    QMessageBox::information(NULL,tr("创建分组"),strText);
}

void FaceRecogWindow::sendCmdForDeleteGroup(QString strDevIP, QString par)
{
    bool flag = false;
    flag = sendOneCmd(strDevIP,STR_DELETE_GROUP,par);
    if(flag)
    {
        qDebug() << "ok";
    }
    else
    {
        qDebug() << "fail";
    }
}
/**
 * @funcname  sendCmdForSerachGroup
 * @brief     查询分组
 * @param     strDevIP 设备IP
 * @param     par 查询命令
 * @return    ret
 */
void FaceRecogWindow::sendCmdForSerachGroup(QString strDevIP, QString par)
{
    bool flag = false;
    flag = sendOneCmd(strDevIP,STR_SERACH_GROUP,par);
    if(flag)
    {
        qDebug() << "ok";
    }
    else
    {
        qDebug() << "fail";
    }
    qDebug() << m_mapElement;
}
/**
 * @funcname  getOnlineDevIPAndDevID
 * @brief     获取在线设备信息，输出设备ID和设备信息
 * @param     strListDevID 输出参数
 * @param     strListDevIP 输出参数
 * @return    no
 */
void FaceRecogWindow::getOnlineDevIDAndDevIP(QList<QString> &strListDevID, QList<QString> &strListDevIP)
{
    DomXmlAnalysisForRegister regXml(m_strRegisterPath);
    regXml.getDevIdListFromOnline(strListDevID);                /*获取在线设备ID*/
    regXml.getDevIpListFromIdList(strListDevID,strListDevIP);
}

void FaceRecogWindow::sendCmdForDeleteDataBase(QString strDevIP,QString par)
{
    bool flag = false;
    flag = sendOneCmd(strDevIP,STR_DELETE_DATABASE,par);
    if(flag)
    {
        qDebug() << "ok";
    }
    else
    {
        qDebug() << "fail";
    }
}
/**
 * @funcname  addTabelViewRow
 * @brief     新增一行
 * @param     listText  每列的显示内容（不包含序号）
 * @return    no
 */
void FaceRecogWindow::addTabelViewRow(QList<QString> listText)
{
    m_myTableForGroupManger->doTable("add",listText);

}
/**
 * @funcname  removeTableRow
 * @brief     删除选中的行
 * @return    无
 */
void FaceRecogWindow::removeTableRow()
{
    QList<QString> listText;
    m_myTableForGroupManger->doTable("remove",listText);
}

void FaceRecogWindow::removeAllTable()
{
    QList<QString> listText;
    m_myTableForGroupManger->doTable("removeAll",listText);
}
/**
 * @funcname  updateDisplayTable
 * @brief     更新列表显示
 * @return    no
 */
void FaceRecogWindow::updateDisplayTable(QMap<QString,QList<QString>> map)
{
    QList<QString> listText;
    QMap<QString,QList<QString>>::const_iterator i;

    for (i = map.constBegin(); i != map.constEnd(); ++i) {
        listText = i.value();
        addTabelViewRow(listText);
    }
}
/**
 * @funcname  getDataFromRowAndCol
 * @brief     获取单元格(row,col)的值，从1开始
 * @param     row
 * @param     col
 * @return    string
 */
QString FaceRecogWindow::getDataFromRowAndCol(int row, int col)
{
    QString str;
    str = m_myTableForGroupManger->getValueFromRowAndCol(row,col);
    return str;
}
/**
 * @funcname  getDevIPFromID
 * @brief     获取设备IP
 * @param     strID
 * @param     strDevIp 设备IP
 * @return    -1 失败 ， 0 ok
 */
int FaceRecogWindow::getDevIPFromID(QString strID, QString &strDevIp)
{
    if(m_mapdevIp.isEmpty())
    {
        strDevIp = "";
        return -1;
    }
    else
    {
        strDevIp = m_mapdevIp.value(strID);
        return 0;
    }
}

void FaceRecogWindow::mouseMoveEvent(QMouseEvent *event)
{

    int dx = event->globalX() - lastPos.x();
    int dy = event->globalY() - lastPos.y();
    lastPos = event->globalPos();
    move(x()+dx, y()+dy);

}

void FaceRecogWindow::mousePressEvent(QMouseEvent *event)
{

    lastPos = event->globalPos();

}

void FaceRecogWindow::mouseReleaseEvent(QMouseEvent *event)
{

    int dx = event->globalX() - lastPos.x();
    int dy = event->globalY() - lastPos.y();
    move(x()+dx, y()+dy);
}
/*
 * 返回主菜单
 */
void FaceRecogWindow::on_btnReTurnMenu_clicked()
{
    //accept();
    //close();
    this->hide();               /*隐藏当前对话框 */
    emit showwnd();             /*显示主窗口 */

}

void FaceRecogWindow::getshowdlg()
{
    this->show();               /*显示对话框*/
}

/**
 * @funcname  onCreateGroupBtnClicked
 * @brief     创建分组
 * @return    no
 */
void FaceRecogWindow::onCreateGroupBtnClicked()
{
    /*1. 新增一行table */
    QList<QString> listText;
    listText << "" << ""<< "";
    addTabelViewRow(listText);
}
/**
 * @funcname  onDeteleGroupBtnClicked
 * @brief     删除分组
 * @return    no
 */
void FaceRecogWindow::onDeteleGroupBtnClicked()
{
    QModelIndex index;
    index = m_myTableForGroupManger->currentIndex();
    if(!index.isValid())
    {
        QMessageBox::critical(NULL,tr("删除分组"),tr("请选中有效分组ID"),QMessageBox::Ok);
        return;
    }
    int row = index.row();
    /*2.获取组ID值*/
    QString strId;
    strId = getDataFromRowAndCol(row+1,2);              /*分组ID*/
    if(strId.isEmpty())
    {
        QMessageBox::critical(NULL,tr("删除分组"),tr("分组ID为空"),QMessageBox::Ok);
        return;
    }
    /*3.发送命令*/
    //qDebug() << strId;
    int ret = QMessageBox::critical(NULL,tr("删除分组"),tr("是否确认删除分组ID[%1]").arg(strId),\
                                    QMessageBox::Yes,QMessageBox::No);
    if(ret == QMessageBox::Yes) {
        //QString strDevIp = "193.169.4.23";
        QString strIp,strId;
        strId = ui->devIdcomboBox->currentText();
        int ret = getDevIPFromID(strId,strIp);
        if(ret == -1){
            qDebug("(%s:%s:%d) get dev ip failed...", __FILE__, __FUNCTION__, __LINE__);
            return;
        }
        sendCmdForDeleteGroup(strIp,strId);
        /*4.更新显示*/
        removeTableRow();
    }else{
        QMessageBox::information(NULL,tr("删除分组"),tr("取消删除"),QMessageBox::Ok);
        return;
    }
}
/**
 * @funcname  onSerachGroupBtnClicked
 * @brief     查询分组
 * @return    no
 */
void FaceRecogWindow::onSerachGroupBtnClicked()
{
    /*0.设置查询设备ID*/
    /*1.发送查询命令*/
    //QString strDevIP = "193.169.4.23";
    QString strIp,strId;
    strId = ui->devIdcomboBox->currentText();
    int ret = getDevIPFromID(strId,strIp);
    if(ret == -1){
        qDebug("(%s:%s:%d) get dev ip failed...", __FILE__, __FUNCTION__, __LINE__);
        return;
    }
    sendCmdForSerachGroup(strIp);
    /*2.刷新显示 */
    removeAllTable();
    qDebug() << m_mapElement;
    updateDisplayTable(m_mapElement);
}
/**
 * @funcname  onSaveGroupBtnClicked
 * @brief     编辑完成后，需要点击保存，发送创建分组命令
 * @return    no
 */
void FaceRecogWindow::onSaveGroupBtnClicked()
{
    /*1.获取当前选中index*/
    QModelIndex index;
    index = m_myTableForGroupManger->currentIndex();
    if(!index.isValid())
    {
        QMessageBox::critical(NULL,tr("创建分组"),tr("请选中有效分组ID"),QMessageBox::Ok);
        return;
    }
    int row = index.row();
    /*2.获取组ID值*/
    QString strGroupId;
    strGroupId = getDataFromRowAndCol(row+1,2);              /*分组ID*/
    if(strGroupId.isEmpty())
    {
        QMessageBox::critical(NULL,tr("创建分组"),tr("分组ID为空"),QMessageBox::Ok);
        return;
    }
    /*3.发送命令*/
    //qDebug() << strId;
    //QString strDevIp = "193.169.4.23";
    sendCmdForCreateGroup(strGroupId);


}
/**
 * @funcname  slotGroupMangerDoubleClicked
 * @brief     双击发送创建组ID
 * @return    wu
 */
void FaceRecogWindow::slotGroupMangerDoubleClicked(const QModelIndex &index)
{

    /*if (index.isValid())
    {
        int row =  index.row();
        QString strId;
        strId = getDataFromRowAndCol(row+1,2);
        //qDebug() << strId;
    }*/
}

void FaceRecogWindow::myTableForGroupMangerClicked(const QModelIndex &index)
{

    if (index.isValid())
    {
        int row =  index.row();
        QString strId;
        strId = getDataFromRowAndCol(row+1,2);
        //qDebug() << strId;
        ui->gidlineEdit->setText(strId);    /*更新gid*/
    }
}
/**
 * @funcname  onDeleteDataMangerBtnClicked
 * @brief     删除数据库，此操作不可逆
 * @return    ret
 */
void FaceRecogWindow::onDeleteDataMangerBtnClicked()
{
    int ret = QMessageBox::critical(NULL,tr("数据库管理"),tr("是否确认删除数据库，此操作不可逆"),QMessageBox::Yes,QMessageBox::No);
    if(ret == QMessageBox::Yes){
        QString strIp = "193.169.4.23";
        sendCmdForDeleteDataBase(strIp);
    }
}

void FaceRecogWindow::onImportPicBtnClicked()
{
    QString OpenFile, OpenFilePath;
    QImage image;
    //打开文件夹中的图片文件
    OpenFile = QFileDialog::getOpenFileName(this,"选择图片文件","",
                                                  "Image Files(*.jpg *.jpeg *.png *.bmp *.pgm *.pbm);;All(*.*)");

    //if( OpenFile != "" ){
       // QPixmap icon(OpenFile);
        //ui->picLabel->setPixmap(icon);
        //ui->picLabel->resize(icon.width(),icon.height());
   //}


    if( OpenFile != "" )
    {
        if( image.load(OpenFile) )
        {
            ui->picLabel->setPixmap(QPixmap::fromImage(image));
        }
    }
    /*获取文件信息*/
    QFileInfo OpenFileInfo;
    OpenFileInfo = QFileInfo(OpenFile);
    OpenFilePath = OpenFileInfo.filePath();
    qint64 size = OpenFileInfo.size();
    QString strSize ;
    strSize = QString("%1").arg(size);
    ui->piclenlineEdit->setText(strSize);
    m_strPicPath = OpenFilePath;

    /*保存文件*/
#if 0
    QFile file(OpenFilePath);
    if(!file.open(QIODevice::ReadOnly)) {
        return ;
    }
    QByteArray ba;
    ba = file.readAll();
    m_picdata = ba.data();
    file.close();
#endif
    ui->info1label->hide();
}

void FaceRecogWindow::onAudiocheckBoxStateChanged(int state)
{
    if (state == Qt::Checked) // "选中"
      {
          ui->audiolineEdit->setPlaceholderText(tr("请载入音频文件"));
          m_item_head.audio_support = 1;

      }
      else // 未选中 - Qt::Unchecked
      {
          ui->audiolineEdit->setPlaceholderText(tr("忽略"));
          m_item_head.audio_support = 0;
    }
}

void FaceRecogWindow::onAudioBtnClicked()
{
    QString OpenFile, OpenFilePath;
    QImage image;
    //打开文件夹中的图片文件

    OpenFile = QFileDialog::getOpenFileName(this,"选择音影文件","",
                                                  "音影文件(*.mp3 *.wav );;All(*.*)");
    QFileInfo OpenFileInfo;
    OpenFileInfo = QFileInfo(OpenFile);
    OpenFilePath = OpenFileInfo.filePath();
    m_strAudioPath = OpenFilePath;
    qint64 size = OpenFileInfo.size();
    QString strSize ;
    strSize = QString("%1").arg(size);
    ui->audiolineEdit->setText(strSize);
    /*音频大小*/
    if(m_item_head.audio_support){

        m_item_head.audio_len = size;
    }
    else
    {
        m_item_head.audio_len = 0;
    }
    ui->info2label->hide();
    qDebug() << size;

}

void FaceRecogWindow::onOkBtnClicked()
{
    onSendData();

}
/**
 * @funcname  onFaceQueryBtnClicked
 * @brief     人脸查询
 * @param     param1
 * @param     param2
 * @return    ret
 */
void FaceRecogWindow::onFaceQueryBtnClicked()
{
    /*发送查询命令*/
    sendCmdForFaceQuery();
}
/**
 * @funcname  onSaveFilesBtnClicked
 * @brief     保存文件
 * @param     param1
 * @param     param2
 * @return    ret
 */
void FaceRecogWindow::onSaveFilesBtnClicked()
{

    QString path = QDir::currentPath();
    QString filename = QFileDialog::getSaveFileName(this, tr("Save As"), path, tr("Image Files (*.bmp);; All Files (*.*)"));
    if(!filename.isNull())
    {
        //QFile::copy(path+ filename);
        QFile file(filename);
        if(!file.open(QIODevice::WriteOnly)) {
            return ;
        }
        char *pdata = (char *)m_feature_data;
        file.write(pdata,512);
        qDebug("(%s:%s:%d) m_feature_data: = %p ,pdata[0] = 0x%x ", __FILE__, __FUNCTION__, __LINE__, m_feature_data, m_feature_data[0]);
        file.flush();
        file.close();
    }
    else
    {
        int ret = QMessageBox::information(NULL,tr("Save As"),tr("取消保存!"),QMessageBox::Yes,QMessageBox::No);
    }
}
/**
 * @funcname  onOpsComboxlndexChanged
 * @brief     ops组合框改变事件
 * @param     param1
 * @param     param2
 * @return    ret
 */
void FaceRecogWindow::onOpsComboxIndexChanged(QString text)
{
    if(text == tr(IN_Database))
    {
        m_request_head.ops = 0;
        m_request_head.type = 0;
        m_item_head.feat_len_0 = 0;     /* 入库时不需要填 */
        m_item_head.feat_len_1 = 0;
        m_item_head.feat_len_2 = 0;
    }
    else if(text == tr(OUT_Feature))
    {
        m_request_head.ops = 1;
        m_request_head.type = 1;
        qDebug() << m_request_head.ops;
    }
    else{
        m_request_head.ops = -1;
        m_request_head.type = -1;
    }
}
/**
 * @funcname  onGenderComboxIndexChanged
 * @brief     年龄组合框改变
 * @param     param1
 * @param     param2
 * @return    no
 */
void FaceRecogWindow::onGenderComboxIndexChanged(QString text)
{
    if(text == tr("男")){
        m_item_head.gender = 0;
    }
    else if(text == tr("女")){
        m_item_head.gender = 1;
    }
    else{
        m_item_head.gender = -1;
    }
}

void FaceRecogWindow::onNameLineEditChanged(const QString &text)
{
    m_strDevName = text;
    qDebug() << m_strDevName;
}
/**
 * @funcname  onRecvFinsed
 * @brief     更新接收信息
 * @param     itype
 * @return    no
 */
void FaceRecogWindow::onRecvFinsed(int itype,char *data)
{
    if(itype == INT_INLibrary){
        memset(&m_fr_reply_context,0,sizeof(face_reply_server_context_t));

        reply_item_head_t reply_item_head;
        reply_item_head = m_tcp_face_request->m_fr_reply_context.reply_item_head;

        QString strTemp,strMagic;
        strTemp += "\nrecv data finished...\n";
        strMagic = QString(QLatin1String((char *)reply_item_head.magic));
        strTemp += QString("magic : %1\n").arg(strMagic);
        strTemp += QString("name : %1\n").arg(m_strDevName);
        strTemp += QString("gid : %1\n").arg(reply_item_head.gid);
        strTemp += QString("id : %1\n").arg(reply_item_head.id);
        strTemp += QString("err : %1\n").arg(reply_item_head.err);
        strTemp += QString("errno_0 : %1\n").arg(reply_item_head.errno_0);
        strTemp += QString("errno_1 : %1\n").arg(reply_item_head.errno_1);
        strTemp += QString("errno_2 : %1\n").arg(reply_item_head.errno_2);
        strTemp += QString("data_num : %1\n").arg(reply_item_head.data_num);
        strTemp += QString("data_len_0 : %1\n").arg(reply_item_head.data_len_0);
        strTemp += QString("data_len_1 : %1\n").arg(reply_item_head.data_len_1);
        strTemp += QString("data_len_2 : %1\n").arg(reply_item_head.data_len_2);
        //ui->infotextEdit->setPlainText(strTemp);
        //ui->infotextEdit->insertPlainText(strTemp);
        QString str = ui->infotextEdit->toPlainText();
        str += strTemp;
        ui->infotextEdit->setPlainText(str);

        /*保存特征值*/
        m_feature_data = (char *)data;
        //m_feature_data = m_tcp_face_request->m_feature_data;
        qDebug("(%s:%s:%d) m_feature_data: = %p ,pdata[0] = 0x%x", __FILE__, __FUNCTION__, __LINE__, m_feature_data, m_feature_data[0]);

    }
}

void FaceRecogWindow::onSendData()
{
    checkSetSataus();
    ui->infotextEdit->clear();
    /*1. 准备数据*/
    QString info;
    info = "\nprepare request_head data....\n";

    updateDataHead();   /*数据头*/
    info += "\nprepare request_item data....\n";

    updateItemHead();   /*信息头*/
#if DEBUG
    QString strTemp,strMagic;
    strMagic =QString(QLatin1String((char *)m_request_head.magic)); /*Info */

    strTemp += QString("\nmagic : %1\n").arg(strMagic);
    strTemp += QString("ops : %1\n").arg(m_request_head.ops);
    strTemp += QString("type : %1\n").arg(m_request_head.type);
    strTemp += QString("obj_num : %1\n").arg(m_request_head.obj_num);

    strMagic = QString(QLatin1String((char *)m_item_head.magic));
    strTemp += QString("\nmagic : %1\n").arg(strMagic);
    strTemp += QString("gid : %1\n").arg(m_item_head.gid);
    strTemp += QString("id : %1\n").arg(m_item_head.id);
    //strTemp += QString("m_item_head.gid : %1\n").arg(m_item_head.gid);
    strTemp += QString("gender : %1\n").arg(m_item_head.gender);
    strTemp += QString("data_num : %1\n").arg(m_item_head.data_num);
    strTemp += QString("data_len_0 : %1\n").arg(m_item_head.data_len_0);
    strTemp += QString("feat_len_0 : %1\n").arg(m_item_head.feat_len_0);
    strTemp += QString("audio_support : %1\n").arg(m_item_head.audio_support);
    strTemp += QString("audio_len : %1\n").arg(m_item_head.audio_len);
    info +=strTemp;
#endif
    //printfDataHead();
    //printfItemHead();



    /*2. 创建连接*/
    QString strIp,strId;
    strId = ui->devIdcomboBox->currentText();
    int ret = getDevIPFromID(strId,strIp);
    if(ret == -1){
        qDebug("(%s:%s:%d) get dev ip failed...", __FILE__, __FUNCTION__, __LINE__);
        return;
    }
    info += "\nsend data...\n";
    ui->infotextEdit->setPlainText(info);
    m_tcp_face_request->newConnect(strIp,TCPClientPort);
    sendDataToServer();
   // m_tcp->sendDataToServer(&m_fr_context);

}
/**
 * @funcname  updateDataHead
 * @brief     更新消息头，填充m_request_head结构体
 * @param     param1
 * @param     param2
 * @return    ret
 */
void FaceRecogWindow::updateDataHead()
{
    strncpy((char *)m_request_head.magic, "HEAD", 4); /*magic*/
   // m_request_head.ops                              /*ops type已赋值*/
    m_request_head.obj_num = 1;                       /*默认1*/

    m_fr_context.request_head = &m_request_head;

}

void FaceRecogWindow::updateItemHead()
{
    quint32 id;
    quint32 age;
    quint32 gid;


    strncpy((char *)m_item_head.magic, "INFO", 4);                  /*INFO*/
    gid = QString(ui->gidlineEdit->text()).toUInt();
    m_item_head.gid = gid;                                          /*GID*/
    id = QString(ui->pidlineEdit->text()).toUInt();

    m_item_head.id = id;                                             /*PID*/
    age = QString(ui->agelineEdit->text()).toUInt();                 /*age*/

    unsigned char *pName;
    int len = 0;
    pName = QStringToUChar(ui->namelineEdit->text(),len);

    memcpy( m_item_head.name, pName ,len);                            /*英文1，中文utf-8*/

    m_item_head.age = age;                                             /**/
    m_item_head.data_num = 1;                                           /*固定1*/
    quint32 datalen;
    datalen = QString(ui->piclenlineEdit->text()).toUInt();
    quint64 totaldatalen;
    totaldatalen = datalen;                      /*图片大小 Byte*/
    if(totaldatalen > UINT32_MAX)
    {
        quint32 data2 = totaldatalen - UINT32_MAX;
        m_item_head.data_len_0 = UINT32_MAX;
        m_item_head.data_len_1 = data2;
        m_item_head.data_len_2 = 0;
    }
    else
    {
        m_item_head.data_len_0 = totaldatalen;
        m_item_head.data_len_1 = 0;
        m_item_head.data_len_2 = 0;
    }
    if(m_request_head.type == 1)
    {
        m_item_head.feat_len_0 = 0;                   /*默认512*/
        m_item_head.feat_len_1 = 0;
        m_item_head.feat_len_2 = 0;
    }
    /*音频文件是导入时赋值的*/
    m_fr_context.request_item_head = &m_item_head;
}
char *FaceRecogWindow::prepareFiles(QString strFilePath)
{
    if(strFilePath.isEmpty())
        return NULL;
    QFile file(strFilePath);
    if(!file.open(QIODevice::ReadOnly)) {
        return NULL;
    }
    QByteArray ba;
    ba = file.readAll();
    file.close();
    return ba.data();
}

qint64 FaceRecogWindow::sendDataToServer()
{
    /*发送 request_head */
    int total = 0, rsize = 0;
    qDebug("(%s:%s:%d) send request_head...", __FILE__, __FUNCTION__, __LINE__);

    rsize = m_tcp_face_request->sendData((char *)&m_request_head,sizeof(m_request_head));
    total += rsize;

    qDebug("(%s:%s:%d) send request_item_head...", __FILE__, __FUNCTION__, __LINE__);
    rsize = 0;
    rsize = m_tcp_face_request->sendData((char *)&m_item_head,sizeof(m_item_head));

    total += rsize;
    rsize = 0;

    {

        qDebug("(%s:%s:%d) send image...", __FILE__, __FUNCTION__, __LINE__);
        int data_len = m_item_head.data_len_0;
        rsize = m_tcp_face_request->sendData(prepareFiles(m_strPicPath),data_len);

        total += rsize;
        if(m_item_head.audio_support)
        {
            qDebug("(%s:%s:%d) send audio...", __FILE__, __FUNCTION__, __LINE__);
            rsize = 0;
            rsize = m_tcp_face_request->sendData(prepareFiles(m_strAudioPath),m_item_head.audio_len);
            total += rsize;
        }

    }

    qDebug("(%s:%s:%d) send all data ok...", __FILE__, __FUNCTION__, __LINE__);
    return total;
}

/**
 * @funcname  checkSetSataus
 * @brief     点击确定前检查状态
 * @param     无
 * @param     无
 * @return    no
 */
void FaceRecogWindow::checkSetSataus()
{
    /*1. 检查有没有导入图片 */
    if(ui->piclenlineEdit->text().isEmpty())
    {
        ui->info1label->setText(tr("请导入图片!"));
        ui->info1label->show();
        ui->sendpushButton->setDisabled(true);
    }
    else
    {
        ui->info1label->hide();
        ui->sendpushButton->setEnabled(true);
    }

    /*2. 检查有没有影音文件*/
    if(m_item_head.audio_support && ui->audiolineEdit->text().isEmpty())
    {
        ui->info2label->setText(tr("请导入音频!"));
        ui->info2label->show();
        ui->sendpushButton->setDisabled(true);
    }
    else
    {
        ui->info2label->hide();
        ui->sendpushButton->setEnabled(true);
    }
}

/**
 * @funcname  QStringToUChar
 * @brief     QString 转换为 unsigned char
 * @param     qstr
 * @param     size
 * @return    返回 unsigned char
 */
unsigned char *FaceRecogWindow::QStringToUChar(QString qstr ,qint32 &size)
{
    char*  ch;
    //QByteArray ba = qstr.toLocal8Bit(); // must
    QByteArray ba = qstr.toUtf8();
    ch=ba.data();
    size = ba.size();
    return (unsigned char *)ch;
}

void FaceRecogWindow::printfDataHead()
{
    qDebug("###############print request head info ################");
    qDebug("m_request_head->magic: = %s",m_request_head.magic);
    qDebug("m_request_head->ops: = %d",m_request_head.ops);
    qDebug("m_request_head->type = %d",m_request_head.type);
    qDebug("m_request_head->obj_num: = %d",m_request_head.obj_num);
    qDebug("###############end print request head info ############\n");
}

void FaceRecogWindow::printfItemHead()
{
    qDebug("@@@@@@@@@@@@@@print request item head info @@@@@@@@@@@@@@@");
    qDebug("m_item_head->magic: = %s",m_item_head.magic);
    qDebug("m_item_head->gid: = %d",m_item_head.gid);
    qDebug("m_item_head->id: = %d",m_item_head.id);
    qDebug("m_item_head->name = %s",m_item_head.name);
    qDebug("m_item_head->gender: = %d",m_item_head.gender);
    qDebug("m_item_head->age: = %d",m_item_head.age);
    qDebug("m_item_head->data_num: = %d",m_item_head.data_num);
    qDebug("m_item_head->data_len_0: = %d",m_item_head.data_len_0);
    qDebug("m_item_head->data_len_1: = %d",m_item_head.data_len_1);
    qDebug("m_item_head->data_len_2: = %d",m_item_head.data_len_2);
    qDebug("m_item_head->feat_len_0->id: = %d",m_item_head.feat_len_0);
    qDebug("m_item_head->feat_len_1->id: = %d",m_item_head.feat_len_1);
    qDebug("m_item_head->feat_len_2->id: = %d",m_item_head.feat_len_2);
    qDebug("m_item_head->audio_support: = %d",m_item_head.audio_support);
    qDebug("m_item_head->audio_len: = %d",m_item_head.audio_len);
    qDebug("@@@@@@@@@@@@@@end print request item head info @@@@@@@@@@@@\n");
}
/**
 * @funcname  sendCmdForFaceQuery
 * @brief     人脸查询
 * @param     param1
 * @param     param2
 * @return    ret
 */
void FaceRecogWindow::sendCmdForFaceQuery()
{
    /*1.获取设备设备IP*/
    QString str = ui->devIdcomboBox->currentText();
    QString strIp;
    getDevIPFromID(str,strIp);
    /*2. 获取查询分组ID */
    QString gid = ui->gidlineEdit->text();
    QString index = "0";
    QString number = "1";
    /*3. 组合命令*/
    QString strPar;
    strPar = QString("%1,%2,%3").arg(gid).arg(index).arg(number);
    qDebug()<< strPar;
    sendOneCmd(strIp,STR_FACE_QUERY,strPar);

}



