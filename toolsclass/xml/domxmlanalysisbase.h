/**
* @filename      domxmlanalysisbase.h
* @brief         dom 解析xml:
*                       1.添加(addDomXml) 、删除(deleteDomXml) 、修改(updateDomXml)某一元素
*                       2.遍历(getDomXmlstrIdList) 元素ID
*                       3.查找(isDomXmlstrIdexist) ID 是否存在
*                       4.读(readDomXml)单一元素、读(readDomXmlTemp)临时元素、遍历(readDomXmlAll)所有元素
*                       5.设置(setXmlElement)根节点，子节点（构造时调用）
*                       6.创建(createDomXml) xml文件
* @author        Wxyang
* @date          2019-08-07
*/
#ifndef DOMXMLANALYSISBASE_H
#define DOMXMLANALYSISBASE_H
#include <QtXml>
#include <QDebug>
#include <QMessageBox>

#define Element_ID "DeviceID"

class DomXmlAnalysisBase
{
public:
    DomXmlAnalysisBase();
    DomXmlAnalysisBase(QString strfilename);
private:
    QString m_strFileName;
    QList<QString> m_ElementList;
    QString m_RootElement;

private:

    //void addDomXml(QString strId,QList<QString> strListText);
    void addDomXml(QString strId,QList<QString> strElementListText);
    void deleteDomXml(QString strId);
    void updateDomXml(QString strId,QList<QString> strListText);
    void getDomXmlstrIdList(QList<QString> &strIdList);
public:
    bool createDomXml(QString strRoot);
    void doXml(QString operate,QString strId,QList<QString> strListText);
    bool isDomXmlstrIdexist(QString strId);
    void readDomXml(QString strId, QList<QString> &strListText);
    void readDomXmlTemp(QList<QString> &strListText);
    void readDomXmlAll(QMap<QString,QList<QString>> &mapElement);
    int setXmlElement(QString strRootElement,QList<QString> strList);
    void removeAllDoxXml();

};

#endif // DOMXMLANALYSISBASE_H
