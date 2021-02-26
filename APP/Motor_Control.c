/*****************************************************************************************************
* @file    Motor_Control.c									     * 
* @author  WanBean										     *
* @version V1.0                                                         			     *
* @date    2020-04-11    									     *
* @brief                                               					             *
******************************************************************************************************/
/* ---------------------------------------Include--------------------------------------------------- */
#include "Motor_Control.h"
/* ---------------------------------------Private typedef ------------------------------------------ */

/* ---------------------------------------Private define ------------------------------------------- */

/* ---------------------------------------Private macro -------------------------------------------- */

/* ---------------------------------------Private variables ---------------------------------------- */
uint16_t Motor_PWMPeriod = 0;
/* ---------------------------------------Private function prototypes  ----------------------------- */

/**
  * @brief  电机引脚初始化
  * @param  None
  * @retval None
  */
static void MotorGPIO_Init(void)
{
  //一号电机正反转 PWM
  GPIO_Init(MOTOR1_PWM1_PORT, MOTOR1_PWM1_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(MOTOR1_PWM2_PORT, MOTOR1_PWM2_PIN, GPIO_Mode_Out_PP_High_Fast);
}

/**
  * @brief  电机PWM定时器初始化
  * @param  None
  * @retval None
  */
static void MotorTIM_Init(uint16_t Period)
{
  //使能TIM2外设时钟门控
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
  
  //TIM2时基设置，8分频(0.5us一个数)，周期50us,频率20K 向上计数，
  TIM2_TimeBaseInit(TIM2_Prescaler_128, TIM2_CounterMode_Up, Period);
  //TIM2 PWM2模式，输出使能，
  TIM2_OC1Init(TIM2_OCMode_PWM2, TIM2_OutputState_Enable, 0,  TIM2_OCPolarity_Low, TIM2_OCIdleState_Set);
  TIM2_OC1PreloadConfig(ENABLE);
  TIM2_OC2Init(TIM2_OCMode_PWM2, TIM2_OutputState_Enable, 0,  TIM2_OCPolarity_Low, TIM2_OCIdleState_Set);
  TIM2_OC2PreloadConfig(ENABLE);
  
  //自动预装载使能
  TIM2_ARRPreloadConfig(ENABLE);

  //使能TIM2输出
  TIM2_CtrlPWMOutputs(ENABLE);
  
  //TIM2计数器使能
  TIM2_Cmd(ENABLE);
}

/**
  * @brief  电机初始化
  * @param  None
  * @retval None
  */
void Motor_Init(uint16_t Period)
{
  MotorGPIO_Init();
  MotorTIM_Init(Period);
}

/**
  * @brief  电机1PWM控制
  * @param  Count:控制周期 Dir:旋转方向 Duty:占空比0-100
  * @retval None
  */
void Motor1_PWM(int8_t Dir,int16_t Duty)
{
  //参数检查
  if(Duty > Motor_PWMPeriod) Duty = Motor_PWMPeriod;
  if(Duty < 0) Duty = 0;
  //正转
  if(Dir == 1)
  {
    TIM2_SetCompare1(Duty);
    TIM2_SetCompare2(0);
  }
  //停止
  else if(Dir == 0)
  {
    TIM2_SetCompare1(0);
    TIM2_SetCompare2(0);
  }
  //反转
  else if(Dir == -1)
  {
    TIM2_SetCompare1(0);
    TIM2_SetCompare2(Duty * MOTOR_BackUp_Scale);
  }
}