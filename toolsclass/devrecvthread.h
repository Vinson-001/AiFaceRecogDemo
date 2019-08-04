#ifndef DEVRECVTHREAD_H
#define DEVRECVTHREAD_H

#include <QObject>
#include <QUdpSocket>
#include <QMutex>
#include <QThread>
class DevRecvThread : public QObject
{
    Q_OBJECT
public:
    explicit DevRecvThread(QObject *parent = 0);
    ~DevRecvThread();
private:
    bool m_isStop;
    QMutex m_stopMutex;                             //线程暂停 m_stopMutex;

signals:
    void sendDevInfo();
public slots:
    void udpRecvData(QUdpSocket *udpSocket);
    void enableRecvThread(bool isEnable);
};

#endif // DEVRECVTHREAD_H
