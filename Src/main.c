/****************************************Copyright (c)**************************************************
**                               江苏XX电子技术有限公司
**                                    产品研发部
**                                        
**                     
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: main.c
**创   建   人: KK
**最后修改日期: 2017年04月28日
**描        述: 应用程序入口
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/

#include "main.h"
#include "cmsis_os.h"
#include "client_socket.h"
#include "app_cpx.h"
#include "CPX_BOARD.h"
 
unsigned int SysFlag = 0,Tick1sCon= 0;//系统标志/1s计数器
uint32_t  HaveRunTime;
osMailQId Server_mailId;/*来自服务器的处方邮箱地址*/

__IO  unsigned short current_SPEED = 0; //记录当前速度
__IO  static  unsigned short current_INCL = 0;  //记录当前坡度

uint16_t Chufang_REC_flag=0,setting_sporttime=0,sport_mode=0;

void Timer1Callback(void const * argument);

osTimerId myTimer01Handle;
 
osThreadId ProcessRpThreadId;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void StartThread(void const * argument);
static void ProcessRpThread (void const * argument);//处理处方
static void BSP_Config(void);
static void Key_Click(uint32_t KeyNum,uint32_t ms);
/* Private functions ---------------------------------------------------------*/
extern void lcd_init(void);
extern void spo2_init(void);
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();

  HAL_Init();  
  
  /* Configure the system clock to 200 MHz */
  SystemClock_Config();
 
  /* Init task */
  osThreadDef(Start, StartThread, osPriorityNormal, 1, configMINIMAL_STACK_SIZE * 2);
  osThreadCreate (osThread(Start), NULL);
  
  /* Start scheduler */
  osKernelStart();
}

/**
  * @brief  Start Thread 
  * @param  argument not used
  * @retval None
  */
static void StartThread(void const * argument)
{ 
  BSP_Config();
  BUTTON_CTL_Init();
//  lcd_init();
  spo2_init();  
  client_socket_init(); 
  app_cpx_init();
  ClrAllSysFlag();//清除SYS标志
  osMailQDef(mail, 1, SERVER_msg);
  Server_mailId = osMailCreate(osMailQ(mail), NULL);
  
  /* definition and creation of myTimer01 */
  osTimerDef(myTimer01, Timer1Callback);
  myTimer01Handle = osTimerCreate(osTimer(myTimer01), osTimerPeriodic, NULL);
   /* Start Timer */
  osTimerStart(myTimer01Handle, 1000);//一秒钟进入一次

  osThreadDef(ProcessRpThreadId, ProcessRpThread , osPriorityBelowNormal, 1, 1024);
  osThreadCreate (osThread(ProcessRpThreadId), NULL);
  

  /* Delete the Init Thread */ 
  osThreadTerminate(NULL);

}

/*处方处理任务*/
static void ProcessRpThread (void const * argument)
{ 
  SERVER_msg *pCMail;
  uint8_t k ;
  uint32_t SpeedChangeTimeInterval;//均匀加减速时间间隔
  osEvent event;
 
  while(1)
  { 
      event = osMailGet(Server_mailId, 10); /* wait for mail */
  
      if(GetSysFlag(Cpx_Ready2Flag))//如果跑步机按下开始键速度达到0.6km/h后则可以响应加减速CMD坡度此过程中不响应
      {
        if(sport_mode==0)//运动模式为0
         {
          if(GetSysFlag(Rp_SteadyStateFlag))//置位前3min后进去稳态标志
          {
            if(event.status == osEventMail)
            {
              pCMail = event.value.p;
              
              printf("前3min后进去稳态标志\r\n");
              if(pCMail->STOP_msg == 1){ //收到服务器停止消息后
                 printf("收到服务器停止消息后\r\n");
                 Key_Click(2,300);//STOP KEY 300ms*2
                 current_SPEED  =  0;   //记录当前速度
                 current_INCL   =  0;   //记录当前坡度
                 pCMail->STOP_msg = 0;
                 ClrAllSysFlag();//清除SYS标志
                 osMailFree(Server_mailId, pCMail);
               }else{
                  if(pCMail->SPEED_msg==1){
                  if(current_SPEED<100)
                    {
                      Key_Click(8,300);//SPEED+ KEY 300ms*2
                      current_SPEED++;
                      pCMail->SPEED_msg=0;
                    }  
                  }else if(pCMail->SPEED_msg==2){
                    if(current_SPEED>0.6)
                    {
                      Key_Click(4,300);//SPEED- KEY 300ms*2
                      current_SPEED--;
                      pCMail->SPEED_msg=0;
                    }
                  }
                 
               }
              
             osMailFree(Server_mailId, pCMail); 
            }
          }
          else 
          {
            if(GetSysFlag(Rp_SteadyOverFlag))//稳态结束标志
            {
              printf("收到稳态结束标志\r\n");
              for(k=current_SPEED;k>6;k--)
              {
                Key_Click(4,300);//SPEED- KEY 300ms*2 3分钟内均匀减速
                SpeedChangeTimeInterval=180000/(current_SPEED-6)-600;//均匀加减速时间间隔
                printf("SpeedChangeTimeInterval = %d\r\n",SpeedChangeTimeInterval);
                osDelay(SpeedChangeTimeInterval);//3min/(current_SPEED-6)-0.6s
              } 
              Key_Click(2,300);//STOP KEY 300ms*2
              current_SPEED  =  0;   //记录当前速度
              current_INCL   =  0;   //记录当前坡度
              pCMail->STOP_msg = 0;
              ClrAllSysFlag();//清除SYS标志
              SetSysFlag(Cpx_StopFlag);//一个完整处方到此运行结束 
            }
            else
            {
              printf("2.current_SPEED = %d\r\n",current_SPEED);
              for(k=6;k<current_SPEED;k++)
              {
                Key_Click(8,300);//SPEED+ KEY 300ms*2 3分钟内均匀提速
                SpeedChangeTimeInterval=180000/(current_SPEED-6)-600;//均匀加减速时间间隔
                printf("SpeedChangeTimeInterval = %d\r\n",SpeedChangeTimeInterval);
                osDelay(SpeedChangeTimeInterval);//3min/(current_SPEED-6)-0.6s
              }
              SetSysFlag(Rp_SteadyStateFlag);//置位前3min后进去稳态标志
            }
          }
        }else if(sport_mode==1){//运动模式为1
          if(GetSysFlag(Rp_OverFlag))//处方结束标志
            {
              printf("收到处方结束标志\r\n");
              
              Key_Click(2,300);//STOP KEY 300ms*2
              current_SPEED  =  0;   //记录当前速度
              current_INCL   =  0;   //记录当前坡度
              pCMail->STOP_msg = 0;
              ClrAllSysFlag();//清除SYS标志
              SetSysFlag(Cpx_StopFlag);//一个完整处方到此运行结束 
            }
            else
            {
              
               if(event.status == osEventMail)
                {
                  pCMail = event.value.p;
                  
                  if(pCMail->SPEED_msg==1){
                  if(current_SPEED<100)
                    {
                      Key_Click(8,300);//SPEED+ KEY 300ms*2
                      current_SPEED++;
                      pCMail->SPEED_msg=0;
                    }  
                  }else if(pCMail->SPEED_msg==2){
                    if(current_SPEED>0.6)
                    {
                      Key_Click(4,300);//SPEED- KEY 300ms*2
                      current_SPEED--;
                      pCMail->SPEED_msg=0;
                    }
                  }
                osMailFree(Server_mailId, pCMail); 
                }
            }
        }
      }else{
        
        if(event.status == osEventMail)
        {
         pCMail = event.value.p;
      
         if(pCMail->STOP_msg == 1){ //收到服务器停止消息后
           printf("收到服务器停止消息后\r\n");
           Key_Click(2,300);//STOP KEY 300ms*2
           current_SPEED  =  0;   //记录当前速度
           current_INCL   =  0;   //记录当前坡度
           pCMail->STOP_msg = 0; 
           ClrAllSysFlag();//清除SYS标志
           osMailFree(Server_mailId, pCMail);
         }else{
             current_SPEED =  pCMail->CTRL_msg.speed_doctor; //处方设定速度
             setting_sporttime = 60*pCMail->CTRL_msg .time ;//记录设定的运动时长单位s
             sport_mode=pCMail ->CTRL_msg .sportmode ;//处方模式
           
             printf("setting_sporttime=%d\r\n",setting_sporttime);
             if(current_SPEED>100) current_SPEED =100;else if(current_SPEED<6) current_SPEED =6;
             printf("1.current_SPEED=%d\r\n",current_SPEED);
           
           if(pCMail->CTRL_msg.slope_doctor>current_INCL){
              current_INCL  =  pCMail->CTRL_msg.slope_doctor; //记录当前坡度
              if(current_INCL>15) current_INCL =15;
              else if(current_INCL<1) current_INCL =1;
              Key_Click(1,300);//START KEY 300ms*2
              osDelay(5000);//按了开始按键后需要5秒进去READY2状态才可以加减坡度//此时当前速度为0.6km/h
              for(k=0;k<current_INCL;k++){
                Key_Click(7,300);//INCL+ KEY 300ms*2 提高坡度
              }
            
            SetSysFlag(Cpx_Ready2Flag);//跑步机按下开始键速度达到0.6km/h后则可以响应加减速CMD
            osMailFree(Server_mailId, pCMail);
            printf("置位Cpx_Ready2Flag\r\n");
           } 
         }           
      }
    }
    osDelay(50);   
  }

}

 
/* Timer1Callback function */
void Timer1Callback(void const * argument)
{
  /* USER CODE BEGIN Timer1Callback *///Rp_SteadyOverFlag
 if(sport_mode==0){//运动模式为0   
  if(GetSysFlag(Rp_SteadyStateFlag))//跑步机稳态速度后计时达到运动时间准备后3min减速运动
  {
    if(++Tick1sCon==setting_sporttime)
    {
      ClrSysFlag(Rp_SteadyStateFlag);//清除进去稳态标志
      printf("清除进去稳态标志\r\n");
      SetSysFlag(Rp_SteadyOverFlag);//稳态结束标志
      printf("清除稳态结束标志\r\n");
      Tick1sCon=0;
    }
    printf("Tick1sCon=%d\r\n",Tick1sCon);
  }
}else if(sport_mode==1){//运动模式为1
  if(++Tick1sCon==(setting_sporttime+6000))
  {
 
    SetSysFlag(Rp_OverFlag);//处方结束标志
    
    Tick1sCon=0;
  }
}
  /* USER CODE END Timer1Callback */
}
 

static void Key_Click(uint32_t KeyNum,uint32_t ms)
{
  switch(KeyNum)
  {
    case 8:
      KEY_PRS(8);//SPEED+ KEY
      osDelay(ms);
      KEY_RLX(8) ;//SPEED+ KEY
      osDelay(ms);
      printf("加速\r\n");
      break;
    case 4:
      KEY_PRS(4);//SPEED- KEY
      osDelay(ms);
      KEY_RLX(4) ;//SPEED- KEY
      osDelay(ms);
      printf("减速\r\n");
      break;
    case 7:
      KEY_PRS(7);//INCL+ KEY
      osDelay(ms);
      KEY_RLX(7) ;//INCL+ KEY
      osDelay(ms);
      printf("提陡\r\n");
      break;
    case 3:
      KEY_PRS(3);//INCL- KEY
      osDelay(ms);
      KEY_RLX(3) ;//INCL- KEY
      osDelay(ms);
      printf("缓坡\r\n");
      break;
    case 1:
      KEY_PRS(1);//START KEY
      osDelay(ms);
      KEY_RLX(1) ;//START KEY
      osDelay(ms);
      printf("启动\r\n");
      break;
    case 2:
      KEY_PRS(2);//STOP KEY
      osDelay(ms);
      KEY_RLX(2) ;//STOP KEY
      osDelay(ms);
      printf("停止\r\n"); 
      break;
    
  }
 
}
  
/**
  * @brief  Initializes the STM32756G-EVAL's LCD and LEDs resources.
  * @param  None
  * @retval None
  */
static UART_HandleTypeDef  UartHandle;
static void BSP_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOA , &GPIO_InitStruct);
  
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_USART2_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
  
  UartHandle.Instance = USART2;
  UartHandle.Init.BaudRate = 115200;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits = UART_STOPBITS_1;
  UartHandle.Init.Parity = UART_PARITY_NONE;
  UartHandle.Init.Mode = UART_MODE_TX_RX;
  UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  UartHandle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_UART_Init(&UartHandle);
  
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 4
  *            PLL_N                          = 216
  *            PLL_P                          = 2
  *            PLL_Q                          = 2
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 432;  
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  /* activate the OverDrive to reach the 216 Mhz Frequency */
  HAL_PWREx_EnableOverDrive();
 
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
}


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART3 and Loop until the end of transmission */
  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}


/********************************** (C) COPYRIGHT 2017 XX *****END OF FILE***********************/
