#ifndef DEVICEMANGERWINDOW_H
#define DEVICEMANGERWINDOW_H
#include <QDialog>
#include <QStandardItemModel>
#include <QNetworkReply>
#include <QThread>
#include "./toolsclass/udpthread/devdiscoverythread.h"
#include <QTimer>
#include "./toolsclass/tabelview/mytableview.h"
#include "./toolsclass/xml/domxmlanalysisforudp.h"
#include "./toolsclass/tabelview/mytableviewdelegate.h"
#include "./toolsclass/http/chttpclient.h"
#include "./toolsclass/cmd/httpcmd.h"
class QToolButton;

namespace Ui {
class DeviceMangerWindow;
}


class DeviceMangerWindow : public QDialog
{
    Q_OBJECT
public:
    explicit DeviceMangerWindow(QWidget *parent = 0);
    ~DeviceMangerWindow();

private:
    Ui::DeviceMangerWindow *ui;

    MyTableView *m_myTableView;
    QThread *m_SendThread;
    DevDiscoveryThread *m_SendObjThread;
    QString m_strRegisterPath;
    QString m_strUnRegisterPath;
    bool m_isUpdate;
    bool m_isAdd;
    bool m_isModified;

    QNetworkReply *m_reply;
    QMutex m_lockForXml;
    QList<QString> m_strListRecv;

private:
    void initForm();
    void initNav();
    void initTableView();
    void addTabelViewRow(QList<QString> listText);
    void removeTableRow();
    void removeAllRow();
    QString getDataFromRowAndCol(int row,int col);
    QList<QString> getDataFromRow(int row);
    void displayRegisterInfo();
    void displayUnRegisterInfo();
    bool isInRegisterXml(QString strId);
    void deleteUnRegisterXml(QString strId);
    void deleteRegisterXml(QString strId);
    void removeAllRegisterXml();
    /*http 操作*/
    void startHttpRequest(QUrl url);
    void sendHttpCmd(QString strCmd);


public:
    void initDevDiscovery();
    void startDevSendThread();
    void stopDevSendThread();

public:

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


public slots:
    void on_btnReTurnMenu_clicked();

    void getshowdlg();
    /*界面按钮*/
    void btnDevFindClicked();
    void btnAddClicked();
    void btnDeleteClicked();
    void btnRemoveAllClicked();

    /*tabelview*/
    void updateDevInfoDisplay();
    void showToolTip(QModelIndex index);
    void addRegisterSlot(const QModelIndex &index);
    void editRegisterSlot(const QModelIndex &index);


signals:
    void showwnd();                 /* 发射显示主窗口信号*/

    void startUdpSendData();

    void getReplyStatusSignal(QString strCmd);

private:
    QPoint lastPos;
};

#endif // SYSSETWINDOW_H
