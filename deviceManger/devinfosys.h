#ifndef DEVINFOSYS_H
#define DEVINFOSYS_H
#include <QNetworkReply>
#include <QDialog>
#include <QMap>
#include "./toolsclass/http/chttpclient.h"
#define isEdit 1
#define isRegister 2

namespace Ui {
class DevInfoSys;
}

class DevInfoSys : public QDialog
{
    Q_OBJECT

public:
    explicit DevInfoSys(QWidget *parent = nullptr);
    DevInfoSys(QMap<QString,QString> map,int type,QWidget *parent = nullptr);
    DevInfoSys(QMap<QString,QString> map,QList<QString> strListRegister, int type,QWidget *parent = nullptr);
    ~DevInfoSys();

private slots:
    void on_btnOk_clicked();

    void on_isCannecl_clicked();

private:
    Ui::DevInfoSys *ui;
    QMap<QString,QString> m_mapSysInfo;
    int m_iType;
    QString m_strFilePath;
    QNetworkReply *m_reply;
    QList<QString> m_ListRegisterInfo;

public:
    QList<QString> m_strListRecv;

private:
    void initDlg();
    void displayDlg();
    void getDlgEditVaule(QString &strId,QMap<QString,QString> &mapList);
    void updateXml(QString strId,QList<QString> strListText);
    void addXml(QString strId,QList<QString> strListText);
    void setCmd(int mode,QString strIp,QList<QString> strListCmdPar);
    void setCmdisEdit(QString strIP,QList<QString> strListCmdPar);
    void setCmdisRegister(QString strIP,QList<QString> strListCmdPar);
    bool sendOneCmd(QString strDevIP,QString strCmdCode,QString par);
    void getCmdKeyFromCmdCode(int cmd,QString &strKry);
    bool startHttpRequest(QUrl url);

public:
    void setFilePath(QString strFilePath);
};

#endif // DEVINFOSYS_H
