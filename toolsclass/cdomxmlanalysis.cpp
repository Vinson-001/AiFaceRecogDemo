#include "cdomxmlanalysis.h"

CDomXmlAnalysis::CDomXmlAnalysis()
{
    m_strFileName = "";
}
/*
 * 互斥
 */
CDomXmlAnalysis::CDomXmlAnalysis(QString strFileName,QString strTempFileName)
{
    m_strFileName = strFileName;
    m_strTempFileName = strTempFileName;
}
/*
 * 创建xml
 */
void CDomXmlAnalysis::CreateDomXml()
{
    QDomDocument doc;

    // 添加处理指令即XML说明
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml",
                                               "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    // 添加根元素
    QDomElement root = doc.createElement("Devices");
    doc.appendChild(root);

    // 添加第一个图书元素及其子元素
    QDomElement device = doc.createElement("Device");
    QDomAttr id = doc.createAttribute("Id");
    QDomElement name = doc.createElement("Name");
    QDomElement sn = doc.createElement("Sn");
    QDomElement prviate = doc.createElement("Prviate");
    QDomElement ip = doc.createElement("IpAddr");
    QDomElement status = doc.createElement("Status");
    QDomText text;
    id.setValue(QString("001"));
    device.setAttributeNode(id);
    text = doc.createTextNode("Dev1");
    name.appendChild(text);
    text = doc.createTextNode("2019010");
    sn.appendChild(text);
    text = doc.createTextNode("private data");
    prviate.appendChild(text);
    text = doc.createTextNode("193.169.3.111");
    ip.appendChild(text);
    text = doc.createTextNode("在线");
    status.appendChild(text);

    device.appendChild(name);
    device.appendChild(sn);
    device.appendChild(prviate);
    device.appendChild(ip);
    device.appendChild(status);
    root.appendChild(device);
#if 0
    // 添加第二个图书元素及其子元素
    book = doc.createElement("图书");
    id = doc.createAttribute("编号");
    title = doc.createElement("书名");
    author = doc.createElement("作者");
    id.setValue(QString("2"));
    book.setAttributeNode(id);
    text = doc.createTextNode("Linux");
    title.appendChild(text);
    text = doc.createTextNode("yafei");
    author.appendChild(text);
    book.appendChild(title);
    book.appendChild(author);
    root.appendChild(book);
 #endif
    QFile file(m_strFileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return ;
    QTextStream out(&file);

    // 将文档保存到文件，4为子元素缩进字符数
    doc.save(out, 4);
    file.close();
}

void CDomXmlAnalysis::CreateDomXml(QString strType)
{
    if(strType == DevInfoUdpType)
    {
        CreateDomXmlForUdp("001","Dev1","Sn1","Private","0.0.0.0");
    }
}
//--------------------以下for udp ---------------------------//
/*
 * 创建xml 用于udp
 *              <Devices>
 *                  <Device Id="001">
 *                      <Name>dev1</Name>
 *                      <Sn>sn1</Sn>
 *                      <Private>private</Private>
 *                      <IpAddr>ip</IpAddr>
 *                      <Status>status</Status>
 *                  </Device>
 *                  ...
 *              </Devices>
 */
void CDomXmlAnalysis::CreateDomXmlForUdp(QString strId, QString strName, QString strSn, \
                                   QString strPrivate, QString strIp, QString strStataus)
{
    QDomDocument doc;

    // 添加处理指令即XML说明
    QDomProcessingInstruction instruction;
    instruction = doc.createProcessingInstruction("xml",
                                               "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    // 添加根元素
    QDomElement root = doc.createElement("Devices");
    doc.appendChild(root);

    // 添加第一个图书元素及其子元素
    QDomElement device = doc.createElement("Device");
    QDomAttr id = doc.createAttribute("Id");
    QDomElement name = doc.createElement("Name");
    QDomElement sn = doc.createElement("Sn");
    QDomElement prviate = doc.createElement("Prviate");
    QDomElement ip = doc.createElement("IpAddr");
    QDomElement status = doc.createElement("Status");
    QDomText text;
    id.setValue(strId);
    device.setAttributeNode(id);
    text = doc.createTextNode(strName);
    name.appendChild(text);
    text = doc.createTextNode(strSn);
    sn.appendChild(text);
    text = doc.createTextNode(strPrivate);
    prviate.appendChild(text);
    text = doc.createTextNode(strIp);
    ip.appendChild(text);
    text = doc.createTextNode(strStataus);
    status.appendChild(text);

    device.appendChild(name);
    device.appendChild(sn);
    device.appendChild(prviate);
    device.appendChild(ip);
    device.appendChild(status);
    root.appendChild(device);

    QFile file(m_strFileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return ;
    QTextStream out(&file);

    // 将文档保存到文件，4为子元素缩进字符数
    doc.save(out, 4);
    file.close();
}

void CDomXmlAnalysis::AddXml(QString strType,const QString strId, QList<QString> strListText)
{
    if (strType == DevInfoUdpType) {
        doXmlForUdp("add",strId,strListText);
    }
}

void CDomXmlAnalysis::UpdateXml(QString strType,const QString strId, QList<QString> strListText)
{
    if (strType == DevInfoUdpType) {
        doXmlForUdp("update",strId,strListText);
    }
}

void CDomXmlAnalysis::DeleteXml(QString strType,const QString strId, QList<QString> strListText)
{
    if (strType == DevInfoUdpType) {
        doXmlForUdp("delete",strId,strListText);
    }
}

void CDomXmlAnalysis::ReadXml(QString strType)
{
    if (strType == DevInfoUdpType) {
        ReadXmlForUdp();
    }
}

void CDomXmlAnalysis::ReadXmlTemp(QString strType)
{
    if (strType == DevInfoUdpType) {
        ReadXmlTempForUdp();
    }
}

void CDomXmlAnalysis::AddElementToXml(QString strId,QString strName,\
                                      QString strSn, QString strPrivate, QString strIp,\
                                      QString strStaus)
{
    // 先清空显示，然后显示“无法添加！”，这样如果添加失败则会显示“无法添加！”
    QFile file(m_strFileName);
    if (!file.open(QIODevice::ReadOnly)) return;
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    QDomElement root = doc.documentElement();
    QDomElement device = doc.createElement("Device");
    QDomAttr id = doc.createAttribute("Id");
    QDomElement name = doc.createElement("Name");
    QDomElement sn = doc.createElement("Sn");
    QDomElement prviate = doc.createElement("Prviate");
    QDomElement ip = doc.createElement("IpAddr");
    QDomElement status = doc.createElement("Status");
    QDomText text;

    // 我们获得了最后一个孩子结点的编号，然后加1，便是新的编号
    //QString num = root.lastChild().toElement().attribute("Id");
    //int count = num.toInt() +1;
    //id.setValue(QString::number(count));
    id.setValue(strId);
    device.setAttributeNode(id);
    text = doc.createTextNode(strName);
    name.appendChild(text);

    text = doc.createTextNode(strSn);
    sn.appendChild(text);

    text = doc.createTextNode(strPrivate);
    prviate.appendChild(text);
    text = doc.createTextNode(strIp);
    ip.appendChild(text);
    text = doc.createTextNode(strStaus);
    status.appendChild(text);


    device.appendChild(name);
    device.appendChild(sn);
    device.appendChild(prviate);
    device.appendChild(ip);
    device.appendChild(status);
    root.appendChild(device);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return ;
    QTextStream out(&file);
    doc.save(out, 4);
    file.close();
}
/*
 * 解析xml
 *               <Devices>
 *                  <Device Id="001">
 *                      <Name>dev1</Name>
 *                      <Sn>sn1</Sn>
 *                      <Private>private</Private>
 *                      <IpAddr>ip</IpAddr>
 *                      <Status>status</Status>
 *                  </Device>
 *                  ...
 *              </Devices>
 *
 */
void CDomXmlAnalysis::ReadXmlForUdp()
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
    if(!m_strListListText.isEmpty())
        m_strListListText.clear();
    while(!n.isNull())
    {
        if (n.isElement())
        {
            QDomElement e = n.toElement();
            //ui->listWidget->addItem(e.tagName() + e.attribute("编号"));
            QDomNodeList list = e.childNodes();
            QList<QString> strlist;
            strlist.insert(0,e.attribute("Id"));
            for (int i=0; i<list.count(); i++)
            {
                QDomNode node = list.at(i);
                if(node.isElement()) {
                    strlist.insert(i+1,node.toElement().text());
                }
            }
            //qDebug() << strlist;
            m_strListListText.append(strlist);
        }
        n = n.nextSibling();
    }
    //qDebug() << m_strListListText;
}
/*
 *
 *          <Device>
 *              <Id>dev1</Id>
 *              <Name>dev1</Name>
 *              <Sn>sn1</Sn>
 *              <Private>private</Private>
 *          </Device>
 *
 *
 */
void CDomXmlAnalysis::ReadXmlTempForUdp()
{
    QFile file(m_strTempFileName);
    if (!file.open(QIODevice::ReadOnly)) return ;
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        return ;
    }
    file.close();
    QDomElement docElem = doc.documentElement();            /*device*/
    QDomNode n = docElem.firstChild();
    if(m_strLisTemp.isEmpty())
        m_strLisTemp.clear();
    while(!n.isNull())
    {
        if (n.isElement())
        {
            QDomElement e = n.toElement();

             m_strLisTemp.append(e.text());
        }
        n = n.nextSibling();
    }
    //qDebug() << m_strLisTemp ;
}

void CDomXmlAnalysis::AddElementToXmlForUdp(QString strId, QList<QString> strListText)
{

    //先查找


    QString strName = strListText.at(0);
    QString strSn = strListText.at(1);
    QString strPrivate = strListText.at(2);
    QString strIp = strListText.at(3);
    QString strStaus = strListText.at(4);
    QFile file(m_strFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        qDebug() << "(AddElementToXmlForUdp:line358)doc.setContent failed";
        file.close();
        return;
    }
    file.close();

    QDomElement root = doc.documentElement();
    QDomElement device = doc.createElement("Device");
    QDomAttr id = doc.createAttribute("Id");
    QDomElement name = doc.createElement("Name");
    QDomElement sn = doc.createElement("Sn");
    QDomElement prviate = doc.createElement("Prviate");
    QDomElement ip = doc.createElement("IpAddr");
    QDomElement status = doc.createElement("Status");
    QDomText text;

    // 我们获得了最后一个孩子结点的编号，然后加1，便是新的编号
    //QString num = root.lastChild().toElement().attribute("Id");
    //int count = num.toInt() +1;
    //id.setValue(QString::number(count));
    id.setValue(strId);
    device.setAttributeNode(id);
    text = doc.createTextNode(strName);
    name.appendChild(text);

    text = doc.createTextNode(strSn);
    sn.appendChild(text);

    text = doc.createTextNode(strPrivate);
    prviate.appendChild(text);
    text = doc.createTextNode(strIp);
    ip.appendChild(text);
    text = doc.createTextNode(strStaus);
    status.appendChild(text);


    device.appendChild(name);
    device.appendChild(sn);
    device.appendChild(prviate);
    device.appendChild(ip);
    device.appendChild(status);
    root.appendChild(device);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qDebug() << "(AddElementToXmlForUdp:401)open failed";
        return ;
    }
    QTextStream out(&file);
    doc.save(out, 4);
    file.close();
}
/*
 * 根据Id 删除获取更新元素值
 *
 */
void CDomXmlAnalysis::doXmlForUdp(const QString operate,\
                            const QString strId,QList<QString> strListText)
{
    bool isSameId = false;
    if(strListText.count()<5)
        return ;
    QString strName = strListText.at(0);
    QString strSn = strListText.at(1);
    QString strPrivate = strListText.at(2);
    QString strIp = strListText.at(3);
    QString strStaus = strListText.at(4);
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
        if(e.attribute("Id") == strId)                  //如果相等测更新或者删除元素值
        {   // 如果元素的“编号”属性值与我们所查的相同
            if (operate == "delete") {
                // 如果是删除操作
                QDomElement root = doc.documentElement();

                // 从根节点上删除该节点
                root.removeChild(list.at(i));
                QFile file(m_strFileName);
                if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
                    return ;
                QTextStream out(&file);
                doc.save(out,4);
                file.close();

            } else if (operate == "update") {
                // 如果是更新操作
                QDomNodeList child = list.at(i).childNodes();

                // 将它子节点的首个子节点（就是文本节点）的内容更新
                if(child.at(0).toElement().text() != strName)
                    child.at(0).toElement().firstChild().setNodeValue(strName);

                if(child.at(1).toElement().text() != strSn)
                    child.at(1).toElement().firstChild().setNodeValue(strSn);

                if(child.at(2).toElement().text() != strPrivate)
                child.at(2).toElement().firstChild().setNodeValue(strPrivate);

                if(child.at(3).toElement().text() != strIp)
                    child.at(3).toElement().firstChild().setNodeValue(strIp);

                if(child.at(4).toElement().text() != strStaus)
                    child.at(4).toElement().firstChild().setNodeValue(strStaus);
                QFile file(m_strFileName);
                if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
                    return ;
                QTextStream out(&file);
                doc.save(out,4);
                file.close();
            }
            isSameId = true;
            qDebug() << "update xml";
        }


    }
    if(!isSameId)        /*如果查询不到id,则创建一条元素*/
    {
        AddElementToXmlForUdp(strId,strListText);
        qDebug() << "add new element";
    }
}
//---------------------------end udp --------------------------------------------//
/*
 * 创建xml 用于http
 *              <Devices>
 *                  <Device Id="001">
 *                      <Name>dev1</Name>
 *                      <Sn>sn1</Sn>
 *                      <Private>private</Private>
 *                      <IpAddr>ip</IpAddr>
 *                      <Status>status</Status>
 *                  </Device>
 *                  ...
 *              </Devices>
 */
