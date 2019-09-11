#include "myrealeventsocket.h"
#include <QFile>
#include <QDir>
MyRealEventSocket::MyRealEventSocket(qintptr socketDescriptor, QObject *parent)
    : QTcpSocket(parent),
      m_socketDescriptor(socketDescriptor),
      isRecvHeadOk(false),
      m_image_size(0)

{
    //connect(this, SIGNAL(readyRead()), this, SLOT(onRecvDataFromServer()));
    //connect(this, &MyRealEventSocket::disconnected, this, &MyRealEventSocket::disconnectToHost);
    this->setSocketDescriptor(socketDescriptor);
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(SocketErr(QAbstractSocket::SocketError)));
}

MyRealEventSocket::~MyRealEventSocket()
{
    //disconnectToHost();
    qDebug() << "~MyRealEventSocket";

}





/**
 * @funcname  printfRtimeEventInfo
 * @brief     打印接收信息，调试用
 * @return    no
 */
void MyRealEventSocket::printfRtimeEventInfo()
{
    qDebug("###############print Rtime Event Info ################");
    qDebug("m_rtime_event_head.magic: = %s",m_rtime_event_head.magic);
    qDebug("m_rtime_event_head.type: = %d",m_rtime_event_head.type);
    qDebug("m_rtime_event_head.direction: = %d",m_rtime_event_head.direction);
    qDebug("m_rtime_event_head.timestamp: = %lld",m_rtime_event_head.timestamp);
    qDebug("m_rtime_event_head.device_id: = %d",m_rtime_event_head.device_id);
    qDebug("m_rtime_event_head.gid: = %d",m_rtime_event_head.gid);
    qDebug("m_rtime_event_head.id: = %d",m_rtime_event_head.id);
    qDebug("m_rtime_event_head.name: = %s",m_rtime_event_head.name);
    qDebug("m_rtime_event_head.gender: = %d",m_rtime_event_head.gender);
    qDebug("m_rtime_event_head.age: = %d",m_rtime_event_head.age);
    qDebug("m_rtime_event_head.data_type: = %d",m_rtime_event_head.data_type);
    qDebug("m_rtime_event_head.data_len: = %d",m_rtime_event_head.data_len);
    qDebug("###############end print Rtime Event info ############");
}


/**
 * @funcname  onRecvDataFromServer
 * @brief     开始接收
 * @param     param1
 * @param     param2
 * @return    ret
 */
void MyRealEventSocket::onRecvDataFromServer()
{

    /*1. 开始接收数据*/

    QMutexLocker locker(&m_Mutex);
    static int icount = 0,jcount = 0;
    if(isRecvHeadOk == false){

        int recvLen = this->bytesAvailable();
        if(recvLen < sizeof(rtime_event_head_t)){   /*不能小于头*/
            return;
        }else{      /*读取头信息*/
            isRecvHeadOk = true;
            m_image_size = 0;
            memset((char *)&m_rtime_event_head,0,sizeof(rtime_event_head_t));   /*首先接收头信息*/
            read((char *)&m_rtime_event_head,sizeof(rtime_event_head_t));
#ifdef DEBUG
            printfRtimeEventInfo();
#endif
            m_image_size = m_rtime_event_head.data_len;
            qDebug("(%s:%s:%d) recv head count = %d ,m_image_size = %d", __FILE__, __FUNCTION__,\
                   __LINE__, icount, m_image_size);

            icount++;
        }
    }
    else{               /*读取实际图像数据*/
#ifdef DEBUG
        qDebug() << "recv data :" << jcount << "m_image_size: "<< m_image_size;
#endif
        static int rtotal = 0;
        QByteArray ba;
        ba = (this->read(m_image_size - rtotal));
        rtotal += ba.size();
        m_tcpRecvBlock.append(ba);
        /*读取完成*/
#ifdef DEBUG
        qDebug("(%s:%s:%d) recv tmp data count = %d ,rtotal size = %d", __FILE__, __FUNCTION__,\
               __LINE__, jcount, rtotal);
#endif
        if(rtotal == m_image_size){
            //qDebug() << "recv data :" << jcount << "rtotal: "<< rtotal;
            qDebug("(%s:%s:%d) recv data count = %d ,rtotal size = %d", __FILE__, __FUNCTION__,\
                   __LINE__, jcount, rtotal);
            /*获取图片数据*/
            /*获取IP地址*/
            QString ip = peerAddress().toString();

            ip += QString("|%1").arg(m_socketDescriptor);
            emit RealTimeDataReady(ip, m_rtime_event_head, m_tcpRecvBlock);
#ifdef DEBUG
            char *pdata = (m_tcpRecvBlock.data()); // 获取图片数据
            QString filePath;
            QString strName ;
            strName = QString(QLatin1String((char *)m_rtime_event_head.name));
            QString current = QDir::currentPath();
            current += "/recog";
            filePath = QString("%1/%2_%3_%4.jpeg").arg(current).arg(strName).arg(m_rtime_event_head.id).arg(jcount);

             QFile file(filePath);
             if(!file.open(QIODevice::WriteOnly)){
                 qDebug() << "open file error";
                 //return;
                 file.close();
             }else{
                 file.write(pdata,m_image_size);
                 file.close();
             }
#endif

            /*重新复位*/
            m_tcpRecvBlock.clear();
            m_image_size = 0;
            rtotal = 0;
            isRecvHeadOk = false;
            jcount++;
        }
        else{       /*未读完*/
            return;
        }

    }

}

void MyRealEventSocket::disconnectToHost()
{
    qDebug("(%s:%s:%d) disconnect client : m_socketDescriptor = %d ,rtotal size = %d", __FILE__, __FUNCTION__,\
           __LINE__, m_socketDescriptor);
}

void MyRealEventSocket::SocketErr(QAbstractSocket::SocketError)
{
    MyRealEventSocket *socket = (MyRealEventSocket*)sender();
    qDebug("socket[%d] ip[%s] port[%d] err[%s]", socket->socketDescriptor(),
            socket->peerAddress().toString().toLocal8Bit().data(),socket->peerPort(),socket->errorString().toLocal8Bit().data());

}



/**
 * @funcname  enableThread
 * @brief     停止接收线程
 * @param     en true 使能线程
 * @param     param2
 * @return    ret
 */
#if 0
void MyRealEventSocket::enableThread(bool en)
{
    QMutexLocker locker(&m_Mutex);
    if(en == false)
        m_isStop = true;        /*停止线程*/
    else
        m_isStop = false;

}
#endif
