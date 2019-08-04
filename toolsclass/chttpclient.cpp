#include "chttpclient.h"
#include <QDebug>
CHttpClient *CHttpClient::m_selfHttpClient = 0;


CHttpClient::CHttpClient()
{

}
/*单例模式*/
CHttpClient *CHttpClient::getInstance()
{
    if (!m_selfHttpClient) {
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if (!m_selfHttpClient) {
            m_selfHttpClient = new CHttpClient;
        }
    }
    return m_selfHttpClient;
}
/* 发起一个请求,返回reply对象*/
QNetworkReply *CHttpClient::startRequest(QUrl url)
{
    return get(QNetworkRequest(url));
}




