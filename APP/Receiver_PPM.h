/*****************************************************************************************************
* @file    Reveiver_PPM.h									     * 
* @author  WanBean										     *
* @version V1.0                                                         			     *
* @date    2020-03-27    									     *
* @brief                                               					             *
******************************************************************************************************/
#ifndef _RECEIVER_PPM_H
#define _RECEIVER_PPM_H
/* ---------------------------------------Include--------------------------------------------------- */
#include "stm8l10x.h"
#include "Motor_Control.h"
#include "Servo_Control.h"
#include "stm8l10x_usart.h"
#include "stm8l10x_tim2.h"
/* ---------------------------------------Private typedef ------------------------------------------ */
typedef struct
{
//  uint8_t PWM_Ready;
  uint8_t Fail_Safe;
  uint16_t PWM_Max[8];
  uint16_t PWM_Mid[8];
  uint16_t PWM_Min[8];
  uint16_t PWM_Data[8];
  uint8_t PWM_Status[8];
} PWM_CURRENTDATA;

typedef struct
{
  int8_t Throttle_Dir;
  int16_t Throttle_Magnitude;
  int8_t Light_Status;
} CONTROL_DATA;
/* ---------------------------------------Private define ------------------------------------------- */
//PPM信号输入引脚
#define PPM_Input_PORT GPIOA
#define PPM_Input_PIN GPIO_Pin_0
//Flash 储存地址
#define PWM1_MAXAdd 0x9BFF
#define PWM1_MIDAdd PWM1_MAXAdd + 2
#define PWM1_MINAdd PWM1_MIDAdd + 2

#define PWM2_MAXAdd PWM1_MINAdd + 2
#define PWM2_MIDAdd PWM2_MAXAdd + 2
#define PWM2_MINAdd PWM2_MIDAdd + 2

#define PWM3_MAXAdd PWM2_MINAdd + 2
#define PWM3_MIDAdd PWM3_MAXAdd + 2
#define PWM3_MINAdd PWM3_MIDAdd + 2

#define PWM4_MAXAdd PWM3_MINAdd + 2
#define PWM4_MIDAdd PWM4_MAXAdd + 2
#define PWM4_MINAdd PWM4_MIDAdd + 2

#define PWM5_MAXAdd PWM4_MINAdd + 2
#define PWM5_MIDAdd PWM5_MAXAdd + 2
#define PWM5_MINAdd PWM5_MIDAdd + 2

#define PWM6_MAXAdd PWM5_MINAdd + 2
#define PWM6_MIDAdd PWM6_MAXAdd + 2
#define PWM6_MINAdd PWM6_MIDAdd + 2

#define PWM7_MAXAdd PWM6_MINAdd + 2
#define PWM7_MIDAdd PWM7_MAXAdd + 2
#define PWM7_MINAdd PWM7_MIDAdd + 2

#define PWM8_MAXAdd PWM7_MINAdd + 2
#define PWM8_MIDAdd PWM8_MAXAdd + 2
#define PWM8_MINAdd PWM8_MIDAdd + 2

//通道映射
#define Bucket_Channel        1
#define Arm_Channel           2
#define Boom_Channel          3
#define Swing_Channel         4
#define Left_Motor_Channel    5
#define Right_Motor_Channel   6


#define DeadZone 20
#define SwitchThreshold 200
#define PWM_MinRef 1000
#define PWM_CentreRef 1550
#define PWM_MaxRef 2000
/* ---------------------------------------Private macro -------------------------------------------- */

/* ---------------------------------------Private variables ---------------------------------------- */
extern PWM_CURRENTDATA PWM_CurrentData;
extern CONTROL_DATA Control_Data;
extern volatile uint8_t PPMCapture_Pried;
/* ---------------------------------------Private function prototypes  ----------------------------- */
void PPM_Reveiver_Init(void);
void PWM_Process(void);
void PPM_Decode(void);

#endif
