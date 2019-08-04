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
    void CreateDomXmlForUdp(QString strId,QString strName,\
                      QString strSn, QString strPrivate, QString strIp,\
                      QString strStataus = QObject::tr("离线"));

    void AddXml(QString strType,const QString strId,QList<QString> strListText);
    void UpdateXml(QString strType,const QString strId,QList<QString> strListText);
    void DeleteXml(QString strType,const QString strId,QList<QString> strListText);
    void ReadXml(QString strType);
    void ReadXmlTemp(QString strType);


private:
    void doXmlForUdp(const QString operate,const QString strId,QList<QString> strListText);
    void AddElementToXmlForUdp(QString strId,QList<QString> strListText);
    void AddElementToXml(QString strId,QString strName,\
                         QString strSn, QString strPrivate, QString strIp,\
                         QString strStaus = QObject::tr("离线"));
    void ReadXmlForUdp();
    void ReadXmlTempForUdp();

private:
    QString m_strFileName;
    QString m_strTempFileName;

    //QList<QList<QString>> m_strListListText;
public:
    QList<QString> m_strLisTemp;
    QList<QList<QString>> m_strListListText;
};

#endif // CDOMXMLANALYSIS_H
