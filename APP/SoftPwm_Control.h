/*****************************************************************************************************
* @file    Motor_Control.h									     * 
* @author  WanBean										     *
* @version V1.0                                                         			     *
* @date    2020-07-25 									     *
* @brief                                               					             *
******************************************************************************************************/
#ifndef _SOFTPWM_CONTROL_H
#define _SOFTPWM_CONTROL_H

#define SoftPWMDeviceCnt   6//设备数量

#if SoftPWMDeviceCnt//如果为零，以下不编译
/* ---------------------------------------Include--------------------------------------------------- */
#include "stm8l10x.h"
#include "Motor_Control.h"
/* ---------------------------------------Private typedef ------------------------------------------ */
typedef struct{
    uint32_t start_cnt;
    uint16_t period;
    uint16_t pwm;
    void (*pin_output_h)(void);
    void (*pin_output_l)(void);
}pwm_control_typedef;

extern pwm_control_typedef SoftPwmDevice[SoftPWMDeviceCnt];
/* ---------------------------------------Private define ------------------------------------------- */
/* ---------------------------------------Private macro -------------------------------------------- */

#define PWM_DeviceTrunOnFuncArray {\
                                                Motor1OutH,\
                                                Motor2OutH,\
                                                Motor3OutH,\
                                                Motor4OutH,\
                                                Motor5OutH,\
                                                Motor6OutH,\
                                                }
    
#define PWM_DeviceTrunOffFuncArray {\
                                                Motor1OutL,\
                                                Motor2OutL,\
                                                Motor3OutL,\
                                                Motor4OutL,\
                                                Motor5OutL,\
                                                Motor6OutL,\
                                                 }
typedef enum 
{//取值必须连续
    Bucket_Motor=0,
    Arm_Motor=1,
    Boom_Motor=2,
    Swing_Motor=3,
    Left_Motor=4,
    Right_Motor=5,
}SOFTPWMDEVICE_TYPE;

/* ---------------------------------------Private variables ---------------------------------------- */

/* ---------------------------------------Private function prototypes  ----------------------------- */
extern void (*PWM_DeviceTurnOnFunc[SoftPWMDeviceCnt])(void);
extern void (*PWM_DeviceTurnOffFunc[SoftPWMDeviceCnt])(void);
void SoftPwmInit(void);
void soft_pwm(uint16_t sys_cnt);
#endif

#endif