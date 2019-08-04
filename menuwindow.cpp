#include "menuwindow.h"
#include "ui_menuwindow.h"

#include "./uilib/iconhelper.h"

#define iconSize    50
#define iconWidth   100
#define iconHeight  70
#define navBtnWidth 45
#define navIcoWidth 18
MenuWindow::MenuWindow(QWidget *parent):
    QDialog(parent),
    ui(new Ui::MenuWindow)
{
    ui->setupUi(this);
    this->initForm();
    this->initNav();
}


MenuWindow::~MenuWindow()
{
    delete ui;
}

void MenuWindow::initForm()
{
    ui->labTitle->setText(tr("人脸识别系统"));
    this->setWindowTitle(ui->labTitle->text());
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    this->setProperty("canMove", true);
    ui->widgetTop->setProperty("flag", "nav");
    ui->widgetTop->setFixedHeight(40);

    IconHelper::Instance()->setIcon(ui->btnReturn, 0xf073, navIcoWidth);
    //IconHelper::Instance()->setIcon(ui->btnMsg, 0xf27a, navIcoWidth);
    IconHelper::Instance()->setIcon(ui->btnMsg, 0xf08b, navIcoWidth);
}

void MenuWindow::initNav()
{
    QList<QString> listColorBg;
    listColorBg << "#1570A5" << "#16A085" << "#C0392B" << "#047058" << "#9B59BB" << "#34495E";
    QList<QString> listColorText;
    listColorText << "#FEFEFE" << "#FEFEFE" << "#FEFEFE" << "#FEFEFE" << "#FEFEFE" << "#FEFEFE";

    QList<QChar> listChar;
    listChar << 0xf0c0 << 0xf109 << 0xf002 << 0xf06e << 0xf0e8 << 0xf085;
    QList<QString> listText;
    listText << "人员管理" << "设备监控" << "记录查询" << "人脸识别" << "设备管理" << "系统设置";

    btns << ui->btnViewPeople << ui->btnViewDevice << ui->btnData << ui->btnFace << ui->btnDeviceManger << ui->btnConfig;
    for (int i = 0; i < btns.count(); i++) {
        QToolButton *btn = btns.at(i);
        btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        btn->setIconSize(QSize(iconWidth, iconHeight));

        QPixmap pix = IconHelper::Instance()->getPixmap(listColorText.at(i), listChar.at(i), iconSize, iconWidth, iconHeight);
        //qDebug() << listChar.at(i);
        btn->setIcon(QIcon(pix));
        btn->setText(listText.at(i));

        //connect(btn, SIGNAL(clicked(bool)), this, SLOT(buttonClicked()));
    }
    connect(btns.at(0), SIGNAL(clicked(bool)), this, SLOT(btnViewPeopleClicked()));
    connect(btns.at(1), SIGNAL(clicked(bool)), this, SLOT(btnViewDeviceClicked()));
    connect(btns.at(2), SIGNAL(clicked(bool)), this, SLOT(btnDataClicked()));
    connect(btns.at(3), SIGNAL(clicked(bool)), this, SLOT(btnFaceClicked()));
    connect(btns.at(4), SIGNAL(clicked(bool)), this, SLOT(btnDeviceMangerClicked()));
    connect(btns.at(5), SIGNAL(clicked(bool)), this, SLOT(btnConfigClicked()));
#if 1
    QStringList qss;
    QFile qssFile(":/qrc/qss/menuwindow.qss");

    qssFile.open(QFile::ReadOnly);

   if(qssFile.isOpen())

    {

        this->setStyleSheet(tr(qssFile.readAll()));
        qssFile.close();
    }
    else
   {
       qDebug() << "open menuwindow.qss failed";
   }
#endif

}

void MenuWindow::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->globalX() - lastPos.x();
    int dy = event->globalY() - lastPos.y();
    lastPos = event->globalPos();
    move(x()+dx, y()+dy);

}

void MenuWindow::mousePressEvent(QMouseEvent *event)
{

    lastPos = event->globalPos();

}

void MenuWindow::mouseReleaseEvent(QMouseEvent *event)
{

    int dx = event->globalX() - lastPos.x();
    int dy = event->globalY() - lastPos.y();
    move(x()+dx, y()+dy);
}

void MenuWindow::on_btnMsg_clicked()
{
    accept();
}

void MenuWindow::btnViewPeopleClicked()
{

}

void MenuWindow::btnViewDeviceClicked()
{

}

void MenuWindow::btnDataClicked()
{

}

void MenuWindow::btnFaceClicked()
{
    this->hide();
    emit showFaceRecogdlg();
}
/*
 * 跳转到设备管理
 */
void MenuWindow::btnDeviceMangerClicked()
{
    this->hide();
    emit showDeviceMangerdlg();
}
/*
 * 跳转到系统配置
 */
void MenuWindow::btnConfigClicked()
{
    this->hide();
    emit showSysSetdlg();
}
/*
 * 显示子对话框
 */
void MenuWindow::getshowwnd()
{
    this->show();
}





