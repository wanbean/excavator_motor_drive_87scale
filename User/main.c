/* Includes ------------------------------------------------------------------*/
#include "stm8l10x.h"
#include "Receiver_PPM.h"
#include "Motor_Control.h"
#include "Servo_Control.h"
#include "SoftPwm_Control.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
****入口参数：无
****出口参数：无
****函数备注：不精确延时函数
****版权信息：蓝旗嵌入式系统
*******************************************************************************/
void Delay(__IO uint16_t nCount)
{
    /* Decrement nCount value */
    while (nCount != 0)
    {
        nCount--;
    }
}
/**
  * @brief  周期控制定时器初始化 每个数0.0.0625*256 250个数 周期20ms
  * @param  None
  * @retval None
  */
//void CycleTIM_Init(void)
//{
//  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4,ENABLE); //使能外设时钟，STM8L外设时钟默认关闭，使用前需使能
//  TIM4_DeInit();
//  TIM4_TimeBaseInit(TIM4_Prescaler_16,100);//16M/1024=15.625K，0xff=255,255*（1/15.625）=0.01632S = 16.32ms，大约61次中断是1S
//  TIM4_ITConfig(TIM4_IT_Update, ENABLE);
//  TIM4_Cmd(ENABLE);//TIM4使能
//}

//大臂灯控制引脚
#define Boom_Lamp_PORT                  GPIOA
#define Boom_Lamp_PIN                   GPIO_Pin_2
//机身灯控制引脚
#define Shell_Lamp_PORT                GPIOA
#define Shell_Lamp_PIN                 GPIO_Pin_6

/**
  * @brief  灯光引脚初始化
  * @param  None
  * @retval None
  */
void LightGPIO_Init(void)
{
  //大臂灯
  GPIO_Init(Boom_Lamp_PORT, Boom_Lamp_PIN, GPIO_Mode_Out_PP_High_Fast);
  //机身灯
  GPIO_Init(Shell_Lamp_PORT, Shell_Lamp_PIN, GPIO_Mode_Out_PP_High_Fast);

  //置低电平
  GPIO_ResetBits(Boom_Lamp_PORT,Boom_Lamp_PIN);
  GPIO_ResetBits(Shell_Lamp_PORT,Shell_Lamp_PIN);
}

uint16_t i = 0,j = 0;
void main(void)
{
  CLK_MasterPrescalerConfig(CLK_MasterPrescaler_HSIDiv1);
  //USART_QuickInit();
  PPM_Reveiver_Init();
  LightGPIO_Init();
  Motor_Init();
  OutHOLD_INIT();

//  GPIO_Init(GPIOA,GPIO_Pin_3,GPIO_Mode_Out_PP_High_Fast);
  enableInterrupts();//开启中断总开关
    
    uint32_t sigLostCnt = 0;
  while(1)
  {
      //debug222 = TIM2_GetCounter();
      
      i+=3;
      if(i >= 60001) i = 0;
#if 0
      if(PWM_CurrentData.Fail_Safe == 0) PWM_CurrentData.Fail_Safe = 1;
      
      
      if(PWM_CurrentData.Fail_Safe == 1)
      {
        if(sigLostCnt++ >= 7500)
        {
          sigLostCnt = 0;
          PWM_CurrentData.Fail_Safe = 2;
        }
      }
      else
      {
        sigLostCnt = 0;
      }
#endif
      
      if(PWM_CurrentData.PWM_Status == 0)
      {
        if(Motor_Beep(1,100))
        {
            PWM_CurrentData.PWM_Status = 5;
        }
      }
      else if(PWM_CurrentData.PWM_Status == 2)
      {
        if(Motor_Beep(5,100))
        {
            PWM_CurrentData.PWM_Status = 5;
        }
      }
      else if(PWM_CurrentData.PWM_Status == 4)
      {
        if(Motor_Beep(10,100))
        {
            PWM_CurrentData.PWM_Status = 5;
        }
      }
      else if(PWM_CurrentData.PWM_Status == 6)
      {
        Motor_Process(i);
      }
      
  }
}


/**
  * @}
  */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/