/****************************************Copyright (c)**************************************************
**                               ����XX���Ӽ������޹�˾
**                                    ��Ʒ�з���
**                                        
**                     
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: main.c
**��   ��   ��: KK
**����޸�����: 2017��04��28��
**��        ��: Ӧ�ó������
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/

#include "main.h"
#include "cmsis_os.h"
#include "client_socket.h"
#include "app_cpx.h"
#include "CPX_BOARD.h"
 
unsigned int SysFlag = 0,Tick1sCon= 0;//ϵͳ��־/1s������
uint32_t  HaveRunTime;
osMailQId Server_mailId;/*���Է������Ĵ��������ַ*/

__IO  unsigned short current_SPEED = 0; //��¼��ǰ�ٶ�
__IO  static  unsigned short current_INCL = 0;  //��¼��ǰ�¶�

uint16_t Chufang_REC_flag=0,setting_sporttime=0,sport_mode=0;

void Timer1Callback(void const * argument);

osTimerId myTimer01Handle;
 
osThreadId ProcessRpThreadId;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void StartThread(void const * argument);
static void ProcessRpThread (void const * argument);//������
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
  ClrAllSysFlag();//���SYS��־
  osMailQDef(mail, 1, SERVER_msg);
  Server_mailId = osMailCreate(osMailQ(mail), NULL);
  
  /* definition and creation of myTimer01 */
  osTimerDef(myTimer01, Timer1Callback);
  myTimer01Handle = osTimerCreate(osTimer(myTimer01), osTimerPeriodic, NULL);
   /* Start Timer */
  osTimerStart(myTimer01Handle, 1000);//һ���ӽ���һ��

  osThreadDef(ProcessRpThreadId, ProcessRpThread , osPriorityBelowNormal, 1, 1024);
  osThreadCreate (osThread(ProcessRpThreadId), NULL);
  

  /* Delete the Init Thread */ 
  osThreadTerminate(NULL);

}

/*������������*/
static void ProcessRpThread (void const * argument)
{ 
  SERVER_msg *pCMail;
  uint8_t k ;
  uint32_t SpeedChangeTimeInterval;//���ȼӼ���ʱ����
  osEvent event;
 
  while(1)
  { 
      event = osMailGet(Server_mailId, 10); /* wait for mail */
  
      if(GetSysFlag(Cpx_Ready2Flag))//����ܲ������¿�ʼ���ٶȴﵽ0.6km/h���������Ӧ�Ӽ���CMD�¶ȴ˹����в���Ӧ
      {
        if(sport_mode==0)//�˶�ģʽΪ0
         {
          if(GetSysFlag(Rp_SteadyStateFlag))//��λǰ3min���ȥ��̬��־
          {
            if(event.status == osEventMail)
            {
              pCMail = event.value.p;
              
              printf("ǰ3min���ȥ��̬��־\r\n");
              if(pCMail->STOP_msg == 1){ //�յ�������ֹͣ��Ϣ��
                 printf("�յ�������ֹͣ��Ϣ��\r\n");
                 Key_Click(2,300);//STOP KEY 300ms*2
                 current_SPEED  =  0;   //��¼��ǰ�ٶ�
                 current_INCL   =  0;   //��¼��ǰ�¶�
                 pCMail->STOP_msg = 0;
                 ClrAllSysFlag();//���SYS��־
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
            if(GetSysFlag(Rp_SteadyOverFlag))//��̬������־
            {
              printf("�յ���̬������־\r\n");
              for(k=current_SPEED;k>6;k--)
              {
                Key_Click(4,300);//SPEED- KEY 300ms*2 3�����ھ��ȼ���
                SpeedChangeTimeInterval=180000/(current_SPEED-6)-600;//���ȼӼ���ʱ����
                printf("SpeedChangeTimeInterval = %d\r\n",SpeedChangeTimeInterval);
                osDelay(SpeedChangeTimeInterval);//3min/(current_SPEED-6)-0.6s
              } 
              Key_Click(2,300);//STOP KEY 300ms*2
              current_SPEED  =  0;   //��¼��ǰ�ٶ�
              current_INCL   =  0;   //��¼��ǰ�¶�
              pCMail->STOP_msg = 0;
              ClrAllSysFlag();//���SYS��־
              SetSysFlag(Cpx_StopFlag);//һ�����������������н��� 
            }
            else
            {
              printf("2.current_SPEED = %d\r\n",current_SPEED);
              for(k=6;k<current_SPEED;k++)
              {
                Key_Click(8,300);//SPEED+ KEY 300ms*2 3�����ھ�������
                SpeedChangeTimeInterval=180000/(current_SPEED-6)-600;//���ȼӼ���ʱ����
                printf("SpeedChangeTimeInterval = %d\r\n",SpeedChangeTimeInterval);
                osDelay(SpeedChangeTimeInterval);//3min/(current_SPEED-6)-0.6s
              }
              SetSysFlag(Rp_SteadyStateFlag);//��λǰ3min���ȥ��̬��־
            }
          }
        }else if(sport_mode==1){//�˶�ģʽΪ1
          if(GetSysFlag(Rp_OverFlag))//����������־
            {
              printf("�յ�����������־\r\n");
              
              Key_Click(2,300);//STOP KEY 300ms*2
              current_SPEED  =  0;   //��¼��ǰ�ٶ�
              current_INCL   =  0;   //��¼��ǰ�¶�
              pCMail->STOP_msg = 0;
              ClrAllSysFlag();//���SYS��־
              SetSysFlag(Cpx_StopFlag);//һ�����������������н��� 
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
      
         if(pCMail->STOP_msg == 1){ //�յ�������ֹͣ��Ϣ��
           printf("�յ�������ֹͣ��Ϣ��\r\n");
           Key_Click(2,300);//STOP KEY 300ms*2
           current_SPEED  =  0;   //��¼��ǰ�ٶ�
           current_INCL   =  0;   //��¼��ǰ�¶�
           pCMail->STOP_msg = 0; 
           ClrAllSysFlag();//���SYS��־
           osMailFree(Server_mailId, pCMail);
         }else{
             current_SPEED =  pCMail->CTRL_msg.speed_doctor; //�����趨�ٶ�
             setting_sporttime = 60*pCMail->CTRL_msg .time ;//��¼�趨���˶�ʱ����λs
             sport_mode=pCMail ->CTRL_msg .sportmode ;//����ģʽ
           
             printf("setting_sporttime=%d\r\n",setting_sporttime);
             if(current_SPEED>100) current_SPEED =100;else if(current_SPEED<6) current_SPEED =6;
             printf("1.current_SPEED=%d\r\n",current_SPEED);
           
           if(pCMail->CTRL_msg.slope_doctor>current_INCL){
              current_INCL  =  pCMail->CTRL_msg.slope_doctor; //��¼��ǰ�¶�
              if(current_INCL>15) current_INCL =15;
              else if(current_INCL<1) current_INCL =1;
              Key_Click(1,300);//START KEY 300ms*2
              osDelay(5000);//���˿�ʼ��������Ҫ5���ȥREADY2״̬�ſ��ԼӼ��¶�//��ʱ��ǰ�ٶ�Ϊ0.6km/h
              for(k=0;k<current_INCL;k++){
                Key_Click(7,300);//INCL+ KEY 300ms*2 ����¶�
              }
            
            SetSysFlag(Cpx_Ready2Flag);//�ܲ������¿�ʼ���ٶȴﵽ0.6km/h���������Ӧ�Ӽ���CMD
            osMailFree(Server_mailId, pCMail);
            printf("��λCpx_Ready2Flag\r\n");
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
 if(sport_mode==0){//�˶�ģʽΪ0   
  if(GetSysFlag(Rp_SteadyStateFlag))//�ܲ�����̬�ٶȺ��ʱ�ﵽ�˶�ʱ��׼����3min�����˶�
  {
    if(++Tick1sCon==setting_sporttime)
    {
      ClrSysFlag(Rp_SteadyStateFlag);//�����ȥ��̬��־
      printf("�����ȥ��̬��־\r\n");
      SetSysFlag(Rp_SteadyOverFlag);//��̬������־
      printf("�����̬������־\r\n");
      Tick1sCon=0;
    }
    printf("Tick1sCon=%d\r\n",Tick1sCon);
  }
}else if(sport_mode==1){//�˶�ģʽΪ1
  if(++Tick1sCon==(setting_sporttime+6000))
  {
 
    SetSysFlag(Rp_OverFlag);//����������־
    
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
      printf("����\r\n");
      break;
    case 4:
      KEY_PRS(4);//SPEED- KEY
      osDelay(ms);
      KEY_RLX(4) ;//SPEED- KEY
      osDelay(ms);
      printf("����\r\n");
      break;
    case 7:
      KEY_PRS(7);//INCL+ KEY
      osDelay(ms);
      KEY_RLX(7) ;//INCL+ KEY
      osDelay(ms);
      printf("�ᶸ\r\n");
      break;
    case 3:
      KEY_PRS(3);//INCL- KEY
      osDelay(ms);
      KEY_RLX(3) ;//INCL- KEY
      osDelay(ms);
      printf("����\r\n");
      break;
    case 1:
      KEY_PRS(1);//START KEY
      osDelay(ms);
      KEY_RLX(1) ;//START KEY
      osDelay(ms);
      printf("����\r\n");
      break;
    case 2:
      KEY_PRS(2);//STOP KEY
      osDelay(ms);
      KEY_RLX(2) ;//STOP KEY
      osDelay(ms);
      printf("ֹͣ\r\n"); 
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
