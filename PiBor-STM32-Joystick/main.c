/*******************************************************************************
  STM32���Գ���
  Desgined by Zweb
*******************************************************************************/  

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "delay.h"
#include "beep_pwm.h"
#include "key.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "platform_config.h"
#include "usb_pwr.h"
    
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
#define BL_OFF() GPIOB->BRR  = GPIO_Pin_0
#define BL_ON()  GPIOB->BSRR = GPIO_Pin_0
    
static unsigned char Key_LastHIDReport[2];
static unsigned char LastHoldState;
unsigned int BrightnessTable[2] = {20, 200};

void GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void ScanKey()
{
  unsigned char Key_HIDReport[8];
  unsigned int a;
  unsigned char NeedSendReport;
  
  for (a=0;a<8;a++)
    Key_HIDReport[a]=0;
  if ((Key_Scan(KEY_UP_GPIO   , KEY_UP_PIN))   ) Key_HIDReport[7] |= (1<<7);
  if ((Key_Scan(KEY_DOWN_GPIO , KEY_DOWN_PIN)) ) Key_HIDReport[6] |= (1<<1);
  if ((Key_Scan(KEY_LEFT_GPIO , KEY_LEFT_PIN)) ) Key_HIDReport[7] |= (1<<6);
  if ((Key_Scan(KEY_RIGHT_GPIO, KEY_RIGHT_PIN))) Key_HIDReport[6] |= (1<<0);
  if ((Key_Scan(KEY_A_GPIO    , KEY_A_PIN))    ) Key_HIDReport[7] |= (1<<3);
  if ((Key_Scan(KEY_B_GPIO    , KEY_B_PIN))    ) Key_HIDReport[7] |= (1<<1);
  if ((Key_Scan(KEY_X_GPIO    , KEY_X_PIN))    ) Key_HIDReport[7] |= (1<<4);
  if ((Key_Scan(KEY_Y_GPIO    , KEY_Y_PIN))    ) Key_HIDReport[7] |= (1<<2);
  if ((Key_Scan(KEY_L_GPIO    , KEY_L_PIN))    ) Key_HIDReport[6] |= (1<<2);
  if ((Key_Scan(KEY_R_GPIO    , KEY_R_PIN))    ) Key_HIDReport[6] |= (1<<6);
  if ((Key_Scan(KEY_SEL_GPIO  , KEY_SEL_PIN))  ) Key_HIDReport[6] |= (1<<3);
  if ((Key_Scan(KEY_START_GPIO, KEY_START_PIN))) Key_HIDReport[6] |= (1<<5);
    
  NeedSendReport = 0;
  for (a=0;a<1;a++)
    if (Key_LastHIDReport[a] != Key_HIDReport[a+6])  NeedSendReport = 1;
  for (a=0;a<1;a++)
    Key_LastHIDReport[a] = Key_HIDReport[a+6];
  if (NeedSendReport) 
  {
    UserToPMABufferCopy(Key_HIDReport, GetEPTxAddr(ENDP1), 8);
    SetEPTxValid(ENDP1);
  }
}

int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */     

   
  USART1_Config();
  printf("\r\n");
  printf("\r\nDev Board Debug Output\r\n");
  printf("     Designed by Zweb\r\n");
  printf("STM32F103C8 64K ROM 20K RAM\r\n\r\n");

  Delayms(1000);
  LCD_Init();
  TIM1_PWM_Init();
  GPIO_Config();
  BL_ON();
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  USB_Interrupts_Config();    
  Set_USBClock();   
  USB_Init();	   
  
  Key_GPIO_Config();
  Key_LastHIDReport[0] = 0;
  Key_LastHIDReport[1] = 0;
  LastHoldState = Key_Scan(KEY_HOLD_GPIO, KEY_HOLD_PIN);
  TIM1_PWM_Duty(BrightnessTable[LastHoldState]);
  while (1)
  {
    //Joystick_Send(PS2_DATA_BUF[0],PS2_DATA_BUF[1],-(PS2_DATA_BUF[2]),PS2_DATA_BUF[3]);
    ScanKey();
    if (LastHoldState != Key_Scan(KEY_HOLD_GPIO, KEY_HOLD_PIN))
    {
      LastHoldState = !LastHoldState;
      TIM1_PWM_Duty(BrightnessTable[LastHoldState]);
    }
    Delayms(30);
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\\r\\n", file, line) */
  printf("\\033[1;40;41mERROR:Wrong parameters value: file %s on line %d\\033[0m\\r\\n", file, line)
  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
