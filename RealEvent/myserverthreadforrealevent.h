#ifndef MYSERVERTHREADFORREALEVENT_H
#define MYSERVERTHREADFORREALEVENT_H
#include <QThread>
#include <QTcpSocket>
#include "./RealEvent/myrealeventsocket.h"

class MyServerThreadForRealEvent : public QThread
{
    Q_OBJECT
public:
    MyServerThreadForRealEvent(qintptr socketDescriptor,QObject * parent = Q_NULLPTR);
    ~MyServerThreadForRealEvent();

private:
    qintptr m_socketDescriptor;
public:
    MyRealEventSocket *m_TcpClient;

protected:
    void run() Q_DECL_OVERRIDE;

signals:
    void error(QTcpSocket::SocketError socketError);
    void RealTimeDataReady(const QString&, const rtime_event_head_t&, const QByteArray&);

private slots:
    void disconnectToHost();
    void recvDataSlot(const QString&, const rtime_event_head_t&, const QByteArray&);

};

#endif // MYSERVERTHREADFORREALEVENT_H
