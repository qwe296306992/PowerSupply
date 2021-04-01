#include "protect.h"
#include "softstart.h"
#include "SCR_Controller.h"


u32 TIM_PROTECT_time; 
int CRL_Voltage;        //�����Ƿѹ����
u8 adcs=0;//

//��������������
__IO uint16_t ADC_ConvertedValue[ADC_Buffer_Size]={0};

u8 OVP_InputOrOutput_Flag = 2;   //0��������OVP��1�������OVP��2�����ޱ���
u8 OCP_SoftOrHard_Flag = 2;//0�������OCP��1����Ӳ��OCP��2�����ޱ���
u8 OTP_SensorX = 0;				 //1����Sensor1������2����Sensor2������3����Sensor3������0�����ޱ���

//����ʹ�ܿ���
FunctionalState keys_EN = DISABLE;

/*���д��PD11������������OC1*/
void TestCurrent_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
}


//�����ж����ȼ�
static void TIM_PROTECT_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	
	// �����ж���Ϊ0
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
	// �����ж���Դ
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	
	// ���������ȼ�Ϊ 1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 
	// ������ռ���ȼ�Ϊ3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	// ������ռ���ȼ�Ϊ3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_Init(&NVIC_InitStructure);	

//	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//PD11Ӳ�����������ж�
//  NVIC_Init(&NVIC_InitStructure);	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	 
	// ������ռ���ȼ�Ϊ3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
  NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel1_IRQn;
  NVIC_Init(&NVIC_InitStructure);	
}

//����TIM4,0.1s��ʱʱ��
static void TIM_PROTECT_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		
	  // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period=999;
	  // ʱ��Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_Prescaler= 7199;		
		// ʱ�ӷ�Ƶ���� 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		// ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	
		// �ظ���������ֵ
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	  // ��ʼ����ʱ��
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
		// ����������жϱ�־λ
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	  
		// �����������ж�
    TIM_ITConfig(TIM4, TIM_IT_Update,ENABLE);
		
		// ʹ�ܼ�����
    TIM_Cmd(TIM4, ENABLE);
}

static void EXTI_PROTECT_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/*��������GPIO�ڵ�ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
												
	
/*--------------------------OC1����-----------------------------*/
//	/* ѡ�񰴼��õ���GPIO */	
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//  /* ����Ϊ�������� */	
////  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	
//  GPIO_Init(GPIOD, &GPIO_InitStructure);

//	/* ѡ��EXTI���ź�Դ */
//  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource11); 
//  EXTI_InitStructure.EXTI_Line = EXTI_Line11;
//	
//	/* EXTIΪ�ж�ģʽ */
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	/* �½������ж� */
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//  /* ʹ���ж� */	
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);
	
  /*--------------------------OC2����-----------------------------*/
	/* ѡ�񰴼��õ���GPIO */	
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//  /* ����Ϊ�������� */	
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//  GPIO_Init(GPIOD, &GPIO_InitStructure);

//	/* ѡ��EXTI���ź�Դ */
//  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource10); 
//  EXTI_InitStructure.EXTI_Line = EXTI_Line10;

//	/* EXTIΪ�ж�ģʽ */
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	/* �½����ж� */
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//  /* ʹ���ж� */	
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);	
	
  /*--------------------------Sensor_1����-----------------------------*/
	/* ѡ�񰴼��õ���GPIO */	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  /* ����Ϊ�������� */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* ѡ��EXTI���ź�Դ */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	
	/* EXTIΪ�ж�ģʽ */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* �������ж� */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	
  /* ʹ���ж� */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);		
	
  /*--------------------------Sensor_2����-----------------------------*/
	/* ѡ�񰴼��õ���GPIO */	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  /* ����Ϊ�������� */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* ѡ��EXTI���ź�Դ */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	
	/* EXTIΪ�ж�ģʽ */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* �������ж� */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* ʹ���ж� */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);		
	
  /*--------------------------Sensor_3����-----------------------------*/
	/* ѡ�񰴼��õ���GPIO */	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  /* ����Ϊ�������� */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* ѡ��EXTI���ź�Դ */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	
	/* EXTIΪ�ж�ģʽ */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* �������ж� */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* ʹ���ж� */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);		
}

static void ADC_PROTECT_Config(void)
{
	//PC5
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;	
	
	// �� ADC IO�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE );
	
	// ���� ADC IO ����ģʽ
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	// ��ʼ�� ADC IO
	GPIO_Init(GPIOC, &GPIO_InitStructure);		

	//ADC1ͨ��15��DMA1ͨ��1
	
	// ��DMAʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	// ��ADCʱ��
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1, ENABLE );
	
	// ��λDMA������
	DMA_DeInit(DMA1_Channel1);
	
	// ���� DMA ��ʼ���ṹ��
	// �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( ADC1->DR ) );
	
	// �洢����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;
	
	// ����Դ��������
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	// ��������С��Ӧ�õ�������Ŀ�ĵصĴ�С
	DMA_InitStructure.DMA_BufferSize = ADC_Buffer_Size;
	
	// ����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	
	// �洢����ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	
	// �������ݴ�СΪ���֣��������ֽ�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	// �ڴ����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	// ѭ������ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
	
	// DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	// ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	// ��ʼ��DMA
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	//ʹ���ж�
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
	
	// ʹ�� DMA ͨ��
	DMA_Cmd(DMA1_Channel1 , ENABLE);
		
	//ADC ģʽ����
	//ֻʹ��һ��ADC�����ڵ�ģʽ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	//ɨ��ģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 
	
	//����ת��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	
	//�����ⲿ����ת���������������
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	
	//ת������Ҷ���
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	//ת��ͨ������
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
	
	//��ʼ��ADC
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//����ADCʱ�ӣΪPCLK2��8��Ƶ����9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	//����ADC ͨ����ת��˳��Ͳ���ʱ��,  ת��һ����Ҫ26.6us
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_239Cycles5);  
	
	//ʹ��ADC DMA ����
	ADC_DMACmd(ADC1, ENABLE);
	
	//����ADC ������ʼת��
	ADC_Cmd(ADC1, ENABLE);
	
    //��ʼ��ADC У׼�Ĵ���  
	ADC_ResetCalibration(ADC1);
    //�ȴ�У׼�Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	//ADC��ʼУ׼
	ADC_StartCalibration(ADC1);
	//�ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADC1));
	
	//����û�в����ⲿ����������ʹ���������ADCת�� 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
}




//�������ܸ��ֳ�ʼ��
void TIM_PROTECT_Init(void)
{
	TIM_PROTECT_Mode_Config();//��������Ķ�ʱ��0.1s����
	TIM_PROTECT_NVIC_Config();//���������ʱ��������Ӳ���ж��Լ�DMA���ȼ���ʼ��
	EXTI_PROTECT_Config();//�ⲿ�жϱ�����ʼ������OC1����Ӳ��������Sensor1_2_3���±���
	ADC_PROTECT_Config();//�����Ƿѹ��ʼ��
	
}


/**
  * @brief  TIM4���жϷ�����,���ڼ�⹦�ʺ͵�ѹ�͵���,ͬʱ�������������ж�ʱ��Ϊ0.1s������ʱ��Ϊ3s
  * @param  None
  * @retval None
  */
u16 softtime = 60;//��������ʱʱ��

void TIM_P_Protect_IT(void)
{	
	if(softtime > 0)//����������
	{
		softtime --;
//		if(softtime == 31) t_count = 5;//9500-5*2*30*5 = 8000  9500-10*70-30*5*10 =7300
		
				//if(softtime == 31) t_count = 12;//9500-12*2*30*5 = 8000  9500-70*10-12*2*30*5 = 5200     ����Ҫt_count

//		else if(softtime == 10) t_count = 33;
		if(softtime == 0)
		{
			keys_EN = ENABLE;//ENABLE����
			t_count = 50;
			SCR_On_Off(ENABLE);
//			SCR_On_Off(DISABLE);
			Power_AC_On();
		}
	}
  
	if(Power > 6100)//ʵ���Ҳ������2500W��16A����Ϊ3500W
	{
		ON_OFF = OFF;
		OUTPUT_NOT_OUTPUT = NOT_OUTPUT;
		
		Output_Zero();				
		
		TIM_DELAY_OFF();
		TIM_CYCLE_OFF();
		
		Error |= OPP;
		
		Display_Interface_Mode = Display_Interface_Mode_Protect_Interface;
	}
	
  if(Voltage > 85)//85
	{
		ON_OFF = OFF;
		OUTPUT_NOT_OUTPUT = NOT_OUTPUT;
		
		Output_Zero();			
		
		TIM_DELAY_OFF();
		TIM_CYCLE_OFF();
		
		Error |= OVP;
		
		OVP_InputOrOutput_Flag = 1;
		
		Display_Interface_Mode = Display_Interface_Mode_Protect_Interface;		
	}
	
	if(Current > 105)//ʵ����������40A,16A����Ϊ60A
	{
		ON_OFF = OFF;
		OUTPUT_NOT_OUTPUT = NOT_OUTPUT;
		
		Output_Zero();			
		
		TIM_DELAY_OFF();
		TIM_CYCLE_OFF();
		
		Error |= OCP;
		
		OCP_SoftOrHard_Flag = 0;
		
		Display_Interface_Mode = Display_Interface_Mode_Protect_Interface;	
	}
//	
//	
	TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);   //����жϱ�־λ	
}

/**
  * @brief  EXTI11���жϷ�����,���ڵ�������(OC1)������
  * @param  None
  * @retval None
  */
//void EXTI11_I_Protect_IT(void)
//{
//	
//  //ȷ���Ƿ������EXTI Line�ж�
//	if(EXTI_GetITStatus(EXTI_Line11) != RESET) 
//	{
//		ON_OFF = OFF;
//		OUTPUT_NOT_OUTPUT = NOT_OUTPUT;
//		
//		Output_Zero();				
//		
//		TIM_DELAY_OFF();
//		TIM_CYCLE_OFF();

//		Error |= OCP;
//		
//		OCP_SoftOrHard_Flag = 1;//Ӳ��OCP����
//		
//		adcs++;
//		
//		Display_Interface_Mode = Display_Interface_Mode_Protect_Interface;
//		
//		//����жϱ�־λ
//	  EXTI_ClearITPendingBit(EXTI_Line11); 	
//	}
//}

/**
  * @brief  EXTI10���жϷ�����,���ڵ�������
  * @param  None
  * @retval None
  */
//void EXTI10_I_Protect_IT(void)
//{
//  //ȷ���Ƿ������EXTI Line�ж�
//	if(EXTI_GetITStatus(EXTI_Line10) != RESET) 
//	{
//		ON_OFF = OFF;
//		OUTPUT_NOT_OUTPUT = NOT_OUTPUT;
//		
//		Output_Zero();				
//		
//		TIM_DELAY_OFF();
//		TIM_CYCLE_OFF();

//		Error |= OCP;
//		
//		Display_Interface_Mode = Display_Interface_Mode_Protect_Interface;  

//    //����жϱ�־λ
//	  EXTI_ClearITPendingBit(EXTI_Line10); 		
//	}	
//}

/**
  * @brief  EXTI5���жϷ�����,����Sensor1�¶ȱ���(IGBTģ����±���)
  * @param  None
  * @retval None
  */
void EXTI5_Temperature_Protect_IT(void)
{
  //ȷ���Ƿ������EXTI Line�ж�
	if(EXTI_GetITStatus(EXTI_Line5) != RESET) 
	{
		SysTick_Delay_Ms(10);
		if(EXTI_GetITStatus(EXTI_Line5) != RESET)
		{
			ON_OFF = OFF;
			OUTPUT_NOT_OUTPUT = NOT_OUTPUT;
			
			Output_Zero();					
			
			TIM_DELAY_OFF();
			TIM_CYCLE_OFF();
			
			Error |= OTP;	

			OTP_SensorX = 1;//Sensor1����
			
			Display_Interface_Mode = Display_Interface_Mode_Protect_Interface;    
			
			//����жϱ�־λ
			EXTI_ClearITPendingBit(EXTI_Line5);
		}
	} 	
}

/**
  * @brief  EXTI6���жϷ�����,����Sensor2�¶ȱ���(�������ģ����±���)
  * @param  None
  * @retval None
  */
void EXTI6_Temperature_Protect_IT(void)
{
  //ȷ���Ƿ������EXTI Line�ж�
	if(EXTI_GetITStatus(EXTI_Line6) != RESET) 
	{
		ON_OFF = OFF;
		OUTPUT_NOT_OUTPUT = NOT_OUTPUT;
		
		Output_Zero();				
		
		TIM_DELAY_OFF();
		TIM_CYCLE_OFF();
		
		Error |= OTP;		
		
		OTP_SensorX = 2;//Sensor2����
		
		Display_Interface_Mode = Display_Interface_Mode_Protect_Interface; 

    //����жϱ�־λ
	  EXTI_ClearITPendingBit(EXTI_Line6); 		
	}	
}

/**
  * @brief  EXTI7���жϷ�����,�����¶ȱ�����Ӳ���̽ӣ�û���õ������ڷ��ӱ���
  * @param  None
  * @retval None
  */
void EXTI7_Temperature_Protect_IT(void)
{
  //ȷ���Ƿ������EXTI Line�ж�
	if(EXTI_GetITStatus(EXTI_Line7) != RESET) 
	{
		ON_OFF = OFF;
		OUTPUT_NOT_OUTPUT = NOT_OUTPUT;
		
		Output_Zero();				
		
		TIM_DELAY_OFF();
		TIM_CYCLE_OFF();
		
		Error |= Sense_Reverse;		
		
//		OTP_SensorX = 3;//Sensor3����
		
		Display_Interface_Mode = Display_Interface_Mode_Protect_Interface;  

    //����жϱ�־λ
	  EXTI_ClearITPendingBit(EXTI_Line7); 			
	}
}


/**
  * @brief  DMA���жϷ�����,���ڹ�Ƿѹ���
  * @param  None
  * @retval None
  */
void ADC_V_Protect_IT(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1))
  {
	  int ADC_Voltage;                     //������Ƶ�·��ѹ��ADC12λֵ
	  int temp_Voltage=0;             //������Ƶ�·��ѹADC���ֵ���ۼ�ֵ
	
    u16 i;
	  for(i=0; i < ADC_Calculate; i++)
	  {
			temp_Voltage=temp_Voltage + ADC_ConvertedValue[i];
//		  temp_Voltage=temp_Voltage + pow((float)ADC_ConvertedValue[i] / 4095 * 3.3 , 2);
	  }
	  ADC_Voltage = temp_Voltage / ADC_Calculate;	
		
//		CRL_Voltage = ADC_Voltage * 330/ 4095;//2020-1-16
//		CRL_Voltage = sqrt(ADC_Voltage);
		CRL_Voltage = ADC_Voltage;//2020-7-10
		
//ԭʼ����2.55		
		//����220V,�õ�1.63V��ADCֵ���1630/3300*4095=2022��ȡ��12%����1780-2265
//		if((CRL_Voltage > 27*12) || (CRL_Voltage < 27*8)) //2020-1-16
	/*	
		if((CRL_Voltage > 2265) || (CRL_Voltage < 1780)) //2020-7-10
		{

			ON_OFF = OFF;
		  OUTPUT_NOT_OUTPUT = NOT_OUTPUT;
		
		  Output_Zero();				
//			Power_AC_Off();

			TIM_DELAY_OFF();
			TIM_CYCLE_OFF();
		
		  Error |= OVP;
			
			OVP_InputOrOutput_Flag = 0;
		
		  Display_Interface_Mode = Display_Interface_Mode_Protect_Interface;	
		}
		*/
	}
  DMA_ClearITPendingBit(DMA1_IT_GL1);
}


/**
  * @brief  ����¶ȿ����Ƿ���ϣ����ڵ���OC1����״̬δ֪��������ʱ�����
  * @param  None
  * @retval None
  */
void Check_T_I_Switch(void)
{
	if( (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 1) || (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 1) || (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 1) )    //û�н��¶ȿ���
	{
		ON_OFF = OFF;
		OUTPUT_NOT_OUTPUT = NOT_OUTPUT;
		
		Output_Zero();					
		
		TIM_DELAY_OFF();
		TIM_CYCLE_OFF();
		
		Error |= OTP;		
		
		Display_Interface_Mode = Display_Interface_Mode_Protect_Interface;  		
	}
//	else if((GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11) == 1) || (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 1))
//	{
//		ON_OFF = OFF;
//		OUTPUT_NOT_OUTPUT = NOT_OUTPUT;
//		
//		Output_Zero();				

//		TIM_DELAY_OFF();
//		TIM_CYCLE_OFF();		

//		Error |= OCP;
//		
//		Display_Interface_Mode = Display_Interface_Mode_Protect_Interface;  
//	}
}



//u8 ScanKeyStatus(GPIO_TypeDef*GPIOx,uint16_t GPIO_Pin)
//{
//	if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin))
//}


/**
  * @brief  Ӳ�����OCP״̬��������õ�����ѯ��ʽ
  * @param  None
  * @retval None
  */
void TestCurrent (void)
{
//	TIM_Cmd(TIM1, DISABLE);//�رչ��ʻ�����ʱ��
	EXTI->IMR &=~EXTI_Line14;//����MISO�½����ж�
	TIM_Cmd(TIM4, DISABLE);//��ʱ�ر�0.1s������ʱ��	
	TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update); //����жϱ�־λ
	ADC_DMACmd(ADC1, DISABLE);//�ر�adc��Ƿѹ�ж�
	DMA_ClearITPendingBit(DMA1_IT_GL1);

	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11) == 0)
	{
//		SysTick_Delay_Ms(5);
		Soft_Delay_us(15);
		if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11) == 0)
		{
		ON_OFF = OFF;
		OUTPUT_NOT_OUTPUT = NOT_OUTPUT;		
		Output_Zero();					
		TIM_DELAY_OFF();
		TIM_CYCLE_OFF();

		Error |= OCP;	
			
		OCP_SoftOrHard_Flag = 1;//Ӳ��OCP����
			
		Display_Interface_Mode = Display_Interface_Mode_Protect_Interface;
		}
	}	

	ADC_DMACmd(ADC1, ENABLE);//�ָ�adc��Ƿѹ�ж�
	TIM_Cmd(TIM4, ENABLE);//�ָ�0.1s������ʱ��
	EXTI->IMR |= EXTI_Line14;//�ָ�MISO�½����ж�
	
	
//	if(Error_Stop_Flag == 1)//�ر�֮ǰTIM1��ʱ��Ϊ����״̬��ָ�
//	{
//		TIM_Cmd(TIM1, ENABLE);//�ָ����ʻ�����ʱ��
//	}
	
}





