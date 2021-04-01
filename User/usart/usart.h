#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include "usart.h"
#include "tim_oled.h"
#include "can.h"

void Usart2_Config(uint32_t BraudRate);
extern void RS232_IT(void);



#endif
