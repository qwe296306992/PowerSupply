#ifndef PROTECT_H
#define PROTECT_H

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
#include "usart.h"
#include "can.h"
#include "adc.h"
#include <math.h>


#define ADC_Buffer_Size            750  //20ms
#define ADC_Calculate              375	//存375个ADC采样值，大概10ms

extern u8 OVP_InputOrOutput_Flag;   //0代表输入OVP，1代表输出OVP，2代表无报警
extern u8 OCP_SoftOrHard_Flag;//0代表软件OCP，1代表硬件OCP，2代表无报警
extern u8 OTP_SensorX;				 //1代表Sensor1报警，2代表Sensor2报警，3代表Sensor3报警，0代表无报警
extern FunctionalState keys_EN;//按键使能控制

extern int CRL_Voltage;        //定义过欠压变量

extern u32 TIM_PROTECT_time;
extern void TIM_PROTECT_Init(void);
extern void TIM_P_Protect_IT(void);
extern void EXTI11_I_Protect_IT(void);
extern void EXTI10_I_Protect_IT(void);
extern void EXTI5_Temperature_Protect_IT(void);
extern void EXTI6_Temperature_Protect_IT(void);
extern void EXTI7_Temperature_Protect_IT(void);
extern void ADC_V_Protect_IT(void);
extern void Check_T_I_Switch(void);

void TestCurrent_GPIO_Config(void);
extern void TestCurrent(void);















#endif /*PROTECT*/



