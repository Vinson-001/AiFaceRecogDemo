#ifndef FACERECOGWINDOW_H
#define FACERECOGWINDOW_H
#include <QDialog>
#include <QNetworkReply>
#include "./toolsclass/tabelview/mytableview.h"
#include "./toolsclass/http/chttpclient.h"
#include "./toolsclass/xml/domxmlanalysisforudp.h"
#include "./toolsclass/cmd/httpcmd.h"
#include "./tcpclentobj.h"
#include "./faceRecog/importmulfaceinfodlg.h"



#define MAX_PIC_SIZE (300 * 1024)
#define GroupMangerTable    "Group Manger"
#define FaceQueryTable      "Face Query"
class QToolButton;
namespace Ui {
class FaceRecogWindow;
}


class FaceRecogWindow : public QDialog
{
    Q_OBJECT
public:
    explicit FaceRecogWindow(QWidget *parent = 0);
    ~FaceRecogWindow();

    QString m_strDevID ;

private:
    Ui::FaceRecogWindow *ui;
    //QList<QToolButton *> btns;
    MyTableViewForGroupManger* m_myTableForGroupManger;
    MyTableViewForFaceQuery * m_myTableForFaceQuery;
    QNetworkReply *m_reply;


    QMap<QString,QList<QString>> m_mapElement;


    /*TCP 用*/
    request_head_t m_request_head;                  /*发送消息头*/
    reply_head_t m_reply_head;                      /*接收消息头*/
    request_item_head_t m_item_head;                /*发送人员信息头*/
    reply_item_head_t m_reply_item_head;            /*接收信息头*/
    QList<reply_item_head_t> m_reply_item_head_list;
   // face_request_server_context_t m_fr_context;     /* 总的结构*/
    //face_reply_server_context_t m_fr_reply_context; /* reply */
    QString m_strPicPath;
    QString m_strAudioPath;
    QString m_devIp;
    QString m_strDevName;                           /* 设备名字 */


public:
    TcpClentObj *m_tcp_face_request;
    QString m_strRegisterPath;
    /*特征值*/
    QList<QByteArray> m_files_baData;

private:
    void initForm();
    void initNav();
    void clearTcpData();                            /*清空TCP 相关结构*/
    void mallocBuffer(char * buffer, quint32 size);                            /*申请内存*/
    void initBtnSytle(QList<QToolButton *> btns, QList<QChar> listChar, QList<QString> listText);
    void initBtnSytle(QList<QToolButton *> btns, QList<QChar> listChar, QList<QString> listText,QList<QString> listTextProperty);
    void initGroupMangerControl();              /*初始化分组管理控件*/
    void initDataMangerControl();               /*初始化数据库管理控件*/
    void initFaceMangerControl();               /*初始化人脸管理控件*/

    /*http 操作*/
    bool startHttpRequest(QUrl url);
    bool startHttpRequest(QUrl url,QString strType);
    /*解析http 返回的值*/
    void readDomXmlForReturnCmd();
    void readDomXmlForReturnValue();
    /*发送命令*/
    bool sendOneCmd(QString strDevIP,QString strCmdCode,QString par);
    /*分组管理*/
    void sendCmdForCreateGroup(QString strDevIP,QString par);
    void sendCmdForCreateGroup(QString groupName);
    void sendCmdForDeleteGroup(QString strDevIP,QString par);
    void sendCmdForSerachGroup(QString strDevIP,QString par="0");
    void getOnlineDevIDAndDevIP(QList<QString> &strListDevID,QList<QString> &strListDevIP);

    /*数据库管理*/
    void sendCmdForDeleteDataBase(QString strDevIP,QString par="1");

    /*人脸管理*/
    void onSendData();                          /*发送*/
    void updateDataHead();                      /*更新消息头*/
    void updateItemHead();                      /*更item头*/
    char *prepareFiles(QString strFilePath);    /*准备图片数据*/
    quint64 prepareFiles(QString strFilePath, QByteArray &baData);
    //void updateFiles();                        /*准备文件数据*/
    qint64 sendDataToServer();                  /*入库操作*/

    bool checkSetSataus();                      /*检查设置状态*/
    unsigned char * QStringToUChar(QString qstr,qint32 &size);
    void printfDataHead();                       /*打印数据头*/
    void printfItemHead();

    void sendCmdForFaceQuery();                 /*人脸查询*/



public:
    void addTabelViewRow(QList<QString> listText,QString strType = GroupMangerTable);
    void removeTableRow(QString strType = GroupMangerTable);
    void removeAllTable(QString strType = GroupMangerTable);
    void updateDisplayTable(QMap<QString,QList<QString>> map,QString strType = GroupMangerTable);
    QString getDataFromRowAndCol(int row,int col);
    int getDevIPFromID(QString strID, QString &strDevIp);  /*获取设备ID*/
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QList<QString> m_strListRecv;   /*接收http返回的值*/
    QMap<QString,QString> m_mapdevIp;/*devid:ip*/
private slots:
    void on_btnReTurnMenu_clicked();
    void getshowdlg();

    /*分组管理*/
    void onCreateGroupBtnClicked();
    void onDeteleGroupBtnClicked();
    void onSerachGroupBtnClicked();
    void onSaveGroupBtnClicked();
    void myTableForGroupMangerClicked(const QModelIndex &index);

    /*数据库管理*/
    void onDeleteDataMangerBtnClicked();

    /*人脸管理*/
    void onImportPicBtnClicked();
    void onAudiocheckBoxStateChanged(int state);
    void onAudioBtnClicked();
    void onOkBtnClicked();
    void onFaceQueryBtnClicked();   /*人脸查询*/
    void onSaveFilesBtnClicked(); /*保存特征值等文件*/
    void onImportMulPicBtnClicked();

    void onOpsComboxIndexChanged(QString text);
    void onGenderComboxIndexChanged(QString text);
    void onNameLineEditChanged(const QString &text);
    void onDevIdComboxIndexChanged(QString text);

    void onRecvFinsed(int itype,const QList<QByteArray> &baData);



signals:
    void showwnd();                 /* 发射显示主窗口信号*/

private:
    QPoint lastPos;
};

#endif // SYSSETWINDOW_H
