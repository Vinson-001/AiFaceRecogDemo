#ifndef SYSSETWINDOW_H
#define SYSSETWINDOW_H
#include <QDialog>

class QToolButton;

namespace Ui {
class SysSetWindow;
}


class SysSetWindow : public QDialog
{
    Q_OBJECT
public:
    explicit SysSetWindow(QWidget *parent = 0);
    ~SysSetWindow();

private:
    Ui::SysSetWindow *ui;
    //QList<QToolButton *> btns;

private:
    void initForm();
    void initNav();
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


private slots:
    void on_btnReTurnMenu_clicked();

    void getshowdlg();
signals:
    void showwnd();                 /* 发射显示主窗口信号*/

private:
    QPoint lastPos;
};

#endif // SYSSETWINDOW_H
