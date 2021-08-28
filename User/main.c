/* Includes ------------------------------------------------------------------*/
#include "stm8l10x.h"
#include "Receiver_PPM.h"
#include "Motor_Control.h"
#include "Servo_Control.h"
#include "SoftPwm_Control.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
****��ڲ�������
****���ڲ�������
****������ע������ȷ��ʱ����
****��Ȩ��Ϣ������Ƕ��ʽϵͳ
*******************************************************************************/
void Delay(__IO uint16_t nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
    }
}
/**
  * @brief  ���ڿ��ƶ�ʱ����ʼ�� ÿ����0.0.0625*256 250���� ����20ms
  * @param  None
  * @retval None
  */
//void CycleTIM_Init(void)
//{
//  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4,ENABLE); //ʹ������ʱ�ӣ�STM8L����ʱ��Ĭ�Ϲرգ�ʹ��ǰ��ʹ��
//  TIM4_DeInit();
//  TIM4_TimeBaseInit(TIM4_Prescaler_16,100);//16M/1024=15.625K��0xff=255,255*��1/15.625��=0.01632S = 16.32ms����Լ61���ж���1S
//  TIM4_ITConfig(TIM4_IT_Update, ENABLE);
//  TIM4_Cmd(ENABLE);//TIM4ʹ��
//}

//��۵ƿ�������
#define Boom_Lamp_PORT                  GPIOA
#define Boom_Lamp_PIN                   GPIO_Pin_2
//����ƿ�������
#define Shell_Lamp_PORT                GPIOA
#define Shell_Lamp_PIN                 GPIO_Pin_6

/**
  * @brief  �ƹ����ų�ʼ��
  * @param  None
  * @retval None
  */
void LightGPIO_Init(void)
{
  //��۵�
  GPIO_Init(Boom_Lamp_PORT, Boom_Lamp_PIN, GPIO_Mode_Out_PP_High_Fast);
  //�����
  GPIO_Init(Shell_Lamp_PORT, Shell_Lamp_PIN, GPIO_Mode_Out_PP_High_Fast);

  //�õ͵�ƽ
  GPIO_ResetBits(Boom_Lamp_PORT,Boom_Lamp_PIN);
  GPIO_ResetBits(Shell_Lamp_PORT,Shell_Lamp_PIN);
}

uint16_t i = 0,j = 0;
void main(void)
{
  CLK_MasterPrescalerConfig(CLK_MasterPrescaler_HSIDiv1);
  //USART_QuickInit();
  PPM_Reveiver_Init();
  LightGPIO_Init();
  Motor_Init();
  OutHOLD_INIT();

//  GPIO_Init(GPIOA,GPIO_Pin_3,GPIO_Mode_Out_PP_High_Fast);
  enableInterrupts();//�����ж��ܿ���
    
    uint32_t sigLostCnt = 0;
  while(1)
  {
      //debug222 = TIM2_GetCounter();
      
      i+=3;
      if(i >= 60001) i = 0;
#if 0
      if(PWM_CurrentData.Fail_Safe == 0) PWM_CurrentData.Fail_Safe = 1;
      
      
      if(PWM_CurrentData.Fail_Safe == 1)
      {
        if(sigLostCnt++ >= 7500)
        {
          sigLostCnt = 0;
          PWM_CurrentData.Fail_Safe = 2;
        }
      }
      else
      {
        sigLostCnt = 0;
      }
#endif
      
      if(PWM_CurrentData.PWM_Status == 0)
      {
        if(Motor_Beep(1,100))
        {
            PWM_CurrentData.PWM_Status = 5;
        }
      }
      else if(PWM_CurrentData.PWM_Status == 2)
      {
        if(Motor_Beep(5,100))
        {
            PWM_CurrentData.PWM_Status = 5;
        }
      }
      else if(PWM_CurrentData.PWM_Status == 4)
      {
        if(Motor_Beep(10,100))
        {
            PWM_CurrentData.PWM_Status = 5;
        }
      }
      else if(PWM_CurrentData.PWM_Status == 6)
      {
        Motor_Process(i);
      }
      
  }
}


/**
  * @}
  */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/