#ifndef __TIM_CYCLE_H
#define __TIM_CYCLE_H

#include "stm32f10x.h"
#include "keys.h"

#define Cycle_Function_Off       0
#define Cycle_Function_On        1

extern u8 Cycle_Function_On_Off;


extern void TIM_CYCLE_Init(void);
extern void TIM_CYCLE_IT(void);
extern void TIM_CYCLE_ON(void);
extern void TIM_CYCLE_OFF(void);




















#endif /*__TIM_CYCLE_H*/







