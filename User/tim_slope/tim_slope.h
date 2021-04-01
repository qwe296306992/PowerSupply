#ifndef __TIM_SLOPE_H
#define __TIM_SLOPE_H

#include "stm32f10x.h" 
#include "spi_oled.h"
#include "systick.h"
#include "keys.h"
#include "encoder.h"
#include "tim_oled.h"
#include "at25.h"
#include "tim_delay.h"
#include "tim_cycle.h"
#include "adc.h"
#include "protect.h"
#include "usart.h"
#include "can.h"
#include "adc.h"
#include "dac.h"
#include "SCR_Controller.h"

#define V_Rise_Off        0
#define V_Rise_On         1

#define V_Fall_Off        0
#define V_Fall_On         1

#define I_Rise_Off        0
#define I_Rise_On         1

#define I_Fall_Off        0
#define I_Fall_On         1

#define P_Rise_K					10 			//功率缓升的斜率系数
																	//100---->1kW     0.01s
																	// 10---->1kW   	 0.1s
																	//  1---->1kW    	   1s

extern u8 V_Rise_On_Off;
extern u8 V_Fall_On_Off;
extern u8 I_Rise_On_Off;
extern u8 I_Fall_On_Off;

extern float Start_Set_Power; 

extern u32 P_Rise_Time;
extern float Slope_Power;

extern void TIM_SLOPE_Init(void);
extern void TIM_SLOPE_IT(void);



extern void V_Rise_Function_On(void);
extern void V_Fall_Function_On(void);
extern void I_Rise_Function_On(void);
extern void I_Fall_Function_On(void);

extern void CP_Slope_Init(void);//CP模式下的缓升定时器TIM1初始化程序，定时时间为1ms
extern void P_Slope_Function_On(void);//CP模式下缓升功能，一次加10W，一次1ms，大概1kW对应加载时间为0.1s
extern void CP_TIM1_SLOPE_IT(void);//定时器1的中断服务函数

extern void Vint_I_SLOPE_DA(void);  //CP模式下，电压电流缓升至DA值，4V
extern void Vint_I_TIM1_SLOPE_IT(void);//定时器1的中断服务函数

extern void I_Fall_DA(void);  //定义CC缓降DA，非用户定义
extern void I_Slope_Function_On_1(void);



extern void V_Slope_Function_On_1(void);    //定义CV缓升DA，非用户定义

extern void V_Slope_Function_On_Vext(void);  //定义Vext缓升DA

#endif /*__TIM_SLOPE_H*/




























