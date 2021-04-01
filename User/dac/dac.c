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



u8 Error_Stop_Flag = 0;  //���ƶ�ʱ��TIM3���ص�״̬λ,0����TIM3���ڹر�״̬��1����TIM3���ڿ���״̬
//u8 Converter_Flag = 0;   //�����л�ģʽ��״̬λ��0�������ϴ�ģʽ��ͬ��1�������ϴ�ģʽ��ͬ
//u8 On_Flag = 0;      //ģʽ��ͬʱ���ڵ�״̬λ��0�������ΪCV_Vint, 1�������ΪCV_Vext, 2�������ΪCC�� 3�������ΪCP

u8 Wait_Below_Set_Value_Flag = Already_Below;






/**
  * @brief  SPI1��ʼ��
  * @param  ��
  * @retval ��
  */
void SPI1_DAC_Config(void)                //16383
{
	SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	
	/*��ʱ��*/
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
		
	Output_Zero();   //һ��ʼ��·0
}

/**
  * @brief  SPI1����һ�ֽ�
  * @param  Ҫ���͵��ֽ�
  * @retval ��
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
  * @brief  ����DAC�趨ֵ
  * @param  ���������ѹ����
  * @param  ������ѹ����
  * @param  ֵ
  * @retval ��
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
  * @brief  ��ʼ�����û�п�slope��ֱ����������˾͵���Function_On����
  * @param  ��
  * @retval ��
  */
void Power_On(void)
{		
//	  PWM0_HIGH;
//	  PWM1_HIGH;	
	  switch(CV_CC_CP)
	  {
		  case CV:      
				//��ѹģʽ
	
			  if(Use_Vint_Or_Vext_Flag == Use_Vint)
		  	{
				
//			  	if(Voltage > Set_Voltage)     //��ѹ��û�����趨��ѹ���£��ȴ������趨��ѹ����
//			    {
//			  	  Wait_Below_Set_Value_Flag = Havent_Below;
////			   	  Power_Off();			
//			  	  ON_OFF = ON;				
//			    }
//					else
					if(V_Rise_On_Off == V_Rise_Off)         //û�д򿪻���ģʽ
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
					
			    else if(V_Rise_On_Off == V_Rise_On)       //���˻���ģʽ
			    {
			  	  Wait_Below_Set_Value_Flag = Already_Below;				
			  	  V_Rise_Function_On();
						SS_DISADBE_Off();
						
		  	  }
				
		      break;
			}
			else if(Use_Vint_Or_Vext_Flag == Use_Vext)     //���ʹ���ⲿ��ѹ
			{
	
//				if(Voltage > Set_Voltage)     //��ѹ��û�����趨��ѹ���£��ȴ������趨��ѹһ��
//			  {
//				  Wait_Below_Set_Value_Flag = Havent_Below;
//				  Power_Off();			
//				  ON_OFF = ON;				
//			  }
//		    else 
				if(V_Rise_On_Off == V_Rise_Off)         //û�д򿪻���ģʽ
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
				
			  else if(V_Rise_On_Off == V_Rise_On)       //���˻���ģʽ
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
						
//			if(Current > Set_Current)     //��ѹ��û�����趨��ѹ���£��ȴ������趨��ѹһ��
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
//				SysTick_Delay_Ms(600);					 //�������룬��û��������ӳ٣���SS���Ѿ��ﵽ0.5V
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
////		    DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, Set_Current * Set_Current * 0.0006 + 131.26 * Set_Current - 7.9388);     //����Ҫ�ĵ���ֵת��Ϊ�Ĵ�����ֵ
////				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 128.82 * Set_Current + 40.966);
//				
////				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 129.38 * Set_Current + 29.218);  // 1ŷ
//					DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 131.48 * Set_Current + 70.61 );

				

			}
			else if(I_Rise_On_Off == I_Rise_On)
			{
				
				SS_DISADBE_Off();
				SysTick_Delay_Ms(100);	
				I_Rise_Function_On();
				
			}			
			
		
			break;
			
			
		case CP: //��Ҫ�޸ĵĻ�������
		
//			if(Power > Set_Power)     //��ѹ��û�����趨��ѹ���£��ȴ������趨��ѹһ��
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
//					DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 13107);		//ԭ��16380		
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
					
					P_Slope_Function_On();//���ʻ���
					
//				}
	
			break;
		  default: break;
	}
// }
	
	
	
}


/**
  * @brief  �ر����
  * @param  ��
  * @retval ��
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
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 0);    //����Ҫ�ĵ�ѹֵת��Ϊ�Ĵ�����ֵ
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
//		    DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 0);     //����Ҫ�ĵ���ֵת��Ϊ�Ĵ�����ֵ
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
//		    DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 0);     //����Ҫ�Ĺ���ֵת��Ϊ�Ĵ�����ֵ
//				SS_DISADBE_Off();   
				SS_DISADBE_On();
				SysTick_Delay_Ms(100);			
				Error_Stop_Flag = 0;//����TIM1�ر�
				TIM_Cmd(TIM1, DISABLE);//�رն�ʱ��TIM1	
				TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);   //����жϱ�־λ
		
//				Error_Stop_Flag = 0;//����TIM3�ر�
//				TIM_Cmd(TIM3, DISABLE);//�رն�ʱ��TIM3		
//				TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);   //����жϱ�־λ
		
				Error_Stop_Flag = 0;//����TIM3�ر�
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
  * @brief  �����
  * @param  ��
  * @retval ��
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
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 0);    //����Ҫ�ĵ�ѹֵת��Ϊ�Ĵ�����ֵ
//		  break;
//		case CC: 
//				ON_OFF = OFF;
//				
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13150);
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 16380);				
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 13096);
//				
//		    DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 0);     //����Ҫ�ĵ���ֵת��Ϊ�Ĵ�����ֵ		
//			break;
//		case CP: 
//				ON_OFF = OFF;
//		
//			  DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 13150);
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vext, 16380);				
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, 13124);
//		
//		    DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, 0);     //����Ҫ�Ĺ���ֵת��Ϊ�Ĵ�����ֵ		
//			break;
//		default: break;
//	}	

				Error_Stop_Flag = 0;//����TIM1�ر�״̬λ
				TIM_Cmd(TIM1, DISABLE);//�رն�ʱ��TIM1
				TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);   //����жϱ�־λ
				Error_Stop_Flag = 0;//����TIM1�ر�״̬λ
				DAC_Send(DAC_CMD_WriteREGn_UpDAC_n, DAC_CHall, 0);		


//				Error_Stop_Flag = 0;//����TIM3�ر�״̬λ
//				TIM_Cmd(TIM3, DISABLE);//�رն�ʱ��TIM3
//				TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);   //����жϱ�־λ
//				Error_Stop_Flag = 0;//����TIM3�ر�״̬λ
//				DAC_Send(DAC_CMD_WriteREGn_UpDAC_n, DAC_CHall, 0);		
				SS_DISADBE_On();
//					SS_DISADBE_Off();  
//				SCR_shift_T = 7800;
				SCR_shift_T = 5000;

}




