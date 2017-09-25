/****************************************Copyright (c)**************************************************
**                               ����XX���Ӽ������޹�˾
**                                    ��Ʒ�з���
**                                        
**                              http://www.XX.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: app_cpx.c
**��   ��   ��: KK
**��  �� �� ��: 2017��05��04��
**��        ��: �ܲ���Ӧ�ó���
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



/*�˶�ƽ������*/
struct MovingPlate_Attribute
{
  uint8_t now_speed;//��ǰ�ٶ�
	uint8_t now_ascent;//��ǰ�¶�
	uint8_t max_speed;//�趨���ٶ�����
	uint8_t min_speed;//�趨���ٶ�����
	uint8_t doctor_ascent;//ҽ���¶�
  uint8_t doctor_speed;//ҽ���ٶ�
	uint32_t run_time;//���е�ʱ��
	uint32_t start_run_time;//��ʼ���е�ʱ��
	
};


 

 


/********************************** (C) COPYRIGHT 2017 XX *****END OF FILE***********************/
