#include "beep_pwm.h"
#include "lcd.h"
#include "key.h"

void TIM1_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//��ʼ��TIM1
void TIM1_Mode_Config(void)
{
  TIM_TimeBaseInitTypeDef  TIM_BaseInitStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  u16 CCR1_Val = 127;
  
  TIM_DeInit(TIM1);
  
TIM_BaseInitStructure.TIM_Period = 384-1;//576-1;//   //125kHZ  (72M/125k)-1=575
    TIM_BaseInitStructure.TIM_Prescaler = 0;
    TIM_BaseInitStructure.TIM_ClockDivision = 0;
    TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_BaseInitStructure);
    //����ARR��Ӱ�ӼĴ�����ֱ�����������¼��Ÿ������ã�
    TIM_ARRPreloadConfig(TIM1, ENABLE);
    //TIM1_OC1ģ�����ã�����1ͨ��ռ�ձȣ�
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity =TIM_OCNPolarity_High; //TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Set;         //�������״̬Ϊ1
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;   //�����������״̬Ϊ0 
    TIM_OCInitStructure.TIM_Pulse = 192;///288;  // 50%ռ�ձ�= 576/2
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    //����CCR1�Ĵ�����Ӱ�ӼĴ�����ֱ�����������¼��Ÿ������ã�
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);   
    //TIM1����
    TIM_Cmd(TIM1, ENABLE);
    //TIM1_OCͨ�����PWM��һ��Ҫ�ӣ�
    TIM_CtrlPWMOutputs(TIM1, ENABLE);}

//TIM1 PWM��ʼ��
void TIM1_PWM_Init(void)
{
  TIM1_GPIO_Config();
  TIM1_Mode_Config();  
}
