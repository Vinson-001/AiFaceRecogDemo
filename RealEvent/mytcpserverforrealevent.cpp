#include "mytcpserverforrealevent.h"

#include <QList>
MyTcpServerForRealEvent::MyTcpServerForRealEvent(QString strIp,quint16 port, QObject *parent)
    :QTcpServer(parent)
{
    m_dialog = dynamic_cast<RealEventWindow *>(parent);
    bool bsucc;
    /*监听端口*/
    if (strIp.isEmpty())
    {
        bsucc = this->listen(QHostAddress::AnyIPv4, port);
    }
    else
    {
        bsucc = this->listen(QHostAddress(strIp), port);
    }

}

MyTcpServerForRealEvent::~MyTcpServerForRealEvent()
{
    this->close();
    qDebug() << "~MyTcpServerForRealEvent: " <<m_socketList;
    QList<MyRealEventSocket*>::iterator it = m_socketList.begin();
    for (; it != m_socketList.end(); )
    {
        MyRealEventSocket* sock = *it;
        it = m_socketList.erase(it);
        sock->deleteLater();
        sock = NULL;
    }



}
/**
 * @funcname  incomingConnection
 * @brief     重载虚函数
 * @param     socketDescriptor 设备描述符
 * @param     param2
 * @return    no
 */
void MyTcpServerForRealEvent::incomingConnection(qintptr socketDescriptor)
{
#if 0
    m_socketList.append(socketDescriptor);

    MyServerThreadForRealEvent *thread = new MyServerThreadForRealEvent(socketDescriptor);  /*新启一个线程*/



    //tcpClient->insert(socketDescriptor,tcpTemp);//插入到连接信息中

    qDebug() <<"incomingConnection THREAD IS：" <<QThread::currentThreadId();



    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(RealTimeDataReady(const QString&, const rtime_event_head_t&, const QByteArray&)),
            m_dialog, SLOT(onIsRealTimeRecvFinshed(const QString&, const rtime_event_head_t&, const QByteArray&)));
    connect(m_dialog, SIGNAL(stopRtimeEventThread()),thread,SLOT(onStopRtimeEventThread()));

    thread->start();
#endif
    MyRealEventSocket *socket = new MyRealEventSocket(socketDescriptor);
   // m_socketList.append(socket);
    m_socketList.append(socket);

    connect(socket, SIGNAL(readyRead()), socket, SLOT(onRecvDataFromServer())); // 会移进线程里

    connect(socket, SIGNAL(disconnected()), socket, SLOT(disconnectToHost()));

    //MyServerThreadForRealEvent *thread = new MyServerThreadForRealEvent(socket);
    // 以socket为父类，当socket释放删除后也会删除线程，或者将线程的quit信号关联槽deleteLater()也可以达到效果
    QThread *thread = new QThread(socket);
    connect(socket, SIGNAL(disconnected()), thread, SLOT(quit()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    qDebug() <<"incomingConnection THREAD IS：" <<QThread::currentThreadId();
    connect(socket, SIGNAL(RealTimeDataReady(const QString&, const rtime_event_head_t&, const QByteArray&)),
            m_dialog, SLOT(onIsRealTimeRecvFinshed(const QString&, const rtime_event_head_t&, const QByteArray&)));

    connect(m_dialog, SIGNAL(stopRtimeEventThread()),this,SLOT(onStopRtimeEventThread()));
    socket->moveToThread(thread);
    thread->start();


}

void MyTcpServerForRealEvent::onStopRtimeEventThread()
{
    /*根据sockdesc 获取sock */
    qDebug() << m_socketList;
    m_socketList.at(0)->disconnectToHost();

}



