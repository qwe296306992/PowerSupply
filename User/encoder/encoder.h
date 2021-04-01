#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x.h"
#include "spi_oled.h"
#include "keys.h"
#include "stm32f10x_it.h"

//宏定义编码器（TIM4）的溢出值
#define TIM_Encoder_Period        4






extern void TIM2_Encoder_IT(void);
extern void Encode_Init(void);






#endif /* __ENCODER_H */

