/****************************************Copyright (c)**************************************************
**                               江苏XX电子技术有限公司
**                                    产品研发部
**                                        
**                              http://www.XX.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: app_cpx.h
**创   建   人: KK
**创  建 日 期: 2017年05月04日
**描        述: 跑步机应用程序
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#include "cmsis_os.h"
#include "stm32f7xx_hal.h"

#define 	CPX_QUEUE_SIZE		100	



extern osMessageQId osQueue_CPX;
/** 
  * @brief  Treadmill base State definition  
  */ 
typedef enum
{
   Plus=0u,                                  
   Minus= !Plus                                   
}TREADMILL_BaseState;

/** 
  * @brief  Treadmill base Type definition  
  */ 
typedef enum
{
   Speed=1u,                                  
   Slope,
   Dir  
}TREADMILL_BaseType;

////////////////////////////////////////////////
typedef struct TLVHeadStrt//设备注册消息
{
  unsigned short	     CodeVers;
	unsigned short	     Length;
  unsigned short	     Crc;
  unsigned short	     IdentPrtc;
}TLVHead;


/** 
  * @brief   EventGroup Bits definition  
  */ 
typedef enum 
{
	ErrFlag					          = 0,
  Cpx_StartFlag 	          = (1<<1),//CPX开始标志				
	Cpx_StopFlag 				    = (1<<2),//CPX停止标志
  Cpx_Ready2Flag 	        = (1<<3),//跑步机按开始后初始化到0.6KM/H进去二级准备开始标志
	SPO2H_DqEnFlag 	        = (1<<4),//血氧数据同步使能标志	
	SPO2H_RecEnFlag 		      = (1<<5),//血氧数据采集使能标志
  Err_SPO2HFlag			      = (1<<6),//血氧端口脱落标志	
	Rp_StartFlag		          = (1<<7),//处方开始标志
	Rp_StopFlag	            = (1<<8),//处方结束标志
	Err_HeartTickFlag	      = (1<<9),//心跳错误标志（未收到上位机心跳消息置位）
	EmergStopFlag				    = (1<<10),//急停标志
  Rp_TimerStartFlag        = (1<<11),//处方开始计时标志
  Rp_SteadyStateFlag       = (1<<12),//处方进行到稳态的标志
  Rp_SteadyOverFlag        = (1<<13),//处方进行到稳态结束的标志
  Rp_OverFlag              = (1<<14),//处方结束的标志
}AllSysFlag;
 
/********************************************************************************************************
** 变量名称: SystemFlag
** 功能描述: 系统标志
********************************************************************************************************/
extern unsigned int SysFlag; 

/*******************************************************************************************************
** 功能描述: 系统标志操作宏定义
********************************************************************************************************/
#define GetSysFlag(flag)        SysFlag & (flag)
#define SetSysFlag(flag)        SysFlag |= (flag)
#define ClrSysFlag(flag)        SysFlag &= ~(flag)
#define ClrAllSysFlag()         SysFlag= 0x00



/*******************************************************************************************************
** 结构名称: 数据包
** 功能描述:  
********************************************************************************************************/
typedef struct
{
	unsigned short       Type;
	unsigned short	     Length;
	unsigned int	       Value;		
}CLET_ID_strc;	//用户ID

typedef struct
{
	unsigned short       Type;
	unsigned short	     Length;
	unsigned short	     Value[3];		
}DEV_MAC_strc;	

typedef struct
{
	unsigned short       Type;
	unsigned short	     Length;
	unsigned short	     Value;		
}DEV_TYPE_strc;
 
////////////////////////////////////////////////
typedef struct
{
	unsigned short       Type;
	unsigned short	     Length;
	unsigned short	     Value;		
}HeaSP_Runstate_strc;	
////////////////////////////////////////////////cmd
typedef struct
{
	unsigned short       Type;
	unsigned short	     Length;
	unsigned short	     Value[8];		
}CTL_DrugCTL_strc;

typedef struct
{
	unsigned short       Type;
	unsigned short	     Length;
	unsigned short	     Value;		
}CTL_Spd_strc;	

typedef struct
{
	unsigned short       Type;
	unsigned short	     Length;
	unsigned short	     Value;		
}CTL_Slo_strc;	

typedef struct
{
	unsigned short       Type;
	unsigned short	     Length;
	unsigned short	     Value;		
}CTL_Stop_strc;	

	
////////////////////////////////////////////////
typedef struct
{
	unsigned short       Type;
	unsigned short	     Length;
	unsigned short	     Value[32];		
}Run_SPO2WAV_strc;	
typedef struct
{
	unsigned short       Type;
	unsigned short	     Length;
	unsigned short	     Value;		
}Run_SPO2_strc;	

typedef struct
{
	unsigned short       Type;
	unsigned short	     Length;
	unsigned short	     Value[5];		
}Run_MACN_strc;	

typedef struct
{
	unsigned short       Type;
	unsigned short	     Length;
	unsigned short	     Value;		
}Run_Drug_strc;	//处方完成状态
////////////////////////////////////////////////



////////////////////////////////////////////////
typedef struct MacnPackStrt//设备注册消息
{
  unsigned short	     CodeVers;
	unsigned short	     Length;
  unsigned short	     Crc;
  unsigned short	     IdentPrtc;
	DEV_MAC_strc         fDEV_MAC_strc;
	DEV_TYPE_strc        fDEV_TYPE_strc;
}MacnPacket;
////////////////////////////////////////////////
typedef struct HeaSPPackStrt//心跳消息
{
  unsigned short	     CodeVers;
	unsigned short	     Length;
  unsigned short	     Crc;
  unsigned short	     IdentPrtc;
	HeaSP_Runstate_strc  fHeasp_strc;
	DEV_MAC_strc				 fDEV_MAC_strc;
}HeaSPPacket;
////////////////////////////////////////////////
typedef struct CTLPackStrt//CMD MSG
{
  unsigned short	     CodeVers;
	unsigned short	     Length;
  unsigned short	     Crc;
  unsigned short	     IdentPrtc;
	CTL_DrugCTL_strc     fCTL_DrugCTL_strc;	
	CLET_ID_strc         fCLET_ID_strc;  //用户ID
	//	CTL_Spd_strc         fCTL_Spd_strc;	
  //	CTL_Slo_strc         fCTL_Slo_strc;
  //CTL_Stop_strc        fCTL_Stop_strc;
}CTLPacket;
////////////////////////////////////////////////
typedef struct RunPackStrt//跑步机数据消息
{
  unsigned short	     CodeVers;
	unsigned short	     Length;
  unsigned short	     Crc;
  unsigned short	     IdentPrtc;
	Run_SPO2WAV_strc     fRun_SPO2WAV_strc;	//血氧波形
	Run_SPO2_strc        fRun_SPO2_strc ;	//血氧值
	Run_MACN_strc        fRun_MACN_strc ;	//跑步机显示值
	Run_Drug_strc        fRun_Drug_strc;	//处方完成状态
	DEV_MAC_strc         fDEV_MAC_strc;   //设备ID
	CLET_ID_strc         fCLET_ID_strc;  //用户ID
}RunPacket;
////////////////////////////////////////////////

 /*跑步机处方命令+停止命令+速度+坡度*/
typedef struct 
{
  unsigned short id;
  unsigned short time;
  unsigned short speed_doctor;
  unsigned short speed_max;
  unsigned short speed_min;
  unsigned short slope_doctor;
  unsigned short slope_max;
  unsigned short slope_min;
  unsigned short sportmode;//2017.7.17日讨论增加运动模式
}CTRL_MSG;


 /*跑步机处方命令+停止命令+速度+坡度*/
typedef struct 
{
  unsigned short type;
  unsigned short length;
  
  CTRL_MSG      CTRL_msg ;
  unsigned short STOP_msg;
  unsigned short SPEED_msg;
  unsigned short INCL_msg;

}SERVER_msg;

 
/////////////////////////////

void MacnPacketCfg(MacnPacket *Pak);
void HeaSPPacketCfg(HeaSPPacket *Pak);
void CTLPacketCfg(CTLPacket *Pak);
void RunPacketCfg(RunPacket *Pak);

void TREADMILL_CTL(TREADMILL_BaseType BaseType, \
                     TREADMILL_BaseState BaseState,uint32_t TimTick);





#define sw16(x)    ((unsigned short)(((x>>8)&0x00ff)|((x<<8)&0xff00)))
#define sw16u(x)   ((unsigned short)((((unsigned short)(x)& (unsigned short)0x00ffU)<<8) |(((unsigned short)(x)& (unsigned short)0xff00U)>>8)))
		
				
#define PULSE_WIDTH									250

#define Run_On_Pin 									GPIO_PIN_4
#define Run_On_GPIO_Port 						GPIOE
#define Run_Off_Pin 								  GPIO_PIN_3
#define Run_Off_GPIO_Port 					  GPIOE

#define Speed_Plus_Pin 							GPIO_PIN_6
#define Speed_Plus_GPIO_Port 				GPIOF
#define Speed_Minus_Pin 						  GPIO_PIN_7
#define Speed_Minus_GPIO_Port 			  GPIOF

#define Slope_Plus_Pin 							GPIO_PIN_8
#define Slope_Plus_GPIO_Port 				GPIOF 
#define Slope_Minus_Pin 						  GPIO_PIN_9
#define Slope_Minus_GPIO_Port 			  GPIOF

#define LED_Pin 										  GPIO_PIN_5
#define LED_GPIO_Port 							  GPIOA


#define CPX_DEV_ON()				          HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET)
#define CPX_DEV_OFF()				        HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)

#define CPX_SPEED_INC()			        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, GPIO_PIN_SET)
#define CPX_SPEED_DEC()			        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_7, GPIO_PIN_SET)


#define CPX_SCENT_INC()			        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_SET)
#define CPX_SCENT_DEC()			        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_SET)



#define SPEED_INC_PIN    			GPIO_PIN_1
#define SPEED_DEC_PIN					GPIO_PIN_2
#define SPEED_GPIO						  GPIOB

#define KEY_INC_SPEED       1
#define KEY_DEC_SPEED       2
void app_cpx_init(void);
void cpx_io_deinit(void);
unsigned char cpx_scan_key(void);


/********************************** (C) COPYRIGHT 2017 XX *****END OF FILE***********************/
