/****************************************Copyright (c)**************************************************
**                               ����XX���Ӽ������޹�˾
**                                    ��Ʒ�з���
**                                        
**                              http://www.XX.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: tlv.c
**��   ��   ��: KK
**�� ��  �� ��: 2017��05��05��
**��        ��: TLVЭ���м��
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
    unsigned short t;/*��������*/
    unsigned short l;/*���ݳ���*/
    void          *v;  /*����ָ��*/
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

#define CODE_HARDWARE_SYSTEM_INFO  0x01   /*Ӳ��ϵͳ��Ϣ*/
#define CODE_ECG_DATA               0x02   /*�ĵ�����*/
#define CODE_DEVICE_REGISTER       0x03   /*�豸ע��*/
#define CODE_HEARTBEAT_MESSAGE     0x04   /*������Ϣ*/
#define CODE_CONTROL_CMD            0x05  /*��������*/
#define CODE_TREADMILL_DATA         0x06  /*�ܲ�������*/

#define CODE_MAX_NUM 0x06           /*CODE���������*/


#define TYPE_ECG_CH0  0x0010    /*�ĵ�CH0����*/
#define TYPE_ECG_CH1  0x1010    /*�ĵ�CH1����*/
#define TYPE_ECG_CH2  0x2010    /*�ĵ�CH2����*/

#define TYPE_ECG_MARK   0x0011  /*�ĵ�������*/

#define TYPE_ACC        0x0020  /*���ٶ�����*/
#define TYPE_GRY        0x0021  /*����������*/
#define TYPE_MAG        0x0022  /*ָ��������*/

#define TYPE_ELECTRICITY  0x0030  /*��������*/

#define TYPE_PRESCRIPTION_PRM  0x5010  /*��������*/
#define TYPE_DEVICE_ID    0x5001      /*�豸ID*/
#define TYPE_DEVICE_TYPE  0x5002      /*�豸����*/
#define TYPE_PRESCRIPTION_STATUS    0x5003  /*�������״̬*/
#define TYPE_DEVICE_STOP  0x5004    /*�豸ֹͣ����*/
#define TYPE_USER_ID    0x5005    /*�û�ID*/
#define TYPE_DEVICE_SHOW_DATA  0x5020  /*�ܲ�����ʾֵ*/
#define TYPE_TREADMILL_STATUS    0x5021  /*�ܲ���״̬*/

#define TYPE_TREADMILL_SPEED    0x5012  /*�ܲ���״̬*/
#define TYPE_TREADMILL_SLOPE    0x5013  /*�ܲ���״̬*/

#define TYPE_SPO2_DATA  0x0012  /*Ѫ����������*/
#define TYPE_SPO2_VALUE  0x0013 /*Ѫ��ֵ*/

#define TYPE_ERROR_CODE 0xF000 /*������*/

#define TREADMILL_STATUS_RUN  (u16)0x0001
#define TREADMILL_STATUS_FREE  (u16)0x0002
#define TREADMILL_STATUS_FAULT  (u16)0x0003

package* package_create(u8 code,u8 version,u8 identifier,u8 protocol,u8 num_tlv,...);



/********************************** (C) COPYRIGHT 2017 XX *****END OF FILE***********************/
