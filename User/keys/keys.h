#ifndef __KEYS_H
#define __KEYS_H

#include "stm32f10x.h"
#include "systick.h"
#include "spi_oled.h"
#include <math.h>
#include "tim_oled.h"
#include "at25.h"
#include "tim_delay.h"
#include "tim_cycle.h"
#include "usart.h"
#include "can.h"
#include "dac.h"
#include "tim_slope.h"

//按键宏定义
#define KeyRow_Port        GPIOB

#define KeyRow_1_Pin       GPIO_Pin_2

#define KeyRow_2_Pin       GPIO_Pin_1

#define KeyRow_3_Pin       GPIO_Pin_0

#define KeyCol_Port        GPIOE

#define KeyCol_1_Pin       GPIO_Pin_13

#define KeyCol_2_Pin       GPIO_Pin_12

#define KeyCol_3_Pin       GPIO_Pin_11

#define KeyCol_4_Pin       GPIO_Pin_10

#define KeyCol_5_Pin       GPIO_Pin_9

#define KeyCol_6_Pin       GPIO_Pin_8

#define KeyCol_7_Pin       GPIO_Pin_7

//定义GPIOE寄存器前七位的值
#define KeyCol_Status (u16)(((GPIOE->IDR)>>7)&0x007F)


//宏定义键盘/编码器输入
#define Keys_Mode               0
#define Encoder_Mode            1

//宏定义串口波特率
#define Usart300                1
#define Usart600                2
#define Usart1200               3
#define Usart2400               4
#define Usart4800               5
#define Usart9600               6
#define Usart19200              7
#define Usart38400              8 
#define Usart43000              9 
#define Usart56000              10 
#define Usart57600              11 
#define Usart115200             12 

//宏定义CAN波特率
#define CAN5               1
#define CAN10              2
#define CAN20              3
#define CAN25              4
#define CAN40              5
#define CAN50              6
#define CAN80              7
#define CAN100             8 
#define CAN125             9 
#define CAN200             10 
#define CAN250             11 
#define CAN400             12 
#define CAN500             13
#define CAN800             14

//宏定义使用哪种模式的电压
#define Use_Vint           0
#define Use_Vext           1



extern u8 Shift_Not_Shift;   
extern u8 ON_OFF;  
extern u8 OUTPUT_NOT_OUTPUT;
extern u8 CV_CC_CP;                    
extern u8 DELAY_NOT_DELAY;   
extern u8 Preset_Actual;        
extern u8 LOCK_UNLOCK; 
extern u8 Recall_Save;
extern int Recall_Save_Number;
extern int First_Selection;
extern u8 Second_Selection;
extern u8 Display_Interface_Mode;
extern int Cursor_Position; 
extern u8 Keys_Encoder_Mode;
extern u8 Keys_Input_Empty_Full;
extern u8 RS232_BaudRate_State;
extern u8 CAN_BaudRate_State;

extern u8 Cycle_Enter;                
extern u16 Cycle_On_s;                            
extern u16 Cycle_On_ms;                                       
extern u16 Cycle_Close_s;                                           
extern u16 Cycle_Close_ms;                                      
extern char String_Cycle_On[11];
extern char String_Cycle_Close[11];

extern u8 Delay_Enter;
extern u8 Delay_h;
extern u8 Delay_m;
extern u8 Delay_s;
extern char String_Delay[10];

extern uint8_t ScanKeyStatus(GPIO_TypeDef*GPIOx,uint16_t GPIO_Pin);
extern void Keys_Init(void);
extern void Keys(void);
extern float String_To_Float(char String[]);
extern void Display_Oled_Delay(void);
extern void Keys_Handler(FunctionalState keys_EN);


extern u8 V_Rise_Enter;                                            
extern u16 V_Rise_s;                                                      
extern u16 V_Rise_ms;                                                   
extern char String_V_Rise[11];                                 
  

extern u8 V_Fall_Enter;                                            
extern u16 V_Fall_s;                                                      
extern u16 V_Fall_ms;                                                   
extern char String_V_Fall[11];                                 


extern u8 I_Rise_Enter;                                            
extern u16 I_Rise_s;                                                      
extern u16 I_Rise_ms;                                                   
extern char String_I_Rise[11];


extern u8 I_Fall_Enter;                                            
extern u16 I_Fall_s;                                                      
extern u16 I_Fall_ms;                                                   
extern char String_I_Fall[11];      

extern char Use_Vint_Or_Vext_Flag;


void Key_0(void);
void Key_1(void);
void Key_2(void);
void Key_3(void);
void Key_4(void);
void Key_5(void);
void Key_6(void);
void Key_7(void);
void Key_8(void);
void Key_9(void);
void Key_ESC(void);
void Key_V_set(void);
void Key_I_set(void);
void Key_Up(void);
void Key_Recall_Save(void);
void Key_Preset_Lock(void);
void Key_Enter(void);
void Key_Decimal_Point(void);
void Key_Shift(void);
void Key_ON_OFF(void);
void Key_Down(void);



#endif /*__KEYS_H*/



