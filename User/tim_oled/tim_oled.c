#include "tim_oled.h"

u8 Cursor_Flash;                                  //光标标志位，用于表示当前是显示还是清空（光标的形式是闪烁）
u8 RS232_Set_ID_BaudRate = RS232_First_Select;    //表示现在设定的是ID号还是波特率
u8 CAN_Set_ID_BaudRate = CAN_First_Select;      //表示现在设定的是ID号还是波特率
u16 RS232_ID = 01;                                     //表示当前的RS232的串口号
u32 RS232_BaudRate = 9600;                         //表示当前的RS232的波特率
u16 CAN_ID = 01;                                     //表示当前的CAN的串口号
u32 CAN_BaudRate = 100;                         //表示当前的CAN的波特率




//配置中断优先级
static void TIM_OLED_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

//配置TIM5，定时周期为0.5s
static void TIM_OLED_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		
	  // 开启定时器时钟,即内部时钟CK_INT=72M
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
		// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period=49999;
	  // 时钟预分频数
    TIM_TimeBaseStructure.TIM_Prescaler= 719;		
		// 时钟分频因子 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		// 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	
		// 重复计数器的值
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	  // 初始化定时器
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
		// 清除计数器中断标志位
    TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(TIM5, TIM_IT_Update,ENABLE);
		
		// 使能计数器
    TIM_Cmd(TIM5, DISABLE);
}

//TIM5初始化
void TIM_OLED_Init(void)
{
	TIM_OLED_NVIC_Config();
	TIM_OLED_Mode_Config();		
}

/*----------------------------------------RS232------------------------------------------------------*/

char String_RS232_ID[5];                          //RS232 ID的字符串

/**
  * @brief  将RS232 ID号转为字符串
  * @param  ID号
  * @retval None
  */
void Display_RS232_ID(u16 RS232_ID)
{
	if(RS232_ID >= 10)
	{
		sprintf(String_RS232_ID, "%2d", RS232_ID);
	}
	else
	{
		sprintf(String_RS232_ID, "%2d", RS232_ID);
		String_RS232_ID[0] = '0';
	}
	
	Write_String_16x32AsicII(0, 12, String_RS232_ID);
}

char String_RS232_BaudRate[10];

/**
  * @brief  将RS232 波特率转为字符串
  * @param  波特率
  * @retval None
  */
void Display_RS232_BaudRate(u32 RS232_BaudRate)
{
	if (RS232_BaudRate >= 100000)
	{
		sprintf(String_RS232_BaudRate, "%6d", RS232_BaudRate);
	}
	else if (RS232_BaudRate >= 10000)
	{
		sprintf(String_RS232_BaudRate, "%6d", RS232_BaudRate);
		String_RS232_BaudRate[0] = '0';
	}
	else if (RS232_BaudRate >= 1000)
	{
		sprintf(String_RS232_BaudRate, "%6d", RS232_BaudRate);
		String_RS232_BaudRate[0] = '0';
		String_RS232_BaudRate[1] = '0';		
	}
	else if (RS232_BaudRate >= 100)
	{
		sprintf(String_RS232_BaudRate, "%6d", RS232_BaudRate);
		String_RS232_BaudRate[0] = '0';
		String_RS232_BaudRate[1] = '0';	
		String_RS232_BaudRate[2] = '0';			
	}
	
  Write_String_16x32AsicII(32, 40, String_RS232_BaudRate);
}

/*--------------------------------------------------------------------CAN----------------------------------------------------*/

char String_CAN_ID[5];                          //RS232 ID的字符串

/**
  * @brief  将RS232 ID号转为字符串
  * @param  ID号
  * @retval None
  */
void Display_CAN_ID(u16 CAN_ID)
{
	if(CAN_ID >= 10)
	{
		sprintf(String_CAN_ID, "%2d", CAN_ID);
	}
	else
	{
		sprintf(String_CAN_ID, "%2d", CAN_ID);
		String_CAN_ID[0] = '0';
	}
	
	Write_String_16x32AsicII(0, 12, String_CAN_ID);
}

char String_CAN_BaudRate[10];

/**
  * @brief  将CAN 波特率转为字符串
  * @param  波特率
  * @retval None
  */
void Display_CAN_BaudRate(u32 CAN_BaudRate)
{

	if (CAN_BaudRate >= 100)
	{
		sprintf(String_CAN_BaudRate, "%3d", CAN_BaudRate);
	
	}
	else if (CAN_BaudRate >= 10)
	{
		sprintf(String_CAN_BaudRate, "%3d", CAN_BaudRate);	
		String_CAN_BaudRate[0] = '0';
	}
	else
	{
		sprintf(String_CAN_BaudRate, "%3d", CAN_BaudRate);	
    String_CAN_BaudRate[0] = '0';		
    String_CAN_BaudRate[1] = '0';				
	}
	
  Write_String_16x32AsicII(32, 40, String_CAN_BaudRate);
}

/**
  * @brief  TIM5的中断服务函数的跳转函数,用于设置电压、电流、功率时的光标
  * @param  None
  * @retval None
  */
void TIM5_Oled_IT(void)
{
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) 
	{		
		if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_V_set) && (Keys_Encoder_Mode == Encoder_Mode))             //处于设置电压模式和编码器模式
		{
		  if(Cursor_Flash == Cursor_On)
		  {
			  Write_Single_16x32AsicII(0, Cursor_Position*4, ' ');
				Display_Main_Interface_CV_CC_CP(CV_CC_CP,DELAY_NOT_DELAY);
	      Display_Main_Interface_ON_OFF(ON_OFF);
				
				Cursor_Flash = Cursor_Off;
		  }
		  else if(Cursor_Flash == Cursor_Off)
		  {
			  Write_Single_16x32AsicII(0, Cursor_Position*4, String_Voltage[Cursor_Position]);
				Display_Main_Interface_CV_CC_CP(CV_CC_CP,DELAY_NOT_DELAY);
	      Display_Main_Interface_ON_OFF(ON_OFF);

				Cursor_Flash = Cursor_On;
		  }
		  TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //清除中断标志位
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_V_set) && (Keys_Encoder_Mode == Keys_Mode))          //处于设置电压模式和键盘模式
		{		
			if(Cursor_Flash == Cursor_On)
		  {
			  Write_String_16x32AsicII(0, 0, "      ");
				Display_Main_Interface_CV_CC_CP(CV_CC_CP,DELAY_NOT_DELAY);
	      Display_Main_Interface_ON_OFF(ON_OFF);
				
				Cursor_Flash = Cursor_Off;
		  }
		  else if(Cursor_Flash == Cursor_Off)
		  {
			  Write_String_16x32AsicII(0, 0, String_Voltage);
				Write_Single_16x32AsicII(0, 20, 'V');
				Display_Main_Interface_CV_CC_CP(CV_CC_CP,DELAY_NOT_DELAY);
	      Display_Main_Interface_ON_OFF(ON_OFF);
				
				Cursor_Flash = Cursor_On;
		  }
		  TIM_ClearITPendingBit(TIM5, TIM_IT_Update);   //清除中断标志位				
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_I_set) && (Keys_Encoder_Mode == Encoder_Mode))      //处于设置电流模式和编码器模式
		{
			if(Cursor_Flash == Cursor_On)
		  {
			  Write_Single_16x32AsicII(0, Cursor_Position*4 + 36, ' ');
				Display_Main_Interface_CV_CC_CP(CV_CC_CP,DELAY_NOT_DELAY);
	      Display_Main_Interface_ON_OFF(ON_OFF);
				
				Cursor_Flash = Cursor_Off;
		  }
		  else if(Cursor_Flash == Cursor_Off)
		  {
			  Write_Single_16x32AsicII(0, Cursor_Position*4 + 36, String_Current[Cursor_Position]);
				Display_Main_Interface_CV_CC_CP(CV_CC_CP,DELAY_NOT_DELAY);
	      Display_Main_Interface_ON_OFF(ON_OFF);
				
				Cursor_Flash = Cursor_On;
		  }
		  TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //清除中断标志位
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_I_set) && (Keys_Encoder_Mode == Keys_Mode))          //处于设置电流模式和键盘模式
		{
			if(Cursor_Flash == Cursor_On)
		  {
			  Write_String_16x32AsicII(0, 36, "       ");
				Display_Main_Interface_CV_CC_CP(CV_CC_CP,DELAY_NOT_DELAY);
	      Display_Main_Interface_ON_OFF(ON_OFF);
				
				Cursor_Flash = Cursor_Off;
		  }
		  else if(Cursor_Flash == Cursor_Off)
		  {
			  Write_String_16x32AsicII(0, 36, String_Current);
				Write_Single_16x32AsicII(0, 60, 'A');
				Display_Main_Interface_CV_CC_CP(CV_CC_CP,DELAY_NOT_DELAY);
	      Display_Main_Interface_ON_OFF(ON_OFF);
				
				Cursor_Flash = Cursor_On;
		  }
		  TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //清除中断标志位
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_P_set) && (Keys_Encoder_Mode == Encoder_Mode))        //处于功率设置模式和编码器模式
		{
			if(Cursor_Flash == Cursor_On)
		  {
			  Write_Single_12x24AsicII(42, Cursor_Position*3, ' ');
				Display_Main_Interface_CV_CC_CP(CV_CC_CP,DELAY_NOT_DELAY);
	      Display_Main_Interface_ON_OFF(ON_OFF);
				
				Cursor_Flash = Cursor_Off;
		  }
		  else if(Cursor_Flash == Cursor_Off)
		  {
			  Write_Single_12x24AsicII(42, Cursor_Position*3, String_Power[Cursor_Position]);
				Display_Main_Interface_CV_CC_CP(CV_CC_CP,DELAY_NOT_DELAY);
	      Display_Main_Interface_ON_OFF(ON_OFF);

				Cursor_Flash = Cursor_On;
		  }
		  TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //清除中断标志位			
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_P_set) && (Keys_Encoder_Mode == Keys_Mode))          //处于设置功率模式和键盘模式
		{
			if(Cursor_Flash == Cursor_On)
		  {
			  Write_String_12x24AsicII(42, 0, "      ");
				Display_Main_Interface_CV_CC_CP(CV_CC_CP,DELAY_NOT_DELAY);
	      Display_Main_Interface_ON_OFF(ON_OFF);
				
				Cursor_Flash = Cursor_Off;
		  }
		  else if(Cursor_Flash == Cursor_Off)
		  {
			  Write_String_12x24AsicII(42, 0, String_Power);
				Write_Single_12x24AsicII(42, 15, 'W');
				Display_Main_Interface_CV_CC_CP(CV_CC_CP,DELAY_NOT_DELAY);
	      Display_Main_Interface_ON_OFF(ON_OFF);
				
				Cursor_Flash = Cursor_On;
		  }
		  TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //清除中断标志位
		}		
		
/*-----------------------------------------------------RS232-------------------------------------------------------------------*/		
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == RS232))      //选择了RS232,显示对RS232参数的设置
		{
			if(RS232_Set_ID_BaudRate == RS232_Set_ID)     //设定RS232的ID号
			{
				if (Keys_Encoder_Mode == Encoder_Mode)       //编码器模式
				{
					if (Cursor_Flash == Cursor_On)
				  {
				  	Write_String_16x32AsicII(0, (12 + Cursor_Position * 4), " ");
				  	Cursor_Flash = Cursor_Off;					
				  }
			  	else if (Cursor_Flash == Cursor_Off)
			  	{
			     	Write_String_16x32AsicII(0, 12, String_RS232_ID);	
            Cursor_Flash = Cursor_On;
			  	}
				}
				else if (Keys_Encoder_Mode == Keys_Mode)
				{
					if (Cursor_Flash == Cursor_On)
				  {
				  	Write_String_16x32AsicII(0, 12, "   ");
				  	Cursor_Flash = Cursor_Off;					
				  }
			  	else if (Cursor_Flash == Cursor_Off)
			  	{
			     	Write_String_16x32AsicII(0, 12, String_RS232_ID);	
            Cursor_Flash = Cursor_On;
			  	}
				}
			}
			
			else if (RS232_Set_ID_BaudRate == RS232_Set_BaudRate)
			{
				if (Cursor_Flash == Cursor_On)
				{
					Write_String_16x32AsicII(32, 40, "      ");
					Cursor_Flash = Cursor_Off;
				}
				else if (Cursor_Flash == Cursor_Off)
				{
		    	switch (RS232_BaudRate_State)
		    	{
		  			case Usart300 : Write_String_16x32AsicII(32, 40, "000300");break;
            case Usart600 : Write_String_16x32AsicII(32, 40, "000600");break;
            case Usart1200 : Write_String_16x32AsicII(32, 40, "001200");break;
            case Usart2400 : Write_String_16x32AsicII(32, 40, "002400");break;
            case Usart4800 : Write_String_16x32AsicII(32, 40, "004800");break;
            case Usart9600 : Write_String_16x32AsicII(32, 40, "009600");break;
            case Usart19200 : Write_String_16x32AsicII(32, 40, "019200");break;
            case Usart38400 : Write_String_16x32AsicII(32, 40, "038400");break;
            case Usart43000 : Write_String_16x32AsicII(32, 40, "043000");break;
            case Usart56000 : Write_String_16x32AsicII(32, 40, "056000");break;
            case Usart57600 : Write_String_16x32AsicII(32, 40, "057600");break;
            case Usart115200 : Write_String_16x32AsicII(32, 40, "115200");break;
	  				default: break;
		    	}
					
          Cursor_Flash = Cursor_On;
				}
			}
			TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //清除中断标志位
		}
		
/*---------------------------------------------------------------------------------CAN---------------------------------------------*/		
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == CAN))      //选择了CAN,显示对CAN参数的设置
		{
			if(CAN_Set_ID_BaudRate == CAN_Set_ID)     //设定CAN的ID号
			{
				if (Keys_Encoder_Mode == Encoder_Mode)       //编码器模式
				{
					if (Cursor_Flash == Cursor_On)
				  {
				  	Write_String_16x32AsicII(0, (12 + Cursor_Position * 4), " ");
				  	Cursor_Flash = Cursor_Off;					
				  }
			  	else if (Cursor_Flash == Cursor_Off)
			  	{
			     	Write_String_16x32AsicII(0, 12, String_CAN_ID);	
            Cursor_Flash = Cursor_On;
			  	}
				}
				else if (Keys_Encoder_Mode == Keys_Mode)
				{
					if (Cursor_Flash == Cursor_On)
				  {
				  	Write_String_16x32AsicII(0, 12, "   ");
				  	Cursor_Flash = Cursor_Off;					
				  }
			  	else if (Cursor_Flash == Cursor_Off)
			  	{
			     	Write_String_16x32AsicII(0, 12, String_CAN_ID);	
            Cursor_Flash = Cursor_On;
			  	}
				}
			}
			
			else if (CAN_Set_ID_BaudRate == CAN_Set_BaudRate)    //设置波特率
			{
				if (Cursor_Flash == Cursor_On)
				{
					Write_String_16x32AsicII(32, 40, "   ");
					Cursor_Flash = Cursor_Off;
				}
				else if (Cursor_Flash == Cursor_Off)
				{
		    	switch (CAN_BaudRate_State)
		    	{
					  case CAN5 : Write_String_16x32AsicII(32, 40, "005");SysTick_Delay_Ms(10);break;				
				  	case CAN10 : Write_String_16x32AsicII(32, 40, "010");SysTick_Delay_Ms(10);break;
            case CAN20 : Write_String_16x32AsicII(32, 40, "020");SysTick_Delay_Ms(10);break;
            case CAN25 : Write_String_16x32AsicII(32, 40, "025");SysTick_Delay_Ms(10);break;
            case CAN40 : Write_String_16x32AsicII(32, 40, "040");SysTick_Delay_Ms(10);break;
            case CAN50 : Write_String_16x32AsicII(32, 40, "050");SysTick_Delay_Ms(10);break;
            case CAN80 : Write_String_16x32AsicII(32, 40, "080");SysTick_Delay_Ms(10);break;
            case CAN100 : Write_String_16x32AsicII(32, 40, "100");SysTick_Delay_Ms(10);break;
            case CAN125 : Write_String_16x32AsicII(32, 40, "125");SysTick_Delay_Ms(10);break;
            case CAN200 : Write_String_16x32AsicII(32, 40, "200");SysTick_Delay_Ms(10);break;
            case CAN250 : Write_String_16x32AsicII(32, 40, "250");SysTick_Delay_Ms(10);break;
            case CAN400 : Write_String_16x32AsicII(32, 40, "400");SysTick_Delay_Ms(10);break;
            case CAN500 : Write_String_16x32AsicII(32, 40, "500");SysTick_Delay_Ms(10);break;
            case CAN800 : Write_String_16x32AsicII(32, 40, "800");SysTick_Delay_Ms(10);break;				
			  		default: break;
		    	}
					
          Cursor_Flash = Cursor_On;
				}
			}
			TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //清除中断标志位
		}
		
/*------------------------------------------------------------------------------------Cycle------------------------------------------------------------------*/
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Cycle))      //选择了Cycle,显示对Cycle参数的设置
		{				
			if (Cycle_Enter == Cycle_Enter_1)       //设置循环开启秒
			{
				if (Keys_Encoder_Mode == Encoder_Mode)       //编码器模式
				{
					if (Cursor_Flash == Cursor_On)
				  {
				  	Write_String_16x32AsicII(32, (12 + Cursor_Position * 4), " ");
				  	Cursor_Flash = Cursor_Off;					
				  }
			  	else if (Cursor_Flash == Cursor_Off)
			  	{
			     	Write_String_16x32AsicII(32, 12, String_Cycle_On);	
            Cursor_Flash = Cursor_On;
			  	}
				}
					
        else if (Keys_Encoder_Mode == Keys_Mode)     //键盘设定模式
				{
				  if (Cursor_Flash == Cursor_On)
				  {
			    	Write_String_16x32AsicII(32, 12, "    ");
						Write_String_16x32AsicII(32, 32, "   ");
						Cursor_Flash = Cursor_Off;					
			  	}
			    else if (Cursor_Flash == Cursor_Off)
			    {
			      Write_String_16x32AsicII(32, 12, String_Cycle_On);	
            Cursor_Flash = Cursor_On;
			    }					
			  }					
		  }
			
			else if (Cycle_Enter == Cycle_Enter_2)       //设置循环开启秒
			{
				if (Keys_Encoder_Mode == Encoder_Mode)       //编码器模式
				{
					if (Cursor_Flash == Cursor_On)
				  {
				  	Write_String_16x32AsicII(32, (12 + Cursor_Position * 4), " ");
				  	Cursor_Flash = Cursor_Off;					
				  }
			  	else if (Cursor_Flash == Cursor_Off)
			  	{
			     	Write_String_16x32AsicII(32, 12, String_Cycle_Close);	
            Cursor_Flash = Cursor_On;
			  	}
				}
					
        else if (Keys_Encoder_Mode == Keys_Mode)      //键盘设定模式
				{
				  if (Cursor_Flash == Cursor_On)
				  {
			    	Write_String_16x32AsicII(32, 12, "    ");
						Write_String_16x32AsicII(32, 32, "   ");
						Cursor_Flash = Cursor_Off;					
			  	}
			    else if (Cursor_Flash == Cursor_Off)
			    {
			      Write_String_16x32AsicII(32, 12, String_Cycle_Close);	
            Cursor_Flash = Cursor_On;
			    }					
			  }					
		  }
			
			TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //清除中断标志位			
		 }
		
/*--------------------------------------------------------V-Rise------------------------------------------------------------*/		 
		 
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == V_Rise))      //选择了V-Rise,显示对V-Rise参数的设置
		{								
			if (V_Rise_Enter == V_Rise_Enter_1)       //设置循环开启秒
			{
				if (Keys_Encoder_Mode == Encoder_Mode)       //编码器模式
				{
					if (Cursor_Flash == Cursor_On)
				  {
				  	Write_String_16x32AsicII(32, (12 + Cursor_Position * 4), " ");
				  	Cursor_Flash = Cursor_Off;					
				  }
			  	else if (Cursor_Flash == Cursor_Off)
			  	{
			     	Write_String_16x32AsicII(32, 12, String_V_Rise);	
            Cursor_Flash = Cursor_On;
			  	}
				}
					
        else if (Keys_Encoder_Mode == Keys_Mode)     //键盘设定模式
				{
				  if (Cursor_Flash == Cursor_On)
				  {
			    	Write_String_16x32AsicII(32, 12, "    ");
						Write_String_16x32AsicII(32, 32, "   ");
						Cursor_Flash = Cursor_Off;					
			  	}
			    else if (Cursor_Flash == Cursor_Off)
			    {
			      Write_String_16x32AsicII(32, 12, String_V_Rise);	
            Cursor_Flash = Cursor_On;
			    }					
				}		
			}
			TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //清除中断标志位			
		}			

/*--------------------------------------------------------V-Fall------------------------------------------------------------*/		 
		 
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == V_Fall))      //选择了V-Fall,显示对V-Fall参数的设置
		{				
			if (V_Fall_Enter == V_Fall_Enter_1)       //设置循环开启秒
			{
				if (Keys_Encoder_Mode == Encoder_Mode)       //编码器模式
				{
					if (Cursor_Flash == Cursor_On)
				  {
				  	Write_String_16x32AsicII(32, (12 + Cursor_Position * 4), " ");
				  	Cursor_Flash = Cursor_Off;					
				  }
			  	else if (Cursor_Flash == Cursor_Off)
			  	{
			     	Write_String_16x32AsicII(32, 12, String_V_Fall);	
            Cursor_Flash = Cursor_On;
			  	}
				}
					
        else if (Keys_Encoder_Mode == Keys_Mode)     //键盘设定模式
				{
				  if (Cursor_Flash == Cursor_On)
				  {
			    	Write_String_16x32AsicII(32, 12, "    ");
						Write_String_16x32AsicII(32, 32, "   ");
						Cursor_Flash = Cursor_Off;					
			  	}
			    else if (Cursor_Flash == Cursor_Off)
			    {
			      Write_String_16x32AsicII(32, 12, String_V_Fall);	
            Cursor_Flash = Cursor_On;
			    }					
				}		
			}
			TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //清除中断标志位			
		 		
		}	
		
/*--------------------------------------------------------I-Rise------------------------------------------------------------*/		 
		 
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == I_Rise))      //选择了V-Rise,显示对V-Rise参数的设置
		{		
			if (I_Rise_Enter == I_Rise_Enter_1)       //设置循环开启秒
			{
				if (Keys_Encoder_Mode == Encoder_Mode)       //编码器模式
				{
					if (Cursor_Flash == Cursor_On)
				  {
				  	Write_String_16x32AsicII(32, (12 + Cursor_Position * 4), " ");
				  	Cursor_Flash = Cursor_Off;					
				  }
			  	else if (Cursor_Flash == Cursor_Off)
			  	{
			     	Write_String_16x32AsicII(32, 12, String_I_Rise);	
            Cursor_Flash = Cursor_On;
			  	}
				}
					
        else if (Keys_Encoder_Mode == Keys_Mode)     //键盘设定模式
				{
				  if (Cursor_Flash == Cursor_On)
				  {
			    	Write_String_16x32AsicII(32, 12, "    ");
						Write_String_16x32AsicII(32, 32, "   ");
						Cursor_Flash = Cursor_Off;					
			  	}
			    else if (Cursor_Flash == Cursor_Off)
			    {
			      Write_String_16x32AsicII(32, 12, String_I_Rise);	
            Cursor_Flash = Cursor_On;
			    }					
				}	
			}				
			
			 TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //清除中断标志位				
		 }			


/*--------------------------------------------------------I-Fall------------------------------------------------------------*/		 
		 
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == I_Fall))      //选择了V-Rise,显示对V-Rise参数的设置
		{		
			if (I_Fall_Enter == I_Fall_Enter_1)       //设置循环开启秒
			{
				if (Keys_Encoder_Mode == Encoder_Mode)       //编码器模式
				{
					if (Cursor_Flash == Cursor_On)
				  {
				  	Write_String_16x32AsicII(32, (12 + Cursor_Position * 4), " ");
				  	Cursor_Flash = Cursor_Off;					
				  }
			  	else if (Cursor_Flash == Cursor_Off)
			  	{
			     	Write_String_16x32AsicII(32, 12, String_I_Fall);	
            Cursor_Flash = Cursor_On;
			  	}
				}
					
        else if (Keys_Encoder_Mode == Keys_Mode)     //键盘设定模式
				{
				  if (Cursor_Flash == Cursor_On)
				  {
			    	Write_String_16x32AsicII(32, 12, "    ");
						Write_String_16x32AsicII(32, 32, "   ");
						Cursor_Flash = Cursor_Off;					
			  	}
			    else if (Cursor_Flash == Cursor_Off)
			    {
			      Write_String_16x32AsicII(32, 12, String_I_Fall);	
            Cursor_Flash = Cursor_On;
			    }					
				}	
			}				
			
			 TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //清除中断标志位			
		 }			 
		 	 
/*--------------------------------------------------------Delay----------------------------------------------------*/
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Delay))  //当处于第二级菜单的Delay
		{	
			if (Keys_Encoder_Mode == Encoder_Mode)   //如果处于编码器模式
			{
			  if (Cursor_Flash == Cursor_On)
			  {
			  	Write_String_16x32AsicII(32, (14 + Cursor_Position * 4), " ");
			  	Cursor_Flash = Cursor_Off;					
			  }
		  	else if (Cursor_Flash == Cursor_Off)
		  	{
					Write_String_16x32AsicII(32, 14, String_Delay);	
          Cursor_Flash = Cursor_On;
		  	}	
			}
			else if (Keys_Encoder_Mode == Keys_Mode)
			{
				if (Cursor_Flash == Cursor_On)
				{
					Write_String_16x32AsicII(32, 14, "  ");
				  Write_String_16x32AsicII(32, 26, "  ");
			    Write_String_16x32AsicII(32, 38, "  ");				
					
					Cursor_Flash = Cursor_Off;					
				}
				else if (Cursor_Flash == Cursor_Off)
				{
					Write_String_16x32AsicII(32, 14, String_Delay);	
					Cursor_Flash = Cursor_On;
				}					
			}
			
			TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //清除中断标志位
		}		 
		
		
		
		
		else      //不处于设置模式，退出中断
		{
			TIM_Cmd(TIM5, DISABLE);
			TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //清除中断标志位
		}
	}		
}



















