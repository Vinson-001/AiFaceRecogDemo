#ifndef MYTCPSERVERFORREALEVENT_H
#define MYTCPSERVERFORREALEVENT_H
#include <QTcpServer>
#include <QObject>
#include <QWidget>

#include "./RealEvent/myserverthreadforrealevent.h"
#include "./RealEvent/myrealeventsocket.h"
#include "./RealEvent/realeventwindow.h"
class RealEventWindow;
class MyTcpServerForRealEvent : public QTcpServer
{
    Q_OBJECT
public:
    MyTcpServerForRealEvent(QString strIp,quint16 port, QObject * parent = Q_NULLPTR);

protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;

private:
    QList<qintptr> m_socketList;
    RealEventWindow *m_dialog;
};

#endif // MYTCPSERVERFORREALEVENT_H
