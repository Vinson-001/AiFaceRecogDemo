#include "canalysisxml.h"
#include <QDebug>
#include <QFile>
CAnalysisXml::CAnalysisXml()
{
    m_listElementText.append("0");
    m_listElementText.append("1");
    m_listElementText.append("3");
    m_listElementText.append("4");

}

void CAnalysisXml::XmlReadRoot()
{
    Q_ASSERT(m_readXml.isStartElement() && m_readXml.name().toString() == ROOT_ELEMENT_ENABLE);

     while (m_readXml.readNextStartElement()) {
         QString strName = m_readXml.name().toString();
         if (m_readXml.name().toString() == ROOT_ELEMENT_ENABLE) {
              XmlReadChild();
         } else {
             m_readXml.skipCurrentElement();  // 跳过当前元素
         }
     }
}

void CAnalysisXml::XmlReadChild()
{
    //Q_ASSERT(xml.isStartElement() && xml.name().toString() == BLOG_ELEMENT);

     while (m_readXml.readNextStartElement()) {
         if (m_readXml.name().toString() == CMD_ELEMENT) {
                m_listElementText[0] = m_readXml.readElementText();
                qDebug() << m_listElementText[0];
            }
         else if (m_readXml.name().toString() == STATUS_ELEMENT){
                m_listElementText[1] = m_readXml.readElementText();
                qDebug() << m_listElementText[1];
         }
         else
             m_readXml.skipCurrentElement();  // 跳过当前元素
     }
}
/*
 * 解析xml
 *      <...>
 *          <strElem1>text</strElem1>
 *          <strElem2>text</strElem2>
 *      </...>
 */
void CAnalysisXml::XmlReadChild(QString strElem1,QString strElem2)
{
    //Q_ASSERT(xml.isStartElement() && xml.name().toString() == BLOG_ELEMENT);

     while (m_readXml.readNextStartElement()) {
         if (m_readXml.name().toString() == strElem1) {
                m_listElementText[0] = m_readXml.readElementText();
                qDebug() << m_listElementText[0];
            }
         else if (m_readXml.name().toString() == strElem2){
                m_listElementText[1] = m_readXml.readElementText();
                qDebug() << m_listElementText[1];
         }
         else
             m_readXml.skipCurrentElement();  // 跳过当前元素
     }
}
/*
 * 解析xml
 *      <...>
 *          <strElem1>text</strElem1>
 *          <strElem2>text</strElem2>
 *          <strElem3>text</strElem4>
 *          <strElem3>text</strElem4>
 *      </...>
 */
void CAnalysisXml::XmlReadChild(QString strElem1,QString strElem2,QString strElem3,QString strElem4)
{

    while (m_readXml.readNextStartElement()) {
        QString strName = m_readXml.name().toString();
        if (strName == strElem1) {
               m_listElementText[0] = m_readXml.readElementText();
               qDebug() << m_listElementText[0];
           }
        else if (m_readXml.name().toString() == strElem2){
               m_listElementText[1] = m_readXml.readElementText();
               qDebug() << m_listElementText[1];
        }
        else if (m_readXml.name().toString() == strElem3){
               m_listElementText[2] = m_readXml.readElementText();
               qDebug() << m_listElementText[2];
        }
        else if (m_readXml.name().toString() == strElem4){
               m_listElementText[3] = m_readXml.readElementText();
               qDebug() << m_listElementText[3];
        }
        else
            m_readXml.skipCurrentElement();  // 跳过当前元素
    }
}
bool CAnalysisXml::read(QIODevice *device)
{
    m_readXml.setDevice(device);

    if (m_readXml.readNextStartElement()) {
        QString strName = m_readXml.name().toString();
        if (strName== ROOT_ELEMENT_ENABLE) {  // 获取根元素

            //XmlReadRoot();
            XmlReadChild();
            return true;
        } else {
            m_readXml.raiseError("XML file format error.");
            }
    }

    return !m_readXml.error();

}
// 解析 XML
/*  <strRootLabel>
 *      <>
 *      <>
 *      ..
 *  </strRootLabel>
 */
bool CAnalysisXml::read(QIODevice *device,QString strRootLabel,int count)
{
    m_readXml.setDevice(device);

    if (m_readXml.readNextStartElement()) {
        QString strName = m_readXml.name().toString();
        if (strName== strRootLabel) {  // 获取根元素

            //XmlReadChild();
            if(count == 2)
                XmlReadChild(CMD_ELEMENT,STATUS_ELEMENT);
            else
            {
                XmlReadChild(ID_ELEMENT,DevName_ELEMENT,SN_ELEMENT,PRIVATE_ELEMENT);
            }

            return true;
        } else {
            m_readXml.raiseError("XML file format error.");
            }
    }

    return !m_readXml.error();

}
// 解析 XML
/*
 *
 */

bool CAnalysisXml::readXML(const QString xmlPath, QString Cmd, QString status) {
    QString strFile(xmlPath);
    QFile file(xmlPath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {  // 以只读模式打开
        qDebug() << QString("Cannot read file %1(%2).").arg(strFile).arg(file.errorString());
        return false;
    }
    if (!read(&file)) {
        qDebug() << QString("Parse error in file %1.").arg(strFile);
        return false;
    }
    if(m_listElementText[0] == Cmd && m_listElementText[1] == status){
        file.close();
        return true;
    }else
    {
        file.close();
        return false;
    }
}
// 解析 XML
/*  <strRootLabel>
 *      <>
 *      <>
 *      ..
 *  </strRootLabel>
 */
bool CAnalysisXml::readXML(const QString xmlPath, QString strRootLabel, int count)
{
    QString strFile(xmlPath);
    QFile file(xmlPath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {  // 以只读模式打开
        qDebug() << QString("Cannot read file %1(%2).").arg(strFile).arg(file.errorString());
        return false;
    }
    if (!read(&file,strRootLabel,count)) {
        qDebug() << QString("Parse error in file %1.").arg(strFile);
        return false;
    }

    file.close();
    return true;

}


bool CAnalysisXml::readXML(QFile *file, QString Cmd, QString status)
{
    if(file == NULL)
    {
        qDebug() << "file is error";
        return false;
    }
    if (!read(file)) {
        qDebug() << QString("Parse error in file ");
        return false;
    }
    if(m_listElementText[0] == Cmd && m_listElementText[1] == status){
        //file.close();
        return true;
    }else
    {
        //file.close();
        return false;
    }
}


