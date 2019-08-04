#include "facerecogwindow.h"
#include "ui_facerecogwindow.h"

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
FaceRecogWindow::FaceRecogWindow(QWidget *parent):
    QDialog(parent),
    ui(new Ui::FaceRecogWindow)
{
    ui->setupUi(this);
    this->initForm();
    this->initNav();
}


FaceRecogWindow::~FaceRecogWindow()
{
    delete ui;
}

void FaceRecogWindow::initForm()
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

void FaceRecogWindow::initNav()
{

#if 1
    QStringList qss;
    QFile qssFile(":/qrc/qss/facerecogwindow.qss");

    qssFile.open(QFile::ReadOnly);

   if(qssFile.isOpen())

    {

        this->setStyleSheet(tr(qssFile.readAll()));
        qssFile.close();
    }
#endif

}

void FaceRecogWindow::mouseMoveEvent(QMouseEvent *event)
{

    int dx = event->globalX() - lastPos.x();
    int dy = event->globalY() - lastPos.y();
    lastPos = event->globalPos();
    move(x()+dx, y()+dy);

}

void FaceRecogWindow::mousePressEvent(QMouseEvent *event)
{

    lastPos = event->globalPos();

}

void FaceRecogWindow::mouseReleaseEvent(QMouseEvent *event)
{

    int dx = event->globalX() - lastPos.x();
    int dy = event->globalY() - lastPos.y();
    move(x()+dx, y()+dy);
}
/*
 * 返回主菜单
 */
void FaceRecogWindow::on_btnReTurnMenu_clicked()
{
    //accept();
    //close();
    this->hide();               /*隐藏当前对话框 */
    emit showwnd();             /*显示主窗口 */

}

void FaceRecogWindow::getshowdlg()
{
    this->show();               /*显示对话框*/
}


