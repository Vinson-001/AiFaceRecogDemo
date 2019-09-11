#ifndef REALEVENTWINDOW_H
#define REALEVENTWINDOW_H
#include <QDialog>
#include <QThread>
#include "./myrealeventsocket.h"
#include "./RealEvent/mytcpserverforrealevent.h"

class QToolButton;
class MyTcpServerForRealEvent;
namespace Ui {
class RealEventWindow;
}


class RealEventWindow : public QDialog
{
    Q_OBJECT
public:
    explicit RealEventWindow(QWidget *parent = 0);
    ~RealEventWindow();

private:
    Ui::RealEventWindow *ui;
    QList<QToolButton *> btns;

    /*RealEvent*/
    MyTcpServerForRealEvent *m_TcpServerRealEvent;
private:
    rtime_event_head_t m_rtime_event_head;



private:
    void initForm();
    void initNav();
    void initBtnSytle(QList<QToolButton *> btns, QList<QChar> listChar, QList<QString> listText);
    void initBtnSytle(QList<QToolButton *> btns, QList<QChar> listChar, QList<QString> listText,QList<QString> listTextProperty);
    void initRealEventGroupControl();

    /*实时事件*/
    void startTcpRealEventThread();
    void stopTcpRealEventThread();
    void printfRtimeEventInfo();
    void showRecogPic(QString strPic);
    void showRecogPicInfo();
    bool isDirExist(QString fullPath);
    bool isFileExist(QString fullFilePath);
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void showwnd();                 /* 发射显示主窗口信号*/

    void startRecvDataFromServer(); /* 开始接收realtime 数据*/
    void stopRtimeEventThread();
private slots:
    void on_btnReTurnMenu_clicked();
    void getshowdlg();

    void onIsRealTimeRecvFinshed(const QString&, const rtime_event_head_t&, const QByteArray&);/* 接收完成*/

    void btnStartClicked(); /*开启线程*/
    void btnStopClicked();  /*stop*/



private:
    QPoint lastPos;
};

#endif // REALEVENTWINDOW_H
