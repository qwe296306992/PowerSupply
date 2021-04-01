#include "tim_cycle.h"

u8 Cycle_Function_On_Off = Cycle_Function_Off;

u32 Cycle_On_Time;
u32 Cycle_Close_Time;

//�����ж����ȼ�
static void TIM_CYCLE_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

//����TIM7
static void TIM_CYCLE_Mode_Config(void)//1ms�ж�
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		
	  // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
		// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period=7199;
	  // ʱ��Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_Prescaler= 9;		
		// ʱ�ӷ�Ƶ���� 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		// ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	
		// �ظ���������ֵ
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	  // ��ʼ����ʱ��
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
	
		// ����������жϱ�־λ
    TIM_ClearFlag(TIM7, TIM_FLAG_Update);
	  
		// �����������ж�
    TIM_ITConfig(TIM7, TIM_IT_Update,ENABLE);
		
		// ʹ�ܼ�����
    TIM_Cmd(TIM7, DISABLE);
}

//TIM7��ʼ��
void TIM_CYCLE_Init(void)
{
	TIM_CYCLE_NVIC_Config();
	TIM_CYCLE_Mode_Config();		
}

/**
  * @brief  ��CYCLE����TIM7��ʱ��
  * @param  None
  * @retval None
  */
void TIM_CYCLE_ON(void)
{
	Cycle_On_Time = Cycle_On_s * 1000 + Cycle_On_ms;
	Cycle_Close_Time = Cycle_Close_s * 1000 + Cycle_Close_ms;
	ON_OFF = ON;
	Power_On();
	TIM_Cmd(TIM7, ENABLE);
}

/**
  * @brief  �ر�CYCLE���ر�TIM7��ʱ��
  * @param  None
  * @retval None
  */
void TIM_CYCLE_OFF(void)
{
	TIM_Cmd(TIM7, DISABLE);	
	ON_OFF = OFF;
}




/**
  * @brief  TIM7���жϷ�����,����DELAY
  * @param  None
  * @retval None
  */
void TIM_CYCLE_IT(void)
{
	if(ON_OFF == ON)    //����״̬��ʱ�䵹��
	{
		if(Cycle_On_Time > 0)
	  {
		  Cycle_On_Time --;
	  }
		else              //��ʱ���ˣ������
		{
			Cycle_On_Time = Cycle_On_s * 1000 + Cycle_On_ms;
			Power_Off();
		}
	}
	else if(ON_OFF == OFF)
	{
		if(Cycle_Close_Time >0)
		{
			Cycle_Close_Time --;
		}
		else              //��ʱ���ˣ������
		{
			Cycle_Close_Time = Cycle_Close_s * 1000 + Cycle_Close_ms;
			Power_On();
		}
	}
	TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);   //����жϱ�־λ		
}














