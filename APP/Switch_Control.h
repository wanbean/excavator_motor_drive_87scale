#ifndef _SWITCH_CONTROL_H
#define _SWITCH_CONTROL_H
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "stm8l10x.h"

#define HoldDeviceCnt   2//设备数量

#if HoldDeviceCnt//如果为零，以下不编译
/*OUTHOLD用户接口----------------------------------------------------------------------*/
void OutHoldTimCnt(void);//在定时器中断中调用此函数

/*OUTHOLD用户配置-----------------------------------------------------------------*/
void BoomLampOn(void)
{
  GPIO_ResetBits(GPIOA,GPIO_Pin_2);
}
void BoomLampOff(void)
{
  GPIO_SetBits(GPIOA,GPIO_Pin_2);
}
void ShellLampOn(void)
{
  GPIO_ResetBits(GPIOA,GPIO_Pin_6);
}
void ShellLampOff(void)
{
  GPIO_SetBits(GPIOA,GPIO_Pin_6);
}

#define TrunOnFuncArray {\
                                                BoomLampOn,\
                                                ShellLampOn,\
                                                }
    
#define TrunOffFuncArray {\
                                                BoomLampOff,\
                                                ShellLampOff,\
                                                 }
typedef enum 
{//取值必须连续
    Boom_Lamp=0,
    Shell_Lamp=1,
}DEVICE_TYPE;
/*---------------------------------------------------------------------------------------------------------------------------------------------*/
/*HOLD----------------------------*/
typedef struct
{
    unsigned char IsWaitTime;//是否有等待时间
    unsigned int OnTime;//开启时间
    unsigned int OffTime;//关闭时间
    unsigned int NowCnt;//当前cnt
    
    unsigned char Times;//开关次数,一开一关算两次
    unsigned char NowTimes;//当前的次数
    unsigned char IsAlways;//是否一直开关
    
    void (*TurnON)(void);//打开的函数指针
    void (*TurnOFF)(void);//关闭的函数指针
}HOLDDef;
typedef struct
{
    unsigned int TimeCnt;//时间计数
    unsigned int isFirst;//是否第一次进入函数
    unsigned char (*CheckInput)(void);//读取的函数指针
}CheckDef;
void OutHOLD_INIT(void);
void OutHoldOnOff(DEVICE_TYPE device,unsigned char onoff);
void OutHoldAWhile(DEVICE_TYPE device,unsigned int waittime,unsigned int ontime);
void OutHoldTimes(DEVICE_TYPE device,unsigned int times,unsigned int waittime,unsigned int ontime,unsigned int offtime);
#endif

#endif

