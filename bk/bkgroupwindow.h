#ifndef BKGROUPWINDOW_H
#define BKGROUPWINDOW_H
#include <QDialog>

class QToolButton;

namespace Ui {
class BkgroupWindow;
}


class BkgroupWindow : public QDialog
{
    Q_OBJECT
public:
    explicit BkgroupWindow(QWidget *parent = 0);
    ~BkgroupWindow();

private:
    Ui::BkgroupWindow *ui;
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



private:
    QPoint lastPos;
};

#endif // BKGROUPWINDOW_H
