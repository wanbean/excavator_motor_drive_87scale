/*****************************************************************************************************
* @file    Servo_Control.c									     * 
* @author  WanBean										     *
* @version V1.0                                                         			     *
* @date    2020-04-16    									     *
* @brief                                               					             *
******************************************************************************************************/
/* ---------------------------------------Include--------------------------------------------------- */
#include "Servo_Control.h"
/* ---------------------------------------Private typedef ------------------------------------------ */

/* ---------------------------------------Private define ------------------------------------------- */

/* ---------------------------------------Private macro -------------------------------------------- */

/* ---------------------------------------Private variables ---------------------------------------- */

/* ---------------------------------------Private function prototypes  ----------------------------- */

/**
  * @brief  定时器3配置
  * @param  None
  * @retval None
  */
static void ServoTIM_Config(void)
{
  //使能TIM3外设时钟门控
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);
  TIM3_DeInit();
  //TIM3时基设置，1分频，向上计数，
  TIM3_TimeBaseInit(TIM3_Prescaler_8, TIM3_CounterMode_Up,40000);
  //TIM2 PWM2模式，输出使能，
  TIM3_OC1Init(TIM3_OCMode_PWM2, TIM3_OutputState_Enable, 3000,  TIM3_OCPolarity_Low, TIM3_OCIdleState_Set);
  TIM3_OC1PreloadConfig(ENABLE);
  TIM3_OC2Init(TIM3_OCMode_PWM2, TIM3_OutputState_Enable, 3000,  TIM3_OCPolarity_Low, TIM3_OCIdleState_Set);
  TIM3_OC2PreloadConfig(ENABLE);
  
  //自动预装载使能
  TIM3_ARRPreloadConfig(ENABLE);

  //使能TIM2输出
  TIM3_CtrlPWMOutputs(ENABLE);
  TIM3_ITConfig(TIM3_IT_Update,ENABLE);
  //TIM3计数器使能
  TIM3_Cmd(ENABLE);
  
  //清捕获标志
  TIM3_ClearFlag(TIM3_FLAG_Update);
}

/**
  * @brief  舵机引脚初始化
  * @param  None
  * @retval None
  */
static void ServoGPIO_Config(void)
{
  //一号舵机PWM
  GPIO_Init(SERVO1_PWM_PORT, SERVO1_PWM_PIN, GPIO_Mode_Out_PP_High_Fast);
  //二号舵机PWM
  GPIO_Init(SERVO2_PWM_PORT, SERVO2_PWM_PIN, GPIO_Mode_Out_PP_High_Fast);
}

/**
  * @brief  舵机引脚初始化
  * @param  None
  * @retval None
  */
void Servo_Init(void)
{
  ServoTIM_Config();
  ServoGPIO_Config();
}

/**
  * @brief  舵机1PWM控制
  * @param  Count:控制周期 Pulse:正脉宽1000-2000
  * @retval None
  */
void Servo1_PWM(uint32_t Count,uint16_t Pulse)
{
  uint16_t Count_Temp = 0;
  Count_Temp = Count%400;//周期 = 50us*400=20ms

  if(Count_Temp < (Pulse/50))
  {
    //高电平
    GPIO_SetBits(SERVO1_PWM_PORT,SERVO1_PWM_PIN);
  }
  else
  {
    //低电平
    GPIO_ResetBits(SERVO1_PWM_PORT,SERVO1_PWM_PIN);
  }
}
//void Servo1_PWM(uint32_t Count,uint8_t Pulse)
//{
////  uint16_t Count_Temp = 0;
////  Count_Temp = Count%2000;//周期 = 10us*2000=20ms
//  
//  if(Count < Pulse)
//  {
//    //高电平
//    GPIO_WriteBit(SERVO1_PWM_PORT,SERVO1_PWM_PIN,SET);
//    //printf("H\r\n");
//  }
//  else
//  {
//    //低电平
//    GPIO_WriteBit(SERVO1_PWM_PORT,SERVO1_PWM_PIN,RESET);
//    //printf("H\r\n");
//  }
//}

/**
  * @brief  舵机2PWM控制
  * @param  Count:控制周期 Pulse:正脉宽100-200
  * @retval None
  */
void Servo2_PWM(uint32_t Count,uint16_t Pulse)
{
{
  uint16_t Count_Temp = 0;
  Count_Temp = Count%400;//周期 = 50us*400=20ms

  if(Count_Temp < (Pulse/50))
  {
    //高电平
    GPIO_SetBits(SERVO2_PWM_PORT,SERVO2_PWM_PIN);
  }
  else
  {
    //低电平
    GPIO_ResetBits(SERVO2_PWM_PORT,SERVO2_PWM_PIN);
  }
}
}