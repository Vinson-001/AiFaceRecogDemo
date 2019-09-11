#ifndef TCPCLENTOBJ_H
#define TCPCLENTOBJ_H
#include <QObject>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QThread>
#include <QDebug>

#define TCPClientPort 8003

#define REQUEST_HEAD_MAGIC             "HEAD"
#define REQUEST_HEAD_SIZE              32    /* 32 Bytes */
#define REQUEST_ITEM_HEAD_MAGIC        "INFO"
#define REQUEST_ITEM_HEAD_SIZE         192   /* 192 Bytes */

#define REPLY_HEAD_MAGIC               REQUEST_HEAD_MAGIC
#define REPLY_HEAD_SIZE                REQUEST_HEAD_SIZE    /* 32 Bytes */
#define REPLY_ITEM_HEAD_MAGIC          REQUEST_ITEM_HEAD_MAGIC
#define REPLY_ITEM_HEAD_SIZE           32   /* 32 Bytes */

/*ops*/
#define TEXT_INLibrary                 "入库"
#define TEXT_OUTFeature                "抽feature"
#define INT_INLibrary                  0
#define INT_OUTFeature                 1
#define STR_INLibrary                  "0"
#define STR_OUTFeature                 "1"

/*type*/
#define TEXT_PIC_TYPE                  "图片"
#define TEXT_PIC_IMG_TYPE              "图片+feature"
#define INT_PIC_TYPE                   INT_INLibrary
#define INT_PIC_IMG_TYPE               INT_OUTFeature
#define STR_PIC_TYPE                   STR_INLibrary
#define STR_PIC_IMG_TYPE               STR_OUTFeature
typedef struct _request_head {
        quint8 magic[4];
        quint8 ops;
        quint8 type;
        quint8 obj_num;
        quint8 reserved_0;
        quint8 reserved_1[24];
} request_head_t, reply_head_t;

/**
 * must be 192 Bytes
 **/
typedef struct _request_item_head {
        quint8 magic[4];
        quint32 gid;
        quint32 id;
        quint8 name[32];
        quint8 gender;
        quint8 age;
        quint8 reserved_0;
        quint8 data_num;
        quint32 data_len_0;
        quint32 data_len_1;
        quint32 data_len_2;
        quint16 feat_len_0;
        quint16 feat_len_1;
        quint16 feat_len_2;
        quint8 reserved_1[2];
        quint8 audio_support;
        quint8 reserved_2[3];
        quint32 audio_len;
        quint8 reserved_3[8];
        quint8 priv[108];
} request_item_head_t;


/**
 * must be 32 Bytes
 **/
typedef struct _reply_item_head {
        quint8 magic[4];
        quint32 gid;
        quint32 id;
        quint8 err;
        quint8 errno_0;
        quint8 errno_1;
        quint8 errno_2;
        quint8 reserved_0;
        quint8 data_num;
        quint16 data_len_0;
        quint16 data_len_1;
        quint16 data_len_2;
        quint8 reserved_1[8];
} reply_item_head_t;


/**
 * face request context
 **/
typedef struct _face_request_server_context {
        int total_send_data_len;
        request_head_t *request_head;
        request_item_head_t *request_item_head;
        char *image_data;
        char *audio_data;
} face_request_server_context_t;

/**
 * face reply context
 **/
typedef struct _face_reply_server_context {
        int total_reply_data_len;
        reply_head_t reply_head;                    /*接收头*/
        reply_item_head_t reply_item_head;          /*接收item*/
        //char *feature_data;                         /*保存文件信息*/
} face_reply_server_context_t;


class TcpClentObj: public QObject
{
    Q_OBJECT
public:
    explicit TcpClentObj(QObject *parent = nullptr);
    ~TcpClentObj();

private:
    QTcpSocket *tcpSocket;

    QString recvData;
    // 用来存放数据的大小信息
    quint16 blockSize;
    QList<QByteArray> m_files_ba;

    bool isRecvHeadOk;
    quint64 m_feature_size;
    quint8 m_reply_objnum;
public:
    face_reply_server_context_t m_fr_reply_context;
    reply_head_t m_reply_head;
    QList<reply_item_head_t> m_reply_item_head_list;
    //char *m_feature_buffer;

public:
    void newConnect(QString strIp,quint16 port);
    void disConnect();
    void mallocBuffer(char **buffer, quint32 size);     /*申请内存*/
    void freeBuffer(char *buffer);
    /*发送*/
    int sendData(const char *buf,qint64 maxsize);   /* 发送单挑数据  */
    int readData(char *buf, qint64 maxsize);        /* 单次读取*/



private:
    /*接收*/

    void readReplyhead(reply_head_t &reply_buff);
    void readReplyitem(reply_item_head_t &reply_buff);
    void readReplyObj(int index,reply_item_head_t &reply_item_head,QByteArray &baData );
    char *readFiles(QByteArray ba, qint64 &maxsize);
    void readFiles(QByteArray &baData,qint64 maxsize);

    void printfReplyHead();
    void printfReplyItemHead();



signals:
    void recvFinshed(int type,const QList<QByteArray> & ba);

public slots:
    void readReplyDataFromServer();
    void displayError(QAbstractSocket::SocketError);
};

#endif // TCPCLENTOBJ_H
