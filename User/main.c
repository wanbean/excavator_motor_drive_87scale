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
#include "Light_Control.h"
#include "Servo_Control.h"
/* Private typedef -----------------------------------------------------------*/
//����LED�ӿ�
#define LED_GPIO_PORT  GPIOD
#define LED_GPIO_PINS  GPIO_Pin_0
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

void main(void)
{
//  uint8_t i = 0;
  CLK_MasterPrescalerConfig(CLK_MasterPrescaler_HSIDiv1);
  //USART_QuickInit();
  PPM_Reveiver_Init();
  LightGPIO_Init();
  Servo_Init();
  GPIO_Init(GPIOA,GPIO_Pin_3,GPIO_Mode_Out_PP_High_Fast);
  enableInterrupts();//�����ж��ܿ���
  while(1)
  {

    if(PWM_CurrentData.Fail_Safe == 20)
    {
      //�ر�PWM,�ƹ���˸
      Motor1_PWM(0,0);
      Lamp_Blink_Single(Turning_Signal_Lamp_L_PORT,Turning_Signal_Lamp_L_PIN);
      Lamp_Blink_Single(Turning_Signal_Lamp_R_PORT,Turning_Signal_Lamp_R_PIN);
    }
    else
    {
      //У׼���ֵ
      if(PWM_CurrentData.PWM_Ready == 0)
      {
        Lamp_Blink_Single(Head_Lamp_PORT,Head_Lamp_PIN);
      }
      //У׼��ֵ
      else if(PWM_CurrentData.PWM_Ready == 1)
      {
        Lamp_Blink_Single(Stop_Lamp_PORT,Stop_Lamp_PIN);
      }
      //У׼��Сֵ
      else if(PWM_CurrentData.PWM_Ready == 2)
      {
        Lamp_Blink_Single(BackUp_Lamp_PORT,BackUp_Lamp_PIN); 
      }
      //���Ų�����λ
      else if((PWM_CurrentData.PWM_Ready == 3)&&(ABS(PWM_CurrentData.PWM_Data[Throttle_Channel] -  PWM_CurrentData.PWM_Mid[0]) > DeadZone))
      {
        Lamp_Blink_All();
      }
      else if(PWM_CurrentData.PWM_Ready == 4)
      {
        BackUp_Lamp_Control(Lamp_Mode.BackUp_Lame_Mode);
        Turning_Signal_Lamp_Control(Lamp_Mode.Turning_Signal_Lamp_Mode);
        Stop_Lamp_Control(&Lamp_Mode.Stop_Lamp_Mode);
        Head_Lamp_Control(Lamp_Mode.Head_Lamp_Mode);
        Plice_Lamp_Control(&Lamp_Mode.Police_Lamp_Mode);
        Motor1_PWM(Control_Data.Throttle_Dir,Control_Data.Throttle_Magnitude);      
      }
    }
    
//    GPIO_ToggleBits(LED_GPIO_PORT,LED_GPIO_PINS);
//    Delay(0xFFFF);
//    Delay(0xFFFF);
//    Delay(0xFFFF);
//    Delay(0xFFFF);
//    Delay(0xFFFF);
//    Delay(0xFFFF);
      //Motor1_PWM(1,250);
    //Turning_Signal_Lamp_Control(Turning_Signal_Turn_Right);
  }
}


/**
  * @}
  */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/