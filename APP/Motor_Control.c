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
  * @brief  ������ų�ʼ��
  * @param  None
  * @retval None
  */
static void MotorGPIO_Init(void)
{
  //һ�ŵ������ת PWM
  GPIO_Init(MOTOR1_DIR_PORT, MOTOR1_DIR_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(MOTOR1_PWM_PORT, MOTOR1_PWM_PIN, GPIO_Mode_Out_PP_High_Fast);
  //���ŵ������ת PWM
  GPIO_Init(MOTOR2_DIR_PORT, MOTOR2_DIR_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(MOTOR2_PWM_PORT, MOTOR2_PWM_PIN, GPIO_Mode_Out_PP_High_Fast);
  //���ŵ������ת PWM
  GPIO_Init(MOTOR3_DIR_PORT, MOTOR3_DIR_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(MOTOR3_PWM_PORT, MOTOR3_PWM_PIN, GPIO_Mode_Out_PP_High_Fast);
  //�ĺŵ������ת PWM
  GPIO_Init(MOTOR4_DIR_PORT, MOTOR4_DIR_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(MOTOR4_PWM_PORT, MOTOR4_PWM_PIN, GPIO_Mode_Out_PP_High_Fast);
  //��ŵ������ת PWM
  GPIO_Init(MOTOR5_DIR_PORT, MOTOR5_DIR_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(MOTOR5_PWM_PORT, MOTOR5_PWM_PIN, GPIO_Mode_Out_PP_High_Fast);
  //���ŵ������ת PWM
  GPIO_Init(MOTOR6_DIR_PORT, MOTOR6_DIR_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(MOTOR6_PWM_PORT, MOTOR6_PWM_PIN, GPIO_Mode_Out_PP_High_Fast);
}

/**
  * @brief  ���PWM��ʱ����ʼ��
  * @param  None
  * @retval None
  */
static void MotorTIM_Init(uint16_t Period)
{
  //ʹ��TIM2����ʱ���ſ�
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
  //TIM2��λ
  TIM2_DeInit();
  //TIM2ʱ�����ã�32��Ƶ(2usһ����)������200us,Ƶ��5K ���ϼ�����
  TIM2_TimeBaseInit(TIM2_Prescaler_32, TIM2_CounterMode_Up, Period);
  
  //�Զ�Ԥװ��ʹ��
  TIM2_ARRPreloadConfig(ENABLE);
  
  //TIM2������ʹ��
  TIM2_Cmd(ENABLE);
}

/**
  * @brief  �����ʼ��
  * @param  None
  * @retval None
  */
void Motor_Init(uint16_t Period)
{
  MotorGPIO_Init();
  MotorTIM_Init(Period);
}

/**
  * @brief  ���1PWM����
  * @param  Count:�������� Dir:��ת���� Duty:ռ�ձ�0-100
  * @retval None
  */
void Motor_Process(uint16_t sys_cnt)
{
  soft_pwm(sys_cnt);
}