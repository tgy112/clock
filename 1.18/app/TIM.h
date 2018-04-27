#ifndef __TIM_h
#define __TIM_h

#include "stm32f10x.h"

///////////////////////////////////////////////
//
//	�������ж��йض���
//
///////////////////////////////////////////////
#define START_TIME  time=0;RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);TIM_Cmd(TIM2, ENABLE)
#define STOP_TIME  TIM_Cmd(TIM2, DISABLE);RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE)

void TIM2_NVIC_Configuration(void);
void TIM2_Configuration(void);


///////////////////////////////////////////////
//
//	��ȡ�������йض���
//
///////////////////////////////////////////////
//#define ENCODER_TIMER   TIM4  // Encoder unit connected to TIM3
////#define ENCODER_PPR           (u16)(100)   // number of pulses per revolution
//#define SPEED_BUFFER_SIZE 8

//#define COUNTER_RESET   (u16)0
//#define ICx_FILTER      (u8) 6 // 6<-> 670nsec

//#define TIMx_PRE_EMPTION_PRIORITY 1
//#define TIMx_SUB_PRIORITY 0

//#define ENCODER_TIM_PERIOD 0xffff//���ֵԤ��Ƶ��65536-1
//#define MAX_COUNT          10000//10000Ҳ����1ms�ڲ��ᳬ��10000������

//void InitializeEncoder4(void);
//void InitializeEncoder3(void);
//s16   Getcount3(void);
//s16   Getcount4(void);

#endif
