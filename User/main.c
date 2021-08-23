/**
  ******************************************************************************
  * @file     Project/STM8L10x_StdPeriph_Templates/main.c
  * @author   MCD Application Team
  * @version V1.2.1
  * @date    30-September-2014
  * @brief    This file contains the firmware main function.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

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

uint16_t i = 0;
void main(void)
{
  CLK_MasterPrescalerConfig(CLK_MasterPrescaler_HSIDiv1);
  //USART_QuickInit();
  PPM_Reveiver_Init();
  LightGPIO_Init();
  Motor_Init();
//  GPIO_Init(GPIOA,GPIO_Pin_3,GPIO_Mode_Out_PP_High_Fast);
  enableInterrupts();//�����ж��ܿ���
    
    uint32_t sigLostCnt = 0;
  while(1)
  {
      //debug222 = TIM2_GetCounter();
      
      i+=3;
      if(i >= 60001) i = 0;
      
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
      Motor_Process(i);
  }
}


/**
  * @}
  */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/