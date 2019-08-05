#ifndef DEVICEMANGERWINDOW_H
#define DEVICEMANGERWINDOW_H
#include <QDialog>
#include <QStandardItemModel>
#include <QUdpSocket>
#include <QThread>
#include "./toolsclass/devsendthread.h"
#include <QHostAddress>
#include <QTimer>
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
    //QList<QToolButton *> btns;
    QStandardItemModel *m_tableviewModel;
    QThread *m_SendThread;
    DevSendThread *m_SendObjThread;
    QString m_strfilePath;


private:
    void initForm();
    void initNav();
    void initTableView();
    void addTabelViewRowText(QStandardItemModel *model, int row, QList<QString> listText);
    void addTabelViewRow(QList<QString> listText);
    void removeTableRow();
    void removeAllRow();
    void createXmlForUdp();
public:
    void initDevDiscovery();
    void startDevSendThread();
    void stopDevSendThread();

public:

protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


private slots:
    void on_btnReTurnMenu_clicked();

    void getshowdlg();

    void btnDevFindClicked();
    void btnAddClicked();
    void btnDeleteClicked();

    //void updateDevInfoDisplay(QList<QList<QString>> strListText);
    void updateDevInfoDisplay(bool isUpdate);

signals:
    void showwnd();                 /* 发射显示主窗口信号*/

    void startUdpSendData();

private:
    QPoint lastPos;
};

#endif // SYSSETWINDOW_H
