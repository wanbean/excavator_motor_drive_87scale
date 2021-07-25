/*****************************************************************************************************
* @file    Receiver_PPM.c									                                                           * 
* @author  WanBean										                                                               *
* @version V1.0                                                         			                       *
* @date    2020-03-27    									                                                           *
* @brief                                               					                                     *
******************************************************************************************************/
/* ---------------------------------------Include--------------------------------------------------- */
#include "Receiver_PPM.h"
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
                                   .PWM_Status = {3,3,3,3,3,3,3,3}};
CONTROL_DATA Control_Data;
volatile uint8_t PPMCapture_Pried = 0; 

/**
  * @brief  PPM�����ʼ��
  * @param  None
  * @retval None
  */
void PPM_Reveiver_Init(void)
{
  /* ��ʱ������ʽ */
//  //ʹ��TIM2����ʱ���ſ�
//  CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
//    
//  //����TIM2 ͨ��1--PB0��GPIO�� �������벻���ж�
//  GPIO_Init(GPIOB, GPIO_Pin_2, GPIO_Mode_In_PU_No_IT);
//  
////  TIM2_TimeBaseInit(TIM2_Prescaler_16, TIM2_CounterMode_Up,59999);
//  TIM2_PrescalerConfig(TIM2_Prescaler_16,TIM2_PSCReloadMode_Immediate);//TIM2_PSCReloadMode_Update
//  //�Զ�Ԥװֵ
//  TIM2_SetAutoreload(65535);
//  //�Զ�Ԥװ��ʹ��
//  TIM2_ARRPreloadConfig(ENABLE);
//
//  /* Capture only every 8 events!!! */
//  //����TIM2���벶��ͨ��1���½��ش��������ӵ���ʱ������ͨ��1  ÿ8���¼�����һ�β������˲���
//  TIM2_ICInit(TIM2_Channel_2, TIM2_ICPolarity_Rising, TIM2_ICSelection_DirectTI, TIM2_ICPSC_Div1, 0);
//  
//  //ʹ��TIM2����
//  TIM2_Cmd(ENABLE);
//  
//  TIM2_ITConfig(TIM2_IT_CC2,ENABLE);
//  //�岶���־
//  TIM2_ClearFlag(TIM2_FLAG_CC2);
  
  /* �����ⲿ�жϷ�ʽ */
  GPIO_Init(PPM_Input_PORT,PPM_Input_PIN,GPIO_Mode_In_PU_IT);
  
  EXTI_DeInit (); //�ָ��жϵ��������� 
  EXTI_SetPinSensitivity (EXTI_Pin_0,EXTI_Trigger_Rising);//�ⲿ�ж�1�������ش������ж�������11
}

  /**
  * @brief  ��ȡFlash�е�PWMֵ
  * @param  None
  * @retval None
  */
static void Read_Flash_PWM(void)
{
  uint8_t Val_H = 0x0,Val_L = 0x0;
  
  FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);//��׼���ʱ��
  FLASH_Unlock(FLASH_MemType_Program);  //�������
  while (FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET);
  FLASH_Unlock(FLASH_MemType_Data);  //�������
  while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET);
  
  /* ���Flash����ֵ */
  //��ȡPWM1_Min Flash�б��������
  Val_H = FLASH_ReadByte(PWM1_MINAdd);
  Val_L = FLASH_ReadByte(PWM1_MINAdd + 1);
  PWM_CurrentData.PWM_Min[0] = (uint16_t)(Val_H << 8) + Val_L;

  //��ȡPWM1_Mid Flash�б��������
  Val_H = FLASH_ReadByte(PWM1_MIDAdd);
  Val_L = FLASH_ReadByte(PWM1_MIDAdd + 1);
  PWM_CurrentData.PWM_Mid[0] = (uint16_t)(Val_H << 8) + Val_L;
  
  //��ȡPWM1_Max Flash�б��������
  Val_H = FLASH_ReadByte(PWM1_MAXAdd);
  Val_L = FLASH_ReadByte(PWM1_MAXAdd + 1);
  PWM_CurrentData.PWM_Max[0] = (uint16_t)(Val_H << 8) + Val_L;
  
  //��ȡPWM1_Min Flash�б��������
  Val_H = FLASH_ReadByte(PWM2_MINAdd);
  Val_L = FLASH_ReadByte(PWM2_MINAdd + 1);
  PWM_CurrentData.PWM_Min[1] = (uint16_t)(Val_H << 8) + Val_L;

  //��ȡPWM1_Mid Flash�б��������
  Val_H = FLASH_ReadByte(PWM2_MIDAdd);
  Val_L = FLASH_ReadByte(PWM2_MIDAdd + 1);
  PWM_CurrentData.PWM_Mid[1] = (uint16_t)(Val_H << 8) + Val_L;
  
  //��ȡPWM1_Max Flash�б��������
  Val_H = FLASH_ReadByte(PWM2_MAXAdd);
  Val_L = FLASH_ReadByte(PWM2_MAXAdd + 1);
  PWM_CurrentData.PWM_Max[1] = (uint16_t)(Val_H << 8) + Val_L;

  //��ȡPWM1_Min Flash�б��������
  Val_H = FLASH_ReadByte(PWM3_MINAdd);
  Val_L = FLASH_ReadByte(PWM3_MINAdd + 1);
  PWM_CurrentData.PWM_Min[2] = (uint16_t)(Val_H << 8) + Val_L;

  //��ȡPWM1_Mid Flash�б��������
  Val_H = FLASH_ReadByte(PWM3_MIDAdd);
  Val_L = FLASH_ReadByte(PWM3_MIDAdd + 1);
  PWM_CurrentData.PWM_Mid[2] = (uint16_t)(Val_H << 8) + Val_L;
  
  //��ȡPWM1_Max Flash�б��������
  Val_H = FLASH_ReadByte(PWM3_MAXAdd);
  Val_L = FLASH_ReadByte(PWM3_MAXAdd + 1);
  PWM_CurrentData.PWM_Max[2] = (uint16_t)(Val_H << 8) + Val_L;

  //��ȡPWM1_Min Flash�б��������
  Val_H = FLASH_ReadByte(PWM4_MINAdd);
  Val_L = FLASH_ReadByte(PWM4_MINAdd + 1);
  PWM_CurrentData.PWM_Min[3] = (uint16_t)(Val_H << 8) + Val_L;

  //��ȡPWM1_Mid Flash�б��������
  Val_H = FLASH_ReadByte(PWM4_MIDAdd);
  Val_L = FLASH_ReadByte(PWM4_MIDAdd + 1);
  PWM_CurrentData.PWM_Mid[3] = (uint16_t)(Val_H << 8) + Val_L;
  
  //��ȡPWM1_Max Flash�б��������
  Val_H = FLASH_ReadByte(PWM4_MAXAdd);
  Val_L = FLASH_ReadByte(PWM4_MAXAdd + 1);
  PWM_CurrentData.PWM_Max[3] = (uint16_t)(Val_H << 8) + Val_L;

  //��ȡPWM1_Min Flash�б��������
  Val_H = FLASH_ReadByte(PWM5_MINAdd);
  Val_L = FLASH_ReadByte(PWM5_MINAdd + 1);
  PWM_CurrentData.PWM_Min[4] = (uint16_t)(Val_H << 8) + Val_L;

  //��ȡPWM1_Mid Flash�б��������
  Val_H = FLASH_ReadByte(PWM5_MIDAdd);
  Val_L = FLASH_ReadByte(PWM5_MIDAdd + 1);
  PWM_CurrentData.PWM_Mid[4] = (uint16_t)(Val_H << 8) + Val_L;
  
  //��ȡPWM1_Max Flash�б��������
  Val_H = FLASH_ReadByte(PWM5_MAXAdd);
  Val_L = FLASH_ReadByte(PWM5_MAXAdd + 1);
  PWM_CurrentData.PWM_Max[4] = (uint16_t)(Val_H << 8) + Val_L;

  //��ȡPWM1_Min Flash�б��������
  Val_H = FLASH_ReadByte(PWM6_MINAdd);
  Val_L = FLASH_ReadByte(PWM6_MINAdd + 1);
  PWM_CurrentData.PWM_Min[5] = (uint16_t)(Val_H << 8) + Val_L;

  //��ȡPWM1_Mid Flash�б��������
  Val_H = FLASH_ReadByte(PWM6_MIDAdd);
  Val_L = FLASH_ReadByte(PWM6_MIDAdd + 1);
  PWM_CurrentData.PWM_Mid[5] = (uint16_t)(Val_H << 8) + Val_L;
  
  //��ȡPWM1_Max Flash�б��������
  Val_H = FLASH_ReadByte(PWM6_MAXAdd);
  Val_L = FLASH_ReadByte(PWM6_MAXAdd + 1);
  PWM_CurrentData.PWM_Max[5] = (uint16_t)(Val_H << 8) + Val_L;

  //��ȡPWM1_Min Flash�б��������
  Val_H = FLASH_ReadByte(PWM7_MINAdd);
  Val_L = FLASH_ReadByte(PWM7_MINAdd + 1);
  PWM_CurrentData.PWM_Min[6] = (uint16_t)(Val_H << 8) + Val_L;

  //��ȡPWM1_Mid Flash�б��������
  Val_H = FLASH_ReadByte(PWM7_MIDAdd);
  Val_L = FLASH_ReadByte(PWM7_MIDAdd + 1);
  PWM_CurrentData.PWM_Mid[6] = (uint16_t)(Val_H << 8) + Val_L;
  
  //��ȡPWM1_Max Flash�б��������
  Val_H = FLASH_ReadByte(PWM7_MAXAdd);
  Val_L = FLASH_ReadByte(PWM7_MAXAdd + 1);
  PWM_CurrentData.PWM_Max[6] = (uint16_t)(Val_H << 8) + Val_L;

  //��ȡPWM1_Min Flash�б��������
  Val_H = FLASH_ReadByte(PWM8_MINAdd);
  Val_L = FLASH_ReadByte(PWM8_MINAdd + 1);
  PWM_CurrentData.PWM_Min[7] = (uint16_t)(Val_H << 8) + Val_L;

  //��ȡPWM1_Mid Flash�б��������
  Val_H = FLASH_ReadByte(PWM8_MIDAdd);
  Val_L = FLASH_ReadByte(PWM8_MIDAdd + 1);
  PWM_CurrentData.PWM_Mid[7] = (uint16_t)(Val_H << 8) + Val_L;
  
  //��ȡPWM1_Max Flash�б��������
  Val_H = FLASH_ReadByte(PWM8_MAXAdd);
  Val_L = FLASH_ReadByte(PWM8_MAXAdd + 1);
  PWM_CurrentData.PWM_Max[7] = (uint16_t)(Val_H << 8) + Val_L;
}

/**
  * @brief  PWMֵд��Flash
  * @param  Address:д���ַ
  * @param  Data��д������
  * @retval None
  */
static void Write_Flash_PWM(uint16_t Address,uint16_t Data)
{
  //д���ֵ
  FLASH_ProgramByte(Address,BYTE_1(Data));
  while (FLASH_GetFlagStatus(FLASH_FLAG_EOP) == RESET);
  FLASH_ProgramByte(Address + 1,BYTE_0(Data));
  while (FLASH_GetFlagStatus(FLASH_FLAG_EOP) == RESET);
}

#if 0
/**
  * @brief  PWM��ֵ�ɼ�
  * @param  PWM_Current:��ǰ�ɼ���PWMֵ
  * @retval None
  */
static void PWM_AVG_Calculate(uint16_t *PWM_AVG,uint16_t *PWM_Current)
{
  static uint8_t CentreCaptureStatus = 0;//0-4:�ɼ� 5:����ƽ��ֵ 6:ready
  static uint16_t PWM_Temp,Last_PWM_Current;
  //ҡ�����������ֲ���1S
  if(abs(Last_PWM_Current - *PWM_Current) < (DeadZone/2))
  {
    CentreCaptureStatus ++;
  }
  else
  {
    CentreCaptureStatus = 0;
    PWM_Temp = 0;
  }
  //ҡ�˲���2S��ʼ�ɼ�
  if(CentreCaptureStatus > 40)
  {
    PWM_Temp += *PWM_Current;
  }
  //�ɼ�10��
  if(CentreCaptureStatus == 50)
  {
    *PWM_AVG = PWM_Temp/10;
    CentreCaptureStatus = 0;
    PWM_Temp = 0;
    Last_PWM_Current = 0;
    //������ֵ �粻�Ϸ�,���²ɼ�
    if(PWM_CurrentData.PWM_Ready % 3 == 0)
    {
      //������ ���߳���2000
      if((*PWM_AVG < PWM_CentreRef + SwitchThreshold)||(*PWM_AVG > PWM_MaxRef))
      {
        return;
      }
      //�Ϸ�,���浽Flash
      else
      {
        Write_Flash_PWM(*PWM_AVG);
      }
    }
    //�����ֵ �粻�Ϸ�,���²ɼ�
    else if(PWM_CurrentData.PWM_Ready % 3 == 1)
    {
      if(ABS(*PWM_AVG - PWM_CentreRef) > 20)
      {
        return;
      }
      //�Ϸ�,���浽Flash
      else
      {
        //д��������ֵ
        Write_Flash_PWM1Mid(*PWM_AVG);
        //д�������ֵ
        
        Write_Flash_PWM2Mid(PWM_CurrentData.PWM_Data[1]);
      }
    }
    //�����Сֵ �粻�Ϸ�,���²ɼ�
    else if(PWM_CurrentData.PWM_Ready % 3 == 2)
    {
      //����С ����С��2000
      if((*PWM_AVG > PWM_CentreRef - SwitchThreshold)||(*PWM_AVG < PWM_MinRef))
      {
        return;
      }
      //�Ϸ�,���浽Flash
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
  * @brief  �ϵ�PWM��Сֵ ��ֵ ���ֵУ׼
  * @param  None
  * @retval None
  */
static void PWM_Calibration(uint16_t *PWM_Current)
{
  static uint8_t CentreCaptureStatus = 0;
  static uint16_t Last_PWM_Current = 0;
  
  /* ���Flash����ֵ */
  Read_Flash_PWM();
  //�ж��ĸ�ֵ��ҪУ׼
  if(PWM_CurrentData.PWM_Min[0] == 0)
  {
    PWM_CurrentData.PWM_Ready = 2;//У׼��Сֵ
  }
  if(PWM_CurrentData.PWM_Mid[0] == 0)
  {
    PWM_CurrentData.PWM_Ready = 1;//У׼��ֵ
  }
  if(PWM_CurrentData.PWM_Max[0] == 0)
  {
    PWM_CurrentData.PWM_Ready = 0;//У׼���ֵ
  }
  /* ��ҪУ׼ */
  if(PWM_CurrentData.PWM_Ready != 24)
  {
    return;
  }
  /* ���Flash����ֵ */
  else//Flash�е�ֵ��ȷ
  {
    /* �ж�����λ�� */
    //ҡ�����������ֲ���1S
    if(ABS(Last_PWM_Current - *PWM_Current) < 2 * DeadZone)
    {
      CentreCaptureStatus ++;
    }
    else
    {
      CentreCaptureStatus = 0;
    }
    //ҡ�˲���1S��ʼ�ж�����λ��
    if(CentreCaptureStatus == 25)
    {
      //������� ����У׼
      if(*PWM_Current > PWM_CurrentData.PWM_Mid[0] + SwitchThreshold)
      {
        PWM_CurrentData.PWM_Ready = 0;
      }
      //��������λ ����ִ��
      else if(ABS(*PWM_Current - PWM_CurrentData.PWM_Mid[0]) < (2 * DeadZone))
      {
        PWM_CurrentData.PWM_Ready = 4;
      }
    }
    Last_PWM_Current = *PWM_Current;
  }
  /* �ж�����λ�� */
}

/**
  * @brief  ����ͨ������
  * @param  Throttle_Data:���ջ�����PWMֵ
  * @retval None
  */
static void Throttle_Analysis(uint16_t Throttle_Data)
{
  static uint8_t LastThrottle_Dir;
  //ǰ��
  if(Throttle_Data > (PWM_CurrentData.PWM_Mid[0] + DeadZone))
  {
    Control_Data.Throttle_Dir = 1;
    //�رյ�����
    Lamp_Mode.BackUp_Lame_Mode = Backup_Lamp_OFF;
    //ѡ��ɲ����ģʽ
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
  //����
  else if(Throttle_Data < (PWM_CurrentData.PWM_Mid[0] - DeadZone))
  {
    Control_Data.Throttle_Dir = -1;
    //����������
    Lamp_Mode.BackUp_Lame_Mode = Backup_Lamp_Normal;
    //ѡ��ɲ����ģʽ
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
  else//ɲ��״̬
  {
    Control_Data.Throttle_Dir = 0;
    //�رյ�����
    Lamp_Mode.BackUp_Lame_Mode = Backup_Lamp_OFF;
    //ѡ��ɲ����ģʽ
    if((Lamp_Mode.Head_Lamp_Mode != Head_Lamp_OFF)&&(Lamp_Mode.Stop_Lamp_Mode != Stop_Lamp_Brake))
    {
      Lamp_Mode.Stop_Lamp_Mode = Stop_Lamp_Normal;
    }
    if(LastThrottle_Dir != 0)
    {
      //ɲ���Ʊ�־��λ
      Lamp_Mode.Stop_Lamp_Mode = Stop_Lamp_Brake;     
    }

  }
  LastThrottle_Dir = Control_Data.Throttle_Dir;
  Control_Data.Throttle_Magnitude = ABS(Throttle_Data - PWM_CurrentData.PWM_Mid[0]);
}

/**
  * @brief  ����ͨ������
  * @param  Light_Data:���ջ�����PWMֵ
  * @retval None
  */
static void Light_Analysis(uint16_t Light_Data)
{
  static uint8_t SwitchH_Click,SwitchL_Click;
  static uint16_t LightPWMH_Count,LightPWMM_Count,LightPWML_Count;
  //���ο�������
  if(Light_Data > PWM_CurrentData.PWM_Mid[1] + SwitchThreshold)
  {
    Control_Data.Light_Status = 1;
  }
  //���ο�������
  else if(Light_Data < PWM_CurrentData.PWM_Mid[1] - SwitchThreshold)
  {
    Control_Data.Light_Status = -1;
  }
  //���ο�����λ
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
    //���³���
    if(LightPWML_Count > 25)//����500ms,��ת��
    {
      Lamp_Mode.Turning_Signal_Lamp_Mode = Turning_Signal_Turn_Left;
      SwitchL_Click = 0;
      //��������
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
    //����
    if(LightPWMM_Count > 25)
    {
      //�� ��ʾ��
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
      //�� ���
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
    //˫��
    if(SwitchL_Click == 3)
    {
      SwitchL_Click = 0;
      //����ģʽ ѭ���л�
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
    //˫��
    if(SwitchH_Click == 3)
    {
      SwitchH_Click = 0;
    }
    
    //���Ƽ���
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
    //���ϳ���
    if(LightPWMH_Count > 25)//����1000ms,��ת��
    {
      Lamp_Mode.Turning_Signal_Lamp_Mode = Turning_Signal_Turn_Right;
      SwitchH_Click = 0;
      //��������
      LightPWMH_Count = 26;
    }
  }
}

/**
  * @brief  PWM���ݴ���
  * @param  None
  * @retval None
  */
void PWM_Process(void)
{
  PWM_CurrentData.Fail_Safe = 0;
  switch (PWM_CurrentData.PWM_Ready)
  {
    case 0://�ɼ����ֵ
      PWM_AVG_Calculate(&PWM_CurrentData.PWM_Max[0],&PWM_CurrentData.PWM_Data[Throttle_Channel]);
      break;
    case 1://�ɼ���ֵ
      PWM_AVG_Calculate(&PWM_CurrentData.PWM_Mid[0],&PWM_CurrentData.PWM_Data[Throttle_Channel]);
      break;
    case 2://�ɼ���Сֵ
      PWM_AVG_Calculate(&PWM_CurrentData.PWM_Min[0],&PWM_CurrentData.PWM_Data[Throttle_Channel]);
      break;
    case 3://���Flash �ж�����λ��
      PWM_Calibration(&PWM_CurrentData.PWM_Data[Throttle_Channel]);
      //ȡ��Сֵ��Ϊ��ʱ������
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
    case 4://����ִ��
      //����״̬ת��
      Throttle_Analysis(PWM_CurrentData.PWM_Data[Throttle_Channel]);
      //����״̬ת��
      Light_Analysis(PWM_CurrentData.PWM_Data[Light_Channel]);
      break;
    default:
      
      break;
  }
}
#endif


/**
  * @brief  PPM����
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
//  //��ȡ����������ֵ
//  ICValue = GetCapture();
//  //�����ֵ�Ϸ���
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
//    //֡����
//    if(PPM_DataTemp[CH] >= 5000)
//    {
//      //֡����,������֡�Ƿ���ȷ,��ʼ�µ�һ֡
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
    //�˲�
    if(abs(PWM_CurrentData.PWM_Data[PPM_CH] - PPM_IntervalTime_Temp) > 300)
    {
       PPM_IntervalTime_Temp = PWM_CurrentData.PWM_Data[PPM_CH];
    }
    PWM_CurrentData.PWM_Data[PPM_CH] = PPM_IntervalTime_Temp;
    PPM_CH ++;
    //һ֡���ݽ���,����20ms���ڶ�ʱ
    if(PPM_CH == 8)
    {
      //TIM3_SetCounter(0);
      //GPIO_ToggleBits(GPIOA,GPIO_Pin_3);
      PWM_CurrentData.PWM_Status[0] = 1;
      //PWM_Process();
    }
  }
  else
  {
    PPM_CH = 0;
  }
  Last_RisingEdge_Time = Current_RisingEdge_Time;
}































