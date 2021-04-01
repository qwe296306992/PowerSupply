#include "at25.h"

u8 Fist_Use_Device = 0;     //第一次使用设备的标志位

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    

/**
  * @brief  使用SPI发送一个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
//由于是全双工通讯，写数据的同时读数据，所以其实写和读数据都是用下面这个函数
u8 SPI_AT25_SendByte(u8 byte)
{
	 SPITimeout = SPIT_FLAG_TIMEOUT;
  /* 等待发送缓冲区为空，TXE事件 */
  while (SPI_I2S_GetFlagStatus(SPI3 , SPI_I2S_FLAG_TXE) == RESET)
	{
    if((SPITimeout--) == 0) 
		{
		  return 0;			
		}
  }

  /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
  SPI_I2S_SendData(SPI3 , byte);

	SPITimeout = SPIT_FLAG_TIMEOUT;
  /* 等待接收缓冲区非空，RXNE事件 */
  while (SPI_I2S_GetFlagStatus(SPI3 , SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((SPITimeout--) == 0)
		{
			return 1;
		}			
   }
	
  /* 读取数据寄存器，获取接收缓冲区数据 */
  return SPI_I2S_ReceiveData(SPI3);
}


 /**
  * @brief  使用SPI读取一个字节的数据
  * @param  无
  * @retval 返回接收到的数据
  */
u8 SPI_AT25_ReadByte(void)
{
  return (SPI_AT25_SendByte(0xff));
}


 /**
  * @brief  向FLASH发送 写使能 命令
  * @param  none
  * @retval none
  */
void SPI_AT25_WriteEnable(void)
{
	/* 通讯开始：CS低 */
  GPIO_ResetBits(GPIOD, GPIO_Pin_2);
	
	//关闭显示屏通信
	GPIO_SetBits(OLED_SPI_CS_PORT, OLED_SPI_CS_PIN);

  /* 发送写使能命令*/
  SPI_AT25_SendByte(AT25_WREN);

	/* 通讯结束：CS高 */
  GPIO_SetBits(GPIOD, GPIO_Pin_2);
	
	//打开显示屏通信
	GPIO_ResetBits(OLED_SPI_CS_PORT, OLED_SPI_CS_PIN);
}	

 /**
  * @brief  等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕
  * @param  none
  * @retval none
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
  u8 FLASH_Status = 0;

	/* 通讯开始：CS低 */
  GPIO_ResetBits(GPIOD, GPIO_Pin_2);
	
	//关闭显示屏通信
	GPIO_SetBits(OLED_SPI_CS_PORT, OLED_SPI_CS_PIN);

  /* 发送 读状态寄存器 命令 */
  SPI_AT25_SendByte(AT25_RDSR);

  /* 若FLASH忙碌，则等待 */
  do
  {
		/* 读取FLASH芯片的状态寄存器 */
    FLASH_Status = SPI_AT25_SendByte(0xff);	 
  }
  while ((FLASH_Status & 0x01) == SET);  /* 正在写入标志 */

	/* 通讯结束：CS高 */
  GPIO_SetBits(GPIOD, GPIO_Pin_2);
	
	//打开显示屏通信
	GPIO_ResetBits(OLED_SPI_CS_PORT, OLED_SPI_CS_PIN);
}


void SPI_AT25_Unlock(void)
{
	SPI_AT25_WriteEnable();
	SPI_FLASH_WaitForWriteEnd();
	
	/* 通讯开始：CS低 */
  GPIO_ResetBits(GPIOD, GPIO_Pin_2);
	
	//关闭显示屏通信
	GPIO_SetBits(OLED_SPI_CS_PORT, OLED_SPI_CS_PIN);
	
	/* 发送写使能命令*/
  SPI_AT25_SendByte(0x01);
	SPI_AT25_SendByte(0x00);
	
	/* 通讯开始：CS低 */
  GPIO_SetBits(GPIOD, GPIO_Pin_2);
	
	//关闭显示屏通信
	GPIO_ResetBits(OLED_SPI_CS_PORT, OLED_SPI_CS_PIN);
}








/*----------------------------------------------------------------------------------------------------------------------------------*/


void SPI_Sendbyte(u8 data)
{
	while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI3, data);
	while (SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_RXNE) == RESET);
	SPI_I2S_ReceiveData(SPI3);
}

u8 SPI_Readbyte(void)
{ 
while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET);
SPI_I2S_SendData(SPI3, 0); 
while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET); 
return SPI_I2S_ReceiveData(SPI3); 
//	SysTick_Delay_Ms(5);
}


void WREN(void)
{
	/* 通讯开始：CS低 */
  GPIO_ResetBits(GPIOD, GPIO_Pin_2);
	
	//关闭显示屏通信
	GPIO_SetBits(OLED_SPI_CS_PORT, OLED_SPI_CS_PIN);
	
	SPI_Sendbyte(0x06);//WREN
	
	/* 通讯结束：CS高 */
  GPIO_SetBits(GPIOD, GPIO_Pin_2);
	
	//打开显示屏通信
	GPIO_ResetBits(OLED_SPI_CS_PORT, OLED_SPI_CS_PIN);
}

void SPI_Save_Word(u16 data,u8 addr)
{
	WREN();
	
	/* 通讯开始：CS低 */
  GPIO_ResetBits(GPIOD, GPIO_Pin_2);
	
	//关闭显示屏通信
	GPIO_SetBits(OLED_SPI_CS_PORT, OLED_SPI_CS_PIN);
	
	SPI_Sendbyte(0x02);//w_code
	SPI_Sendbyte(addr);
	SPI_Sendbyte(data>>8);
	SPI_Sendbyte(data);
	
	/* 通讯结束：CS高 */
  GPIO_SetBits(GPIOD, GPIO_Pin_2);
	
	//打开显示屏通信
	GPIO_ResetBits(OLED_SPI_CS_PORT, OLED_SPI_CS_PIN);
	
	SysTick_Delay_Ms(5);
}

u16 SPI_Load_Word(u8 addr)
{
	u16 tmp;
	
	/* 通讯开始：CS低 */
  GPIO_ResetBits(GPIOD, GPIO_Pin_2);
	
	//关闭显示屏通信
	GPIO_SetBits(OLED_SPI_CS_PORT, OLED_SPI_CS_PIN);
	
	SPI_Sendbyte(0x03);//READ
	SPI_Sendbyte(addr);
	tmp=SPI_Readbyte()<<8;
	tmp|=SPI_Readbyte();
	
	/* 通讯结束：CS高 */
  GPIO_SetBits(GPIOD, GPIO_Pin_2);
	
	//打开显示屏通信
	GPIO_ResetBits(OLED_SPI_CS_PORT, OLED_SPI_CS_PIN);
	
	SysTick_Delay_Ms(5);
	return tmp;
}


 /**
  * @brief  对AT25写入数据
  * @param	pBuffer，要写入数据的指针
  * @param  WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度，必须小于等于SPI_FLASH_PerWritePageSize
  * @retval 无
  */
void SPI_AT25_Write(u16* pBuffer, u8 WriteAddr, u16 NumByteToWrite)
{
	NumByteToWrite = NumByteToWrite/2;
	
  while (NumByteToWrite > 0)
  {
		SPI_Save_Word(* pBuffer, WriteAddr);
		pBuffer ++;
		NumByteToWrite --;
		WriteAddr = WriteAddr + 2;
  }
}


 /**
  * @brief   读取AT25数据
  * @param   pBuffer，存储读出数据的指针
  * @param   ReadAddr，读取地址
  * @param   NumByteToRead，读取数据长度
  * @retval  无
  */
void SPI_AT25_Read(u16* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{
	NumByteToRead = NumByteToRead/2;	
	
	/* 读取数据 */
  while (NumByteToRead > 0) /* while there is data to be read */
  {
		* pBuffer = SPI_Load_Word(ReadAddr);
		pBuffer ++;
		NumByteToRead --;
		ReadAddr = ReadAddr + 2;
  }
}



/*---------------------------------------------------------------------------------------------------------------------*/

/*----AT25地址表-----
| 物理量 |   地址   |
|   V    |     0    |
|   I    |     2    |
|   P    |    124   |
---------------------
|    Recall_Save    |
| V[n]   |   6+6n   |
| I[n]   |   8+6n   |
| P[n]   |   10+6n  |
---------------------
|      RS232        |
|  id    |  66-67   |
| 波特率 |   68-71  |
---------------------
|       CAN         |
|  id    |  72-73   |
| 波特率 |   74-77  |
---------------------
|      Delay        |
|   h    |    78    |
|   m    |    79    |
|   s    |    80    |
|        |    90    |
---------------------
|      Cycle        |
| On_s   |    82    |
| On_ms  |    84    |
| Off_s  |    86    |
| Off_ms |    88    |
---------------------
|     CV_CC_CP      |
|CV_CC_CP|    90    |
|        |    91    |
---------------------
|     V-Rise        |
|   s    |   92-93  |
|   ms   |   94-95  |
---------------------
|     V-Fall        |
|   s    |   96-97  |
|   ms   |   98-99  |
---------------------
|     I-Rise        |
|   s    |  100-101 |
|   ms   |  102-103 |
---------------------
|     I-Fall        |
|   s    |  104-105 |
|   ms   |  106-107 |
-----------------------
|Delay_Function_On_Off|
|     108-109         |
-----------------------
|Cycle_Function_On_Off|
|     110-111         |
-----------------------
|   V_Rise_On_Off     |
|     112-113         |
-----------------------
|   V_Fall_On_Off     |
|     114-115         |
-----------------------
|   I_Rise_On_Off     |
|     116-117         |
-----------------------
|   I_Fall_On_Off     |
|     118-119         |
-----------------------
|   Fist_Use_Device   |
|      120-121        |
-----------------------
|Use_Vint_Or_Vext_Flag|
|      122-123        |
-----------------------
|         P           |
|      124-127        |
-----------------------*/

/*--------------------------------------------------保存--------------------------------------*/

 /**
  * @brief  对AT25写入电流电压功率设定值
  * @param	无
  * @retval 无
  */
void AT25_Save_VIPSet(void)
{
	u16 temp[2];
	u32 temp_1[1];
	
	//将设定值强制类型转换成u16
	temp[0] = (u16) (Set_Voltage * 100); 
	temp[1] = (u16) (Set_Current * 100);
	temp_1[0] = (u32) (Set_Power * 100);
	
	SPI_AT25_Write(temp, 0, sizeof(temp));
	SPI_AT25_Write((u16*)temp_1, 124, sizeof(temp_1));
}

 /**
  * @brief  对AT25写入Recall_Save的电流电压功率设定值
  * @param	Recall_Save的编号
  * @retval 无
  */
void AT25_Save_RecallSaveSet(u8 Number)
{
	u16 temp[3];
	
	//将设定值强制类型转换成u16
	temp[0] = (u16) Recall_Save_Voltage[Number] * 100; 
	temp[1] = (u16) Recall_Save_Current[Number] * 100;
	temp[2] = (u16) Recall_Save_Power[Number] * 100;
	
	SPI_AT25_Write(temp, 6+6*Number, sizeof(temp));
}

 /**
  * @brief  对AT25写入RS232设定值
  * @param	无
  * @retval 无
  */
void AT25_Save_RS232(void)
{
	u16 temp_ID[1];
	u32 temp_BaudRate[1];
	
	//保存id号
	temp_ID[0] = RS232_ID;
	SPI_AT25_Write(temp_ID, 66, sizeof(temp_ID));
	
	//保存波特率
	temp_BaudRate[0] = RS232_BaudRate;
	SPI_AT25_Write((u16*)temp_BaudRate, 68, sizeof(temp_BaudRate));	
}

 /**
  * @brief  对AT25写入CAN设定值
  * @param	无
  * @retval 无
  */
void AT25_Save_CAN(void)
{
	u16 temp_ID[1];
	u32 temp_BaudRate[1];
	
	//保存id号
	temp_ID[0] = CAN_ID;
	SPI_AT25_Write(temp_ID, 72, sizeof(temp_ID));
	
	//保存波特率
	temp_BaudRate[0] = CAN_BaudRate;
	SPI_AT25_Write((u16*)temp_BaudRate, 74, sizeof(CAN_BaudRate));	
}

 /**
  * @brief  对AT25写入延时时间设定值
  * @param	无
  * @retval 无
  */
void AT25_Save_Delay(void)
{
	u8 temp[4];
	u8 temp_1[2];
	
	temp[0] = Delay_h;
	temp[1] = Delay_m;
	temp[2] = Delay_s;	
	temp[3] = 0x00;
	
	temp_1[0] = Delay_Function_On_Off;
	temp_1[1] = 0x00;
	
	SPI_AT25_Write((u16*)temp, 78, sizeof(temp));	
	SPI_AT25_Write((u16*)temp_1, 108, sizeof(temp_1));	
}

 /**
  * @brief  对AT25写入循环时间设定值
  * @param	无
  * @retval 无
  */
void AT25_Save_Cycle(void)
{
	u16 temp[4];
	
	u8 temp_1[2];
	
	temp[0] = Cycle_On_s;
	temp[1] = Cycle_On_ms;
	temp[2] = Cycle_Close_s;	
	temp[3] = Cycle_Close_ms;
	
	temp_1[0] = Cycle_Function_On_Off;
	temp_1[1] = 0x00;
	
	SPI_AT25_Write(temp, 82, sizeof(temp));	
	SPI_AT25_Write((u16*)temp_1, 110, sizeof(temp_1));		
}

 /**
  * @brief  对AT25写入CC_CV_CP设定值
  * @param	无
  * @retval 无
  */
void AT25_Save_CC_CV_CP(void)
{
	u8 temp[2];
	
	temp[0] = CV_CC_CP;
	temp[1] = 0x00;
	
	SPI_AT25_Write((u16*)temp, 90, sizeof(temp));	
}

 /**
  * @brief  对AT25写入I_Fall设定值
  * @param	无
  * @retval 无
  */
void AT25_Save_V_Rise(void)
{
	u16 temp[2];
	u8 temp_1[2];
	
	temp[0] = V_Rise_s;
	temp[1] = V_Rise_ms;
	
	temp_1[0] = V_Rise_On_Off;
	temp_1[1] = 0x00;
	
	SPI_AT25_Write(temp, 92, sizeof(temp));	
  SPI_AT25_Write((u16*)temp_1, 112, sizeof(temp_1));	
}

 /**
  * @brief  对AT25写入I_Fall设定值
  * @param	无
  * @retval 无
  */
void AT25_Save_V_Fall(void)
{
	u16 temp[2];
	u8 temp_1[2];	
	
	temp[0] = V_Fall_s;
	temp[1] = V_Fall_ms;
	
	temp_1[0] = V_Fall_On_Off;
	temp_1[1] = 0x00;	
	
	SPI_AT25_Write(temp, 96, sizeof(temp));	
  SPI_AT25_Write((u16*)temp_1, 114, sizeof(temp_1));		
}

 /**
  * @brief  对AT25写入I_Fall设定值
  * @param	无
  * @retval 无
  */
void AT25_Save_I_Rise(void)
{
	u16 temp[2];
	u8 temp_1[2];		
	
	temp[0] = I_Rise_s;
	temp[1] = I_Rise_ms;
	
	temp_1[0] = I_Rise_On_Off;
	temp_1[1] = 0x00;		
	
	SPI_AT25_Write(temp, 100, sizeof(temp));	
  SPI_AT25_Write((u16*)temp_1, 116, sizeof(temp_1));	
}

 /**
  * @brief  对AT25写入I_Fall设定值
  * @param	无
  * @retval 无
  */
void AT25_Save_I_Fall(void)
{
	u16 temp[2];
	u8 temp_1[2];	
	
	temp[0] = I_Fall_s;
	temp[1] = I_Fall_ms;
	
	temp_1[0] = I_Fall_On_Off;
	temp_1[1] = 0x00;	
	
	SPI_AT25_Write((u16*)temp, 104, sizeof(temp));	
  SPI_AT25_Write((u16*)temp_1, 118, sizeof(temp_1));	
}

 /**
  * @brief  对AT25写入I_Fall设定值
  * @param	无
  * @retval 无
  */
void AT25_Save_Use_Vint_or_Vext_Flag(void)
{
	u16 temp[2];
	
	temp[0] = Use_Vint_Or_Vext_Flag;
	temp[1] = 0x00;	
	
	SPI_AT25_Write((u16*)temp, 122, sizeof(temp));		
}


 /**
  * @brief  对AT25重置所有设定值
  * @param	无
  * @retval 无
  */
void AT25_Reset(void)
{
	u16 temp[60] = {0};
	u32 temp_RS232_BaudRate[1] = {9600};
	u32 temp_CAN_BaudRate[1] = {100};	
	
	u16 temp_RS232_ID[1] = {1};
	u16 temp_CAN_ID[1] = {1};
	
	SPI_AT25_Write((u16*)temp, 0, sizeof(temp));

	SPI_AT25_Write(temp_RS232_ID, 66, sizeof(temp_RS232_ID));
	SPI_AT25_Write((u16*)temp_RS232_BaudRate, 68, sizeof(temp_RS232_BaudRate));	

	SPI_AT25_Write(temp_CAN_ID, 72, sizeof(temp_CAN_ID));
	SPI_AT25_Write((u16*)temp_CAN_BaudRate,74, sizeof(temp_CAN_BaudRate));	

  CV_CC_CP = CV;
  AT25_Save_CC_CV_CP();	
	
	Use_Vint_Or_Vext_Flag = Use_Vint;
	AT25_Save_Use_Vint_or_Vext_Flag();
}


/*--------------------------------------------------------读取---------------------------------------------------*/


 /**
  * @brief  对AT25读出电流电压功率设定值
  * @param	无
  * @retval 无
  */
static void AT25_Load_VIPSet(void)
{
	u16 temp[2];
	u32 temp_1[1];
	
	SPI_AT25_Read(temp, 0, sizeof(temp));
  SPI_AT25_Read((u16*)temp_1, 124, sizeof(temp));	
	
	//将设定值强制类型转换成u16
  Set_Voltage = ((float)temp[0]) / 100;
  Set_Current = ((float)temp[1]) / 100;
  Set_Power = ((float)temp_1[0]) / 100;
}

 /**
  * @brief  对AT25读出Recall_Save的电流电压功率设定值
  * @param	无
  * @retval 无
  */
static void AT25_Load_RecallSaveSet(void)
{
	u16 temp[30];
	
	int i = 0;
	
	u8 Number = 0;
	
	SPI_AT25_Read(temp, 6, sizeof(temp));	
	
	for(; i < 30; i++)
	{
	  //将设定值强制类型转换成u16
    Recall_Save_Voltage[Number] = ((float)temp[i]) / 100;
		i ++;
    Recall_Save_Current[Number] = ((float)temp[i]) / 100;
		i ++;
    Recall_Save_Power[Number] = ((float)temp[i]) / 100;		
		
		Number ++;
	}
}

 /**
  * @brief  对AT25写入RS232设定值
  * @param	Recall_Save的编号
  * @retval 无
  */
static void AT25_Load_RS232(void)
{
	u16 temp_ID[1];
	u32 temp_BaudRate[1];
	
	//读取id号
	SPI_AT25_Read(temp_ID, 66, sizeof(temp_ID));
	RS232_ID = temp_ID[0]; 
	
	//读取波特率
	SPI_AT25_Read((u16*)temp_BaudRate, 68, sizeof(temp_BaudRate));	
	RS232_BaudRate = temp_BaudRate[0];
}

 /**
  * @brief  对AT25写入CAN设定值
  * @param	Recall_Save的编号
  * @retval 无
  */
static void AT25_Load_CAN(void)
{
	u16 temp_ID[1];
	u32 temp_BaudRate[1];
	
	//读取id号
	SPI_AT25_Read(temp_ID, 72, sizeof(temp_ID));
	CAN_ID = temp_ID[0]; 
	
	//读取波特率
	SPI_AT25_Read((u16*)temp_BaudRate, 74, sizeof(temp_BaudRate));	
	CAN_BaudRate = temp_BaudRate[0];
}

 /**
  * @brief  对AT25读出延时时间设定值
  * @param	无
  * @retval 无
  */
static void AT25_Load_Delay(void)
{
	u8 temp[4];
	u8 temp_1[2];	
	
	SPI_AT25_Read((u16*)temp, 78, sizeof(temp));
	SPI_AT25_Read((u16*)temp_1, 108, sizeof(temp_1));
	
	Delay_h = temp[0]; 
	Delay_m = temp[1];
	Delay_s = temp[2];	

	Delay_Function_On_Off = temp_1[0];
}

 /**
  * @brief  对AT25读出循环时间设定值
  * @param	无
  * @retval 无
  */
static void AT25_Load_Cycle(void)
{
	u16 temp[4];
	u8 temp_1[2];	
	
	SPI_AT25_Read(temp, 82, sizeof(temp));	
	SPI_AT25_Read((u16*)temp_1, 110, sizeof(temp_1));	
	
	Cycle_On_s = temp[0];
	Cycle_On_ms = temp[1];
	Cycle_Close_s = temp[2];	
	Cycle_Close_ms = temp[3];	

	Cycle_Function_On_Off = temp_1[0];
}

 /**
  * @brief  对AT25读出CC_CV_CP设定值
  * @param	无
  * @retval 无
  */
static void AT25_Load_CC_CV_CP(void)
{
	u8 temp[2];
	
	SPI_AT25_Read((u16*)temp, 90, sizeof(temp));		
	
	CV_CC_CP = temp[0];
}

 /**
  * @brief  对AT25写入I_Fall设定值
  * @param	无
  * @retval 无
  */
static void AT25_Load_V_Rise(void)
{
	u16 temp[2];
	u8 temp_1[2];	
	
	SPI_AT25_Read((u16*)temp, 92, sizeof(temp));	
	SPI_AT25_Read((u16*)temp_1, 112, sizeof(temp_1));
	
	V_Rise_s = temp[0];
	V_Rise_ms = temp[1];
	
	V_Rise_On_Off = temp_1[0];		
}

 /**
  * @brief  对AT25写入I_Fall设定值
  * @param	无
  * @retval 无
  */
static void AT25_Load_V_Fall(void)
{
	u16 temp[2];
	u8 temp_1[2];	
	
	SPI_AT25_Read((u16*)temp, 96, sizeof(temp));
	SPI_AT25_Read((u16*)temp_1, 114, sizeof(temp_1));	
	
	V_Fall_s = temp[0];
	V_Fall_ms = temp[1];
	
	V_Fall_On_Off = temp_1[0];	
}

 /**
  * @brief  对AT25写入I_Fall设定值
  * @param	无
  * @retval 无
  */
static void AT25_Load_I_Rise(void)
{
	u16 temp[2];
	u8 temp_1[2];	
	
	SPI_AT25_Read((u16*)temp, 100, sizeof(temp));
	SPI_AT25_Read((u16*)temp_1, 116, sizeof(temp_1));	
	
	I_Rise_s = temp[0];
	I_Rise_ms = temp[1];
	
	I_Rise_On_Off = temp_1[0];	
}

 /**
  * @brief  对AT25写入I_Fall设定值
  * @param	无
  * @retval 无
  */
static void AT25_Load_I_Fall(void)
{
	u16 temp[2];
	u8 temp_1[2];	
	
	SPI_AT25_Write((u16*)temp, 104, sizeof(temp));		
	SPI_AT25_Read((u16*)temp_1, 118, sizeof(temp_1));	
	
	I_Fall_s = temp[0];
	I_Fall_ms = temp[1];
	
	I_Fall_On_Off = temp_1[0];	
}

 /**
  * @brief  对AT25读出Fist_Use_Device设定值
  * @param	无
  * @retval 无
  */
static void AT25_Load_Fist_Use_Device(void)
{
	u8 temp[2];
	
	SPI_AT25_Read((u16*)temp, 120, sizeof(temp));		
	
	Fist_Use_Device = temp[0];
	
	temp[0] = 0x00;
	temp[1] = 0x00;
	
	SPI_AT25_Write((u16*)temp, 120, sizeof(temp));	
}

 /**
  * @brief  对AT25读出Fist_Use_Device设定值
  * @param	无
  * @retval 无
  */
static void AT25_Load_Use_Vint_Or_Vext_Flag(void)
{
	u8 temp[2];
	
	SPI_AT25_Read((u16*)temp, 122, sizeof(temp));		
	
	Use_Vint_Or_Vext_Flag = temp[0];
}


 /**
  * @brief  对AT25读出所有设定值
  * @param	无
  * @retval 无
  */
void AT25_Load_Data(void)
{
	AT25_Load_Fist_Use_Device();
	if(Fist_Use_Device == 0xff)   //如果第一次写入设备，就重置at25
	{
		AT25_Reset();
	}
	
	AT25_Load_VIPSet();
	AT25_Load_RecallSaveSet();
	AT25_Load_RS232();
	AT25_Load_CAN();
	AT25_Load_Delay();
	AT25_Load_Cycle();
	AT25_Load_CC_CV_CP();
	AT25_Load_V_Rise();
	AT25_Load_V_Fall();
	AT25_Load_I_Rise();
	AT25_Load_I_Fall();
	AT25_Load_Use_Vint_Or_Vext_Flag();
}


