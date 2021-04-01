#include "tim_oled.h"

u8 Cursor_Flash;                                  //����־λ�����ڱ�ʾ��ǰ����ʾ������գ�������ʽ����˸��
u8 RS232_Set_ID_BaudRate = RS232_First_Select;    //��ʾ�����趨����ID�Ż��ǲ�����
u8 CAN_Set_ID_BaudRate = CAN_First_Select;      //��ʾ�����趨����ID�Ż��ǲ�����
u16 RS232_ID = 01;                                     //��ʾ��ǰ��RS232�Ĵ��ں�
u32 RS232_BaudRate = 9600;                         //��ʾ��ǰ��RS232�Ĳ�����
u16 CAN_ID = 01;                                     //��ʾ��ǰ��CAN�Ĵ��ں�
u32 CAN_BaudRate = 100;                         //��ʾ��ǰ��CAN�Ĳ�����




//�����ж����ȼ�
static void TIM_OLED_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

//����TIM5����ʱ����Ϊ0.5s
static void TIM_OLED_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		
	  // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
		// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period=49999;
	  // ʱ��Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_Prescaler= 719;		
		// ʱ�ӷ�Ƶ���� 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		// ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	
		// �ظ���������ֵ
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	  // ��ʼ����ʱ��
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	
		// ����������жϱ�־λ
    TIM_ClearFlag(TIM5, TIM_FLAG_Update);
	  
		// �����������ж�
    TIM_ITConfig(TIM5, TIM_IT_Update,ENABLE);
		
		// ʹ�ܼ�����
    TIM_Cmd(TIM5, DISABLE);
}

//TIM5��ʼ��
void TIM_OLED_Init(void)
{
	TIM_OLED_NVIC_Config();
	TIM_OLED_Mode_Config();		
}

/*----------------------------------------RS232------------------------------------------------------*/

char String_RS232_ID[5];                          //RS232 ID���ַ���

/**
  * @brief  ��RS232 ID��תΪ�ַ���
  * @param  ID��
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
  * @brief  ��RS232 ������תΪ�ַ���
  * @param  ������
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

char String_CAN_ID[5];                          //RS232 ID���ַ���

/**
  * @brief  ��RS232 ID��תΪ�ַ���
  * @param  ID��
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
  * @brief  ��CAN ������תΪ�ַ���
  * @param  ������
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
  * @brief  TIM5���жϷ���������ת����,�������õ�ѹ������������ʱ�Ĺ��
  * @param  None
  * @retval None
  */
void TIM5_Oled_IT(void)
{
	if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) 
	{		
		if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_V_set) && (Keys_Encoder_Mode == Encoder_Mode))             //�������õ�ѹģʽ�ͱ�����ģʽ
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
		  TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //����жϱ�־λ
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_V_set) && (Keys_Encoder_Mode == Keys_Mode))          //�������õ�ѹģʽ�ͼ���ģʽ
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
		  TIM_ClearITPendingBit(TIM5, TIM_IT_Update);   //����жϱ�־λ				
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_I_set) && (Keys_Encoder_Mode == Encoder_Mode))      //�������õ���ģʽ�ͱ�����ģʽ
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
		  TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //����жϱ�־λ
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_I_set) && (Keys_Encoder_Mode == Keys_Mode))          //�������õ���ģʽ�ͼ���ģʽ
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
		  TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //����жϱ�־λ
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_P_set) && (Keys_Encoder_Mode == Encoder_Mode))        //���ڹ�������ģʽ�ͱ�����ģʽ
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
		  TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //����жϱ�־λ			
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_P_set) && (Keys_Encoder_Mode == Keys_Mode))          //�������ù���ģʽ�ͼ���ģʽ
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
		  TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //����жϱ�־λ
		}		
		
/*-----------------------------------------------------RS232-------------------------------------------------------------------*/		
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == RS232))      //ѡ����RS232,��ʾ��RS232����������
		{
			if(RS232_Set_ID_BaudRate == RS232_Set_ID)     //�趨RS232��ID��
			{
				if (Keys_Encoder_Mode == Encoder_Mode)       //������ģʽ
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
			TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //����жϱ�־λ
		}
		
/*---------------------------------------------------------------------------------CAN---------------------------------------------*/		
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == CAN))      //ѡ����CAN,��ʾ��CAN����������
		{
			if(CAN_Set_ID_BaudRate == CAN_Set_ID)     //�趨CAN��ID��
			{
				if (Keys_Encoder_Mode == Encoder_Mode)       //������ģʽ
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
			
			else if (CAN_Set_ID_BaudRate == CAN_Set_BaudRate)    //���ò�����
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
			TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //����жϱ�־λ
		}
		
/*------------------------------------------------------------------------------------Cycle------------------------------------------------------------------*/
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Cycle))      //ѡ����Cycle,��ʾ��Cycle����������
		{				
			if (Cycle_Enter == Cycle_Enter_1)       //����ѭ��������
			{
				if (Keys_Encoder_Mode == Encoder_Mode)       //������ģʽ
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
					
        else if (Keys_Encoder_Mode == Keys_Mode)     //�����趨ģʽ
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
			
			else if (Cycle_Enter == Cycle_Enter_2)       //����ѭ��������
			{
				if (Keys_Encoder_Mode == Encoder_Mode)       //������ģʽ
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
					
        else if (Keys_Encoder_Mode == Keys_Mode)      //�����趨ģʽ
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
			
			TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //����жϱ�־λ			
		 }
		
/*--------------------------------------------------------V-Rise------------------------------------------------------------*/		 
		 
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == V_Rise))      //ѡ����V-Rise,��ʾ��V-Rise����������
		{								
			if (V_Rise_Enter == V_Rise_Enter_1)       //����ѭ��������
			{
				if (Keys_Encoder_Mode == Encoder_Mode)       //������ģʽ
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
					
        else if (Keys_Encoder_Mode == Keys_Mode)     //�����趨ģʽ
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
			TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //����жϱ�־λ			
		}			

/*--------------------------------------------------------V-Fall------------------------------------------------------------*/		 
		 
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == V_Fall))      //ѡ����V-Fall,��ʾ��V-Fall����������
		{				
			if (V_Fall_Enter == V_Fall_Enter_1)       //����ѭ��������
			{
				if (Keys_Encoder_Mode == Encoder_Mode)       //������ģʽ
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
					
        else if (Keys_Encoder_Mode == Keys_Mode)     //�����趨ģʽ
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
			TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //����жϱ�־λ			
		 		
		}	
		
/*--------------------------------------------------------I-Rise------------------------------------------------------------*/		 
		 
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == I_Rise))      //ѡ����V-Rise,��ʾ��V-Rise����������
		{		
			if (I_Rise_Enter == I_Rise_Enter_1)       //����ѭ��������
			{
				if (Keys_Encoder_Mode == Encoder_Mode)       //������ģʽ
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
					
        else if (Keys_Encoder_Mode == Keys_Mode)     //�����趨ģʽ
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
			
			 TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //����жϱ�־λ				
		 }			


/*--------------------------------------------------------I-Fall------------------------------------------------------------*/		 
		 
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == I_Fall))      //ѡ����V-Rise,��ʾ��V-Rise����������
		{		
			if (I_Fall_Enter == I_Fall_Enter_1)       //����ѭ��������
			{
				if (Keys_Encoder_Mode == Encoder_Mode)       //������ģʽ
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
					
        else if (Keys_Encoder_Mode == Keys_Mode)     //�����趨ģʽ
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
			
			 TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //����жϱ�־λ			
		 }			 
		 	 
/*--------------------------------------------------------Delay----------------------------------------------------*/
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Delay))  //�����ڵڶ����˵���Delay
		{	
			if (Keys_Encoder_Mode == Encoder_Mode)   //������ڱ�����ģʽ
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
			
			TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //����жϱ�־λ
		}		 
		
		
		
		
		else      //����������ģʽ���˳��ж�
		{
			TIM_Cmd(TIM5, DISABLE);
			TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);   //����жϱ�־λ
		}
	}		
}



















