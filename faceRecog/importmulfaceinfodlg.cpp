#include "importmulfaceinfodlg.h"
#include "ui_importmulfaceinfodlg.h"

/*
 * 批量导入数据结构
 *      数据头  |人员信息头+ face   + audio    |...|人员信息头+face+audio|
 *      32bytes|192bytes+face_len+ audio_len|
 */


ImportMulFaceInfoDlg::ImportMulFaceInfoDlg(QWidget *parent) :
    QDialog(parent),
    m_ops(0),
    m_type(0),
    m_audio_support(0),
    m_myTableForImportMulPic(NULL),
    ui(new Ui::ImportMulFaceInfoDlg)
{
    m_faceDlg = dynamic_cast<FaceRecogWindow *>(parent);
    m_strDevId = m_faceDlg->m_strDevID;
    m_tcp_face_request = m_faceDlg->m_tcp_face_request;
    m_strRegisterPath = m_faceDlg->m_strRegisterPath;
    ui->setupUi(this);
    initDataStruct();
    initUIControl();
}

ImportMulFaceInfoDlg::~ImportMulFaceInfoDlg()
{
    delete ui;
}
/**
 * @funcname  initDataStruct
 * @brief     初始化数据结构
 * @param     no
 * @param     no
 * @return    no
 */
void ImportMulFaceInfoDlg::initDataStruct()
{
    /*1.清空*/
    memset(&m_request_head,0,sizeof(request_head_t));

    /*2.初始化*/
    strncpy((char *)m_request_head.magic, REQUEST_HEAD_MAGIC, 4);
    /*objnum最后更新*/
    m_request_head.ops = INT_INLibrary;                              /*默认入库*/
    m_request_head.type = 0;                                         /*默认只是图片入库*/
}

void ImportMulFaceInfoDlg::initUIControl()
{
    /*设置设备ID*/

    ui->devIdlineEdit->setText(m_strDevId);
    /* ops 设置*/
    ui->opsComboBox->addItem(tr(TEXT_INLibrary),STR_INLibrary);
    ui->opsComboBox->addItem(tr(TEXT_OUTFeature),STR_OUTFeature);
    /* type 设置*/
    ui->typeComboBox->addItem(tr(TEXT_PIC_TYPE),STR_PIC_TYPE);
    ui->typeComboBox->addItem(tr(TEXT_PIC_IMG_TYPE),STR_PIC_IMG_TYPE);

    /*设置table*/
    m_myTableForImportMulPic = new  MyTableViewForImportMulPic(this);
    QStandardItemModel *ImporMulPicModel = new QStandardItemModel(m_myTableForImportMulPic);
    /*3.1 设置model*/
    m_myTableForImportMulPic->setTableModel(ImporMulPicModel);
    /*3.2 创建表格，并初始化 */
    m_myTableForImportMulPic->initMyTableView();
    /*3.3 调整位置*/
    m_myTableForImportMulPic->setTableColWidth(600);
    m_myTableForImportMulPic->setGeometry(20,60,605,300);



    /*检查ops状态*/
    connect(ui->opsComboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(onOpsComboxTextChanged(QString)));
    /*检查type*/
    connect(ui->typeComboBox,SIGNAL(currentTextChanged(QString)),this,SLOT(onTypeComboxTextChanged(QString)));

    /*音频支持*/
    connect(ui->audioCheckBox, SIGNAL(stateChanged(int)), this, SLOT(onAudiocheckBoxStateChanged(int)));

    /*加载文件*/
    connect(ui->loadFilesBtn, SIGNAL(clicked(bool)), this, SLOT(onLoadFilesBtnClicked()));
    /*保存，最后入库*/
    connect(ui->sendBtn, SIGNAL(clicked(bool)), this, SLOT(onSendBtnClicked()));

    /*保存特征值*/
    connect(ui->saveFeatBtn, SIGNAL(clicked(bool)), this, SLOT(onSaveFeatureBtnClicked()));

    ui->sendBtn->setEnabled(false);
   // ui->saveFeatureBtn->setEnabled(false);



}
/**
 * @funcname  addTabelViewRow
 * @brief     新增一行
 * @param     listText  每列的显示内容（不包含序号）
 * @param     strType  表格类型
 * @return    no
 */
void ImportMulFaceInfoDlg::addTabelViewRow(QList<QString> listText)
{
    m_myTableForImportMulPic->doTable("add",listText);
}
/**
 * @funcname  removeTableRow
 * @brief     删除选中的行
 * @return    无
 */
void ImportMulFaceInfoDlg::removeTableRow()
{
    QList<QString> listText;
    m_myTableForImportMulPic->doTable("remove",listText);
}
/**
 * @funcname  removeAllTable
 * @brief     清空表格
 * @param     param1
 * @param     param2
 * @return    ret
 */
void ImportMulFaceInfoDlg::removeAllTable()
{
    QList<QString> listText;
    m_myTableForImportMulPic->doTable("removeAll",listText);

}
/**
 * @funcname  updateDisplayTable
 * @brief     更新列表显示
 * @return    no
 */
void ImportMulFaceInfoDlg::updateDisplayTable(QMap<QString, QList<QString> > map)
{
    QList<QString> listText;
    QMap<QString,QList<QString>>::const_iterator i;

    for (i = map.constBegin(); i != map.constEnd(); ++i) {
        listText = i.value();
        addTabelViewRow(listText);
    }
}
/**
 * @funcname  设置表格某一列的值
 * @brief     gaiyao
 * @param     col 从1开始
 * @param     param2
 * @return    ret
 */
void ImportMulFaceInfoDlg::setTableColsText(int col, QString strText)
{
    QStandardItemModel *model = m_myTableForImportMulPic->m_model;
    int count  = model->rowCount();
    for(int i = 0; i < count; i++){
        /*设置某一列的值*/
        model->setItem(i,col-1,new QStandardItem(strText));
        model->item(i,col-1)->setTextAlignment(Qt::AlignCenter);
    }
}
/**
 * @funcname  updateDisplayTableForPicPath
 * @brief     更新表格信息
 * @param     strFileNames 文件全路径
 * @param     param2
 * @return    ret
 */
void ImportMulFaceInfoDlg::updateDisplayTableForPicPath(QList<QString> strFileNames)
{
    int count = strFileNames.count();
    QList<QString> listText;
    listText << "" << "" << "" << "" << "" << "" << "" << "" << ""; /*不包含序号*/
    for(int i = 0; i < count;i++){
        QString strFileName = strFileNames.at(i);

        listText.replace(5,strFileName);
        addTabelViewRow(listText);
    }
}
/**
 * @funcname  updateDisplayTableForAll
 * @brief     更新整张表的内容
 * @param     strFileName 配置文件路径
 * @param     param2
 * @return    ret
 */
void ImportMulFaceInfoDlg::updateDisplayTableForAll(QString strFileName)
{
    /*1. 读配置文件，获取整个list值*/
    DomXmlAnalysisForHttpTemp xml(strFileName);
    QMap<QString,QList<QString>> map;
    xml.readDomXmlTempForReturnValue(map);
    /*2. 获取原表格内容*/
    QStandardItemModel *model = m_myTableForImportMulPic->m_model;
    int count  = model->rowCount();
    int mcount = map.count();
    if(count == 0 || mcount != count)
        return;
    //QMap<QString,QList<QString>> textMap;
    for(int i = 0 ;i < count; i++){
        QList<QString> strlistOld = getDataFromRow(i+1);
        QString key = QString("Index%1").arg(i+1);
        QList<QString> strListNew = map.value(key);
        int newCount = strListNew.count();
        for(int j = 0;j < newCount; j++){
            strlistOld.replace(j,strListNew.at(j));
        }
        strlistOld.prepend(QString("%1").arg(i));
        setDataForRow(i+1,strlistOld);
        //textMap.insert(key,strlistOld);
    }
    //removeAllTable();
    //updateDisplayTable(textMap);
}
/**
 * @funcname  getDataFromRow
 * @brief     获取行信息
 * @param     row 行从1开始
 * @param     param
 * @return    返回整行信息（不包含序号）
 */
QList<QString> ImportMulFaceInfoDlg::getDataFromRow(int row)
{
    QList<QString> strList;
    strList = m_myTableForImportMulPic->getValueFromRow(row);
    return strList;
}
/**
 * @funcname  setDataForRow
 * @brief     设置某一行的值
 * @param     param1
 * @param     param2
 * @return    ret
 */
void ImportMulFaceInfoDlg::setDataForRow(int row, QList<QString> text)
{
    QStandardItemModel *model = m_myTableForImportMulPic->m_model;
    int count  = model->columnCount();
    if(text.isEmpty() || count == 0)
        return;
    for(int i = 0; i < count; i++){
        /*设置某一行的值*/
        QString strText = text.at(i);
        model->setItem(row-1,i,new QStandardItem(strText));
        model->item(row-1,i)->setTextAlignment(Qt::AlignCenter);
    }
}
/**
 * @funcname  QStringToUChar
 * @brief     QString 转换为 unsigned char
 * @param     qstr
 * @param     size
 * @return    返回 unsigned char
 */
unsigned char* ImportMulFaceInfoDlg::QStringToUChar(QString qstr, qint32 &size)
{
    char*  ch;
    //QByteArray ba = qstr.toLocal8Bit(); // must
    QByteArray ba = qstr.toUtf8();
    ch=ba.data();
    size = ba.size();
    return (unsigned char *)ch;
}
/**
 * @funcname  prepareDataHead
 * @brief     准备数据头
 * @param     obj_num 人员个数
 * @param     param2
 * @return    ret
 */
void ImportMulFaceInfoDlg::prepareDataHead(quint8 obj_num)
{
    memset(&m_request_head,0,sizeof(request_head_t));

    strncpy((char *)m_request_head.magic, REQUEST_HEAD_MAGIC, 4);
    /*objnum最后更新*/
    m_request_head.ops = m_ops;
    m_request_head.type = m_type;
    m_request_head.obj_num = obj_num;
}
/**
 * @funcname  prepareItemHead
 * @brief     准备信息头
 * @param     row 行从1开始
 * @param     request_item_head 输出值
 * @param     baData 文件buffer
 * @return    ret
 */
void ImportMulFaceInfoDlg::prepareItemHead(int row,request_item_head_t &request_item_head,QByteArray &baData)
{
    /*获取行信息*/
    QList<QString> strList;                 /* name,gender,age,gid,pid,picpath,featpath,adudio_support,audiopath*/
    strList = getDataFromRow(row);          /* 不包含序号*/
    quint32 gid = strList.at(3).toInt();    /* gid */
    quint32 pid = strList.at(4).toInt();    /* pid */
    QString strName = strList.at(0);        /* name*/
    unsigned char *pName;
    int len = 0;
    pName = QStringToUChar(strName,len);

    quint8 gender;
    if(strList.at(1) == tr("男")){           /* 性别*/
        gender = 0;
    }
    else{
        gender = 1;
    }
    quint8 age = strList.at(2).toInt();     /* 年龄*/
    QString strPicPath = strList.at(5);
    strncpy((char *)request_item_head.magic, REQUEST_ITEM_HEAD_MAGIC, 4);
    request_item_head.gid = gid;
    request_item_head.id = pid;
    memcpy(request_item_head.name, pName ,len);                            /*英文1，中文utf-8*/
    request_item_head.gender = gender;
    request_item_head.age = age;
    request_item_head.data_num = 1;

    /*准备pic*/
    quint64 picSize = prepareFiles(strPicPath,baData);
    request_item_head.data_len_0 = picSize;
    //request_item_head.data_len_0
    if(m_ops == INT_INLibrary){
        request_item_head.feat_len_0 = 0;
    }
    else{
        /*获取特征值*/
        request_item_head.feat_len_0 = 512;
    }
    request_item_head.audio_support = m_audio_support;
    if(m_audio_support==1){

        //request_item_head.audio_len = audio_size; /*获取音频大小*/
    }
    else{
        /*获取音频*/
        request_item_head.audio_len = 0;
    }
    printfItemHead(request_item_head);

}
/**
 * @funcname  prepareFiles
 * @brief     准备文件数据
 * @param     filePath 文件路径
 * @param     baData 保存的文件值
 * @return    返回文件大小
 */
quint64 ImportMulFaceInfoDlg::prepareFiles(QString filePath, QByteArray &baData)
{
    QFile file(filePath);
    QFileInfo fileInfo(file);
    quint64 size = fileInfo.size();
    if(!file.exists())
        return -1;
    if(!file.open(QIODevice::ReadOnly)) {
        return -1;
    }
    int rtotal = 0;
    while(rtotal != size)  {
        QByteArray ba;
        ba = (file.read(size - rtotal));
        rtotal += ba.size();
        baData.append(ba);
    }
    return size;
}

void ImportMulFaceInfoDlg::getDevIPFromID(QString strID, QString &strDevIp)
{
    DomXmlAnalysisForRegister regXml(m_strRegisterPath);
    regXml.getDevIpFromId(strID,strDevIp);

}
/**
 * @funcname  newConnection
 * @brief     连接服务器
 * @param     strIp 设备IP
 * @param     port 端口号
 * @return    ret
 */
void ImportMulFaceInfoDlg::newConnection(QString strIp, quint16 port)
{
    m_tcp_face_request->newConnect(strIp,port);
}
/**
 * @funcname  sendDataHead
 * @brief     发送数据头
 * @param     request_head 数据头信息
 * @param     no
 * @return    返回成功发送的字节数
 */
quint64 ImportMulFaceInfoDlg::sendDataHead(request_head_t &request_head)
{
    int  rsize = 0;

    rsize = m_tcp_face_request->sendData((char *)&request_head,sizeof(request_head_t));
    return rsize;
}
/**
 * @funcname  sendOneItemInfoToTcpServer
 * @brief     发送单个人的信息
 * @param     request_item_head 信息头
 * @param     fileData 图片数据
 * @return    返回成功发送的字节数
 */
quint64 ImportMulFaceInfoDlg::sendOneItemInfoToTcpServer(request_item_head_t &request_item_head, QByteArray &fileData)
{

    int total = 0, rsize = 0;
    qDebug("(%s:%s:%d) send request_item_head...", __FILE__, __FUNCTION__, __LINE__);
    rsize = m_tcp_face_request->sendData((char *)&request_item_head,sizeof(request_item_head));
    total += rsize;
    rsize = 0;

    {

        qDebug("(%s:%s:%d) send image...", __FILE__, __FUNCTION__, __LINE__);
        int data_len = request_item_head.data_len_0;
        if(m_ops == INT_PIC_TYPE)
            rsize = m_tcp_face_request->sendData(fileData,data_len);
        else{
            /*特征值*/
        }

        total += rsize;
        if(request_item_head.audio_support)
        {
            qDebug("(%s:%s:%d) send audio...", __FILE__, __FUNCTION__, __LINE__);
            rsize = 0;
            rsize = m_tcp_face_request->sendData(fileData,request_item_head.audio_len);
            total += rsize;
        }

    }
    return total;
}
/**
 * @funcname  printfDataHead
 * @brief     打印信息头
 * @param     param1
 * @param     param2
 * @return    ret
 */
void ImportMulFaceInfoDlg::printfDataHead()
{
    qDebug("###############print request head info ################");
    qDebug("m_request_head->magic: = %s",m_request_head.magic);
    qDebug("m_request_head->ops: = %d",m_request_head.ops);
    qDebug("m_request_head->type = %d",m_request_head.type);
    qDebug("m_request_head->obj_num: = %d",m_request_head.obj_num);
    qDebug("###############end print request head info ############\n");
}

void ImportMulFaceInfoDlg::printfItemHead(request_item_head_t request_item_head)
{
    qDebug("@@@@@@@@@@@@@@print request item head info @@@@@@@@@@@@@@@");
    qDebug("request_item_head->magic: = %s",request_item_head.magic);
    qDebug("request_item_head->gid: = %d",request_item_head.gid);
    qDebug("request_item_head->id: = %d",request_item_head.id);
    qDebug("request_item_head->name = %s",request_item_head.name);
    qDebug("request_item_head->gender: = %d",request_item_head.gender);
    qDebug("request_item_head->age: = %d",request_item_head.age);
    qDebug("request_item_head->data_num: = %d",request_item_head.data_num);
    qDebug("request_item_head->data_len_0: = %d",request_item_head.data_len_0);
    qDebug("request_item_head->data_len_1: = %d",request_item_head.data_len_1);
    qDebug("request_item_head->data_len_2: = %d",request_item_head.data_len_2);
    qDebug("request_item_head->feat_len_0: = %d",request_item_head.feat_len_0);
    qDebug("request_item_head->feat_len_1: = %d",request_item_head.feat_len_1);
    qDebug("request_item_head->feat_len_2: = %d",request_item_head.feat_len_2);
    qDebug("request_item_head->audio_support: = %d",request_item_head.audio_support);
    qDebug("request_item_head->audio_len: = %d",request_item_head.audio_len);
    qDebug("@@@@@@@@@@@@@@end print request item head info @@@@@@@@@@@@\n");
}
/**
 * @funcname  getFeatureName
 * @brief     组合保存的文件名
 * @param     row 行号（从表格获取信息）
 * @param     exp           文件名扩展
 * @return    ret
 */
QString ImportMulFaceInfoDlg::getFeatureName(int row,QString exp)
{
    /*获取行信息*/
    QList<QString> strList;                 /* name,gender,age,gid,pid,picpath,featpath,adudio_support,audiopath*/
    strList = getDataFromRow(row);          /* 不包含序号*/
    if(strList.isEmpty())
        return "";
    QString strFileName;
    QString gid = strList.at(3);
    strFileName = QString("grp%1_").arg(gid);
    QString id = strList.at(4);
    strFileName += QString("pid%1_").arg(id);
    QString strName = strList.at(0);
    strFileName += QString("%1_").arg(strName);
    if(strList.at(1) == tr("男")){
        strFileName += "man_";
    }else{
        strFileName += "women_";
    }
    strFileName += QString("%1").arg(strList.at(2));   /*age*/
    strFileName += exp;
    return strFileName;
}

void ImportMulFaceInfoDlg::saveFile(QString fileName,const QByteArray baData)
{
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly)){
        qDebug() << "open file error";
        //return;
        file.close();
    }else{
        file.write(baData);
        file.flush();
        file.close();
    }
}
/**
 * @funcname  saveRecvFeatures
 * @brief     保存特征值信息
 * @param     strDirPath 保存路径
 * @param     param2
 * @return    ret
 */
void ImportMulFaceInfoDlg::saveRecvFeatures(QString strDirPath)
{
    quint8 objnum = m_tcp_face_request->m_reply_item_head_list.count();
  //  qDebug() <<"objnum:" << objnum;

    for(int i = 0; i < objnum; i++){
        /*获取item*/
       // reply_item_head_t reply_item_head;
       // memset(&reply_item_head, 0 ,sizeof(reply_item_head_t));
        //reply_item_head = m_tcp_face_request->m_reply_item_head_list.at(i);
        QString featureName = getFeatureName(i+1);
        QString strfilePath;
        strfilePath = strDirPath + QString("/%1").arg(featureName);
       // qDebug() << strfilePath;
        QByteArray baData;
        baData = m_faceDlg->m_files_baData.at(i);
        saveFile(strfilePath,baData);
        baData.clear();
    }
}
/**
 * @funcname  onOpsComboxIndexChanged
 * @brief     ops状态值
 * @param     text
 * @param     param2
 * @return    ret
 */
void ImportMulFaceInfoDlg::onOpsComboxTextChanged(QString text)
{
    if(text == TEXT_INLibrary)  /*入库*/
    {
        m_ops = 0;
    }
    else{
        m_ops = 1;
    }
    qDebug() << m_ops;
}
/**
 * @funcname  onTypeComboxTextChanged
 * @brief     type状态值
 * @param     param1
 * @param     param2
 * @return    ret
 */
void ImportMulFaceInfoDlg::onTypeComboxTextChanged(QString text)
{
    if(text == TEXT_PIC_TYPE)   /*图片*/
    {
        m_type = 0;
        setTableColsText(8,"--");/*第8行特征值*/

    }
    else{                       /*图片+feature*/
        m_type = 1;
    }
    /*刷新表格*/
    //setTableColsText()
}

void ImportMulFaceInfoDlg::onAudiocheckBoxStateChanged(int state)
{
    if (state == Qt::Checked) // "选中"
      {
            m_audio_support = 1;
            setTableColsText(9,"√");

      }
      else // 未选中 - Qt::Unchecked
      {
          m_audio_support = 0;
          setTableColsText(9,"--");
          setTableColsText(10,"--");
      }
}
/**
 * @funcname  onLoadFilesBtnClicked
 * @brief     加载文件，保存选中的文件名
 * @param     param1
 * @param     param2
 * @return    no
 */
void ImportMulFaceInfoDlg::onLoadFilesBtnClicked()
{
    //定义文件对话框类
   QFileDialog fileDialog;
   //定义文件对话框标题
   fileDialog.setWindowTitle(tr("加载文件"));
   //设置默认文件路径
   //fileDialog.setDirectory(".");
   //设置文件过滤器
   fileDialog.setNameFilter(tr("Images(*.png *.jpg *.jpeg *.bmp);; All Files(*.*)"));
   //设置可以选择多个文件,默认为只能选择一个文件QFileDialog::ExistingFiles
   fileDialog.setFileMode(QFileDialog::ExistingFiles);
   //设置视图模式
   fileDialog.setViewMode(QFileDialog::Detail);
   //打印所有选择的文件的路径
   QStringList fileNames;
   if(fileDialog.exec())
   {
       fileNames.clear();
       fileNames = fileDialog.selectedFiles(); /*选中的文件*/
       QFileInfo info(fileNames.at(0));
       QString suffix = info.suffix();
       if(fileNames.count() == 1 && suffix == "xml"){
            /*配置文件*/
           updateDisplayTableForAll(fileNames.at(0));
       }
       else{
           /*刷新表格*/
           removeAllTable();

           updateDisplayTableForPicPath(fileNames);
           ui->sendBtn->setEnabled(true);
       }
   }
   else{
       fileNames.clear();
   }

}
/**
 * @funcname  onSendBtnClicked
 * @brief     最后入库操作
 * @param     param1
 * @param     param2
 * @return    ret
 */
void ImportMulFaceInfoDlg::onSendBtnClicked()
{
    QStandardItemModel *model = m_myTableForImportMulPic->m_model;
    int count  = model->rowCount();
    int rsize = 0;
    if(count == 0)
        return;
    /*准备数据头*/
    prepareDataHead(count);
    /*建立连接*/
    QString strIp;
    getDevIPFromID(m_strDevId,strIp);
    newConnection(strIp,TCPClientPort);
    /*发送数据头*/
    qDebug("(%s:%s:%d) send request_head...", __FILE__, __FUNCTION__, __LINE__);
    rsize = sendDataHead(m_request_head);
    printfDataHead();
    if(rsize == REQUEST_HEAD_SIZE){
        qDebug("(%s:%s:%d) send request_head...ok", __FILE__, __FUNCTION__, __LINE__);
    }
    for(int i = 0; i < count; i++){
        /*循环发送*/
        /*准备信息头*/
        request_item_head_t request_item_head;
        QByteArray baData;
        memset(&request_item_head,0,sizeof(request_item_head_t));
        prepareItemHead(i+1,request_item_head,baData);
        qDebug("(%s:%s:%d) send request obj%d", __FILE__, __FUNCTION__, __LINE__, i+1);
        sendOneItemInfoToTcpServer(request_item_head,baData);
        qDebug("(%s:%s:%d) send request obj%d...ok", __FILE__, __FUNCTION__, __LINE__, i+1);
        baData.clear();
    }
    //ui->saveFeatureBtn->setEnabled(true);
}
/**
 * @funcname  onSaveFeatureBtnClicked
 * @brief     保存特征值信息
 * @param     param1
 * @param     param2
 * @return    ret
 */
void ImportMulFaceInfoDlg::onSaveFeatureBtnClicked()
{
    QString strFilePath = QFileDialog::getExistingDirectory(this,tr("请选择保存的路径"),"./");
    if(strFilePath.isEmpty())
        return;
    else{
        qDebug() << strFilePath;
        saveRecvFeatures(strFilePath);
    }

}
