#ifndef __TIM_OLED_H
#define __TIM_OLED_H

#include "stm32f10x.h"
#include "spi_oled.h"
#include "keys.h"
#include "stm32f10x_it.h"

//宏定义光标是否要显示
#define Cursor_On               1
#define Cursor_Off              0

//宏定义现在设定的是RS232的ID号还是波特率
#define RS232_First_Select      0
#define RS232_Set_ID            1
#define RS232_Set_BaudRate      2

//宏定义现在设定的是CAN的ID号还是波特率
#define CAN_First_Select      0
#define CAN_Set_ID            1
#define CAN_Set_BaudRate      2

//宏定义在cycle中按enter的次数
#define Cycle_Enter_0               0
#define Cycle_Enter_1               1
#define Cycle_Enter_2               2
#define Cycle_Enter_3               3

//宏定义记录在DELAY中按enter次数
#define Delay_Enter_0               0
#define Delay_Enter_1               1
#define Delay_Enter_2               2

//宏定义现在设定的是V-Rise的s还是ms
#define V_Rise_Enter_0              0
#define V_Rise_Enter_1              1
#define V_Rise_Enter_2              2

//宏定义现在设定的是V-Fall的s还是ms
#define V_Fall_Enter_0              0
#define V_Fall_Enter_1              1
#define V_Fall_Enter_2              2

//宏定义现在设定的是I-Rise的s还是ms
#define I_Rise_Enter_0              0
#define I_Rise_Enter_1              1
#define I_Rise_Enter_2              2

//宏定义现在设定的是I-Fall的s还是ms
#define I_Fall_Enter_0              0
#define I_Fall_Enter_1              1
#define I_Fall_Enter_2              2








extern u8 RS232_Set_ID_BaudRate;
extern u8 CAN_Set_ID_BaudRate;
extern char String_RS232_ID[5];
extern u16 RS232_ID;

extern char String_RS232_BaudRate[10];
extern u32 RS232_BaudRate;

extern char String_CAN_ID[5];
extern u16 CAN_ID;     

extern u32 CAN_BaudRate; 
extern char String_CAN_BaudRate[10];

extern void TIM5_Oled_IT(void);
extern void TIM_OLED_Init(void);
extern void Display_RS232_ID(u16 RS232_ID);
extern void Display_RS232_BaudRate(u32 RS232_BaudRate);
extern void Display_CAN_ID(u16 RS232_ID);
extern void Display_CAN_BaudRate(u32 RS232_BaudRate);








#endif /* __TIM_OLED_H */




