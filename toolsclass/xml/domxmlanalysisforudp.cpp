#include "domxmlanalysisforudp.h"
#include <QFile>
/**
* @fileName      DomXmlAnalysisForUnRegister
* @brief         未注册表单
* @author        Wxyang
* @date          2019-08-09
*/
DomXmlAnalysisForUnRegister::DomXmlAnalysisForUnRegister(QString strfilename)
    :DomXmlAnalysisBase (strfilename),
      m_strFileName(strfilename)
{

    QFileInfo file(strfilename);
    bool isflag = file.exists();
    if(!isflag)
    {
        createDomXml("Devices");
    }
    initUnRegisterXml();
}
DomXmlAnalysisForUnRegister::DomXmlAnalysisForUnRegister()
{

}

void DomXmlAnalysisForUnRegister::initUnRegisterXml()
{
    QString strRootElement = "Device";
    QList<QString> strElementListText;
    strElementListText << "DeviceName" << "DeviceModel" << "DeviceSn" << "DeviceMac" << "Private" << "DeviceIpAddr" << "Status";
    setXmlElement(strRootElement,strElementListText);
}
/**
 * @funcname  getDevInfo
 * @brief     获取设备信息,用于显示,同时添加strID
 * @param     strId
 * @param     strDevInfoList output
 * @return    no
 */
bool DomXmlAnalysisForUnRegister::getDevInfo(QString strId, QList<QString> &strDevInfoList)
{
    QList<QString> strListText;
    readDomXml(strId,strDevInfoList);
    strDevInfoList.prepend(strId);
    return true;
}

void DomXmlAnalysisForUnRegister::getDevIdList(QList<QString> &strIdList)
{
    QFile file(m_strFileName);
    if (!file.open(QIODevice::ReadOnly)) return ;
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        return ;
    }
    file.close();
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    if(!strIdList.isEmpty())
        strIdList.clear();
    while(!n.isNull())
    {
        if (n.isElement())
        {
            QDomElement e = n.toElement();
            //ui->listWidget->addItem(e.tagName() + e.attribute("编号"));
            QDomNodeList list = e.childNodes();
            strIdList.append(e.attribute(Element_ID));
        }
        n = n.nextSibling();
    }
}







/**
* @fileName      DomXmlAnalysisForRegister
* @brief         注册表单
* @author        Wxyang
* @date          2019-08-09
*/
DomXmlAnalysisForRegister::DomXmlAnalysisForRegister(QString strfilename)
    :DomXmlAnalysisBase (strfilename),
     m_strFileName(strfilename)
{

    QFileInfo file(strfilename);
    if(!file.exists())
    {
        createDomXml("Devices");
        qDebug() << "Devices";
    }
    initRegisterXml();
}
DomXmlAnalysisForRegister::DomXmlAnalysisForRegister()
{

}

/**
 * @funcname  initRegisterXml
 * @brief     插入一个节点
 * @return    void
 * @example
 *           <Devices>
 *               <Device Id="1">
 *                   <Name>test_dev</Name>
 *                   <Model>T-1031</Model>
 *                   <Sn>0123456789</Sn>
 *                   <Mac>00-0c-00-01-14-3d</Mac>
 *                   <Private>this is prvate data </Private>
 *                   <DevIpAddr>193.169.3.111</DevIpAddr>
 *                   <Status>在线</Status>
 *                   <FtpIpAddr>0.0.0.0</FtpIpAddr>
 *                   <FtpCapturePath>/mnt/path</FtpCapturePath>
 *                   <FtpRecogPath>/mnt/path</FtpRecogPath>
 *                   <RtcEventPort>5555</RtcEventPort>
 *                   <warnport>5555</warnport>
 *               </Device>
 *               ...
 *           </Devices>
 */

void DomXmlAnalysisForRegister::initRegisterXml()
{
    QString strRootElement = "Device";
    QList<QString> strElementListText;
    strElementListText << "DeviceName" << "DeviceModel" << "DeviceSn" << "DeviceMac" << "Private" << "DeviceIpAddr" << "Status"\
                           << "FtpServerIpAddr" << "FtpServerCapPath" << "FtpServerRecoPath" << "HostRealTimeEventPort" << "HostAlarmEventPort";
    setXmlElement(strRootElement,strElementListText);
    if(!m_strElementListText.isEmpty())
        m_strElementListText.clear();
    m_strElementListText = strElementListText;
}
/**
 * @funcname  getDevInfo
 * @brief     获取设备信息,用于显示,同时添加strID
 * @param     strId
 * @param     strDevInfoList output
 * @return    no
 */

bool DomXmlAnalysisForRegister::getDevInfo(QString strId,QList<QString> &strDevInfoList)
{
    QList<QString> strListText;
    readDomXml(strId,strListText);
    if(strListText.count() != 12)
        return false;
    strDevInfoList.clear();
    strListText.removeLast();
    strListText.removeLast();
    strListText.removeLast();
    strListText.removeLast();
    strListText.removeLast();
    strDevInfoList = strListText;
    strDevInfoList.prepend(strId);
    return true;
}
/**
 * @funcname  getDevInfo
 * @brief     获取设置信息,同时添加ID
 * @param     strId
 * @param     strSysInfoList output
 * @return    no
 */
bool DomXmlAnalysisForRegister::getSysSetInfo(QString strId, QList<QString> &strSysInfoList)
{
    QList<QString> strListText;
    readDomXml(strId,strListText);
    if(strListText.count() != 12)
        return false;
    strSysInfoList.clear();
    strSysInfoList.append(strId);                   /*Dev Id*/
    strSysInfoList.append(strListText.at(0));       /*Dev Name*/
    strSysInfoList.append(strListText.at(5));       /*Dev IP*/
    strSysInfoList.append(strListText.at(7));       /*Ftp IP*/
    strSysInfoList.append(strListText.at(8));       /*Ftp Capture*/
    strSysInfoList.append(strListText.at(9));       /*Ftp Recog*/
    strSysInfoList.append(strListText.at(10));      /*RTC Port*/
    strSysInfoList.append(strListText.at(11));      /*Warn Port*/
    return true;
}
/**
 * @funcname  getElementTextFromDevId
 * @brief     获取某一指定元素值
 * @param     param1
 * @param     param2
 * @return    ret
 */
void DomXmlAnalysisForRegister::getElementTextFromDevId(QString strId, QString strElement, QString &strText)
{
    QList<QString> strListText;
    readDomXml(strId,strListText);
    int index = 0;
    if(m_strElementListText.contains(strElement)) {
        index = m_strElementListText.indexOf(strElement);
        strText = strListText.at(index);
    }

}

void DomXmlAnalysisForRegister::getDevIdList(QList<QString> &strIdList)
{
    QFile file(m_strFileName);
    if (!file.open(QIODevice::ReadOnly)) return ;
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        return ;
    }
    file.close();
    QDomElement docElem = doc.documentElement();
    QDomNode n = docElem.firstChild();
    if(!strIdList.isEmpty())
        strIdList.clear();
    while(!n.isNull())
    {
        if (n.isElement())
        {
            QDomElement e = n.toElement();
            //ui->listWidget->addItem(e.tagName() + e.attribute("编号"));
            QDomNodeList list = e.childNodes();
            strIdList.append(e.attribute(Element_ID));
        }
        n = n.nextSibling();
    }
}
/**
 * @funcname  getDevIpListFromIdList
 * @brief     根据设备ID获取对应IP
 * @param     strIdList 输入设备ID
 * @param     strIpList 输出设备IP
 * @return    no
 */
void DomXmlAnalysisForRegister::getDevIpListFromIdList(QList<QString> strIdList, QList<QString> &strIpList)
{
   int count = strIdList.count();
   for (int i = 0; i < count; i++) {
       QString strId = strIdList.at(i);
       QString strText;
       getElementTextFromDevId(strId,"DeviceIpAddr",strText);
       strIpList.append(strText);
   }

}
/**
 * @funcname  getDevIdListFromOnline
 * @brief     获取在线状态的设备ID
 * @param     strIdList 输出参数
 * @return    no
 */
void DomXmlAnalysisForRegister::getDevIdListFromOnline(QList<QString> &strIdList)
{
    QFile file(m_strFileName);
    if (!file.open(QIODevice::ReadOnly)) return ;
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        return ;
    }
    file.close();
    QDomElement docElem = doc.documentElement();                /*root:Devices*/
    QDomNode n = docElem.firstChild();                          /*Device node*/
    while(!n.isNull())
    {
        if (n.isElement())
        {
            QDomElement e = n.toElement();
            //ui->listWidget->addItem(e.tagName() + e.attribute("编号"));
            QDomNodeList list = e.childNodes();                 /*Device element*/
            QList<QString> strlistElement;
            QString strId = e.attribute(Element_ID);

            if (list.isEmpty()) {
                return;
            }
            QDomNode node = list.at(6);                         /*list[6] 在线、离线*/
            QString strStatus = node.toElement().text();
            //qDebug() << strStatus;
            if (strStatus == QObject::tr("在线"))
            {
                strIdList.append(strId);
            }
        }
        n = n.nextSibling();
    }
    //qDebug() << mapElement;
}
/**
 * @funcname  getDevIpFromId
 * @brief     根据设备ID获取IP
 * @param     strId 设备ID
 * @param     strIp 设备IP
 * @return    ret
 */
void DomXmlAnalysisForRegister::getDevIpFromId(QString strId, QString &strIp)
{
    getElementTextFromDevId(strId,"DeviceIpAddr",strIp);
}

/**
 * @funcname  setSysInfoToXml
 * @brief     设置系统信息到注册表
 * @param     strId
 * @param     strList
 * @return    no
 */
void DomXmlAnalysisForRegister::setSysInfoToXml(QString strId, QList<QString> strList)
{
    if(strList.count()<8) {
        QMessageBox::critical(NULL,"注意","strList 小于8");
        return ;
    }

    QString strName = strList.at(1);
    QString strIp = strList.at(2);
    QString ftpIp = strList.at(3);
    QString ftpCapPath = strList.at(4);
    QString ftpRecogPath = strList.at(5);
    QString rtcPort = strList.at(6);
    QString warnPort = strList.at(7);

    QFile file(m_strFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return ;
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        return ;
    }
    file.close();

    // 以标签名进行查找
    QDomNodeList list = doc.elementsByTagName("Device");

    for(int i=0; i<list.count(); i++)
    {
        QDomElement e = list.at(i).toElement();
        if(e.attribute(Element_ID) == strId)
        {   // 如果元素的“编号”属性值与我们所查的相同

            // 如果是更新操作
            QDomNodeList child = list.at(i).childNodes();

            // 将它子节点的首个子节点（就是文本节点）的内容更新，离线时只更新状态
            child.at(0).toElement().firstChild().setNodeValue(strName);
            child.at(5).toElement().firstChild().setNodeValue(strIp);
            child.at(7).toElement().firstChild().setNodeValue(ftpIp);
            child.at(8).toElement().firstChild().setNodeValue(ftpCapPath);
            child.at(9).toElement().firstChild().setNodeValue(ftpRecogPath);
            child.at(10).toElement().firstChild().setNodeValue(rtcPort);
            child.at(11).toElement().firstChild().setNodeValue(warnPort);

            QFile file(m_strFileName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            return ;
            QTextStream out(&file);
            doc.save(out,4);
            file.close();
            qDebug() << "update Element xml";
            break;
        }
    }
}
/**
 * @funcname  setDevOnlineStatus
 * @brief     更新离线状态
 * @param     strListId 需要更新的离线ID列表
 * @param     param2
 * @return    ret
 */
void DomXmlAnalysisForRegister::setDevOutlineStatus(QList<QString> strListId)
{
    int count = strListId.count();
    for(int i = 0;i < count;i++){
        setDevStatus(strListId.at(i),QObject::tr("离线"));
    }
}
/**
 * @funcname  setDevStatus
 * @brief     设置设备状态
 * @param     strListId 需要更新的离线ID
 * @param     param2
 * @return    ret
 */
void DomXmlAnalysisForRegister::setDevStatus(QString strId,QString strStatus)
{
    QFile file(m_strFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return ;
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        return ;
    }
    file.close();

    // 以标签名进行查找
    QDomNodeList list = doc.elementsByTagName("Device");

    for(int i=0; i<list.count(); i++)
    {
        QDomElement e = list.at(i).toElement();
        if(e.attribute(Element_ID) == strId)
        {   // 如果元素的“编号”属性值与我们所查的相同

            // 如果是更新操作
            QDomNodeList child = list.at(i).childNodes();

            // 将它子节点的首个子节点（就是文本节点）的内容更新，离线时只更新状态
            child.at(6).toElement().firstChild().setNodeValue(strStatus);

            QFile file(m_strFileName);
            if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
            return ;
            QTextStream out(&file);
            doc.save(out,4);
            file.close();
            qDebug() << "update Element xml";
            break;
        }
    }
}



//-------------------------------------------------------------------------------------------//
/**
* @fileName
* @brief         以下是Http接收的Xml临时处理
* @author        Wxyang
* @date          2019-08-13
*/
DomXmlAnalysisForHttpTemp::DomXmlAnalysisForHttpTemp(QString strfilename)
    :DomXmlAnalysisBase(strfilename)
{
    QFileInfo file(strfilename);
    m_strFileName = strfilename;
    //if(!file.exists())
    {
       // createDomXml();
    }
}
/**
 * @funcname  readDomXmlTempForReturnCmd
 * @brief     解析xml返回的命令状态
 *                                  <Funtion>
 *                                      <Cmd><Cmd>
 *                                      <Status></Status>
 *                                  </Funtion>
 * @param     strList 输出结果
 * @return    no
 */
void DomXmlAnalysisForHttpTemp::readDomXmlTempForReturnCmd(QList<QString> &strList)
{
    readDomXmlTemp(strList);
}
/**
 * @funcname  readDomXmlTempForReturnValue
 * @brief     解析xml返回的value
 *                                  <Funtion>
 *                                      <Group>
 *                                          <ID></ID>
 *                                          <Members></Members>
 *                                      <Group>
 *                                      <Group>
 *                                          <ID></ID>
 *                                          <Members></Members>
 *                                      <Group>
 *                                      ....
 *                                  </Funtion>
 * @param     strList 输出结果
 * @return    no
 */
void DomXmlAnalysisForHttpTemp::readDomXmlTempForReturnValue(QMap<QString,QList<QString>> &mapElement)
{
    QFile file(m_strFileName);
    if (!file.open(QIODevice::ReadOnly)) return ;
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        return ;
    }
    file.close();
    QDomElement docElem = doc.documentElement();                            /*Funtion*/
    QDomNode n = docElem.firstChild();                                      /*Group1*/
    int icount = 0;
    if(!mapElement.isEmpty())
        mapElement.clear();
    while(!n.isNull())
    {
        if (n.isElement())
        {
            QDomElement e = n.toElement();                                  /*Group1 node*/
            //ui->listWidget->addItem(e.tagName() + e.attribute("编号"));
            QDomNodeList list = e.childNodes();
            QList<QString> strlistElement;
            //strlistElement.insert(0,e.attribute("Id"));
            //QString strId = e.attribute("Id");
            QString Index ;
            icount++;
            Index = QString("Index%1").arg(icount);
            for (int i=0; i<list.count(); i++)                              /*每个子标签元素*/
            {

                QDomNode node = list.at(i);
                if(node.isElement()) {
                    strlistElement.insert(i,node.toElement().text());
                }
            }
            //qDebug() << strlist;
            mapElement.insert(Index,strlistElement);

        }
        n = n.nextSibling();                                                /* 跳转到下一节点 */
    }
    //qDebug() << mapElement;
}
/**
 * @funcname  createDomXml
 * @brief     创建普通文件即可,重载
 * @param     param1
 * @param     param2
 * @return    ret
 */

void DomXmlAnalysisForHttpTemp::createDomXml()
{

    QFile file(m_strFileName);

    int ret = file.open(QIODevice::WriteOnly| QIODevice::Text);
    if(!ret)
    {
        qDebug() << file.errorString();
        file.close();
        return;
    }
    file.close();
}
