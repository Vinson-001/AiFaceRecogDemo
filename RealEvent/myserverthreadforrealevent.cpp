#include "myserverthreadforrealevent.h"

MyServerThreadForRealEvent::MyServerThreadForRealEvent(qintptr socketDescriptor,QObject * parent)
    : QThread(parent),
      m_socketDescriptor(socketDescriptor),
      m_TcpClient(NULL)
{

}

MyServerThreadForRealEvent::~MyServerThreadForRealEvent()
{
    m_TcpClient->close();
    if(m_TcpClient){
        delete m_TcpClient;
        m_TcpClient = NULL;
    }

}

void MyServerThreadForRealEvent::run()
{
    m_TcpClient=new MyRealEventSocket(m_socketDescriptor);
    if (!m_TcpClient->setSocketDescriptor(m_socketDescriptor)) {
        emit error(m_TcpClient->error());
        return;
    }
    qDebug() << "run";
    connect(m_TcpClient, &MyRealEventSocket::disconnected, this, &MyServerThreadForRealEvent::disconnectToHost);

    connect(m_TcpClient, SIGNAL(RealTimeDataReady(const QString&, const rtime_event_head_t&,const QByteArray&)),
            this, SLOT(recvDataSlot(const QString&, const rtime_event_head_t&, const QByteArray&)));

    this->exec();

}

void MyServerThreadForRealEvent::disconnectToHost()
{
    qDebug() << disconnect() ;
    m_TcpClient->disconnectFromHost();
    this->quit();
    qDebug()<<"disconnect client : " << m_socketDescriptor;
}
/**
 * @funcname  recvDataSlot
 * @brief     客户端接收数据槽函数
 * @param     param1
 * @param     param2
 * @return    ret
 */
void MyServerThreadForRealEvent::recvDataSlot(const QString &ip, const rtime_event_head_t&rtime_event_head,const QByteArray &data)
{
    /*发射到uI界面*/
    emit RealTimeDataReady(ip, rtime_event_head, data);
}
