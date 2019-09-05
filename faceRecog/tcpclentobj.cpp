#include "tcpclentobj.h"
#include <QFile>
TcpClentObj::TcpClentObj(QObject *parent):
    QObject(parent),
    m_feature_buffer(NULL)
{
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readReplyDataFromServer()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));
}

TcpClentObj::~TcpClentObj()
{
    disConnect();
    if(tcpSocket){
        delete tcpSocket;
        tcpSocket = NULL;
    }
    freeBuffer(m_feature_buffer);
    qDebug() << "TcpClentObj free";
}
/**
 * @funcname  newConnect
 * @brief     建立一个连接
 * @param     param1
 * @param     param2
 * @return    ret
 */
void TcpClentObj::newConnect(QString strIp,quint16 port)
{
    // 初始化数据大小信息为0
    blockSize = 0;

    // 取消已有的连接
    tcpSocket->abort();
    tcpSocket->connectToHost(strIp,port);
    //等待连接成功
    if(!tcpSocket->waitForConnected(30000))
    {
       qDebug() << "Connection failed!";
       return;
    }
    qDebug() << "Connect successfully!";

}

void TcpClentObj::disConnect()
{
    //断开连接
    tcpSocket->disconnectFromHost();
    qDebug() << "stop Connect";
}

void TcpClentObj::mallocBuffer(char **buffer, quint32 size)
{
    if(*buffer == NULL) {
        *buffer = (char *)malloc(size);
    }
}

void TcpClentObj::freeBuffer(char *buffer)
{
    if(buffer){
        free(buffer);
        buffer = NULL;
    }
}

int TcpClentObj::sendData(const char *buf,qint64 maxsize)
{
    //获取文本框内容并以ASCII码形式发送
    //QByteArray buf;
    int ret = tcpSocket->write(buf,maxsize);
    tcpSocket->flush();
    return ret;

}

int TcpClentObj::readData(char *buf, qint64 maxsize)
{
    if(tcpSocket->bytesAvailable())
    {
        return tcpSocket->read(buf,maxsize);
    }

    return -1;
}
/**
 * @function  sendDataToServer
 * @brief     入库操作
 * @return    ret 返回发送的大小
 */
qint64 TcpClentObj::sendDataToServer(face_request_server_context_t *fr_context)
{
    /*发送 request_head */
    int total = 0, rsize = 0;
    qDebug("(%s:%s:%d) send request_head...", __FILE__, __FUNCTION__, __LINE__);
    char *request_head_buf = (char *)(fr_context->request_head);
    rsize = sendData(request_head_buf,REQUEST_HEAD_SIZE);
    total += rsize;

    qDebug("(%s:%s:%d) send request_item_head...", __FILE__, __FUNCTION__, __LINE__);
    rsize = 0;
    char *request_item_head_buf = (char *)(fr_context->request_item_head);
    rsize = sendData(request_item_head_buf,REQUEST_ITEM_HEAD_SIZE);

    total += rsize;
    rsize = 0;

    {
        request_item_head_t *item_head = fr_context->request_item_head;
        int image_size = item_head->data_len_0 + item_head->data_len_1 + item_head->data_len_2;
        qDebug("(%s:%s:%d) send image...", __FILE__, __FUNCTION__, __LINE__);
        rsize = sendData(fr_context->image_data,image_size);
        total += rsize;
        if(item_head->audio_support)
        {
            qDebug("(%s:%s:%d) send audio...", __FILE__, __FUNCTION__, __LINE__);
            rsize = 0;
            int audio_size = item_head->audio_len;
            rsize = sendData(fr_context->audio_data,audio_size);
            total += rsize;
        }

    }

    qDebug("(%s:%s:%d) send all data ok...", __FILE__, __FUNCTION__, __LINE__);
    return total;

}

/**
 * @funcname  readReplyhead
 * @brief     接收reply_head_t
 * @param     reply_data 输出参数
 * @return    no
 */
void TcpClentObj::readReplyhead(reply_head_t &reply_buff)
{
    readData((char *)&reply_buff,REQUEST_HEAD_SIZE);
}
/**
 * @funcname  readReplyitem
 * @brief     reply_item_head_t
 * @param     reply_buff 输出参数
 * @return    no
 */
void TcpClentObj::readReplyitem(reply_item_head_t &reply_buff)
{
    readData((char *)&reply_buff,REQUEST_HEAD_SIZE);
}
/**
 * @funcname  funcname
 * @brief     gaiyao
 * @param     ba 输入参数
 * @param     maxsize 输出参数
 * @return    ret
 */
char * TcpClentObj::readFiles(QByteArray ba, qint64 &maxsize)
{
    //return readData(buf,maxsize);
    ba = tcpSocket->readAll();
    maxsize = ba.size();
    return ba.data();
}

void TcpClentObj::printfReplyHead()
{
    qDebug("@@@@@@@@@@@@@@@print reply head info @@@@@@@@@@@@@@@@");
    qDebug("m_fr_reply_context->reply_head->magic: = %s",m_fr_reply_context.reply_head.magic);
    qDebug("m_fr_reply_context->reply_head->ops: = %d",m_fr_reply_context.reply_head.ops);
    qDebug("m_fr_reply_context->reply_head->type: = %d",m_fr_reply_context.reply_head.type);
    qDebug("m_fr_reply_context->reply_head->obj_num: = %d",m_fr_reply_context.reply_head.obj_num);
    qDebug("@@@@@@@@@@@@@@@end print reply head info @@@@@@@@@@@@");
}

void TcpClentObj::printfReplyItemHead()
{
    qDebug("###############print reply item head info ################");
    qDebug("m_fr_reply_context->reply_item_head->magic: = %s",m_fr_reply_context.reply_item_head.magic);
    qDebug("m_fr_reply_context->reply_item_head->gid: = %d",m_fr_reply_context.reply_item_head.gid);
    qDebug("m_fr_reply_context->reply_item_head->id: = %d",m_fr_reply_context.reply_item_head.id);
    qDebug("m_fr_reply_context->reply_item_head->err: = %d",m_fr_reply_context.reply_item_head.err);
    qDebug("m_fr_reply_context->reply_item_head->errno_0: = %d",m_fr_reply_context.reply_item_head.errno_0);
    qDebug("m_fr_reply_context->reply_item_head->errno_1: = %d",m_fr_reply_context.reply_item_head.errno_1);
    qDebug("m_fr_reply_context->reply_item_head->errno_2: = %d",m_fr_reply_context.reply_item_head.errno_2);
    qDebug("m_fr_reply_context->reply_item_head->data_num: = %d",m_fr_reply_context.reply_item_head.data_num);
    qDebug("m_fr_reply_context->reply_item_head->data_len_0: = %d",m_fr_reply_context.reply_item_head.data_len_0);
    qDebug("m_fr_reply_context->reply_item_head->data_len_1: = %d",m_fr_reply_context.reply_item_head.data_len_1);
    qDebug("m_fr_reply_context->reply_item_head->data_len_2: = %d",m_fr_reply_context.reply_item_head.data_len_2);
    qDebug("###############end print reply item head info ############");
}

void TcpClentObj::readReplyDataFromServer()
{
    //QByteArray buffer;
    //读取缓冲区数据
    /*1. 第一次读reply_head */
    static bool isReadHead = true;
    if(isReadHead)
    {
        reply_head_t reply_head;
        memset(&(m_fr_reply_context.reply_head),0,REQUEST_HEAD_SIZE);
        readReplyhead(reply_head);
        m_fr_reply_context.reply_head = reply_head;
        printfReplyHead();
        isReadHead = false;
    }
    else
    {
        /*2. 第二次读reply_item_head_t*/
#if 1
        reply_item_head_t reply_item_head;
        memset(&(m_fr_reply_context.reply_item_head),0,REQUEST_HEAD_SIZE);
        readReplyitem(reply_item_head);
        m_fr_reply_context.reply_item_head = reply_item_head;
        printfReplyItemHead();
#endif

        /*3. recv feature */
        int feature_num = reply_item_head.data_num;
        if(feature_num > 0){
            qint64 rlen = 0;

            char *feature_data = readFiles(m_files_ba,rlen);


            //qDebug("recv: pdata:%p, pdata[0] = 0x%x",m_fr_reply_context.feature_data,m_fr_reply_context.feature_data[0]);
            #if 0
                QFile file("feat");
                if(!file.open(QIODevice::WriteOnly)) {
                    return ;
                }
                file.write(pdata,512);
                file.flush();
                file.close();
            #endif
            qDebug("(%s:%s:%d) m_feature_data: = %p ,pdata[0] = 0x%x", __FILE__, __FUNCTION__, __LINE__, feature_data, feature_data[0]);
            mallocBuffer(&m_feature_buffer,rlen);

            memset(m_feature_buffer,0,rlen);
            memcpy(m_feature_buffer,feature_data,rlen);
            emit recvFinshed(INT_INLibrary,(char *)m_feature_buffer);

        }
        isReadHead = true;
    }
    qDebug() << isReadHead ;



}

void TcpClentObj::displayError(QAbstractSocket::SocketError)
{
    qDebug() << tcpSocket->errorString();
}
