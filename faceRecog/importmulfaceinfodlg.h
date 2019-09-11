#ifndef IMPORTMULFACEINFODLG_H
#define IMPORTMULFACEINFODLG_H

#include <QDialog>
#include "tcpclentobj.h"
#include "./facerecogwindow.h"
#include <QFileDialog>

class FaceRecogWindow;


namespace Ui {
class ImportMulFaceInfoDlg;
}

class ImportMulFaceInfoDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ImportMulFaceInfoDlg(QWidget *parent = 0);
    ~ImportMulFaceInfoDlg();

private:
    Ui::ImportMulFaceInfoDlg *ui;
    FaceRecogWindow *m_faceDlg;
    MyTableViewForImportMulPic *m_myTableForImportMulPic;

private:
    QString m_strDevId;
    quint8 m_ops;
    quint8 m_type;
    quint8 m_audio_support;                         /*音频支持*/
    request_head_t m_request_head;                  /*发送消息头*/
    //request_item_head_t m_item_head;                /*发送人员信息头*/

    //QList<QString> m_strPicFileNames;                  /*保存文件路径*/

    /*TCP */
    TcpClentObj *m_tcp_face_request;
public:
    QString m_strRegisterPath;

private:
    void initDataStruct();                           /*初始化数据结构*/
    void initUIControl();

    /*table操作*/
    void addTabelViewRow(QList<QString> listText);
    void removeTableRow();
    void removeAllTable();
    void updateDisplayTable(QMap<QString,QList<QString>> map);
    void setTableColsText(int col,QString strText);                     /*设置某一列为相同的值*/
    void updateDisplayTableForPicPath(QList<QString> strFileNames);
    void updateDisplayTableForAll(QString strFileName);                                    /*更新整张表*/

    QList<QString> getDataFromRow(int row);                             /*获取某一行的值*/
    void setDataForRow(int row,QList<QString> text);                    /*设置某一行的值*/

    unsigned char* QStringToUChar(QString qstr ,qint32 &size);
    /*准备数据*/
    void prepareDataHead(quint8 obj_num);
    void prepareItemHead(int row,request_item_head_t &request_item_head, QByteArray &baData);
    quint64 prepareFiles(QString filePath, QByteArray &baData);
    /*建立连接*/
    void getDevIPFromID(QString strID, QString &strDevIp);  /*获取设备ID*/
    void newConnection(QString strIp,quint16 port);
    /*发送数据头*/
    quint64 sendDataHead(request_head_t &request_head);
    /*发送入库信息到设备 单人*/
    quint64 sendOneItemInfoToTcpServer(request_item_head_t &request_item_head,QByteArray &fileData);


    void printfDataHead();
    void printfItemHead(request_item_head_t request_item_head);
    QString getFeatureName(int row, QString exp = ".feat");
    void saveFile(QString fileName,const QByteArray baData);

    void saveRecvFeatures(QString strDirPath);


public slots:
    void onOpsComboxTextChanged(QString text);
    void onTypeComboxTextChanged(QString text);
    void onAudiocheckBoxStateChanged(int state);
    void onLoadFilesBtnClicked();
    void onSendBtnClicked();
    void onSaveFeatureBtnClicked();
};

#endif // IMPORTMULFACEINFODLG_H
