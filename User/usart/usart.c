

#include "stm32f10x.h"
#include "usart.h"
#include "spi_oled.h"

uint8_t USART2_Num;//USART接收字节数
uint8_t USART2_Send_Data[20];  //发送数据数组
uint8_t USART2_Receive_Data[20]; //接收数据数组
uint8_t USART2_Need_To_Send_Num;//需发送数据字节数
uint16_t CrcValue;//校验码

/**
  * @brief  USART_DMA初始化设置
  * @param  无
  * @retval 无
  */
static void USART2_DMA1_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_DeInit(DMA1_Channel7);//u2tx
	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)(&USART2->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&USART2_Send_Data;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 20;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
	DMA_Init(DMA1_Channel7, &DMA_InitStructure); 
	DMA_Cmd(DMA1_Channel7, ENABLE);
	
	DMA_DeInit(DMA1_Channel6);
	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)(&USART2->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&USART2_Receive_Data;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 20;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; 
	DMA_Init(DMA1_Channel6, &DMA_InitStructure); 
	DMA_Cmd(DMA1_Channel6, ENABLE);
}

/**
  * @brief  USART_接受中断初始化设置
  * @param  无
  * @retval 无
  */
static void USART2_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

/**
  * @brief  USART初始化设置
  * @param  BraudRate - 波特率
  * @retval 无
  */
void Usart2_Config(uint32_t BraudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	//使能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE); 
	
	//USART2_TX   PD5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//
	GPIO_Init(GPIOD, &GPIO_InitStructure);//

	//USART2_RX	  PD6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING;//
	GPIO_Init(GPIOD, &GPIO_InitStructure);//  

	//USART2_RE/DE   PD4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//
	GPIO_Init(GPIOD,&GPIO_InitStructure);//
	

	USART_InitStructure .USART_BaudRate =  BraudRate;//设置波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure); 	
	
	USART2_NVIC_Config ();
	
//检测到空闲电路中断使能	
	USART_ITConfig(USART2,USART_IT_TC,ENABLE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	
//使能USART的DMA发送/接受请求	
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE); 
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);	
	
	
//	// 使能串口接收中断
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	

	USART_Cmd(USART2,ENABLE);
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_4);
	
	USART2_DMA1_Config();

}



const unsigned char auchCRCHi[]=
{
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
        0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
        0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40
};
const unsigned char auchCRCLo[] = 
{ 
        0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
        0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
        0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
        0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
        0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
        0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
        0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
        0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
        0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
        0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
        0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
        0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
        0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
        0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
        0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
        0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
        0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
        0x40
}; 
u16 CRC16(u8 *puchMsg,u8 usDataLen )
{
		unsigned char uchCRCHi=0xFF; 
		unsigned char uchCRCLo=0xFF; 
		unsigned uIndex;
		while(usDataLen--) 
		{
			uIndex =uchCRCLo^*puchMsg++; 
			uchCRCLo=uchCRCHi^auchCRCHi[uIndex];
			uchCRCHi=auchCRCLo[uIndex];
		}
		return (uchCRCHi<<8|uchCRCLo);
}  

/**
  * @brief  USART发送报文
  * @param  无
  * @retval 无
  */
void USART2_Send(void)
{
	GPIO_SetBits(GPIOD,GPIO_Pin_4);//USART2_DE
	DMA_Cmd(DMA1_Channel7,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel7,USART2_Need_To_Send_Num);
	DMA_Cmd(DMA1_Channel7,ENABLE);
	USART_ClearFlag(USART2,USART_IT_TC);
	USART_ITConfig(USART2,USART_IT_TC,ENABLE);	
}


/**
  * @brief  CRC 16位转8位
  * @param  X-16位数据   *t-8位数据地址
  * @retval 无
  */
void u16to8(u16 x,u8 *t)
{
	*(t+1)=x;
	*t=x>>8;
}


/**
  * @brief  输出控制命令：0x20
  * @param  无
  * @retval 无
  */
static void function20(void)
{

	USART2_Send_Data [0]=(u8)RS232_ID;
	USART2_Send_Data [1]=0x20;
	USART2_Send_Data [2]=0x01;


	if(USART2_Receive_Data[3]==ON)
	{
		OUTPUT_NOT_OUTPUT = OUTPUT;
		if(Delay_Function_On_Off == Delay_Function_On)               //打开了延时功能
		{
			TIM_DELAY_ON();
		}
		else if(Cycle_Function_On_Off == Cycle_Function_On)          //打开了循环功能
		{
			TIM_CYCLE_ON();
		}
		else
		{
			Power_On();
		}
	}
	else
	{
		OUTPUT_NOT_OUTPUT = NOT_OUTPUT;
		TIM_DELAY_OFF();
		TIM_CYCLE_OFF();
		Power_Off();
	}
	USART2_Send_Data [3]=USART2_Receive_Data[3];
    
	CrcValue = CRC16( USART2_Send_Data, 4) ;
	USART2_Send_Data [4]=CrcValue&0xff;
	USART2_Send_Data [5]=((CrcValue>>8)&0xff);
	USART2_Need_To_Send_Num =6;
	USART2_Send ();		
}

/**
  * @brief  设定输出电压命令：0x21
  * @param  无
  * @retval 无
  */
static void function21(void)
{

	USART2_Send_Data [0]=(u8)RS232_ID;//本机ID
	USART2_Send_Data [1]=0x21;//指令类型
	USART2_Send_Data [2]=0x02;
	
	
	Set_Voltage=(USART2_Receive_Data[3]<<8|USART2_Receive_Data[4])/100.0;
	CV_CC_CP = CV;
	AT25_Save_VIPSet();
	AT25_Save_CC_CV_CP();
	
	
	USART2_Send_Data [3]=USART2_Receive_Data[3];
	USART2_Send_Data [4]=USART2_Receive_Data[4];
	

	CrcValue = CRC16( USART2_Send_Data, 5) ;
	USART2_Send_Data [5]=CrcValue&0xff;
	USART2_Send_Data [6]=(CrcValue>>8)&0xff;
	USART2_Need_To_Send_Num =7;	
	USART2_Send ();			


}


/**
  * @brief  设定输出电流命令：0x22
  * @param  无
  * @retval 无
  */
static void function22(void)
{

	USART2_Send_Data [0]=(u8)RS232_ID ;
	USART2_Send_Data [1]=0x22;
	USART2_Send_Data [2]=0x02;
	
	
	//立浩要改  这里设定电流
    Set_Current=(USART2_Receive_Data[3]<<8|USART2_Receive_Data[4])/100.0;
	CV_CC_CP = CC;
	AT25_Save_VIPSet();
	AT25_Save_CC_CV_CP();
	
	USART2_Send_Data [3]=USART2_Receive_Data[3];
	USART2_Send_Data [4]=USART2_Receive_Data[4];

	CrcValue = CRC16( USART2_Send_Data, 5) ;
	USART2_Send_Data [5]=CrcValue&0xff;
	USART2_Send_Data [6]=(CrcValue>>8)&0xff;
	USART2_Need_To_Send_Num =7;	
	USART2_Send ();	

}

/**
  * @brief  设定输出功率：0x23
  * @param  无
  * @retval 无
  */
static void function23(void)
{

	USART2_Send_Data [0]=(u8)RS232_ID ;
	USART2_Send_Data [1]=0x23;
	USART2_Send_Data [2]=0x02;
	
	//立浩要改  这里设定电压 电流
	Set_Power=(USART2_Receive_Data[3]<<8|USART2_Receive_Data[4]);
	CV_CC_CP = CP;
	AT25_Save_VIPSet();
	AT25_Save_CC_CV_CP();
	
	
	USART2_Send_Data [3]=USART2_Receive_Data[3];
	USART2_Send_Data [4]=USART2_Receive_Data[4];
	
//	USART2_Send_Data [5]=USART2_Receive_Data[5];
//	USART2_Send_Data [6]=USART2_Receive_Data[6];

//	CrcValue = CRC16( USART2_Send_Data, 7) ;
//	USART2_Send_Data [7]=CrcValue&0xff;//低字节在前
//	USART2_Send_Data [8]=(CrcValue>>8)&0xff;
//	USART2_Need_To_Send_Num =9;

	CrcValue = CRC16( USART2_Send_Data, 5) ;
	USART2_Send_Data [5]=CrcValue&0xff;//低字节在前
	USART2_Send_Data [6]=(CrcValue>>8)&0xff;
	USART2_Need_To_Send_Num =7;

	USART2_Send ();	

}


/**
  * @brief 读取电压及电流反馈数据：0x26
  * @param  无
  * @retval 无
  */
static void function26(void)
{

	USART2_Send_Data [0]=(u8)RS232_ID;
	USART2_Send_Data [1]=0x26;
	USART2_Send_Data [2]=0x04;
	USART2_Send_Data [3]=(u16)(Voltage*100)>>8;
	USART2_Send_Data [4]=(u16)(Voltage*100);
	USART2_Send_Data [5]=(u16)(Current*100)>>8;
	USART2_Send_Data [6]=(u16)(Current*100);

	CrcValue = CRC16( USART2_Send_Data,7);
	USART2_Send_Data [7]=CrcValue&0xff;
	USART2_Send_Data [8]=(CrcValue>>8)&0xff;
	USART2_Need_To_Send_Num =9;
	USART2_Send ();
	
}


/**
  * @brief  读取电压、电流、功率最大值数据：0x27
  * @param  无
  * @retval 无
  */
static void function27(void)
{
	USART2_Send_Data [0]=(u8)RS232_ID;
	USART2_Send_Data [1]=0x27;
	USART2_Send_Data [2]=0x06;
	
	USART2_Send_Data [3]=0x1f;
	USART2_Send_Data [4]=0x40;//8000
	USART2_Send_Data [5]=2;
	USART2_Send_Data [6]=0x27;
	USART2_Send_Data [7]=0x10;//10000
	USART2_Send_Data [8]=2;
	USART2_Send_Data [9]=0x17;
	USART2_Send_Data [10]=0x70;//6000
	USART2_Send_Data [11]=0;


	CrcValue = CRC16( USART2_Send_Data,12);
	USART2_Send_Data [12]=CrcValue&0xff;
	USART2_Send_Data [13]=(CrcValue>>8)&0xff;
	USART2_Need_To_Send_Num =14;
	USART2_Send ();
}


/**
  * @brief  读取电压及电流设定输出数据：0x28
  * @param  无
  * @retval 无
  */
static void function28(void)
{
	
	USART2_Send_Data [0]=(u8)RS232_ID;
	USART2_Send_Data [1]=0x28;
	USART2_Send_Data [2]=0x05;
	
	USART2_Send_Data [3]=ON_OFF;
	USART2_Send_Data [4]=(u16)(Set_Voltage*100)>>8;
	USART2_Send_Data [5]=(u16)(Set_Voltage*100);
	USART2_Send_Data [6]=(u16)(Set_Current*100)>>8;
	USART2_Send_Data [7]=(u16)(Set_Current*100);
	

	CrcValue =CRC16(USART2_Send_Data ,8);
	USART2_Send_Data [8]=CrcValue&0xff;
	USART2_Send_Data [9]=(CrcValue>>8)&0xff;		
	USART2_Need_To_Send_Num =10;
	USART2_Send ();	
}
	
/**
  * @brief  设定机器编号：0x29
  * @param  无
  * @retval 无
  */
static void function29(void)
{
	USART2_Send_Data[0]=(u8)RS232_ID ;
	USART2_Send_Data[1]=0x29;
	USART2_Send_Data[2]=0x01;
	
	RS232_ID = (u16)USART2_Receive_Data[3];
	USART2_Send_Data[3]=USART2_Receive_Data[3];

	CrcValue =CRC16 (USART2_Send_Data,4);
	USART2_Send_Data [4]=CrcValue&0xff;
	USART2_Send_Data [5]=(CrcValue>>8)&0xff;	
	USART2_Need_To_Send_Num =6;
	USART2_Send ();				
}

/**
  * @brief  读取电压及电流反馈数据：0x2a
  * @param  无
  * @retval 无
  */
static void function2a(void)
{
	USART2_Send_Data[0]=(u8)RS232_ID ;
	USART2_Send_Data[1]=0x2a;
	USART2_Send_Data[2]=0x05;
	
	USART2_Send_Data [3]=ON_OFF ;
	USART2_Send_Data [4]=(u16)(Voltage*100)>>8;
	USART2_Send_Data [5]=(u16)(Voltage*100);
	USART2_Send_Data [6]=(u16)(Current*100)>>8;
	USART2_Send_Data [7]=(u16)(Current*100);

	CrcValue =CRC16 (USART2_Send_Data,8);
	USART2_Send_Data [8]=CrcValue&0xff;
	USART2_Send_Data [9]=(CrcValue>>8)&0xff;	
	USART2_Need_To_Send_Num =10;
	USART2_Send ();				
}


///**
//  * @brief  使能线阻功能：0x2c
//  * @param  无
//  * @retval 无
//  */
//static void function2c(void)
//{
//	USART2_Send_Data[0]=(u8)RS232_ID ;
//	USART2_Send_Data[1]=0x2c;
//	USART2_Send_Data[2]=0x01;
//	
//	//立浩改使能线阻功能
//	if(USART2_Receive_Data[3]==0x01) WireRES_OnOFF=1;
//	else WireRES_OnOFF=0;
//	
//	USART2_Send_Data [3]=USART2_Receive_Data[3] ;

//	CrcValue =CRC16 (USART2_Send_Data,4);
//	USART2_Send_Data [4]=CrcValue&0xff;
//	USART2_Send_Data [5]=(CrcValue>>8)&0xff;	
//	USART2_Need_To_Send_Num =6;
//	USART2_Send ();				
//}


///**
//  * @brief  设置线阻阻值：0x2d
//  * @param  无
//  * @retval 无
//  */
//static void function2d(void)
//{
//	USART2_Send_Data[0]=(u8)RS232_ID ;
//	USART2_Send_Data[1]=0x2d;
//	USART2_Send_Data[2]=0x02;
//	
//	//立浩改设定线阻数值
//	WireRES=(USART2_Receive_Data[3]<<8|USART2_Receive_Data[4]);
//	
//	
//	USART2_Send_Data [3]=USART2_Receive_Data[3] ;
//	USART2_Send_Data [4]=USART2_Receive_Data[4] ;

//	CrcValue =CRC16 (USART2_Send_Data,5);
//	USART2_Send_Data [5]=CrcValue&0xff;
//	USART2_Send_Data [6]=(CrcValue>>8)&0xff;	
//	USART2_Need_To_Send_Num =7;
//	USART2_Send ();				
//}



/**
  * @brief  RS232报文处理
  * @param  无
  * @retval 无
  */
void USART2_Handler(void)
{
	
	if((USART2_Receive_Data [0] == (u8)RS232_ID ) || (USART2_Receive_Data [0] ==0)) 
	{
		CrcValue=((USART2_Receive_Data [USART2_Num-2])|((uint16_t)(USART2_Receive_Data[USART2_Num -1])<<8));
		if(CrcValue==CRC16(USART2_Receive_Data,USART2_Num-2 ))
		{
			switch(USART2_Receive_Data [1])
			{
				case 0x20:function20();break; 
				case 0x21:function21();break; 
				case 0x22:function22();break; 
				case 0x23:function23();break; 
				case 0x26:function26();break; 
				case 0x27:function27();break; 
				case 0x28:function28();break;
				case 0x29:function29();break;
				case 0x2a:function2a();break;
//				case 0x2c:function2c();break;
//				case 0x0d:function2d();break;
				default: break;
			}			
		}
		else return;

	}			
	else return;
	
}


/**
  * @brief  RS232接收中断
  * @param  无
  * @retval 无
  */
void RS232_IT(void)
{
    if(USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)  
	{
		USART_ReceiveData(USART2);
		USART_ClearFlag(USART2, USART_FLAG_ORE);
	}
	
	if(USART_GetITStatus(USART2, USART_IT_TC) == SET)
	{
		USART_ITConfig(USART2,USART_IT_TC,DISABLE);
		USART_ClearITPendingBit(USART2,USART_IT_TC);
		
		GPIO_ResetBits(GPIOD,GPIO_Pin_4);
	}
	
	if(USART_GetITStatus(USART2, USART_IT_IDLE) == SET)
	{
		u8 c;
		c=USART2->SR;
		c=USART2->DR;
        c = c;      //没用的，只是为了让编译器不报警告
		DMA_Cmd(DMA1_Channel6, DISABLE);
		USART2_Num=50-DMA_GetCurrDataCounter(DMA1_Channel6);
		DMA_SetCurrDataCounter(DMA1_Channel6,50);
		DMA_Cmd(DMA1_Channel6, ENABLE);//rx
		USART2_Handler();
		USART_ClearITPendingBit(USART2,USART_IT_IDLE);
	} 	
}
	







