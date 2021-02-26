/*****************************************************************************************************
* @file    Servo_Control.h									     * 
* @author  WanBean										     *
* @version V1.0                                                         			     *
* @date    2020-04-16    									     *
* @brief                                               					             *
******************************************************************************************************/
#ifndef _SERVO_CONTROL_H
#define _SERVO_CONTROL_H
/* ---------------------------------------Include--------------------------------------------------- */
#include "stm8l10x.h"
#include "stm8l10x_gpio.h"
#include "Receiver_PPM.h"
/* ---------------------------------------Private typedef ------------------------------------------ */

/* ---------------------------------------Private define ------------------------------------------- */

/* ---------------------------------------Private macro -------------------------------------------- */
//¶æ»ú1¿ØÖÆÒý½ÅÓ³Éä
#define SERVO1_PWM_PORT GPIOB
#define SERVO1_PWM_PIN  GPIO_Pin_1
//¶æ»ú2¿ØÖÆÒý½ÅÓ³Éä
#define SERVO2_PWM_PORT GPIOD
#define SERVO2_PWM_PIN  GPIO_Pin_0
/* ---------------------------------------Private variables ---------------------------------------- */

/* ---------------------------------------Private function prototypes  ----------------------------- */
void Servo_Init(void);
void Servo1_PWM(uint32_t Count,uint16_t Pulse);
void Servo2_PWM(uint32_t Count,uint16_t Pulse);


#endif