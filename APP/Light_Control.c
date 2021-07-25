/*****************************************************************************************************
* @file    Light_Control.c									     * 
* @author  WanBean										     *
* @version V1.0                                                         			     *
* @date    2020-04-15    									     *
* @brief                                               					             *
******************************************************************************************************/
/* ---------------------------------------Include--------------------------------------------------- */
#include "Light_Control.h"
#include "stm8l10x_tim4.h"
/* ---------------------------------------Private typedef ------------------------------------------ */

/* ---------------------------------------Private define ------------------------------------------- */

/* ---------------------------------------Private macro -------------------------------------------- */

/* ---------------------------------------Private variables ---------------------------------------- */
uint8_t Stop_Lamp_Delay = 0;    
uint8_t Turning_Signal_Lamp_Delay = 0;
uint8_t Plice_Lamp_Delay = 0;
LAMP_MODE Lamp_Mode={Head_Lamp_OFF,Stop_Lamp_OFF,Backup_Lamp_OFF,Turning_Signal_OFF};
/* ---------------------------------------Private function prototypes  ----------------------------- */

  GPIO_ResetBits(Stop_Lamp_PORT,Stop_Lamp_PIN);
  GPIO_ResetBits(Turning_Signal_Lamp_L_PORT,Turning_Signal_Lamp_L_PIN);
  GPIO_ResetBits(Turning_Signal_Lamp_R_PORT,Turning_Signal_Lamp_R_PIN);
  GPIO_ResetBits(Police_Lamp_R_PORT, Police_Lamp_R_PIN);
  GPIO_ResetBits(Police_Lamp_B_PORT, Police_Lamp_B_PIN);
  
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4,ENABLE); //ʹ������ʱ�ӣ�STM8L����ʱ��Ĭ�Ϲرգ�ʹ��ǰ��ʹ��
  TIM4_DeInit();
  TIM4_TimeBaseInit(TIM4_Prescaler_2048,157);//16M/1024=15.625K��0xff=255,255*��1/15.625��=0.01632S = 16.32ms����Լ61���ж���1S
  TIM4_ITConfig(TIM4_IT_Update, ENABLE);
  TIM4_Cmd(ENABLE);//TIM4ʹ��
}

/**
  * @brief  ��ƿ���
  * @param  Count:��������(1��CountΪ10Us) Mode:�ƹ�ģʽ
  * @retval None
  */
void Head_Lamp_Control(HEAD_LAMP_MODE Mode)
{
  uint8_t Tim4_Count = TIM4_GetCounter(); 
  switch(Mode)
  {
    //�ƹ�ر�
    case Head_Lamp_OFF:
      GPIO_ResetBits(Head_Lamp_PORT,Head_Lamp_PIN);
      break;
    //�����
    case Head_Lamp_LB:
      if(Tim4_Count < Head_Lamp_Bright)
      {
        GPIO_SetBits(Head_Lamp_PORT,Head_Lamp_PIN);
      }
      else
      {
        GPIO_ResetBits(Head_Lamp_PORT,Head_Lamp_PIN);
      }
      break;
    //Զ���
    case Head_Lamp_HB:
      GPIO_SetBits(Head_Lamp_PORT,Head_Lamp_PIN);
      break;
    default:
      break;
  }
}

/**
  * @brief  ɲ���ƿ���
  * @param  Count:��������(1��CountΪ10Us) Mode:�ƹ�ģʽ
  * @retval None
  */
void Stop_Lamp_Control(STOP_LAMP_MODE *Mode)
{
  static uint16_t Stop_Lamp_Check;//ɲ���˲�
  uint8_t Tim4_Count = TIM4_GetCounter();
  switch(*Mode)
  {
    //�ƹ�ر�
    case Stop_Lamp_OFF:
      GPIO_ResetBits(Stop_Lamp_PORT,Stop_Lamp_PIN);
      Stop_Lamp_Check = 0;
      break;
    //����
    case Stop_Lamp_Normal:
      GPIO_SetBits(Stop_Lamp_PORT,Stop_Lamp_PIN);
      Stop_Lamp_Check = 0;
      if(Tim4_Count < Stop_Lamp_Bright)
      {
        GPIO_SetBits(Stop_Lamp_PORT,Stop_Lamp_PIN);
      }
      else
      {
        GPIO_ResetBits(Stop_Lamp_PORT,Stop_Lamp_PIN);
      }
      break;
    //ɲ��
    case Stop_Lamp_Brake:
      if(Stop_Lamp_Check < Stop_Lamp_Smoothing)
      {
        //ɲ���˲�,��ֹ�󴥷�,δ�ﵽɲ��ʱ��ʱ����Ϊ������ر�ģʽ
        if(Lamp_Mode.Head_Lamp_Mode != Head_Lamp_OFF)
        {
          if(Tim4_Count < Stop_Lamp_Bright)
          {
            GPIO_SetBits(Stop_Lamp_PORT,Stop_Lamp_PIN);
          }
          else
          {
            GPIO_ResetBits(Stop_Lamp_PORT,Stop_Lamp_PIN);
          }
        }
        Stop_Lamp_Check ++;
      }
      //����ɲ����
      else if(Stop_Lamp_Check == Stop_Lamp_Smoothing)
      {
        GPIO_SetBits(Stop_Lamp_PORT,Stop_Lamp_PIN);
        if(Stop_Lamp_Delay == 0)
        {
          Stop_Lamp_Delay = Stop_Lamp_DelayTime;
        }
        Stop_Lamp_Check = Stop_Lamp_Smoothing + 1;
      }
      //���������� �ر�ɲ����
      else if(Stop_Lamp_Delay == 1)
      {
        *Mode = Stop_Lamp_OFF;
        Stop_Lamp_Delay = 0;
        Stop_Lamp_Check = 0;
      }
      break;
    default:
      break;
  }
}

/**
  * @brief  ת��ƿ���
  * @param  Count:��������(1��CountΪ10Us) Mode:�ƹ�ģʽ
  * @retval None
  */
void Turning_Signal_Lamp_Control(TURNING_SIGNAL_LAMP_MODE Mode)
{
  switch(Mode)
  {
    //�ƹ�ر�
    case Turning_Signal_OFF:
      GPIO_ResetBits(Turning_Signal_Lamp_L_PORT,Turning_Signal_Lamp_L_PIN);
      GPIO_ResetBits(Turning_Signal_Lamp_R_PORT,Turning_Signal_Lamp_R_PIN);
      Turning_Signal_Lamp_Delay = 0;
      break;
    //��ת
    case Turning_Signal_Turn_Left:
      if(Turning_Signal_Lamp_Delay == 0)
      {
        //Turning_Signal_Lamp_DelayTimeΪ��˸����
        Turning_Signal_Lamp_Delay = Turning_Signal_Lamp_DelayTime;
        GPIO_SetBits(Turning_Signal_Lamp_L_PORT,Turning_Signal_Lamp_L_PIN);
        GPIO_ResetBits(Turning_Signal_Lamp_R_PORT,Turning_Signal_Lamp_R_PIN);
      }
      else if(Turning_Signal_Lamp_Delay == Turning_Signal_Lamp_DelayTime / 2)
      {
        GPIO_ResetBits(Turning_Signal_Lamp_L_PORT,Turning_Signal_Lamp_L_PIN);
        GPIO_ResetBits(Turning_Signal_Lamp_R_PORT,Turning_Signal_Lamp_R_PIN);
      }
      break;
    //��ת
    case Turning_Signal_Turn_Right:
      if(Turning_Signal_Lamp_Delay == 0)
      {
        Turning_Signal_Lamp_Delay = Turning_Signal_Lamp_DelayTime;
        GPIO_SetBits(Turning_Signal_Lamp_R_PORT,Turning_Signal_Lamp_R_PIN);
        GPIO_ResetBits(Turning_Signal_Lamp_L_PORT,Turning_Signal_Lamp_L_PIN);
      }
      else if(Turning_Signal_Lamp_Delay == Turning_Signal_Lamp_DelayTime / 2)
      {
        GPIO_ResetBits(Turning_Signal_Lamp_R_PORT,Turning_Signal_Lamp_R_PIN);
        GPIO_ResetBits(Turning_Signal_Lamp_L_PORT,Turning_Signal_Lamp_L_PIN);
      }
      break;
    //˫��
    case Turning_Signal_Beacon:
      if(Turning_Signal_Lamp_Delay == 0)
      {
        Turning_Signal_Lamp_Delay = Turning_Signal_Lamp_DelayTime;
        GPIO_SetBits(Turning_Signal_Lamp_L_PORT,Turning_Signal_Lamp_L_PIN);
        GPIO_SetBits(Turning_Signal_Lamp_R_PORT,Turning_Signal_Lamp_R_PIN);
      }
      else if(Turning_Signal_Lamp_Delay == Turning_Signal_Lamp_DelayTime / 2)
      {
        GPIO_ResetBits(Turning_Signal_Lamp_L_PORT,Turning_Signal_Lamp_L_PIN);
        GPIO_ResetBits(Turning_Signal_Lamp_R_PORT,Turning_Signal_Lamp_R_PIN);
      }

      break;
    default:
      break;
  }
}

  /**
  * @brief  ���ƿ���
  * @param  Count:��������(1��CountΪ10Us) Mode:�ƹ�ģʽ
  * @retval None
  */
void Plice_Lamp_Control(POLICE_LAMP_MODE *Mode)
{
  switch(*Mode)
  {
    //�ƹ�ر�
    case Police_Lamp_OFF:
      Plice_Lamp_Delay = 0;
      GPIO_ResetBits(Police_Lamp_R_PORT,Police_Lamp_R_PIN);
      GPIO_ResetBits(Police_Lamp_B_PORT,Police_Lamp_B_PIN);
      break;
    //����
    case Police_Lamp_M1:
      Plice_Lamp_Delay = 0;
      GPIO_SetBits(Police_Lamp_R_PORT,Police_Lamp_R_PIN);
      GPIO_SetBits(Police_Lamp_B_PORT,Police_Lamp_B_PIN);
      break;
    //������˸
    case Police_Lamp_M2:
      if(Plice_Lamp_Delay == 0)
      {
        Plice_Lamp_Delay = Police_Lamp_M2_DelayTime;
        GPIO_SetBits(Police_Lamp_R_PORT,Police_Lamp_R_PIN);
        GPIO_ResetBits(Police_Lamp_B_PORT,Police_Lamp_B_PIN);
      }
      else if(Plice_Lamp_Delay == Police_Lamp_M2_DelayTime / 2)
      {
        GPIO_ResetBits(Police_Lamp_R_PORT,Police_Lamp_R_PIN);
        GPIO_SetBits(Police_Lamp_B_PORT,Police_Lamp_B_PIN);
      }
      break;
    //˫�ؽ�����˸
    case Police_Lamp_M3:
      if(Plice_Lamp_Delay == 0)
      {
        Plice_Lamp_Delay = Police_Lamp_M3_DelayTime;
        GPIO_SetBits(Police_Lamp_R_PORT,Police_Lamp_R_PIN);
        GPIO_ResetBits(Police_Lamp_B_PORT,Police_Lamp_B_PIN);
      }
      //LED״̬0 0 220 180 140 100 60
      else if((Plice_Lamp_Delay == (Police_Lamp_M3_DelayTime - Police_Lamp_M3_DelayTime / 12))||(Plice_Lamp_Delay == (Police_Lamp_M3_DelayTime - 3 * (Police_Lamp_M3_DelayTime / 12)))||(Plice_Lamp_Delay == (Police_Lamp_M3_DelayTime - 5 * (Police_Lamp_M3_DelayTime / 12)))||(Plice_Lamp_Delay == (Police_Lamp_M3_DelayTime - 7 * (Police_Lamp_M3_DelayTime / 12)))||(Plice_Lamp_Delay == (Police_Lamp_M3_DelayTime - 9 * (Police_Lamp_M3_DelayTime / 12))))
      {
        GPIO_ResetBits(Police_Lamp_R_PORT,Police_Lamp_R_PIN);
        GPIO_ResetBits(Police_Lamp_B_PORT,Police_Lamp_B_PIN);
      }
      //LED״̬1 0 200
      else if(Plice_Lamp_Delay == (Police_Lamp_M3_DelayTime - 2 * (Police_Lamp_M3_DelayTime / 12)))
      {
        GPIO_SetBits(Police_Lamp_R_PORT,Police_Lamp_R_PIN);
        GPIO_ResetBits(Police_Lamp_B_PORT,Police_Lamp_B_PIN);
      }
      //LED״̬0 1 160 120
      else if((Plice_Lamp_Delay == (Police_Lamp_M3_DelayTime - 4 * (Police_Lamp_M3_DelayTime / 12)))||(Plice_Lamp_Delay == (Police_Lamp_M3_DelayTime - 6 * (Police_Lamp_M3_DelayTime / 12))))
      {
        GPIO_ResetBits(Police_Lamp_R_PORT,Police_Lamp_R_PIN);
        GPIO_SetBits(Police_Lamp_B_PORT,Police_Lamp_B_PIN);
      }
      //LED״̬1 1 80 40
      else if((Plice_Lamp_Delay == (Police_Lamp_M3_DelayTime - 8 * (Police_Lamp_M3_DelayTime / 12)))||(Plice_Lamp_Delay == (Police_Lamp_M3_DelayTime - 10 * (Police_Lamp_M3_DelayTime / 12))))
      {
        GPIO_SetBits(Police_Lamp_R_PORT,Police_Lamp_R_PIN);
        GPIO_SetBits(Police_Lamp_B_PORT,Police_Lamp_B_PIN);
      }
      break;
    default:
      break;
  }
}

/**
  * @brief  �����ƿ���
  * @param  None
  * @retval None
  */
void BackUp_Lamp_Control(BACKUP_LAMP_MODE Mode)
{
  uint8_t Tim4_Count = TIM4_GetCounter();
  if(Mode !=  Backup_Lamp_OFF)
  {
    if(Tim4_Count < BackUp_Lamp_Bright)
    {
      GPIO_SetBits(BackUp_Lamp_PORT,BackUp_Lamp_PIN);
    }
    else
    {
      GPIO_ResetBits(BackUp_Lamp_PORT,BackUp_Lamp_PIN);
    }
  }
  else
  {
    GPIO_ResetBits(BackUp_Lamp_PORT,BackUp_Lamp_PIN);
  }
}

/**
  * @brief  ��ʱ����
  * @param  None
  * @retval None
  */
static void Delay(__IO uint16_t nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
    }
}

/**
  * @brief  �ƹ���˸
  * @param  None
  * @retval None
  */
void Lamp_Blink_Single(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin)
{
  GPIO_SetBits(GPIOx,GPIO_Pin);
  Delay(0xFFFF);
  Delay(0xFFFF);
  Delay(0xFFFF);
  GPIO_ResetBits(GPIOx,GPIO_Pin);
  Delay(0xFFFF);
  Delay(0xFFFF);
  Delay(0xFFFF);
}

/**
  * @brief  �ƹ���˸
  * @param  None
  * @retval None
  */
void Lamp_Blink_All(void)
{
  GPIO_SetBits(Head_Lamp_PORT,Head_Lamp_PIN);
  GPIO_SetBits(BackUp_Lamp_PORT,BackUp_Lamp_PIN);
  GPIO_SetBits(Stop_Lamp_PORT,Stop_Lamp_PIN);
  GPIO_SetBits(Turning_Signal_Lamp_L_PORT,Turning_Signal_Lamp_L_PIN);
  GPIO_SetBits(Turning_Signal_Lamp_R_PORT,Turning_Signal_Lamp_R_PIN);      
  Delay(0xFFFF);
  Delay(0xFFFF);
  Delay(0xFFFF);
  GPIO_ResetBits(Head_Lamp_PORT,Head_Lamp_PIN);
  GPIO_ResetBits(BackUp_Lamp_PORT,BackUp_Lamp_PIN);
  GPIO_ResetBits(Stop_Lamp_PORT,Stop_Lamp_PIN);
  GPIO_ResetBits(Turning_Signal_Lamp_L_PORT,Turning_Signal_Lamp_L_PIN);
  GPIO_ResetBits(Turning_Signal_Lamp_R_PORT,Turning_Signal_Lamp_R_PIN);
  Delay(0xFFFF);
  Delay(0xFFFF);
  Delay(0xFFFF);
}
