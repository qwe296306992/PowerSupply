
#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"
#include "systick.h"
#include "tim_oled.h"
#include "SCR_Controller.h"
#include <math.h>

#define ADC_Average_Number         32
//#define ADC_Average_Number         8
//#define ADC_Average_Number         6


//#define ADC_Value_NUM 100*2  //收集个数  50


#define ADC_Vref 3.3;  //电流参考值

/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT         ((uint32_t)0x10000)
#define SPIT_LONG_TIMEOUT         ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))


//定义命令
#define Config1     (u16)((0x1<<15) || (0x0<<12) || (0x2<<9) || (0x1<<8) || (0x4<<5) || (0x0<<4) || (0x0<<3) || (0x1<<1) || (0x1<<0)) 








extern void SPI2_ADC_config(void);
extern uint32_t SPI_RW_Reg(uint16_t CofigReg);
//extern void DMA1_Channel4_IT(void);
extern void EXTI15_10_IT(void);

extern int16_t CH0[2],CH1[2],CH2[2],CH3[2];

float ADC_Current(void);
float ADC_VoltageEXT(void);
float ADC_VoltageINT(void);
float ADC_Power(void);
u16 test_ADC_Current(void);
void Average_Calculate(void);//计算平均值程序

int Bubble_Sort_Calculate(int16_t Array[],u16 n);

//extern void SPI2_ADC_config(void);
//extern uint32_t SPI_RW_Reg(uint16_t CofigReg);
////extern void DMA1_Channel4_IT(void);
//extern void EXTI15_10_IT(void);

//void data_exchange(void);
//void OUTPUT_CV_CC_CP(void);//输出状态，选择CV_CC_CP


#endif







