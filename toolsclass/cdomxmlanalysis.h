#ifndef CDOMXMLANALYSIS_H
#define CDOMXMLANALYSIS_H
#include <QtXml>
#include <QFile>
#define DevInfoUdpType  "DevInfoUdp"
class CDomXmlAnalysis
{
public:
    CDomXmlAnalysis();
    CDomXmlAnalysis(QString strFileName,QString strTempFileName);
    void CreateDomXml();
    void CreateDomXml(QString strType);
    void CreateDomXmlForUdp();

    void AddXml(QString strType,const QString strId,QList<QString> strListText);
    void UpdateXml(QString strType,const QString strId,QList<QString> strListText);
    void UpdateXml(QString strType,const QList<QString> strListId,QList<QList<QString>> strListListText);
    void DeleteXml(QString strType,const QString strId,QList<QString> strListText);
    void ReadXml(QString strType);
    void ReadXmlTemp(QString strType);
    void getXmlCount(QString strType, int &count);


private:
    void doXmlForUdp(const QString operate,const QString strId,QList<QString> strListText, QString strStataus);
    void AddElementToXmlForUdp(QString strId,QList<QString> strListText);
    void AddElementToXml(QString strId,QString strName,\
                         QString strSn, QString strPrivate, QString strIp,\
                         QString strStaus = QObject::tr("在线"));
    void ReadXmlForUdp();
    void ReadXmlTempForUdp();
    void getXmlCountForUdp(int &count);
    void getCompareRecvAndXmlElement(const QList<QString> strRecvList, const QList<QString> strXmlList, \
                                     QList<QString> &isRecvList, QList<QString> &isXmlList,QList<QString> &isSameList);
    void getXmlListId(QList<QString> &xmlListId);

private:
    QString m_strFileName;
    QString m_strTempFileName;

    //QList<QList<QString>> m_strListListText;
public:
    QList<QString> m_strLisTemp;                    /*解析xml 临时保存的值*/
    QList<QList<QString>> m_strListListText;        /*解析完成后保存的值*/
};

#endif // CDOMXMLANALYSIS_H
