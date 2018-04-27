#include "pwm.h"

 void TIM1_GPIO_Config(void)
 {
 GPIO_InitTypeDef GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);
//����AFIO//����TIM1
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_TIM1, ENABLE);

//	GPIO_AFIODeInit();  //�տ�ʼû�������䣬����һֱû����ӳ��ɹ�
//    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1 , ENABLE); //���������ӳ�书�ܺ���

//Step2. GPIO����Ӧ���ã�ΪAF���
//PA.8/9������ΪTIM1��OC1�����
GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;  //C8T6�ĵڶ���ͨ��������
GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure);

////PB.13/14������ΪTIM1_CH1N��TIM1_CH2N�����
//GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7 ;  // 
//GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
//GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//GPIO_Init(GPIOA, &GPIO_InitStructure);
}
//Step3. TIMģ���ʼ��
void TIM_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
//    TIM_BDTRInitTypeDef TIM_BDTRInitStructure;

    //TIM1�������������ã�����PWMƵ�ʣ�
    //Ƶ��=TIM1_CLK/(ARR+1)
    TIM_BaseInitStructure.TIM_Period = 1000-1;
    TIM_BaseInitStructure.TIM_Prescaler = 72-1;
    TIM_BaseInitStructure.TIM_ClockDivision = 0;
    TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_BaseInitStructure);
    //����ARR��Ӱ�ӼĴ�����ֱ�����������¼��Ÿ������ã�
    TIM_ARRPreloadConfig(TIM1, ENABLE);


    //TIM1_OC1ģ�����ã�����1ͨ��ռ�ձȣ�
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    //����CCR1�Ĵ�����Ӱ�ӼĴ�����ֱ�����������¼��Ÿ������ã�
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

//    //TIM2_OC2ģ�����ã�����2ͨ��ռ�ձȣ�
//    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
//    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
//    TIM_OCInitStructure.TIM_Pulse = 0;
//    TIM_OC4Init(TIM1, &TIM_OCInitStructure);
//    //����CCR2�Ĵ�����Ӱ�ӼĴ�����ֱ�����������¼��Ÿ������ã�
//    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
//   
//    //��������
//    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
//    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
//    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
//    TIM_BDTRInitStructure.TIM_DeadTime = 0x0;  //�������������С0-0xff
//    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
//    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
//    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
//    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
   
    //TIM1����
    TIM_Cmd(TIM1, ENABLE);
    //TIM1_OCͨ�����PWM��һ��Ҫ�ӣ�
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

//void Direction_init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0 | GPIO_Pin_1;
//	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOA,GPIO_Pin_0 | GPIO_Pin_1);
//}

//void Dir(void)
//{
//	GPIO_SetBits(GPIOA,GPIO_Pin_0);
//	GPIO_SetBits(GPIOA,GPIO_Pin_1);

//	TIM1->CCER &= (uint16_t)~((uint16_t)TIM_CCER_CC1P);
//	TIM1->CCER |= (uint16_t)TIM_OCPolarity_Low;
//		
//	TIM1->CCER &= (uint16_t)~((uint16_t)TIM_CCER_CC4P);
//	TIM1->CCER |= (uint16_t)TIM_OCPolarity_Low << 12;	
//}
//void DirN(void)
//{
//	 GPIO_ResetBits(GPIOA,GPIO_Pin_0);
//	 GPIO_ResetBits(GPIOA,GPIO_Pin_1);

//	 TIM1->CCER &= (uint16_t)~((uint16_t)TIM_CCER_CC1P);	   
//	 TIM1->CCER |= (uint16_t)TIM_OCPolarity_High;	 ////����ʱ������ֵС�ڡ�CCR1_Val��ʱΪ�ߵ�ƽ
//		
//	 TIM1->CCER &= (uint16_t)~((uint16_t)TIM_CCER_CC4P);
//	 TIM1->CCER |= (uint16_t)TIM_OCPolarity_High << 12;
//}
