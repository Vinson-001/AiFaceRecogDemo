#include "devdiscoverythread.h"
#define DISCOVERY_SERVER_ADDR "239.255.255.221"
#define DISCOVERY_SERVER_PORT 3188
DevDiscoveryThread::DevDiscoveryThread(QObject *parent) : QObject(parent)
{
    m_timer = NULL;
    m_udpSocket = NULL;
    m_isStop = false;
    initUdp();
}

DevDiscoveryThread::~DevDiscoveryThread()
{
    enableThread(isStop);
    delete m_udpSocket;
    m_udpSocket = NULL;
}
/**
 * @funcname  initUdp
 * @brief     bind udp
 * @return    no
 */

void DevDiscoveryThread::initUdp()
{
    m_timer = new QTimer(this);
    m_udpSocket = new QUdpSocket(this);
    if (m_udpSocket->state() != m_udpSocket->BoundState)
    {
        // Rx not in bound state, attempt to bind
        m_udpSocket->bind(QHostAddress::AnyIPv4,DISCOVERY_SERVER_PORT,QUdpSocket::ShareAddress);
    }
    connect(m_udpSocket, SIGNAL(readyRead()),this, SLOT(udpRecvData()),Qt::DirectConnection);
#if DEBUG_TIMER
    connect(m_timer, SIGNAL(timeout()), this, SLOT(udpSendData()),Qt::DirectConnection);
#endif
    m_strfileTempPath = QDir::currentPath();
    m_strfileTempPath += "/DevInfoTemp.xml";
    m_strfileRegPath = QDir::currentPath() + "/DevInfoRegister.xml";
    m_strfileUnRegPath = QDir::currentPath() + "/DevInfoUnRegister.xml";
    createTempXmlforRecvData(m_strfileTempPath);
}
/**
 * @funcname  createTempXmlforRecvData
 * @brief     创建临时文件
 * @param     fileName
 * @return    no
 */

void DevDiscoveryThread::createTempXmlforRecvData(QString fileName)
{
    QFileInfo file(fileName);
    if(file.exists() == false) {
        QFile file(fileName);
        int ret = file.open(QIODevice::WriteOnly| QIODevice::Text);
        if(!ret) {
            qDebug() << file.errorString() << "DevInfoTemp.xml failed";
            return;
        }
        file.close();
    }
}
/**
 * @funcname  readDomTempXml
 * @brief     读取xml临时文件
 * @param     strListText output param
 * @return    no
 */

void DevDiscoveryThread::readDomTempXml(QList<QString> &strListText)
{
    DomXmlAnalysisBase domXml(m_strfileTempPath);
    domXml.readDomXmlTemp(strListText);
}
/**
 * @funcname  isDomRegisterXml
 * @brief     查询注册表中是否存在id
 * @param     strId
 * @return    ret
 */

bool DevDiscoveryThread::isDomRegisterXml(QString strId)
{
    DomXmlAnalysisForRegister domXml(m_strfileRegPath);
    return domXml.isDomXmlstrIdexist(strId);
}
/**
 * @funcname  updateRegXml
 * @brief     更新注册表信息(name,sn,private,ip,status)
 * @param     strId
 * @param     strListText
 * @return    no
 */

void DevDiscoveryThread::updateRegXml(QString strId, QList<QString> strListText)
{
    DomXmlAnalysisForRegister domXml(m_strfileRegPath);
    domXml.doXml("update",strId,strListText);
}
/**
 * @funcname  addUnRegXml
 * @brief     添加到未注册表单里,添加时已经查重过
 * @param     strId
 * @param     strListText
 * @return    no
 */

void DevDiscoveryThread::addUnRegXml(QString strId, QList<QString> strListText)
{
    DomXmlAnalysisForUnRegister domXml(m_strfileUnRegPath);
    //domXml.createDomXml("Devices");                 /*添加前重新创建文件，重写*/
    domXml.doXml("add",strId,strListText);
}
/**
 * @funcname  setDevOutlineStatusList
 * @brief     设置离线状态
 * @param     strLitId 需要更新的列表
 * @param     无
 * @return    无
 */
void DevDiscoveryThread::setDevOutlineStatusList(QList<QString> strLitId)
{
    DomXmlAnalysisForRegister domXml(m_strfileRegPath);
    domXml.setDevOutlineStatus(strLitId);
}
/**
 * @funcname  setDevOnlineStatus
 * @brief     设置strID 的在线状态
 * @param     param1
 * @param     param2
 * @return    ret
 */
void DevDiscoveryThread::setDevOnlineStatus(QString strId)
{
    DomXmlAnalysisForRegister domXml(m_strfileRegPath);
    domXml.setDevStatus(strId,tr("在线"));
}
/**
 * @funcname  getCommonString
 * @brief     从两个列表里提取相同的字符串(2,3,4) (1,2,3)
 * @param     strXmlId
 * @param     strListRecvId
 * @return    QList<QString> 返回相同的字符串
 */
QList<QString> DevDiscoveryThread::getOutlineStringId(QList<QString> strListRecvId, QList<QString> strXmlListId)
{
    QList<QString> strTmpRecv(strListRecvId);                  //strRecvId副本(2,3,4)
    for(int i = 0; i < strXmlListId.count(); i++)
    {
        strTmpRecv.removeAll(strXmlListId.at(i));              //去除与strxml相同的元素(4)
    }
    //qDebug() << "strid 新增" << strTmpRecv;
    QList<QString> strTmpXml(strXmlListId);                   //strXmlId副本(1,2,3)
    for(int i = 0; i < strListRecvId.count(); i++)
    {
        strTmpXml.removeAll(strListRecvId.at(i));             //去除与recvId 相同的元素(1)
    }
    //qDebug() << "xmlId 离线" << strTmp2;
    return strTmpXml;

#if 0
    QList<QString> strTmpSame(strRecvList);                   //(2,3,4)
    for(int i = 0; i < strTmpXml.count(); i++)
    {
        strTmpSame.removeAll(strTmpXml.at(i));                //(2,3)
    }
#endif
}

QList<QString> DevDiscoveryThread::getOutlineStringIdInRegisterXml(QList<QString> strListRecvId)
{
    /*1.获取注册表ID列表*/
    QList<QString> strListXmlId;
    DomXmlAnalysisForRegister domXml(m_strfileRegPath);
    domXml.getDevIdList(strListXmlId);
    return getOutlineStringId(strListRecvId,strListXmlId);
}
/**
 * @funcname  enableThread
 * @brief     使能线程
 * @param     en true | stop false
 * @return    no
 */

void DevDiscoveryThread::enableThread(bool en)
{
    QMutexLocker locker(&m_stopMutex);
    m_isStop = en;
}


/**
 * @funcname  udpSendData
 * @brief     udp发送设备请求
 * @return    no
 */

void DevDiscoveryThread::udpSendData()
{
    QByteArray datagram = "Device Discovery";
    m_udpSocket->writeDatagram(datagram.data(), datagram.size(),QHostAddress(DISCOVERY_SERVER_ADDR),DISCOVERY_SERVER_PORT);
    qDebug() << datagram.data();
}

void DevDiscoveryThread::udpRecvData()
{
    enableThread(isRun);
    //int count = 0;
    int icount = 0;
    if (!m_isStop) {
       qDebug() << "break recvdata";
       return;
    }
    DomXmlAnalysisForUnRegister domXml(m_strfileUnRegPath);
    domXml.createDomXml("Devices");                 /*添加前重新创建文件，重写*/
    QList<QString> strSaveRecvId;                   /*保存接收的ID*/
    while (m_udpSocket->hasPendingDatagrams()) {
        QThread::msleep(1000);

        QByteArray datagram;
        QHostAddress sendAddress;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(), datagram.size(),&sendAddress);
        QString recv_ip = sendAddress.toString();

        if(sendAddress.protocol() == QAbstractSocket::IPv4Protocol){
               /*1. 保存xml */
               QFile file(m_strfileTempPath);
               if(!file.open(QIODevice::WriteOnly| QIODevice::Text))
                   return;
               file.resize(0);                  /*清空*/
               file.write(datagram);
               file.close();

               /*2. 解析xml */
               /*2.1 解析临时xml */
               QList<QString> strListTemp;
               readDomTempXml(strListTemp);
               /*2.2 保存id：ip */
               //qDebug() << strListTemp;
               strListTemp.append(recv_ip);         /*id,name,model,sn,private,ip,status*/
               strListTemp.append(tr("在线"));

               /*3. 查询注册表xml*/
               QString strId = strListTemp.at(0);
               //strId.remove("\r\n");
               bool isexist = isDomRegisterXml(strId);
               if(!isexist)
               {
                   QString strUnRegisterId,strTempId;

                   strTempId = QString("ID%1:").arg(icount);
                   /*ID1:(-1)*/
                   strUnRegisterId = strTempId + QString("(%1)").arg(strId);
                   icount++;
                   strListTemp.removeAt(0);
                   if(strListTemp.at(4).isEmpty())
                       strListTemp.replace(4,"NULL");
                   QMutexLocker locker(&m_lockForXml);
                   addUnRegXml(strUnRegisterId,strListTemp);
                   //strSaveRecvId.append(strUnRegisterId);           /*保存未注册的ID*/
                   qDebug() << strListTemp;
                }
               else
               {
                    strSaveRecvId.append(strId);                    /*保存已注册的ID*/
                    setDevOnlineStatus(strId);                      /*更新在线状态*/
               }
               //count++;
           }
       }


       //isUpdate = true;
       /*5.查询注册表，更新离线的设备状态*/
        QList<QString>isOutlineListId;
        isOutlineListId = getOutlineStringIdInRegisterXml(strSaveRecvId);
        qDebug() << "isOnlineListId" << isOutlineListId;
        if(!isOutlineListId.isEmpty()){
            setDevOutlineStatusList(isOutlineListId);
        }
       /*6. 发射信号*/
       emit startUpdateDevInfo();

}

void DevDiscoveryThread::startSending()
{
#if DEBUG_TIMER
    if(!m_timer->isActive())
        m_timer->start(1000*10);
#else
    udpSendData();
#endif
}
