#include "tim_cycle.h"

u8 Cycle_Function_On_Off = Cycle_Function_Off;

u32 Cycle_On_Time;
u32 Cycle_Close_Time;

//配置中断优先级
static void TIM_CYCLE_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

//配置TIM7
static void TIM_CYCLE_Mode_Config(void)//1ms中断
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		
	  // 开启定时器时钟,即内部时钟CK_INT=72M
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
		// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period=7199;
	  // 时钟预分频数
    TIM_TimeBaseStructure.TIM_Prescaler= 9;		
		// 时钟分频因子 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		// 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	
		// 重复计数器的值
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	  // 初始化定时器
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
	
		// 清除计数器中断标志位
    TIM_ClearFlag(TIM7, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(TIM7, TIM_IT_Update,ENABLE);
		
		// 使能计数器
    TIM_Cmd(TIM7, DISABLE);
}

//TIM7初始化
void TIM_CYCLE_Init(void)
{
	TIM_CYCLE_NVIC_Config();
	TIM_CYCLE_Mode_Config();		
}

/**
  * @brief  打开CYCLE，打开TIM7定时器
  * @param  None
  * @retval None
  */
void TIM_CYCLE_ON(void)
{
	Cycle_On_Time = Cycle_On_s * 1000 + Cycle_On_ms;
	Cycle_Close_Time = Cycle_Close_s * 1000 + Cycle_Close_ms;
	ON_OFF = ON;
	Power_On();
	TIM_Cmd(TIM7, ENABLE);
}

/**
  * @brief  关闭CYCLE，关闭TIM7定时器
  * @param  None
  * @retval None
  */
void TIM_CYCLE_OFF(void)
{
	TIM_Cmd(TIM7, DISABLE);	
	ON_OFF = OFF;
}




/**
  * @brief  TIM7的中断服务函数,用于DELAY
  * @param  None
  * @retval None
  */
void TIM_CYCLE_IT(void)
{
	if(ON_OFF == ON)    //开启状态，时间倒数
	{
		if(Cycle_On_Time > 0)
	  {
		  Cycle_On_Time --;
	  }
		else              //到时间了，关输出
		{
			Cycle_On_Time = Cycle_On_s * 1000 + Cycle_On_ms;
			Power_Off();
		}
	}
	else if(ON_OFF == OFF)
	{
		if(Cycle_Close_Time >0)
		{
			Cycle_Close_Time --;
		}
		else              //到时间了，开输出
		{
			Cycle_Close_Time = Cycle_Close_s * 1000 + Cycle_Close_ms;
			Power_On();
		}
	}
	TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);   //清除中断标志位		
}














