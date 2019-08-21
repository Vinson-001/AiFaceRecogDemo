#ifndef SYSSETWINDOW_H
#define SYSSETWINDOW_H
#include <QDialog>
#include <QButtonGroup>
#include <QStandardItemModel>
#include "./toolsclass/vipmodel.h"
#include <QUrl>
//#include "./toolsclass/canalysisxml.h"
class QToolButton;
class QButtonGroup;
class QStandardItemModel;
class VipModel;
class QNetworkReply;
class QFile;

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
    QButtonGroup *m_pButtonGroup ;
    //VipModel  *m_Vipmodel;
    QNetworkReply *m_reply;
    QFile *m_file;
    QString m_strfilePath;
    //CAnalysisXml m_readxml;
    QString m_xmlCmd;
    QString m_xmlStatus;
    QString m_xmlPar;
    QList<QString> m_listText;
    int m_row;

private:
    void initForm();
    void initNav();
    void startHttpRequest(QUrl url);
    void createRecvDataToXml(QString fileName);
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);


private slots:
    void on_btnReTurnMenu_clicked();

    void getshowdlg();
    //void onButtonGroupClicked(QAbstractButton *button);

    //checkbox
    void onRstpVideoStateChanged(int state);
    void onRstpOSDStateChanged(int state);
    void onfaceDetectionStateChanged(int state);
    void onfaceREcogStateChanged(int state);
    void onhumanDetectionStateChanged(int state);
    void onhumanRecogStateChanged(int state);
    void onpicSendStateChanged(int state);

    //http
    void httpFinished();
    void httpReadyRead();

signals:
    void showwnd();                 /* 发射显示主窗口信号*/

private:
    QPoint lastPos;

public:
    void addTabelViewRow(VipModel  *model,  int row, QList<QString> listText);  /*设置行数据*/
    void updateDataForTableViewRowandCol(VipModel  *model, int row,int col, QString strText);/*刷新数据*/
    void updateDataForTableViewRow(VipModel  *model, int row, QList<QString> listText); /*刷新一行 */
};

#endif // SYSSETWINDOW_H
