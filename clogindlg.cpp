#include "clogindlg.h"
#include "ui_clogindlg.h"
#include <QMessageBox>


CloginDlg::CloginDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CloginDlg)
{
    ui->setupUi(this);
    initUicontrol();
    m_userName = "admin";
    m_passWord = "";

}

CloginDlg::~CloginDlg()
{
    delete ui;
}

void CloginDlg::initUicontrol()
{
    //1.禁止最大化按钮
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());

    ui->comboBoxUserName->clear();
    QStringList strlist;
    strlist << "admin" << "user1" << "user2";
    ui->comboBoxUserName->addItems(strlist);

    //2. 连接信号与槽函数
    connect(&m_menuWin,SIGNAL(showSysSetdlg()),&m_sysSetDlg,SLOT(getshowdlg()));            /*显示系统设置子窗口*/
    connect(&m_sysSetDlg,SIGNAL(showwnd()),&m_menuWin,SLOT(getshowwnd()));                  /*显示主窗口 */

    connect(&m_menuWin,SIGNAL(showDeviceMangerdlg()),&m_deviceMangerDlg,SLOT(getshowdlg()));      /*显示人脸识别子窗口*/
    connect(&m_deviceMangerDlg,SIGNAL(showwnd()),&m_menuWin,SLOT(getshowwnd()));               /* 显示主窗口 */

    connect(&m_menuWin,SIGNAL(showFaceRecogdlg()),&m_faceRecogDlg,SLOT(getshowdlg()));      /*显示人脸识别子窗口*/
    connect(&m_faceRecogDlg,SIGNAL(showwnd()),&m_menuWin,SLOT(getshowwnd()));               /* 显示主窗口 */
}

void CloginDlg::on_pushButton_clicked()
{
    close();
}

void CloginDlg::on_btnLogin_clicked()
{
    if(m_userName == "admin" && m_passWord == "admin")
    {
        this->hide();
        m_menuWin.show();
        m_sysSetDlg.hide();
        m_faceRecogDlg.hide();
        m_deviceMangerDlg.hide();
        //qint32 ret = w.exec();
       // if(ret == MenuWindow::Accepted)
//        {
            //close();
        //}

    }
    else if (m_userName == "user1" && m_passWord == "")
    {
        close();
        //MenuWindow w;
        //qint32 ret = w.exec();
        //if(ret == MenuWindow::Accepted)
        {

        }
    }
    else if (m_userName == "user2" && m_passWord == "")
    {
        close();
        SysSetWindow w;
        qint32 ret = w.exec();
        if(ret == SysSetWindow::Accepted)
        {

        }
    }
    else
    {
        QMessageBox::information(this,tr("提示对话框"),tr("请输入正确的用户名和密码"),QMessageBox::Ok);
    }
}
/*
 * 当选中窗口
 */
void CloginDlg::on_comboBoxUserName_currentIndexChanged(const QString &arg1)
{
    m_userName = arg1;

}

void CloginDlg::on_lineEditPassWord_textChanged(const QString &arg1)
{
    m_passWord = arg1;
}
