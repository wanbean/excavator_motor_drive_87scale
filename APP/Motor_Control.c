/*****************************************************************************************************
* @file    Motor_Control.c									     * 
* @author  WanBean										     *
* @version V1.0                                                         			     *
* @date    2020-04-11    									     *
* @brief                                               					             *
******************************************************************************************************/
/* ---------------------------------------Include--------------------------------------------------- */
#include "Motor_Control.h"
#include "SoftPwm_Control.h"
/* ---------------------------------------Private typedef ------------------------------------------ */

/* ---------------------------------------Private define ------------------------------------------- */

/* ---------------------------------------Private macro -------------------------------------------- */

/* ---------------------------------------Private variables ---------------------------------------- */

/* ---------------------------------------Private function prototypes  ----------------------------- */

#if 1
void Motor1OutH(void)
{
  GPIO_SetBits(MOTOR1_PWM_PORT, MOTOR1_PWM_PIN);
}
void Motor2OutH(void)
{
  GPIO_SetBits(MOTOR2_PWM_PORT, MOTOR2_PWM_PIN);
}
void Motor3OutH(void)
{
  GPIO_SetBits(MOTOR3_PWM_PORT, MOTOR3_PWM_PIN);
}
void Motor4OutH(void)
{
  GPIO_SetBits(MOTOR4_PWM_PORT, MOTOR4_PWM_PIN);
}
void Motor5OutH(void)
{
  GPIO_SetBits(MOTOR5_PWM_PORT, MOTOR5_PWM_PIN);
}
void Motor6OutH(void)
{
  GPIO_SetBits(MOTOR6_PWM_PORT, MOTOR6_PWM_PIN);
}

void Motor1OutL(void)
{
  GPIO_ResetBits(MOTOR1_PWM_PORT, MOTOR1_PWM_PIN);
}
void Motor2OutL(void)
{
  GPIO_ResetBits(MOTOR2_PWM_PORT, MOTOR2_PWM_PIN);
}
void Motor3OutL(void)
{
  GPIO_ResetBits(MOTOR3_PWM_PORT, MOTOR3_PWM_PIN);
}
void Motor4OutL(void)
{
  GPIO_ResetBits(MOTOR4_PWM_PORT, MOTOR4_PWM_PIN);
}
void Motor5OutL(void)
{
  GPIO_ResetBits(MOTOR5_PWM_PORT, MOTOR5_PWM_PIN);
}
void Motor6OutL(void)
{
  GPIO_ResetBits(MOTOR6_PWM_PORT, MOTOR6_PWM_PIN);
}
#endif

/**
  * @brief  电机引脚初始化
  * @param  None
  * @retval None
  */
static void MotorGPIO_Init(void)
{
  //一号电机正反转 PWM
  GPIO_Init(MOTOR1_DIR_PORT, MOTOR1_DIR_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(MOTOR1_PWM_PORT, MOTOR1_PWM_PIN, GPIO_Mode_Out_PP_High_Fast);
  //二号电机正反转 PWM
  GPIO_Init(MOTOR2_DIR_PORT, MOTOR2_DIR_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(MOTOR2_PWM_PORT, MOTOR2_PWM_PIN, GPIO_Mode_Out_PP_High_Fast);
  //三号电机正反转 PWM
  GPIO_Init(MOTOR3_DIR_PORT, MOTOR3_DIR_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(MOTOR3_PWM_PORT, MOTOR3_PWM_PIN, GPIO_Mode_Out_PP_High_Fast);
  //四号电机正反转 PWM
  GPIO_Init(MOTOR4_DIR_PORT, MOTOR4_DIR_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(MOTOR4_PWM_PORT, MOTOR4_PWM_PIN, GPIO_Mode_Out_PP_High_Fast);
  //五号电机正反转 PWM
  GPIO_Init(MOTOR5_DIR_PORT, MOTOR5_DIR_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(MOTOR5_PWM_PORT, MOTOR5_PWM_PIN, GPIO_Mode_Out_PP_High_Fast);
  //六号电机正反转 PWM
  GPIO_Init(MOTOR6_DIR_PORT, MOTOR6_DIR_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(MOTOR6_PWM_PORT, MOTOR6_PWM_PIN, GPIO_Mode_Out_PP_High_Fast);
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
  //TIM2复位
  TIM2_DeInit();
  //TIM2时基设置，32分频(2us一个数)，周期200us,频率5K 向上计数，
  TIM2_TimeBaseInit(TIM2_Prescaler_32, TIM2_CounterMode_Up, Period);
  
  //自动预装载使能
  TIM2_ARRPreloadConfig(ENABLE);
  
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
void Motor_Process(uint16_t sys_cnt)
{
  soft_pwm(sys_cnt);
}