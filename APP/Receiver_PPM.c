/*****************************************************************************************************
* @file    Receiver_PPM.c									     * 
* @author  WanBean                                                                                   *
* @version V1.0                                                         			     *
* @date    2020-03-27                                                                                *
* @brief                                               					             *
******************************************************************************************************/
/* ---------------------------------------Include--------------------------------------------------- */
#include "Receiver_PPM.h"
#include "SoftPwm_Control.h"
#include <stdlib.h>

/* ---------------------------------------Private typedef ------------------------------------------ */

/* ---------------------------------------Private define ------------------------------------------- */
/* ---------------------------------------Private macro -------------------------------------------- */

/* ---------------------------------------Private variables ---------------------------------------- */

/* ---------------------------------------Private function prototypes  ----------------------------- */
PWM_CURRENTDATA PWM_CurrentData = {.Fail_Safe = 0,
                                   .PWM_Max = {PWM_MaxRef,PWM_MaxRef,PWM_MaxRef,PWM_MaxRef,PWM_MaxRef,PWM_MaxRef,PWM_MaxRef,PWM_MaxRef},
                                   .PWM_Mid = {PWM_CentreRef,PWM_CentreRef,PWM_CentreRef,PWM_CentreRef,PWM_CentreRef,PWM_CentreRef,PWM_CentreRef,PWM_CentreRef},
                                   .PWM_Min = {PWM_MinRef,PWM_MinRef,PWM_MinRef,PWM_MinRef,PWM_MinRef,PWM_MinRef,PWM_MinRef,PWM_MinRef},
                                   .PWM_Data = {PWM_CentreRef,PWM_CentreRef,PWM_CentreRef,PWM_CentreRef,PWM_CentreRef,PWM_CentreRef,PWM_CentreRef,PWM_CentreRef},
                                   .PWM_Status = {8,7,6,5,4,3,2,1},};
CONTROL_DATA Control_Data = {.Magnitude = {0,0,0,0,0,0},
                            };

/**
  * @brief  PPM捕获初始化
  * @param  None
  * @retval None
  */
void PPM_Reveiver_Init(void)
{
  //使能TIM3外设时钟门控
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);
  TIM3_DeInit();
  //TIM3时基设置，1分频，向上计数，
  TIM3_TimeBaseInit(TIM3_Prescaler_8, TIM3_CounterMode_Up,40000);
  
  //自动预装载使能
  TIM3_ARRPreloadConfig(ENABLE);

  //使能TIM2输出
  TIM3_ITConfig(TIM3_IT_Update,ENABLE);
  //TIM3计数器使能
  TIM3_Cmd(ENABLE);
  
  //清捕获标志
  TIM3_ClearFlag(TIM3_FLAG_Update);
  
  /* 引脚外部中断方式 */
  GPIO_Init(PPM_Input_PORT,PPM_Input_PIN,GPIO_Mode_In_PU_IT);
  
  EXTI_DeInit (); //恢复中断的所有设置 
  EXTI_SetPinSensitivity (EXTI_Pin_0,EXTI_Trigger_Rising);//外部中断1，上升沿触发，中断向量号11
}

  /**
  * @brief  读取Flash中的PWM值
  * @param  None
  * @retval None
  */
static void Read_Flash_PWM(void)
{
  uint8_t Val_H = 0x0,Val_L = 0x0;
  
  FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);//标准编程时间
  FLASH_Unlock(FLASH_MemType_Program);  //解除保护
  while (FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET);
  FLASH_Unlock(FLASH_MemType_Data);  //解除保护
  while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET);
  
  /* 检查Flash保存值 */
  //读取PWM1_Min Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM1_MINAdd);
  Val_L = FLASH_ReadByte(PWM1_MINAdd + 1);
  PWM_CurrentData.PWM_Min[0] = (uint16_t)(Val_H << 8) + Val_L;

  //读取PWM1_Mid Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM1_MIDAdd);
  Val_L = FLASH_ReadByte(PWM1_MIDAdd + 1);
  PWM_CurrentData.PWM_Mid[0] = (uint16_t)(Val_H << 8) + Val_L;
  
  //读取PWM1_Max Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM1_MAXAdd);
  Val_L = FLASH_ReadByte(PWM1_MAXAdd + 1);
  PWM_CurrentData.PWM_Max[0] = (uint16_t)(Val_H << 8) + Val_L;
  
  //读取PWM1_Min Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM2_MINAdd);
  Val_L = FLASH_ReadByte(PWM2_MINAdd + 1);
  PWM_CurrentData.PWM_Min[1] = (uint16_t)(Val_H << 8) + Val_L;

  //读取PWM1_Mid Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM2_MIDAdd);
  Val_L = FLASH_ReadByte(PWM2_MIDAdd + 1);
  PWM_CurrentData.PWM_Mid[1] = (uint16_t)(Val_H << 8) + Val_L;
  
  //读取PWM1_Max Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM2_MAXAdd);
  Val_L = FLASH_ReadByte(PWM2_MAXAdd + 1);
  PWM_CurrentData.PWM_Max[1] = (uint16_t)(Val_H << 8) + Val_L;

  //读取PWM1_Min Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM3_MINAdd);
  Val_L = FLASH_ReadByte(PWM3_MINAdd + 1);
  PWM_CurrentData.PWM_Min[2] = (uint16_t)(Val_H << 8) + Val_L;

  //读取PWM1_Mid Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM3_MIDAdd);
  Val_L = FLASH_ReadByte(PWM3_MIDAdd + 1);
  PWM_CurrentData.PWM_Mid[2] = (uint16_t)(Val_H << 8) + Val_L;
  
  //读取PWM1_Max Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM3_MAXAdd);
  Val_L = FLASH_ReadByte(PWM3_MAXAdd + 1);
  PWM_CurrentData.PWM_Max[2] = (uint16_t)(Val_H << 8) + Val_L;

  //读取PWM1_Min Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM4_MINAdd);
  Val_L = FLASH_ReadByte(PWM4_MINAdd + 1);
  PWM_CurrentData.PWM_Min[3] = (uint16_t)(Val_H << 8) + Val_L;

  //读取PWM1_Mid Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM4_MIDAdd);
  Val_L = FLASH_ReadByte(PWM4_MIDAdd + 1);
  PWM_CurrentData.PWM_Mid[3] = (uint16_t)(Val_H << 8) + Val_L;
  
  //读取PWM1_Max Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM4_MAXAdd);
  Val_L = FLASH_ReadByte(PWM4_MAXAdd + 1);
  PWM_CurrentData.PWM_Max[3] = (uint16_t)(Val_H << 8) + Val_L;

  //读取PWM1_Min Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM5_MINAdd);
  Val_L = FLASH_ReadByte(PWM5_MINAdd + 1);
  PWM_CurrentData.PWM_Min[4] = (uint16_t)(Val_H << 8) + Val_L;

  //读取PWM1_Mid Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM5_MIDAdd);
  Val_L = FLASH_ReadByte(PWM5_MIDAdd + 1);
  PWM_CurrentData.PWM_Mid[4] = (uint16_t)(Val_H << 8) + Val_L;
  
  //读取PWM1_Max Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM5_MAXAdd);
  Val_L = FLASH_ReadByte(PWM5_MAXAdd + 1);
  PWM_CurrentData.PWM_Max[4] = (uint16_t)(Val_H << 8) + Val_L;

  //读取PWM1_Min Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM6_MINAdd);
  Val_L = FLASH_ReadByte(PWM6_MINAdd + 1);
  PWM_CurrentData.PWM_Min[5] = (uint16_t)(Val_H << 8) + Val_L;

  //读取PWM1_Mid Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM6_MIDAdd);
  Val_L = FLASH_ReadByte(PWM6_MIDAdd + 1);
  PWM_CurrentData.PWM_Mid[5] = (uint16_t)(Val_H << 8) + Val_L;
  
  //读取PWM1_Max Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM6_MAXAdd);
  Val_L = FLASH_ReadByte(PWM6_MAXAdd + 1);
  PWM_CurrentData.PWM_Max[5] = (uint16_t)(Val_H << 8) + Val_L;

  //读取PWM1_Min Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM7_MINAdd);
  Val_L = FLASH_ReadByte(PWM7_MINAdd + 1);
  PWM_CurrentData.PWM_Min[6] = (uint16_t)(Val_H << 8) + Val_L;

  //读取PWM1_Mid Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM7_MIDAdd);
  Val_L = FLASH_ReadByte(PWM7_MIDAdd + 1);
  PWM_CurrentData.PWM_Mid[6] = (uint16_t)(Val_H << 8) + Val_L;
  
  //读取PWM1_Max Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM7_MAXAdd);
  Val_L = FLASH_ReadByte(PWM7_MAXAdd + 1);
  PWM_CurrentData.PWM_Max[6] = (uint16_t)(Val_H << 8) + Val_L;

  //读取PWM1_Min Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM8_MINAdd);
  Val_L = FLASH_ReadByte(PWM8_MINAdd + 1);
  PWM_CurrentData.PWM_Min[7] = (uint16_t)(Val_H << 8) + Val_L;

  //读取PWM1_Mid Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM8_MIDAdd);
  Val_L = FLASH_ReadByte(PWM8_MIDAdd + 1);
  PWM_CurrentData.PWM_Mid[7] = (uint16_t)(Val_H << 8) + Val_L;
  
  //读取PWM1_Max Flash中保存的数据
  Val_H = FLASH_ReadByte(PWM8_MAXAdd);
  Val_L = FLASH_ReadByte(PWM8_MAXAdd + 1);
  PWM_CurrentData.PWM_Max[7] = (uint16_t)(Val_H << 8) + Val_L;
}

/**
  * @brief  PWM值写入Flash
  * @param  Address:写入地址
  * @param  Data：写入数据
  * @retval None
  */
static void Write_Flash_PWM(uint16_t Address,uint16_t Data)
{
  //写最大值
  FLASH_ProgramByte(Address,BYTE_1(Data));
  while (FLASH_GetFlagStatus(FLASH_FLAG_EOP) == RESET);
  FLASH_ProgramByte(Address + 1,BYTE_0(Data));
  while (FLASH_GetFlagStatus(FLASH_FLAG_EOP) == RESET);
}

#if 0
/**
  * @brief  PWM均值采集
  * @param  PWM_Current:当前采集的PWM值
  * @retval None
  */
static void PWM_AVG_Calculate(uint16_t *PWM_AVG,uint16_t *PWM_Current)
{
  static uint8_t CentreCaptureStatus = 0;//0-4:采集 5:计算平均值 6:ready
  static uint16_t PWM_Temp,Last_PWM_Current;
  //摇杆需连续保持不动1S
  if(abs(Last_PWM_Current - *PWM_Current) < (DeadZone/2))
  {
    CentreCaptureStatus ++;
  }
  else
  {
    CentreCaptureStatus = 0;
    PWM_Temp = 0;
  }
  //摇杆不动2S开始采集
  if(CentreCaptureStatus > 40)
  {
    PWM_Temp += *PWM_Current;
  }
  //采集10次
  if(CentreCaptureStatus == 50)
  {
    *PWM_AVG = PWM_Temp/10;
    CentreCaptureStatus = 0;
    PWM_Temp = 0;
    Last_PWM_Current = 0;
    //检查最大值 如不合法,重新采集
    if(PWM_CurrentData.PWM_Ready % 3 == 0)
    {
      //不够大 或者超过2000
      if((*PWM_AVG < PWM_CentreRef + SwitchThreshold)||(*PWM_AVG > PWM_MaxRef))
      {
        return;
      }
      //合法,保存到Flash
      else
      {
        Write_Flash_PWM(*PWM_AVG);
      }
    }
    //检查中值 如不合法,重新采集
    else if(PWM_CurrentData.PWM_Ready % 3 == 1)
    {
      if(ABS(*PWM_AVG - PWM_CentreRef) > 20)
      {
        return;
      }
      //合法,保存到Flash
      else
      {
        //写入油门中值
        Write_Flash_PWM1Mid(*PWM_AVG);
        //写入灯组中值
        
        Write_Flash_PWM2Mid(PWM_CurrentData.PWM_Data[1]);
      }
    }
    //检查最小值 如不合法,重新采集
    else if(PWM_CurrentData.PWM_Ready % 3 == 2)
    {
      //不够小 或者小于2000
      if((*PWM_AVG > PWM_CentreRef - SwitchThreshold)||(*PWM_AVG < PWM_MinRef))
      {
        return;
      }
      //合法,保存到Flash
      else
      {
        Write_Flash_PWM1Min(*PWM_AVG);
      }
    }
    PWM_CurrentData.PWM_Ready ++;
  }
  Last_PWM_Current = *PWM_Current;
}


/**
  * @brief  上电PWM最小值 中值 最大值校准
  * @param  None
  * @retval None
  */
static void PWM_Calibration(uint16_t *PWM_Current)
{
  static uint8_t CentreCaptureStatus = 0;
  static uint16_t Last_PWM_Current = 0;
  
  /* 检查Flash保存值 */
  Read_Flash_PWM();
  //判断哪个值需要校准
  if(PWM_CurrentData.PWM_Min[0] == 0)
  {
    PWM_CurrentData.PWM_Ready = 2;//校准最小值
  }
  if(PWM_CurrentData.PWM_Mid[0] == 0)
  {
    PWM_CurrentData.PWM_Ready = 1;//校准中值
  }
  if(PWM_CurrentData.PWM_Max[0] == 0)
  {
    PWM_CurrentData.PWM_Ready = 0;//校准最大值
  }
  /* 需要校准 */
  if(PWM_CurrentData.PWM_Ready != 24)
  {
    return;
  }
  /* 检查Flash保存值 */
  else//Flash中的值正确
  {
    /* 判断油门位置 */
    //摇杆需连续保持不动1S
    if(ABS(Last_PWM_Current - *PWM_Current) < 2 * DeadZone)
    {
      CentreCaptureStatus ++;
    }
    else
    {
      CentreCaptureStatus = 0;
    }
    //摇杆不动1S开始判断油门位置
    if(CentreCaptureStatus == 25)
    {
      //油门最大 重新校准
      if(*PWM_Current > PWM_CurrentData.PWM_Mid[0] + SwitchThreshold)
      {
        PWM_CurrentData.PWM_Ready = 0;
      }
      //油门在中位 正常执行
      else if(ABS(*PWM_Current - PWM_CurrentData.PWM_Mid[0]) < (2 * DeadZone))
      {
        PWM_CurrentData.PWM_Ready = 4;
      }
    }
    Last_PWM_Current = *PWM_Current;
  }
  /* 判断油门位置 */
}

/**
  * @brief  油门通道解析
  * @param  Throttle_Data:接收机油门PWM值
  * @retval None
  */
static void Throttle_Analysis(uint16_t Throttle_Data)
{
  static uint8_t LastThrottle_Dir;
  //前进
  if(Throttle_Data > (PWM_CurrentData.PWM_Mid[0] + DeadZone))
  {
    Control_Data.Throttle_Dir = 1;
    //关闭倒车灯
    Lamp_Mode.BackUp_Lame_Mode = Backup_Lamp_OFF;
    //选择刹车灯模式
    if(Lamp_Mode.Head_Lamp_Mode != Head_Lamp_OFF)
    {
      Lamp_Mode.Stop_Lamp_Mode = Stop_Lamp_Normal;
    }
    else
    {
      Lamp_Mode.Stop_Lamp_Mode = Stop_Lamp_OFF;
    }
    Stop_Lamp_Delay = 0;
  }
  //倒车
  else if(Throttle_Data < (PWM_CurrentData.PWM_Mid[0] - DeadZone))
  {
    Control_Data.Throttle_Dir = -1;
    //开启倒车灯
    Lamp_Mode.BackUp_Lame_Mode = Backup_Lamp_Normal;
    //选择刹车灯模式
    if(Lamp_Mode.Head_Lamp_Mode != Head_Lamp_OFF)
    {
      Lamp_Mode.Stop_Lamp_Mode = Stop_Lamp_Normal;
    }
    else
    {
      Lamp_Mode.Stop_Lamp_Mode = Stop_Lamp_OFF;
    }
    Stop_Lamp_Delay = 0;
  }
  else//刹车状态
  {
    Control_Data.Throttle_Dir = 0;
    //关闭倒车灯
    Lamp_Mode.BackUp_Lame_Mode = Backup_Lamp_OFF;
    //选择刹车灯模式
    if((Lamp_Mode.Head_Lamp_Mode != Head_Lamp_OFF)&&(Lamp_Mode.Stop_Lamp_Mode != Stop_Lamp_Brake))
    {
      Lamp_Mode.Stop_Lamp_Mode = Stop_Lamp_Normal;
    }
    if(LastThrottle_Dir != 0)
    {
      //刹车灯标志置位
      Lamp_Mode.Stop_Lamp_Mode = Stop_Lamp_Brake;     
    }

  }
  LastThrottle_Dir = Control_Data.Throttle_Dir;
  Control_Data.Throttle_Magnitude = ABS(Throttle_Data - PWM_CurrentData.PWM_Mid[0]);
}

/**
  * @brief  灯组通道解析
  * @param  Light_Data:接收机灯组PWM值
  * @retval None
  */
static void Light_Analysis(uint16_t Light_Data)
{
  static uint8_t SwitchH_Click,SwitchL_Click;
  static uint16_t LightPWMH_Count,LightPWMM_Count,LightPWML_Count;
  //三段开关向上
  if(Light_Data > PWM_CurrentData.PWM_Mid[1] + SwitchThreshold)
  {
    Control_Data.Light_Status = 1;
  }
  //三段开关向下
  else if(Light_Data < PWM_CurrentData.PWM_Mid[1] - SwitchThreshold)
  {
    Control_Data.Light_Status = -1;
  }
  //三段开关中位
  else
  {
    Control_Data.Light_Status = 0;
  }
  
  if(Control_Data.Light_Status == -1)
  {
    LightPWMH_Count = 0;
    LightPWMM_Count = 0;
    LightPWML_Count ++;
    if(SwitchL_Click == 0) SwitchL_Click = 1;
    if(SwitchL_Click == 2) SwitchL_Click = 3;
    //向下长按
    if(LightPWML_Count > 25)//长按500ms,打转向
    {
      Lamp_Mode.Turning_Signal_Lamp_Mode = Turning_Signal_Turn_Left;
      SwitchL_Click = 0;
      //计数限制
      LightPWML_Count = 26;
    }
  }
  else if(Control_Data.Light_Status == 0)
  {
    LightPWMH_Count = 0;
    LightPWMM_Count ++;
    LightPWML_Count = 0;
    if(Lamp_Mode.Turning_Signal_Lamp_Mode != Turning_Signal_Beacon)
    {
      Lamp_Mode.Turning_Signal_Lamp_Mode = Turning_Signal_OFF;
    }
    //单击
    if(LightPWMM_Count > 25)
    {
      //下 警示灯
      if(SwitchL_Click == 2)
      {
        if(Lamp_Mode.Turning_Signal_Lamp_Mode != Turning_Signal_Beacon)
        {
          Lamp_Mode.Turning_Signal_Lamp_Mode = Turning_Signal_Beacon;
        }
        else
        {
          Lamp_Mode.Turning_Signal_Lamp_Mode = Turning_Signal_OFF;
        }
      }
      //上 大灯
      if(SwitchH_Click == 2)
      {
        if(Lamp_Mode.Head_Lamp_Mode == Head_Lamp_OFF)
        {
          Lamp_Mode.Head_Lamp_Mode = Head_Lamp_LB;
//         Lamp_Mode.Stop_Lamp_Mode = Stop_Lamp_Normal;
        }
        else if(Lamp_Mode.Head_Lamp_Mode == Head_Lamp_LB)
        {
          Lamp_Mode.Head_Lamp_Mode = Head_Lamp_HB;
//          Lamp_Mode.Stop_Lamp_Mode = Stop_Lamp_Normal;
        }
        else
        {
          Lamp_Mode.Head_Lamp_Mode = Head_Lamp_OFF;
          Lamp_Mode.Stop_Lamp_Mode = Stop_Lamp_OFF;
        }
      }
      SwitchL_Click = 0;
      SwitchH_Click = 0;
    }
    if(SwitchL_Click == 1) SwitchL_Click = 2;
    if(SwitchH_Click == 1) SwitchH_Click = 2;
    //双击
    if(SwitchL_Click == 3)
    {
      SwitchL_Click = 0;
      //警灯模式 循环切换
      if(Lamp_Mode.Police_Lamp_Mode == Police_Lamp_OFF)
      {
        Lamp_Mode.Police_Lamp_Mode = Police_Lamp_M1;
      }
      else if(Lamp_Mode.Police_Lamp_Mode == Police_Lamp_M1)
      {
        Lamp_Mode.Police_Lamp_Mode = Police_Lamp_M2;
      }
      else if(Lamp_Mode.Police_Lamp_Mode == Police_Lamp_M2)
      {
        Lamp_Mode.Police_Lamp_Mode = Police_Lamp_M3;
      }
      else if(Lamp_Mode.Police_Lamp_Mode == Police_Lamp_M3)
      {
        Lamp_Mode.Police_Lamp_Mode = Police_Lamp_OFF;
      }
    }
    //双击
    if(SwitchH_Click == 3)
    {
      SwitchH_Click = 0;
    }
    
    //限制计数
    if(LightPWMM_Count > 25)
    {
      LightPWMM_Count = 26;
    }
  }
  else
  {
    LightPWMH_Count ++;
    LightPWMM_Count = 0;
    LightPWML_Count = 0;
    if(SwitchH_Click == 0) SwitchH_Click = 1;
    if(SwitchH_Click == 2) SwitchH_Click = 3;
    //向上长按
    if(LightPWMH_Count > 25)//长按1000ms,打转向
    {
      Lamp_Mode.Turning_Signal_Lamp_Mode = Turning_Signal_Turn_Right;
      SwitchH_Click = 0;
      //计数限制
      LightPWMH_Count = 26;
    }
  }
}

/**
  * @brief  PWM数据处理
  * @param  None
  * @retval None
  */
void PWM_Process(void)
{
  PWM_CurrentData.Fail_Safe = 0;
  switch (PWM_CurrentData.PWM_Ready)
  {
    case 0://采集最大值
      PWM_AVG_Calculate(&PWM_CurrentData.PWM_Max[0],&PWM_CurrentData.PWM_Data[Throttle_Channel]);
      break;
    case 1://采集中值
      PWM_AVG_Calculate(&PWM_CurrentData.PWM_Mid[0],&PWM_CurrentData.PWM_Data[Throttle_Channel]);
      break;
    case 2://采集最小值
      PWM_AVG_Calculate(&PWM_CurrentData.PWM_Min[0],&PWM_CurrentData.PWM_Data[Throttle_Channel]);
      break;
    case 3://检查Flash 判断油门位置
      PWM_Calibration(&PWM_CurrentData.PWM_Data[Throttle_Channel]);
      //取较小值作为定时器周期
      if((PWM_CurrentData.PWM_Max[0] - PWM_CurrentData.PWM_Mid[0]) < (PWM_CurrentData.PWM_Mid[0] - PWM_CurrentData.PWM_Min[0]))
      {
        Motor_PWMPeriod = ABS(PWM_CurrentData.PWM_Max[0] - PWM_CurrentData.PWM_Mid[0]);
      }
      else
      {
        Motor_PWMPeriod = ABS(PWM_CurrentData.PWM_Min[0] - PWM_CurrentData.PWM_Mid[0]);
      }
      Motor_Init(Motor_PWMPeriod - 1);
      break;
    case 4://正常执行
      //油门状态转化
      Throttle_Analysis(PWM_CurrentData.PWM_Data[Throttle_Channel]);
      //灯组状态转化
      Light_Analysis(PWM_CurrentData.PWM_Data[Light_Channel]);
      break;
    default:
      
      break;
  }
}
#endif


/**
  * @brief  PPM解码
  * @param  None
  * @retval None
  */
//void PPM_Decode(void)
//{
//  uint8_t i = 0;
//  uint32_t ICValue = 0;
//  static uint8_t CH = 0;
//  static uint16_t PPM_DataTemp[8] = {0};
//  static uint32_t ICValue_Backup = 0;
//
//  GetCapture = TIM2_GetCapture2;
//
//  //获取计数器捕获值
//  ICValue = GetCapture();
//  //检查数值合法性
//  if(ICValue < 0)
//  {
//    ICValue = 0;
//    ICValue_Backup = 0;
//    CH = 0;
//  }
//  else if(ICValue > 0)
//  {
//    if(ICValue < ICValue_Backup)
//    {
//      ICValue = ICValue + TIM_Period;
//    }
//    PPM_DataTemp[CH] = ICValue - ICValue_Backup;
//    ICValue_Backup = ICValue;
//    //printf("PPM:%d\r\n",PPM_DataTemp[CH]);
//    
//    //帧结束
//    if(PPM_DataTemp[CH] >= 5000)
//    {
//      //帧结束,无论这帧是否正确,开始新的一帧
//      CH = 0;
//    }
//    else 
//    {
//      if((PPM_DataTemp[CH] >= 900)&&(PPM_DataTemp[CH] <= 2100))
//      {
//        PPM_Data[CH] = PPM_DataTemp[CH]; 
//      }
//      CH++;
//      if(CH==8)
//      {          
//        for(i=0;i<8;i++)
//        {
//          //printf("PPM[%d]:%d\r\n",i,PPM_Data[i]);
//        }    
//      }
//      
//    }
//  }
//}
  
/**
  * @brief  电调通道解析
  * @param  Throttle_Data:接收机油门PWM值
  * @retval None
  */
void Throttle_Analysis(PWM_CURRENTDATA *currentdata,CONTROL_DATA *controldata)
{
  static float pwm_float = 0;
  for(uint8_t i = 0;i < 6;i ++)
  {
    if(currentdata->PWM_Data[i] > currentdata->PWM_Mid[i])
    {
      if(currentdata->PWM_Data[i] - currentdata->PWM_Mid[i] > DeadZone)
      {
        controldata->Dir[i] = 1;
        MotorDirForwardFunc[i]();
        pwm_float =  (float)((float)(currentdata->PWM_Data[i] - currentdata->PWM_Mid[i]) / (float)(currentdata->PWM_Max[i] - currentdata->PWM_Mid[i]));
        controldata->Magnitude[i] = (uint16_t)(pwm_float * 100);
      }
      else
      {
        controldata->Dir[i] = 1;
        controldata->Magnitude[i] = 0;
      }
    }
    else if(currentdata->PWM_Mid[i] >= currentdata->PWM_Data[i])
    {
      if(currentdata->PWM_Mid[i] - currentdata->PWM_Data[i] > DeadZone)
      {
        controldata->Dir[i] = 0;
        MotorDirBackwardsFunc[i]();
        pwm_float =  (float)((float)(currentdata->PWM_Mid[i] - currentdata->PWM_Data[i]) / (float)(currentdata->PWM_Mid[i] - currentdata->PWM_Min[i]));
        controldata->Magnitude[i] = (uint16_t)(pwm_float * 100);
      }
      else
      {
        controldata->Dir[i] = 1;
        controldata->Magnitude[i] = 0;
      }
    }
  }
  if(GPIO_ReadInputDataBit(BOOM_CHECK_H_PORT,BOOM_CHECK_H_PIN) == 0)
  {
    if(controldata->Dir[Boom_Channel] == 1)
    {
      controldata->Magnitude[Boom_Channel] = 0;
    }
  }
  if(GPIO_ReadInputDataBit(BOOM_CHECK_L_PORT,BOOM_CHECK_L_PIN) == 0)
  {
    if(controldata->Dir[Boom_Channel] == 0)
    {
      controldata->Magnitude[Boom_Channel] = 0;
    }
  }
  
}

void PPM_Decode(void)
{
  static uint8_t PPM_CH;
  static uint16_t Last_RisingEdge_Time;
  uint16_t Current_RisingEdge_Time = 0,PPM_IntervalTime_Temp = 0;
  
  Current_RisingEdge_Time = TIM3_GetCounter();
  if(Current_RisingEdge_Time < Last_RisingEdge_Time)
  {
    PPM_IntervalTime_Temp = (Current_RisingEdge_Time + 40000 - Last_RisingEdge_Time) / 2;
  }
  else
  {
    PPM_IntervalTime_Temp = (Current_RisingEdge_Time - Last_RisingEdge_Time) / 2;
  }
  
  
  if(PPM_IntervalTime_Temp < 2100)
  {
    //滤波
    if(abs(PWM_CurrentData.PWM_Data[PPM_CH] - PPM_IntervalTime_Temp) > 300)
    {
       PPM_IntervalTime_Temp = PWM_CurrentData.PWM_Data[PPM_CH];
    }
    PWM_CurrentData.PWM_Data[PPM_CH] = PPM_IntervalTime_Temp;
    PPM_CH ++;
    //一帧数据结束,产生20ms周期定时
    if(PPM_CH == 8)
    {
      PWM_CurrentData.Fail_Safe = 0;
      //TIM3_SetCounter(0);
      //GPIO_ToggleBits(GPIOA,GPIO_Pin_3);
//      PWM_CurrentData.PWM_Max[0] = 1;
//      PWM_CurrentData.PWM_Status[1] = 17;
//      PWM_CurrentData.PWM_Status[2] = 16;
//      PWM_CurrentData.PWM_Status[3] = 15;
//      PWM_CurrentData.PWM_Status[4] = 14;
//      PWM_CurrentData.PWM_Status[5] = 13;
//      PWM_CurrentData.PWM_Status[6] = 12;
//      PWM_CurrentData.PWM_Status[7] = 11;
      //PWM_Process();
    }
  }
  else
  {
    PPM_CH = 0;
  }
  Last_RisingEdge_Time = Current_RisingEdge_Time;
}































