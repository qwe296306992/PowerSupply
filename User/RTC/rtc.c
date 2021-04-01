#include "rtc.h" 		    
//Mini STM32������
//RTCʵʱʱ�� ��������			 
//����ԭ��@ALIENTEK
//2010/6/6
	 
 
static void RTC_NVIC_Config(void)
{	
  NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTCȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռ���ȼ�1λ,�����ȼ�3λ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//��ռ���ȼ�0λ,�����ȼ�4λ
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//ʹ�ܸ�ͨ���ж�
	NVIC_Init(&NVIC_InitStructure);		//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

//ʵʱʱ������
//��ʼ��RTCʱ��,ͬʱ���ʱ���Ƿ�������
//BKP->DR1���ڱ����Ƿ��һ�����õ�����
//����0:����
//����:�������

u8 RTC_Init(void)
{
	//����ǲ��ǵ�һ������ʱ��
//	u8 temp=0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��   
	PWR_BackupAccessCmd(ENABLE);	//ʹ�ܺ󱸼Ĵ�������  
	BKP_DeInit();	//��λ�������� 	
	RCC_LSICmd(ENABLE);
//	RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div128);		//����RTCʱ��(RTCCLK),ѡ��LSI��ΪRTCʱ��  
	while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);	//���ָ����RCC��־λ�������,�ȴ����پ������
//	{
//		temp++;
//		SysTick_Delay_Ms(10);
//	}
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);		//����RTCʱ��(RTCCLK),ѡ��LSI��ΪRTCʱ��      
	RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ��  
	RTC_WaitForSynchro();		//�ȴ�RTC�Ĵ���ͬ�� 
	RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
	 
//		RTC_ITConfig(RTC_IT_SEC, ENABLE);		//ʹ��RTC���ж�
//	RTC_EnterConfigMode();/// ��������	
	RTC_SetPrescaler((40000/50)-1); //����RTCԤ��Ƶ��ֵ		656����ֵΪ20ms����
	RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������

	RTC_ExitConfigMode(); //�˳�����ģʽ  

	RTC_NVIC_Config();//RCT�жϷ�������		    				     
	return 0; //ok

}		 				    
//RTCʱ���ж�
//ÿ20ms����һ��  
//extern u16 tcnt; 
void RTC_IRQHandler(void)
{		 
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//�����ж�
	{	
//		if(PWM1_R)PWM1_LOW;	
//		else PWM1_HIGH;	
			EXTI->IMR |= EXTI_Line9;  //�����ж�
			RTC_ITConfig(RTC_IT_SEC, DISABLE);		//��ֹRTC���ж�		
			RTC_WaitForLastTask();	 		
 	}
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//�����ж�
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);		//�������ж�	  	

  	} 				  								 
	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);		//�������ж�
	RTC_WaitForLastTask();	  	    						 	   	 
}


