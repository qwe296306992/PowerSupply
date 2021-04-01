#include "dac.h"
#include "stdio.h"
#include "stm32f10x.h"   
#include "spi_oled.h"
#include "systick.h"
#include "keys.h"
#include "encoder.h"
#include "tim_oled.h"
#include "at25.h"
#include "tim_delay.h"
#include "tim_cycle.h"
#include "adc.h"
#include "protect.h"
#include "usart.h"
#include "can.h"
#include "tim_slope.h"
#include "softstart.h"
#include "SCR_Controller.h"



u8 Error_Stop_Flag = 0;  //控制定时器TIM3开关的状态位,0代表TIM3处于关闭状态，1代表TIM3处于开启状态
//u8 Converter_Flag = 0;   //控制切换模式的状态位，0代表与上次模式不同，1代表与上次模式相同
//u8 On_Flag = 0;      //模式不同时现在的状态位，0代表这次为CV_Vint, 1代表这次为CV_Vext, 2代表这次为CC， 3代表这次为CP

u8 Wait_Below_Set_Value_Flag = Already_Below;






/**
  * @brief  SPI1初始化
  * @param  无
  * @retval 无
  */
void SPI1_DAC_Config(void)                //16383
{
	SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开时钟*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	
	/*SCK MOSI MISO*/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//SCK
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//MOSI
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	/*CS */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//CS
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	
	GPIO_SetBits(GPIOC, GPIO_Pin_4);
	
	/*- SPI1 -*/
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 										  
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	 								  
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		 										  
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;											 
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		   										  
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; 	
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  							  
  SPI_Init(SPI1, &SPI_InitStructure);																	
  SPI_Cmd(SPI1 , ENABLE);	
	
	
	ON_OFF = OFF;
	OUTPUT_NOT_OUTPUT = NOT_OUTPUT;
		
	Output_Zero();   //一开始四路0
}

/**
  * @brief  SPI1发送一字节
  * @param  要发送的字节
  * @retval 无
  */
u8 SPI1_SendByte(u8 byte)
{
	u8 tmp;
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE) == RESET);
	SPI1->DR = byte;
	while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == RESET);
//	while (SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY) == SET);
	tmp=SPI1->DR;
	return tmp;
}

/**
  * @brief  发送DAC设定值
  * @param  命令，电流电压功率
  * @param  电流电压功率
  * @param  值
  * @retval 无
  */
void DAC_Send(u8 cmd, u8 ch, u16 data)
{
	u8 tmp1,tmp2,tmp3;
	
	GPIO_ResetBits(GPIOC, GPIO_Pin_4);
	tmp1=(cmd<<3)|ch;
	tmp2=data>>6;
	tmp3=data<<2;
	SPI1_SendByte(tmp1);
	SPI1_SendByte(tmp2);
	SPI1_SendByte(tmp3);
	GPIO_SetBits(GPIOC, GPIO_Pin_4);
}







/**
  * @brief  开始输出，没有开slope就直接输出，开了就调用Function_On函数
  * @param  无
  * @retval 无
  */
void Power_On(void)
{		
//	  PWM0_HIGH;
//	  PWM1_HIGH;	
	  switch(CV_CC_CP)
	  {
		  case CV:      
				//恒压模式
	
			  if(Use_Vint_Or_Vext_Flag == Use_Vint)
		  	{
				
//			  	if(Voltage > Set_Voltage)     //电压还没降到设定电压以下，等待降到设定电压以下
//			    {
//			  	  Wait_Below_Set_Value_Flag = Havent_Below;
////			   	  Power_Off();			
//			  	  ON_OFF = ON;				
//			    }
//					else
					if(V_Rise_On_Off == V_Rise_Off)         //没有打开缓升模式
					{
				    Wait_Below_Set_Value_Flag = Already_Below;
						SS_DISADBE_Off();
//				    ON_OFF = ON;
						SysTick_Delay_Ms(100);
						V_Slope_Function_On_1();
						
						

//						SysTick_Delay_Ms(5);	
//						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13096);	
//						SysTick_Delay_Ms(5);	
//				    DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13107);	
//						SysTick_Delay_Ms(5);							
//			  	  DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13124);
//			  	  SysTick_Delay_Ms(5);	
//						DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 166.0 * Set_Voltage - 11.2836);
						
          }
					
			    else if(V_Rise_On_Off == V_Rise_On)       //打开了缓升模式
			    {
			  	  Wait_Below_Set_Value_Flag = Already_Below;				
			  	  V_Rise_Function_On();
						SS_DISADBE_Off();
						
		  	  }
				
		      break;
			}
			else if(Use_Vint_Or_Vext_Flag == Use_Vext)     //如果使用外部电压
			{
	
//				if(Voltage > Set_Voltage)     //电压还没降到设定电压以下，等待降到设定电压一下
//			  {
//				  Wait_Below_Set_Value_Flag = Havent_Below;
//				  Power_Off();			
//				  ON_OFF = ON;				
//			  }
//		    else 
				if(V_Rise_On_Off == V_Rise_Off)         //没有打开缓升模式
			  {
						Wait_Below_Set_Value_Flag = Already_Below;
//				  ON_OFF = ON;
						SS_DISADBE_Off();
						SysTick_Delay_Ms(100);
					
						V_Slope_Function_On_Vext();

						
						

//					SysTick_Delay_Ms(5);	

//					DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13096);

//					SysTick_Delay_Ms(5);						
//				  DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13124);
//					SysTick_Delay_Ms(5);	
//					DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13150);
//					SysTick_Delay_Ms(5);	


//					DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 166.0 * Set_Voltage - 11.2836);
					
        }
				
			  else if(V_Rise_On_Off == V_Rise_On)       //打开了缓升模式
			  {
				  Wait_Below_Set_Value_Flag = Already_Below;	
					V_Rise_Function_On();
          SS_DISADBE_Off();					
				  
			  }
				
		
		    break;
			}

//			break;
//			
		case CC: 
						
//			if(Current > Set_Current)     //电压还没降到设定电压以下，等待降到设定电压一下
//			{
//				Wait_Below_Set_Value_Flag = Havent_Below;
//				Power_Off();			
//				ON_OFF = ON;				
//			}
//		  else 
			if(I_Rise_On_Off == I_Rise_Off)
			{
//				if(Set_Current < 3)                             
//				{
//					ON_OFF = ON;
//					SS_DISADBE_Off();
//					DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13096);
//					SysTick_Delay_Ms(5);
//					DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13150);
//					SysTick_Delay_Ms(5);
//					DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13107);
//					SysTick_Delay_Ms(5);
////					DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 131.48 * Set_Current + 70.61 );
//					DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 131.48 * Set_Current + 119.26 );
//				}
//				else
//				{
					SS_DISADBE_Off();
					SysTick_Delay_Ms(100);
					I_Slope_Function_On_1();
					
					
//				}
//				SysTick_Delay_Ms(600);					 //先有输入，但没输出，故延迟，让SS脚已经达到0.5V
//					
////				DAC_Send(DAC_CMD_WriteREGn_UpDAC_n, DAC_CHall, 0);
//				SysTick_Delay_Ms(5);	
////				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 8050);
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13096);
////				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 920);
////				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 2440);
//				SysTick_Delay_Ms(5);	
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13150);
////				SysTick_Delay_Ms(5);	
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13107);
//				SysTick_Delay_Ms(5);					

//				
////		    DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, Set_Current * Set_Current * 0.0006 + 131.26 * Set_Current - 7.9388);     //将想要的电流值转变为寄存器的值
////				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 128.82 * Set_Current + 40.966);
//				
////				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 129.38 * Set_Current + 29.218);  // 1欧
//					DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 131.48 * Set_Current + 70.61 );

				

			}
			else if(I_Rise_On_Off == I_Rise_On)
			{
				
				SS_DISADBE_Off();
				SysTick_Delay_Ms(100);	
				I_Rise_Function_On();
				
			}			
			
		
			break;
			
			
		case CP: //需要修改的缓升功率
		
//			if(Power > Set_Power)     //电压还没降到设定电压以下，等待降到设定电压一下
//			{
//				Wait_Below_Set_Value_Flag = Havent_Below;
//				Power_Off();			
//				ON_OFF = ON;				
//			}
//      else
//      {
//				if(Set_Power < 50)
//				{
//					ON_OFF = ON;
//				
//					
//					DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13107);		//原来16380		
//					SysTick_Delay_Ms(5);	
//					
//					DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13124);
//					SysTick_Delay_Ms(5);	
//					
//					DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13150);
//					SysTick_Delay_Ms(5);	
//					
//					DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 2.176 * Set_Power + 49.833); 
//						
//					SS_DISADBE_Off();
//				}

//				else				
//				{
					SS_DISADBE_Off();
					SysTick_Delay_Ms(150);
					
					P_Slope_Function_On();//功率缓升
					
//				}
	
			break;
		  default: break;
	}
// }
	
	
	
}


/**
  * @brief  关闭输出
  * @param  无
  * @retval 无
  */
void Power_Off(void)
{
	switch(CV_CC_CP)
	{
		case CV: 
		
		  if(V_Fall_On_Off == V_Fall_Off)
			{
				ON_OFF = OFF;
				
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 16380);				
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13124);
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13096);
//				
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 0);    //将想要的电压值转变为寄存器的值
//				SS_DISADBE_Off();   
				SS_DISADBE_On();
				SysTick_Delay_Ms(100);	

				DAC_Send(DAC_CMD_WriteREGn_UpDAC_n, DAC_CHall, 0);
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 0);	
				
			}
			else if(V_Fall_On_Off == V_Fall_On)
			{
				V_Fall_Function_On();
//				SS_DISADBE_On();
			}
		  break;
		case CC: 
		  if(I_Fall_On_Off == I_Fall_Off)
			{
				ON_OFF = OFF;
				
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13150);
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 16380);				
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13096);
//				
//		    DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 0);     //将想要的电流值转变为寄存器的值
//				I_Fall_DA();
				
//				SysTick_Delay_Ms(10);
//				SS_DISADBE_Off();   
				SS_DISADBE_On();
				SysTick_Delay_Ms(100);
				DAC_Send(DAC_CMD_WriteREGn_UpDAC_n, DAC_CHall, 0);
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 0); 
			}
			else if(I_Fall_On_Off == I_Fall_On)
			{
				I_Fall_Function_On();		
//        SS_DISADBE_On();				
			}			
			break;
		case CP: 
				ON_OFF = OFF;
		
//			  DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13150);
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 16380);				
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13124);
//		
//		    DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 0);     //将想要的功率值转变为寄存器的值
//				SS_DISADBE_Off();   
				SS_DISADBE_On();
				SysTick_Delay_Ms(100);			
				Error_Stop_Flag = 0;//代表TIM1关闭
				TIM_Cmd(TIM1, DISABLE);//关闭定时器TIM1	
				TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);   //清除中断标志位
		
//				Error_Stop_Flag = 0;//代表TIM3关闭
//				TIM_Cmd(TIM3, DISABLE);//关闭定时器TIM3		
//				TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);   //清除中断标志位
		
				Error_Stop_Flag = 0;//代表TIM3关闭
				DAC_Send(DAC_CMD_WriteREGn_UpDAC_n, DAC_CHall, 0);
//				SysTick_Delay_Ms(10);	
//				SS_DISADBE_On();
		
			break;
		default: break;
	}
//	SCR_On_Off(DISABLE);
//	SCR_shift_T = 7800;
//	SysTick_Delay_Ms(100);	
		SCR_shift_T = 5000;
	SCR_On_Off(ENABLE);
//	PWM0_LOW;
//	PWM0_LOW;	
}


/**
  * @brief  输出零
  * @param  无
  * @retval 无
  */
void Output_Zero(void)
{
//	switch(CV_CC_CP)
//	{
//		case CV: 
//				ON_OFF = OFF;
//				
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 16380);				
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13124);
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13096);
//				
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 0);    //将想要的电压值转变为寄存器的值
//		  break;
//		case CC: 
//				ON_OFF = OFF;
//				
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13150);
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 16380);				
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13096);
//				
//		    DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 0);     //将想要的电流值转变为寄存器的值		
//			break;
//		case CP: 
//				ON_OFF = OFF;
//		
//			  DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13150);
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 16380);				
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13124);
//		
//		    DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 0);     //将想要的功率值转变为寄存器的值		
//			break;
//		default: break;
//	}	

				Error_Stop_Flag = 0;//代表TIM1关闭状态位
				TIM_Cmd(TIM1, DISABLE);//关闭定时器TIM1
				TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);   //清除中断标志位
				Error_Stop_Flag = 0;//代表TIM1关闭状态位
				DAC_Send(DAC_CMD_WriteREGn_UpDAC_n, DAC_CHall, 0);		


//				Error_Stop_Flag = 0;//代表TIM3关闭状态位
//				TIM_Cmd(TIM3, DISABLE);//关闭定时器TIM3
//				TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);   //清除中断标志位
//				Error_Stop_Flag = 0;//代表TIM3关闭状态位
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_n, DAC_CHall, 0);		
				SS_DISADBE_On();
//					SS_DISADBE_Off();  
//				SCR_shift_T = 7800;
				SCR_shift_T = 5000;

}




