//#ifndef _SCR_CONTROLLER_
//#define _SCR_CONTROLLER_

//#include "stm32f10x.h"
//#include "systick.h"
//#include "adc.h"




//extern void SCR_TIM1_IT(void);
//extern void SCR_Controller_Init(void);
//extern void EXTI9_ZERO_CROSS_DETECT_IT(void);

//#endif/*_SCR_CONTROLLER_*/

#ifndef _SCR_TIM1_
#define _SCR_TIM1_

#include "stm32f10x.h"
#include "systick.h"
#include "keys.h"
#include "softstart.h"
#include "wdg.h"
#include "rtc.h" 
#include "dac.h"
#include "adc.h"
#include "tim_slope.h"

#define PWM1CLC      RCC_APB2Periph_GPIOC
#define PWM1         GPIOC
#define PWM1PIN      GPIO_Pin_8
#define PWM1_HIGH GPIO_SetBits(PWM1, PWM1PIN)
#define PWM1_LOW GPIO_ResetBits(PWM1, PWM1PIN)
#define PWM1_R GPIO_ReadOutputDataBit(PWM1, PWM1PIN)

#define PWM0CLC      RCC_APB2Periph_GPIOD
#define PWM0         GPIOD   //  PC7 改为PD15
#define PWM0PIN      GPIO_Pin_15
#define PWM0_HIGH GPIO_SetBits(PWM0, PWM0PIN)
#define PWM0_LOW GPIO_ResetBits(PWM0, PWM0PIN)
#define PWM0_R GPIO_ReadOutputDataBit(PWM0, PWM0PIN)

#define TRIGGERPORT     GPIOC
#define TRIGGERPIN      GPIO_Pin_9
#define TRIGGEREXITPIN  EXTI_Line9
#define TRIGGERCLC      RCC_APB2Periph_GPIOC
#define TRIGGERSOURCE   GPIO_PortSourceGPIOC
#define TRIGGERSOURCEPIN   GPIO_PinSource9

extern uint16_t SCR_shift_T;  // (u16)5000	//7800用调压器，输出143
#define	SCR_ENABLE_POWER	(u16)300	//小于300W用半控
#define	SCR_DISABLE_POWER	(u16)400	//大于500W开全控
#define	Power_limit 50

#define PWM0_1	TIM3->CCR1
#define PWM0_0	TIM3->CCR2
#define PWM1_1	TIM3->CCR3
#define PWM1_0	TIM3->CCR4


extern FunctionalState SCR_P_FLAG;

extern u16 t_count;//周期计数@1us
extern void SCR_TIM1_IT(void);

void SCR_GPIO_Config(void);

extern void SCR_Controller_Init(void);
extern void EXTI9_ZERO_CROSS_DETECT_IT(void);

extern void SCR_On_Off(FunctionalState NewState);

extern void I_Compare(void);

void SCR_Ser(float P);
#endif/*_SCR_TIM1_*/
