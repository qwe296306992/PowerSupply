#include "tim_delay.h"

u32 TIM_Delay_time;

u8 Delay_Function_On_Off = Delay_Function_On;            //DELAY�����Ƿ�����־




//�����ж����ȼ�
static void TIM_DELAY_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

//����TIM6,��ʱ����Ϊ1S
static void TIM_DELAY_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		
	  // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
		// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period=9999;
	  // ʱ��Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_Prescaler= 7199;		
		// ʱ�ӷ�Ƶ���� 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		// ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	
		// �ظ���������ֵ
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	  // ��ʼ����ʱ��
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
	
		// ����������жϱ�־λ
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	  
		// �����������ж�
    TIM_ITConfig(TIM6, TIM_IT_Update,ENABLE);
		
		// ʹ�ܼ�����
    TIM_Cmd(TIM6, DISABLE);
}

//TIM6��ʼ��
void TIM_DELAY_Init(void)
{
	TIM_DELAY_NVIC_Config();
	TIM_DELAY_Mode_Config();		
}

/**
  * @brief  ����DELAY���ܣ����ö�ʱ��ʱ���ʹ�ܶ�ʱ��
  * @param  None
  * @retval None
  */
void TIM_DELAY_ON(void)
{
	TIM_Delay_time = Delay_h * 3600 + Delay_m * 60 + Delay_s;
	DELAY_NOT_DELAY = DELAY;
	ON_OFF = OFF;
	TIM_Cmd(TIM6, ENABLE);	
}

/**
  * @brief  �ر�DELAY����
  * @param  None
  * @retval None
  */
void TIM_DELAY_OFF(void)
{
	TIM_Cmd(TIM6, DISABLE);
	DELAY_NOT_DELAY = NOT_DELAY;
}


/**
  * @brief  TIM6���жϷ�����,����DELAY
  * @param  None
  * @retval None
  */
void TIM_DELAY_IT(void)
{
	//��ʱ
	if(TIM_Delay_time > 0)
	{
		TIM_Delay_time --;
	}
	else  //��ʱ����
	{
    TIM_DELAY_OFF();
		
		if(Cycle_Function_On_Off == Cycle_Function_On)    //����ѭ�����ܣ�����ѭ������
		{
			TIM_CYCLE_ON();
		}
		else if(Cycle_Function_On_Off == Cycle_Function_Off)   //û��ѭ�����ܣ�ֱ�����
		{
			Power_On();
		}
	}
	TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);   //����жϱ�־λ	
}







