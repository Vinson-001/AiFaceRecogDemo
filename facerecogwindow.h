#ifndef FACERECOGWINDOW_H
#define FACERECOGWINDOW_H
#include <QDialog>

class QToolButton;

namespace Ui {
class FaceRecogWindow;
}


class FaceRecogWindow : public QDialog
{
    Q_OBJECT
public:
    explicit FaceRecogWindow(QWidget *parent = 0);
    ~FaceRecogWindow();

private:
    Ui::FaceRecogWindow *ui;
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
