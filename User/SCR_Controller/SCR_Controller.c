#include "SCR_Controller.h"
//#include "systick.h"

//��ʼ
//uint16_t SCR_shift_T = 7800;	//7800�õ�ѹ�������143
uint16_t SCR_shift_T = 5000;
uint16_t temp;
u16 t_count = 70;//���ڼ���@1us
FunctionalState SCR_shift = ENABLE;//���л�״̬��־
FunctionalState SCR_ON = DISABLE;//���л������־,ȫ�ص����
FunctionalState SCR_OFF = ENABLE;//���л������־����ص�ȫ��

FunctionalState SCR_P_FLAG = DISABLE; //

//u16 t = 7800;

//FunctionalState SCR_shift = DISABLE;//���л�״̬��־
//FunctionalState SCR_ON = DISABLE;//���л������־,ȫ�ص����
//FunctionalState SCR_OFF = ENABLE;//���л������־����ص�ȫ��

//�����ж����ȼ� 
static void ZERO_CROSS_DETECT_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(TRIGGERCLC | RCC_APB2Periph_AFIO, ENABLE);
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
	//�ж�Դ
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	//��ռ���ȼ�
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;

	//ʹ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


//�����ⲿ�ж�
static void ZERO_CROSS_DETECT_EXTI_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(TRIGGERCLC | RCC_APB2Periph_AFIO, ENABLE);
	
	//�ն�GPIO��ʼ��
	GPIO_InitStructure.GPIO_Pin  = TRIGGERPIN;    //PC9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
 	GPIO_Init(TRIGGERPORT, &GPIO_InitStructure);
	
	//�ж�����
	GPIO_EXTILineConfig(TRIGGERSOURCE, TRIGGERSOURCEPIN);
	EXTI_InitStructure.EXTI_Line = TRIGGEREXITPIN;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ���ж�
	EXTI_Init(&EXTI_InitStructure);
}

//�ɿع败�����ų�ʼ��
void SCR_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(PWM0CLC, ENABLE);
	RCC_APB2PeriphClockCmd(PWM1CLC, ENABLE);
	//������ų�ʼ��
	GPIO_InitStructure.GPIO_Pin = PWM0PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(PWM0, &GPIO_InitStructure);
	
	//�������ų�ʼ��
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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	
		//TIM3���ж����ȼ�����
	// �����ж���Ϊ2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
	// �����ж���Դ
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	

	// ���������ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	// ���������ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = 19500;                  //�����Զ���װ�ؼĴ�������ֵ//50Hz
  TIM_TimeBaseStructure.TIM_Prescaler = (72-1);              //����Ԥ��Ƶֵ  ͨ�����ٷ�Ƶ��������ֵ���������
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;   //����ʱ�ӷָ�
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;          //�ظ���������
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);           //������ʼ��
//	PWM0_1 = 9500;	//PWM0 	��
//	PWM0_0 = 10700;	//PWM0 	��
//	PWM1_1 = 19500;	//PWM1	��
//	PWM1_0 = 19600;	//PWM1	��
	PWM0_1 = 7800;	//PWM0 	��
	PWM0_0 = 9000;	//PWM0 	��
	PWM1_1 = 17800;	//PWM1	��
	PWM1_0 = 19000;	//PWM1	��	

	// �����������ж�
  TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 | TIM_IT_Update,ENABLE);
	
	TIM_ClearFlag(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 | TIM_IT_Update);   
		
	// ��ʹ�ܼ�����
	TIM_Cmd(TIM3, DISABLE);//�ɹ����ⰴ����ȷʱ���
	TIM_ARRPreloadConfig(TIM3, ENABLE);
}


//�ⲿ�ж�ΪPC9����������PC8��PC7��TIM3��������Ϊ1us��TIM3�ж�Ƶ���ɹ����ж�Ƶ�ʾ��������Ϊ50Hz
void SCR_Controller_Init(void)
{
	PWM0_LOW;
	PWM1_LOW;	
//	SysTick_Delay_Ms(41);
//	Power_AC_On();//�ȸ����ݳ���ٿ��̵���
	
	ZERO_CROSS_DETECT_NVIC_Config();
	ZERO_CROSS_DETECT_EXTI_Init();
	SCR_TIM1_Init();
	RTC_Init();
//	SCR_GPIO_Config();//��������������
//	WWDG_Init(41,32,WWDG_Prescaler_8);//������ֵΪ0x29,���ڼĴ���Ϊ0x20,��Ƶ��Ϊ8	 
}


/**
  * @brief  EXTI9��������жϷ�����
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
			if( (TIM3->CR1 & TIM_CR1_CEN) == 0)  //ʹ���źţ�0����ʱ���أ�1����
			{
				TIM3->CNT = 0;
				TIM_Cmd(TIM3, ENABLE); //�˴������и���Ӱ�죬��ʱ��û�п������ж������ˣ����°��ʧЧ	
//				while((TIM3->CR1 & TIM_CR1_CEN) == 0)TIM_Cmd(TIM3, ENABLE);
				RTC_ITConfig(RTC_IT_SEC, ENABLE);	
				RTC_WaitForLastTask();
				while((RTC->CRH & RTC_IT_SEC) == 0);
				EXTI->IMR &=~EXTI_Line9;  //�����ж�				
			}
////			else TIM_Cmd(TIM3, DISABLE);
//			if( (TIM3->CR1 & TIM_CR1_CEN) == 0) TIM_Cmd(TIM3, ENABLE);
		}
	}
}

FunctionalState WWDG_ON = DISABLE;
/**
  * @brief  TIM3���жϷ�����
  * @param  None
  * @retval None
  */
void SCR_TIM1_IT(void)//�ж�����20ms
{		
	switch(TIM3->SR & (TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4))
	{
		case TIM_IT_CC1:
		{			
			PWM0_HIGH; 
			//������ͨ���Զ�˫�򽥱�
			if(SCR_shift == ENABLE)//���/ȫ�����л�
			{
				u16 PWM_buff = (u16)PWM0_1;
			  if(SCR_OFF == ENABLE)
				{
					if(PWM_buff > SCR_shift_T) //��ͨ����0��ķ���仯
					{
						PWM_buff -=t_count;
						if(PWM_buff < SCR_shift_T) PWM_buff = SCR_shift_T;
					}
					else if(PWM_buff < SCR_shift_T) //��ͨ����0��ķ���仯
					{
						PWM_buff +=t_count;
						if(PWM_buff > SCR_shift_T) PWM_buff = SCR_shift_T;
					}
					else
					{	
						SCR_shift = DISABLE;//���л���ȫ�����
						SCR_OFF = DISABLE;
					}
//					PWM_buff = SCR_shift_T;
				}
				else if(SCR_ON == ENABLE)//�л������
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
					{	 //��ͨ����90��+�ķ���仯
						SCR_shift = DISABLE;//���л���������
						SCR_ON = DISABLE;
					}
//					PWM_buff = SCR_shift_T;
				}
/*					if(SCR_OFF == ENABLE)//�л���ȫ��
					{
						if(PWM_buff > 200) //��ͨ����0��ķ���仯
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
							SCR_shift = DISABLE;//���л���ȫ�����
							SCR_OFF = DISABLE;
						}
					}
					else if(SCR_ON == ENABLE)//�л������
					{
						if(PWM_buff < SCR_shift_T) 
						{
							PWM_buff += t_count;
							if(PWM_buff > SCR_shift_T)PWM_buff = SCR_shift_T;
						}
						else
						{	 //��ͨ����90��+�ķ���仯
							SCR_shift = DISABLE;//���л���������
							SCR_ON = DISABLE;
						}
					}	*/
				PWM0_1 = PWM_buff;
				PWM1_1 = PWM_buff + 10000;//Ĭ��50Hz
				PWM0_0 = PWM_buff + 1200;//����1.2ms
				PWM1_0 = PWM_buff + 11200;//����1.2ms
				temp = PWM0_1;
//				if(PWM_buff > 5500)
//				{
//					PWM0_0 = PWM_buff + 1200;//����1.2ms
//					PWM1_0 = PWM_buff + 11200;//����1.2ms
////					PWM0_0 = 9000;//����1.2ms
////					PWM1_0 = 19000;//����1.2ms
//				}
//				else
//				{
//					PWM1_0 = 16500;//�����̶����½��ع̶�
//					PWM0_0 = 6500;//�����̶����½��ع̶�
//				}
			}
			break;//��0-10ms�ж�
		}
		case TIM_IT_CC2: //��6.5~10ms�ж�
			PWM0_LOW;
//			if(WWDG_ON != ENABLE)
//			{
//				//WWDG_Init(41,34,WWDG_Prescaler_8);//������ֵΪ0x29,���ڼĴ���Ϊ0x20,��Ƶ��Ϊ8	 
//				WWDG_ON = ENABLE;
//			}
//			else WWDG_SetCounter(WWDG_CNT);
			break;
		case TIM_IT_CC3://��10-20ms�ж�
		{	
			PWM1_HIGH; 
			//������ͨ���Զ�˫�򽥱�
			if(SCR_shift == ENABLE)//���/ȫ�����л�
			{
				u16 PWM_buff = (u16)PWM0_1;
			  if(SCR_OFF == ENABLE)
				{
					if(PWM_buff > SCR_shift_T) //��ͨ����0��ķ���仯
					{
						PWM_buff -=t_count;
						if(PWM_buff < SCR_shift_T) PWM_buff = SCR_shift_T;
					}
					else if(PWM_buff < SCR_shift_T) //��ͨ����0��ķ���仯
					{
						PWM_buff +=t_count;
						if(PWM_buff > SCR_shift_T) PWM_buff = SCR_shift_T;
					}
					else
					{	
						SCR_shift = DISABLE;//���л���ȫ�����
						SCR_OFF = DISABLE;
					}
//					PWM_buff = SCR_shift_T;
				}
				else if(SCR_ON == ENABLE)//�л������
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
					{	 //��ͨ����90��+�ķ���仯
						SCR_shift = DISABLE;//���л���������
						SCR_ON = DISABLE;
					}
//					PWM_buff = SCR_shift_T;
				}
/*					if(SCR_OFF == ENABLE)//�л���ȫ��
					{
						if(PWM_buff > 200) //��ͨ����0��ķ���仯
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
							SCR_shift = DISABLE;//���л���ȫ�����
							SCR_OFF = DISABLE;
						}
					}
					else if(SCR_ON == ENABLE)//�л������
					{
						if(PWM_buff < SCR_shift_T) 
						{
							PWM_buff += t_count;
							if(PWM_buff > SCR_shift_T)PWM_buff = SCR_shift_T;
						}
						else
						{	 //��ͨ����90��+�ķ���仯
							SCR_shift = DISABLE;//���л���������
							SCR_ON = DISABLE;
						}
					}	*/
				PWM0_1 = PWM_buff;	
				PWM1_1 = PWM_buff + 10000;//Ĭ��50Hz
				PWM0_0 = PWM_buff + 1200;//����1.2ms
				PWM1_0 = PWM_buff + 11200;//����1.2ms
//				
//				if(PWM_buff > 5500)
//				{
//					PWM0_0 = PWM_buff + 1200;//����1.2ms
//					PWM1_0 = PWM_buff + 11200;//����1.2ms
////					PWM0_0 = 9000;//����1.2ms
////					PWM1_0 = 19000;//����1.2ms
//				}
//				else
//				{
//					PWM0_0 = 6500; //�����̶����½��ع̶�
//					PWM1_0 = 16500;//�����̶����½��ع̶�
//				}
			}
		}
			break;

		case TIM_IT_CC4: //��16.5~19.5ms�ж�
		{
			TIM_Cmd(TIM3, DISABLE);
			PWM1_LOW;
			EXTI->IMR |= EXTI_Line9;  //�����ж�
//			while((EXTI->IMR & EXTI_Line9) == 0)EXTI->IMR |= EXTI_Line9;
			RTC_ITConfig(RTC_IT_SEC, DISABLE);	
//			WWDG_SetCounter(WWDG_CNT);
		} break;
	}
	if(TIM3->SR & TIM_IT_Update)//CCR3���ڶ�ʱ������ʱ(19.5ms),�رն�ʱ����
	{
		TIM_Cmd(TIM3, DISABLE);//
		PWM1_LOW;//
		EXTI->IMR |= EXTI_Line9;  //�����ж�
//		while((EXTI->IMR & EXTI_Line9) == 0)EXTI->IMR |= EXTI_Line9;		
		RTC_ITConfig(RTC_IT_SEC, DISABLE);	
//		WWDG_SetCounter(WWDG_CNT);
	}
	TIM3->SR = 0x0000; 
}


/**
  * @brief  �л���ص�ͨ�ǵĺ���
  * @param  None
  * @retval None
  */
//ENABLE--�뿪
//DISABLE--ȫ��
void SCR_On_Off(FunctionalState NewState)
{
		if (NewState != DISABLE)    //�����
		 {
				SCR_OFF = DISABLE;//ȡ����һ��δ��ɵ�ȫ���л�
				SCR_shift = ENABLE;//ʹ���л�
				SCR_ON = ENABLE;//�����뿪�л�	
		 }
		 else
		 {	 
			 SCR_ON = DISABLE;//ȡ����һ��δ��ɵİ뿪�л�
			 SCR_shift = ENABLE;//ʹ���л�
			 SCR_OFF = ENABLE;//����ȫ���л�
		 }
}	

//����:����
//���ܣ��л����/ȫ��
void SCR_Ser(float P)
{
	uint16_t temp = SCR_shift_T;
	if(ON_OFF == OFF)  return;
//	if(P < 2) SCR_shift = DISABLE;	//����̫С����ѹ�½������ᵼ���´ι�������������

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
		SCR_shift = ENABLE;//ʹ���л�
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
//	static int d_I = 0;//�����仯��
//	static int d_P = 0;//���ʱ仯��
//	static int I_F = 0;//��һ�βɼ��ĵ���
//	static int P_F = 0;//��һ�βɼ��Ĺ���
//	d_I = Current - I_F;
//	I_F = Current;
//	d_P = Power - P_F;
//	P_F = Power;
//	if(Current > Set_Current)   //��С�����ѹ
//	{
//		SCR_shift_T +=10;
//	}
//	else if(d_I < 1)   //���������ѹ
//	{
//		SCR_shift_T -=10;
//	}
//	else return; //
//	if(SCR_shift_T < 0) SCR_shift_T = 0;
//	else if(SCR_shift_T > 10000)  SCR_shift_T = 10000;






//}




