#ifndef CHTTPCLIENT_H
#define CHTTPCLIENT_H
#include <QtCore>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
//#include <QUrl>
class CHttpClient : public QNetworkAccessManager
{
private:
    class CReleaseHttpClient                       /*释放 */
    {
    public:
        ~CReleaseHttpClient()
        {
            if(CHttpClient::m_selfHttpClient)
                delete CHttpClient::m_selfHttpClient;
                CHttpClient::m_selfHttpClient = NULL;
        }

    };
    static CReleaseHttpClient ReleaseHttpClient;    //自动析构
public:
    explicit CHttpClient();
    static CHttpClient *getInstance();              //获取一个实例
    QNetworkReply *startRequest(QUrl url);



private:
    static CHttpClient *m_selfHttpClient;
};

#endif // CHTTPCLIENT_H
