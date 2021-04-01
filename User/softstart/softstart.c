#include "softstart.h"
#include "protect.h"
#include "keys.h"
#include "SCR_Controller.h"

////�����ж����ȼ�
//static void TIM_DELAY3_NVIC_Config(void)
//{
//    NVIC_InitTypeDef NVIC_InitStructure; 
//    // �����ж���Ϊ0
////    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);		
//		// �����ж���Դ
//    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;	
//		// ���������ȼ�Ϊ 0
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	 
//	  // ������ռ���ȼ�Ϊ3
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//}

void SoftStart_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //ʱ��ʹ��
	Power_AC_Off();
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //OUT_1  PB9
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
//	while(CRL_Voltage < 29*8);
//	Power_AC_On();
//	SysTick_Delay_Ms(100);//�ȴ��̵����������ȶ���ͨ
	SCR_GPIO_Config();//ʹ�ܰ��������ͨ����������������ݳ��
	SysTick_Delay_Ms(200);//
}

void SS_DISADBE_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //OUT_2  PE0
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOE, &GPIO_InitStructure);	
	

	
			SS_DISADBE_On();   
//			SS_DISADBE_Off();   

	
}





void Power_AC_Off(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
}

void Power_AC_On(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}


void SS_DISADBE_On(void)       //  SS�Žӵأ�2895������   OUT_2��   off
{
	GPIO_SetBits(GPIOE, GPIO_Pin_0); 
	
}

void SS_DISADBE_Off(void)       //  SS�Ų��ӵأ�2895����   OUT_2��    on
{
	
	GPIO_ResetBits(GPIOE, GPIO_Pin_0);
	
}
//void OUT2_1(void)
//{
//	
//GPIO_SetBits(GPIOE, GPIO_Pin_0);
//}
//void OUT2_0(void)
//{
//	GPIO_ResetBits(GPIOE, GPIO_Pin_0);
//}

//void TIM1_SoftStatr_IT(void)
//{
//	SoftStart_Off();
//	TIM_ClearFlag(TIM1, TIM_FLAG_Update);                     //���жϱ�־λ
//	
//  TIM_Cmd(TIM1, DISABLE);	
//		SoftStart_Off();
//}









