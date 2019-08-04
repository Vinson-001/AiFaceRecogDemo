#include "devsendthread.h"
#include <QThread>

#define DISCOVERY_SERVER_ADDR "239.255.255.221"
#define DISCOVERY_SERVER_PORT 3188
DevSendThread::DevSendThread(QObject *parent) : QObject(parent)
{
    m_isStop = false;
}

DevSendThread::~DevSendThread()
{
    enableSendThread(false);

}

void DevSendThread::enableSendThread(bool isEnable)
{
    QMutexLocker locker(&m_stopMutex);
    m_isStop = isEnable;


}
/*
 * 发送设备连接
 */
void DevSendThread::udpSendData(QUdpSocket *udpSocket, QString hostAddr, quint16 port)
{
    enableSendThread(true);

#if 1
    while(1) {
        QMutexLocker locker(&m_stopMutex);
        if (!udpSocket ||!m_isStop) {
            qDebug() << "break";
            break;
        }
        QByteArray datagram = "Device Discovery";
        QHostAddress ip_addr = QHostAddress(hostAddr);
        udpSocket->writeDatagram(datagram.data(), datagram.size(),ip_addr,port);
        qDebug() << datagram.data();
        QThread::sleep(3);
    }
#endif
    qDebug() << "stop";
}
