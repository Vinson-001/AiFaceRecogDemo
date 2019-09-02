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
#define DEV_ID_CMD_CODE             "1101"
#define DEV_NAME_CMD_CODE           "1102"
#define DEV_IP_CMD_CODE             "1104"
#define FTP_IP_CMD_CODE             "1105"
#define FTP_CAP_PATH_CMD_CODE       "1106"
#define FTP_RECOG_PATH_CMD_CODE     "1107"
#define RTC_EVENT_PORT_CMD_CODE     "1108"
#define WARN_PORT_CMD_CODE          "1109"

#define INT_DEV_ID_CMD_CODE             1101
#define INT_DEV_NAME_CMD_CODE           1102
#define INT_DEV_IP_CMD_CODE             1104
#define INT_FTP_IP_CMD_CODE             1105        /*FTP server IP*/
#define INT_FTP_CAP_PATH_CMD_CODE       1106
#define INT_FTP_RECOG_PATH_CMD_CODE     1107
#define INT_RTC_EVENT_PORT_CMD_CODE     1108
#define INT_WARN_PORT_CMD_CODE          1109

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
