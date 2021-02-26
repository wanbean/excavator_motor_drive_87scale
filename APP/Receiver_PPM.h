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
#include "Light_Control.h"
#include "Motor_Control.h"
#include "Servo_Control.h"
#include "stm8l10x_usart.h"
#include "stm8l10x_tim2.h"
/* ---------------------------------------Private typedef ------------------------------------------ */
typedef struct{
  uint8_t PWM_Ready;
  uint16_t PWM_Max[2];
  uint16_t PWM_Mid[2];
  uint16_t PWM_Min[2];
  uint16_t PWM_Data[8];
  uint8_t  PWM1_Status;
  uint8_t  PWM2_Status;
  uint8_t  Fail_Safe;
}PWM_CURRENTDATA;

typedef struct{
  int8_t Throttle_Dir;
  int16_t Throttle_Magnitude;
  int8_t Light_Status;
}CONTROL_DATA;
/* ---------------------------------------Private define ------------------------------------------- */
//PPM信号输入引脚
#define PPM_Input_PORT      GPIOA
#define PPM_Input_PIN       GPIO_Pin_0
//Flash 储存地址
#define PWM1_MAXAdd 0x9BFF
#define PWM1_MIDAdd PWM1_MAXAdd + 2
#define PWM1_MINAdd PWM1_MIDAdd + 2
#define PWM2_MIDAdd PWM1_MINAdd + 2
//通道映射
#define Throttle_Channel 2
#define Diverter_Channel 0
#define Light_Channel    4        


#define DeadZone         20
#define SwitchThreshold  200
#define PWM_MinRef       1000
#define PWM_CentreRef    1550  
#define PWM_MaxRef       2000  
/* ---------------------------------------Private macro -------------------------------------------- */

/* ---------------------------------------Private variables ---------------------------------------- */
extern PWM_CURRENTDATA PWM_CurrentData;
extern CONTROL_DATA Control_Data;
extern volatile uint8_t PPMCapture_Pried;
/* ---------------------------------------Private function prototypes  ----------------------------- */
void PPM_Reveiver_Init(void);
int16_t ABS(int16_t Data);
void PWM_Process(void);
void PPM_Decode(void);

#endif




































