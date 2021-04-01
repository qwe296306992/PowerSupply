#include "encoder.h"

/**
  * @brief  ��������ʼ��
  * @param  None
  * @retval None
  */
void Encode_Init(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
	TIM_ICInitTypeDef        TIM_ICInitStructure;
	GPIO_InitTypeDef         GPIO_InitStructure;
	NVIC_InitTypeDef         NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);//AHBû��GPIOA��AFIO���裿��
	
	GPIO_StructInit(&GPIO_InitStructure);//��ʼ��GPIO�ṹ��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//PA0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//PA1
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ֻ��ʼ��һ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;//�ظ�������
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
	
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Period = TIM_Encoder_Period;                     //�趨��������װֵ
  TIM_TimeBaseStructure.TIM_Prescaler = 0;                      //TIM2ʱ��Ԥ��Ƶֵ
  TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;       //����ʱ�ӷָ� T_dts = T_ck_int    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //TIM���ϼ��� 
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); //ʹ�ñ�����ģʽ3�������½�������
	TIM_ICStructInit(&TIM_ICInitStructure);//���ṹ���е�����ȱʡ����
  TIM_ICInitStructure.TIM_ICFilter = 6;  //ѡ������Ƚ��˲��� 
  TIM_ICInit(TIM2, &TIM_ICInitStructure);//��TIM_ICInitStructure�е�ָ��������ʼ��TIM3
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//���TIM2�ĸ��±�־λ
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//���и����ж�
	TIM2->CNT = 0; //Reset counter�����������ֵ
	TIM_Cmd(TIM2, ENABLE);   //����TIM2��ʱ��
}



/**
  * @brief  �Ե�ѹ�ڱ�����ģʽ�½�λ�Ĵ���
  * @param  None
  * @retval None
  */
static void Voltage_Carry(void)
{
	float Encoder_Voltage;
	
	Encoder_Voltage = String_To_Float(String_Voltage);
	
	if (Encoder_Voltage < 80)   //δ�����ֵ
	{
		if (String_Voltage[4] == ':')              //С������λ����9
		{
			String_Voltage[4] = '0';
			String_Voltage[3] = String_Voltage[3] + 1;       //��λ��1
			Write_String_16x32AsicII(0, 0, String_Voltage);  //�ı����ʾһ��	
		}
		if (String_Voltage[3] == ':')        //С����һλ����9
		{
			String_Voltage[3] = '0';       
			String_Voltage[1] = String_Voltage[1] + 1;       //��λ��1
			Write_String_16x32AsicII(0, 0, String_Voltage);  //�ı����ʾһ��				
		}
		if (String_Voltage[1] == ':')           //��λ����9
		{
			String_Voltage[1] = '0';
			String_Voltage[0] = String_Voltage[0] + 1;       //��λ��1
			Write_String_16x32AsicII(0, 0, String_Voltage);  //�ı����ʾһ��			
		}		
		if (String_Voltage[0] == ':')
		{
			String_Voltage[0] = '0';             //ʮλ����9
			Write_String_16x32AsicII(0, 0, String_Voltage);  //�ı����ʾһ��
		}
	}
	else
	{
		strcpy(String_Voltage, "80.00");
		Write_String_16x32AsicII(0, 0, String_Voltage);  //�ı����ʾһ��
	}
}

/**
  * @brief  �Ե�ѹ�ڱ�����ģʽ����λ�Ĵ���
  * @param  None
  * @retval None
  */
static void Voltage_Abdication(void)
{
	float Encoder_Voltage;
	
	Encoder_Voltage = String_To_Float(String_Voltage);
	
	if (Encoder_Voltage > 0)   //δ����Сֵ
	{
		if (String_Voltage[4] == '/')              //ʮλ����0
		{
			String_Voltage[4] = '9';
			String_Voltage[3] = String_Voltage[3] - 1;       //��λ��1
			Write_String_16x32AsicII(0, 0, String_Voltage);  //�ı����ʾһ��	
		}
		if (String_Voltage[3] == '/')        //��λ����0
		{
			String_Voltage[3] = '9';       
			String_Voltage[1] = String_Voltage[1] - 1;       //��λ��1
			Write_String_16x32AsicII(0, 0, String_Voltage);  //�ı����ʾһ��				
		}
		if (String_Voltage[1] == '/')           //С����һλ����0
		{
			String_Voltage[1] = '9';
			String_Voltage[0] = String_Voltage[0] - 1;       //��λ��1
			Write_String_16x32AsicII(0, 0, String_Voltage);  //�ı����ʾһ��			
		}		
		if (String_Voltage[0] == '/')
		{
			String_Voltage[0] = '0';             //ʮλ����0
			Write_String_16x32AsicII(0, 0, String_Voltage);  //�ı����ʾһ��
		}
	}
	else
	{
		strcpy(String_Voltage, "00.00");
		Write_String_16x32AsicII(0, 0, String_Voltage);  //�ı����ʾһ��
	}
}

/**
  * @brief  �Ե����ڱ�����ģʽ�½�λ�Ĵ���
  * @param  None
  * @retval None
  */
static void Current_Carry(void)
{
	float Encoder_Current;
	
	Encoder_Current = String_To_Float(String_Current);
	
	if (Encoder_Current < 100)   //δ�����ֵ
	{
		if (String_Current[5] == ':')              //С������λ����9
		{
			String_Current[5] = '0';
			String_Current[4] = String_Current[4] + 1;       //��λ��1
			Write_String_16x32AsicII(0, 36, String_Current);  //�ı����ʾһ��	
		}
		if (String_Current[4] == ':')        //С����һλ����9
		{
			String_Current[4] = '0';       
			String_Current[2] = String_Current[2] + 1;       //��λ��1
			Write_String_16x32AsicII(0, 36, String_Current);  //�ı����ʾһ��				
		}
		if (String_Current[2] == ':')           //��λ����9
		{
			String_Current[2] = '0';
			String_Current[1] = String_Current[1] + 1;       //��λ��1
			Write_String_16x32AsicII(0, 36, String_Current);  //�ı����ʾһ��			
		}		
		if (String_Current[1] == ':')           //ʮλ����9
		{
			String_Current[1] = '0';
			String_Current[0] = String_Current[0] + 1;       //��λ��1
			Write_String_16x32AsicII(0, 36, String_Current);  //�ı����ʾһ��			
		}	
		if (String_Current[0] == ':')
		{
			String_Current[0] = '0';             //��λ����9
			Write_String_16x32AsicII(0, 36, String_Current);  //�ı����ʾһ��
		}
	}
	else
	{
		strcpy(String_Current, "100.00");
		Write_String_16x32AsicII(0, 36, String_Current);  //�ı����ʾһ��
	}
}

/**
  * @brief  �Ե����ڱ�����ģʽ����λ�Ĵ���
  * @param  None
  * @retval None
  */
static void Current_Abdication(void)
{
	float Encoder_Current;
	
	Encoder_Current = String_To_Float(String_Current);
	
	if (Encoder_Current > 0)   //δ�����ֵ
	{
		if (String_Current[5] == '/')              //С������λ����0
		{
			String_Current[5] = '9';
			String_Current[4] = String_Current[4] - 1;       //��λ��1
			Write_String_16x32AsicII(0, 36, String_Current);  //�ı����ʾһ��	
		}
		if (String_Current[4] == '/')        //С����һλ����0
		{
			String_Current[4] = '9';       
			String_Current[2] = String_Current[2] - 1;       //��λ��1
			Write_String_16x32AsicII(0, 36, String_Current);  //�ı����ʾһ��				
		}
		if (String_Current[2] == '/')           //��λ����0
		{
			String_Current[2] = '9';
			String_Current[1] = String_Current[1] - 1;       //��λ��1
			Write_String_16x32AsicII(0, 36, String_Current);  //�ı����ʾһ��			
		}		
		if (String_Current[1] == '/')           //ʮλ����0
		{
			String_Current[1] = '9';
			String_Current[0] = String_Current[0] - 1;       //��λ��1
			Write_String_16x32AsicII(0, 36, String_Current);  //�ı����ʾһ��			
		}	
		if (String_Current[0] == '/')
		{
			String_Current[0] = '0';             //��λ����0
			Write_String_16x32AsicII(0, 36, String_Current);  //�ı����ʾһ��
		}
	}
	else
	{
		strcpy(String_Current, "000.00");
		Write_String_16x32AsicII(0, 36, String_Current);  //�ı����ʾһ��
	}
}

/**
  * @brief  �Թ����ڱ�����ģʽ�� ��λ�Ĵ���
  * @param  None
  * @retval None
  */
static void Power_Carry(void)
{
	float Encoder_Power;
	
	Encoder_Power = String_To_Float(String_Power);
	
	String_Power[Cursor_Position] = String_Power[Cursor_Position] + 1;
	
	if(Encoder_Power < 6000)
	{
		if(String_Power[4] == ':')
		{
			String_Power[4] = '0';                  //����9���
			
			if(String_Power[3] != '.')              //����С����
			{
				String_Power[3] = String_Power[3] + 1;
			}
			else
			{
				String_Power[2] = String_Power[2] + 1;
			}
			
			Write_String_12x24AsicII(42, 0, String_Power);  //�ı����ʾһ��
		}
		if (String_Power[3] == ':')
		{
			String_Power[3] = '0';                  //����9���0
			
			if(String_Power[2] != '.')              //����С����
			{
				String_Power[2] = String_Power[2] + 1;
			}
			else
			{
				String_Power[1] = String_Power[1] + 1;
			}
			
			Write_String_12x24AsicII(42, 0, String_Power);  //�ı����ʾһ��			
		}
		if (String_Power[2] == ':')
		{
			String_Power[2] = '0';                  //����9���
			
			if(String_Power[1] != '.')              //����С����
			{
				String_Power[1] = String_Power[1] + 1;
			}
			else
			{
				String_Power[0] = String_Power[0] + 1;
			}
			Write_String_12x24AsicII(42, 0, String_Power);  //�ı����ʾһ��			
		}
		if (String_Power[1] == ':')
		{
			String_Power[1] = '0';                  //����9���
			
			String_Power[0] = String_Power[0] + 1;
			
			Write_String_12x24AsicII(42, 0, String_Power);  //�ı����ʾһ��	
		}
		if (String_Power[0] == ':')
		{
			if(Encoder_Power < 100)
			{
				String_Power[4] = String_Power[3];
				String_Power[3] = String_Power[2];
				String_Power[2] = String_Power[1];
				String_Power[1] = '0';
				String_Power[0] = '1';
				
				if(Cursor_Position == 4)
				{
					Cursor_Position = 4;
				}
				else
				{
					Cursor_Position = Cursor_Position + 1;
				}
			}
			else if(Encoder_Power < 1000)
			{
				String_Power[4] = String_Power[2];
				String_Power[3] = String_Power[1];
				String_Power[2] = '0';
				String_Power[1] = '1';
				String_Power[0] = '0';
				
				if(Cursor_Position == 4)
				{
					Cursor_Position = 4;
				}
				else
				{
					Cursor_Position = Cursor_Position + 2;
				}
			}
			else
			{
				String_Power[0] = '0';                  //����9���
			}
			
			Write_String_12x24AsicII(42, 0, String_Power);  //�ı����ʾһ��				
		}
	}
	else
	{
		strcpy(String_Power, "06000");
		Write_String_12x24AsicII(42, 0, String_Power);  //�ı����ʾһ��
	}
}


/**
  * @brief  �Թ����ڱ�����ģʽ����λ�Ĵ���
  * @param  None
  * @retval None
  */
static void Power_Abdication(void)
{
	float Encoder_Power;
	
	Encoder_Power = String_To_Float(String_Power);
	
	String_Power[Cursor_Position] = String_Power[Cursor_Position] - 1;
	
	if (Encoder_Power > 0)
	{
		if (String_Power[4] == '/')
		{
			String_Power[4] = '9';            //����0���9
			
			if(String_Power[3] != '.')        //����С����
			{
				String_Power[3] = String_Power[3] - 1;
			}
			else
			{
				String_Power[2] = String_Power[2] - 1;
			}
			
			Write_String_12x24AsicII(42, 0, String_Power);  //�ı����ʾһ��
		}
		if (String_Power[3] == '/')
		{
			String_Power[3] = '9';            //����0���9
			
			if(String_Power[2] != '.')        //����С����
			{
				String_Power[2] = String_Power[2] - 1;
			}
			else
			{
				String_Power[1] = String_Power[1] - 1;
			}
			
			Write_String_12x24AsicII(42, 0, String_Power);  //�ı����ʾһ��			
		}
		if (String_Power[2] == '/')
		{
			String_Power[2] = '9';            //����0���9
			
			if(String_Power[1] != '.')        //����С����
			{
				String_Power[1] = String_Power[1] - 1;
			}
			else
			{
				String_Power[0] = String_Power[0] - 1;
			}
			Write_String_12x24AsicII(42, 0, String_Power);  //�ı����ʾһ��			
		}
		
		
		if((String_Power[1] == '0') && (String_Power[0] == '0'))   //1000��λ
		{
			if(Encoder_Power >= 1000)
			{
				String_Power[0] = String_Power[2];
				String_Power[1] = String_Power[3];
				String_Power[2] = String_Power[4];
				String_Power[3] = '.';
				String_Power[4] = '0';
				
				if((Cursor_Position == 0) || (Cursor_Position == 1) || (Cursor_Position == 2))
				{
					Cursor_Position = 0;
				}
				else
				{
					Cursor_Position = Cursor_Position - 2;
				}
			
				if (String_Power[0] == '0')    //�����λ������100
				{
						String_Power[0] = String_Power[1];
						String_Power[1] = String_Power[2];
						String_Power[2] = String_Power[3];
						String_Power[3] = String_Power[4];
						String_Power[4] = '0';
				
						if(Cursor_Position == 0)
						{
							Cursor_Position = 0;
						}
						else
						{
							Cursor_Position = Cursor_Position - 1;
						}	
				}
			
			Write_String_12x24AsicII(42, 0, String_Power);  //�ı����ʾһ��	
			
			}
		}
		else if (String_Power[1] == '/')
		{
			String_Power[1] = '9';            //����0���9
			
			String_Power[0] = String_Power[0] - 1;
			
			Write_String_12x24AsicII(42, 0, String_Power);  //�ı����ʾһ��	
		}
		
		
		if (String_Power[0] == '0')
		{
		  if((Encoder_Power >= 100) && (Encoder_Power < 1000))   //100��λ
			{
				String_Power[0] = String_Power[1];
				String_Power[1] = String_Power[2];
				String_Power[2] = String_Power[3];
				String_Power[3] = String_Power[4];
				String_Power[4] = '0';
				
				if(Cursor_Position == 0)
				{
					Cursor_Position = 0;
				}
				else
				{
					Cursor_Position = Cursor_Position - 1;
				}
			}
			
			else 
			{
				String_Power[0] = '0';            //����0���9
			}
			
			Write_String_12x24AsicII(42, 0, String_Power);  //�ı����ʾһ��				
		}
	}
	else
	{
		strcpy(String_Power, "00.00");
		Write_String_12x24AsicII(42, 0, String_Power);  //�ı����ʾһ��
	}
}

/*----------------------------------------------------------------����ID����������----------------------------------------------------------------*/

/**
  * @brief  �Դ���ID�ڱ�����ģʽ����λ�Ĵ���
  * @param  ����id�ŵ��ַ�����ʽ
  * @retval None
  */
static void Usart_Carry(char* String_ID)
{
	float ID;
	
	ID = String_To_Float(String_ID);
	
  if (ID < 17)
	{
		if(String_ID[1] == ':')
		{
			String_ID[1] = '0';
			String_ID[0] = String_ID[0] + 1;
			Write_String_16x32AsicII(0, 12, String_ID);  //�ı����ʾһ��
		}
	}
	else
	{
		strcpy(String_ID, "16");
		Write_String_16x32AsicII(0, 12, String_ID);  //�ı����ʾһ��
	}
}

/**
  * @brief  �Դ���ID�ڱ�����ģʽ����λ�Ĵ���
  * @param  ����id�ŵ��ַ�����ʽ
  * @retval None
  */
static void Usart_Abdication(char* String_ID)
{
	float ID;
	
	ID = String_To_Float(String_ID);
	
  if (ID > 0)
	{
		if(String_ID[2] == '/')
		{
			String_ID[2] = '9';
			String_ID[1] = String_ID[1] - 1;
			
			Write_String_16x32AsicII(0, 12, String_ID);  //�ı����ʾһ��
		}
		if(String_ID[1] == '/')
		{
			String_ID[1] = '9';
			String_ID[0] = String_ID[0] - 1;
			Write_String_16x32AsicII(0, 12, String_ID);  //�ı����ʾһ��
		}
	}
	else
	{
		strcpy(String_ID, "01");
		Write_String_16x32AsicII(0, 12, String_ID);  //�ı����ʾһ��
	}
}

/*--------------------------------------------------------------------Cycle����λ����--------------------------------------------------------------*/

/**
  * @brief  ��CYCLE�ڱ�����ģʽ�½�λ�Ĵ���
  * @param  ��
  * @retval None
  */
static void Cycle_Carry(char* String_Cycle)
{
	if(String_Cycle[7] == ':')
	{
		String_Cycle[7] = '0';
		String_Cycle[6] ++ ;
	}
	if(String_Cycle[6] == ':')
	{
		String_Cycle[6] = '0';
		String_Cycle[5] ++;
	}
	if(String_Cycle[5] == ':')
	{
		String_Cycle[5] = '0';
		String_Cycle[3] ++;
	}
	if(String_Cycle[3] == ':')
	{
		String_Cycle[3] = '0';
		String_Cycle[2] ++;
	}
	if(String_Cycle[2] == ':')
	{
		String_Cycle[2] = '0';
		String_Cycle[1] ++;
	}
	if(String_Cycle[1] == ':')
	{
		String_Cycle[1] = '0';
		String_Cycle[0] ++;
	}
	if(String_Cycle[0] == ':')
	{
		String_Cycle[0] = '9';
		String_Cycle[1] = '9';
		String_Cycle[2] = '9';
		String_Cycle[3] = '9';
		String_Cycle[5] = '9';
		String_Cycle[6] = '9';
		String_Cycle[7] = '9';
	}
}

/**
  * @brief  ��CYCLE�ڱ�����ģʽ����λ�Ĵ���
  * @param  ��
  * @retval None
  */
static void Cycle_Abdication(char* String_Cycle)
{
	if(String_Cycle[7] == '/')
	{
		String_Cycle[7] = '9';
		String_Cycle[6] -- ;
	}
	if(String_Cycle[6] == '/')
	{
		String_Cycle[6] = '9';
		String_Cycle[5] --;
	}
	if(String_Cycle[5] == '/')
	{
		String_Cycle[5] = '9';
		String_Cycle[3] --;
	}
	if(String_Cycle[3] == '/')
	{
		String_Cycle[3] = '9';
		String_Cycle[2] --;
	}
	if(String_Cycle[2] == '/')
	{
		String_Cycle[2] = '9';
		String_Cycle[1] --;
	}
	if(String_Cycle[1] == '/')
	{
		String_Cycle[1] = '9';
		String_Cycle[0] --;
	}
	if(String_Cycle[0] == '/')
	{
		String_Cycle[0] = '0';
		String_Cycle[1] = '0';
		String_Cycle[2] = '0';
		String_Cycle[3] = '0';
		String_Cycle[5] = '0';
		String_Cycle[6] = '0';
		String_Cycle[7] = '0';
	}
}

/*--------------------------------------------------------------------Slope����λ����--------------------------------------------------------------*/

/**
  * @brief  ��V_Rise�����ڱ�����ģʽ�½�λ�Ĵ���
  * @param  ��
  * @retval None
  */
static void Slope_Carry(char* String_Slope)
{
	if(String_Slope[7] == ':')
	{
		String_Slope[7] = '0';
		String_Slope[6] ++ ;
	}
	if(String_Slope[6] == ':')
	{
		String_Slope[6] = '0';
		String_Slope[5] ++;
	}
	if(String_Slope[5] == ':')
	{
		String_Slope[5] = '0';
		String_Slope[3] ++;
	}
	if(String_Slope[3] == ':')
	{
		String_Slope[3] = '0';
		String_Slope[2] ++;
	}
	if(String_Slope[2] == ':')
	{
		String_Slope[2] = '0';
		String_Slope[1] ++;
	}
	if(String_Slope[1] == ':')
	{
		String_Slope[1] = '0';
		String_Slope[0] ++;
	}
	if(String_Slope[0] == ':')
	{
		String_Slope[0] = '9';
		String_Slope[1] = '9';
		String_Slope[2] = '9';
		String_Slope[3] = '9';
		String_Slope[5] = '9';
		String_Slope[6] = '9';
		String_Slope[7] = '9';
	}
}

/**
  * @brief  ��SLOPE�ڱ�����ģʽ����λ�Ĵ���
  * @param  ��
  * @retval None
  */
static void Slope_Abdication(char* String_Slope)
{
	if(String_Slope[7] == '/')
	{
		String_Slope[7] = '9';
		String_Slope[6] -- ;
	}
	if(String_Slope[6] == '/')
	{
		String_Slope[6] = '9';
		String_Slope[5] --;
	}
	if(String_Slope[5] == '/')
	{
		String_Slope[5] = '9';
		String_Slope[3] --;
	}
	if(String_Slope[3] == '/')
	{
		String_Slope[3] = '9';
		String_Slope[2] --;
	}
	if(String_Slope[2] == '/')
	{
		String_Slope[2] = '9';
		String_Slope[1] --;
	}
	if(String_Slope[1] == '/')
	{
		String_Slope[1] = '9';
		String_Slope[0] --;
	}
	if(String_Slope[0] == '/')
	{
		String_Slope[0] = '0';
		String_Slope[1] = '0';
		String_Slope[2] = '0';
		String_Slope[3] = '0';
		String_Slope[5] = '0';
		String_Slope[6] = '0';
		String_Slope[7] = '0';
	}
}


/*---------------------------------------------------------------------------Delay------------------------------------------------------------------------------*/
/**
  * @brief  ��Delay�ڱ�����ģʽ�½�λ�Ĵ���
  * @param  h
  * @retval None
  */
static void Delay_Carry(char* String_Delay)
{
	if(String_Delay[7] == ':')
	{
		String_Delay[7] = '0';
		String_Delay[6] ++;
	}
	if(String_Delay[6] == '6')
	{
		String_Delay[6] = '0';
		String_Delay[4] ++;
	}
	if(String_Delay[4] == ':')
	{
		String_Delay[4] = '0';
		String_Delay[3] ++;
	}
	if(String_Delay[3] == '6')
	{
		String_Delay[3] = '0';
		String_Delay[1] ++;
	}
	if(String_Delay[1] == ':')
	{
		String_Delay[1] = '0';
		String_Delay[0] ++;
	}
	if(String_Delay[0] == ':')
	{
		String_Delay[0] = '9';
		String_Delay[1] = '9';
		String_Delay[3] = '5';
		String_Delay[4] = '9';
		String_Delay[6] = '5';
		String_Delay[7] = '9';
	}
}

/**
  * @brief  ��Delay�ڱ�����ģʽ����λ�Ĵ���
  * @param  h
  * @retval None
  */
static void Delay_Abdication(char* String_Delay)
{
	if(String_Delay[7] == '/')
	{
		String_Delay[7] = '9';
		String_Delay[6] --;
	}
	if(String_Delay[6] == '/')
	{
		String_Delay[6] = '5';
		String_Delay[4] --;
	}
	if(String_Delay[4] == '/')
	{
		String_Delay[4] = '9';
		String_Delay[3] --;
	}
	if(String_Delay[3] == '/')
	{
		String_Delay[3] = '5';
		String_Delay[1] --;
	}
	if(String_Delay[1] == '/')
	{
		String_Delay[1] = '9';
		String_Delay[0] --;
	}
	if(String_Delay[0] == '/')
	{
		String_Delay[0] = '0';
		String_Delay[1] = '0';
		String_Delay[3] = '0';
		String_Delay[4] = '0';
		String_Delay[6] = '0';
		String_Delay[7] = '0';
	}
}


/*-----------------------------------------------------------------�жϷ�����--------------------------------------------------------------------------*/

/**
  * @brief  TIM2���жϷ���������ת����,���ڱ��������ж�,�üĴ���CR1���б�����ת
  * @param  None
  * @retval None
  */
void TIM2_Encoder_IT(void)
{
	static u8 Last_State;
	u8 State;
	const u8 Forward = 1;
	const u8 Reverse = 0;
	
	if((TIM_GetITStatus( TIM2, TIM_IT_Update) != RESET))          //ȷ�ϱ���������ж���
	{
		if(Keys_Encoder_Mode == Encoder_Mode)                     //ֻ�б�����ģʽ������
		{
			SysTick_Delay_Ms(10);            //��ʱ��ȷ��������Ϊ�Ĵ����ķ�Ӧ�Ƚ�����
	    
      if (TIM2 -> CR1 == 0x00000001)   //��ʾ��ת
			{
				State = Forward;
			}				
			else if (TIM2 -> CR1 == 0x00000011)  //��ʾ��ת
			{
				State = Reverse;
			}
			
		  if((Last_State == State) && (State == Forward))         //��ʾ��ת��ת����ϴ���ͬ
	    {
			  if(Display_Interface_Mode == Display_Interface_Mode_Main_Interface_V_set)             //�������õ�ѹģʽ
			  {
				  String_Voltage[Cursor_Position] = String_Voltage[Cursor_Position]+1;                //��תһ�Σ���Ӧ���ַ������ּ�1
					
          Voltage_Carry();                                                                    //��λ
					
				  Write_Single_16x32AsicII(0, Cursor_Position * 4, String_Voltage[Cursor_Position]);  //�ı����ʾһ��
          SysTick_Delay_Ms(5);				
			  }
			
			  else if(Display_Interface_Mode == Display_Interface_Mode_Main_Interface_I_set)        //�������õ���ģʽ
			  {
				  String_Current[Cursor_Position] = String_Current[Cursor_Position]+1;                //��תһ�Σ���Ӧ���ַ������ּ�1
					
          Current_Carry();     //��λ
					
          Write_Single_16x32AsicII(0, Cursor_Position*4 + 36, String_Current[Cursor_Position]);  //�ı����ʾһ��		
          SysTick_Delay_Ms(5);					
			  }
			
			  else if(Display_Interface_Mode == Display_Interface_Mode_Main_Interface_P_set)        //�������ù���ģʽ
			  {
          Power_Carry();	           //��λ
					
          Write_Single_12x24AsicII(42, Cursor_Position*3, String_Power[Cursor_Position]);  //�ı����ʾһ��
          SysTick_Delay_Ms(5);				
			  }
				
				else if((Display_Interface_Mode == Display_Interface_Mode_Recall_Interface) || (Display_Interface_Mode == Display_Interface_Mode_Save_Interface))   //���ڴ�ȡģʽ
		    {
		     	Recall_Save_Number ++;
		    	if(Recall_Save_Number == 10)
		  	  {
		    		Recall_Save_Number =0;
		  	  }
		  	  Recall_Save_Display_Voltage = Recall_Save_Voltage[Recall_Save_Number];
		    	Recall_Save_Display_Current = Recall_Save_Current[Recall_Save_Number];
			    Recall_Save_Display_Power = Recall_Save_Power[Recall_Save_Number];
					if(Display_Interface_Mode == Display_Interface_Mode_Recall_Interface)
					{
						Display_Access_Interface(Recall, Recall_Save_Number,Recall_Save_Display_Power, Recall_Save_Display_Voltage, Recall_Save_Display_Current);
					}
					else if(Display_Interface_Mode == Display_Interface_Mode_Save_Interface)
					{
						Display_Access_Interface(Save, Recall_Save_Number,Recall_Save_Display_Power, Recall_Save_Display_Voltage, Recall_Save_Display_Current);
					}
					SysTick_Delay_Ms(20);
		    }
				
/*-------------------------------------------------------RS232----------------------------------------------------------------*/				
				
				else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == RS232) && (RS232_Set_ID_BaudRate == RS232_Set_ID))    //����RS232��ID
				{
					String_RS232_ID[Cursor_Position] = String_RS232_ID[Cursor_Position] + 1;
					
					Usart_Carry(String_RS232_ID);
					
					Write_Single_16x32AsicII(0, (12 + Cursor_Position * 4), String_RS232_ID[Cursor_Position]);
					SysTick_Delay_Ms(5);
				}
				
				else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == RS232) && (RS232_Set_ID_BaudRate == RS232_Set_BaudRate))    //����RS232�Ĳ�����
	    	{
			
			    RS232_BaudRate_State ++;
			
			    switch (RS232_BaudRate_State)                                                                    //��ʾ�趨ֵ
		    	{
						case (Usart300 - 1) : Write_String_16x32AsicII(32, 40, "115200");SysTick_Delay_Ms(5);RS232_BaudRate_State = Usart115200;break;
			    	case Usart300 : Write_String_16x32AsicII(32, 40, "000300");SysTick_Delay_Ms(5);break;
            case Usart600 : Write_String_16x32AsicII(32, 40, "000600");SysTick_Delay_Ms(5);break;
            case Usart1200 : Write_String_16x32AsicII(32, 40, "001200");SysTick_Delay_Ms(5);break;
            case Usart2400 : Write_String_16x32AsicII(32, 40, "002400");SysTick_Delay_Ms(5);break;
            case Usart4800 : Write_String_16x32AsicII(32, 40, "004800");SysTick_Delay_Ms(5);break;
            case Usart9600 : Write_String_16x32AsicII(32, 40, "009600");SysTick_Delay_Ms(5);break;
            case Usart19200 : Write_String_16x32AsicII(32, 40, "019200");SysTick_Delay_Ms(5);break;
            case Usart38400 : Write_String_16x32AsicII(32, 40, "038400");SysTick_Delay_Ms(5);break;
            case Usart43000 : Write_String_16x32AsicII(32, 40, "043000");SysTick_Delay_Ms(5);break;
            case Usart56000 : Write_String_16x32AsicII(32, 40, "056000");SysTick_Delay_Ms(5);break;
            case Usart57600 : Write_String_16x32AsicII(32, 40, "057600");SysTick_Delay_Ms(5);break;
            case Usart115200 : Write_String_16x32AsicII(32, 40, "115200");SysTick_Delay_Ms(5);break;
						case (Usart115200 + 1) : Write_String_16x32AsicII(32, 40, "000300");SysTick_Delay_Ms(5);RS232_BaudRate_State = Usart300;break;
		  			default: break;
		    	}
	    	}
				
/*-------------------------------------------------------------CAN---------------------------------------------*/				
				
				else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == CAN) && (CAN_Set_ID_BaudRate == CAN_Set_ID))    //����CAN��ID
				{
					String_CAN_ID[Cursor_Position] = String_CAN_ID[Cursor_Position] + 1;
					
					Usart_Carry(String_CAN_ID);
					
					Write_Single_16x32AsicII(0, (12 + Cursor_Position * 4), String_CAN_ID[Cursor_Position]);
					SysTick_Delay_Ms(5);
				}
				
				else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == CAN) && (CAN_Set_ID_BaudRate == CAN_Set_BaudRate))    //����CAN�Ĳ�����
	    	{
			
			    CAN_BaudRate_State ++;
			
			    switch (CAN_BaudRate_State)                                                                    //��ʾ�趨ֵ
		    	{
				    case (CAN5 - 1) : Write_String_16x32AsicII(32, 40, "800");SysTick_Delay_Ms(10);CAN_BaudRate_State = CAN800;break;
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
			  	  case (CAN800 + 1) : Write_String_16x32AsicII(32, 40, "005");SysTick_Delay_Ms(10);CAN_BaudRate_State = CAN5;break;
		  			default: break;
		    	}
	    	}

/*------------------------------------------------------------Cycle--------------------------------------------------------*/
				
		    else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Cycle))  //�����ڵڶ����˵���Cycle
		    {
          if (Cycle_Enter == Cycle_Enter_1)
					{
				   	String_Cycle_On[Cursor_Position] ++;
			 		
			   		Cycle_Carry(String_Cycle_On);
					
			  		Write_String_16x32AsicII(32, 12, String_Cycle_On);
				  	SysTick_Delay_Ms(5);						
					}	
					
					
					else if (Cycle_Enter == Cycle_Enter_2)
					{
				   	String_Cycle_Close[Cursor_Position] ++;
			 		
			   		Cycle_Carry(String_Cycle_Close);
					
			  		Write_String_16x32AsicII(32, 12, String_Cycle_Close);
				  	SysTick_Delay_Ms(5);							
					}
		    }
				
/*------------------------------------------------------------V_Rise--------------------------------------------------------*/
				
		    else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == V_Rise))  //�����ڵڶ����˵���V_Rise
		    {
          if (V_Rise_Enter == V_Rise_Enter_1)
					{
				   	String_V_Rise[Cursor_Position] ++;
			 		
			   		Slope_Carry(String_V_Rise);
					
			  		Write_String_16x32AsicII(32, 12, String_V_Rise);
						
				  	SysTick_Delay_Ms(5);						
					}	
		    }				

/*------------------------------------------------------------V_Fall--------------------------------------------------------*/
				
		    else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == V_Fall))  //�����ڵڶ����˵���V_Fall
		    {
          if (V_Fall_Enter == V_Fall_Enter_1)
					{
				   	String_V_Fall[Cursor_Position] ++;
			 		
			   		Slope_Carry(String_V_Fall);
					
			  		Write_String_16x32AsicII(32, 12, String_V_Fall);
						
				  	SysTick_Delay_Ms(5);						
					}	
		    }

/*------------------------------------------------------------I_Rise--------------------------------------------------------*/
				
		    else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == I_Rise))  //�����ڵڶ����˵���I_Rise
		    {
          if (I_Rise_Enter == I_Rise_Enter_1)
					{
				   	String_I_Rise[Cursor_Position] ++;
			 		
			   		Slope_Carry(String_I_Rise);
					
			  		Write_String_16x32AsicII(32, 12, String_I_Rise);
						
				  	SysTick_Delay_Ms(5);						
					}	
		    }

/*------------------------------------------------------------I_Fall--------------------------------------------------------*/
				
		    else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == I_Fall))  //�����ڵڶ����˵���I_Fall
		    {
          if (I_Fall_Enter == I_Fall_Enter_1)
					{
				   	String_I_Fall[Cursor_Position] ++;
			 		
			   		Slope_Carry(String_I_Fall);
					
			  		Write_String_16x32AsicII(32, 12, String_I_Fall);
						
				  	SysTick_Delay_Ms(5);						
					}	
		    }				
				
				
				
			
/*--------------------------------------------------Delay------------------------------------------*/
			
    		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Delay))  //�����ڵڶ����˵���Delay
		    {
			    if (Delay_Enter == Delay_Enter_1)    
			    {
						String_Delay[Cursor_Position] ++;
			 		
						Delay_Carry(String_Delay);
					
						Write_String_16x32AsicII(32, 14, String_Delay);
						SysTick_Delay_Ms(5);				
					}
		    }

/*-------------------------------------------------------------------�˵�---------------------------------------------------*/

				else if(Display_Interface_Mode == Display_Interface_Mode_First_Menu)   //���ڵ�һ��ѡ����棬����ʾ���ݶ�Ӧ�ı�
				{
					First_Selection = First_Selection + 1;
					if(First_Selection == 6)
					{
						First_Selection = 0;
					}
					Display_Menu_Function_Interface_First_Menu(First_Selection);
//					SysTick_Delay_Ms(30);
				}
		
				else if(Display_Interface_Mode == Display_Interface_Mode_Second_Menu)   //���ڵڶ����˵����棬����ʾ���ݶ�Ӧ�ı�
				{
					switch(Second_Selection)
					{
						case Delay: Second_Selection = Cycle;Display_Menu_Function_Interface_Second_Menu(First_Selection, Second_Selection);break;
						case Cycle: Second_Selection = Delay;Display_Menu_Function_Interface_Second_Menu(First_Selection, Second_Selection);break;
						case V_Rise: Second_Selection = V_Fall;Display_Menu_Function_Interface_Second_Menu(First_Selection, Second_Selection);break;
						case V_Fall: Second_Selection = I_Rise;Display_Menu_Function_Interface_Second_Menu(First_Selection, Second_Selection);break;
						case I_Rise: Second_Selection = I_Fall;Display_Menu_Function_Interface_Second_Menu(First_Selection, Second_Selection);break;
						case I_Fall: Second_Selection = V_Rise;Display_Menu_Function_Interface_Second_Menu(First_Selection, Second_Selection);break;
						case RS232: Second_Selection = CAN;Display_Menu_Function_Interface_Second_Menu(First_Selection, Second_Selection);break;
						case CAN: Second_Selection = RS232;Display_Menu_Function_Interface_Second_Menu(First_Selection, Second_Selection);break;
						case Menu_Vint: Second_Selection = Menu_Vext; Display_Menu_Function_Interface_Second_Menu(First_Selection, Second_Selection);break;
						case Menu_Vext: Second_Selection = Menu_Vint; Display_Menu_Function_Interface_Second_Menu(First_Selection, Second_Selection);break;
						default: break;
					}
					SysTick_Delay_Ms(5);
				}
	    }
			
			
			
/*-------------------------------------------------------------------��ת-----------------------------------------------------------------------------*/
			
	    else if ((State == Reverse) && (Last_State == State))    //��ʾ��ת��ת����ϴ�һ��
	    {
		    if(Display_Interface_Mode == Display_Interface_Mode_Main_Interface_V_set)             //�������õ�ѹģʽ
			  {
				  String_Voltage[Cursor_Position] = String_Voltage[Cursor_Position]-1;                //��תһ�Σ���Ӧ���ַ������ּ�1

					Voltage_Abdication();                                                             //���С��0�ˣ��ͱ��9
				  
				  Write_Single_16x32AsicII(0, Cursor_Position * 4, String_Voltage[Cursor_Position]);  //�ı����ʾһ��	
          SysTick_Delay_Ms(5);					
			  }
			
		    else if(Display_Interface_Mode == Display_Interface_Mode_Main_Interface_I_set)        //�������õ�ѹģʽ
			  {
				  String_Current[Cursor_Position] = String_Current[Cursor_Position]-1;                //��תһ�Σ���Ӧ���ַ������ּ�1
					
          Current_Abdication();           //��λ
					
				  Write_Single_16x32AsicII(0, Cursor_Position*4 + 36, String_Current[Cursor_Position]);  //�ı����ʾһ��
					SysTick_Delay_Ms(5);
			  }

		    else if(Display_Interface_Mode == Display_Interface_Mode_Main_Interface_P_set)        //�������ù���ģʽ
			  {
          Power_Abdication();   //��λ
					
				  Write_Single_12x24AsicII(42, Cursor_Position*3, String_Power[Cursor_Position]);  //�ı����ʾһ��
					SysTick_Delay_Ms(5);
			  }		

        else if((Display_Interface_Mode == Display_Interface_Mode_Recall_Interface) || (Display_Interface_Mode == Display_Interface_Mode_Save_Interface))   //���ڴ�ȡģʽ
		    {
		    	Recall_Save_Number --;
	    		if(Recall_Save_Number == -1)
		     	{
		    		Recall_Save_Number =9;
		    	}
					
		    	Recall_Save_Display_Voltage = Recall_Save_Voltage[Recall_Save_Number];
		    	Recall_Save_Display_Current = Recall_Save_Current[Recall_Save_Number];
		    	Recall_Save_Display_Power = Recall_Save_Power[Recall_Save_Number];
					
					if(Display_Interface_Mode == Display_Interface_Mode_Recall_Interface)
					{
						Display_Access_Interface(Recall, Recall_Save_Number,Recall_Save_Display_Power, Recall_Save_Display_Voltage, Recall_Save_Display_Current);
					}
					else if(Display_Interface_Mode == Display_Interface_Mode_Save_Interface)
					{
						Display_Access_Interface(Save, Recall_Save_Number,Recall_Save_Display_Power, Recall_Save_Display_Voltage, Recall_Save_Display_Current);
					}
					SysTick_Delay_Ms(20);
		    }
				
		/*-----------------------------------------------RS232-----------------------------------------------------*/		
				
				else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == RS232) && (RS232_Set_ID_BaudRate == RS232_Set_ID))    //����RS232��ID
				{
					String_RS232_ID[Cursor_Position] = String_RS232_ID[Cursor_Position] - 1;
					
					Usart_Abdication(String_RS232_ID);
					
					Write_Single_16x32AsicII(0, (12 + Cursor_Position * 4), String_RS232_ID[Cursor_Position]);
					SysTick_Delay_Ms(5);
				}
				
				else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == RS232) && (RS232_Set_ID_BaudRate == RS232_Set_BaudRate))    //����RS232�Ĳ�����
	    	{
			
			    RS232_BaudRate_State --;
			
			    switch (RS232_BaudRate_State)                                                                    //��ʾ�趨ֵ
		    	{
						case (Usart300 - 1) : Write_String_16x32AsicII(32, 40, "115200");SysTick_Delay_Ms(5);RS232_BaudRate_State = Usart115200;break;
			    	case Usart300 : Write_String_16x32AsicII(32, 40, "000300");SysTick_Delay_Ms(5);break;
            case Usart600 : Write_String_16x32AsicII(32, 40, "000600");SysTick_Delay_Ms(5);break;
            case Usart1200 : Write_String_16x32AsicII(32, 40, "001200");SysTick_Delay_Ms(5);break;
            case Usart2400 : Write_String_16x32AsicII(32, 40, "002400");SysTick_Delay_Ms(5);break;
            case Usart4800 : Write_String_16x32AsicII(32, 40, "004800");SysTick_Delay_Ms(5);break;
            case Usart9600 : Write_String_16x32AsicII(32, 40, "009600");SysTick_Delay_Ms(5);break;
            case Usart19200 : Write_String_16x32AsicII(32, 40, "019200");SysTick_Delay_Ms(5);break;
            case Usart38400 : Write_String_16x32AsicII(32, 40, "038400");SysTick_Delay_Ms(5);break;
            case Usart43000 : Write_String_16x32AsicII(32, 40, "043000");SysTick_Delay_Ms(5);break;
            case Usart56000 : Write_String_16x32AsicII(32, 40, "056000");SysTick_Delay_Ms(5);break;
            case Usart57600 : Write_String_16x32AsicII(32, 40, "057600");SysTick_Delay_Ms(5);break;
            case Usart115200 : Write_String_16x32AsicII(32, 40, "115200");SysTick_Delay_Ms(5);break;
						case (Usart115200 + 1) : Write_String_16x32AsicII(32, 40, "000300");SysTick_Delay_Ms(5);RS232_BaudRate_State = Usart300;break;
		  			default: break;
		    	}
	    	}
				
				/*------------------------------------------------CAN-------------------------------------------------------*/
				
				else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == CAN) && (CAN_Set_ID_BaudRate == CAN_Set_ID))    //����CAN��ID
				{
					String_CAN_ID[Cursor_Position] = String_CAN_ID[Cursor_Position] - 1;
					
					Usart_Abdication(String_CAN_ID);
					
					Write_Single_16x32AsicII(0, (12 + Cursor_Position * 4), String_CAN_ID[Cursor_Position]);
					SysTick_Delay_Ms(5);
				}
				
				else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == CAN) && (CAN_Set_ID_BaudRate == CAN_Set_BaudRate))    //����CAN�Ĳ�����
	    	{
			
			    CAN_BaudRate_State --;
			
			    switch (CAN_BaudRate_State)                                                                    //��ʾ�趨ֵ
		    	{
				    case (CAN5 - 1) : Write_String_16x32AsicII(32, 40, "800");SysTick_Delay_Ms(10);CAN_BaudRate_State = CAN800;break;
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
			  	  case (CAN800 + 1) : Write_String_16x32AsicII(32, 40, "005");SysTick_Delay_Ms(10);CAN_BaudRate_State = CAN5;break;
				  	default: break;
		    	}
	    	}

/*-----------------------------------------------------------------------CYCLE--------------------------------------------------------------*/				
		    else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Cycle))  //�����ڵڶ����˵���Cycle
		    {
          if (Cycle_Enter == Cycle_Enter_1)
					{
				   	String_Cycle_On[Cursor_Position] --;
			 		
			   		Cycle_Abdication(String_Cycle_On);
					
			  		Write_String_16x32AsicII(32, 12, String_Cycle_On);
				  	SysTick_Delay_Ms(5);						
					}	
					else if (Cycle_Enter == Cycle_Enter_2)
					{
				   	String_Cycle_Close[Cursor_Position] --;
			 		
			   		Cycle_Abdication(String_Cycle_Close);
					
			  		Write_String_16x32AsicII(32, 12, String_Cycle_Close);
				  	SysTick_Delay_Ms(5);							
					}
		    }
				
/*-----------------------------------------------------------------------V_Rise--------------------------------------------------------------*/				
		    else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == V_Rise))  //�����ڵڶ����˵���V_Rise
		    {
          if (V_Rise_Enter == V_Rise_Enter_1)
					{
				   	String_V_Rise[Cursor_Position] --;
			 		
			   		Slope_Abdication(String_V_Rise);
					
			  		Write_String_16x32AsicII(32, 12, String_V_Rise);
				  	SysTick_Delay_Ms(5);						
					}	
		    }		

/*-----------------------------------------------------------------------V_Fall--------------------------------------------------------------*/				
		    else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == V_Fall))  //�����ڵڶ����˵���V_Fall
		    {
          if (V_Fall_Enter == V_Fall_Enter_1)
					{
				   	String_V_Fall[Cursor_Position] --;
			 		
			   		Slope_Abdication(String_V_Fall);
					
			  		Write_String_16x32AsicII(32, 12, String_V_Fall);
				  	SysTick_Delay_Ms(5);						
					}	
		    }		
				
/*-----------------------------------------------------------------------I_Rise--------------------------------------------------------------*/				
		    else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == I_Rise))  //�����ڵڶ����˵���I_Rise
		    {
          if (I_Rise_Enter == I_Rise_Enter_1)
					{
				   	String_I_Rise[Cursor_Position] --;
			 		
			   		Slope_Abdication(String_I_Rise);
					
			  		Write_String_16x32AsicII(32, 12, String_I_Rise);
				  	SysTick_Delay_Ms(5);						
					}	
		    }		

/*-----------------------------------------------------------------------I_Fall--------------------------------------------------------------*/				
		    else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == I_Fall))  //�����ڵڶ����˵���I_Fall
		    {
          if (I_Fall_Enter == I_Fall_Enter_1)
					{
				   	String_I_Fall[Cursor_Position] --;
			 		
			   		Slope_Abdication(String_I_Fall);
					
			  		Write_String_16x32AsicII(32, 12, String_I_Fall);
				  	SysTick_Delay_Ms(5);						
					}	
		    }						

				

/*--------------------------------------------------Delay------------------------------------------*/
			
    		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Delay))  //�����ڵڶ����˵���Delay
		    {
					if (Delay_Enter == Delay_Enter_1)    
					{
						String_Delay[Cursor_Position] --;
			 		
						Delay_Abdication(String_Delay);
					
						Write_String_16x32AsicII(32, 14, String_Delay);
						
						SysTick_Delay_Ms(5);				
					}
		    }
				

/*---------------------------------------------------------�˵�------------------------------------------------*/
				else if(Display_Interface_Mode == Display_Interface_Mode_First_Menu)   //���ڵ�һ��ѡ����棬����ʾ���ݶ�Ӧ�ı�
				{
					First_Selection = First_Selection - 1;
					if(First_Selection == -1)
					{
						First_Selection = 5;
					}
					Display_Menu_Function_Interface_First_Menu(First_Selection);
//				  SysTick_Delay_Ms(30);
				}
		
				else if(Display_Interface_Mode == Display_Interface_Mode_Second_Menu)   //���ڵڶ����˵����棬����ʾ���ݶ�Ӧ�ı�
				{
					switch(Second_Selection)
					{
						case Delay: Second_Selection = Cycle;Display_Menu_Function_Interface_Second_Menu(First_Selection, Second_Selection);break;
						case Cycle: Second_Selection = Delay;Display_Menu_Function_Interface_Second_Menu(First_Selection, Second_Selection);break;
						case I_Fall: Second_Selection = I_Rise;Display_Menu_Function_Interface_Second_Menu(First_Selection, Second_Selection);break;
						case I_Rise: Second_Selection = V_Fall;Display_Menu_Function_Interface_Second_Menu(First_Selection, Second_Selection);break;
						case V_Fall: Second_Selection = V_Rise;Display_Menu_Function_Interface_Second_Menu(First_Selection, Second_Selection);break;
						case V_Rise: Second_Selection = I_Fall;Display_Menu_Function_Interface_Second_Menu(First_Selection, Second_Selection);break;
						case RS232: Second_Selection = CAN;Display_Menu_Function_Interface_Second_Menu(First_Selection, Second_Selection);break;
						case CAN: Second_Selection = RS232;Display_Menu_Function_Interface_Second_Menu(First_Selection, Second_Selection);break;
						case Menu_Vint: Second_Selection = Menu_Vext; Display_Menu_Function_Interface_Second_Menu(First_Selection, Second_Selection);break;
						case Menu_Vext: Second_Selection = Menu_Vint; Display_Menu_Function_Interface_Second_Menu(First_Selection, Second_Selection);break;
						default: break;
					}
				}			
				SysTick_Delay_Ms(5);
	    }
			else if (State != Last_State)       //���Ŀǰת����ϴβ�ͬ�����ϴ�ת���ΪĿǰת��
			{
				Last_State = State;
			}
		}
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);  //����жϱ�־λ
	}
}







