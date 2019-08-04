#ifndef DEVSENDTHREAD_H
#define DEVSENDTHREAD_H
#include <QUdpSocket>
#include <QObject>
#include <QHostAddress>
#include <QMutex>
#include <QUdpSocket>
class DevSendThread : public QObject
{
    Q_OBJECT
public:
    explicit DevSendThread(QObject *parent = 0);
    ~DevSendThread();


private:
    bool m_isStop;
    QMutex m_stopMutex;                             //线程暂停 m_stopMutex;

signals:

public slots:
    void udpSendData(QUdpSocket *udpSocket, QString hostAddr, quint16 port);
    void enableSendThread(bool isEnable);
};

#endif // DEVSENDTHREAD_H
