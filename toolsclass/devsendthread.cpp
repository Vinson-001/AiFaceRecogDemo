#include "devsendthread.h"
#include <QThread>
#include <QDir>


#define DISCOVERY_SERVER_ADDR "239.255.255.221"
#define DISCOVERY_SERVER_PORT 3188
DevSendThread::DevSendThread(QObject *parent) : QObject(parent)
{
    m_isStop = false;
    m_udpSocket = NULL;
    m_file = NULL;
    m_strfilePath = "";
    m_strfileTempPath = "";
    isfileOpen = false;
    initUdp();
}

DevSendThread::~DevSendThread()
{
    enableSendThread(false);
    delete m_udpSocket;

    m_udpSocket = NULL;

    if(m_file) {
        m_file->close();
        delete m_file;
        m_file = NULL;
    }
}

void DevSendThread::initUdp()
{
    m_groupAddress = QHostAddress(DISCOVERY_SERVER_ADDR);

    m_udpSocket = new QUdpSocket(this);
    send_timer = new QTimer(this);

    if (m_udpSocket->state() != m_udpSocket->BoundState)
    {
        // Rx not in bound state, attempt to bind
        m_udpSocket->bind(QHostAddress::AnyIPv4,DISCOVERY_SERVER_PORT,QUdpSocket::ShareAddress);
    }
    connect(m_udpSocket, SIGNAL(readyRead()),this, SLOT(udpRecvData()),Qt::DirectConnection);
    connect(send_timer, SIGNAL(timeout()), this, SLOT(udpSendData()),Qt::DirectConnection);
    //send_timer->start(1000);

    m_strfileTempPath = QDir::currentPath();
    m_strfilePath = QDir::currentPath();
    m_strfileTempPath += "/DevInfoTemp.xml";
    m_strfilePath += "/DevInfo.xml";
}

void DevSendThread::createRecvDataToXml(QString fileName)
{

    m_file = new QFile(fileName);           //如果文件为空则创建文件
    int ret = m_file->open(QIODevice::WriteOnly| QIODevice::Text);
    if(!ret)
    {
        qDebug() << m_file->errorString() << "DevInfoTemp.xml failed";
        delete m_file;
        m_file = 0;
        isfileOpen = false;
        return;
    }
    isfileOpen = true;
}

void DevSendThread::closeFile()
{
    if(isfileOpen) {
        m_file->close();
    }
    isfileOpen = false;
}

void DevSendThread::openFile()
{
    if(!isfileOpen) {
        int ret = m_file->open(QIODevice::WriteOnly| QIODevice::Text);
        if(!ret)
        {
            qDebug() << m_file->errorString() << "DevInfoTemp.xml failed";
            delete m_file;
            m_file = 0;
            isfileOpen = false;
            return;
        }
        isfileOpen = true;
    }
}

void DevSendThread::enableSendThread(bool isEnable)
{
    QMutexLocker locker(&m_stopMutex);
    m_isStop = isEnable;


}

void DevSendThread::udpRecvData()
{
     enableSendThread(true);
     bool isUpdate = false;
     int count = 0;
     QList<QList<QString>> strListList;
     QList<QString> strListId;

    if (!m_udpSocket || !m_isStop) {
        qDebug() << "break recvdata";
        return;
    }
    if(strListId.isEmpty())
        strListId.cend();
    while (m_udpSocket->hasPendingDatagrams()) {
        QThread::msleep(1000);
        QMutexLocker locker(&m_stopMutex);


        QByteArray datagram;
        QHostAddress sendAddress;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(), datagram.size(),&sendAddress);
        QString recv_ip = sendAddress.toString();
        if(sendAddress.protocol() == QAbstractSocket::IPv4Protocol){
            //qDebug() << "[IP]: "<<recv_ip;
            //qDebug() << datagram.data();
            /*1. 保存xml */
            openFile();
            if (m_file) {
                m_file->resize(0);                  /*清空*/
                m_file->write(datagram);
                //m_file->close();
                closeFile();
            }
            /*2. 解析xml */
            CDomXmlAnalysis domxml(m_strfilePath,m_strfileTempPath);
            /*2.1 解析临时xml */
            domxml.ReadXmlTemp(DevInfoUdpType);
            /*3. 保存id：ip */

            QList<QString> strListDevInfo;
            QString strId = domxml.m_strLisTemp.at(0);
            strListId.append(strId);
            strListDevInfo << strId << domxml.m_strLisTemp.at(1) << domxml.m_strLisTemp.at(2) \
                    << domxml.m_strLisTemp.at(3) << recv_ip << tr("在线");
            /*4. 保存xml*/
            //domxml.UpdateXml(DevInfoUdpType,strId,strListDevInfo);
            //qDebug() << strId << strListDevInfo <<;
            strListList.append(strListDevInfo);
            count++;
        }
    }
    /*4. 保存xml*/
    CDomXmlAnalysis domxml(m_strfilePath,m_strfileTempPath);
    domxml.UpdateXml(DevInfoUdpType,strListId,strListList);
    /*5. 发射信号*/

    isUpdate = true;
    emit startUpdateDevInfo(isUpdate);
    qDebug() << count;
    //qDebug() << strListList;


}

void DevSendThread::startSending()
{
    if(!send_timer->isActive())
        send_timer->start(1000 * 30);

}
/*
 * 发送设备连接
 */
void DevSendThread::udpSendData()
{
        createRecvDataToXml(m_strfileTempPath);                     /*创建xml*/

        QByteArray datagram = "Device Discovery";
        m_udpSocket->writeDatagram(datagram.data(), datagram.size(),m_groupAddress,DISCOVERY_SERVER_PORT);
        qDebug() << datagram.data();


}
