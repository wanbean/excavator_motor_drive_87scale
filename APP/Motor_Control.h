/*****************************************************************************************************
* @file    Motor_Control.h									     * 
* @author  WanBean										     *
* @version V1.0                                                         			     *
* @date    2020-04-11    									     *
* @brief                                               					             *
******************************************************************************************************/
#ifndef _MOTOR_CONTROL_H
#define _MOTOR_CONTROL_H
/* ---------------------------------------Include--------------------------------------------------- */
#include "stm8l10x.h"
#include "Receiver_PPM.h"
/* ---------------------------------------Private typedef ------------------------------------------ */

/* ---------------------------------------Private define ------------------------------------------- */
/* ---------------------------------------Private macro -------------------------------------------- */
#define MOTOR_BackUp_Scale 3/4
//一号电机控制引脚
#define MOTOR1_PWM1_PORT GPIOB
#define MOTOR1_PWM1_PIN  GPIO_Pin_0
#define MOTOR1_PWM2_PORT GPIOB
#define MOTOR1_PWM2_PIN  GPIO_Pin_2

/* ---------------------------------------Private variables ---------------------------------------- */
extern uint16_t Motor_PWMPeriod;
/* ---------------------------------------Private function prototypes  ----------------------------- */

void Motor_Init(uint16_t Period);
void Motor1_PWM(int8_t Dir,int16_t Duty);


#endif