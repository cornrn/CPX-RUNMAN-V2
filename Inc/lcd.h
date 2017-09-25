/****************************************Copyright (c)**************************************************
**                               ����XX���Ӽ������޹�˾
**                                    ��Ʒ�з���
**                                        
**                              http://www.XX.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: lcd.c
**��   ��   ��: KK
**����޸�����: 2017��04��28��
**��        ��: LCDӦ�ó���
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#define LCD_CMD_HEAD  0xAA
#define LCD_CMD_END   0xCC 0x33 0xC3 0x3C

#include "main.h"
#include "cmsis_os.h"

#define LCD_MAIL_SIZE        (uint32_t) 10

typedef struct 
{ /* Mail object structure */
  uint8_t type; /* var1 is a uint32_t */
  uint8_t value1; /* var3 is a uint8_t */
  uint8_t value2; /* var3 is a uint8_t */
} LCD_mail_TypeDef;

extern osMailQId LCD_mailId;

#define MAILE_TYPE_SPO2   1


/*�ٶ����Ӱ�������*/
#define BTN_ADD_SPEED_XS  642
#define BTN_ADD_SPEED_YS  72
#define BTN_ADD_SPEED_XE  753
#define BTN_ADD_SPEED_YE  123
/*�ٶȼ��ٰ�������*/
#define BTN_REDUCE_SPEED_XS  642
#define BTN_REDUCE_SPEED_YS  158
#define BTN_REDUCE_SPEED_XE  753
#define BTN_REDUCE_SPEED_YE  209
/*�¶����Ӱ�������*/
#define BTN_ADD_SLOPE_XS  642
#define BTN_ADD_SLOPE_YS  279
#define BTN_ADD_SLOPE_XE  754
#define BTN_ADD_SLOPE_YE  330
/*�¶ȼ��ٰ�������*/
#define BTN_REDUCE_SLOPE_XS  642
#define BTN_REDUCE_SLOPE_YS  366
#define BTN_REDUCE_SLOPE_XE  753
#define BTN_REDUCE_SLOPE_YE  417


#define TEXT_SPO2_X   100
#define TEXT_SPO2_Y   100
#define TEXT_HR_X     400
#define TEXT_HR_Y     100

#define COLOR_RED     0xF800
#define COLOR_GREEN   0x07E0
#define COLOR_BLUE    0x001F
/********************************** (C) COPYRIGHT 2017 XX *****END OF FILE***********************/











