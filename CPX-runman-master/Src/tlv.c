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
#include "tlv.h"
#include <stdarg.h>
#include "lwip/api.h"


/*������Ϣ������ʼ���̶���*/
package* package_create(unsigned char code,unsigned char version,unsigned char identifier,unsigned char protocol,unsigned char num_tlv,...)
{
  package *pkg;
  
  unsigned char *buff,tmp;
  u16 len = 8;
  u32 index,i;
  tlv arg;
  va_list arg_ptr; 
  
  head *cur;
  
  va_start(arg_ptr,num_tlv); 
  for(i=0;i<num_tlv;i++)
  { 
    arg = va_arg(arg_ptr,tlv);
    len += arg.l;
  }   
  va_end(arg_ptr); 

  buff = (unsigned char*)malloc(len);
  if(!buff) {
      return NULL;
  } 
  memset(buff, 0x00, len);
  pkg->length = len;
  pkg->pbuf = buff;
  
  cur = (head *)buff;
  cur->code = code;
  cur->version = version;
  cur->length = htons(len);/*�ֽ���ת��*/
  cur->crc = htons(0x0);
  cur->identifier = identifier;
  cur->protocol = protocol;
  
  va_start(arg_ptr,num_tlv); 
  for(i=0,index=0;i<num_tlv;i++)
  { 
    arg = va_arg(arg_ptr,tlv);
   
    memcpy(buff+8+index,&arg,4);
    memcpy(buff+12+index,arg.v,arg.l-4);
    /*���ݳ����ֽ���ת��*/
    tmp = buff[10+index];
    buff[10+index] = buff[11+index];
    buff[11+index] = tmp;
    
    index += arg.l;
  }   
  va_end(arg_ptr);
  
  return pkg;
}









 




/********************************** (C) COPYRIGHT 2017 XX *****END OF FILE***********************/
