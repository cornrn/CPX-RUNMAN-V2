#ifndef __CPX_BOARD_H
#define __CPX_BOARD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

 /** @addtogroup STM32F7XX_CPX_BOARD_LEVEL_CTL
* @{
*/
   
   
#define  KEY1_Port   GPIOD
#define  KEY2_Port   GPIOE
#define  KEY3_Port   GPIOE
#define  KEY4_Port   GPIOE
#define  KEY5_Port   GPIOE
#define  KEY6_Port   GPIOE
#define  KEY7_Port   GPIOD
#define  KEY8_Port   GPIOD
#define  KEY9_Port   GPIOD
#define  KEY10_Port  GPIOF
 
#define  KEY_Port(x)  KEY##x##_Port

  
#define  KEY1_PIN    GPIO_PIN_9
#define  KEY2_PIN    GPIO_PIN_15
#define  KEY3_PIN    GPIO_PIN_13
#define  KEY4_PIN    GPIO_PIN_11
#define  KEY5_PIN    GPIO_PIN_9
#define  KEY6_PIN    GPIO_PIN_7
#define  KEY7_PIN    GPIO_PIN_0
#define  KEY8_PIN    GPIO_PIN_14
#define  KEY9_PIN    GPIO_PIN_4
#define  KEY10_PIN   GPIO_PIN_12
 

#define  KEY_Pin(x)  KEY##x##_PIN

//模拟开关控制
#define  KEY_PRS(NUM)   HAL_GPIO_WritePin(KEY_Port(NUM), KEY_Pin(NUM), GPIO_PIN_SET)
 
#define  KEY_RLX(NUM)  HAL_GPIO_WritePin(KEY_Port(NUM), KEY_Pin(NUM), GPIO_PIN_RESET)

#define  KEY_PRS_START()  KEY_PRS(9)//swt2&s2 
#define  KEY_RLX_START()  KEY_RLX(9)

#define  KEY_PRS_STOP()   KEY_PRS(10)
#define  KEY_RLX_STOP()   KEY_RLX(10)

//急停控制
#define  JerkStop_ON()      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_SET)
#define  JerkStop_OFF()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_6, GPIO_PIN_RESET)
#define  JerkStop_TOG()     HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_6)

#define  JerkStop_Read()    HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)

extern void   BUTTON_CTL_Init(void);//模拟开关控制端初始化
   
#endif   



