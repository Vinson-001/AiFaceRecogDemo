#include "mytcpserverforrealevent.h"

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
/**
 * @funcname  incomingConnection
 * @brief     重载虚函数
 * @param     socketDescriptor 设备描述符
 * @param     param2
 * @return    no
 */
void MyTcpServerForRealEvent::incomingConnection(qintptr socketDescriptor)
{
    m_socketList.append(socketDescriptor);

    MyServerThreadForRealEvent *thread = new MyServerThreadForRealEvent(socketDescriptor);  /*新启一个线程*/



    //tcpClient->insert(socketDescriptor,tcpTemp);//插入到连接信息中

    qDebug() <<"incomingConnection THREAD IS：" <<QThread::currentThreadId();

    //发送连接信号

    //emit connectClient(tcpTemp->socketDescriptor(),tcpTemp->peerAddress().toString(),tcpTemp->peerPort());


    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(RealTimeDataReady(const QString&, const rtime_event_head_t&, const QByteArray&)),
            m_dialog, SLOT(onIsRealTimeRecvFinshed(const QString&, const rtime_event_head_t&, const QByteArray&)));

    thread->start();

}
