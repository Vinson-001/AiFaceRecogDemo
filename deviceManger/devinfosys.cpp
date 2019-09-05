#include "devinfosys.h"
#include "ui_devinfosys.h"
#include "./toolsclass/xml/domxmlanalysisforudp.h"
#include "./toolsclass/cmd/httpcmd.h"
#include "./deviceManger/deviceMangerwindow.h"
#include <QDebug>
static const QList<QString> g_mapKey= {"DeviceID","DeviceName","DeviceIP","FtpServerIP","FtpServerCapPath",\
                                   "FtpServerRecoPath","HostRealTimeEventPort","HostAlarmEventPort"};
static const QList<QString> g_cmdCode = {DEV_ID_CMD_CODE, DEV_NAME_CMD_CODE, \
                                         DEV_IP_CMD_CODE, FTP_IP_CMD_CODE, FTP_CAP_PATH_CMD_CODE, \
                                         FTP_RECOG_PATH_CMD_CODE, RTC_EVENT_PORT_CMD_CODE, AlarmEvent_PORT_CMD_CODE};

DevInfoSys::DevInfoSys(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DevInfoSys),
    m_reply(NULL)
{
    ui->setupUi(this);
    initDlg();
}

DevInfoSys::DevInfoSys(QMap<QString,QString> map,int type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DevInfoSys),
    m_mapSysInfo(map),
    m_iType(type)
{
    ui->setupUi(this);
    initDlg();
    displayDlg();
}
/**
 * @funcname  DevInfoSys
 * @brief     构造函数
 * @param     QMap ： 设置信息
 * @param     strListRegister ： 注册信息，整张表
 * @return    no
 */
DevInfoSys::DevInfoSys(QMap<QString, QString> map, QList<QString> strListRegister, int type, QWidget *parent):
    QDialog(parent),
    ui(new Ui::DevInfoSys),
    m_mapSysInfo(map),
    m_iType(type),
    m_ListRegisterInfo(strListRegister)
{
    ui->setupUi(this);
    initDlg();
    displayDlg();
}

DevInfoSys::~DevInfoSys()
{
    delete ui;
}
/**
 * @funcname  initDlg
 * @brief     初始化空间
 * @param     无
 * @param     无
 * @return    no
 */
void DevInfoSys::initDlg()
{
    //1. 限制输入
    ui->iDLineEdit->setValidator(new QIntValidator(0, 65535, this));   //但是仍然能输入0~65535
    if(m_iType == isEdit)      /*如果是修改*/
    {
        //ui->iDLineEdit->setFocusPolicy(Qt::NoFocus);
        ui->iDLineEdit->setEnabled(false);
    }

    QRegExp rx1("^(.){1,32}$");
    //rx1.setPattern("^(.){1,32}$"); //限制接受1至32个字符，包含标点符号

    //rx1.setPattern("^[-|0-9|a-z|^\s]{1,16}$"); //限制接受1至16个字符,减号、数字和英文字母
    ui->devNameLineEdit->setValidator(new QRegExpValidator(rx1,this));  //设备名不超过32字符
    rx1.setPattern("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    ui->deviPLineEdit->setValidator(new QRegExpValidator(rx1,this));  //设被IP
    ui->fTPIPLineEdit->setValidator(new QRegExpValidator(rx1,this));  //ftpIP


}
/**
 * @funcname  displayDlg
 * @brief     显示空间信息
 * @param     无
 * @param     无
 * @return    无
 */
void DevInfoSys::displayDlg()
{
    if(m_mapSysInfo.isEmpty())
        return;
    QString strKey;
    strKey = g_mapKey.at(0);                            /*DeviceID*/
    QString strId = m_mapSysInfo.value(strKey);

    strKey = g_mapKey.at(1);                            /*DeviceName*/
    QString strDevName = m_mapSysInfo.value(strKey);

    strKey = g_mapKey.at(2);
    QString strDevIp = m_mapSysInfo.value(strKey);

    strKey = g_mapKey.at(3);
    QString strFtpIp = m_mapSysInfo.value(strKey);

    strKey = g_mapKey.at(4);
    QString strFtpCap = m_mapSysInfo.value(strKey);

    strKey = g_mapKey.at(5);
    QString strFtpRecog = m_mapSysInfo.value(strKey);

    strKey = g_mapKey.at(6);
    QString strRtcPort = m_mapSysInfo.value(strKey);

    strKey = g_mapKey.at(7);
    QString strWarnPort = m_mapSysInfo.value(strKey);

    ui->iDLineEdit->setText(strId);
    ui->devNameLineEdit->setText(strDevName);
    ui->deviPLineEdit->setText(strDevIp);
    if(strFtpCap=="NULL")
        strFtpCap="";
    ui->fTPCaptureLineEdit->setText(strFtpCap);
    if(strFtpIp=="NULL")
        strFtpIp="";
    ui->fTPIPLineEdit->setText(strFtpIp);
    if(strFtpRecog=="NULL")
        strFtpRecog="";
    ui->fTPRecogLineEdit->setText(strFtpRecog);
    if(strRtcPort=="NULL")
        strRtcPort="";
    ui->rtcPortLineEdit->setText(strRtcPort);
    if(strWarnPort=="NULL")
        strWarnPort="";
    ui->warnPortLineEdit->setText(strWarnPort);
}
/**
 * @funcname  setFilePath
 * @brief     设置xml文件路径
 * @param     strFilePath
 * @return    no
 */
void DevInfoSys::setFilePath(QString strFilePath)
{
    m_strFilePath = strFilePath;
}
/**
 * @funcname  getDlgEditVaule
 * @brief     获取编辑框的值
 * @param     strId
 * @param     mapList
 * @return    no
 */
void DevInfoSys::getDlgEditVaule(QString &strId, QMap<QString,QString> &mapList)
{
    QString str;
    QList<QString> strList;
    str = ui->iDLineEdit->text();               /*ID*/
    strList.append(str);

    str = ui->devNameLineEdit->text();          /*devName*/
    strList.append(str);

    str = ui->deviPLineEdit->text();            /*设备IP */
    strList.append(str);

    str = ui->fTPIPLineEdit->text();            /*FTP IP*/
    if(str.isEmpty())
        str="NULL";
    strList.append(str);

    str = ui->fTPCaptureLineEdit->text();       /*FTP Cap Path*/
    if(str.isEmpty())
        str="NULL";
    strList.append(str);

    str = ui->fTPRecogLineEdit->text();         /*FTP Recog Path*/
    if(str.isEmpty())
        str="NULL";
    strList.append(str);

    str = ui->rtcPortLineEdit->text();          /*RTC PORT*/
    if(str.isEmpty())
        str="NULL";
    strList.append(str);
    str = ui->warnPortLineEdit->text();         /*WARN PORT*/
    if(str.isEmpty())
        str="NULL";
    strList.append(str);

    for(int i = 0; i < strList.count();i++){
        mapList.insert(g_mapKey.at(i),strList.at(i));
    }

    strId = ui->iDLineEdit->text();

}
/**
 * @funcname  updateXml
 * @brief     更新信息到注册表
 * @param     strId
 * @param     strListText
 * @return    no
 */
void DevInfoSys::updateXml(QString strId,QList<QString> strListText)
{
    DomXmlAnalysisForRegister regXml(m_strFilePath);
    regXml.setSysInfoToXml(strId,strListText);

}
/**
 * @funcname  addXml
 * @brief     新增一项到注册表
 * @param     strId 新增id
 * @param     strListText新增的内容(全部新增)
 * @return    无
 */
void DevInfoSys::addXml(QString strId, QList<QString> strListText)
{
    DomXmlAnalysisForRegister regXml(m_strFilePath);
    regXml.doXml("add",strId,strListText);
}
/**
 * @funcname  setCmd
 * @brief     发送命令
 * @param     mode 模式
 * @param     strIp 设备IP
 * @param     strListCmdPar 参数值
 * @return    no
 */
void DevInfoSys::setCmd(int mode,QString strIp,QList<QString> strListCmdPar)
{
    if(mode == isEdit) {
        setCmdisEdit(strIp,strListCmdPar);
    }else if(mode == isRegister) {
        setCmdisRegister(strIp,strListCmdPar);
    }

}
/**
 * @funcname  setCmdisEdit
 * @brief     发送修改命令
 * @param     无
 * @param     无
 * @return    no
 */
void DevInfoSys::setCmdisEdit(QString strIP,QList<QString> strListCmdPar)
{
    /*发送命令：按顺序
     * 1.设备ID:1101
     * 2.设备名: 1102
     * 3.设备IP: 1104
     * 4.FTP 服务器IP:1105
     * 5.FTP抓拍人脸图片路径:1106
     * 6.FTP识别人脸路径:1107
     * 7.实时事件端口:1108
     * 8.主机报警端口:1109
     */
    bool ret = false;
    QMap<QString,QString> mapList;
    //int icmd = INT_START_CMD_CODE;
    int count = strListCmdPar.count();
    /*1. 循环发送命令*/
    for(int i = 0; i < count;i++){
        QString strCmd,strPar;

        //strCmd = QString("%1").arg(icmd);
        strCmd = g_cmdCode.at(i);

        strPar = strListCmdPar.at(i);

        /*非空的时候发送*/
        //if(strPar!="NULL")
        {
            ret = sendOneCmd(strIP,strCmd,strPar);
            QString strkey;
            int icmd;
            icmd = strCmd.toInt();
            getCmdKeyFromCmdCode(icmd,strkey);      /*获取命令码的key*/

            if(ret)
            {
                //strList.append("ok");
                QString strText = QString(tr("设置 par = [%1] 成功")).arg(strPar);
                mapList.insert(strkey,strText);
            }
            else{
                QString strText = QString(tr("设置 par = [%1] 失败")).arg(strPar);
                mapList.insert(strkey,strText);
            }
        }
        //icmd++;

    }

    /*2.获取发送状态*/
    QString strText;
    QMap<QString,QString>::const_iterator i;
    for (i = mapList.constBegin(); i != mapList.constEnd(); ++i) {
        QString strTemp;
        strTemp = i.key() + ": " + i.value() + "\n";
        strText+=strTemp;
    }
    //qDebug() << strText;
    /*3.提示发送状态*/
    QMessageBox::information(NULL,tr("修改"),strText);
}
/**
 * @funcname  setCmdisRegister
 * @brief     发送注册命令,只修改ID
 * @param     param1
 * @param     param2
 * @return    ret
 */
void DevInfoSys::setCmdisRegister(QString strIP,QList<QString> strListCmdPar)
{
    /*发送命令：按顺序
     * 1.设备ID:1101
     * 2.设备名: 1102
     * 3.设备IP: 1104
     * 4.FTP 服务器IP:1105
     * 5.FTP抓拍人脸图片路径:1106
     * 6.FTP识别人脸路径:1107
     * 7.实时事件端口:1108
     * 8.主机报警端口:1109
     */
    bool ret = false;
    QMap<QString,QString> mapList;
    int icmd = INT_START_CMD_CODE;
    int count = strListCmdPar.count();
    /*1. 循环发送命令*/
    for(int i = 0; i < count;i ++)
    {
        QString strCmd,strPar;
        if(i==2){
            icmd++;     /*跳过1103*/
        }
        strCmd = QString("%1").arg(icmd);
        strPar = strListCmdPar.at(i);

        /*非空的时候发送*/
       // if(strPar!="NULL")
        {
            ret = sendOneCmd(strIP,strCmd,strPar);
            QString strkey;
            getCmdKeyFromCmdCode(icmd,strkey);      /*获取命令码的key*/

            if(ret)
            {
                QString strText = QString(tr("设置 par = [%1] 成功")).arg(strPar);
                mapList.insert(strkey,strText);
            }
            else{
                QString strText = QString(tr("设置 par = [%1] 失败")).arg(strPar);
                mapList.insert(strkey,strText);
            }
        }
        icmd++;
    }
    /*2.获取发送状态*/
    QString strText;
    QMap<QString,QString>::const_iterator i;
    for (i = mapList.constBegin(); i != mapList.constEnd(); ++i) {
        QString strTemp;
        strTemp = i.key() + ": " + i.value() + "\n";
        strText+=strTemp;
    }
    //qDebug() << strText;
    /*3.提示发送状态*/
    QMessageBox::information(NULL,tr("注册"),strText);
}
/**
 * @funcname  sendOneCmd
 * @brief     发送一条命令
 * @param     strDevIp 设备ip
 * @param     strCmdCode 命令码
 * @param     par        参数
 * @return    true 发送成功
 */
bool DevInfoSys::sendOneCmd(QString strDevIp,QString strCmdCode, QString par)
{
    //1. 获取命令码
    HttpCmd cmd(this);
    QString str = cmd.CombineHttpCmd(strDevIp,strCmdCode,par);
    QUrl url = str;
    //2. 发送请求,接收数据阻塞状态
    startHttpRequest(url);

    //4. 判断结果
    if(m_strListRecv.isEmpty())
        return false;
    else
    {
        if(m_strListRecv.at(0) == strCmdCode&&  m_strListRecv.at(1)== "0")
        {
            return true;
        }else
        {
            return false;
        }
    }
}
/**
 * @funcname  getCmdKeyFromCmdCode
 * @brief     根据命令码获取，命令对应的键值
 * @param     cmd 命令码
 * @param     strKry 键值
 * @return    no
 */
void DevInfoSys::getCmdKeyFromCmdCode(int cmd, QString &strKry)
{
    HttpCmd httpcmd(this);
    httpcmd.getCmdKeyFromCmdCode(cmd,strKry);
}
/**
 * @funcname  startHttpRequest
 * @brief     开启http请求,同时保存数据
 * @param     url
 * @param     无
 * @return    无
 */
bool DevInfoSys::startHttpRequest(QUrl url)
{
    QEventLoop eventLoop;
    m_reply = CHttpClient::getInstance()->startRequest(url);
    //connect(m_reply, &QNetworkReply::readyRead, this, &DevInfoSys::httpReadyRead);
   // connect(m_reply, &QNetworkReply::finished, this, &DevInfoSys::httpFinished);
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
    /*保存值*/
    file.write(responseByte);
    file.close();

    /*解析命令值*/
    DomXmlAnalysisForHttpTemp httpRemp(strFile);
    httpRemp.readDomXmlTempForReturnCmd(m_strListRecv);

    return true;
}


void DevInfoSys::on_btnOk_clicked()
{
    QString strId;
    QMap<QString,QString> mapList;
    getDlgEditVaule(strId,mapList);
    /*1.修改设置值*/
    QList<QString> strListSysInfo;
    for(int i = 0; i < g_mapKey.count(); i++)
    {
        strListSysInfo.append(mapList.value(g_mapKey.at(i)));
        if(strListSysInfo.at(i).isEmpty()){
            strListSysInfo.replace(i,"NULL");
        }
    }
    /*2.更新到xml*/
    if(m_iType==isRegister)
    {
        /*
         * m_ListRegisterInfo:
         * Id
         * name
         * model
         * sn
         * mac
         * pri
         * devip
         * status
         * ftpip
         * ftpcappath
         * ftprecopath
         * hotsrtcport
         * hostalarmport
         */
        m_ListRegisterInfo.removeAt(0);                                 /*去掉ID*/
        m_ListRegisterInfo.replace(0,mapList.value(g_mapKey.at(1)));    /*DeviceName*/
        m_ListRegisterInfo.replace(5,mapList.value(g_mapKey.at(2)));    /*DeviceIp*/
        m_ListRegisterInfo.replace(7,mapList.value(g_mapKey.at(3)));    /*FtpServerIp*/
        m_ListRegisterInfo.replace(8,mapList.value(g_mapKey.at(4)));    /*FtpServerCapPath*/
        m_ListRegisterInfo.replace(9,mapList.value(g_mapKey.at(5)));    /*FtpServerRecoPath*/
        m_ListRegisterInfo.replace(10,mapList.value(g_mapKey.at(6)));   /*HostRealTimeEventPort*/
        m_ListRegisterInfo.replace(11,mapList.value(g_mapKey.at(7)));   /*HostAlarmEventPort*/
        //qDebug() << m_ListRegisterInfo;
        addXml(strId,m_ListRegisterInfo);                               /*新增数据到注册表*/
    }
    else
    {
        updateXml(strId,strListSysInfo);                                 /*修改设置信息*/
    }
    /*获取ID：IP*/
    /*发送命令*/
    QString strIp = mapList.value(g_mapKey.at(2));                      /*获取设备IP*/
    setCmd(m_iType,strIp,strListSysInfo);

    accept();                                                           /*关闭对话框*/
}


/**
 * @funcname  on_isCannecl_clicked
 * @brief     取消操作
 * @return    no
 */
void DevInfoSys::on_isCannecl_clicked()
{
    reject();
}
