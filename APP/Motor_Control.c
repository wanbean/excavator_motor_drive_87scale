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
#include "Receiver_PPM.h"
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


void Motor1DirF(void)
{
  GPIO_SetBits(MOTOR1_DIR_PORT, MOTOR1_DIR_PIN);
}
void Motor2DirF(void)
{
  GPIO_SetBits(MOTOR2_DIR_PORT, MOTOR2_DIR_PIN);
}
void Motor3DirF(void)
{
  GPIO_SetBits(MOTOR3_DIR_PORT, MOTOR3_DIR_PIN);
}
void Motor4DirF(void)
{
  GPIO_SetBits(MOTOR4_DIR_PORT, MOTOR4_DIR_PIN);
}
void Motor5DirF(void)
{
  GPIO_SetBits(MOTOR5_DIR_PORT, MOTOR5_DIR_PIN);
}
void Motor6DirF(void)
{
  GPIO_SetBits(MOTOR6_DIR_PORT, MOTOR6_DIR_PIN);
}

void Motor1DirB(void)
{
  GPIO_ResetBits(MOTOR1_DIR_PORT, MOTOR1_DIR_PIN);
}
void Motor2DirB(void)
{
  GPIO_ResetBits(MOTOR2_DIR_PORT, MOTOR2_DIR_PIN);
}
void Motor3DirB(void)
{
  GPIO_ResetBits(MOTOR3_DIR_PORT, MOTOR3_DIR_PIN);
}
void Motor4DirB(void)
{
  GPIO_ResetBits(MOTOR4_DIR_PORT, MOTOR4_DIR_PIN);
}
void Motor5DirB(void)
{
  GPIO_ResetBits(MOTOR5_DIR_PORT, MOTOR5_DIR_PIN);
}
void Motor6DirB(void)
{
  GPIO_ResetBits(MOTOR6_DIR_PORT, MOTOR6_DIR_PIN);
}
#endif

void (*MotorDirForwardFunc[6])(void) = {Motor1DirF,Motor2DirF,Motor3DirF,Motor4DirF,Motor5DirF,Motor6DirF};
void (*MotorDirBackwardsFunc[6])(void) = {Motor1DirB,Motor2DirB,Motor3DirB,Motor4DirB,Motor5DirB,Motor6DirB};

/**
  * @brief  ??????????????
  * @param  None
  * @retval None
  */
static void MotorGPIO_Init(void)
{
  //?????????????? PWM
  GPIO_Init(MOTOR1_DIR_PORT, MOTOR1_DIR_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(MOTOR1_PWM_PORT, MOTOR1_PWM_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_ResetBits(MOTOR1_PWM_PORT, MOTOR1_PWM_PIN);
  //?????????????? PWM
  GPIO_Init(MOTOR2_DIR_PORT, MOTOR2_DIR_PIN, GPIO_Mode_Out_OD_HiZ_Fast);
  GPIO_Init(MOTOR2_PWM_PORT, MOTOR2_PWM_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_ResetBits(MOTOR2_PWM_PORT, MOTOR2_PWM_PIN);
  //?????????????? PWM
  GPIO_Init(MOTOR3_DIR_PORT, MOTOR3_DIR_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(MOTOR3_PWM_PORT, MOTOR3_PWM_PIN, GPIO_Mode_Out_OD_HiZ_Fast);
  GPIO_ResetBits(MOTOR3_PWM_PORT, MOTOR3_PWM_PIN);
  //?????????????? PWM
  GPIO_Init(MOTOR4_DIR_PORT, MOTOR4_DIR_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(MOTOR4_PWM_PORT, MOTOR4_PWM_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_ResetBits(MOTOR4_PWM_PORT, MOTOR4_PWM_PIN);
  //?????????????? PWM
  GPIO_Init(MOTOR5_DIR_PORT, MOTOR5_DIR_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(MOTOR5_PWM_PORT, MOTOR5_PWM_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_ResetBits(MOTOR5_PWM_PORT, MOTOR5_PWM_PIN);
  //?????????????? PWM
  GPIO_Init(MOTOR6_DIR_PORT, MOTOR6_DIR_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(MOTOR6_PWM_PORT, MOTOR6_PWM_PIN, GPIO_Mode_Out_PP_High_Fast);
  GPIO_ResetBits(MOTOR6_PWM_PORT, MOTOR6_PWM_PIN);
}

/**
  * @brief  ????PWM????????????
  * @param  None
  * @retval None
  */
static void MotorTIM_Init(uint16_t Period)
{
  //????TIM2????????????
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
  //TIM2????
  TIM2_DeInit();
  //TIM2??????????32????(2us??????)??????200us,????5K ??????????
  TIM2_TimeBaseInit(TIM2_Prescaler_32, TIM2_CounterMode_Up, Period);
  
  //??????????????
  TIM2_ARRPreloadConfig(ENABLE);
  
  //TIM2??????????
  TIM2_Cmd(ENABLE);
}

/**
  * @brief  ??????????????????
  * @param  None
  * @retval None
  */
static void BoomCheckGPIO_Init(void)
{
  //????????
  GPIO_Init(BOOM_CHECK_H_PORT, BOOM_CHECK_H_PIN, GPIO_Mode_In_PU_No_IT);
  //????????
  GPIO_Init(BOOM_CHECK_L_PORT, BOOM_CHECK_L_PIN, GPIO_Mode_In_PU_No_IT);
}

/**
  * @brief  ??????????
  * @param  None
  * @retval None
  */
void Motor_Init(void)
{
  MotorGPIO_Init();
  SoftPwmInit();
  BoomCheckGPIO_Init();
}

/**
  * @brief  ????1PWM????
  * @param  Count:???????? Dir:???????? Duty:??????0-100
  * @retval None
  */
void Motor_Process(uint16_t sys_cnt)
{
  Throttle_Analysis(&PWM_CurrentData,&Control_Data);
  SoftPwmDevice[Bucket_Motor].pwm = Control_Data.Magnitude[Bucket_Motor];
  SoftPwmDevice[Arm_Motor].pwm = Control_Data.Magnitude[Arm_Motor];
  SoftPwmDevice[Boom_Motor].pwm = Control_Data.Magnitude[Boom_Motor];
  SoftPwmDevice[Swing_Motor].pwm = Control_Data.Magnitude[Swing_Motor];
  SoftPwmDevice[Left_Motor].pwm = Control_Data.Magnitude[Left_Motor];
  SoftPwmDevice[Right_Motor].pwm = Control_Data.Magnitude[Right_Motor];
#if 0
  if(PWM_CurrentData.Fail_Safe == 2)
  {
    SoftPwmDevice[Bucket_Motor].pwm = 0;
    SoftPwmDevice[Arm_Motor].pwm = 0;
    SoftPwmDevice[Boom_Motor].pwm = 0;
    SoftPwmDevice[Swing_Motor].pwm = 0;
    SoftPwmDevice[Left_Motor].pwm = 0;
    SoftPwmDevice[Right_Motor].pwm = 0;
    
    Control_Data.Magnitude[Bucket_Motor] = 0;
    Control_Data.Magnitude[Arm_Motor] = 0;
    Control_Data.Magnitude[Boom_Motor] = 0;
    Control_Data.Magnitude[Swing_Motor] = 0;
    Control_Data.Magnitude[Left_Motor] = 0;
    Control_Data.Magnitude[Right_Motor] = 0;
  }
#endif
//  SoftPwmDevice[Arm_Motor].pwm = 10;
  soft_pwm(sys_cnt);
}

/**
  * @brief  ????????
  * @param  ????????
  * @retval None
  */
uint8_t Motor_Beep(uint8_t Frequency,uint16_t HoldTime)
{
  static uint16_t LocalCnt = 0,LocalTime = 0;

  if(LocalTime == 0)
  {
    GPIO_SetBits(MOTOR1_PWM_PORT, MOTOR1_PWM_PIN);
    GPIO_SetBits(MOTOR2_PWM_PORT, MOTOR2_PWM_PIN);
    GPIO_SetBits(MOTOR3_PWM_PORT, MOTOR3_PWM_PIN);
    GPIO_SetBits(MOTOR4_PWM_PORT, MOTOR4_PWM_PIN);
    GPIO_SetBits(MOTOR5_PWM_PORT, MOTOR5_PWM_PIN);
    GPIO_SetBits(MOTOR6_PWM_PORT, MOTOR6_PWM_PIN);
  }

  if(++ LocalTime >= HoldTime)
  {
    LocalCnt = 0;
    LocalTime = 0;
    GPIO_ResetBits(MOTOR1_PWM_PORT, MOTOR1_PWM_PIN);
    GPIO_ResetBits(MOTOR2_PWM_PORT, MOTOR2_PWM_PIN);
    GPIO_ResetBits(MOTOR3_PWM_PORT, MOTOR3_PWM_PIN);
    GPIO_ResetBits(MOTOR4_PWM_PORT, MOTOR4_PWM_PIN);
    GPIO_ResetBits(MOTOR5_PWM_PORT, MOTOR5_PWM_PIN);
    GPIO_ResetBits(MOTOR6_PWM_PORT, MOTOR6_PWM_PIN);
    return 1;
  }

  if((SysCnt % Frequency) == 0)
  {
      GPIO_ToggleBits(MOTOR1_DIR_PORT, MOTOR1_DIR_PIN);
      GPIO_ToggleBits(MOTOR2_DIR_PORT, MOTOR2_DIR_PIN);
      GPIO_ToggleBits(MOTOR3_DIR_PORT, MOTOR3_DIR_PIN);
      GPIO_ToggleBits(MOTOR4_DIR_PORT, MOTOR4_DIR_PIN);
      GPIO_ToggleBits(MOTOR5_DIR_PORT, MOTOR5_DIR_PIN);
      GPIO_ToggleBits(MOTOR6_DIR_PORT, MOTOR6_DIR_PIN);
  }
  return 0;
}

