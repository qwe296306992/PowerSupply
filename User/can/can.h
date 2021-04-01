#ifndef __CAN_H
#define __CAN_H

#include "stm32f10x.h"
#include "tim_oled.h"
#include "usart.h"




void CAN_Config(uint16_t BraudRate);
void CAN1_Receive_Handler(void);

extern void CAN1_IT(void);
//extern uint16_t  WireRES;
//extern uint8_t  WireRES_OnOFF;
//extern  void CAN_function20(void);


#endif
