#include "httpcmd.h"

#include <QDebug>
HttpCmd::HttpCmd(QObject *parent)
{
}
/**
 * @funcname  CombineHttpCmd
 * @brief     重组HTTP命令(http:/193.169.3.111/?custom=1&cmd=1001&par=1)
 * @param     strIp
 * @param     strCmdCode
 * @param     par
 * @return    重组后的命令
 */
QString HttpCmd::CombineHttpCmd(QString strIp, QString strCmdCode, QString par)
{
    QString strCmd;
    strCmd  +=  HTTP_HEAD;      /*http:/*/
    strCmd  +=  strIp;          /*IP */
    strCmd  +=  CUSTOM;
    QString cmdCode;
    QString cmdPar;
    cmdCode = "&cmd=" + strCmdCode;/*CMDCODE*/
    cmdPar = "&par=" + par;
    strCmd+=cmdCode;
    strCmd+=cmdPar;
    //qDebug() << strCmd;
    return strCmd;
}

QString HttpCmd::getDevIdCmd(QString strIp,QString par)
{
    return CombineHttpCmd(strIp,DEV_ID_CMD_CODE,par);
}

QString HttpCmd::getDevNameCmd(QString strIp,QString par)
{
    return CombineHttpCmd(strIp,DEV_NAME_CMD_CODE,par);
}
QString HttpCmd::getDevIpCmd(QString strIp,QString par)
{
    return CombineHttpCmd(strIp,DEV_IP_CMD_CODE,par);
}
QString HttpCmd::getFtpServerIpCmd(QString strIp,QString par)
{
    return CombineHttpCmd(strIp,FTP_IP_CMD_CODE,par);
}
QString HttpCmd::getFtpCapturePathCmd(QString strIp,QString par)
{
    return CombineHttpCmd(strIp,FTP_CAP_PATH_CMD_CODE,par);
}
QString HttpCmd::getFtpFaceRecogPathCmd(QString strIp,QString par)
{
    return CombineHttpCmd(strIp,FTP_RECOG_PATH_CMD_CODE,par);
}
QString HttpCmd::getRtcPortCmd(QString strIp,QString par)
{
    return CombineHttpCmd(strIp,RTC_EVENT_PORT_CMD_CODE,par);
}
QString HttpCmd::getWarnEventPortCmd(QString strIp,QString par)
{
    return CombineHttpCmd(strIp,AlarmEvent_PORT_CMD_CODE,par);
}
/**
 * @funcname  getCmdKeyFromCmdCode
 * @brief     获取命令键值
 * @param     strCmdcode 命令码
 * @param     命令对应键值
 * @return    no
 */
void HttpCmd::getCmdKeyFromCmdCode(int strCmdcode, QString &strCmdKey)
{
    switch(strCmdcode)
    {
        case INT_DEV_ID_CMD_CODE :          strCmdKey = "Dev ID";break;
        case INT_DEV_NAME_CMD_CODE :        strCmdKey = "Dev Name";break;
        case INT_DEV_IP_CMD_CODE :          strCmdKey = "Dev IP";break;
        case INT_FTP_IP_CMD_CODE :          strCmdKey = "FTP IP";break;
        case INT_FTP_CAP_PATH_CMD_CODE:     strCmdKey = "FTP Capture";break;
        case INT_FTP_RECOG_PATH_CMD_CODE:   strCmdKey = "FTP Recog";break;
        case INT_RTC_EVENT_PORT_CMD_CODE:   strCmdKey = "RTC Port";break;
        case INT_WARN_PORT_CMD_CODE:        strCmdKey = "WARN Port";break;
    default:
        break;
    }

}








