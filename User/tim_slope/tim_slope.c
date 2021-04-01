#include "tim_slope.h"
#include "softstart.h"

u8 V_Rise_On_Off = V_Rise_Off;
u8 V_Fall_On_Off = V_Fall_Off;
u8 I_Rise_On_Off = I_Rise_Off;
u8 I_Fall_On_Off = I_Fall_Off;

u32 V_Rise_Time;
u32 V_Fall_Time;
u32 I_Rise_Time;
u32 I_Fall_Time;
u32 P_Rise_Time = 0;//设置CP恒功率模式下的计数次数，1次对应1ms
u32 Vint_I_DA_Rise_Time;   //设置CP功率模式下，Vint缓升至DA值  4V的次数

u32 I_Fall_Time2;   //定义电流缓降时间，不是用户自己设定的
u32 I_Rise_Time_1;    //不是用户设定的缓升

u32 V_Rise_Time_1;    //不是用户设定的缓升

u32 V_Rise_Time_Vext;   //不是用户设定的缓升


float Slope_I_Minimum;			//定义每毫秒下降的电流DA值
float Slope_Current2;   

float Start_Set_Power = 100.0; 


float Slope_Voltage;        //定义当前应该输出的电压
float Slope_Current;        //定义当前应该输出的电流

float Slope_Minimum;              //定义每毫秒应该增加的值

float Slope_CP_Minimum;			//定义每毫秒增加值（CP）

float Slope_Power;

float Slope_Vint_DA_Minimum;     //定义每毫秒应增加的值，Vint_DA至4V

float Slope_Vint_DA;

float Slope_I_DA_Minimum;

float Slope_I_DA;

float Slope_P_DA_Minimum;

float Slope_P_DA;

float Slope_P_DA_END;

float Slope_I_Minimum_1;
float Slope_Current_1;  

float Slope_V_Minimum_1;
float Slope_Voltage_1;  

float Slope_V_Minimum_Vext;
float Slope_Voltage_Vext;




//配置中断优先级
static void TIM_V_RISE_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;	
		// 设置主优先级为 3
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

//配置TIM1，定时时间为1ms
static void TIM_V_RISE_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		
	  // 开启定时器时钟,即内部时钟CK_INT=72M
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
		// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period=9;
	  // 时钟预分频数
    TIM_TimeBaseStructure.TIM_Prescaler= 7199;		
		// 时钟分频因子 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		// 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	
		// 重复计数器的值
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	  // 初始化定时器
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
		// 清除计数器中断标志位
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(TIM1, TIM_IT_Update,ENABLE);
		
		// 先不使能计数器
    TIM_Cmd(TIM1, DISABLE);
}

//TIM1初始化
void TIM_SLOPE_Init(void)
{
	TIM_V_RISE_NVIC_Config();
	TIM_V_RISE_Mode_Config();		
}


/**
  * @brief  CP模式下缓升功能，一次加10W，一次1ms，大概1kW对应加载时间为0.1s
  * @param  None
  * @retval None
  */
void P_Slope_Function_On(void)
{
	ON_OFF = ON;
//	P_Rise_Time = P_Rise_s * 1000 + P_Rise_ms;
	P_Rise_Time = 100; 
	Slope_CP_Minimum = Set_Power / P_Rise_Time;//每ms递增的值
	Slope_Power = 0;
	TIM_Cmd(TIM1, ENABLE);
}

void I_Slope_Function_On_1(void)
{
	
	ON_OFF = ON;
//P_Rise_Time = (u32)(Set_Power / P_Rise_K) + 1;//计算出总进入中断次数
	I_Rise_Time_1 = 100;      //10000对应的10s
	Slope_I_Minimum_1 = Set_Current / I_Rise_Time_1;//每ms递增的值
	Slope_Current_1 = 0.0;
	
//	DAC_Send(DAC_CMD_WriteREGn_UpDAC_n, DAC_CHall, 0);
//	DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13150);
//	DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 16380);				
//	DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13124);
//	DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 2.2099 * Set_Power  - 11.622);
	
	Error_Stop_Flag = 1;//代表TIM1开启
	TIM_Cmd(TIM1, ENABLE);
}



/**
  * @brief  不是用户设定缓升，设定100ms缓升以解决设定值突变问题引起的电流过大问题
  * @param  None
  * @retval None
  */
void V_Slope_Function_On_1(void)    //Vint缓升
{
	
	ON_OFF = ON;
//P_Rise_Time = (u32)(Set_Power / P_Rise_K) + 1;//计算出总进入中断次数
	V_Rise_Time_1 = 150;      //10000对应的10s
	Slope_V_Minimum_1 = Set_Voltage / V_Rise_Time_1;//每ms递增的值
	Slope_Voltage_1 = 0.0;
	

	
	Error_Stop_Flag = 1;//代表TIM1开启
	TIM_Cmd(TIM1, ENABLE);
}


/**
  * @brief  不是用户设定缓升，设定100ms缓升以解决设定值突变问题引起的电流过大问题
  * @param  None
  * @retval None
  */
void V_Slope_Function_On_Vext(void)    //Vext缓升
{
	
	ON_OFF = ON;
//P_Rise_Time = (u32)(Set_Power / P_Rise_K) + 1;//计算出总进入中断次数
	V_Rise_Time_Vext = 150;      //10000对应的10s
	Slope_V_Minimum_Vext = Set_Voltage / V_Rise_Time_Vext;//每ms递增的值
	Slope_Voltage_Vext = 0.0;
	

	
	Error_Stop_Flag = 1;//代表TIM1开启
	TIM_Cmd(TIM1, ENABLE);
}



/**
  * @brief  启动V-Rise功能
  * @param  None
  * @retval None
  */
void V_Rise_Function_On(void)
{
	ON_OFF = ON;
	V_Rise_Time = V_Rise_s * 1000 + V_Rise_ms;
	Slope_Voltage = 0;
	Slope_Minimum = Set_Voltage / V_Rise_Time;
	TIM_Cmd(TIM1, ENABLE);
}

/**
  * @brief  启动V-Fall功能
  * @param  None
  * @retval None
  */
void V_Fall_Function_On(void)
{
	ON_OFF = OFF;
	V_Fall_Time = V_Fall_s * 1000 + V_Fall_ms;
	Slope_Voltage = Set_Voltage;
	Slope_Minimum = Set_Voltage / V_Fall_Time;
	TIM_Cmd(TIM1, ENABLE);
}

/**
  * @brief  启动I-Rise功能
  * @param  None
  * @retval None
  */
void I_Rise_Function_On(void)
{

	ON_OFF = ON;
	I_Rise_Time = I_Rise_s * 1000 + I_Rise_ms;
	Slope_Current = 0;
	Slope_Minimum = Set_Current / I_Rise_Time;
	TIM_Cmd(TIM1, ENABLE);
}

/**
  * @brief  启动I-Fall功能
  * @param  None
  * @retval None
  */
void I_Fall_Function_On(void)
{
	ON_OFF = OFF;
	I_Fall_Time = I_Fall_s * 1000 + I_Fall_ms;
  Slope_Current = Set_Current;
	Slope_Minimum = Set_Current / I_Fall_Time;
	TIM_Cmd(TIM1, ENABLE);
}



/**
  * @brief  TIM1的中断服务函数,用于DELAY
  * @param  None
  * @retval None
  */
void TIM_SLOPE_IT(void)
{
	if(ON_OFF == ON)
	{
		switch(CV_CC_CP)
		{
			case CV:
				if(Use_Vint_Or_Vext_Flag == Use_Vint)
				{
					if(V_Rise_On_Off == V_Rise_Off)
					{
						if(V_Rise_Time_1 > 1)
						{
	//						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 140);
							Slope_Voltage_1 = Slope_Voltage_1 + Slope_V_Minimum_1;
							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13926);				 
							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13926);
							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13926);
							

//							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 166.0 * Slope_Voltage_1 - 11.2836);  
//							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 167.29 * Slope_Voltage_1 - 24.891);
							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 164.97 * Slope_Voltage_1 + 1.7868);
									
							V_Rise_Time_1 --;
						}
					 else if(V_Rise_Time_1 == 1)
						{
	//						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 140);
							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13926);				
							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13926);
							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13926);
//							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 166.0 * Set_Voltage - 11.2836); 
//							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 167.29 * Set_Voltage - 24.891);
							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 164.97 * Set_Voltage + 1.7868);
							
							V_Rise_Time_1 --;

						}	
						else
						{
							TIM_Cmd(TIM1, DISABLE);			
							Slope_Voltage_1 = 0;
						}					
						break;
					}
					else if(V_Rise_On_Off == V_Rise_On)
					{
						
						if(V_Rise_Time > 1)
						{
	//						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 140);
							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13926);				
							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13926);
							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13926);
							

							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 164.97 * Slope_Voltage + 1.7868);  
							Slope_Voltage = Slope_Voltage + Slope_Minimum;		
							V_Rise_Time --;
						}
						else if(V_Rise_Time == 1)
						{
	//						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 140);
							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13926);				
							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13926);
							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13926);
							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 164.97 * Set_Voltage + 1.7868); 
							V_Rise_Time --;
						}	
						else
						{
							TIM_Cmd(TIM1, DISABLE);			
							Slope_Voltage = 0;
						}					 
						break;
					}	
				}
				
				
				else if(Use_Vint_Or_Vext_Flag == Use_Vext)
				{
							if(V_Rise_On_Off == V_Rise_Off)
							{
								if(V_Rise_Time_Vext > 1)
								{
			//						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 140);
									Slope_Voltage_Vext = Slope_Voltage_Vext + Slope_V_Minimum_Vext;
									DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13926);				
									DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13926);
									DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13926);
									

//									DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 166.0 * Slope_Voltage_Vext - 11.2836);  
//									DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 166.12 * Slope_Voltage_Vext - 31.148);
//									DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 164.97 * Slope_Voltage_Vext + 1.7868);
									DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 167.32 * Slope_Voltage_Vext - 21.908);
											
									V_Rise_Time_Vext --;
								}
							 else if(V_Rise_Time_Vext == 1)
								{
			//						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 140);
									DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13926);				
									DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13926);
									DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13926);
//									DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 166.0 * Set_Voltage - 11.2836); 
//									DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 166.12 * Set_Voltage - 31.148);
//									DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 164.97 * Set_Voltage + 1.7868);
									DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 167.32 * Set_Voltage - 21.908);
									
									V_Rise_Time_Vext --;

								}	
								else
								{
									TIM_Cmd(TIM1, DISABLE);			
									Slope_Voltage_Vext = 0;
								}					
								break;
							}
							else if(V_Rise_On_Off == V_Rise_On)
							{
								if(V_Rise_Time > 1)
								{
									DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13926);				
									DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13926);
									DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13926);
									DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 167.32 * Slope_Voltage - 21.908);   
									Slope_Voltage = Slope_Voltage + Slope_Minimum;		
									V_Rise_Time --;
								}
								else if(V_Rise_Time == 1)
								{
									DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13926);				
									DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13926);
									DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13926);
									DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 167.32 * Set_Voltage - 21.908); 
									V_Rise_Time --;
								}	
								else
								{
									TIM_Cmd(TIM1, DISABLE);			
									Slope_Voltage = 0;
								}					
								break;				
							}
					}
				
				
			case CC:
				if(I_Rise_On_Off == I_Rise_Off)
				{
					if(I_Rise_Time_1 > 1)	
					{
						Slope_Current_1 = Slope_Current_1 + Slope_I_Minimum_1;
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13926);
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13926);				
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13926);
//						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 129.38 * Slope_Current_1 + 29.218); 
//						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 131.48 * Slope_Current_1 + 70.61); 
//						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 131.48 * Slope_Current_1 + 119.26 );
//						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 0.0123 * Slope_Current_1*Slope_Current_1+ 132.87 * Slope_Current_1 - 10.568);
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 0.0098 * Slope_Current_1*Slope_Current_1+ 133.15 * Slope_Current_1 - 9.196);
						
						
						I_Rise_Time_1 --;
						
//						Error_Stop_Flag = 1;//代表TIM1开启
					
					}
					else if(I_Rise_Time_1 == 1)
					{
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13926);
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13926);				
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13926); 
//						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 129.38 * Set_Current + 29.218); 
//						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 131.48 * Set_Current + 70.61); 
//						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 131.48 * Set_Current + 119.26 );
//						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 0.0123 * Slope_Current_1*Slope_Current_1+ 132.87 * Slope_Current_1 - 10.568);
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 0.0098 * Slope_Current_1*Slope_Current_1+ 133.15 * Slope_Current_1 - 9.196);
						
						
						I_Rise_Time_1 --;
						
//						Error_Stop_Flag = 1;//代表TIM1开启
					}
					else  
					{
//						Error_Stop_Flag = 0;//代表TIM1关闭
						TIM_Cmd(TIM1, DISABLE);//关闭定时器TIM1
						Slope_Current_1 = 0;//复位Slope_Current_1值，以备下一次
					}
					break;
//				TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);   //清除中断标志位	
		 	 }
				else if(I_Rise_On_Off == I_Rise_On)
				{
					if(I_Rise_Time > 1)
					{
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13926);				
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13926);
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13926);
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 0.0098 * Slope_Current*Slope_Current+ 133.15 * Slope_Current - 9.196);   
						Slope_Current = Slope_Current + Slope_Minimum;	
						I_Rise_Time --;
					}
         else if(I_Rise_Time == 1)
				  {
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13926);				
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13926);
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13926);
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 0.0098 * Slope_Current*Slope_Current+ 133.15 * Slope_Current - 9.196);
				  	I_Rise_Time --;
				  }	
          else
				  {
						TIM_Cmd(TIM1, DISABLE);			
						Slope_Current = 0;
					}					
				  break;
				}
				
			case CP:
//				if(SCR_P_FLAG == ENABLE)break;
				if(P_Rise_Time > 1)	
				{
							//Slope_Power = Slope_Power + P_Rise_K;
							Slope_Power = Slope_Power + Slope_CP_Minimum;
		
							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13926);
							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13926);				
							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13926);
//		        DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 0.0000001 * Slope_Power * Slope_Power + 2.1869 * Slope_Power - 28.65);     //将想要的功率值转变为寄存器的值
//			      DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 2.2099 * Slope_Power  - 11.622);
//						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 2.2255 * Slope_Power + 21.649); 
//							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 1.001 * (2.1866 * Slope_Power + 14.064)); 
					
							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P,  2.2832 * Slope_Power + 37.121); 


		
							P_Rise_Time --;
		
							Error_Stop_Flag = 1;		//代表TIM1开启
	
				}
				else if(P_Rise_Time == 1)
				{
							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13926);
							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13926);				
							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13926);
//						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 0.0000001 * Set_Power * Set_Power + 2.1869 * Set_Power - 28.65);     //将想要的功率值转变为寄存器的值
//						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 2.2099 * Set_Power  - 11.622);
							
//						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 2.2255 * Set_Power + 21.649); 
//							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 1.001 * (2.1866 * Set_Power + 14.064)); 

							DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 2.2832 * Set_Power + 37.121); 
		
							P_Rise_Time --;
		
							Error_Stop_Flag = 1;//代表TIM1			开启
				}
				else  
				{
					Error_Stop_Flag = 0;//代表TIM1关闭
					TIM_Cmd(TIM1, DISABLE);//关闭定时器TIM1
					Slope_Power = 0;//复位Slope_Power值，以备下一次
				}
				break;			
		default: break;
		}
	}
	else if(ON_OFF == OFF)
	{
		switch(CV_CC_CP)
		{
			case CV:
				if(Use_Vint_Or_Vext_Flag == Use_Vint)
				{
					if(V_Fall_Time > 0)
					{
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13926);				
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13926);
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13926);
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 164.97 * Slope_Voltage + 1.7868);
						Slope_Voltage = Slope_Voltage - Slope_Minimum;		
						V_Fall_Time --;

					}
					else if (V_Fall_Time == 0)
					{
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_n, DAC_CHall, 0);
						SysTick_Delay_Ms(10);	

						SS_DISADBE_On();
						TIM_Cmd(TIM1, DISABLE);
					}	
				}
				else if(Use_Vint_Or_Vext_Flag == Use_Vext)
				{
					if(V_Fall_Time > 0)
					{
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13926);				
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13926);
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13926);
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 167.32 * Slope_Voltage - 21.908);
						Slope_Voltage = Slope_Voltage - Slope_Minimum;		
						V_Fall_Time --;

					}
					else if (V_Fall_Time == 0)
					{
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_n, DAC_CHall, 0);
						SysTick_Delay_Ms(10);	

						SS_DISADBE_On();
						TIM_Cmd(TIM1, DISABLE);
					}	
				}
				
				break;
				
				
				
			case CC: 
				if(I_Fall_Time > 0)
					{
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13926);
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13926);				
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13926);				
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 0.0098 * Slope_Current*Slope_Current+ 133.15 * Slope_Current - 9.196);
						Slope_Current = Slope_Current - Slope_Minimum;			
						I_Fall_Time --;

					}
					else if (I_Fall_Time == 0)
					{
						DAC_Send(DAC_CMD_WriteREGn_UpDAC_n, DAC_CHall, 0);
						SysTick_Delay_Ms(10);	

						SS_DISADBE_On();
						TIM_Cmd(TIM1, DISABLE);
					}	
				break;
				
//			case CP:
//				Slope_Power = Slope_Power - Slope_CP_Minimum;
//	    	DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13150);
//	    	DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13107);				
//	    	DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13124);
//		    DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 2.2099 * Slope_Power  - 11.622);
//  		
//        if(P_Rise_Time == 0)
//				{
//					TIM_Cmd(TIM1, DISABLE);
//				}	
//        else
//				{
//				  P_Rise_Time --;
//				}					
//				break;						
				
				default: break;
		}		
	}
	TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);   //清除中断标志位	
}

/**
  * @brief  CC模式下缓降功能，不是用户自己设定，10ms
  * @param  None
  * @retval None
  */

//		void I_Fall_DA(void)
//		{
//			I_Fall_Time2 = 10;   //10ms缓降
//			Slope_Current2 = Set_Current;   
//			Slope_I_Minimum = Set_Current / I_Fall_Time2;   //每ms应降多少值
//			
//			if(I_Fall_Time2 > 0)
//			{
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13150);
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13107);				
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 8050);				
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 131.48 * Slope_Current2 + 119.26);
//				Slope_Current2 = Slope_Current2 - Slope_Minimum;			
//				I_Fall_Time2 --;
//			}
//			else if (I_Fall_Time2 == 0)
//			{
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_n, DAC_CHall, 0);
//			}
//			
//		}



///***************************************************************************************************************************/
///**
//  * @brief  CP模式下的缓升定时器TIM1初始化程序，定时时间为1ms
//  * @param  None
//  * @retval None
//	* @date 2019/7/6
//  */
//void CP_Slope_Init(void)
//{
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure; 
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能
//	
//	//TIM1的中断优先级配置
//	// 设置中断组为2
//	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
//	// 设置中断来源
//	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;	
//	// 设置主优先级为2
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	 
//	// 设置子优先级为0
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//	
//	TIM_TimeBaseStructure.TIM_Period = 99;                  //设置自动重装载寄存器周期值
//  TIM_TimeBaseStructure.TIM_Prescaler = 719;              //设置预分频值
//  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;   //设置时钟分割
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
//  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;          //重复计数设置
//  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);           //参数初始化
//  TIM_ClearFlag(TIM1, TIM_FLAG_Update);                     //清中断标志位

//	// 开启计数器中断
//  TIM_ITConfig(TIM1, TIM_IT_Update,ENABLE);
//	
//	TIM_ClearFlag(TIM1, TIM_FLAG_Update);   
//		
//	// 不使能计数器
//  TIM_Cmd(TIM1, DISABLE);
//	
//}


///**
//  * @brief  CP模式下缓升功能，一次加10W，一次1ms，大概1kW对应加载时间为0.1s
//  * @param  None
//  * @retval None
//  */
//void P_Slope_Function_On(void)
//{
//	
//	ON_OFF = ON;
////P_Rise_Time = (u32)(Set_Power / P_Rise_K) + 1;//计算出总进入中断次数
//	P_Rise_Time = 2000;      //10000对应的10s
//	Slope_CP_Minimum = Set_Power / 2000.0;//每ms递增的值
//	Slope_Power = 0.0;
//	
////	DAC_Send(DAC_CMD_WriteREGn_UpDAC_n, DAC_CHall, 0);
////	DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13150);
////	DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 16380);				
////	DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13124);
////	DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 2.2099 * Set_Power  - 11.622);
//	
//	Error_Stop_Flag = 1;//代表TIM1开启
//	TIM_Cmd(TIM1, ENABLE);
//}






///**
//  * @brief  TIM1的中断服务函数
//  * @param  None
//  * @retval None
//  */
//void CP_TIM1_SLOPE_IT(void)
//{
////	static float Slope_Power = 0;
//	
//	if(P_Rise_Time > 1)
//	{
//		//Slope_Power = Slope_Power + P_Rise_K;
//		Slope_Power = Slope_Power + Slope_CP_Minimum;
//		
//		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13150);
//		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13107);				
//		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13124);
////		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 0.0000001 * Slope_Power * Slope_Power + 2.1869 * Slope_Power - 28.65);     //将想要的功率值转变为寄存器的值
//			DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 2.2099 * Slope_Power  - 11.622);
//		
//		P_Rise_Time --;
//		
//		Error_Stop_Flag = 1;//代表TIM1开启
//	
//	}
//	else if(P_Rise_Time == 1)
//	{
//		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13150);
//		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13107);				
//		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13124);
////		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 0.0000001 * Set_Power * Set_Power + 2.1869 * Set_Power - 28.65);     //将想要的功率值转变为寄存器的值
//			DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 2.2099 * Set_Power  - 11.622);
//		
//		P_Rise_Time --;
//		
//		Error_Stop_Flag = 1;//代表TIM1开启
//	}
//	else  
//	{
//		Error_Stop_Flag = 0;//代表TIM1关闭
//		TIM_Cmd(TIM1, DISABLE);//关闭定时器TIM1
//		Slope_Power = 0;//复位Slope_Power值，以备下一次
//	}
//	
//	TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);   //清除中断标志位	
//}




///**
//  * @brief  TIM1，CP模式下，电压电流缓升至DA值
//  * @param  None
//  * @retval None
//  */
//void Vint_I_SLOPE_DA(void)
//{
//	ON_OFF = ON;
//	Vint_I_DA_Rise_Time = 20000;     //10000对应的10s
//	Slope_Vint_DA_Minimum = 13150 / 20000.0;    //每ms上升的值
//	Slope_Vint_DA = 0.0;
//	
//	Slope_I_DA_Minimum = 13124 / 20000.0;
//	Slope_I_DA = 0.0;
//	

//	
//	Error_Stop_Flag = 1;//代表TIM1开启
//	TIM_Cmd(TIM1, ENABLE);
//	
//}



///**
//  * @brief  TIM1的中断服务函数，用于电压电流DA缓升
//  * @param  None
//  * @retval None
//  */
//void Vint_I_TIM1_SLOPE_IT(void)
//{
//	
//	if(Vint_I_DA_Rise_Time > 1)
//	{
//		Slope_Vint_DA = Slope_Vint_DA_Minimum + Slope_Vint_DA;
//						
//		Slope_I_DA = Slope_I_DA_Minimum + Slope_I_DA;

//		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, Slope_Vint_DA);
//		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, Slope_I_DA);

////		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13107);		//原来16380		
////		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 2.2099 * Slope_Power  - 11.622);

//		Vint_I_DA_Rise_Time --;
//		Error_Stop_Flag = 1;
//	}
//	
//	else if(Vint_I_DA_Rise_Time == 1)
//	{
//		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13150);
//		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13124);
////		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13107);		//原来16380		
////		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 2.2099 * Set_Power  - 11.622); 
//		
//		Vint_I_DA_Rise_Time --;
//		Error_Stop_Flag = 1;
//	}
//	
//	else
//	{
//		Error_Stop_Flag = 0;//代表TIM1关闭
//		TIM_Cmd(TIM1, DISABLE);//关闭定时器TIM1
//		Slope_I_DA = 0;//复位Slope_I_DA，以备下一次
//		Slope_Vint_DA = 0;
//	}
//	
//		TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);   //清除中断标志位	
//}






///**
//  * @brief  TIM1，CP模式下，10s内电压电流缓升至DA值，10-20s功率缓升至DA值
//  * @param  None
//  * @retval None
//  */
//void Vint_I_SLOPE_DA(void)
//{
//	ON_OFF = ON;
//	Vint_I_DA_Rise_Time = 20000;     //10000对应的10s
//	Slope_Vint_DA_Minimum = 13150 / 10000.0;    //每ms上升的值
//	Slope_Vint_DA = 0.0;
//	
//	Slope_I_DA_Minimum = 13124 / 10000.0;
//	Slope_I_DA = 0.0;
//	
//	Slope_P_DA_END = 2.2099 * Set_Power - 11.622;
//	Slope_P_DA = 2.2099 * Start_Set_Power - 11.622;
//	Slope_P_DA_Minimum = (Slope_P_DA_END - Slope_P_DA) / 10000.0;
//	
//	
//	Error_Stop_Flag = 1;//代表TIM1开启
//	TIM_Cmd(TIM1, ENABLE);
//	
//}



//	
///**
//  * @brief  TIM1的中断服务函数，10s内电压电流缓升至DA值，10-20s功率缓升至DA值
//  * @param  None
//  * @retval None
//  */
//void Vint_I_TIM1_SLOPE_IT(void)
//{
//	
//	
//	if((Vint_I_DA_Rise_Time > 10000) & (Vint_I_DA_Rise_Time <= 20000))
//	{
//		Slope_Vint_DA = Slope_Vint_DA_Minimum + Slope_Vint_DA;
//						
//		Slope_I_DA = Slope_I_DA_Minimum + Slope_I_DA;

//		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, Slope_Vint_DA);
//		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, Slope_I_DA);

////		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13107);		//原来16380		
////		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 2.2099 * Slope_Power  - 11.622);
//    
//		Vint_I_DA_Rise_Time --;
//		Error_Stop_Flag = 1;
//	}
//	
//	else if(Vint_I_DA_Rise_Time == 10000)
//	{
//		
//			DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13150);
//		  DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13124);
////		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13107);		//原来16380		
////		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 2.2099 * Set_Power  - 11.622); 
//		
//		
//		
//		Vint_I_DA_Rise_Time --;
//		Error_Stop_Flag = 1;
//	}
//	
//	else if((Vint_I_DA_Rise_Time > 1) & (Vint_I_DA_Rise_Time < 10000))
//	{
//		
//		
//		Slope_P_DA = Slope_P_DA_Minimum + Slope_P_DA;
//		
//		DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, Slope_P_DA);
//		

//		Vint_I_DA_Rise_Time --;
//		Error_Stop_Flag = 1;
//	}
//	
//	else if(Vint_I_DA_Rise_Time == 1)
//	{
//    DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 2.2099 * Set_Power  - 11.622); 
//		
//		Vint_I_DA_Rise_Time --;
//		Error_Stop_Flag = 1;
//	}
//	
//	else
//	{
//		Error_Stop_Flag = 0;//代表TIM1关闭
//		TIM_Cmd(TIM1, DISABLE);//关闭定时器TIM1
//		Slope_I_DA = 0;//复位Slope_I_DA，以备下一次
//		Slope_Vint_DA = 0;
//	}
//	
//		TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);   //清除中断标志位	
//}


























