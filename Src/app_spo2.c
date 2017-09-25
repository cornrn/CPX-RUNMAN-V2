/****************************************Copyright (c)**************************************************
**                               江苏XX电子技术有限公司
**                                    产品研发部
**                                        
**                              http://www.XX.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: app_spo2.c
**创   建   人: KK
**创  建 日 期: 2017年05月08日
**描        述: 血氧模块
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#include "main.h"
#include "cmsis_os.h"
#include "lcd.h"

UART_HandleTypeDef  SPO2_UartHandle;

extern uint16_t SOP2H_VALUE;

static void BSP_SPO2_Config(void);
static void SPO2_thread(void const * argument);


void spo2_init(void)
{
  BSP_SPO2_Config();
  
  osThreadDef(SPO2, SPO2_thread, osPriorityBelowNormal, 0, configMINIMAL_STACK_SIZE * 2);
  osThreadCreate (osThread(SPO2), NULL);
}


static void SPO2_thread(void const * argument)
{
  unsigned char rx[5],tick = 0;
  
//  LCD_mail_TypeDef *pTMail;
  
  while(1)
  { 
    if(HAL_OK == HAL_UART_Receive_IT(&SPO2_UartHandle, (uint8_t *)rx, 5))
    {
      if(rx[0]&0x10)
      {
        if(tick++ == 5)
        {
          tick = 0;
//          pTMail = osMailAlloc(LCD_mailId, osWaitForever); /* Allocate memory */
//          pTMail->type = MAILE_TYPE_SPO2;
//          pTMail->value1 = rx[1]&0x7F;
//          pTMail->value2 = rx[2]&0x7F;
         
          SOP2H_VALUE = rx[1]&0x7F; 
//          if(osMailPut(LCD_mailId, pTMail) != osOK) /* Send Mail */  
//          {      
//            //printf("LCD mail put has fail\n");
//          }
        }
      }
    }
  }
}

 
static void BSP_SPO2_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
//  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;
  
  __HAL_RCC_GPIOC_CLK_ENABLE();
  
  __HAL_RCC_UART4_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  
  SPO2_UartHandle.Instance = UART4;
  SPO2_UartHandle.Init.BaudRate = 115200;
  SPO2_UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  SPO2_UartHandle.Init.StopBits = UART_STOPBITS_1;
  SPO2_UartHandle.Init.Parity = UART_PARITY_NONE;
  SPO2_UartHandle.Init.Mode = UART_MODE_TX_RX;
  SPO2_UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  SPO2_UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  SPO2_UartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  SPO2_UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_UART_Init(&SPO2_UartHandle);
  
  HAL_NVIC_SetPriority(UART4_IRQn, 5, 1);
  HAL_NVIC_EnableIRQ(UART4_IRQn);
}



/**
  * @brief  This function handles UART interrupt request.  
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA stream 
  *         used for USART data transmission     
  */
void UART4_IRQHandler(void)
{
  HAL_UART_IRQHandler(&SPO2_UartHandle);
}



/********************************** (C) COPYRIGHT 2017 XX *****END OF FILE***********************/



