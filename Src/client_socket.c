/****************************************Copyright (c)**************************************************
**                               江苏XX电子技术有限公司
**                                    产品研发部
**                                        
**                            
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: client_socket.c
**创   建   人: KK
**最后修改日期: 2017年05月02日
**描        述: 网络客户端应用程序
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "string.h"
#include <stdio.h>
#include "client_socket.h"
#include "cmsis_os.h"

#include "lwip/netif.h"
#include "lwip/tcpip.h"
#include "ethernetif.h"
#include "app_cpx.h"
#include "tlv.h"

#include "CPX_BOARD.h"
 
extern __IO  unsigned short current_SPEED; //记录当前速度
extern  uint32_t HaveRunTime;
extern  uint16_t SOP2H_VALUE ;
extern  uint16_t Chufang_REC_flag;
extern  osMailQId Server_mailId;/*来自服务器的处方邮箱地址*/
#define WEBSERVER_THREAD_PRIO     ( tskIDLE_PRIORITY + 4)
#define READ_THREAD_PRIO          ( tskIDLE_PRIORITY + 5)
#define UNPACK_THREAD_PRIO        ( tskIDLE_PRIORITY + 6)

SERVER_msg server_msg; /*跑步机处方命令*/
osMailQId DQ_mailId;/*DQ邮箱地址*/
struct netif gnetif; /* network interface structure */
static void Netif_Config(void);
static void tlv_unpack_thread(void *arg); /*解包线程*/
 
/**
  * 发送注册消息给服务器，在连接到服务器之后发送
  */
static void send_dev_register_message(int conn) ;
/**
  * 发送心跳消息给服务器，在连接到服务器之后，每秒钟发送一次
  */
static void send_dev_heartbeat_message(int conn,u16 status);
/**
  * 发送跑步机消息给服务器，在启动设备之后，每2秒钟发送一次
  */
static void send_dev_data_message(int conn);

 
#define QUEUE_SIZE        (uint32_t) 100
osMessageQId osQueue;/*TLV数据包接收队列*/
 
/*用户ID*/
__IO struct user_id
{
  unsigned short type;
  unsigned short length;
  unsigned int    id;
}User_ID;

/*p跑步机显示参数*/
struct run_info
{
  unsigned short type;
  unsigned short length;
  unsigned short time;
  unsigned short distance;
  unsigned short calorie;
  unsigned short speeed;
  unsigned short INCL;
  unsigned short sportmode;
}Run_info;


/*跑步机停止命令*/
struct stop_cmd
{
  unsigned short type;
  unsigned short length;
  unsigned short cmd;
}Stop_Dev;

/*跑步机速度控制命令*/
struct speedCtrl_cmd
{
  unsigned short type;
  unsigned short length;
  unsigned short cmd;
}SpeedCtrl_Dev;

/*跑步机坡度控制命令*/
struct INCL_cmd
{
  unsigned short type;
  unsigned short length;
  unsigned short cmd;
}INCL_Dev;


/**
  * 接收数据线程，并解包
  */
static void client_read_thread(void *arg)
{
  int *sock = arg,len,i;
  
  unsigned char buff[8];
 
  osMessageQDef(osqueue, QUEUE_SIZE, uint8_t);
  osQueue = osMessageCreate(osMessageQ(osqueue), NULL);
  
  
  sys_thread_new("UNPACK", tlv_unpack_thread, NULL, DEFAULT_THREAD_STACKSIZE * 2, UNPACK_THREAD_PRIO);
  
 
  /* Read in the request */
  while (1) 
  {
    len = read(*sock, buff, 8);
    if(len < 0)return;
    
    for(i=0;i<len;i++)
    { 
      if(osMessagePut(osQueue, (uint32_t)buff[i], osWaitForever) != osOK) 
      {
        printf("消息入队错误\n");
      }
    }
  }    
}


/**
  * TLV解包线程
  */
static void tlv_unpack_thread(void *arg)
{
  int index = 0;
  
  osEvent event;
  
  head pkg_head;
  
  SERVER_msg  *pSMail;
 
  unsigned char *p,*buff,temp[8],rev_tlv_flag = 0;
  
	while(1)
	{	
    event = osMessageGet(osQueue, osWaitForever);
    if(event.status == osEventMessage)
    {
      
      if(rev_tlv_flag == 0)/*接收帧头域(目前为8字节)*/
      {
        if(index < 2)/*读取前2字节进行比较*/
        {
          temp[0] = temp[1];
          temp[1] = event.value.v;
          if(temp[1] == TLV_VERSION && temp[0] <= (CODE_MAX_NUM) && temp[0] != 0) 
          {
            index = 2;/*从length开始接收*/  
          }
        }
        else
        {
          temp[index++] = event.value.v;
        }
        
        if(index == sizeof(head))/*接收到包头的字节数*/
        {
          memcpy(&pkg_head,temp,sizeof(head));/*帧头包数据拷贝*/
          pkg_head.length = htons(pkg_head.length);
          pkg_head.crc = htons(pkg_head.crc);
          buff = (unsigned char *)malloc(pkg_head.length - sizeof(head));/*申请缓存,存储TLV数据*/
          p = buff; /*赋值首地址*/
          index = 0;
          rev_tlv_flag = 1;
        }
      }
      else if(rev_tlv_flag == 1)/*接收DI数据域,即TLV*/
      {
        buff[index++] = event.value.v; 
        if(index == pkg_head.length - sizeof(head))/*读取指定长度*/
        { 
          switch(pkg_head.code)
          {
            case CODE_CONTROL_CMD:/*与协议冲突,暂时未改*/
              
                do{ /*截取信息*/
                  switch(p[0]<<8|p[1])
                  {
                     case TYPE_USER_ID:/*用户ID*/
                        memcpy(&User_ID,p,p[2]<<8|p[3]);
                        printf("用户ID=%x\n",htonl(User_ID.id));
                       break;
                     case TYPE_PRESCRIPTION_PRM:/*处方*/
                        memcpy(&server_msg,p,p[2]<<8|p[3]);
                        pSMail = osMailAlloc(Server_mailId, osWaitForever); /* Allocate memory */
                        pSMail->CTRL_msg.speed_doctor = htons(server_msg.CTRL_msg.speed_doctor);
                        pSMail->CTRL_msg.slope_doctor = htons(server_msg.CTRL_msg.slope_doctor); 
                        pSMail->CTRL_msg.time = htons(server_msg.CTRL_msg.time);
                        Run_info.speeed  = htons(server_msg.CTRL_msg .speed_doctor);
                        Run_info.INCL = htons(server_msg.CTRL_msg.slope_doctor);    
                        Run_info.sportmode = htons(server_msg.CTRL_msg.sportmode); 
                        ClrAllSysFlag();//清除SYS标志     
                        HaveRunTime=0;
                        Chufang_REC_flag=0;                     
                        printf("运动时间:%d,速度:%d,速度上限:%d,速度下限:%d,医嘱坡度:%d,坡度上限:%d,坡度下限:%d 运动模式:%d \n",
                            htons(server_msg.CTRL_msg.time),htons(server_msg.CTRL_msg.speed_doctor),htons(server_msg.CTRL_msg.speed_max),htons(server_msg.CTRL_msg.speed_min),\
                            htons(server_msg.CTRL_msg.slope_doctor),htons(server_msg.CTRL_msg.slope_max),htons(server_msg.CTRL_msg.slope_min),htons(server_msg.CTRL_msg.sportmode));
                
                       break;
                     case TYPE_DEVICE_STOP:/*停止设备*/
                        memcpy(&Stop_Dev,p,p[2]<<8|p[3]);
                        pSMail = osMailAlloc(Server_mailId, osWaitForever); /* Allocate memory */
                        pSMail->STOP_msg  = 1;      
                        printf("停止设备8 \n");
                       break;
                     case TYPE_TREADMILL_SPEED:/*速度控制*/
                        memcpy(&SpeedCtrl_Dev,p,p[2]<<8|p[3]);

                        pSMail = osMailAlloc(Server_mailId, osWaitForever); /* Allocate memory */
                        pSMail->SPEED_msg = htons(SpeedCtrl_Dev.cmd);
                        printf("速度控制speeed msg=%x \n",pSMail->SPEED_msg );
                     case TYPE_TREADMILL_SLOPE:/*坡度控制*/
                        memcpy(&INCL_Dev,p,p[2]<<8|p[3]);
                        pSMail = osMailAlloc(Server_mailId, osWaitForever); /* Allocate memory */
                        pSMail->INCL_msg = htons(INCL_Dev.cmd);
                  
                       printf("坡度控制INCL msg=%x \n",pSMail->INCL_msg );
                       break;      
                  }
                  p += p[2]<<8|p[3];/*地址偏移*/
                }while(p < (buff + pkg_head.length - sizeof(head)));
 
                if(osMailPut(Server_mailId, pSMail) != osOK) /* Send Mail */  
                {      
                   printf("发送处方邮箱 不成功\n");
                }else{
                   printf("发送处方邮箱 成功\n");
                }
              break;
              
             case CODE_DEVICE_REGISTER:/*注册响应消息*/
               do{
                  switch(p[0]<<8|p[1])
                  {
                    case TYPE_ERROR_CODE:/*错误码*/
                          printf("收到注册响应消息\n");
                      break;   
                  }
                  p += p[2]<<8|p[3];/*增加偏移量*/
                }while(p < (buff + pkg_head.length - sizeof(head)));
                printf("收到注册响应消息2\n");
               break;
          }
          free(buff);
          index = 0;
          rev_tlv_flag = 0;    
        }  
      }     
    }   
    
	}
}


/**
  * 发送注册消息给服务器，在连接到服务器之后发送
  */
static void send_dev_register_message(int conn) 
{
  package *pkg;
  tlv dev_id,dev_type;
  u16 mac[3] = {htons(0x1122),htons(0x3344),htons(0x5566)},type = htons(1);
  /*设备ID*/
  dev_id.t = htons(TYPE_DEVICE_ID);
  dev_id.l = 10;
  dev_id.v = mac;
  /*设备类型*/
  dev_type.t = htons(TYPE_DEVICE_TYPE);
  dev_type.l = 6;
  dev_type.v = &type;
  
  pkg = package_create(CODE_DEVICE_REGISTER,TLV_VERSION,0,1,2,dev_id,dev_type);
  
  write(conn, (unsigned char *)pkg->pbuf, pkg->length);
  
  free(pkg->pbuf);
}

/**
  * 发送心跳消息给服务器，在连接到服务器之后，每秒钟发送一次
  */
static void send_dev_heartbeat_message(int conn,u16 status) 
{
  package *pkg;
  tlv dev_id,dev_type;
  u16 mac[3] = {htons(0x1122),htons(0x3344),htons(0x5566)},stas;
  /*设备ID*/
  dev_id.t = htons(TYPE_DEVICE_ID);
  dev_id.l = 10;
  dev_id.v = mac;
  /*设备状态*/
  dev_type.t = htons(TYPE_TREADMILL_STATUS);
  dev_type.l = 6;
  stas = htons(status);
  dev_type.v = &stas;
  
  pkg = package_create(CODE_HEARTBEAT_MESSAGE,TLV_VERSION,0,1,2,dev_type,dev_id);
  
  write(conn, (unsigned char *)pkg->pbuf, pkg->length);
  
  free(pkg->pbuf);
}
 
/**
  * 发送跑步机消息给服务器，在启动设备之后，每2秒钟发送一次
  */
static void send_dev_data_message(int conn) 
{
  package *pkg;
  tlv spo2_data,spo2_value,show_data,doctor_status,dev_id,user_id;
  u16 mac[3],status,status2,runShow[5];
  u32 u_id;
  /*血氧波形*/
  spo2_data.t = htons(TYPE_SPO2_DATA);
  spo2_data.l = 204;
  status = htons(0x1122);
  spo2_data.v = &status;
   
  /*血氧值*/
  spo2_value.t = htons( TYPE_SPO2_VALUE);
  spo2_value.l = 6;
//  printf ("SOP2H_VALUE2=%d\r\n",SOP2H_VALUE);
  status2 = htons(SOP2H_VALUE);
  spo2_value.v = &status2;

  /*跑步机显示值*/
  show_data.t = htons(TYPE_DEVICE_SHOW_DATA);
  show_data.l = 14;
    
  runShow[0] = htons(HaveRunTime);
  runShow[1] = htons(Run_info.speeed*HaveRunTime/3.6);
  runShow[2] = 0;
  runShow[3] = htons(current_SPEED);
  runShow[4] = htons(Run_info.INCL);
  show_data.v = runShow;
//  printf("runShow= %x+%x+%x+%x+%x\n", runShow[0] , runShow[1] , runShow[2] , runShow[3] , runShow[4] );
  /*处方状态*/
  doctor_status.t = htons(TYPE_PRESCRIPTION_STATUS);
  doctor_status.l = 6;
  status = Chufang_REC_flag;        
  doctor_status.v = &status;
  
  /*设备ID*/
  dev_id.t = htons(TYPE_DEVICE_ID);
  dev_id.l = 10;
  mac[0] = htons(0x1122);
  mac[1] = htons(0x3344);
  mac[2] = htons(0x5566);
  dev_id.v = mac;
  
  /*用户ID*/
  user_id.t = htons(TYPE_USER_ID);
  user_id.l = 8;
  u_id = User_ID.id;
  user_id.v = &u_id;

  if(GetSysFlag(Cpx_StopFlag))
  {
    Chufang_REC_flag=0x0100;        
    doctor_status.v = &Chufang_REC_flag;
    pkg = package_create(CODE_TREADMILL_DATA,TLV_VERSION,0,1,3,doctor_status,dev_id,user_id);
    write(conn, (unsigned char *)pkg->pbuf, pkg->length);
    free(pkg->pbuf);
    ClrSysFlag(Cpx_StopFlag);
    Chufang_REC_flag=0;//一个处方完成清零准备记录下一次处方状态 
  }
  else
  {
    if(GetSysFlag(Cpx_Ready2Flag))
    {
      pkg = package_create(CODE_TREADMILL_DATA,TLV_VERSION,0,1,4,spo2_value,show_data,dev_id,user_id);
      write(conn, (unsigned char *)pkg->pbuf, pkg->length);
      free(pkg->pbuf);
    }
  }
}

/**
  * @brief  http server thread 
  * @param arg: pointer on argument(not used here) 
  * @retval None
  */
static void client_socket_thread(void *arg)
{
  int *sock = arg,i=0;
  osEvent event;
  
  osDelay(3000);
  send_dev_register_message(*sock); /*发送注册消息*/
  while (1) 
  { 
    if(JerkStop_Read()==RESET) JerkStop_OFF();
    else  JerkStop_ON();
    send_dev_heartbeat_message(*sock,TREADMILL_STATUS_RUN); /*发送心跳消息,每秒一次*/
    HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
    event = osMailGet(DQ_mailId, 10); /* wait for mail */
    if(++i%2==0)
    {
      send_dev_data_message(*sock); /*发送跑步机消息给服务器，在启动设备之后，每2秒钟发送一次*/
    }      
    osDelay(1000);
  }
}
 
/**
  * @brief  Initialize the client socket (start its thread) 
  * @param  none
  * @retval None
  */
void client_socket_init()
{
  static int sock;
  struct sockaddr_in address;
  /* Create tcp_ip stack thread */
  tcpip_init(NULL, NULL);
  
  /* Initialize the LwIP stack */
  Netif_Config(); 
  
 /* 创建套接字 */
  if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) 
  {
    return;
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(SERVER_IP);/*IP地址*/
  address.sin_port = htons(SERVER_PORT);/*端口号*/
  if(connect(sock,(struct sockaddr*)&address,sizeof(address)) < 0)/*连接服务器*/ 
  {
    close(sock);
  }
  else
  {
    sys_thread_new("CLIENT", client_socket_thread, &sock, DEFAULT_THREAD_STACKSIZE, WEBSERVER_THREAD_PRIO);
    sys_thread_new("READ", client_read_thread, &sock, DEFAULT_THREAD_STACKSIZE, READ_THREAD_PRIO);
//    sys_thread_new("UNPACK", tlv_unpack_thread, NULL, DEFAULT_THREAD_STACKSIZE, UNPACK_THREAD_PRIO);
  }
}


/**
  * @brief  Initializes the lwIP stack
  * @param  None
  * @retval None
  */
static void Netif_Config(void)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;	
  
  /* IP address default setting */
  IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
  IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
  IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
  
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);
  
  /*  Registers the default network interface. */
  netif_set_default(&gnetif);
  
  if (netif_is_link_up(&gnetif))
  {
    /* When the netif is fully configured this function must be called.*/
    netif_set_up(&gnetif);
  }
  else
  {
    /* When the netif link is down this function must be called */
    netif_set_down(&gnetif);
  }

  /* Set the link callback function, this function is called on change of link status*/
  netif_set_link_callback(&gnetif, ethernetif_update_config);
  
  /* Create the Ethernet link handler thread */
  osThreadDef(LinkThr, ethernetif_set_link, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
  osThreadCreate (osThread(LinkThr), &gnetif);
}

/********************************** (C) COPYRIGHT 2017 XX *****END OF FILE***********************/
