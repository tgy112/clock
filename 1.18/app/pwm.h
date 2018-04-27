#ifndef __PWM_H
#define	__PWM_H

#include "stm32f10x.h"

#define Low tmpccer

void TIM1_PWM_Init(void);
void TIM1_GPIO_Config(void);
void TIM_Configuration(void);
void Dir(void);
void DirN(void);
void Direction_init(void);

#endif /* __PWM_OUTPUT_H */
