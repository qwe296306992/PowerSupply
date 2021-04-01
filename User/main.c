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
#include "dac.h"
#include "softstart.h"
#include "software.h"
#include "SCR_Controller.h"


int main(void)
{

//////////////////////////////////////////////////////////////////////	//������
//////////////////////////////////////////////////////////////////////	FLASH_Unlock();
//////////////////////////////////////////////////////////////////////	FLASH_ReadOutProtection(ENABLE);
	SoftStart_Init();//���������ų�ʼ��
	TIM_PROTECT_Init();//���ñ������������ѹ�������ʱ�����ֵ����������ʱҲ������
	//�ɿع��ʼ��
	SCR_Controller_Init();
	//���ֳ�ʼ��
	SysTick_Delay_Ms(10);
	SPI_FUN();
	OLED_Init();
  Keys_Init();
  Encode_Init();//�жϷ����Ѿ��ڴ˳�ʼ��һ��	
	
  TIM_OLED_Init();
	TIM_DELAY_Init();
	TIM_CYCLE_Init();
	
//	while(1);  		û©
	
//	TIM_PROTECT_Init();//���ñ������������ѹ�������ʱ�����ֵ����������ʱҲ������
	TIM_SLOPE_Init();//
//	while(1);			û©
  SPI1_DAC_Config();		
//	CP_Slope_Init();//�㹦��CPģʽ�µĶ�ʱ����ʼ������
//	SoftStart_Init();//������Ӧ�÷�����ǰ��
//	while(1);     ©
	SS_DISADBE_Init();
	
	TestCurrent_GPIO_Config();//OC1�����������
	
	//��鿪���Ƿ������
	Check_T_I_Switch();

	//����AT25
	SPI_AT25_Unlock();
	
	//��AT25��������
	AT25_Load_Data();
	
  //��ʼ��ͨ��
	CAN_Config(CAN_BaudRate);
	Usart2_Config(RS232_BaudRate);
	
		//ADC��ʼ��
	SPI2_ADC_config();

//0.053s(53ms)һ��whileѭ��
	while(1)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_12);//����CS��ƽ
		SysTick_Delay_Ms(1);		
		Display_Interface(Display_Interface_Mode);
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);//�������CS��ƽ��ѡ��оƬ
		SysTick_Delay_Ms(1);
		Keys_Handler(keys_EN);//�������ڼ�DISABLE����
		Computer_Control_Device();
		TestCurrent();//�����������	
		
//		if( (TIM3->CR1 & TIM_CR1_CEN) == 0 && ((EXTI->IMR & EXTI_Line9) == 0))  //��������
//		{
//				EXTI->IMR |= EXTI_Line9;  //�����ж�
//		}
		
//		DISADBE_On_Or_Off();
	}

}



