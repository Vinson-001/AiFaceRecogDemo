/**
* @fileName      Httpcmd.h
* @brief         Http cmd 封装
* @author        Wxyang
* @date          2019-08-13
*/
#ifndef HTTPCMD_H
#define HTTPCMD_H
#include <QString>

#include <QObject>
#include "cmd.h"

class HttpCmd: public QObject
{
    Q_OBJECT
public:
    explicit HttpCmd( QObject *parent = nullptr);
private:

public:
    QString CombineHttpCmd(QString strIp,QString strCmdCode,QString par);
    QString getDevIdCmd(QString strIp,QString par);
    QString getDevNameCmd(QString strIp,QString par);
    QString getDevIpCmd(QString strIp,QString par);
    QString getFtpServerIpCmd(QString strIp,QString par);
    QString getFtpCapturePathCmd(QString strIp,QString par);
    QString getFtpFaceRecogPathCmd(QString strIp,QString par);
    QString getRtcPortCmd(QString strIp,QString par);
    QString getWarnEventPortCmd(QString strIp,QString par);
    void getCmdKeyFromCmdCode(int strCmdcode,QString &strCmdKey);
private:

public slots:

};

#endif // HTTPCMD_H
