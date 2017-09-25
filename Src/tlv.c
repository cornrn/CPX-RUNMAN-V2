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
#include "tlv.h"
#include <stdarg.h>
#include "lwip/api.h"


/*创建消息包，初始化固定域*/
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
  cur->length = htons(len);/*字节序转换*/
  cur->crc = htons(0x0);
  cur->identifier = identifier;
  cur->protocol = protocol;
  
  va_start(arg_ptr,num_tlv); 
  for(i=0,index=0;i<num_tlv;i++)
  { 
    arg = va_arg(arg_ptr,tlv);
   
    memcpy(buff+8+index,&arg,4);
    memcpy(buff+12+index,arg.v,arg.l-4);
    /*数据长度字节序转换*/
    tmp = buff[10+index];
    buff[10+index] = buff[11+index];
    buff[11+index] = tmp;
    
    index += arg.l;
  }   
  va_end(arg_ptr);
  
  return pkg;
}









 




/********************************** (C) COPYRIGHT 2017 XX *****END OF FILE***********************/
