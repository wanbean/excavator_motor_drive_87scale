/*****************************************************************************************************
* @file    Reveiver_PWM.h									     * 
* @author  WanBean										     *
* @version V1.0                                                         			     *
* @date    2020-04-20    									     *
* @brief                                               					             *
******************************************************************************************************/
#ifndef _RECEIVER_PWM_H
#define _RECEIVER_PWM_H
/* ---------------------------------------Include--------------------------------------------------- */
#include "stm8l10x.h"
#include "Motor_Control.h"
/* ---------------------------------------Private typedef ------------------------------------------ */
//#define ABS(x) ((x)>0?(x):-(x))
/* ---------------------------------------Private define ------------------------------------------- */
//Flash 储存地址
#define PWM1_MAXAdd 0x9BFF
#define PWM1_MIDAdd PWM1_MAXAdd + 2
#define PWM1_MINAdd PWM1_MIDAdd + 2
#define PWM2_MIDAdd PWM1_MINAdd + 2

#define DeadZone  20
#define SwitchThreshold  200
#define PWM_MinRef     1000
#define PWM_CentreRef  1520  
#define PWM_MaxRef     2000  
/* ---------------------------------------Private macro -------------------------------------------- */

/* ---------------------------------------Private variables ---------------------------------------- */
typedef struct{
  uint8_t PWM_Ready;
  uint16_t PWM_Max[2];
  uint16_t PWM_Centre[2];
  uint16_t PWM_Min[2];
  uint16_t PWM_Data[2];
  uint8_t  PWM1_Status;
  uint8_t  PWM2_Status;
  uint8_t  Fail_Safe;
}PWM_CURRENTDATA;

typedef struct{
  int8_t Throttle_Dir;
  int16_t Throttle_Magnitude;
  int8_t Light_Status;
}CONTROL_DATA;

extern PWM_CURRENTDATA PWM_CurrentData;
extern CONTROL_DATA Control_Data;
/* ---------------------------------------Private function prototypes  ----------------------------- */
int16_t ABS(int16_t Data);
void PWM_Reveiver_Init(void);
void PWM1_Decode(void);
void PWM2_Decode(void);
void PWM_Process(void);

#endif




































