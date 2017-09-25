/****************************************Copyright (c)**************************************************
**                               ����XX���Ӽ������޹�˾
**                                    ��Ʒ�з���
**                                        
**                              http://www.XX.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: app_cpx.h
**��   ��   ��: KK
**��  �� �� ��: 2017��05��04��
**��        ��: �ܲ���Ӧ�ó���
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
typedef struct TLVHeadStrt//�豸ע����Ϣ
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
  Cpx_StartFlag 	          = (1<<1),//CPX��ʼ��־				
	Cpx_StopFlag 				    = (1<<2),//CPXֹͣ��־
  Cpx_Ready2Flag 	        = (1<<3),//�ܲ�������ʼ���ʼ����0.6KM/H��ȥ����׼����ʼ��־
	SPO2H_DqEnFlag 	        = (1<<4),//Ѫ������ͬ��ʹ�ܱ�־	
	SPO2H_RecEnFlag 		      = (1<<5),//Ѫ�����ݲɼ�ʹ�ܱ�־
  Err_SPO2HFlag			      = (1<<6),//Ѫ���˿������־	
	Rp_StartFlag		          = (1<<7),//������ʼ��־
	Rp_StopFlag	            = (1<<8),//����������־
	Err_HeartTickFlag	      = (1<<9),//���������־��δ�յ���λ��������Ϣ��λ��
	EmergStopFlag				    = (1<<10),//��ͣ��־
  Rp_TimerStartFlag        = (1<<11),//������ʼ��ʱ��־
  Rp_SteadyStateFlag       = (1<<12),//�������е���̬�ı�־
  Rp_SteadyOverFlag        = (1<<13),//�������е���̬�����ı�־
  Rp_OverFlag              = (1<<14),//���������ı�־
}AllSysFlag;
 
/********************************************************************************************************
** ��������: SystemFlag
** ��������: ϵͳ��־
********************************************************************************************************/
extern unsigned int SysFlag; 

/*******************************************************************************************************
** ��������: ϵͳ��־�����궨��
********************************************************************************************************/
#define GetSysFlag(flag)        SysFlag & (flag)
#define SetSysFlag(flag)        SysFlag |= (flag)
#define ClrSysFlag(flag)        SysFlag &= ~(flag)
#define ClrAllSysFlag()         SysFlag= 0x00



/*******************************************************************************************************
** �ṹ����: ���ݰ�
** ��������:  
********************************************************************************************************/
typedef struct
{
	unsigned short       Type;
	unsigned short	     Length;
	unsigned int	       Value;		
}CLET_ID_strc;	//�û�ID

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
}Run_Drug_strc;	//�������״̬
////////////////////////////////////////////////



////////////////////////////////////////////////
typedef struct MacnPackStrt//�豸ע����Ϣ
{
  unsigned short	     CodeVers;
	unsigned short	     Length;
  unsigned short	     Crc;
  unsigned short	     IdentPrtc;
	DEV_MAC_strc         fDEV_MAC_strc;
	DEV_TYPE_strc        fDEV_TYPE_strc;
}MacnPacket;
////////////////////////////////////////////////
typedef struct HeaSPPackStrt//������Ϣ
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
	CLET_ID_strc         fCLET_ID_strc;  //�û�ID
	//	CTL_Spd_strc         fCTL_Spd_strc;	
  //	CTL_Slo_strc         fCTL_Slo_strc;
  //CTL_Stop_strc        fCTL_Stop_strc;
}CTLPacket;
////////////////////////////////////////////////
typedef struct RunPackStrt//�ܲ���������Ϣ
{
  unsigned short	     CodeVers;
	unsigned short	     Length;
  unsigned short	     Crc;
  unsigned short	     IdentPrtc;
	Run_SPO2WAV_strc     fRun_SPO2WAV_strc;	//Ѫ������
	Run_SPO2_strc        fRun_SPO2_strc ;	//Ѫ��ֵ
	Run_MACN_strc        fRun_MACN_strc ;	//�ܲ�����ʾֵ
	Run_Drug_strc        fRun_Drug_strc;	//�������״̬
	DEV_MAC_strc         fDEV_MAC_strc;   //�豸ID
	CLET_ID_strc         fCLET_ID_strc;  //�û�ID
}RunPacket;
////////////////////////////////////////////////

 /*�ܲ�����������+ֹͣ����+�ٶ�+�¶�*/
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
  unsigned short sportmode;//2017.7.17�����������˶�ģʽ
}CTRL_MSG;


 /*�ܲ�����������+ֹͣ����+�ٶ�+�¶�*/
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
