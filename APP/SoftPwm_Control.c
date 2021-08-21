#include "SoftPwm_Control.h"
#include <stdio.h>

#if SoftPWMDeviceCnt//如果为零，以下不编译
pwm_control_typedef SoftPwmDevice[SoftPWMDeviceCnt];

/*函数指针数组，用于初始化各个设备的开关函数*/
void (*PWM_DeviceTurnOnFunc[SoftPWMDeviceCnt])(void) = PWM_DeviceTrunOnFuncArray;
void (*PWM_DeviceTurnOffFunc[SoftPWMDeviceCnt])(void) = PWM_DeviceTrunOffFuncArray;



void SoftPwmInit(void)
{
    for(uint8_t i=0;i<SoftPWMDeviceCnt;i++)
    {
        SoftPwmDevice[i].start_cnt = 0;
        SoftPwmDevice[i].period = 100;
        SoftPwmDevice[i].pwm = 0;
        SoftPwmDevice[i].pin_output_h = PWM_DeviceTurnOnFunc[i];
        SoftPwmDevice[i].pin_output_l = PWM_DeviceTurnOffFunc[i];
    }
}
float debug1,debug2,debug3,debug4;
void soft_pwm(uint16_t sys_cnt)
{
    static uint32_t sys_cnt_now = 0;
    static uint32_t on_time = 0;
    static float pwm_float = 0;
    uint8_t i = 0;
    for(i = 0;i < SoftPWMDeviceCnt;i++)
    {
      sys_cnt_now = 0;
      
      pwm_float = (float)((float)SoftPwmDevice[i].pwm / (float)SoftPwmDevice[i].period);
      on_time = pwm_float * 100;
      //on_time = (float)(((float)SoftPwmDevice[i].period * (float)SoftPwmDevice[i].pwm) / 100);
      //on_time = SoftPwmDevice[i].pwm;
      //start_cnt = sys_cnt;
      if(sys_cnt < SoftPwmDevice[i].start_cnt)
      {
          sys_cnt_now = sys_cnt + 60000;
      }
      else
      {
          sys_cnt_now = sys_cnt;
      }
      if (SoftPwmDevice[i].start_cnt == 0)
      {
          SoftPwmDevice[i].start_cnt = sys_cnt;
      }
      if(SoftPwmDevice[i].pwm == 0)
      {
          SoftPwmDevice[i].pin_output_l();
      }
      else if(SoftPwmDevice[i].pwm == 100)
      {
          SoftPwmDevice[i].pin_output_h();
      }
      else
      {
          if (sys_cnt_now < (SoftPwmDevice[i].start_cnt + on_time))
          {
              SoftPwmDevice[i].pin_output_h();
          }
          else
          {
              SoftPwmDevice[i].pin_output_l();
          }
      }

      if(sys_cnt >= ((SoftPwmDevice[i].start_cnt + SoftPwmDevice[i].period) - 1))
      {
          SoftPwmDevice[i].start_cnt = sys_cnt + 1;
      }
      else if(sys_cnt < SoftPwmDevice[i].start_cnt)
      {
        if(sys_cnt_now >= ((SoftPwmDevice[i].start_cnt + SoftPwmDevice[i].period) - 1))
        {
          SoftPwmDevice[i].start_cnt = sys_cnt +1;
        }
        
      }
    }
}

#endif