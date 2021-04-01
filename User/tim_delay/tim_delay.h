#ifndef __TIM_DELAY_H
#define __TIM_DELAY_H

#include "stm32f10x.h"
#include "systick.h"
#include "keys.h"
#include "encoder.h"
#include "tim_oled.h"
#include "tim_cycle.h"

#define Delay_Function_Off           0
#define Delay_Function_On            1




extern u8 Delay_Function_On_Off;
extern u32 TIM_Delay_time;

extern void TIM_DELAY_Init(void);
extern void TIM_DELAY_IT(void);
extern void TIM_DELAY_ON(void);
extern void TIM_DELAY_OFF(void);























#endif /*__TIM_DELAY_H*/


