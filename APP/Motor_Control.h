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
#define MOTOR1_DIR_PORT GPIOC
#define MOTOR1_DIR_PIN  GPIO_Pin_3
#define MOTOR1_PWM_PORT GPIOC
#define MOTOR1_PWM_PIN  GPIO_Pin_4
//二号电机控制引脚
#define MOTOR2_DIR_PORT GPIOC
#define MOTOR2_DIR_PIN  GPIO_Pin_1
#define MOTOR2_PWM_PORT GPIOC
#define MOTOR2_PWM_PIN  GPIO_Pin_2
//三号电机控制引脚
#define MOTOR3_DIR_PORT GPIOB
#define MOTOR3_DIR_PIN  GPIO_Pin_7
#define MOTOR3_PWM_PORT GPIOC
#define MOTOR3_PWM_PIN  GPIO_Pin_0
//四号电机控制引脚
#define MOTOR4_DIR_PORT GPIOB
#define MOTOR4_DIR_PIN  GPIO_Pin_5
#define MOTOR4_PWM_PORT GPIOB
#define MOTOR4_PWM_PIN  GPIO_Pin_6
//五号电机控制引脚
#define MOTOR5_DIR_PORT GPIOB
#define MOTOR5_DIR_PIN  GPIO_Pin_3
#define MOTOR5_PWM_PORT GPIOB
#define MOTOR5_PWM_PIN  GPIO_Pin_4
//六号电机控制引脚
#define MOTOR6_DIR_PORT GPIOB
#define MOTOR6_DIR_PIN  GPIO_Pin_1
#define MOTOR6_PWM_PORT GPIOB
#define MOTOR6_PWM_PIN  GPIO_Pin_2

/* ---------------------------------------Private variables ---------------------------------------- */

/* ---------------------------------------Private function prototypes  ----------------------------- */
void Motor1OutH(void);
void Motor2OutH(void);
void Motor3OutH(void);
void Motor4OutH(void);
void Motor5OutH(void);
void Motor6OutH(void);
void Motor1OutL(void);
void Motor2OutL(void);
void Motor3OutL(void);
void Motor4OutL(void);
void Motor5OutL(void);
void Motor6OutL(void);
void Motor_Init(uint16_t Period);
void Motor_Process(uint16_t sys_cnt);


#endif