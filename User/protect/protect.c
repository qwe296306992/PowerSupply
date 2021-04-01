#include "protect.h"
#include "softstart.h"
#include "SCR_Controller.h"


u32 TIM_PROTECT_time; 
int CRL_Voltage;        //定义过欠压变量
u8 adcs=0;//

//设置区域存放数据
__IO uint16_t ADC_ConvertedValue[ADC_Buffer_Size]={0};

u8 OVP_InputOrOutput_Flag = 2;   //0代表输入OVP，1代表输出OVP，2代表无报警
u8 OCP_SoftOrHard_Flag = 2;//0代表软件OCP，1代表硬件OCP，2代表无报警
u8 OTP_SensorX = 0;				 //1代表Sensor1报警，2代表Sensor2报警，3代表Sensor3报警，0代表无报警

//按键使能控制
FunctionalState keys_EN = DISABLE;

/*秋金写，PD11，检测输出过流OC1*/
void TestCurrent_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
}


//配置中断优先级
static void TIM_PROTECT_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	
	// 设置中断组为0
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
	// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	
	// 设置主优先级为 1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 
	// 设置抢占优先级为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	// 设置抢占优先级为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_Init(&NVIC_InitStructure);	

//	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//PD11硬件过流保护中断
//  NVIC_Init(&NVIC_InitStructure);	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;	 
	// 设置抢占优先级为3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
  NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel1_IRQn;
  NVIC_Init(&NVIC_InitStructure);	
}

//配置TIM4,0.1s定时时间
static void TIM_PROTECT_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;		
	  // 开启定时器时钟,即内部时钟CK_INT=72M
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period=999;
	  // 时钟预分频数
    TIM_TimeBaseStructure.TIM_Prescaler= 7199;		
		// 时钟分频因子 
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		// 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	
		// 重复计数器的值
		TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	  // 初始化定时器
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
		// 清除计数器中断标志位
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(TIM4, TIM_IT_Update,ENABLE);
		
		// 使能计数器
    TIM_Cmd(TIM4, ENABLE);
}

static void EXTI_PROTECT_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/*开启按键GPIO口的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
												
	
/*--------------------------OC1配置-----------------------------*/
//	/* 选择按键用到的GPIO */	
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//  /* 配置为浮空输入 */	
////  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	
//  GPIO_Init(GPIOD, &GPIO_InitStructure);

//	/* 选择EXTI的信号源 */
//  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource11); 
//  EXTI_InitStructure.EXTI_Line = EXTI_Line11;
//	
//	/* EXTI为中断模式 */
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	/* 下降沿沿中断 */
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//  /* 使能中断 */	
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);
	
  /*--------------------------OC2配置-----------------------------*/
	/* 选择按键用到的GPIO */	
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//  /* 配置为浮空输入 */	
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//  GPIO_Init(GPIOD, &GPIO_InitStructure);

//	/* 选择EXTI的信号源 */
//  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource10); 
//  EXTI_InitStructure.EXTI_Line = EXTI_Line10;

//	/* EXTI为中断模式 */
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//	/* 下降沿中断 */
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//  /* 使能中断 */	
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);	
	
  /*--------------------------Sensor_1配置-----------------------------*/
	/* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  /* 配置为浮空输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line5;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 上升沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);		
	
  /*--------------------------Sensor_2配置-----------------------------*/
	/* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  /* 配置为浮空输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 上升沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);		
	
  /*--------------------------Sensor_3配置-----------------------------*/
	/* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  /* 配置为浮空输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line7;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 上升沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);		
}

static void ADC_PROTECT_Config(void)
{
	//PC5
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;	
	
	// 打开 ADC IO端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE );
	
	// 配置 ADC IO 引脚模式
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	
	// 初始化 ADC IO
	GPIO_Init(GPIOC, &GPIO_InitStructure);		

	//ADC1通道15，DMA1通道1
	
	// 打开DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	// 打开ADC时钟
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1, ENABLE );
	
	// 复位DMA控制器
	DMA_DeInit(DMA1_Channel1);
	
	// 配置 DMA 初始化结构体
	// 外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( u32 ) ( & ( ADC1->DR ) );
	
	// 存储器地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADC_ConvertedValue;
	
	// 数据源来自外设
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	// 缓冲区大小，应该等于数据目的地的大小
	DMA_InitStructure.DMA_BufferSize = ADC_Buffer_Size;
	
	// 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	
	// 存储器地址递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	
	// 外设数据大小为半字，即两个字节
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	// 内存数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	// 循环传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
	
	// DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	// 禁止存储器到存储器模式，因为是从外设到存储器
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	// 初始化DMA
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	//使能中断
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
	
	// 使能 DMA 通道
	DMA_Cmd(DMA1_Channel1 , ENABLE);
		
	//ADC 模式配置
	//只使用一个ADC，属于单模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	//扫描模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 
	
	//连续转换模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	
	//不用外部触发转换，软件开启即可
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	
	//转换结果右对齐
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	//转换通道个数
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
	
	//初始化ADC
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//配置ADC时钟Ｎ狿CLK2的8分频，即9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	//配置ADC 通道的转换顺序和采样时间,  转换一个数要26.6us
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_239Cycles5);  
	
	//使能ADC DMA 请求
	ADC_DMACmd(ADC1, ENABLE);
	
	//开启ADC ，并开始转换
	ADC_Cmd(ADC1, ENABLE);
	
    //初始化ADC 校准寄存器  
	ADC_ResetCalibration(ADC1);
    //等待校准寄存器初始化完成
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	//ADC开始校准
	ADC_StartCalibration(ADC1);
	//等待校准完成
	while(ADC_GetCalibrationStatus(ADC1));
	
	//由于没有采用外部触发，所以使用软件触发ADC转换 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	
}




//保护功能各种初始化
void TIM_PROTECT_Init(void)
{
	TIM_PROTECT_Mode_Config();//软件保护的定时器0.1s设置
	TIM_PROTECT_NVIC_Config();//软件保护定时器，各种硬件中断以及DMA优先级初始化
	EXTI_PROTECT_Config();//外部中断保护初始化，如OC1过流硬件保护，Sensor1_2_3过温保护
	ADC_PROTECT_Config();//输入过欠压初始化
	
}


/**
  * @brief  TIM4的中断服务函数,用于检测功率和电压和电流,同时用于软启动，中断时间为0.1s，软开关时间为3s
  * @param  None
  * @retval None
  */
u16 softtime = 60;//软启动定时时间

void TIM_P_Protect_IT(void)
{	
	if(softtime > 0)//软启动控制
	{
		softtime --;
//		if(softtime == 31) t_count = 5;//9500-5*2*30*5 = 8000  9500-10*70-30*5*10 =7300
		
				//if(softtime == 31) t_count = 12;//9500-12*2*30*5 = 8000  9500-70*10-12*2*30*5 = 5200     不需要t_count

//		else if(softtime == 10) t_count = 33;
		if(softtime == 0)
		{
			keys_EN = ENABLE;//ENABLE按键
			t_count = 50;
			SCR_On_Off(ENABLE);
//			SCR_On_Off(DISABLE);
			Power_AC_On();
		}
	}
  
	if(Power > 6100)//实验室测试最大2500W，16A插座为3500W
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
	
	if(Current > 105)//实验室最大测试40A,16A插座为60A
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
	TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);   //清除中断标志位	
}

/**
  * @brief  EXTI11的中断服务函数,用于电流保护(OC1)，不用
  * @param  None
  * @retval None
  */
//void EXTI11_I_Protect_IT(void)
//{
//	
//  //确保是否产生了EXTI Line中断
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
//		OCP_SoftOrHard_Flag = 1;//硬件OCP报错
//		
//		adcs++;
//		
//		Display_Interface_Mode = Display_Interface_Mode_Protect_Interface;
//		
//		//清除中断标志位
//	  EXTI_ClearITPendingBit(EXTI_Line11); 	
//	}
//}

/**
  * @brief  EXTI10的中断服务函数,用于电流保护
  * @param  None
  * @retval None
  */
//void EXTI10_I_Protect_IT(void)
//{
//  //确保是否产生了EXTI Line中断
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

//    //清除中断标志位
//	  EXTI_ClearITPendingBit(EXTI_Line10); 		
//	}	
//}

/**
  * @brief  EXTI5的中断服务函数,用于Sensor1温度保护(IGBT模块过温保护)
  * @param  None
  * @retval None
  */
void EXTI5_Temperature_Protect_IT(void)
{
  //确保是否产生了EXTI Line中断
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

			OTP_SensorX = 1;//Sensor1报警
			
			Display_Interface_Mode = Display_Interface_Mode_Protect_Interface;    
			
			//清除中断标志位
			EXTI_ClearITPendingBit(EXTI_Line5);
		}
	} 	
}

/**
  * @brief  EXTI6的中断服务函数,用于Sensor2温度保护(用于输出模块过温保护)
  * @param  None
  * @retval None
  */
void EXTI6_Temperature_Protect_IT(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(EXTI_Line6) != RESET) 
	{
		ON_OFF = OFF;
		OUTPUT_NOT_OUTPUT = NOT_OUTPUT;
		
		Output_Zero();				
		
		TIM_DELAY_OFF();
		TIM_CYCLE_OFF();
		
		Error |= OTP;		
		
		OTP_SensorX = 2;//Sensor2报警
		
		Display_Interface_Mode = Display_Interface_Mode_Protect_Interface; 

    //清除中断标志位
	  EXTI_ClearITPendingBit(EXTI_Line6); 		
	}	
}

/**
  * @brief  EXTI7的中断服务函数,用于温度保护，硬件短接，没有用到，用在反接保护
  * @param  None
  * @retval None
  */
void EXTI7_Temperature_Protect_IT(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(EXTI_Line7) != RESET) 
	{
		ON_OFF = OFF;
		OUTPUT_NOT_OUTPUT = NOT_OUTPUT;
		
		Output_Zero();				
		
		TIM_DELAY_OFF();
		TIM_CYCLE_OFF();
		
		Error |= Sense_Reverse;		
		
//		OTP_SensorX = 3;//Sensor3报警
		
		Display_Interface_Mode = Display_Interface_Mode_Protect_Interface;  

    //清除中断标志位
	  EXTI_ClearITPendingBit(EXTI_Line7); 			
	}
}


/**
  * @brief  DMA的中断服务函数,用于过欠压检测
  * @param  None
  * @retval None
  */
void ADC_V_Protect_IT(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC1))
  {
	  int ADC_Voltage;                     //定义控制电路电压的ADC12位值
	  int temp_Voltage=0;             //定义控制电路电压ADC检测值的累加值
	
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
		
//原始数据2.55		
		//输入220V,得到1.63V，ADC值算得1630/3300*4095=2022，取±12%，得1780-2265
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
  * @brief  检查温度开关是否接上，由于电流OC1开关状态未知，所以暂时不检测
  * @param  None
  * @retval None
  */
void Check_T_I_Switch(void)
{
	if( (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 1) || (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 1) || (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 1) )    //没有接温度开关
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
  * @brief  硬件检测OCP状态，这里采用的是轮询方式
  * @param  None
  * @retval None
  */
void TestCurrent (void)
{
//	TIM_Cmd(TIM1, DISABLE);//关闭功率缓升定时器
	EXTI->IMR &=~EXTI_Line14;//屏蔽MISO下降沿中断
	TIM_Cmd(TIM4, DISABLE);//暂时关闭0.1s软保护定时器	
	TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update); //清除中断标志位
	ADC_DMACmd(ADC1, DISABLE);//关闭adc过欠压中断
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
			
		OCP_SoftOrHard_Flag = 1;//硬件OCP报错
			
		Display_Interface_Mode = Display_Interface_Mode_Protect_Interface;
		}
	}	

	ADC_DMACmd(ADC1, ENABLE);//恢复adc过欠压中断
	TIM_Cmd(TIM4, ENABLE);//恢复0.1s软保护定时器
	EXTI->IMR |= EXTI_Line14;//恢复MISO下降沿中断
	
	
//	if(Error_Stop_Flag == 1)//关闭之前TIM1定时器为开启状态则恢复
//	{
//		TIM_Cmd(TIM1, ENABLE);//恢复功率缓升定时器
//	}
	
}





