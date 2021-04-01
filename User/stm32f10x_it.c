

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "spi_oled.h"
#include "encoder.h"
#include "tim_oled.h"
#include <string.h>
#include "encoder.h"
#include "tim_cycle.h"
#include "tim_delay.h"
#include "protect.h"
#include "usart.h"
#include "can.h"
#include "adc.h"
#include "tim_slope.h"
#include "softstart.h"
#include "SCR_Controller.h"




//extern u16 t_flag;



/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}




/*-----------------------------------------功率缓升部分-------------------------------------------*/
void TIM1_UP_IRQHandler(void)
{
//	TIM1_SoftStatr_IT();
//	CP_TIM1_SLOPE_IT(); 
//	  Vint_I_TIM1_SLOPE_IT();
//	SCR_TIM1_IT();
	TIM_SLOPE_IT();
}


void TIM1_CC_IRQHandler(void)
{
	SCR_TIM1_IT();
}

/*---------------------------------------------定时器-------------------------------------------*/


void TIM2_IRQHandler(void)
{
	TIM2_Encoder_IT();
}

void TIM3_IRQHandler(void)
{
//  TIM_SLOPE_IT();
	SCR_TIM1_IT();
}

void TIM5_IRQHandler(void)
{
	TIM5_Oled_IT();
}

void TIM6_IRQHandler(void)
{
  TIM_DELAY_IT();
}

void TIM7_IRQHandler(void)
{
  TIM_CYCLE_IT();
}

/*-----------------------------------------------保护---------------------------------------*/

void TIM4_IRQHandler(void)
{
  TIM_P_Protect_IT();
}

void EXTI9_5_IRQHandler(void)
{
	EXTI5_Temperature_Protect_IT();
	EXTI6_Temperature_Protect_IT();
	EXTI7_Temperature_Protect_IT();
	EXTI9_ZERO_CROSS_DETECT_IT();
}

void DMA1_Channel1_IRQHandler(void)
{
	ADC_V_Protect_IT();
}

/*--------------------------------------通信-------------------------------------*/

//USART2 RS232接受中断
void USART2_IRQHandler(void)
{
	RS232_IT();	
}

//USART2 发送完成中断
void DMA1_Channel7_IRQHandler(void)
{
  RS232_IT();
}

//CAN1FIFO0 接受中断
//void CAN1_RX1_IRQHandler(void)
//{
//  CAN1_IT();
//}

void CAN1_RX0_IRQHandler(void)
{
  CAN1_IT();
}



/*----------------------------------ADC-----------------------------------*/

void EXTI15_10_IRQHandler(void)  
{ 
	
		//电流保护
//	EXTI11_I_Protect_IT();
	
	//ADC
	EXTI15_10_IT();
	
//  EXTI10_I_Protect_IT();
}

//void DMA1_Channel4_IRQHandler(void)
//{
//	DMA1_Channel4_IT();
//}





/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
