#ifndef MYTCPSERVERFORREALEVENT_H
#define MYTCPSERVERFORREALEVENT_H
#include <QTcpServer>
#include <QObject>
#include <QWidget>

#include "./RealEvent/myrealeventsocket.h"
#include "./RealEvent/realeventwindow.h"
class RealEventWindow;
class MyTcpServerForRealEvent : public QTcpServer
{
    Q_OBJECT
public:
    MyTcpServerForRealEvent(QString strIp,quint16 port, QObject * parent = Q_NULLPTR);
    ~MyTcpServerForRealEvent();


protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;

private:

    QList<MyRealEventSocket *> m_socketList;
    RealEventWindow *m_dialog;

public slots:
    void onStopRtimeEventThread();

};

#endif // MYTCPSERVERFORREALEVENT_H
