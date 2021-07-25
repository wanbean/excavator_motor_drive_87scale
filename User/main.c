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
****入口参数：无
****出口参数：无
****函数备注：不精确延时函数
****版权信息：蓝旗嵌入式系统
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
  * @brief  周期控制定时器初始化 每个数0.0.0625*256 250个数 周期20ms
  * @param  None
  * @retval None
  */
//void CycleTIM_Init(void)
//{
//  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4,ENABLE); //使能外设时钟，STM8L外设时钟默认关闭，使用前需使能
//  TIM4_DeInit();
//  TIM4_TimeBaseInit(TIM4_Prescaler_16,100);//16M/1024=15.625K，0xff=255,255*（1/15.625）=0.01632S = 16.32ms，大约61次中断是1S
//  TIM4_ITConfig(TIM4_IT_Update, ENABLE);
//  TIM4_Cmd(ENABLE);//TIM4使能
//}

//大臂灯控制引脚
#define Boom_Lamp_PORT                  GPIOA
#define Boom_Lamp_PIN                   GPIO_Pin_2
//机身灯控制引脚
#define Shell_Lamp_PORT                GPIOA
#define Shell_Lamp_PIN                 GPIO_Pin_6

/**
  * @brief  灯光引脚初始化
  * @param  None
  * @retval None
  */
void LightGPIO_Init(void)
{
  //大臂灯
  GPIO_Init(Boom_Lamp_PORT, Boom_Lamp_PIN, GPIO_Mode_Out_PP_High_Fast);
  //机身灯
  GPIO_Init(Shell_Lamp_PORT, Shell_Lamp_PIN, GPIO_Mode_Out_PP_High_Fast);

  //置低电平
  GPIO_ResetBits(Boom_Lamp_PORT,Boom_Lamp_PIN);
  GPIO_ResetBits(Shell_Lamp_PORT,Shell_Lamp_PIN);
}

void main(void)
{
  CLK_MasterPrescalerConfig(CLK_MasterPrescaler_HSIDiv1);
  //USART_QuickInit();
  PPM_Reveiver_Init();
  LightGPIO_Init();
  SoftPwmInit();
  Motor_Init(60000);
  GPIO_Init(GPIOA,GPIO_Pin_3,GPIO_Mode_Out_PP_High_Fast);
  enableInterrupts();//开启中断总开关
  while(1)
  {
    if(PWM_CurrentData.PWM_Status[0] == 1)
    {
      PWM_CurrentData.PWM_Status[0] = 0;
      //油门赋值...
      Motor_Process(TIM2_GetCounter());
    }
#if 0
    if(PWM_CurrentData.Fail_Safe == 20)
    {
      //关闭PWM,灯光闪烁
      Motor1_PWM(0,0);
      Lamp_Blink_Single(Turning_Signal_Lamp_L_PORT,Turning_Signal_Lamp_L_PIN);
      Lamp_Blink_Single(Turning_Signal_Lamp_R_PORT,Turning_Signal_Lamp_R_PIN);
    }
    else
    {
      //校准最大值
      if(PWM_CurrentData.PWM_Ready == 0)
      {
        Lamp_Blink_Single(Head_Lamp_PORT,Head_Lamp_PIN);
      }
      //校准中值
      else if(PWM_CurrentData.PWM_Ready == 1)
      {
        Lamp_Blink_Single(Stop_Lamp_PORT,Stop_Lamp_PIN);
      }
      //校准最小值
      else if(PWM_CurrentData.PWM_Ready == 2)
      {
        Lamp_Blink_Single(BackUp_Lamp_PORT,BackUp_Lamp_PIN); 
      }
      //油门不在中位
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
    #endif
  }

}


/**
  * @}
  */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/