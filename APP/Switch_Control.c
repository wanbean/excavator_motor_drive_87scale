#include "Switch_Control.h"

/*输出--------------------------------------------------------------------------------*/
#if HoldDeviceCnt//如果为零，以下不编译
HOLDDef Device[HoldDeviceCnt];
/*函数指针数组，用于初始化各个设备的开关函数*/
void (*TurnOnFunc[HoldDeviceCnt])(void) = TrunOnFuncArray;
void (*TurnOffFunc[HoldDeviceCnt])(void) = TrunOffFuncArray;
    
/*-----------------------------
 -HOLD 初始化
-----------------------------*/
void OutHOLD_INIT(void)
{
    unsigned int i;
    for(i=0;i<HoldDeviceCnt;i++)
    {
        Device[i].IsWaitTime=0;
        Device[i].OnTime=0;
        Device[i].OffTime=0;
        Device[i].NowCnt=0;
        Device[i].Times=0;
        Device[i].NowTimes=0;
        Device[i].IsAlways=0;
        
        Device[i].TurnON=TurnOnFunc[i];
        Device[i].TurnOFF=TurnOffFunc[i];
    }
    
}
/*-----------------------------
 -让设备保持开一段时间后关闭
 -device 设备枚举名 
 -waittime 开之前等待时间 
 -ontime 开保持时间
-----------------------------*/
void OutHoldOnOff(DEVICE_TYPE device,unsigned char onoff)
{
    if(device>=HoldDeviceCnt||device<0) return;
    if(Device[device].TurnON==NULL) return;
    if(Device[device].TurnOFF==NULL) return;
    
    Device[device].NowCnt=0;
    if(onoff)
    {
        Device[device].TurnON();
    }
    else
    {
        Device[device].TurnOFF();
    }
}
/*-----------------------------
 -让设备保持开一段时间后关闭
 -device 设备枚举名 
 -waittime 开之前等待时间 
 -ontime 开保持时间
-----------------------------*/
void OutHoldAWhile(DEVICE_TYPE device,unsigned int waittime,unsigned int ontime)
{
    if(device>=HoldDeviceCnt||device<0) return;
    if(ontime==0) return;//检查变量
    if(Device[device].TurnON==NULL) return;
    if(Device[device].TurnOFF==NULL) return;
    
    Device[device].OnTime=0;
    Device[device].OffTime=0;
    Device[device].Times=0;
    Device[device].NowTimes=0;
    Device[device].IsAlways=0;
    
    if(waittime)
    {//如果waittime不为0
        Device[device].IsWaitTime=1;
        Device[device].OnTime=ontime;
        Device[device].NowCnt=waittime;
        Device[device].TurnOFF();
    }
    else
    {
        Device[device].IsWaitTime=0;
        Device[device].NowCnt=ontime;
        Device[device].TurnON();
    }
}

/*-----------------------------
 -让设备交替开关多次(或者一直交替)
 -device 设备枚举名 
 -times 交替次数 (为0或1时，一直保持开关状态;为偶数时开关次数为times/2;为奇数时最后保持开的状态)
 -waittime 开之前等待时间 
 -ontime 开保持时间
 -offtime 关保持时间
-----------------------------*/
void OutHoldTimes(DEVICE_TYPE device,unsigned int times,unsigned int waittime,unsigned int ontime,unsigned int offtime)
{
    if(device>=HoldDeviceCnt||device<0) return;
    if((ontime==0)||(offtime==0)) return;//检查变量
    if(Device[device].TurnON==NULL) return;
    if(Device[device].TurnOFF==NULL) return;
    
    if(times<2) 
    {
        Device[device].IsAlways=1;
    }
    else    
    {
        Device[device].IsAlways=0;
        Device[device].Times=times-2;
    }
    Device[device].NowTimes=0;
    Device[device].OnTime=ontime;
    Device[device].OffTime=offtime;
    
    if(waittime)
    {//如果waittime不为0
        Device[device].IsWaitTime=1;
        Device[device].NowCnt=waittime;
        Device[device].TurnOFF();
    }
    else
    {
        Device[device].IsWaitTime=0;
        Device[device].NowCnt=ontime;
        Device[device].TurnON();
    }
    
    
    
}


/*-----------------------------
 -变量计时并执行开关
-----------------------------*/
void OutHoldTimCnt(void)
{
    unsigned short i;
    for(i=0;i<HoldDeviceCnt;i++)
    {
        
        if(Device[i].TurnON==NULL) continue;
        if(Device[i].TurnOFF==NULL) continue;
        
        if(Device[i].NowCnt!=0)
        {//当前计数不为0，减1
            Device[i].NowCnt--;
            if(Device[i].NowCnt==0)
            {
                if(Device[i].IsWaitTime!=0)
                {//如果有等待时间，重新赋值ontime并退出
                    Device[i].IsWaitTime=0;
                    Device[i].NowCnt=Device[i].OnTime;
                    Device[i].TurnON();
                    continue;
                }
                
                /*判断当前该置高还是置低*/
                if(Device[i].NowTimes%2==0)
                    Device[i].TurnOFF();
                else
                    Device[i].TurnON();
                
                if((Device[i].NowTimes<Device[i].Times)||(Device[i].IsAlways!=0))
                {//如果还有次数或需要一直执行，继续
                    if(Device[i].NowTimes%2==0)
                        Device[i].NowCnt=Device[i].OffTime;
                    else
                        Device[i].NowCnt=Device[i].OnTime;
                    
                    Device[i].NowTimes++;
                    
                }
                else
                {//否则清零退出
                    Device[i].NowTimes=0;
                    Device[i].Times=0;
                }
                
            }
            
        }
    }
    
}
#endif

