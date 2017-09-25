/****************************************Copyright (c)**************************************************
**                               江苏XX电子技术有限公司
**                                    产品研发部
**                                        
**                              http://www.XX.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: app_cpx.c
**创   建   人: KK
**创  建 日 期: 2017年05月04日
**描        述: 跑步机应用程序
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#include "app_cpx.h"
#include "tlv.h"


static void app_cpx_thread(void const * argument);


void app_cpx_init(void)
{
	osThreadDef(CPX, app_cpx_thread, osPriorityBelowNormal, 0, 1000);
  osThreadCreate (osThread(CPX), NULL);
}


static void app_cpx_thread(void const * argument)
{

	while(1)
	{	
  //  HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
    osDelay(1000);
	}
}



/*运动平板属性*/
struct MovingPlate_Attribute
{
  uint8_t now_speed;//当前速度
	uint8_t now_ascent;//当前坡度
	uint8_t max_speed;//设定的速度上限
	uint8_t min_speed;//设定的速度下限
	uint8_t doctor_ascent;//医嘱坡度
  uint8_t doctor_speed;//医嘱速度
	uint32_t run_time;//运行的时长
	uint32_t start_run_time;//开始运行的时刻
	
};


 

 


/********************************** (C) COPYRIGHT 2017 XX *****END OF FILE***********************/
