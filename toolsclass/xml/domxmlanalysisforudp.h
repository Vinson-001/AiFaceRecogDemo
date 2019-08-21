/**
* @fileName      DomXmlAnalysisforUdp.h
* @brief         用于UDP传输 xml 解析
* @author        Wxyang
* @date          2019-08-07
*/
#ifndef DOMXMLANALYSISFORUDP_H
#define DOMXMLANALYSISFORUDP_H
#include "./domxmlanalysisbase.h"
#include <QFileInfo>
class DomXmlAnalysisForUnRegister : public DomXmlAnalysisBase
{
public:
    DomXmlAnalysisForUnRegister(QString strfilename);
    DomXmlAnalysisForUnRegister();


private:
    void initUnRegisterXml();
public:
    bool getDevInfo(QString strId, QList<QString> &strDevInfoList);
    void getDevIdList(QList<QString> &strIdList);

private:
    QString m_strFileName;

};

class DomXmlAnalysisForRegister : public DomXmlAnalysisBase
{
public:
    DomXmlAnalysisForRegister(QString strfilename);
    DomXmlAnalysisForRegister();


private:
    void initRegisterXml();

public:
    bool getDevInfo(QString strId, QList<QString> &strDevInfoList);
    bool getSysSetInfo(QString strId, QList<QString> &strSysInfoList);
    void getDevIdList(QList<QString> &strIdList);
    void setSysInfoToXml(QString strId,QList<QString> strList);
    void setDevOutlineStatus(QList<QString> strListId);
    void setDevStatus(QString strId,QString strStatus);

private:
    QString m_strFileName;
};

class DomXmlAnalysisForHttpTemp : public DomXmlAnalysisBase
{
public:
    DomXmlAnalysisForHttpTemp(QString strfilename);
    DomXmlAnalysisForHttpTemp();


private:

public:
    void createDomXml();

private:
    QString m_strFileName;
};

#endif // DOMXMLANALYSISFORUDP_H
