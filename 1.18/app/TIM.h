#ifndef __TIM_h
#define __TIM_h

#include "stm32f10x.h"

///////////////////////////////////////////////
//
//	计数器中断有关定义
//
///////////////////////////////////////////////
#define START_TIME  time=0;RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);TIM_Cmd(TIM2, ENABLE)
#define STOP_TIME  TIM_Cmd(TIM2, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE)

void TIM2_NVIC_Configuration(void);
void TIM2_Configuration(void);


///////////////////////////////////////////////
//
//	获取编码器有关定义
//
///////////////////////////////////////////////
//#define ENCODER_TIMER   TIM4  // Encoder unit connected to TIM3
////#define ENCODER_PPR           (u16)(100)   // number of pulses per revolution
//#define SPEED_BUFFER_SIZE 8

//#define COUNTER_RESET   (u16)0
//#define ICx_FILTER      (u8) 6 // 6<-> 670nsec

//#define TIMx_PRE_EMPTION_PRIORITY 1
//#define TIMx_SUB_PRIORITY 0

//#define ENCODER_TIM_PERIOD 0xffff//最大值预分频是65536-1
//#define MAX_COUNT          10000//10000也就是1ms内不会超过10000个脉冲

//void InitializeEncoder4(void);
//void InitializeEncoder3(void);
//s16   Getcount3(void);
//s16   Getcount4(void);

#endif
