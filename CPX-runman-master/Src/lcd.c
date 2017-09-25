/****************************************Copyright (c)**************************************************
**                               江苏XX电子技术有限公司
**                                    产品研发部
**                                        
**                              http://www.XX.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: lcd.c
**创   建   人: KK
**最后修改日期: 2017年04月28日
**描        述: LCD应用程序
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#include <stdarg.h>
#include "tlv.h"
#include "lcd.h"

//UART_HandleTypeDef  LCD_UartHandle;

uint16_t SOP2H_VALUE = 0;

//static void BSP_LCD_Config(void);
//static void LCD_thread(void const * argument);

//void lcd_set_mode(void);
//void lcd_clear(void);
//void lcd_open_rtc(u16 color,u16 x,u16 y);
//void lcd_open_buzzer(unsigned char time);
//void lcd_printf_value(u16 x,u16 y,u16 color,unsigned char value);
//void lcd_cut_picture(unsigned char id,u16 xs,u16 ys,u16 xe,u16 ye,u16 x,u16 y);
//void lcd_show_picture(unsigned char id);
//void lcd_set_color(u16 f_color,u16 b_color);
//void lcd_rect_invert(u16 xs,u16 ys,u16 xe,u16 ye);

//osMailQId LCD_mailId;/*LCD显示内容邮箱地址*/

///*按键坐标*/
//const u16 btn_rect[4][4] = 
//{
//  {BTN_ADD_SPEED_XS,BTN_ADD_SPEED_YS,BTN_ADD_SPEED_XE,BTN_ADD_SPEED_YE},
//  {BTN_REDUCE_SPEED_XS,BTN_REDUCE_SPEED_YS,BTN_REDUCE_SPEED_XE,BTN_REDUCE_SPEED_YE},
//  {BTN_ADD_SPEED_XS,BTN_ADD_SLOPE_YS,BTN_ADD_SLOPE_XE,BTN_ADD_SLOPE_YE},
//  {BTN_REDUCE_SLOPE_XS,BTN_REDUCE_SLOPE_YS,BTN_REDUCE_SLOPE_XE,BTN_REDUCE_SLOPE_YE}
//}; 
//  

///**
//  * 初始化LCD模组
//  */
//void lcd_init(void)
//{
//  BSP_LCD_Config();
//  
//  osMailQDef(mail, LCD_MAIL_SIZE, LCD_mail_TypeDef);
//  LCD_mailId = osMailCreate(osMailQ(mail), NULL);
//  
//  osThreadDef(LCD, LCD_thread, osPriorityBelowNormal, 0, configMINIMAL_STACK_SIZE * 2);
//  osThreadCreate (osThread(LCD), NULL);
//}

///**
//  * LCD线程函数 
//  */
//static void LCD_thread(void const * argument)
//{ 
//  unsigned char rx[10];
//  
//  u16 x,y,btn_num = 0xFFFF;
//  
//  osEvent event;
//  LCD_mail_TypeDef *pRMail;

//  lcd_open_buzzer(100);
//  
//  lcd_open_rtc(0xF800,5,7);
//  
//  while(1)
//  {  
//    event = osMailGet(LCD_mailId, 40); /* wait for mail */
//    
//    if(event.status == osEventMail)
//    {
//      pRMail = event.value.p;
//      if(pRMail->type == MAILE_TYPE_SPO2)
//      {
//        lcd_printf_value(TEXT_SPO2_X,TEXT_SPO2_Y,COLOR_GREEN,pRMail->value1);
//        lcd_printf_value(TEXT_HR_X,TEXT_HR_Y,COLOR_GREEN,pRMail->value2);
//        SOP2H_VALUE = pRMail->value1;
//        //printf ("SOP2H_VALUE=%d\r\n",SOP2H_VALUE);
//      }
//      osMailFree(LCD_mailId, pRMail); /* free memory allocated for mail */       
//    }		

//    if(HAL_OK == HAL_UART_Receive_IT(&LCD_UartHandle, rx, 10))
//    {
//      if(rx[0] == 0xAA && rx[1] == 0x73)/*按下键码*/
//      {
//        x = rx[2]<<8|rx[3];/*获取坐标*/
//        y = rx[4]<<8|rx[5];
//        
//        if(x > BTN_ADD_SPEED_XS && x < BTN_ADD_SPEED_XE)
//        {
//          if(y>BTN_ADD_SPEED_YS&&y<BTN_ADD_SPEED_YE)btn_num = 0;
//          else if(y>BTN_REDUCE_SPEED_YS&&y<BTN_REDUCE_SPEED_YE)btn_num = 1;
//          else if(y>BTN_ADD_SLOPE_YS&&y<BTN_ADD_SLOPE_YE)btn_num = 2;
//          else if(y>BTN_REDUCE_SLOPE_YS&&y<BTN_REDUCE_SLOPE_YE)btn_num = 3; 
//          else btn_num = 0xFFFF; 
//          if(btn_num != 0xFFFF){
//            lcd_rect_invert(btn_rect[btn_num][0],btn_rect[btn_num][1],btn_rect[btn_num][2],btn_rect[btn_num][3]);
//          }
//        }
//      }
//    
//      if(rx[0] == 0xAA && rx[1] == 0x72 && btn_num != 0xFFFF)/*离开键码*/
//      {       
//        lcd_rect_invert(btn_rect[btn_num][0],btn_rect[btn_num][1],btn_rect[btn_num][2],btn_rect[btn_num][3]);
//        btn_num = 0xFFFF;
//      }
//    } 
//  }
//}


///**
//  * 矩形区域反色显示
//  */
//void lcd_rect_invert(u16 xs,u16 ys,u16 xe,u16 ye)
//{
//  unsigned char cmd[14];
//  
//  cmd[0] = 0xAA;
//  cmd[1] = 0x5C;
//  
//  cmd[2] =  (xs&0xFF00)>>8;
//  cmd[3] =  (xs&0xFF);
//  cmd[4] =  (ys&0xFF00)>>8;
//  cmd[5] =  (ys&0xFF);
//  cmd[6] =  (xe&0xFF00)>>8;
//  cmd[7] =  (xe&0xFF);
//  cmd[8] =  (ye&0xFF00)>>8;
//  cmd[9] = (ye&0xFF);

//  cmd[10] = 0xCC;
//  cmd[11] = 0x33;
//  cmd[12] = 0xC3;
//  cmd[13] = 0x3C;
//  
//  HAL_UART_Transmit(&LCD_UartHandle,cmd, 14, 0xFFFF);
//}

///**
//  * 打印数值，0～0xFF
//  */
//void lcd_printf_value(u16 x,u16 y,u16 color,unsigned char value)
//{
//  unsigned char *cmd,a,b;
//  
//  int i=0,len;
//  
//  if(value<10)len = 18;
//  if(value<100)len = 19;
//  else len = 20;
//  
//  cmd = (unsigned char *)malloc(len);
//  
//  cmd[i++] = 0xAA;
//  cmd[i++] = 0x98;
//  cmd[i++] = (x&0xFF00)>>8;
//  cmd[i++] = (x&0xFF);
//  cmd[i++] = (y&0xFF00)>>8;
//  cmd[i++] = (y&0xFF);
//  cmd[i++] = 0x00;
//  cmd[i++] = 0x81;
//  cmd[i++] = 0x07;
//  cmd[i++] = (color&0xFF00)>>8;
//  cmd[i++] = (color&0xFF);
//  cmd[i++] = 0x00;
//  cmd[i++] = 0x00;
//  
//  a = value/100;
//  b = (value - a*100)/10;
//  
//  if(a>0)cmd[i++] = a + 0x30;
//  if(b>0)cmd[i++] = b + 0x30;
//  cmd[i++] = (value - 100*a - 10*b) + 0x30;
//  
//  cmd[i++] = 0xCC;
//  cmd[i++] = 0x33;
//  cmd[i++] = 0xC3;
//  cmd[i++] = 0x3C;
//  
//  lcd_cut_picture(0,x,y,x+96,y+64,x,y);
//  HAL_UART_Transmit(&LCD_UartHandle,cmd, len, 0xFFFF);
//  free(cmd);
//}

///**
//  * 剪切图标
//  */
//void lcd_cut_picture(unsigned char id,u16 xs,u16 ys,u16 xe,u16 ye,u16 x,u16 y)
//{
//  unsigned char cmd[19];
//  
//  cmd[0] = 0xAA;
//  cmd[1] = 0x71;
//  
//  cmd[2] = id;
//  cmd[3] =  (xs&0xFF00)>>8;
//  cmd[4] =  (xs&0xFF);
//  cmd[5] =  (ys&0xFF00)>>8;
//  cmd[6] =  (ys&0xFF);
//  cmd[7] =  (xe&0xFF00)>>8;
//  cmd[8] =  (xe&0xFF);
//  cmd[9] =  (ye&0xFF00)>>8;
//  cmd[10] = (ye&0xFF);
//  cmd[11] = (x&0xFF00)>>8;
//  cmd[12] = (x&0xFF);
//  cmd[13] = (y&0xFF00)>>8;
//  cmd[14] = (y&0xFF);
//  
//  cmd[15] = 0xCC;
//  cmd[16] = 0x33;
//  cmd[17] = 0xC3;
//  cmd[18] = 0x3C;
//  HAL_UART_Transmit(&LCD_UartHandle,cmd, 19, 0xFFFF);
//}

///**
//  * 显示图片
//  */
//void lcd_show_picture(unsigned char id)
//{
//  unsigned char cmd[7];
//  
//  cmd[0] = 0xAA;
//  cmd[1] = 0x70;
//  
//  cmd[2] = id;

//  cmd[3] = 0xCC;
//  cmd[4] = 0x33;
//  cmd[5] = 0xC3;
//  cmd[6] = 0x3C;
//  HAL_UART_Transmit(&LCD_UartHandle,cmd, 7, 0xFFFF);
//}

///**
//  * 设置调色板
//  */
//void lcd_set_color(u16 f_color,u16 b_color)
//{
//  unsigned char cmd[10];
//  
//  cmd[0] = 0xAA;
//  cmd[1] = 0x40;
//  
//  cmd[2] = (f_color&0xFF00)>>8;
//  cmd[3] = (f_color&0xFF);
//  cmd[4] = (b_color&0xFF00)>>8;
//  cmd[5] = (b_color&0xFF);

//  cmd[6] = 0xCC;
//  cmd[7] = 0x33;
//  cmd[8] = 0xC3;
//  cmd[9] = 0x3C;
//  HAL_UART_Transmit(&LCD_UartHandle,cmd, 10, 0xFFFF);
//}


///**
//  * 打开实时时钟，开启time毫秒后关闭
//  */
//void lcd_open_rtc(u16 color,u16 x,u16 y)
//{
//  unsigned char cmd[15] = {0xAA,0x9B,0xFF,0x01,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0xCC,0x33,0xC3,0x3C};
//  cmd[5] = (color&0xFF00)>>8;
//  cmd[6] = (color&0xFF);
//  cmd[7] = (x&0xFF00)>>8;
//  cmd[8] = (x&0xFF);
//  cmd[9] = (y&0xFF00)>>8;
//  cmd[10] = (y&0xFF);
//  HAL_UART_Transmit(&LCD_UartHandle,cmd, 15, 0xFFFF);
//}
///**
//  * 打开蜂鸣器，开启time毫秒后关闭
//  */
//void lcd_open_buzzer(unsigned char time)
//{
//  unsigned char cmd[7] = {0xAA,0x79,0,0xCC,0x33,0xC3,0x3C};
//  cmd[2] = (unsigned char)time/10;
//  HAL_UART_Transmit(&LCD_UartHandle,cmd, 7, 0xFFFF);
//}

///**
//  * LCD清屏
//  */
//void lcd_clear(void)
//{
//  unsigned char cmd[6] = {0xAA,0x52,0xCC,0x33,0xC3,0x3C};
//  HAL_UART_Transmit(&LCD_UartHandle,cmd, 6, 0xFFFF);
//}

///**
//  * LCD工作模式设定
//  */
//void lcd_set_mode(void)
//{
//  unsigned char cmd[13];
//  
//  cmd[0] = 0xAA;
//  cmd[1] = 0xE0;
//  
//  cmd[2] = 0x55;
//  cmd[3] = 0xAA;
//  cmd[4] = 0x5A;
//  cmd[5] = 0xA5;
//  cmd[6] = 0x02;
//  cmd[7] = 0x07;
//  cmd[8] = 0x40;

//  cmd[9] = 0xCC;
//  cmd[10] = 0x33;
//  cmd[11] = 0xC3;
//  cmd[12] = 0x3C;
//  HAL_UART_Transmit(&LCD_UartHandle,cmd, 13, 0xFFFF);
//}

///**
//  * LCD配置
//  */
//static void BSP_LCD_Config(void)
//{
//  GPIO_InitTypeDef GPIO_InitStruct;
//  
//  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit;
//  
//  __HAL_RCC_GPIOA_CLK_ENABLE();
//  
//  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
//  RCC_PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_SYSCLK;
//  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit);
//  
//  __HAL_RCC_USART1_CLK_ENABLE();

//  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
//  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//  GPIO_InitStruct.Pull = GPIO_PULLUP;
//  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
//  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//  
//  LCD_UartHandle.Instance = USART1;
//  LCD_UartHandle.Init.BaudRate = 115200;
//  LCD_UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
//  LCD_UartHandle.Init.StopBits = UART_STOPBITS_1;
//  LCD_UartHandle.Init.Parity = UART_PARITY_NONE;
//  LCD_UartHandle.Init.Mode = UART_MODE_TX_RX;
//  LCD_UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//  LCD_UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
//  LCD_UartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
//  LCD_UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
//  HAL_UART_Init(&LCD_UartHandle);
//  
//  HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
//  HAL_NVIC_EnableIRQ(USART1_IRQn);
//}

///**
//  * @brief  This function handles UART interrupt request.    
//  */
//void USART1_IRQHandler(void)
//{
//  HAL_UART_IRQHandler(&LCD_UartHandle);
//}



///********************************** (C) COPYRIGHT 2017 XX *****END OF FILE***********************/
