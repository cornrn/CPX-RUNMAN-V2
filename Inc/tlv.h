/****************************************Copyright (c)**************************************************
**                               江苏XX电子技术有限公司
**                                    产品研发部
**                                        
**                              http://www.XX.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: tlv.c
**创   建   人: KK
**新 建  日 期: 2017年05月05日
**描        述: TLV协议中间件
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/

 
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
 
 

#define VALUE_WIDTH_08  0
#define VALUE_WIDTH_16  1
#define VALUE_WIDTH_32  2


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;


typedef struct _tlv
{
    unsigned short t;/*数据类型*/
    unsigned short l;/*数据长度*/
    void          *v;  /*数据指针*/
}tlv; 

typedef struct _head
{
  unsigned char code;
  unsigned char version;
  unsigned short length;
  unsigned short crc;
  unsigned char identifier;
  unsigned char protocol;     
}head;



typedef struct _package
{
  unsigned short length;
  unsigned char *pbuf; 
}package;

 

#define TLV_VERSION   0x00

#define CODE_HARDWARE_SYSTEM_INFO  0x01   /*硬件系统信息*/
#define CODE_ECG_DATA               0x02   /*心电数据*/
#define CODE_DEVICE_REGISTER       0x03   /*设备注册*/
#define CODE_HEARTBEAT_MESSAGE     0x04   /*心跳消息*/
#define CODE_CONTROL_CMD            0x05  /*控制命令*/
#define CODE_TREADMILL_DATA         0x06  /*跑步机数据*/

#define CODE_MAX_NUM 0x06           /*CODE域的最大编码*/


#define TYPE_ECG_CH0  0x0010    /*心电CH0数据*/
#define TYPE_ECG_CH1  0x1010    /*心电CH1数据*/
#define TYPE_ECG_CH2  0x2010    /*心电CH2数据*/

#define TYPE_ECG_MARK   0x0011  /*心电打标数据*/

#define TYPE_ACC        0x0020  /*加速度数据*/
#define TYPE_GRY        0x0021  /*陀螺仪数据*/
#define TYPE_MAG        0x0022  /*指南针数据*/

#define TYPE_ELECTRICITY  0x0030  /*电量数据*/

#define TYPE_PRESCRIPTION_PRM  0x5010  /*处方参数*/
#define TYPE_DEVICE_ID    0x5001      /*设备ID*/
#define TYPE_DEVICE_TYPE  0x5002      /*设备类型*/
#define TYPE_PRESCRIPTION_STATUS    0x5003  /*处方完成状态*/
#define TYPE_DEVICE_STOP  0x5004    /*设备停止命令*/
#define TYPE_USER_ID    0x5005    /*用户ID*/
#define TYPE_DEVICE_SHOW_DATA  0x5020  /*跑步机显示值*/
#define TYPE_TREADMILL_STATUS    0x5021  /*跑步机状态*/

#define TYPE_TREADMILL_SPEED    0x5012  /*跑步机状态*/
#define TYPE_TREADMILL_SLOPE    0x5013  /*跑步机状态*/

#define TYPE_SPO2_DATA  0x0012  /*血氧波形数据*/
#define TYPE_SPO2_VALUE  0x0013 /*血氧值*/

#define TYPE_ERROR_CODE 0xF000 /*错误码*/

#define TREADMILL_STATUS_RUN  (u16)0x0001
#define TREADMILL_STATUS_FREE  (u16)0x0002
#define TREADMILL_STATUS_FAULT  (u16)0x0003

package* package_create(u8 code,u8 version,u8 identifier,u8 protocol,u8 num_tlv,...);



/********************************** (C) COPYRIGHT 2017 XX *****END OF FILE***********************/
