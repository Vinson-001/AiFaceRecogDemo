#ifndef CANALYSISXML_H
#define CANALYSISXML_H
#include <QXmlStreamReader>
#include <QFile>
/* root node */
#define ROOT_ELEMENT_ENABLE "Function"
#define ROOT_ELEMENT_DEVICE "Device"


/* http */
#define CMD_ELEMENT         "Cmd"
#define STATUS_ELEMENT      "Status"

/* udp */
#define ID_ELEMENT          "Id"
#define DevName_ELEMENT     "Name"
#define SN_ELEMENT          "Sn"
#define PRIVATE_ELEMENT     "Private"

#define VERSION_ATTRIBUTE   "version"

class CAnalysisXml
{
public:
    CAnalysisXml();
    void XmlReadRoot();
    void XmlReadChild();
    void XmlReadChild(QString strElem1,QString strElem2);
    void XmlReadChild(QString strElem1,QString strElem2,QString strElem3,QString strElem4);
    void readElementText();
    bool read(QIODevice *device);
    bool read(QIODevice *device, QString strRootLabel, int count);
    bool readXML(const QString xmlPath, QString Cmd,QString status);
    bool readXML(QFile *file, QString Cmd,QString status);
    bool readXML(const QString xmlPath, QString strRootLabel, int count = 2);


public:
     QXmlStreamReader m_readXml;
     QList <QString> m_listElementText;
};

#endif // CANALYSISXML_H
