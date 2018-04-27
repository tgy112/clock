#include "pwm.h"

 void TIM1_GPIO_Config(void)
 {
 GPIO_InitTypeDef GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);
//启动AFIO//启动TIM1
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_TIM1, ENABLE);

//	GPIO_AFIODeInit();  //刚开始没有这个语句，所以一直没有重映射成功
//    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM1 , ENABLE); //这个就是重映射功能函数

//Step2. GPIO做相应设置，为AF输出
//PA.8/9口设置为TIM1的OC1输出口
GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;  //C8T6的第二个通道不能用
GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure);

////PB.13/14口设置为TIM1_CH1N和TIM1_CH2N输出口
//GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7 ;  // 
//GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
//GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//GPIO_Init(GPIOA, &GPIO_InitStructure);
}
//Step3. TIM模块初始化
void TIM_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
//    TIM_BDTRInitTypeDef TIM_BDTRInitStructure;

    //TIM1基本计数器设置（设置PWM频率）
    //频率=TIM1_CLK/(ARR+1)
    TIM_BaseInitStructure.TIM_Period = 1000-1;
    TIM_BaseInitStructure.TIM_Prescaler = 72-1;
    TIM_BaseInitStructure.TIM_ClockDivision = 0;
    TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_BaseInitStructure);
    //启用ARR的影子寄存器（直到产生更新事件才更改设置）
    TIM_ARRPreloadConfig(TIM1, ENABLE);


    //TIM1_OC1模块设置（设置1通道占空比）
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    //启用CCR1寄存器的影子寄存器（直到产生更新事件才更改设置）
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

//    //TIM2_OC2模块设置（设置2通道占空比）
//    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
//    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
//    TIM_OCInitStructure.TIM_Pulse = 0;
//    TIM_OC4Init(TIM1, &TIM_OCInitStructure);
//    //启用CCR2寄存器的影子寄存器（直到产生更新事件才更改设置）
//    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
//   
//    //死区设置
//    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
//    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
//    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
//    TIM_BDTRInitStructure.TIM_DeadTime = 0x0;  //这里调整死区大小0-0xff
//    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
//    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
//    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
//    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
   
    //TIM1开启
    TIM_Cmd(TIM1, ENABLE);
    //TIM1_OC通道输出PWM（一定要加）
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
//	 TIM1->CCER |= (uint16_t)TIM_OCPolarity_High;	 ////当定时器计数值小于“CCR1_Val”时为高电平
//		
//	 TIM1->CCER &= (uint16_t)~((uint16_t)TIM_CCER_CC4P);
//	 TIM1->CCER |= (uint16_t)TIM_OCPolarity_High << 12;
//}
