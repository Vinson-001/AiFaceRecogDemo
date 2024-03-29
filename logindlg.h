#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QWidget>
#include "./menu/menuwindow.h"
#include "./systemset/syssetwindow.h"
#include "./faceRecog/facerecogwindow.h"
#include "./deviceManger/deviceMangerwindow.h"
#include "./RealEvent/realeventwindow.h"

class SysSetWindow;
class MenuWindow;
class FaceRecogWindow;
class DeviceMangerWindow;
namespace Ui {
class loginDlg;
}

class loginDlg : public QWidget
{
    Q_OBJECT

public:
    explicit loginDlg(QWidget *parent = 0);
    ~loginDlg();

public:
    void initUicontrol();
private:
    QString m_userName;
    QString m_passWord;

    MenuWindow m_menuWin;                       /*主窗口*/
    SysSetWindow m_sysSetDlg;                   /*系统设置*/
    FaceRecogWindow m_faceRecogDlg;             /*人脸管理*/
    DeviceMangerWindow m_deviceMangerDlg;       /*设备管理*/
    RealEventWindow m_realEventDlg;

private slots:
    void on_pushButton_clicked();

    void on_btnLogin_clicked();

    void on_comboBoxUserName_currentIndexChanged(const QString &arg1);

    void on_lineEditPassWord_textChanged(const QString &arg1);

private:
    Ui::loginDlg *ui;

};

#endif // LOGINDLG_H

