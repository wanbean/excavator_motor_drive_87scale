/*****************************************************************************************************
* @file    Light_Control.h									                                         * 
* @author  WanBean										                                             *
* @version V1.0                                                         			                 *
* @date    2020-04-15    									                                         *
* @brief                                               					                             *
******************************************************************************************************/
#ifndef _LIGHT_CONTROL_H
#define _LIGHT_CONTROL_H
/* ---------------------------------------Include--------------------------------------------------- */
#include "stm8l10x.h"
#include "Receiver_PPM.h"
#include "stm8l10x_tim4.h"
/* ---------------------------------------Private typedef ------------------------------------------ */

/* ---------------------------------------Private define ------------------------------------------- */


#define Head_Lamp_Bright 25    //近光灯亮度

#define BackUp_Lamp_Bright 50  //倒车灯正常亮度

#define Stop_Lamp_Bright 25    //刹车灯正常亮度
#define Stop_Lamp_DelayTime 50 //刹车灯高亮时间系数 20ms一个数
#define Stop_Lamp_Smoothing 1000 //刹车灯滤波系数,主函数计数

#define Turning_Signal_Lamp_DelayTime 50 //转向灯闪烁系数 20ms一个数

#define Police_Lamp_M2_DelayTime 10 //警灯闪烁系数 20ms一个数
#define Police_Lamp_M3_DelayTime 84 //警灯闪烁系数 20ms一个数
/* ---------------------------------------Private macro -------------------------------------------- */
//大灯控制引脚
#define Head_Lamp_PORT                  GPIOB
#define Head_Lamp_PIN                   GPIO_Pin_3
//倒车灯控制引脚
#define BackUp_Lamp_PORT                GPIOB
#define BackUp_Lamp_PIN                 GPIO_Pin_4
//刹车灯控制引脚
#define Stop_Lamp_PORT                  GPIOB
#define Stop_Lamp_PIN                   GPIO_Pin_5
//左转向灯控制引脚
#define Turning_Signal_Lamp_L_PORT      GPIOB
#define Turning_Signal_Lamp_L_PIN       GPIO_Pin_6
//右转向灯控制引脚
#define Turning_Signal_Lamp_R_PORT      GPIOB
#define Turning_Signal_Lamp_R_PIN       GPIO_Pin_7
//红色警灯灯控制引脚
#define Police_Lamp_R_PORT              GPIOC
#define Police_Lamp_R_PIN               GPIO_Pin_2
//蓝色警灯灯控制引脚
#define Police_Lamp_B_PORT              GPIOC
#define Police_Lamp_B_PIN               GPIO_Pin_3
/* ---------------------------------------Private variables ---------------------------------------- */
typedef enum 
{
  Head_Lamp_OFF = 0,
  Head_Lamp_LB,
  Head_Lamp_HB
}HEAD_LAMP_MODE;

typedef enum 
{
  Stop_Lamp_OFF = 0,
  Stop_Lamp_Normal,
  Stop_Lamp_Brake,
}STOP_LAMP_MODE;

typedef enum 
{
  Backup_Lamp_OFF = 0,
  Backup_Lamp_Normal,
}BACKUP_LAMP_MODE;

typedef enum 
{
  Turning_Signal_OFF = 0,
  Turning_Signal_Turn_Left,
  Turning_Signal_Turn_Right,
  Turning_Signal_Beacon,
}TURNING_SIGNAL_LAMP_MODE;

typedef enum
{
  Police_Lamp_OFF = 0,
  Police_Lamp_M1,
  Police_Lamp_M2,
  Police_Lamp_M3
}POLICE_LAMP_MODE;


typedef struct
{
  HEAD_LAMP_MODE Head_Lamp_Mode;
  STOP_LAMP_MODE Stop_Lamp_Mode;
  BACKUP_LAMP_MODE BackUp_Lame_Mode;
  TURNING_SIGNAL_LAMP_MODE Turning_Signal_Lamp_Mode;
  POLICE_LAMP_MODE Police_Lamp_Mode;
}LAMP_MODE;

extern LAMP_MODE Lamp_Mode;
extern uint8_t Stop_Lamp_Delay;
extern uint8_t Turning_Signal_Lamp_Delay;
extern uint8_t Plice_Lamp_Delay;
/* ---------------------------------------Private function prototypes  ----------------------------- */
void LightGPIO_Init(void);
void Head_Lamp_Control(HEAD_LAMP_MODE Mode);
void Stop_Lamp_Control(STOP_LAMP_MODE *Mode);
void Turning_Signal_Lamp_Control(TURNING_SIGNAL_LAMP_MODE Mode);
void Plice_Lamp_Control(POLICE_LAMP_MODE *Mode);
void BackUp_Lamp_Control(BACKUP_LAMP_MODE Switch);
void Lamp_Blink_Single(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin);
void Lamp_Blink_All(void);

#endif