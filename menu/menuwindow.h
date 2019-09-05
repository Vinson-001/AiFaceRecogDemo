#ifndef MENUWINDOW_H
#define MENUWINDOW_H
#include <QDialog>
#include "./systemset/syssetwindow.h"
class SysSetWindow;
class QToolButton;


namespace Ui {
class MenuWindow;
}


class MenuWindow : public QDialog
{
    Q_OBJECT
public:
    explicit MenuWindow(QWidget *parent = 0);
    ~MenuWindow();

private:
    Ui::MenuWindow *ui;
    QList<QToolButton *> btns;

private:
    void initForm();
    void initNav();
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


private slots:
    void on_btnMsg_clicked();

    void btnViewPeopleClicked();
    void btnViewDeviceClicked();
    void btnDataClicked();
    void btnFaceClicked();
    void btnDeviceMangerClicked();
    void btnConfigClicked();
    void btnRealEventClicked();


   void getshowwnd();
//自定义信号
signals:
   void showSysSetdlg();        /*显示系统设置对话框*/
   void showFaceRecogdlg();     /* 显示人脸识别对话框 */
   void showDeviceMangerdlg();  /* 显示设备管理*/
   void showRealEventdlg();     /* 显示实时事件对话框*/

private:
    QPoint lastPos;
    //BkgroupWindow m_dlg;
    SysSetWindow m_sysSetDlg;
private:

};

#endif // MENUWINDOW_H
