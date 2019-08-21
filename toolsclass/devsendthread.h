#ifndef DEVSENDTHREAD_H
#define DEVSENDTHREAD_H
#include <QObject>
#include <QHostAddress>
#include <QMutex>
#include <QUdpSocket>
#include <QTimer>
#include <QFile>

class QFile;
class DevSendThread : public QObject
{
    Q_OBJECT
public:
    explicit DevSendThread(QObject *parent = 0);
    ~DevSendThread();


private:
    bool m_isStop;
    QMutex m_stopMutex;                             //线程暂停 m_stopMutex;
    QUdpSocket *m_udpSocket;
    QHostAddress m_groupAddress;
    QTimer * send_timer;

    QString m_strfileTempPath;
    QString m_strfilePath;
    QFile *m_file;


    bool isfileOpen;

private:
    void initUdp();
    void createRecvDataToXml(QString fileName);
    void closeFile();
    void openFile();

signals:
    //void startUpdateDevInfo(QList<QList<QString>> strlistText);
    void startUpdateDevInfo(bool isUpdate);
public slots:
    void udpSendData();
    void enableSendThread(bool isEnable);
    void udpRecvData();
    void startSending();
};

#endif // DEVSENDTHREAD_H
