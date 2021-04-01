#include "softstart.h"
#include "protect.h"
#include "keys.h"
#include "SCR_Controller.h"

////配置中断优先级
//static void TIM_DELAY3_NVIC_Config(void)
//{
//    NVIC_InitTypeDef NVIC_InitStructure; 
//    // 设置中断组为0
////    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		
//		// 设置中断来源
//    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;	
//		// 设置主优先级为 0
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	 
//	  // 设置抢占优先级为3
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//}

void SoftStart_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能
	Power_AC_Off();
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //OUT_1  PB9
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
//	while(CRL_Voltage < 29*8);
//	Power_AC_On();
//	SysTick_Delay_Ms(100);//等待继电器及开关稳定接通
	SCR_GPIO_Config();//使能半控整流，通过软启动电阻给电容充电
	SysTick_Delay_Ms(200);//
}

void SS_DISADBE_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //OUT_2  PE0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOE, &GPIO_InitStructure);	
	

	
			SS_DISADBE_On();   
//			SS_DISADBE_Off();   

	
}





void Power_AC_Off(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
}

void Power_AC_On(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}


void SS_DISADBE_On(void)       //  SS脚接地，2895不工作   OUT_2高   off
{
	GPIO_SetBits(GPIOE, GPIO_Pin_0); 
	
}

void SS_DISADBE_Off(void)       //  SS脚不接地，2895工作   OUT_2低    on
{
	
	GPIO_ResetBits(GPIOE, GPIO_Pin_0);
	
}
//void OUT2_1(void)
//{
//	
//GPIO_SetBits(GPIOE, GPIO_Pin_0);
//}
//void OUT2_0(void)
//{
//	GPIO_ResetBits(GPIOE, GPIO_Pin_0);
//}

//void TIM1_SoftStatr_IT(void)
//{
//	SoftStart_Off();
//	TIM_ClearFlag(TIM1, TIM_FLAG_Update);                     //清中断标志位
//	
//  TIM_Cmd(TIM1, DISABLE);	
//		SoftStart_Off();
//}









