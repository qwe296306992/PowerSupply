#include "SCR_Controller.h"
//#include "systick.h"

//开始
//uint16_t SCR_shift_T = 7800;	//7800用调压器，输出143
uint16_t SCR_shift_T = 5000;
uint16_t temp;
u16 t_count = 70;//周期计数@1us
FunctionalState SCR_shift = ENABLE;//软切换状态标志
FunctionalState SCR_ON = DISABLE;//软切换方向标志,全控到半控
FunctionalState SCR_OFF = ENABLE;//软切换方向标志，半控到全控

FunctionalState SCR_P_FLAG = DISABLE; //

//u16 t = 7800;

//FunctionalState SCR_shift = DISABLE;//软切换状态标志
//FunctionalState SCR_ON = DISABLE;//软切换方向标志,全控到半控
//FunctionalState SCR_OFF = ENABLE;//软切换方向标志，半控到全控

//配置中断优先级 
static void ZERO_CROSS_DETECT_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(TRIGGERCLC | RCC_APB2Periph_AFIO, ENABLE);
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
	//中断源
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	//主优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	//抢占优先级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;

	//使能
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


//配置外部中断
static void ZERO_CROSS_DETECT_EXTI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(TRIGGERCLC | RCC_APB2Periph_AFIO, ENABLE);
	
	//终端GPIO初始化
	GPIO_InitStructure.GPIO_Pin  = TRIGGERPIN;    //PC9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
 	GPIO_Init(TRIGGERPORT, &GPIO_InitStructure);
	
	//中断配置
	GPIO_EXTILineConfig(TRIGGERSOURCE, TRIGGERSOURCEPIN);
	EXTI_InitStructure.EXTI_Line = TRIGGEREXITPIN;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能中断
	EXTI_Init(&EXTI_InitStructure);
}

//可控硅触发引脚初始化
void SCR_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(PWM0CLC, ENABLE);
	RCC_APB2PeriphClockCmd(PWM1CLC, ENABLE);
	//输出引脚初始化
	GPIO_InitStructure.GPIO_Pin = PWM0PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PWM0, &GPIO_InitStructure);
	
	//输入引脚初始化
	GPIO_InitStructure.GPIO_Pin = PWM1PIN;
	GPIO_Init(PWM1, &GPIO_InitStructure);	
//	
//	PWM0_LOW;
//	PWM1_LOW;	
	
	PWM0_HIGH;
	PWM1_HIGH;	
}

void SCR_TIM1_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	
		//TIM3的中断优先级配置
	// 设置中断组为2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
	// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	

	// 设置主优先级为0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	// 设置子优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = 19500;                  //设置自动重装载寄存器周期值//50Hz
  TIM_TimeBaseStructure.TIM_Prescaler = (72-1);              //设置预分频值  通过减少分频计数器的值来降低误差
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;   //设置时钟分割
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;          //重复计数设置
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);           //参数初始化
//	PWM0_1 = 9500;	//PWM0 	开
//	PWM0_0 = 10700;	//PWM0 	关
//	PWM1_1 = 19500;	//PWM1	开
//	PWM1_0 = 19600;	//PWM1	关
	PWM0_1 = 7800;	//PWM0 	开
	PWM0_0 = 9000;	//PWM0 	关
	PWM1_1 = 17800;	//PWM1	开
	PWM1_0 = 19000;	//PWM1	关	

	// 开启计数器中断
  TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 | TIM_IT_Update,ENABLE);
	
	TIM_ClearFlag(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 | TIM_IT_Update);   
		
	// 不使能计数器
	TIM_Cmd(TIM3, DISABLE);//由过零检测按照正确时序打开
	TIM_ARRPreloadConfig(TIM3, ENABLE);
}


//外部中断为PC9，脉冲引脚PC8，PC7，TIM3计数周期为1us，TIM3中断频率由过零中断频率决定，大概为50Hz
void SCR_Controller_Init(void)
{
	PWM0_LOW;
	PWM1_LOW;	
//	SysTick_Delay_Ms(41);
//	Power_AC_On();//先给电容充电再开继电器
	
	ZERO_CROSS_DETECT_NVIC_Config();
	ZERO_CROSS_DETECT_EXTI_Init();
	SCR_TIM1_Init();
	RTC_Init();
//	SCR_GPIO_Config();//在软启动处调用
//	WWDG_Init(41,32,WWDG_Prescaler_8);//计数器值为0x29,窗口寄存器为0x20,分频数为8	 
}


/**
  * @brief  EXTI9过零检测的中断服务函数
  * @param  None
  * @retval None
  */
void EXTI9_ZERO_CROSS_DETECT_IT(void)
{
	if(EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line9);
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9) == 0)
		{
			if( (TIM3->CR1 & TIM_CR1_CEN) == 0)  //使能信号，0代表定时器关，1代表开
			{
				TIM3->CNT = 0;
				TIM_Cmd(TIM3, ENABLE); //此处可能有干扰影响，定时器没有开启，中断屏蔽了，导致半控失效	
//				while((TIM3->CR1 & TIM_CR1_CEN) == 0)TIM_Cmd(TIM3, ENABLE);
				RTC_ITConfig(RTC_IT_SEC, ENABLE);	
				RTC_WaitForLastTask();
				while((RTC->CRH & RTC_IT_SEC) == 0);
				EXTI->IMR &=~EXTI_Line9;  //屏蔽中断				
			}
////			else TIM_Cmd(TIM3, DISABLE);
//			if( (TIM3->CR1 & TIM_CR1_CEN) == 0) TIM_Cmd(TIM3, ENABLE);
		}
	}
}

FunctionalState WWDG_ON = DISABLE;
/**
  * @brief  TIM3的中断服务函数
  * @param  None
  * @retval None
  */
void SCR_TIM1_IT(void)//中断周期20ms
{		
	switch(TIM3->SR & (TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4))
	{
		case TIM_IT_CC1:
		{			
			PWM0_HIGH; 
			//触发导通角自动双向渐变
			if(SCR_shift == ENABLE)//半控/全控软切换
			{
				u16 PWM_buff = (u16)PWM0_1;
			  if(SCR_OFF == ENABLE)
				{
					if(PWM_buff > SCR_shift_T) //导通角往0°的方向变化
					{
						PWM_buff -=t_count;
						if(PWM_buff < SCR_shift_T) PWM_buff = SCR_shift_T;
					}
					else if(PWM_buff < SCR_shift_T) //导通角往0°的方向变化
					{
						PWM_buff +=t_count;
						if(PWM_buff > SCR_shift_T) PWM_buff = SCR_shift_T;
					}
					else
					{	
						SCR_shift = DISABLE;//软切换到全控完成
						SCR_OFF = DISABLE;
					}
//					PWM_buff = SCR_shift_T;
				}
				else if(SCR_ON == ENABLE)//切换到半控
				{				
					if(PWM_buff < SCR_shift_T) 
					{
						PWM_buff += t_count;
						if(PWM_buff > SCR_shift_T)PWM_buff = SCR_shift_T;
					}
					else if(PWM_buff > SCR_shift_T)
					{
						PWM_buff -= t_count;
						if(PWM_buff < SCR_shift_T)PWM_buff = SCR_shift_T;
					}
					else
					{	 //导通角往90°+的方向变化
						SCR_shift = DISABLE;//软切换到半控完成
						SCR_ON = DISABLE;
					}
//					PWM_buff = SCR_shift_T;
				}
/*					if(SCR_OFF == ENABLE)//切换到全控
					{
						if(PWM_buff > 200) //导通角往0°的方向变化
						{
							PWM_buff -=t_count;
							if(PWM_buff < 200) PWM_buff = 200;
						}
						else
						{	
							if(SCR_P_FLAG == ENABLE)
							{
								SCR_P_FLAG = DISABLE;
								if(P_Rise_Time == 0)
								{
									if(CV_CC_CP == CP)
									{
										if(ON_OFF == ON) DAC_Send(DAC_CMD_WriteREGn_UpDAC_n, DAC_P, 2.176 * Set_Power + 49.833);
									}
									else
									DAC_Send(DAC_CMD_WriteREGn_UpDAC_n, DAC_P, 2.176 * 6000 + 49.833);
								}
								else TIM_Cmd(TIM1, ENABLE);//	
							}
							SCR_shift = DISABLE;//软切换到全控完成
							SCR_OFF = DISABLE;
						}
					}
					else if(SCR_ON == ENABLE)//切换到半控
					{
						if(PWM_buff < SCR_shift_T) 
						{
							PWM_buff += t_count;
							if(PWM_buff > SCR_shift_T)PWM_buff = SCR_shift_T;
						}
						else
						{	 //导通角往90°+的方向变化
							SCR_shift = DISABLE;//软切换到半控完成
							SCR_ON = DISABLE;
						}
					}	*/
				PWM0_1 = PWM_buff;
				PWM1_1 = PWM_buff + 10000;//默认50Hz
				PWM0_0 = PWM_buff + 1200;//脉宽1.2ms
				PWM1_0 = PWM_buff + 11200;//脉宽1.2ms
				temp = PWM0_1;
//				if(PWM_buff > 5500)
//				{
//					PWM0_0 = PWM_buff + 1200;//脉宽1.2ms
//					PWM1_0 = PWM_buff + 11200;//脉宽1.2ms
////					PWM0_0 = 9000;//脉宽1.2ms
////					PWM1_0 = 19000;//脉宽1.2ms
//				}
//				else
//				{
//					PWM1_0 = 16500;//脉宽不固定，下降沿固定
//					PWM0_0 = 6500;//脉宽不固定，下降沿固定
//				}
			}
			break;//在0-10ms中断
		}
		case TIM_IT_CC2: //在6.5~10ms中断
			PWM0_LOW;
//			if(WWDG_ON != ENABLE)
//			{
//				//WWDG_Init(41,34,WWDG_Prescaler_8);//计数器值为0x29,窗口寄存器为0x20,分频数为8	 
//				WWDG_ON = ENABLE;
//			}
//			else WWDG_SetCounter(WWDG_CNT);
			break;
		case TIM_IT_CC3://在10-20ms中断
		{	
			PWM1_HIGH; 
			//触发导通角自动双向渐变
			if(SCR_shift == ENABLE)//半控/全控软切换
			{
				u16 PWM_buff = (u16)PWM0_1;
			  if(SCR_OFF == ENABLE)
				{
					if(PWM_buff > SCR_shift_T) //导通角往0°的方向变化
					{
						PWM_buff -=t_count;
						if(PWM_buff < SCR_shift_T) PWM_buff = SCR_shift_T;
					}
					else if(PWM_buff < SCR_shift_T) //导通角往0°的方向变化
					{
						PWM_buff +=t_count;
						if(PWM_buff > SCR_shift_T) PWM_buff = SCR_shift_T;
					}
					else
					{	
						SCR_shift = DISABLE;//软切换到全控完成
						SCR_OFF = DISABLE;
					}
//					PWM_buff = SCR_shift_T;
				}
				else if(SCR_ON == ENABLE)//切换到半控
				{				
					if(PWM_buff < SCR_shift_T) 
					{
						PWM_buff += t_count;
						if(PWM_buff > SCR_shift_T)PWM_buff = SCR_shift_T;
					}
					else if(PWM_buff > SCR_shift_T)
					{
						PWM_buff -= t_count;
						if(PWM_buff < SCR_shift_T)PWM_buff = SCR_shift_T;
					}
					else
					{	 //导通角往90°+的方向变化
						SCR_shift = DISABLE;//软切换到半控完成
						SCR_ON = DISABLE;
					}
//					PWM_buff = SCR_shift_T;
				}
/*					if(SCR_OFF == ENABLE)//切换到全控
					{
						if(PWM_buff > 200) //导通角往0°的方向变化
						{
							PWM_buff -=t_count;
							if(PWM_buff < 200) PWM_buff = 200;
						}
						else
						{	
							if(SCR_P_FLAG == ENABLE)
							{
								SCR_P_FLAG = DISABLE;
								if(P_Rise_Time == 0)
								{
									if(CV_CC_CP == CP)
									{
										if(ON_OFF == ON) DAC_Send(DAC_CMD_WriteREGn_UpDAC_n, DAC_P, 2.176 * Set_Power + 49.833);
									}
									else
									DAC_Send(DAC_CMD_WriteREGn_UpDAC_n, DAC_P, 2.176 * 6000 + 49.833);
								}
								else TIM_Cmd(TIM1, ENABLE);//								
							}
							SCR_shift = DISABLE;//软切换到全控完成
							SCR_OFF = DISABLE;
						}
					}
					else if(SCR_ON == ENABLE)//切换到半控
					{
						if(PWM_buff < SCR_shift_T) 
						{
							PWM_buff += t_count;
							if(PWM_buff > SCR_shift_T)PWM_buff = SCR_shift_T;
						}
						else
						{	 //导通角往90°+的方向变化
							SCR_shift = DISABLE;//软切换到半控完成
							SCR_ON = DISABLE;
						}
					}	*/
				PWM0_1 = PWM_buff;	
				PWM1_1 = PWM_buff + 10000;//默认50Hz
				PWM0_0 = PWM_buff + 1200;//脉宽1.2ms
				PWM1_0 = PWM_buff + 11200;//脉宽1.2ms
//				
//				if(PWM_buff > 5500)
//				{
//					PWM0_0 = PWM_buff + 1200;//脉宽1.2ms
//					PWM1_0 = PWM_buff + 11200;//脉宽1.2ms
////					PWM0_0 = 9000;//脉宽1.2ms
////					PWM1_0 = 19000;//脉宽1.2ms
//				}
//				else
//				{
//					PWM0_0 = 6500; //脉宽不固定，下降沿固定
//					PWM1_0 = 16500;//脉宽不固定，下降沿固定
//				}
			}
		}
			break;

		case TIM_IT_CC4: //在16.5~19.5ms中断
		{
			TIM_Cmd(TIM3, DISABLE);
			PWM1_LOW;
			EXTI->IMR |= EXTI_Line9;  //开放中断
//			while((EXTI->IMR & EXTI_Line9) == 0)EXTI->IMR |= EXTI_Line9;
			RTC_ITConfig(RTC_IT_SEC, DISABLE);	
//			WWDG_SetCounter(WWDG_CNT);
		} break;
	}
	if(TIM3->SR & TIM_IT_Update)//CCR3大于定时器周期时(19.5ms),关闭定时器。
	{
		TIM_Cmd(TIM3, DISABLE);//
		PWM1_LOW;//
		EXTI->IMR |= EXTI_Line9;  //开放中断
//		while((EXTI->IMR & EXTI_Line9) == 0)EXTI->IMR |= EXTI_Line9;		
		RTC_ITConfig(RTC_IT_SEC, DISABLE);	
//		WWDG_SetCounter(WWDG_CNT);
	}
	TIM3->SR = 0x0000; 
}


/**
  * @brief  切换半控导通角的函数
  * @param  None
  * @retval None
  */
//ENABLE--半开
//DISABLE--全开
void SCR_On_Off(FunctionalState NewState)
{
		if (NewState != DISABLE)    //开半控
		 {
				SCR_OFF = DISABLE;//取消上一次未完成的全开切换
				SCR_shift = ENABLE;//使能切换
				SCR_ON = ENABLE;//开启半开切换	
		 }
		 else
		 {	 
			 SCR_ON = DISABLE;//取消上一次未完成的半开切换
			 SCR_shift = ENABLE;//使能切换
			 SCR_OFF = ENABLE;//开启全开切换
		 }
}	

//参数:功率
//功能：切换半控/全控
void SCR_Ser(float P)
{
	uint16_t temp = SCR_shift_T;
	if(ON_OFF == OFF)  return;
//	if(P < 2) SCR_shift = DISABLE;	//功率太小，电压下降慢，会导致下次功率上升出问题

	if(P < SCR_ENABLE_POWER)
	{
		if(CV_CC_CP == CV)
		{
			if(Set_Voltage > 30) 	SCR_shift_T = 5000;
			else SCR_shift_T = 7800;
			SCR_On_Off(ENABLE);
		}
		else if(CV_CC_CP == CC)
		{
//			if(Set_Current < 2)		SCR_shift_T = 9200;
//			else 
//				if(Set_Current < 20)  SCR_shift_T = 7800;
//			else 
				if(Set_Current < 40) 	SCR_shift_T = 7800;
				else  SCR_shift_T = 5000;
//			I_Compare();
//			SCR_shift_T = 7000;
				SCR_On_Off(ENABLE);
		}
		else if(CV_CC_CP == CP)
		{
			if(Set_Power < 200)	 SCR_shift_T = 7800;
//			else if(Set_Power < 200)  SCR_shift_T = 7800;
			else SCR_shift_T = 5000;
			SCR_On_Off(ENABLE);
		}
	}	
//	else if(P > 200) 
//	{
//		SCR_shift_T = 5000;
//		SCR_On_Off(ENABLE);
//	}
	else if(P > SCR_DISABLE_POWER)
	{
//		SCR_shift_T = 200;	
		SCR_shift_T = 5000;
		SCR_On_Off(DISABLE);
	}		
	if(temp != SCR_shift_T)
	{
		SCR_shift = ENABLE;//使能切换
	}
}

//void I_Compare(void)
//{
//	if(Current > Set_Current)
//	{	
//		SCR_shift_T = 9000;
//	}		
//}

//void V_in_I_out_P_out_ser(void)
//{
//	static int d_I = 0;//电流变化率
//	static int d_P = 0;//功率变化率
//	static int I_F = 0;//上一次采集的电流
//	static int P_F = 0;//上一次采集的功率
//	d_I = Current - I_F;
//	I_F = Current;
//	d_P = Power - P_F;
//	P_F = Power;
//	if(Current > Set_Current)   //减小输入电压
//	{
//		SCR_shift_T +=10;
//	}
//	else if(d_I < 1)   //增大输入电压
//	{
//		SCR_shift_T -=10;
//	}
//	else return; //
//	if(SCR_shift_T < 0) SCR_shift_T = 0;
//	else if(SCR_shift_T > 10000)  SCR_shift_T = 10000;






//}




