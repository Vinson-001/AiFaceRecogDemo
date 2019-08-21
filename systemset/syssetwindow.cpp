#include "syssetwindow.h"
#include "ui_syssetwindow.h"

#include "./uilib/iconhelper.h"
#include "./toolsclass/readonlydelegatefortableview.h"
#include "./toolsclass/http/chttpclient.h"



#define iconSize    50
#define iconWidth   45
#define iconHeight  45
#define navBtnWidth 45
#define navIcoWidth 18

#define ColorBg "#34495E"
#define ColorFontText "#FEFEFE"
#define ColorChar 0xf085
#define ColorlistText tr("系统设置")
SysSetWindow::SysSetWindow(QWidget *parent):
    QDialog(parent),
    ui(new Ui::SysSetWindow)
{
    ui->setupUi(this);
    m_pButtonGroup = NULL;
    //m_Vipmodel = NULL;
    m_reply = NULL;
    m_file = NULL;
    m_strfilePath = "";
    m_listText << "" << "" << "" <<"";
    this->initForm();
    this->initNav();
    //createRecvDataToXml(m_strfilePath);
}


SysSetWindow::~SysSetWindow()
{
    delete ui;
}

void SysSetWindow::initForm()
{



    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    //this->setProperty("canMove", true);

    IconHelper::Instance()->setIcon(ui->btnSysSet, 0xf0c0, navIcoWidth);
    IconHelper::Instance()->setIcon(ui->btnReTurnMenu, 0xf122, navIcoWidth);


    QToolButton *btn = ui->btnSysSet;
    btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btn->setIconSize(QSize(iconWidth, iconHeight));
    QPixmap pix = IconHelper::Instance()->getPixmap(ColorFontText, ColorChar, iconSize/2, iconWidth, iconHeight);
    btn->setIcon(QIcon(pix));
    btn->setText(ColorlistText);

    //checkbutton
    //if(m_pButtonGroup == NULL)
    // = new QButtonGroup(this);
    //m_pButtonGroup->setExclusive(false);                /*设置不互斥*/
    //m_pButtonGroup->addButton(ui->rtspVideoEnCheckBox);
    //m_pButtonGroup->addButton(ui->rtspOSDEnCheckBox);
    //m_pButtonGroup->addButton(ui->faceDetectionEnCheckBox);
    //m_pButtonGroup->addButton(ui->faceREcogEnCheckBox);
    //m_pButtonGroup->addButton(ui->humanDetectionEnCheckBox);
    //m_pButtonGroup->addButton(ui->humanRecogEnCheckBox);
   // m_pButtonGroup->addButton(ui->picSendEnCheckBox);

    //tableview
#if 0
    m_Vipmodel = new VipModel(7,6);      //设置表头
    m_Vipmodel->setColumnCount(6);
    m_Vipmodel->setHeaderData(0,Qt::Horizontal,tr("功能"));

    m_Vipmodel->setHeaderData(1,Qt::Horizontal,tr("发送命令字"));
    m_Vipmodel->setHeaderData(2,Qt::Horizontal,tr("参数值"));
    m_Vipmodel->setHeaderData(3,Qt::Horizontal,tr("接收命令字"));
    m_Vipmodel->setHeaderData(4,Qt::Horizontal,tr("发送结果"));
    m_Vipmodel->setHeaderData(5,Qt::Horizontal,tr("当前状态"));
    //单元格格式
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignVCenter |Qt::AlignHCenter );    //表头信息居中
    ui->tableView->horizontalHeader()->setObjectName("hHeader");
    ui->tableView->verticalHeader()->setObjectName("vHeader");
    ui->tableView->setAlternatingRowColors(true);                                                 /* 奇偶行颜色区分 */
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch );                  /*设置表格自适应宽度*/
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch); /*自适应行高*/
    QList<QString> listText;
    listText << "RSTP使能" << "2010" << "0" << "0" << tr("0") << tr("关");
    addTabelViewRow(m_Vipmodel,0,listText);
    listText[0] = tr("OSD 使能");
    listText[1] = "2011";
    addTabelViewRow(m_Vipmodel,1,listText);

    listText[0] = tr("人脸检测使能");
    listText[1] = "2020";
    addTabelViewRow(m_Vipmodel,2,listText);

    listText[0] = tr("人脸识别使能");
    listText[1] = "2021";
    addTabelViewRow(m_Vipmodel,3,listText);

    listText[0] = tr("人形检测使能");
    listText[1] = "2030";
    addTabelViewRow(m_Vipmodel,4,listText);

    listText[0] = tr("人形属性使能");
    listText[1] = "2031";
    addTabelViewRow(m_Vipmodel,5,listText);

    listText[0] = tr("图片上传使能");
    listText[1] = "2040";
    addTabelViewRow(m_Vipmodel,6,listText);

    ui->tableView->setModel(m_Vipmodel);
    //为每一列加载委托
    ui->tableView->setItemDelegateForColumn(0, new ReadOnlyDelegateforTableView(this));
    ui->tableView->setItemDelegateForColumn(1, new ReadOnlyDelegateforTableView(this));

#endif
    //信号与槽函数
    connect(ui->rtspVideoEnCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onRstpVideoStateChanged(int)));
    connect(ui->rtspOSDEnCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onRstpOSDStateChanged(int)));
    connect(ui->faceDetectionEnCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onfaceDetectionStateChanged(int)));
    connect(ui->faceREcogEnCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onfaceREcogStateChanged(int)));
    connect(ui->humanDetectionEnCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onhumanDetectionStateChanged(int)));
    connect(ui->humanRecogEnCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onhumanRecogStateChanged(int)));
    connect(ui->picSendEnCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onpicSendStateChanged(int)));
   // connect(m_pButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(onButtonGroupClicked(QAbstractButton*)));

    /*QModelIndex index=m_Vipmodel->index(1,3,QModelIndex());
    QString str1= index.data().toString();
    m_Vipmodel->setData(index,QString(tr("开")));
    qDebug() << str1;*/
}

void SysSetWindow::initNav()
{

    m_strfilePath = QDir::currentPath();
    m_strfilePath += "/HttpRecv.xml";
#if 1
    QStringList qss;
    QFile qssFile(":/qrc/qss/syssetwindow.qss");

    qssFile.open(QFile::ReadOnly);

   if(qssFile.isOpen())

    {

        this->setStyleSheet(tr(qssFile.readAll()));
        qssFile.close();
    }
#endif

}


/*
 * 开启http请求
 */
void SysSetWindow::startHttpRequest(QUrl url)
{
    m_reply = CHttpClient::getInstance()->startRequest(url);
    //createRecvDataToXml(m_strfilePath);
    connect(m_reply, &QNetworkReply::readyRead, this, &SysSetWindow::httpReadyRead);

    connect(m_reply, &QNetworkReply::finished, this, &SysSetWindow::httpFinished);
}

void SysSetWindow::createRecvDataToXml(QString fileName)
{
    QFileInfo info(fileName);             //返回路径
    QString filename(info.fileName());
    //if (filename.isEmpty())

    m_file = new QFile(fileName);           //如果文件为空则创建文件

    int ret = m_file->open(QIODevice::ReadWrite| QIODevice::Text);
    if(!ret)
    {
        qDebug() << m_file->errorString();
        delete m_file;
        m_file = 0;
        return;
    }
    //startRequest(url);
}
/*
 * http 准备读
 */
void SysSetWindow::httpFinished()
{
    if(m_file) {
        m_file->close();
        delete m_file;
        m_file = NULL;
    }
    m_reply->deleteLater();
    m_reply = NULL;
}
/*
 * http 结束
 */
void SysSetWindow::httpReadyRead()
{
    /* 写入xml*/
    if (m_file) {
        m_file->write(m_reply->readAll());
        m_file->close();
    }
    /* 解析xml */
#if 0
   bool status = m_readxml.readXML(m_strfilePath,m_xmlCmd,m_xmlStatus);
   if (status)
   {
       m_listText[2] = tr("发送成功");
       updateDataForTableViewRow(m_Vipmodel,m_row,m_listText);
   }
   else
   {
       m_listText[2] = tr("发送失败");
       updateDataForTableViewRow(m_Vipmodel,m_row,m_listText);
   }
#endif
}
void SysSetWindow::mouseMoveEvent(QMouseEvent *event)
{

    int dx = event->globalX() - lastPos.x();
    int dy = event->globalY() - lastPos.y();
    lastPos = event->globalPos();
    move(x()+dx, y()+dy);

}

void SysSetWindow::mousePressEvent(QMouseEvent *event)
{

    lastPos = event->globalPos();

}

void SysSetWindow::mouseReleaseEvent(QMouseEvent *event)
{

    int dx = event->globalX() - lastPos.x();
    int dy = event->globalY() - lastPos.y();
    move(x()+dx, y()+dy);
}
/*
 * 返回主菜单
 */
void SysSetWindow::on_btnReTurnMenu_clicked()
{
    //accept();
    //close();
    this->hide();               /*隐藏当前对话框 */
    emit showwnd();             /*显示主窗口 */

}

void SysSetWindow::getshowdlg()
{
    this->show();               /*显示对话框*/
}


void SysSetWindow::onRstpVideoStateChanged(int state)
{
    QUrl url;
    m_xmlCmd = "2010";
    m_xmlStatus = "0";
    m_row = 1 - 1;
    if (state == Qt::Checked) // "选中"
     {
        url = QString("http://193.169.3.116/?custom=1&cmd=2010&par=1");

        m_listText[0] = "1";
        m_listText[1] = "2010";
        m_listText[3] = tr("开");
     }
     else
    {
        //发送请求
        m_listText[0] = "0";    /*par*/
        m_listText[1] = "2010"; /*cmd*/
        m_listText[3] = tr("关");

        url = QString("http://193.169.3.116/?custom=1&cmd=2010&par=0");
    }
    startHttpRequest(url);
}

void SysSetWindow::onRstpOSDStateChanged(int state)
{
    QUrl url;
    m_xmlCmd = "2011";
    m_xmlStatus = "0";
    m_row = 2 - 1;
    if (state == Qt::Checked) // "选中"
     {
        //发送请求
        url = QString("http://193.169.3.116/?custom=1&cmd=2011&par=1");
        m_listText[0] = "1";    /*par*/
        m_listText[1] = "2011"; /*cmd*/
        m_listText[3] = tr("开");

     }
     else
    {
        //发送请求
        url = QString("http://193.169.3.116/?custom=1&cmd=2011&par=0");
        m_listText[0] = "0";    /*par*/
        m_listText[1] = "2011"; /*cmd*/
        m_listText[3] = tr("关");
    }
    startHttpRequest(url);
}

void SysSetWindow::onfaceDetectionStateChanged(int state)
{

    QUrl url;
    m_xmlCmd = "2020";
    m_xmlStatus = "0";
    m_row = 3 - 1;
    if (state == Qt::Checked) // "选中"
     {
        //发送请求
        url = QString("http://193.169.3.116/?custom=1&cmd=2020&par=1");
        m_listText[0] = "1";    /*par*/
        m_listText[1] = "2020"; /*cmd*/
        m_listText[3] = tr("开");

     }
     else
    {
        //发送请求
        url = QString("http://193.169.3.116/?custom=1&cmd=2020&par=0");
        m_listText[0] = "0";    /*par*/
        m_listText[1] = "2020"; /*cmd*/
        m_listText[3] = tr("关");
    }
    startHttpRequest(url);
}

void SysSetWindow::onfaceREcogStateChanged(int state)
{
    QUrl url;
    m_xmlCmd = "2021";
    m_xmlStatus = "0";
    m_row = 4 - 1;
    if (state == Qt::Checked) // "选中"
     {
        //发送请求
        url = QString("http://193.169.3.116/?custom=1&cmd=2021&par=1");
        m_listText[0] = "1";    /*par*/
        m_listText[1] = "2021"; /*cmd*/
        m_listText[3] = tr("开");

     }
     else
    {
        //发送请求
        url = QString("http://193.169.3.116/?custom=1&cmd=2021&par=0");
        m_listText[0] = "0";    /*par*/
        m_listText[1] = "2021"; /*cmd*/
        m_listText[3] = tr("关");
    }
    startHttpRequest(url);
}

void SysSetWindow::onhumanDetectionStateChanged(int state)
{
    QUrl url;
    m_xmlCmd = "2030";
    m_xmlStatus = "0";
    m_row = 5 - 1;
    if (state == Qt::Checked) // "选中"
     {
        //发送请求
        url = QString("http://193.169.3.116/?custom=1&cmd=2030&par=1");
        m_listText[0] = "1";    /*par*/
        m_listText[1] = "2030"; /*cmd*/
        m_listText[3] = tr("开");

     }
     else
    {
        //发送请求
        url = QString("http://193.169.3.116/?custom=1&cmd=2030&par=0");
        m_listText[0] = "0";    /*par*/
        m_listText[1] = "2030"; /*cmd*/
        m_listText[3] = tr("关");
    }
    startHttpRequest(url);
}

void SysSetWindow::onhumanRecogStateChanged(int state)
{
    QUrl url;
    m_xmlCmd = "2031";
    m_xmlStatus = "0";
    m_row = 6 - 1;
    if (state == Qt::Checked) // "选中"
     {
        //发送请求
        url = QString("http://193.169.3.116/?custom=1&cmd=2031&par=1");
        m_listText[0] = "1";    /*par*/
        m_listText[1] = "2031"; /*cmd*/
        m_listText[3] = tr("开");

     }
     else
    {
        //发送请求
        url = QString("http://193.169.3.116/?custom=1&cmd=2031&par=0");
        m_listText[0] = "0";    /*par*/
        m_listText[1] = "2031"; /*cmd*/
        m_listText[3] = tr("关");
    }
    startHttpRequest(url);
}

void SysSetWindow::onpicSendStateChanged(int state)
{
    QUrl url;
    m_xmlCmd = "2040";
    m_xmlStatus = "0";
    m_row = 7 - 1;
    if (state == Qt::Checked) // "选中"
     {
        //发送请求
        url = QString("http://193.169.3.116/?custom=1&cmd=2040&par=1");
        m_listText[0] = "1";    /*par*/
        m_listText[1] = "2040"; /*cmd*/
        m_listText[3] = tr("开");

     }
     else
    {
        //发送请求
        url = QString("http://193.169.3.116/?custom=1&cmd=2040&par=0");
        m_listText[0] = "0";    /*par*/
        m_listText[1] = "2040"; /*cmd*/
        m_listText[3] = tr("关");
    }
    startHttpRequest(url);
}


/*
 * 添加一行数据到表头
 */
void SysSetWindow::addTabelViewRow(VipModel *model, int row, QList<QString> listText)
{
    for(int i = 0; i < listText.count(); i++) {
        model->setItem(row, i, new QStandardItem(listText[i]));
        model->item(row,i)->setTextAlignment(Qt::AlignCenter);
    }
}
/*
 * 刷新某一单元格数据
 */
void SysSetWindow::updateDataForTableViewRowandCol(VipModel *model, int row, int col, QString strText)
{
    QModelIndex index=model->index(row,col,QModelIndex());
    //m_Vipmodel->setData(index,strText);
}
/*
 * 刷新某一行显示
 */
void SysSetWindow::updateDataForTableViewRow(VipModel *model, int row, QList<QString> listText)
{
    for(int i = 0 ; i <=3; i++){
        /*3,4,5列*/
        updateDataForTableViewRowandCol(model,row, i+2 ,listText[i]);
    }
}


