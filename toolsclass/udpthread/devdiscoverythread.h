/**
* @fileName      DevDiscoveryThread
* @brief         udp组播接收与发送线程
* @author        Wxyang
* @date          2019-08-07
*/

#ifndef DEVDISCOVERYTHREAD_H
#define DEVDISCOVERYTHREAD_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QThread>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTimer>
#include <QMutex>
#include "./toolsclass/xml/domxmlanalysisforudp.h"
#define isRun   true
#define isStop  false
class DevDiscoveryThread : public QObject
{
    Q_OBJECT
public:
    explicit DevDiscoveryThread(QObject *parent = nullptr);
    ~DevDiscoveryThread();

private:
    void initUdp();
    void createTempXmlforRecvData(QString fileName);
    void readDomTempXml(QList<QString> &strListText);
    bool isDomRegisterXml(QString strId);
    void updateRegXml(QString strId,QList<QString> strListText);
    void addUnRegXml(QString strId,QList<QString> strListText);
    void setDevOutlineStatusList(QList<QString> strLitId);
    void setAllDevOutlineStatusList();
    void setDevOnlineStatus(QString strId);
    QList<QString> getOutlineStringId(QList<QString> strListRecvId,QList<QString> strXmlListId);
    QList<QString> getOutlineStringIdInRegisterXml(QList<QString> strListRecvId);
public:

    void enableThread(bool en=false);


private:
    QUdpSocket *m_udpSocket;
    QString m_strfileTempPath;
    QString m_strfileRegPath;
    QString m_strfileUnRegPath;
    bool m_isStop;
    QMutex m_stopMutex;                             //线程暂停 m_stopMutex;
    QMutex m_lockForXml;
    QTimer *m_timer;



signals:
    void startUpdateDevInfo();

public slots:
    void udpSendData();
    void udpRecvData();
    void startSending();
};

#endif // DEVDISCOVERYTHREAD_H
