#include "syssetwindow.h"
#include "ui_syssetwindow.h"

#include "./uilib/iconhelper.h"

#define iconSize    50
#define iconWidth   50
#define iconHeight  45
#define navBtnWidth 45
#define navIcoWidth 18

#define ColorBg "#34495E"
#define ColorFontText "#FEFEFE"
#define ColorChar 0xf085
#define ColorlistText tr("系统设置")
SysSetWindow::SysSetWindow(QWidget *parent):
    QDialog(parent),
    ui(new Ui::SysSetWindow)
{
    ui->setupUi(this);
    this->initForm();
    this->initNav();
}


SysSetWindow::~SysSetWindow()
{
    delete ui;
}

void SysSetWindow::initForm()
{
    //QString ColorBg = "#34495E";

    //QString ColorText = "#FEFEFE";

    //QChar Char = 0xf085;
    //QString listText = tr("系统设置");

    //ui->labTitle->setText(tr("系统设置"));
    //->setWindowTitle(ui->labTitle->text());
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    this->setProperty("canMove", true);
    //->widgetTop->setProperty("flag", "nav");
    //ui->widgetTop->setFixedHeight(40);

    IconHelper::Instance()->setIcon(ui->btnPeople, 0xf0c0, navIcoWidth);
    //IconHelper::Instance()->setIcon(ui->btnMsg, 0xf27a, navIcoWidth);
    IconHelper::Instance()->setIcon(ui->btnReTurnMenu, 0xf122, navIcoWidth);


    QToolButton *btn = ui->btnPeople;
    btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    btn->setIconSize(QSize(iconWidth, iconHeight));

    QPixmap pix = IconHelper::Instance()->getPixmap(ColorFontText, ColorChar, iconSize/2, iconWidth, iconHeight);

    btn->setIcon(QIcon(pix));
    btn->setText(ColorlistText);
}

void SysSetWindow::initNav()
{

#if 1
    QStringList qss;
    QFile qssFile(":/qrc/qss/syssetwindow.qss");

    qssFile.open(QFile::ReadOnly);

   if(qssFile.isOpen())

    {

        this->setStyleSheet(tr(qssFile.readAll()));
        qssFile.close();
    }
#endif

}

void SysSetWindow::mouseMoveEvent(QMouseEvent *event)
{

    int dx = event->globalX() - lastPos.x();
    int dy = event->globalY() - lastPos.y();
    lastPos = event->globalPos();
    move(x()+dx, y()+dy);

}

void SysSetWindow::mousePressEvent(QMouseEvent *event)
{

    lastPos = event->globalPos();

}

void SysSetWindow::mouseReleaseEvent(QMouseEvent *event)
{

    int dx = event->globalX() - lastPos.x();
    int dy = event->globalY() - lastPos.y();
    move(x()+dx, y()+dy);
}
/*
 * 返回主菜单
 */
void SysSetWindow::on_btnReTurnMenu_clicked()
{
    //accept();
    //close();
    this->hide();               /*隐藏当前对话框 */
    emit showwnd();             /*显示主窗口 */

}

void SysSetWindow::getshowdlg()
{
    this->show();               /*显示对话框*/
}


