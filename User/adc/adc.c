//#include "adc.h"



//static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    

////u16 SPI2_Tx[8]={0xF1CB,0xF1CB,0xC1CB,0xC1CB,0xD1CB,0xD1CB,0xE1CB,0xE1CB};      //0xC1AB ����һ�ε��δ��䣬��A0 GND��������ѹ��,FS=6.144V,һ�δ���ģʽ�������ٶ�250SPS��ADCģʽ���������������¼Ĵ�����
//                                                                               //0xD1AB ��A1,GND�����ʣ�
//                                                                               //0xE1AB ��A2,GND��������
//                                                                               //0xF1AB ��A3,GND����ѹ��
//																																							 //0xF18B ��A3, 475SPS��2.1msת��һ������
////u16 SPI2_Tx[8]={0xF18B,0xF18B,0xC18B,0xC18B,0xD18B,0xD18B,0xE18B,0xE18B};      //0xC1AB ����һ�ε��δ��䣬��A0 GND��������ѹ������ ,FS=6.144V,һ�δ���ģʽ�������ٶ�250SPS��ADCģʽ���������������¼Ĵ�����
//                                                                               //0xD1AB ��A1,GND�����ʣ�
////��ѹ ���� ���� ��ѹ����                                                      //0xE1AB ��A2,GND�������� ��ѹ����
//                                                                               //0xF1AB ��A3,GND����ѹ��
//																																							 //0xF18B ��A3, 128SPS��7.8msת��һ������

////static const u16 ADC_Array_Number = ADC_Average_Number * 2;           //��������������Ĵ�С                   
//u16 SPI2_Tx[4]={0xF18B,0xC18B,0xD18B,0xE18B};     //0xC18B ����һ�ε��δ��䣬��A0 GND ���� ,FS=6.144V,һ�δ���ģʽ�������ٶ�128SPS��ADCģʽ���������������¼Ĵ�����
//                                                  //0xD18B ��A1,GND ���� 
////��ѹ ���� ���� ��ѹ����                         //0xE18B ��A2,GND ��ѹ����
//																									//0xF18B ��A3,GND ��ѹ(�ڲ�) 
//																									//128SPS��7.8msת��һ������

//int16_t Register_P[ADC_Average_Number],Register_I[ADC_Average_Number], Register_Vext[ADC_Average_Number], Register_Vint[ADC_Average_Number];    //���������ݵ�����
//static u16 I_Number = 0;     //�����ջ�������Ӧ�÷���������ĸ��ط�
//static u16 P_Number = 0;
//static u16 Vext_Number = 0;
//static u16 Vint_Number = 0;

//static __IO uint32_t  SPI2Timeout = SPIT_LONG_TIMEOUT; 

//uint8_t ADC_CH;


////SPI2��ͨѶ����
//int16_t SPI2_ADC_Transfer(int16_t halfword)
//{
//	 SPI2Timeout = SPIT_FLAG_TIMEOUT;
//  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
//  while (SPI_I2S_GetFlagStatus(SPI2 , SPI_I2S_FLAG_TXE) == RESET)
//	{
//    if((SPITimeout--) == 0) 
//		{
//		  return 0;			
//		}
//  }

//  /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
//  SPI_I2S_SendData(SPI2 ,halfword);

//	SPITimeout = SPIT_FLAG_TIMEOUT;
//  /* �ȴ����ջ������ǿգ�RXNE�¼� */
//  while (SPI_I2S_GetFlagStatus(SPI2 , SPI_I2S_FLAG_RXNE) == RESET)
//  {
//    if((SPITimeout--) == 0)
//		{
//			return 1;
//		}			
//   }
//	
//  /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
//  return SPI_I2S_ReceiveData(SPI2);
//}


///**
//  * @brief  SPI2��ʼ��
//  * @param  ��
//  * @retval ��
//  */
//void SPI2_ADC_config(void)
//{
//	SPI_InitTypeDef  SPI_InitStructure;
//  GPIO_InitTypeDef GPIO_InitStructure;
//	EXTI_InitTypeDef EXTI_InitStructure;
////	DMA_InitTypeDef DMA_InitStructure;  	
//	NVIC_InitTypeDef NVIC_InitStructure;  
//	
//	/*��ʱ��*/
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
////	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	
//	
//	GPIO_SetBits(GPIOB, GPIO_Pin_12);//����CS��ƽ
//	
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line14;  
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //������Ըĳ�EXTI_Trigger_Rising_Falling��
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;  
//	EXTI->IMR &=~EXTI_Line14;  //�����ж�  	
//	EXTI_Init(&EXTI_InitStructure); 
//	
//	/*SCK MOSI MISO*/
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//SCK
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//MOSI
//	GPIO_Init(GPIOB, &GPIO_InitStructure);  
//	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//MISO������ĳ�
//  GPIO_Init(GPIOB, &GPIO_InitStructure);

//	
//	
//	/*CS */
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//CS
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//	

//	
//	/*- SPI2 -*/
//  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	
//  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 										  
//  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;	 								  
//  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		 										  
//  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;											 
//  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		   										  
//  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; 	
//  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  							  
//  SPI_Init(SPI2, &SPI_InitStructure);																	
//  SPI_Cmd(SPI2 , ENABLE);	
//	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //PPP�ⲿ�ж���  
////	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
//	
////	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//���ȼ����ɸ��ڰ��  

//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
//	NVIC_Init(&NVIC_InitStructure);  
//		
//	EXTI->PR = EXTI_Line14;//���ж�
//	EXTI->IMR |= EXTI_Line14;//�����ж�
//	GPIO_ResetBits(GPIOB, GPIO_Pin_12);//�������CS��ƽ��ѡ��оƬ
//	SysTick_Delay_Ms(50);
//	EXTI_GenerateSWInterrupt(EXTI_Line14);//��������ж�һ��
//	
//}
//	u8 adc_count;
////MISO�½����жϺ���
//void EXTI15_10_IT(void)  //7.8MS�ж�
//{ 
//	
//    if ((EXTI->PR & EXTI_Line14) != RESET)//�ж��ж�
//    {
//			EXTI->PR = EXTI_Line14;						//����жϱ�־λ
//			EXTI->IMR &=~EXTI_Line14;         //����EXTI�ж�
//			
//			switch(ADC_CH)
//			{
//				case 0://���Ͳ���Vintָ�����Vext����
//					Register_Vext[Vext_Number] = SPI2_ADC_Transfer(SPI2_Tx[0]);//���Ͳ���ָ������ϴε�AD����
//				  Vext_Number++;
//					SPI2_ADC_Transfer(SPI2_Tx[0]);//���ͻض����üĴ���ֵ
////					if(Vext_Number >= ADC_Average_Number) 				Vext_Number = 0;
//					ADC_CH++;	
//					break;

//				case 1://���Ͳ���Iָ�����Vint����
//					Register_Vint[Vint_Number] = SPI2_ADC_Transfer(SPI2_Tx[1]);
//				  Vint_Number++;

//					SPI2_ADC_Transfer(SPI2_Tx[1]);
////				if(Vint_Number >= ADC_Average_Number) 				Vint_Number = 0;

//					ADC_CH++;	
//				break;
//				
//				case 2://���Ͳ���Pָ�����I����
//					Register_I[I_Number] = SPI2_ADC_Transfer(SPI2_Tx[2]);
//				  I_Number++;
////					adc_count ++;
//					SPI2_ADC_Transfer(SPI2_Tx[2]);
////				if(I_Number >= ADC_Average_Number) 				I_Number = 0;
//					ADC_CH++;
//				break;
//				
//				case 3://���Ͳ���Vextָ�����P����
//					Register_P[P_Number] = SPI2_ADC_Transfer(SPI2_Tx[3]);
//				  P_Number++;
//					adc_count ++;
//					SPI2_ADC_Transfer(SPI2_Tx[3]);
////				if(I_Number >= ADC_Average_Number) 				I_Number = 0;
//					ADC_CH = 0;	//��ʼ�µĲ�������
//				break;
//				default:break;
//			}
//			if(adc_count == 4)
//			{
//				adc_count = 0;
//				data_exchange();
//				Vext_Number = 0;
//				Vint_Number = 0;
//				P_Number = 0;
//				I_Number = 0;
//				
//			}				
//			EXTI->IMR |= EXTI_Line14;//�����ж�
//		}  
//} 


//void data_exchange(void)
//{
//	int z;
//	uint32_t temp = 0;
//	uint32_t Value_Vint = 0;
//	uint32_t Value_I = 0;
//	uint32_t Value_P = 0;
//	uint32_t Value_Vext = 0;
//	for(z=0;z<4;z++)//�������
//	{
//		Value_Vint 	+= Register_Vint[z];
//		Value_I 		+= Register_I[z];
//		Value_P 		+= Register_P[z];
//		Value_Vext 	+= Register_Vext[z];
//	}
//	Value_Vint >>= 2;			//����4ȡƽ��ֵ
//	Value_I		 >>= 2;
//	Value_P		 >>= 2;
//	Value_Vext >>= 2;
//	
//	temp = Value_I;
//////	if(Value_Vint >= 42) Value_Vint -= 42;
//////	else 
////	if(Value_Vint < 42)	Value_Vint = 0;
//////	if(Value_Vext >= 88)Value_Vext -= 88;
//////	else 
////	if(Value_Vext < 88)	Value_Vext = 0;
//////	if(Value_I >= 105)Value_I -= 105;
//////	else 
////	if(Value_I < 105)	Value_I = 0;
////	
//	Value_Vint = 37157 * (u16)Value_Vint + 353990;	//����Vint�ĵ�ѹ
//	if(Value_Vint & 0x80000000) Vint = 0;
//	else 
//		Vint = Value_Vint/10000000.0;

//	Value_I = 47 * (u16)Value_I - 5091;					//�������
////	Value_I = 47 * (u16)Value_I + 3000;					//�������
//	if(Value_I & 0x80000000)Current = 0;//��ȥ��ֵ
//	else 
//		Current = Value_I / (10000.0/0.995);
//	
//	
////			I_Compare();

//	Value_Vext = 39 * (u16)Value_Vext - 2671;			//����Vext�ĵ�ѹ
//	if(Value_Vext > 1000000) Vext = 0;
//	else 
//		Vext = Value_Vext / 10000.0;
//	
////		Vint = 0.0037157 * (u16)Value_Vint + 0.0353990;	//����Vint�ĵ�ѹ

////		Current = (0.0047 * 0.995) * (u16)Value_I - (0.5091 * 0.995);					//�������

////		Vext = 0.0039 * (u16)Value_Vext - 0.2671;			//����Vext�ĵ�ѹ

//	if(Use_Vint_Or_Vext_Flag == Use_Vint) Voltage = Vint;
//	else if(Use_Vint_Or_Vext_Flag == Use_Vext) Voltage = Vext;

//	if(Value_P < 570)  //100W����
//		{
////					P = (float)0.2196 * temp - 19.922;
//			Value_P = 2211 * (u16)Value_P - 183590;			//
//		}
//		else 
//		{
////					P = (float)0.231 * temp - 28.695;
//					Value_P = 2314 * (u16)Value_P - 272910;			//
//		}
//	if(Value_P & 0x80000000) Power = 0;
//	else 
//		Power = Value_P / 10000.0;
////	Power = Voltage * Current;	
//	SCR_Ser(Power);	
//}



#include "adc.h"

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    

u16 SPI2_Tx[8]={0xF1CB,0xF1CB,0xC1CB,0xC1CB,0xD1CB,0xD1CB,0xE1CB,0xE1CB};      //0xC1AB ����һ�ε��δ��䣬��A0 GND��������ѹ��,FS=6.144V,һ�δ���ģʽ�������ٶ�250SPS��ADCģʽ���������������¼Ĵ�����
                                                                               //0xD1AB ��A1,GND�����ʣ�
                                                                               //0xE1AB ��A2,GND��������
                                                                               //0xF1AB ��A3,GND����ѹ��
																																							 //0xF18B ��A3, 475SPS��2.1msת��һ������
//u16 SPI2_Tx[8]={0xF18B,0xF18B,0xC18B,0xC18B,0xD18B,0xD18B,0xE18B,0xE18B};      //0xC1AB ����һ�ε��δ��䣬��A0 GND��������ѹ������ ,FS=6.144V,һ�δ���ģʽ�������ٶ�250SPS��ADCģʽ���������������¼Ĵ�����
                                                                               //0xD1AB ��A1,GND�����ʣ�
//��ѹ ���� ���� ��ѹ����                                                      //0xE1AB ��A2,GND�������� ��ѹ����
                                                                               //0xF1AB ��A3,GND����ѹ��
																																							 //0xF18B ��A3, 128SPS��7.8msת��һ������

//static const u16 ADC_Array_Number = ADC_Average_Number * 2;           //��������������Ĵ�С                   
int16_t Register_I[ADC_Average_Number], Register_P[ADC_Average_Number], Register_Vext[ADC_Average_Number], Register_Vint[ADC_Average_Number];    //���������ݵ�����
static u16 I_Number = 0;     //�����ջ�������Ӧ�÷���������ĸ��ط�
static u16 P_Number = 0;
static u16 Vext_Number = 0;
static u16 Vint_Number = 0;

static __IO uint32_t  SPI2Timeout = SPIT_LONG_TIMEOUT; 


int16_t CH0[2],CH1[2],CH2[2],CH3[2];     //���������ʣ�������ѹ���ڲ���ѹ

uint8_t ADC_CH;


//SPI2��ͨѶ����
int16_t SPI2_ADC_Transfer(int16_t halfword)
{
	 SPI2Timeout = SPIT_FLAG_TIMEOUT;
  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
  while (SPI_I2S_GetFlagStatus(SPI2 , SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) 
		{
		  return 0;			
		}
  }

  /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
  SPI_I2S_SendData(SPI2 ,halfword);

	SPITimeout = SPIT_FLAG_TIMEOUT;
  /* �ȴ����ջ������ǿգ�RXNE�¼� */
  while (SPI_I2S_GetFlagStatus(SPI2 , SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((SPITimeout--) == 0)
		{
			return 1;
		}			
   }
	
  /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
  return SPI_I2S_ReceiveData(SPI2);
}

/**
  * @brief  ���ݼĴ�����ֵ�����ڲ���ѹ
  * @param  �Ĵ�����ֵ
  * @retval �ڲ���ѹ
  */
static float Calculate_Vint(int temp)
{
	int Vint = 0;
	
//  Vint = 0.00364 * temp + 0.446;
//	Vint = 0.99*(0.0038*temp+0.1248);      //CVģʽ ������
//	Vint = 0.999*(0.0037*temp+0.0275)-0.015; //CCģʽ���
//	  Vint = 1.006 *(0.0037 * temp + 0.7658);   //CPģʽ���
//	  Vint = 0.991 * (0.0038 * temp - 0.0532);    //CV����  200ŷ
//				Vint =  temp*80000/21333;    //CV   1ŷ

	//	if (Vint < 60)
//	{
//		Vint = 0;
//	}
//	return (float)(Vint/1000.0);
	
//	Vint = 1.01 * (0.0037 * temp - 0.1508); 
	Vint = 37.37 * temp - 1523.08;
	
	if(Vint < 1500)
	{
		Vint = 0;
	}
	
	return (float)(Vint/10000.0);
	

}

/**
  * @brief  ���ݼĴ�����ֵ�������
  * @param  �Ĵ�����ֵ
  * @retval ����ֵ
  */
static float Calculate_I(int temp)
{
	int I;
	
//	I = (float)0.0001875 * temp + 0.08;
//	I = (float) I * 25;
//	I = 0.994*(0.0047*temp-0.3227)-0.02;	
//	  I = 0.0046676*temp - 0.3195166;
//	  I = 1.01*(0.0047*temp-0.2814);         //CCģʽ
//	  I = 0.9966 * (0.0048 * temp + 0.555);       //CPģʽ
//	  I = 0.9915 * (0.0048 * temp - 0.373);     //CCģʽ   1ŷ     
//			I = 100000 * temp/21333;     //-8407

//	I = 46.437 * temp - 416.9235;	

	
		I = 46.4324 * temp - 716.8822;
	
	
//	I = 46.3128 * temp - 99.6732;
	
	
	if(I < 2000)
	{
		I = 0;
	}
	return (float)(I/10000.0);
	//return (float)(I*(0.995/10000.0));
}

/**
  * @brief  ���ݼĴ�����ֵ���㹦��
  * @param  �Ĵ�����ֵ
  * @retval ����
  */
static float Calculate_P(int temp)
{
	int P;
	
//	P = (float)0.0001875 * temp + 0.08;
//	P = (float)0.2799 * temp + 1.7507;
//	  if(temp <= -150)     //10W����
//			P = 0;
//	  else if((-150 < temp) & (temp < 160))
//		{
////			P = (float)0.00007 * temp * temp + 0.2989 * temp + 54.257;
//			P = (float)0.00007 * temp * temp + 0.2989 * temp;

//		}
//		else 
//		{
////				P = (float)0.2802 * temp + 54.257;
//				P = (float)0.2802 * temp ;

//		}
//	
	
			if(temp < 570)  //100W����
			{
//					P = (float)0.2196 * temp - 19.922;
						P = 2211 * temp - 183590;

			}
			else 
			{
//					P = (float)0.231 * temp - 28.695;
						P = 2314 * temp - 272910;
			}
			
	    if(P < 10000)
				P = 0;

	  return (float)(P/10000.0);
}

/**
  * @brief  ���ݼĴ�����ֵ���㲹����ѹ
  * @param  �Ĵ�����ֵ
  * @retval ������ѹ
  */
static float Calculate_Vext(int temp)
{
	int Vext;
	
//	Vext = (float)0.0001875 * temp + 0.08;
//	  Vext = (float)1.006 * (0.0039 * temp - 0.2745);
//		Vext = 37.259 * temp - 745.18 ;
	Vext = 36.741 * temp +624.597;

	if(Vext < 3200)
		Vext = 0;
	
	return (float)(Vext/10000.0);
}

/**
  * @brief  SPI2��ʼ��
  * @param  ��
  * @retval ��
  */
void SPI2_ADC_config(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
//	DMA_InitTypeDef DMA_InitStructure;  	
	NVIC_InitTypeDef NVIC_InitStructure;  
	
	/*��ʱ��*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	
	
	GPIO_SetBits(GPIOB, GPIO_Pin_12);//����CS��ƽ
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;  
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //������Ըĳ�EXTI_Trigger_Rising_Falling��
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;  
	EXTI->IMR &=~EXTI_Line14;  //�����ж�  	
	EXTI_Init(&EXTI_InitStructure); 
	
	/*SCK MOSI MISO*/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//SCK
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//MOSI
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//MISO������ĳ�
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	
	
	/*CS */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//CS
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	

	
	/*- SPI2 -*/
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 										  
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;	 								  
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		 										  
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;											 
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		   										  
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; 	
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  							  
  SPI_Init(SPI2, &SPI_InitStructure);																	
  SPI_Cmd(SPI2 , ENABLE);	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //PPP�ⲿ�ж���  
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
	
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//���ȼ����ɸ��ڰ��  

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  
		
	EXTI->PR = EXTI_Line14;//���ж�
	EXTI->IMR |= EXTI_Line14;//�����ж�
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);//�������CS��ƽ��ѡ��оƬ
	SysTick_Delay_Ms(50);
	EXTI_GenerateSWInterrupt(EXTI_Line14);//��������ж�һ��
	
}
	u8 adc_count;
//MISO�½����жϺ���
void EXTI15_10_IT(void)  
{ 
	
    if ((EXTI->PR & EXTI_Line14) != RESET)//�ж��ж�
    {
			EXTI->PR = EXTI_Line14;						//����жϱ�־λ
			EXTI->IMR &=~EXTI_Line14;         //����EXTI�ж�
			
			switch(ADC_CH)
			{
				case 0://���Ͳ���Vintָ�����Vext����
					Register_Vext[Vext_Number] = SPI2_ADC_Transfer(SPI2_Tx[0]);//���Ͳ���ָ������ϴε�AD����
				  Vext_Number++;
					adc_count ++;

				SPI2_ADC_Transfer(SPI2_Tx[0]);//���ͻض����üĴ���ֵ
					if(Vext_Number >= ADC_Average_Number)Vext_Number = 0;
					ADC_CH++;	
					break;

				case 1://���Ͳ���Iָ�����Vint����
					Register_Vint[Vint_Number] = SPI2_ADC_Transfer(SPI2_Tx[2]);
				  Vint_Number++;

					SPI2_ADC_Transfer(SPI2_Tx[2]);
				if(Vint_Number >= ADC_Average_Number)Vint_Number = 0;
					ADC_CH++;	
				break;
				
				case 2://���Ͳ���Vextָ�����I����
					Register_I[I_Number] = SPI2_ADC_Transfer(SPI2_Tx[6]);
				  I_Number++;

					SPI2_ADC_Transfer(SPI2_Tx[6]);
					if(I_Number >= ADC_Average_Number)I_Number = 0;
//					ADC_CH++;	
					ADC_CH = 0;	//��ʼ�µĲ�������
				break;
				
				case 3://���Ͳ���Vextָ�����P����
					Register_P[P_Number] = SPI2_ADC_Transfer(SPI2_Tx[6]);
				  P_Number++;

					SPI2_ADC_Transfer(SPI2_Tx[6]);
				if(P_Number >= ADC_Average_Number)P_Number = 0;
					ADC_CH = 0;	//��ʼ�µĲ�������
				break;
			}
			if(adc_count > 25)
			{
				adc_count = 0;
				Average_Calculate();//����ƽ��ֵ 
			}				
			EXTI->IMR |= EXTI_Line14;//�����ж�
		}  
} 


int	Calculate(int16_t Array[],u16 n)
{
	int z;
	int32_t Sum = 0;
	for(z=0;z<n;z++)
	{
		Sum = Sum + Array[z];
	}
	return (Sum/ADC_Average_Number);
}

/*ð���������ƽ��ֵ�ӳ���*/
int Bubble_Sort_Calculate(int16_t Array[],u16 n)
{
	int i,j,z;
	int32_t Sum = 0;
	int end;
	int16_t buff_ADC[ADC_Average_Number];
	for(i=0;i<n-1;i++)buff_ADC[i] = Array[i];
	for(i=0;i<n-1;i++)
	{
		for(j=0;j<n-i-1;j++)
		{
			if(buff_ADC[j] > buff_ADC[j+1])		//���ǰ�ߴ��ں���
			{
				int16_t result = buff_ADC[j];//�򽻻����ߵ�ֵ
				buff_ADC[j] = buff_ADC[j+1];
				buff_ADC[j+1] = result;
			}
		}
	}
	
	
	//32�����ݣ�ȥ����β8����ȡ16������
	for(z=8;z<n-8;z++)
	{
		Sum = Sum + Array[z];
	}
	
	end = Sum / 16;
	
	return end;
}






			int ADC_Vint = 0;
			int ADC_I = 0;
			int ADC_P = 0;
			int ADC_Vext = 0;
 
//����ƽ��ֵ����
void Average_Calculate(void)
{
//			if(Vint_Number >= ADC_Average_Number)      //�����ڲ���ѹ
//			{
				int Value_Vint;
				int Value_I;
				int Value_P;
				int Value_Vext;
				
//				Value_Vint = Calculate(Register_Vint,ADC_Average_Number);//ֱ����ƽ��
				Value_Vint = Bubble_Sort_Calculate(Register_Vint,ADC_Average_Number);//ð�ݼ���

				ADC_Vint = Value_Vint;//��ֵ���۲����
				
				Vint = Calculate_Vint(Value_Vint);    //����Vintf�ĵ�ѹ
			
				if(Use_Vint_Or_Vext_Flag == Use_Vint)
				{
					Voltage = Vint;
				}
			
				if((Wait_Below_Set_Value_Flag == Havent_Below) && (Voltage < 1) && (ON_OFF == ON))
				{
					Wait_Below_Set_Value_Flag = Already_Below;	
					Power_On();
				}
				

		
//				Vint_Number = 0;   //���ü���
//			}
			
			
//			if(I_Number >= ADC_Average_Number)    //�������
//			{
//				
//				float Value_I;
				
				Value_I = Calculate(Register_I,ADC_Average_Number);//ֱ����ƽ��
//				Value_I = Bubble_Sort_Calculate(Register_I,ADC_Average_Number);//ð�ݼ���
				
				ADC_I = Value_I;//�۲����
				Current = Calculate_I(Value_I);
					
//				I_Number = 0;
			
				if((Wait_Below_Set_Value_Flag == Havent_Below) && (Current < 1) && (ON_OFF == ON) && (Use_Vint_Or_Vext_Flag == Use_Vint))
				{
					Wait_Below_Set_Value_Flag = Already_Below;
					Power_On();
				}
//			}



//		if(P_Number > ADC_Average_Number)    //���㹦��
//		{
//			float Value_P;
			Power = Voltage * Current;
//			Value_P=Calculate(Register_P,ADC_Average_Number);//ֱ����ƽ��
//			Value_P=Bubble_Sort_Calculate(Register_P,ADC_Average_Number);//ð�ݼ���
//			Power = Calculate_P(Value_P);		
			SCR_Ser(Power);//�ð��
			ADC_P = Value_P;//�۲����
//			P_Number = 0;
//			
			if((Wait_Below_Set_Value_Flag == Havent_Below) && (Power < 50) && (ON_OFF == ON))
			{
				Wait_Below_Set_Value_Flag = Already_Below;
				
				Power_On();
			}
//		}
		
		
		
		
//		if(Vext_Number > ADC_Average_Number)    //���㲹����ѹ
//		{
//				float Value_Vext;
			
//				Value_Vext=Calculate(Register_Vext,ADC_Average_Number);//ֱ����ƽ��
				Value_Vext=Bubble_Sort_Calculate(Register_Vext,ADC_Average_Number);//ð�ݼ���
				Vext = Calculate_Vext(Value_Vext);
			
				ADC_Vext = Value_Vext;//�۲����
				
				if(Use_Vint_Or_Vext_Flag == Use_Vext)
				{
					Voltage = Vext;
				}
			
//				Vext_Number = 0;
//			
				if((Wait_Below_Set_Value_Flag == Havent_Below) && (Current < 1) && (ON_OFF == ON) && (Use_Vint_Or_Vext_Flag == Use_Vext))
				{
					Wait_Below_Set_Value_Flag = Already_Below;
				
					Power_On();
				}
//		}					
}









































 


