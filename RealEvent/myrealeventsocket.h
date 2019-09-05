#ifndef MYREALEVENTSOCKET_H
#define MYREALEVENTSOCKET_H

#include <QObject>
#include <QAbstractSocket>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMutex>

#define MAX_RTIME_EVENT_DATA_LEN      (300 * 1024)
#define MIN_RTIME_EVENT_DATA_LEN      (128)




/**
 * must be 128 Bytes
 **/
typedef struct _rtime_event_head {
        quint8 magic[4];
        quint8 type;
        quint8 direction;
        quint8 reserved0[2];
        quint64 timestamp;
        quint32 device_id;
        quint32 gid;
        quint32 id;
        quint8 name[32];
        quint8 gender;
        quint8 age;
        quint8 reserved1[1];
        quint8 data_type;
        quint32 data_len;
        quint8 reserved2[28];
        quint8 extend[32];
} rtime_event_head_t;


class MyRealEventSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit MyRealEventSocket(qintptr socketDescriptor, QObject *parent = nullptr);
    ~MyRealEventSocket();
private:
    rtime_event_head_t m_rtime_event_head;
    //bool m_isStop;
    QMutex m_Mutex;
    qintptr m_socketDescriptor;
    bool isRecvHeadOk;
    quint32 m_image_size;
    QByteArray m_tcpRecvBlock;

public:

    //void enableThread(bool en=false);
private:
    void printfRtimeEventInfo();

signals:
    void isRealTimeRecvFinshed(char *);                                       /* 更新数据 */
    void RealTimeDataReady(const QString &, const rtime_event_head_t&, const QByteArray &);

public slots:
    void onRecvDataFromServer();                                       /* 从服务器开始接收 */

};

#endif // MYREALEVENTSOCKET_H
