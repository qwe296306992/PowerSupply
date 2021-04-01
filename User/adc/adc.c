//#include "adc.h"



//static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    

////u16 SPI2_Tx[8]={0xF1CB,0xF1CB,0xC1CB,0xC1CB,0xD1CB,0xD1CB,0xE1CB,0xE1CB};      //0xC1AB 开启一次单次传输，用A0 GND（补偿电压）,FS=6.144V,一次传输模式，传输速度250SPS，ADC模式，引脚上拉，更新寄存器，
//                                                                               //0xD1AB 用A1,GND（功率）
//                                                                               //0xE1AB 用A2,GND（电流）
//                                                                               //0xF1AB 用A3,GND（电压）
//																																							 //0xF18B 用A3, 475SPS，2.1ms转换一个数据
////u16 SPI2_Tx[8]={0xF18B,0xF18B,0xC18B,0xC18B,0xD18B,0xD18B,0xE18B,0xE18B};      //0xC1AB 开启一次单次传输，用A0 GND（补偿电压）电流 ,FS=6.144V,一次传输模式，传输速度250SPS，ADC模式，引脚上拉，更新寄存器，
//                                                                               //0xD1AB 用A1,GND（功率）
////电压 电流 功率 电压补偿                                                      //0xE1AB 用A2,GND（电流） 电压补偿
//                                                                               //0xF1AB 用A3,GND（电压）
//																																							 //0xF18B 用A3, 128SPS，7.8ms转换一个数据

////static const u16 ADC_Array_Number = ADC_Average_Number * 2;           //定义收数据数组的大小                   
//u16 SPI2_Tx[4]={0xF18B,0xC18B,0xD18B,0xE18B};     //0xC18B 开启一次单次传输，用A0 GND 电流 ,FS=6.144V,一次传输模式，传输速度128SPS，ADC模式，引脚上拉，更新寄存器，
//                                                  //0xD18B 用A1,GND 功率 
////电压 电流 功率 电压补偿                         //0xE18B 用A2,GND 电压补偿
//																									//0xF18B 用A3,GND 电压(内部) 
//																									//128SPS，7.8ms转换一个数据

//int16_t Register_P[ADC_Average_Number],Register_I[ADC_Average_Number], Register_Vext[ADC_Average_Number], Register_Vint[ADC_Average_Number];    //定义收数据的数组
//static u16 I_Number = 0;     //定义收回来的数应该放在数组的哪个地方
//static u16 P_Number = 0;
//static u16 Vext_Number = 0;
//static u16 Vint_Number = 0;

//static __IO uint32_t  SPI2Timeout = SPIT_LONG_TIMEOUT; 

//uint8_t ADC_CH;


////SPI2的通讯程序
//int16_t SPI2_ADC_Transfer(int16_t halfword)
//{
//	 SPI2Timeout = SPIT_FLAG_TIMEOUT;
//  /* 等待发送缓冲区为空，TXE事件 */
//  while (SPI_I2S_GetFlagStatus(SPI2 , SPI_I2S_FLAG_TXE) == RESET)
//	{
//    if((SPITimeout--) == 0) 
//		{
//		  return 0;			
//		}
//  }

//  /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
//  SPI_I2S_SendData(SPI2 ,halfword);

//	SPITimeout = SPIT_FLAG_TIMEOUT;
//  /* 等待接收缓冲区非空，RXNE事件 */
//  while (SPI_I2S_GetFlagStatus(SPI2 , SPI_I2S_FLAG_RXNE) == RESET)
//  {
//    if((SPITimeout--) == 0)
//		{
//			return 1;
//		}			
//   }
//	
//  /* 读取数据寄存器，获取接收缓冲区数据 */
//  return SPI_I2S_ReceiveData(SPI2);
//}


///**
//  * @brief  SPI2初始化
//  * @param  无
//  * @retval 无
//  */
//void SPI2_ADC_config(void)
//{
//	SPI_InitTypeDef  SPI_InitStructure;
//  GPIO_InitTypeDef GPIO_InitStructure;
//	EXTI_InitTypeDef EXTI_InitStructure;
////	DMA_InitTypeDef DMA_InitStructure;  	
//	NVIC_InitTypeDef NVIC_InitStructure;  
//	
//	/*开时钟*/
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
////	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	
//	
//	GPIO_SetBits(GPIOB, GPIO_Pin_12);//拉高CS电平
//	
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
//	EXTI_InitStructure.EXTI_Line = EXTI_Line14;  
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //或许可以改成EXTI_Trigger_Rising_Falling？
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;  
//	EXTI->IMR &=~EXTI_Line14;  //屏蔽中断  	
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
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//MISO，或许改成
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
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //PPP外部中断线  
////	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
//	
////	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//优先级不可高于半控  

//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
//	NVIC_Init(&NVIC_InitStructure);  
//		
//	EXTI->PR = EXTI_Line14;//清中断
//	EXTI->IMR |= EXTI_Line14;//开放中断
//	GPIO_ResetBits(GPIOB, GPIO_Pin_12);//最后拉低CS电平，选中芯片
//	SysTick_Delay_Ms(50);
//	EXTI_GenerateSWInterrupt(EXTI_Line14);//软件触发中断一次
//	
//}
//	u8 adc_count;
////MISO下降沿中断函数
//void EXTI15_10_IT(void)  //7.8MS中断
//{ 
//	
//    if ((EXTI->PR & EXTI_Line14) != RESET)//判断中断
//    {
//			EXTI->PR = EXTI_Line14;						//清除中断标志位
//			EXTI->IMR &=~EXTI_Line14;         //屏蔽EXTI中断
//			
//			switch(ADC_CH)
//			{
//				case 0://发送测量Vint指令，返回Vext数据
//					Register_Vext[Vext_Number] = SPI2_ADC_Transfer(SPI2_Tx[0]);//发送测量指令，返回上次的AD数据
//				  Vext_Number++;
//					SPI2_ADC_Transfer(SPI2_Tx[0]);//发送回读配置寄存器值
////					if(Vext_Number >= ADC_Average_Number) 				Vext_Number = 0;
//					ADC_CH++;	
//					break;

//				case 1://发送测量I指令，返回Vint数据
//					Register_Vint[Vint_Number] = SPI2_ADC_Transfer(SPI2_Tx[1]);
//				  Vint_Number++;

//					SPI2_ADC_Transfer(SPI2_Tx[1]);
////				if(Vint_Number >= ADC_Average_Number) 				Vint_Number = 0;

//					ADC_CH++;	
//				break;
//				
//				case 2://发送测量P指令，返回I数据
//					Register_I[I_Number] = SPI2_ADC_Transfer(SPI2_Tx[2]);
//				  I_Number++;
////					adc_count ++;
//					SPI2_ADC_Transfer(SPI2_Tx[2]);
////				if(I_Number >= ADC_Average_Number) 				I_Number = 0;
//					ADC_CH++;
//				break;
//				
//				case 3://发送测量Vext指令，返回P数据
//					Register_P[P_Number] = SPI2_ADC_Transfer(SPI2_Tx[3]);
//				  P_Number++;
//					adc_count ++;
//					SPI2_ADC_Transfer(SPI2_Tx[3]);
////				if(I_Number >= ADC_Average_Number) 				I_Number = 0;
//					ADC_CH = 0;	//开始新的测量周期
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
//			EXTI->IMR |= EXTI_Line14;//开放中断
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
//	for(z=0;z<4;z++)//数据求和
//	{
//		Value_Vint 	+= Register_Vint[z];
//		Value_I 		+= Register_I[z];
//		Value_P 		+= Register_P[z];
//		Value_Vext 	+= Register_Vext[z];
//	}
//	Value_Vint >>= 2;			//除以4取平均值
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
//	Value_Vint = 37157 * (u16)Value_Vint + 353990;	//计算Vint的电压
//	if(Value_Vint & 0x80000000) Vint = 0;
//	else 
//		Vint = Value_Vint/10000000.0;

//	Value_I = 47 * (u16)Value_I - 5091;					//计算电流
////	Value_I = 47 * (u16)Value_I + 3000;					//计算电流
//	if(Value_I & 0x80000000)Current = 0;//舍去负值
//	else 
//		Current = Value_I / (10000.0/0.995);
//	
//	
////			I_Compare();

//	Value_Vext = 39 * (u16)Value_Vext - 2671;			//计算Vext的电压
//	if(Value_Vext > 1000000) Vext = 0;
//	else 
//		Vext = Value_Vext / 10000.0;
//	
////		Vint = 0.0037157 * (u16)Value_Vint + 0.0353990;	//计算Vint的电压

////		Current = (0.0047 * 0.995) * (u16)Value_I - (0.5091 * 0.995);					//计算电流

////		Vext = 0.0039 * (u16)Value_Vext - 0.2671;			//计算Vext的电压

//	if(Use_Vint_Or_Vext_Flag == Use_Vint) Voltage = Vint;
//	else if(Use_Vint_Or_Vext_Flag == Use_Vext) Voltage = Vext;

//	if(Value_P < 570)  //100W以下
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

u16 SPI2_Tx[8]={0xF1CB,0xF1CB,0xC1CB,0xC1CB,0xD1CB,0xD1CB,0xE1CB,0xE1CB};      //0xC1AB 开启一次单次传输，用A0 GND（补偿电压）,FS=6.144V,一次传输模式，传输速度250SPS，ADC模式，引脚上拉，更新寄存器，
                                                                               //0xD1AB 用A1,GND（功率）
                                                                               //0xE1AB 用A2,GND（电流）
                                                                               //0xF1AB 用A3,GND（电压）
																																							 //0xF18B 用A3, 475SPS，2.1ms转换一个数据
//u16 SPI2_Tx[8]={0xF18B,0xF18B,0xC18B,0xC18B,0xD18B,0xD18B,0xE18B,0xE18B};      //0xC1AB 开启一次单次传输，用A0 GND（补偿电压）电流 ,FS=6.144V,一次传输模式，传输速度250SPS，ADC模式，引脚上拉，更新寄存器，
                                                                               //0xD1AB 用A1,GND（功率）
//电压 电流 功率 电压补偿                                                      //0xE1AB 用A2,GND（电流） 电压补偿
                                                                               //0xF1AB 用A3,GND（电压）
																																							 //0xF18B 用A3, 128SPS，7.8ms转换一个数据

//static const u16 ADC_Array_Number = ADC_Average_Number * 2;           //定义收数据数组的大小                   
int16_t Register_I[ADC_Average_Number], Register_P[ADC_Average_Number], Register_Vext[ADC_Average_Number], Register_Vint[ADC_Average_Number];    //定义收数据的数组
static u16 I_Number = 0;     //定义收回来的数应该放在数组的哪个地方
static u16 P_Number = 0;
static u16 Vext_Number = 0;
static u16 Vint_Number = 0;

static __IO uint32_t  SPI2Timeout = SPIT_LONG_TIMEOUT; 


int16_t CH0[2],CH1[2],CH2[2],CH3[2];     //电流，功率，补偿电压，内部电压

uint8_t ADC_CH;


//SPI2的通讯程序
int16_t SPI2_ADC_Transfer(int16_t halfword)
{
	 SPI2Timeout = SPIT_FLAG_TIMEOUT;
  /* 等待发送缓冲区为空，TXE事件 */
  while (SPI_I2S_GetFlagStatus(SPI2 , SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) 
		{
		  return 0;			
		}
  }

  /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
  SPI_I2S_SendData(SPI2 ,halfword);

	SPITimeout = SPIT_FLAG_TIMEOUT;
  /* 等待接收缓冲区非空，RXNE事件 */
  while (SPI_I2S_GetFlagStatus(SPI2 , SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((SPITimeout--) == 0)
		{
			return 1;
		}			
   }
	
  /* 读取数据寄存器，获取接收缓冲区数据 */
  return SPI_I2S_ReceiveData(SPI2);
}

/**
  * @brief  根据寄存器的值计算内部电压
  * @param  寄存器的值
  * @retval 内部电压
  */
static float Calculate_Vint(int temp)
{
	int Vint = 0;
	
//  Vint = 0.00364 * temp + 0.446;
//	Vint = 0.99*(0.0038*temp+0.1248);      //CV模式 ，空载
//	Vint = 0.999*(0.0037*temp+0.0275)-0.015; //CC模式测的
//	  Vint = 1.006 *(0.0037 * temp + 0.7658);   //CP模式测的
//	  Vint = 0.991 * (0.0038 * temp - 0.0532);    //CV空载  200欧
//				Vint =  temp*80000/21333;    //CV   1欧

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
  * @brief  根据寄存器的值计算电流
  * @param  寄存器的值
  * @retval 电流值
  */
static float Calculate_I(int temp)
{
	int I;
	
//	I = (float)0.0001875 * temp + 0.08;
//	I = (float) I * 25;
//	I = 0.994*(0.0047*temp-0.3227)-0.02;	
//	  I = 0.0046676*temp - 0.3195166;
//	  I = 1.01*(0.0047*temp-0.2814);         //CC模式
//	  I = 0.9966 * (0.0048 * temp + 0.555);       //CP模式
//	  I = 0.9915 * (0.0048 * temp - 0.373);     //CC模式   1欧     
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
  * @brief  根据寄存器的值计算功率
  * @param  寄存器的值
  * @retval 功率
  */
static float Calculate_P(int temp)
{
	int P;
	
//	P = (float)0.0001875 * temp + 0.08;
//	P = (float)0.2799 * temp + 1.7507;
//	  if(temp <= -150)     //10W以下
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
	
			if(temp < 570)  //100W以下
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
  * @brief  根据寄存器的值计算补偿电压
  * @param  寄存器的值
  * @retval 补偿电压
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
  * @brief  SPI2初始化
  * @param  无
  * @retval 无
  */
void SPI2_ADC_config(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
//	DMA_InitTypeDef DMA_InitStructure;  	
	NVIC_InitTypeDef NVIC_InitStructure;  
	
	/*开时钟*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	
	
	GPIO_SetBits(GPIOB, GPIO_Pin_12);//拉高CS电平
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);
	EXTI_InitStructure.EXTI_Line = EXTI_Line14;  
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;  
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //或许可以改成EXTI_Trigger_Rising_Falling？
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;  
	EXTI->IMR &=~EXTI_Line14;  //屏蔽中断  	
	EXTI_Init(&EXTI_InitStructure); 
	
	/*SCK MOSI MISO*/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;//SCK
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//MOSI
	GPIO_Init(GPIOB, &GPIO_InitStructure);  
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;//MISO，或许改成
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
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //PPP外部中断线  
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
	
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//优先级不可高于半控  

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  
		
	EXTI->PR = EXTI_Line14;//清中断
	EXTI->IMR |= EXTI_Line14;//开放中断
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);//最后拉低CS电平，选中芯片
	SysTick_Delay_Ms(50);
	EXTI_GenerateSWInterrupt(EXTI_Line14);//软件触发中断一次
	
}
	u8 adc_count;
//MISO下降沿中断函数
void EXTI15_10_IT(void)  
{ 
	
    if ((EXTI->PR & EXTI_Line14) != RESET)//判断中断
    {
			EXTI->PR = EXTI_Line14;						//清除中断标志位
			EXTI->IMR &=~EXTI_Line14;         //屏蔽EXTI中断
			
			switch(ADC_CH)
			{
				case 0://发送测量Vint指令，返回Vext数据
					Register_Vext[Vext_Number] = SPI2_ADC_Transfer(SPI2_Tx[0]);//发送测量指令，返回上次的AD数据
				  Vext_Number++;
					adc_count ++;

				SPI2_ADC_Transfer(SPI2_Tx[0]);//发送回读配置寄存器值
					if(Vext_Number >= ADC_Average_Number)Vext_Number = 0;
					ADC_CH++;	
					break;

				case 1://发送测量I指令，返回Vint数据
					Register_Vint[Vint_Number] = SPI2_ADC_Transfer(SPI2_Tx[2]);
				  Vint_Number++;

					SPI2_ADC_Transfer(SPI2_Tx[2]);
				if(Vint_Number >= ADC_Average_Number)Vint_Number = 0;
					ADC_CH++;	
				break;
				
				case 2://发送测量Vext指令，返回I数据
					Register_I[I_Number] = SPI2_ADC_Transfer(SPI2_Tx[6]);
				  I_Number++;

					SPI2_ADC_Transfer(SPI2_Tx[6]);
					if(I_Number >= ADC_Average_Number)I_Number = 0;
//					ADC_CH++;	
					ADC_CH = 0;	//开始新的测量周期
				break;
				
				case 3://发送测量Vext指令，返回P数据
					Register_P[P_Number] = SPI2_ADC_Transfer(SPI2_Tx[6]);
				  P_Number++;

					SPI2_ADC_Transfer(SPI2_Tx[6]);
				if(P_Number >= ADC_Average_Number)P_Number = 0;
					ADC_CH = 0;	//开始新的测量周期
				break;
			}
			if(adc_count > 25)
			{
				adc_count = 0;
				Average_Calculate();//计算平均值 
			}				
			EXTI->IMR |= EXTI_Line14;//开放中断
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

/*冒泡排序计算平均值子程序*/
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
			if(buff_ADC[j] > buff_ADC[j+1])		//如果前者大于后者
			{
				int16_t result = buff_ADC[j];//则交换两者的值
				buff_ADC[j] = buff_ADC[j+1];
				buff_ADC[j+1] = result;
			}
		}
	}
	
	
	//32个数据，去掉首尾8个，取16个数据
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
 
//计算平均值程序
void Average_Calculate(void)
{
//			if(Vint_Number >= ADC_Average_Number)      //计算内部电压
//			{
				int Value_Vint;
				int Value_I;
				int Value_P;
				int Value_Vext;
				
//				Value_Vint = Calculate(Register_Vint,ADC_Average_Number);//直接求平均
				Value_Vint = Bubble_Sort_Calculate(Register_Vint,ADC_Average_Number);//冒泡计算

				ADC_Vint = Value_Vint;//赋值给观察变量
				
				Vint = Calculate_Vint(Value_Vint);    //计算Vintf的电压
			
				if(Use_Vint_Or_Vext_Flag == Use_Vint)
				{
					Voltage = Vint;
				}
			
				if((Wait_Below_Set_Value_Flag == Havent_Below) && (Voltage < 1) && (ON_OFF == ON))
				{
					Wait_Below_Set_Value_Flag = Already_Below;	
					Power_On();
				}
				

		
//				Vint_Number = 0;   //重置计数
//			}
			
			
//			if(I_Number >= ADC_Average_Number)    //计算电流
//			{
//				
//				float Value_I;
				
				Value_I = Calculate(Register_I,ADC_Average_Number);//直接求平均
//				Value_I = Bubble_Sort_Calculate(Register_I,ADC_Average_Number);//冒泡计算
				
				ADC_I = Value_I;//观察变量
				Current = Calculate_I(Value_I);
					
//				I_Number = 0;
			
				if((Wait_Below_Set_Value_Flag == Havent_Below) && (Current < 1) && (ON_OFF == ON) && (Use_Vint_Or_Vext_Flag == Use_Vint))
				{
					Wait_Below_Set_Value_Flag = Already_Below;
					Power_On();
				}
//			}



//		if(P_Number > ADC_Average_Number)    //计算功率
//		{
//			float Value_P;
			Power = Voltage * Current;
//			Value_P=Calculate(Register_P,ADC_Average_Number);//直接求平均
//			Value_P=Bubble_Sort_Calculate(Register_P,ADC_Average_Number);//冒泡计算
//			Power = Calculate_P(Value_P);		
			SCR_Ser(Power);//用半控
			ADC_P = Value_P;//观察变量
//			P_Number = 0;
//			
			if((Wait_Below_Set_Value_Flag == Havent_Below) && (Power < 50) && (ON_OFF == ON))
			{
				Wait_Below_Set_Value_Flag = Already_Below;
				
				Power_On();
			}
//		}
		
		
		
		
//		if(Vext_Number > ADC_Average_Number)    //计算补偿电压
//		{
//				float Value_Vext;
			
//				Value_Vext=Calculate(Register_Vext,ADC_Average_Number);//直接求平均
				Value_Vext=Bubble_Sort_Calculate(Register_Vext,ADC_Average_Number);//冒泡计算
				Vext = Calculate_Vext(Value_Vext);
			
				ADC_Vext = Value_Vext;//观察变量
				
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









































 


