#include "rtc.h" 		    
//Mini STM32开发板
//RTC实时时钟 驱动代码			 
//正点原子@ALIENTEK
//2010/6/6
	 
 
static void RTC_NVIC_Config(void)
{	
  NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTC全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//先占优先级1位,从优先级3位
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//先占优先级0位,从优先级4位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//使能该通道中断
	NVIC_Init(&NVIC_InitStructure);		//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

//实时时钟配置
//初始化RTC时钟,同时检测时钟是否工作正常
//BKP->DR1用于保存是否第一次配置的设置
//返回0:正常
//其他:错误代码

u8 RTC_Init(void)
{
	//检查是不是第一次配置时钟
//	u8 temp=0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问  
	BKP_DeInit();	//复位备份区域 	
	RCC_LSICmd(ENABLE);
//	RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);		//设置RTC时钟(RTCCLK),选择LSI作为RTC时钟  
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);	//检查指定的RCC标志位设置与否,等待低速晶振就绪
//	{
//		temp++;
//		SysTick_Delay_Ms(10);
//	}
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);		//设置RTC时钟(RTCCLK),选择LSI作为RTC时钟      
	RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟  
	RTC_WaitForSynchro();		//等待RTC寄存器同步 
	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
	 
//		RTC_ITConfig(RTC_IT_SEC, ENABLE);		//使能RTC秒中断
//	RTC_EnterConfigMode();/// 允许配置	
	RTC_SetPrescaler((40000/50)-1); //设置RTC预分频的值		656计数值为20ms左右
	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成

	RTC_ExitConfigMode(); //退出配置模式  

	RTC_NVIC_Config();//RCT中断分组设置		    				     
	return 0; //ok

}		 				    
//RTC时钟中断
//每20ms触发一次  
//extern u16 tcnt; 
void RTC_IRQHandler(void)
{		 
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//秒钟中断
	{	
//		if(PWM1_R)PWM1_LOW;	
//		else PWM1_HIGH;	
			EXTI->IMR |= EXTI_Line9;  //开放中断
			RTC_ITConfig(RTC_IT_SEC, DISABLE);		//禁止RTC秒中断		
			RTC_WaitForLastTask();	 		
 	}
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//闹钟中断
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);		//清闹钟中断	  	

  	} 				  								 
	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);		//清闹钟中断
	RTC_WaitForLastTask();	  	    						 	   	 
}


