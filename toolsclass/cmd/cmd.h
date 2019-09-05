#ifndef CMD_H
#define CMD_H

/*
 * 命令格式：http://<IP addr>/?custom=1&cmd=CMD_CODE&par=<param>
 */

#define HTTP_HEAD                       "http://"
#define CUSTOM                          "/?custom=1"
#define _CMDCODE(x)                     "&cmd="#x                   /*连接字符串*/
#define CMDCODE(x)                      _CMDCODE(x)
#define _PARAM(x)                        "&par="#x
#define PARAM(x)                        _PARAM(x)


/*设备添加命令码*/
#define DEV_ID_CMD_CODE                 "1101"
#define DEV_NAME_CMD_CODE               "1102"
#define DEV_PASSWARD_CMD_CODE           "1103"
#define DEV_IP_CMD_CODE                 "1104"

/*ftp ip*/
#define QUERY_FTP_IP_CMD_CODE           "1201"
#define FTP_IP_CMD_CODE                 "1202"

/*ftp cap*/
#define QUERY_FTP_CAP_PATH_CMD_CODE     "1205"
#define FTP_CAP_PATH_CMD_CODE           "1206"

/*ftp recog*/
#define QUERY_FTP_RECOG_PATH_CMD_CODE   "1207"
#define FTP_RECOG_PATH_CMD_CODE         "1208"

/*rtc real event */
#define QUERY_RTC_EVENT_PORT_CMD_CODE   "1301"
#define RTC_EVENT_PORT_CMD_CODE         "1302"

/*AlarmEvent port */
#define QUERY_AlarmEvent_PORT_CMD_CODE  "1401"
#define AlarmEvent_PORT_CMD_CODE        "1402"

#define INT_DEV_ID_CMD_CODE             1101
#define INT_DEV_NAME_CMD_CODE           1102
#define INT_DEV_IP_CMD_CODE             1104

#define INT_FTP_IP_CMD_CODE             1202        /*FTP server IP*/
#define INT_FTP_CAP_PATH_CMD_CODE       1206
#define INT_FTP_RECOG_PATH_CMD_CODE     1208
#define INT_RTC_EVENT_PORT_CMD_CODE     1302
#define INT_WARN_PORT_CMD_CODE          1402

#define INT_START_CMD_CODE              1101


/*以下是设备管理*/

/* 分组管理 */

#define STR_CREATE_GROUP                "2001"
#define STR_SERACH_GROUP                "2002"
#define STR_DELETE_GROUP                "2003"

#define INT_CREATE_GROUP                2001
#define INT_SERACH_GROUP                2002
#define INT_DELETE_GROUP                2003

/* 数据库管理*/

#define STR_DELETE_DATABASE                "2103"

#define INT_DELETE_DATABASE                2103


/* 区分返回信息 用于xml解析*/
#define RETURN_GROUP_INFO "group_info"
#define RETURN_HUAMN_INFO "human_info"

#define RETURN_SUCESS "0"

/* 人脸查询 */

#define STR_FACE_QUERY      "2101"


#endif // CMD_H
