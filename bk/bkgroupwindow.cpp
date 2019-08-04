#include "bkgroupwindow.h"
#include "ui_bkgroupwindow.h"

#include "./uilib/iconhelper.h"

#define iconSize    50
#define iconWidth   100
#define iconHeight  70
#define navBtnWidth 45
#define navIcoWidth 18
BkgroupWindow::BkgroupWindow(QWidget *parent):
    QDialog(parent),
    ui(new Ui::BkgroupWindow)
{
    ui->setupUi(this);
    this->initForm();
    this->initNav();
}


BkgroupWindow::~BkgroupWindow()
{
    delete ui;
}

void BkgroupWindow::initForm()
{
    ui->labTitle->setText(tr("人脸识别系统"));
    this->setWindowTitle(ui->labTitle->text());
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    this->setProperty("canMove", true);
    ui->widgetTop->setProperty("flag", "nav");
    ui->widgetTop->setFixedHeight(40);

    IconHelper::Instance()->setIcon(ui->btnPeople, 0xf0c0, navIcoWidth);
    //IconHelper::Instance()->setIcon(ui->btnMsg, 0xf27a, navIcoWidth);
    IconHelper::Instance()->setIcon(ui->btnReTurnMenu, 0xf122, navIcoWidth);
}

void BkgroupWindow::initNav()
{

#if 1
    QStringList qss;
    QFile qssFile(":/qrc/qss/bkgroupwindow.qss");

    qssFile.open(QFile::ReadOnly);

   if(qssFile.isOpen())

    {

        this->setStyleSheet(tr(qssFile.readAll()));
        qssFile.close();
    }
#endif

}

void BkgroupWindow::mouseMoveEvent(QMouseEvent *event)
{

    int dx = event->globalX() - lastPos.x();
    int dy = event->globalY() - lastPos.y();
    lastPos = event->globalPos();
    move(x()+dx, y()+dy);

}

void BkgroupWindow::mousePressEvent(QMouseEvent *event)
{

    lastPos = event->globalPos();

}

void BkgroupWindow::mouseReleaseEvent(QMouseEvent *event)
{

    int dx = event->globalX() - lastPos.x();
    int dy = event->globalY() - lastPos.y();
    move(x()+dx, y()+dy);
}

void BkgroupWindow::on_btnMsg_clicked()
{
    accept();
}


