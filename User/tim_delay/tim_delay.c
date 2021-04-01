#include "tim_delay.h"

u32 TIM_Delay_time;

u8 Delay_Function_On_Off = Delay_Function_On;            //DELAY功能是否开启标志




//配置中断优先级
static void TIM_DELAY_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

//配置TIM6,定时周期为1S
static void TIM_DELAY_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		
	  // 开启定时器时钟,即内部时钟CK_INT=72M
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
		// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period=9999;
	  // 时钟预分频数
    TIM_TimeBaseStructure.TIM_Prescaler= 7199;		
		// 时钟分频因子 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		// 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	
		// 重复计数器的值
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	  // 初始化定时器
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	
		// 清除计数器中断标志位
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(TIM6, TIM_IT_Update,ENABLE);
		
		// 使能计数器
    TIM_Cmd(TIM6, DISABLE);
}

//TIM6初始化
void TIM_DELAY_Init(void)
{
	TIM_DELAY_NVIC_Config();
	TIM_DELAY_Mode_Config();		
}

/**
  * @brief  开启DELAY功能，设置定时器时间和使能定时器
  * @param  None
  * @retval None
  */
void TIM_DELAY_ON(void)
{
	TIM_Delay_time = Delay_h * 3600 + Delay_m * 60 + Delay_s;
	DELAY_NOT_DELAY = DELAY;
	ON_OFF = OFF;
	TIM_Cmd(TIM6, ENABLE);	
}

/**
  * @brief  关闭DELAY功能
  * @param  None
  * @retval None
  */
void TIM_DELAY_OFF(void)
{
	TIM_Cmd(TIM6, DISABLE);
	DELAY_NOT_DELAY = NOT_DELAY;
}


/**
  * @brief  TIM6的中断服务函数,用于DELAY
  * @param  None
  * @retval None
  */
void TIM_DELAY_IT(void)
{
	//计时
	if(TIM_Delay_time > 0)
	{
		TIM_Delay_time --;
	}
	else  //到时间了
	{
    TIM_DELAY_OFF();
		
		if(Cycle_Function_On_Off == Cycle_Function_On)    //打开了循环功能，调用循环函数
		{
			TIM_CYCLE_ON();
		}
		else if(Cycle_Function_On_Off == Cycle_Function_Off)   //没打开循环功能，直接输出
		{
			Power_On();
		}
	}
	TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);   //清除中断标志位	
}







