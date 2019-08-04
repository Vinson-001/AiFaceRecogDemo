#ifndef CLOGINDLG_H
#define CLOGINDLG_H

#include <QWidget>
#include "menuwindow.h"
#include "syssetwindow.h"
#include "facerecogwindow.h"
#include "deviceMangerwindow.h"

class SysSetWindow;
class MenuWindow;
class FaceRecogWindow;
class DeviceMangerWindow;
namespace Ui {
class CloginDlg;
}

class CloginDlg : public QWidget
{
    Q_OBJECT

public:
    explicit CloginDlg(QWidget *parent = 0);
    ~CloginDlg();

public:
    void initUicontrol();
private:
    QString m_userName;
    QString m_passWord;
    MenuWindow m_menuWin;
    SysSetWindow m_sysSetDlg;
    FaceRecogWindow m_faceRecogDlg;
    DeviceMangerWindow m_deviceMangerDlg;
private slots:
    void on_pushButton_clicked();

    void on_btnLogin_clicked();

    void on_comboBoxUserName_currentIndexChanged(const QString &arg1);

    void on_lineEditPassWord_textChanged(const QString &arg1);

private:
    Ui::CloginDlg *ui;

};

#endif // CLOGINDLG_H
