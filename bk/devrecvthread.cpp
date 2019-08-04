#include "devrecvthread.h"

DevRecvThread::DevRecvThread(QObject *parent) : QObject(parent)
{
    m_isStop = false;

}

DevRecvThread::~DevRecvThread()
{

}

void DevRecvThread::udpRecvData(QUdpSocket *udpSocket)
{
    enableRecvThread(true);
#if 1
    while (1) {
        QMutexLocker locker(&m_stopMutex);
        if (!udpSocket || !m_isStop) {
            qDebug() << "break";
            break;
        }
        if (udpSocket->hasPendingDatagrams()) {

            QByteArray datagram;
            QHostAddress sendAddress;
            datagram.resize(udpSocket->pendingDatagramSize());
            udpSocket->readDatagram(datagram.data(), datagram.size(),&sendAddress);
            QString recv_ip = sendAddress.toString();
            //if(sendAddress.protocol() == QAbstractSocket::IPv4Protocol)
                qDebug() << "[IP]: "<<recv_ip;
                qDebug() << datagram.data();

        //emit startRecvDevInfo(recv_ip,datagram.data());   /*发射信号*/

        }
        QThread::sleep(1);
    }
#endif

}

void DevRecvThread::enableRecvThread(bool isEnable)
{
    QMutexLocker locker(&m_stopMutex);
    m_isStop = isEnable;
}
