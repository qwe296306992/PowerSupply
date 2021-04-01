#include "spi_oled.h"

   
u8 Error;
u8 Last_Interface;

#define set_OLED GPIO_SetBits(GPIOA, GPIO_Pin_12)
#define res_OLED GPIO_ResetBits(GPIOA, GPIO_Pin_12)



extern u8 OCP_SoftOrHard_Flag;//OCP硬件报警还是软件报警
extern u8 OTP_SensorX;				//OTP报警是哪个传感器

float Recall_Save_Display_Voltage;  //显示的存取电压
float Recall_Save_Display_Current;  //显示的存取电流
float Recall_Save_Display_Power;    //显示的存取功率

float Recall_Save_Voltage[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float Recall_Save_Current[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float Recall_Save_Power[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};


float Set_Voltage;
float Set_Current;
float Set_Power;


float Voltage;
float Current;
float Power;

float Vint;
float Vext;


/**
  * @brief  SPI_OLED初始化
  * @param  无
  * @retval 无
  */
void SPI_FUN(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	
	/*- 开启GPIOB以及SPI3时钟 -*/
  OLED_SPI_CS_APBxClock_FUN(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
  OLED_SPI_APBxClock_FUN(OLED_SPI_CLK, ENABLE);
	
	//关JTAG
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SPI3,ENABLE);
	
	/*- SPI3引脚 - SCLK、MOSI -*/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	
	GPIO_InitStructure.GPIO_Pin =OLED_SPI_SCK_PIN;
  GPIO_Init(OLED_SPI_SCK_PORT, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = OLED_SPI_MOSI_PIN;
	GPIO_Init(OLED_SPI_MOSI_PORT, &GPIO_InitStructure);
	
	/*- 片选引脚->PB10、数据命令切换->PB11 -*/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	
	GPIO_InitStructure.GPIO_Pin = OLED_SPI_CS_PIN;
  GPIO_Init(OLED_SPI_CS_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = OLED_SPI_DC_PIN;
	GPIO_Init(OLED_SPI_DC_PORT, &GPIO_InitStructure);
	
	/*AT25:CS PD2引脚*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//CS
  GPIO_Init(GPIOD, &GPIO_InitStructure);

	/*- SPI3配置 -*/
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//全双工模式
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 										  //主模式
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	 								  //数据大小8位
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		 										  //时钟极性，空闲时为高
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;											  //第2个边沿有效，上升沿为采样时刻
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		   										  //NSS信号由软件产生
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; 	//设置分频，所得频率=36M/分频比
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  							  //高位在前
	SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(OLED_SPIx, &SPI_InitStructure);														//初始化结构体

  /* 使能 SPI  */
  SPI_Cmd(OLED_SPIx , ENABLE);
	
}

 /**
  * @brief  使用SPI发送一个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
static u8 SPI_SendByte(u8 byte)
{
	
  /* 等待发送缓冲区为空，TXE事件 */
  while (SPI_I2S_GetFlagStatus(OLED_SPIx , SPI_I2S_FLAG_TXE) == RESET)
	{
		
	}

  /* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
  SPI_I2S_SendData(OLED_SPIx , byte);
	
	/*等待接受缓冲区非空*/
	while (SPI_I2S_GetFlagStatus(OLED_SPIx, SPI_I2S_FLAG_RXNE) == RESET)
	{
		
	}
	
	return SPI_I2S_ReceiveData(OLED_SPIx);	
}


 /**
  * @brief  向OLED发送数据
  * @param  byte：要发送的数据
  * @retval None
  */
void SPI_OLED_SendData(u8 byte)
{
	/* 选择 OLED: CS 低 */
  SPI_OLED_CS_LOW();
	
	//选择写数据模式
	SPI_OLED_DC_HIGH();
	
	//写入命令
	SPI_SendByte(byte);
	
	//选择写数据模式
	SPI_OLED_DC_HIGH();
	
	/* 停止信号 OLED: CS 高电平 */
  SPI_OLED_CS_HIGH();
}


 /**
  * @brief  向OLED发送命令
  * @param  byte：要发送的命令
  * @retval None
  */
void SPI_OLED_SendCommond(uint8_t byte)
{
	/* 选择 OLED: CS 低 */
  SPI_OLED_CS_LOW();
	
	//选择写命令模式
	SPI_OLED_DC_LOW();
	
	//写入数据
	SPI_SendByte(byte);
	
	//选择写数据模式
	SPI_OLED_DC_HIGH();
	
	/* 停止信号 OLED: CS 高电平 */
  SPI_OLED_CS_HIGH();
}


 /**
  * @brief  OLED初始化
  */
void OLED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//输出引脚初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	res_OLED;
	SysTick_Delay_Ms(1);
	set_OLED;
	SysTick_Delay_Ms(5);
	
	SPI_OLED_SendCommond(Set_Command_Lock);//开锁
	SPI_OLED_SendData(0x12);
	
	SPI_OLED_SendCommond(Set_Column_Address_Commond);//设置列地址
	SPI_OLED_SendData(0x1c);
	SPI_OLED_SendData(0x5b);
	
	SPI_OLED_SendCommond(Set_Row_Address_Commond);//设置行地址
	SPI_OLED_SendData(0x00);
	SPI_OLED_SendData(0x3f);
	
	SPI_OLED_SendCommond(Sleep_mode_ON);//休眠

	SPI_OLED_SendCommond(Set_Front_Clock_Divider);//设置前时钟分频器/振荡器频率
	SPI_OLED_SendData(0x91);
	
	SPI_OLED_SendCommond(Set_MUX_Ratio);//设置多路选择开关比值
	SPI_OLED_SendData(0x3f);
	
	SPI_OLED_SendCommond(Set_Display_Offset);//从0-127设置垂直滚动
	SPI_OLED_SendData(0x00);
	
	SPI_OLED_SendCommond(Set_Display_Start_Line);//设置启动行
	SPI_OLED_SendData(0x00);
	
	SPI_OLED_SendCommond(Normal_Display);//设置正常显示
	
	SPI_OLED_SendCommond(Set_Re_map_and_Dual_COM_Line_mode);//设置扫描方向
	SPI_OLED_SendData(0x14);
	SPI_OLED_SendData(0x11);
	
	SPI_OLED_SendCommond(Set_GPIO);//引脚设定
	SPI_OLED_SendData(0x00);
	
	SPI_OLED_SendCommond(Function_Selection);//内部电压调节
	SPI_OLED_SendData(0x01);
	
	SPI_OLED_SendCommond(0xb4);
	SPI_OLED_SendData(0xa0);
	SPI_OLED_SendData(0xfd);
	
	SPI_OLED_SendCommond(Set_Contrast_Current);//设置对比度电流
	SPI_OLED_SendData(0xaf);
	
	SPI_OLED_SendCommond(Master_Contrast_Current_Control);//主对比度电流控制
	SPI_OLED_SendData(0x0f);
	
	SPI_OLED_SendCommond(Set_Gray_Scale_Table);//设置灰度表
	SPI_OLED_SendData(0x0c);
	SPI_OLED_SendData(0x18);
	SPI_OLED_SendData(0x24);
	SPI_OLED_SendData(0x30);
	SPI_OLED_SendData(0x3c);
	SPI_OLED_SendData(0x48);
	SPI_OLED_SendData(0x54);
	SPI_OLED_SendData(0x60);
 	SPI_OLED_SendData(0x6c);
	SPI_OLED_SendData(0x78);
	SPI_OLED_SendData(0x84);
	SPI_OLED_SendData(0x90);
	SPI_OLED_SendData(0x9c);
	SPI_OLED_SendData(0xa8);
	SPI_OLED_SendData(0xb4);
	
	SPI_OLED_SendCommond(Enable_Gray_Scale_table);//开启灰度表
	
	SPI_OLED_SendCommond(Set_Phase_Length);//设置相位长度
	SPI_OLED_SendData(0xE2);
	
	SPI_OLED_SendCommond(0xD1); 								//Display Enhancement B
	SPI_OLED_SendData(0xa2);
	SPI_OLED_SendData(0x20);										//反显
	
	SPI_OLED_SendCommond(Set_Pre_charge_voltage);//设置预充电电压
	SPI_OLED_SendData(0x1F);
	
	SPI_OLED_SendCommond(Set_Second_Precharge_Period);//设置第二预充期间
	SPI_OLED_SendData(0x08);
	
	SPI_OLED_SendCommond(Set_VCOMH );//设置COM取消选择电压等级
	SPI_OLED_SendData(0x07);
	
	SPI_OLED_SendCommond(Normal_Display);//正常显示
	
	SPI_OLED_SendCommond(Exit_Partial_Display);//该命令被发送到退出部分显示模式
	
	SPI_OLED_SendCommond(Write_RAM_Command);
	
	 Fill_RAM(0x00);   //清屏
	
	SPI_OLED_SendCommond(Sleep_mode_OFF);//关休眠
}

/*----------------------------------------------显示函数-----------------------------------------------*/

 /**
  * @brief  设置列地址
  * @param  byte：起始地址
  * @param  byte：结束地址
  * @retval None
  */
void Set_Column_Address(u8 a, u8 b)
{
	SPI_OLED_SendCommond(Set_Column_Address_Commond);			// Set Column Address
	SPI_OLED_SendData(0x1c+a);					                  //0x1c地址偏移量
	SPI_OLED_SendData(0x1c+b);				                   
}

 /**
  * @brief  设置行地址
  * @param  byte：起始地址
  * @param  byte：结束地址
  * @retval None
  */
void Set_Row_Address(u8 a, u8 b)
{
	SPI_OLED_SendCommond(Set_Row_Address_Commond);			// Set Row Address
	SPI_OLED_SendData(a);					//   Default => 0x00
	SPI_OLED_SendData(b);					//   Default => 0x7F
}

 /**
  * @brief  写命令
  * @param  None
  * @retval None
  */
void Set_WriteData(void)
{
	SPI_OLED_SendCommond(Write_RAM_Command);			// Enable MCU to Write into RAM
}

 /**
  * @brief  写顺序
  * @param  A[0]=0 水平扫描；A[0]=1 竖直扫描
            A[1]设置像素四位总体的顺序
            A[2]设置一个像素四位的顺序
            A[5]设置COM的奇偶分裂
            这个参数一般设置为0x14
  * @retval None
  */
void Set_Remap_Format(unsigned char A)
{
	SPI_OLED_SendCommond(Set_Re_map_and_Dual_COM_Line_mode);			// Set Re-Map / Dual COM Line Mode
	SPI_OLED_SendData(A);					                                //   Default => 0x40
	SPI_OLED_SendData(0x11);	   	                               	//   Default => 0x01 (Disable Dual COM Mode)
}

 /**
  * @brief  整屏显示
  * @param  要整屏显示的颜色
  * @retval None
  */
void Fill_RAM(u8 Data)
{
	unsigned char i,j;

	Set_Column_Address(0x00,0x3f);//0到63列    
	Set_Row_Address(0x00,0x3f);   //0到63行
	Set_WriteData();

	for(i=0;i<64;i++)             //一共64行
	{
		for(j=0;j<64;j++)           //一共64列
		{
			SPI_OLED_SendData(Data);  //一列两个像素点
			SPI_OLED_SendData(Data);
		}
	}
}

 /**
  * @brief  行间隔显示
  * @param  要行间隔显示的颜色
  * @retval None
  */
void Row_Interval(u8 Data)
{
	u8 i;
	u8 j;
	
	Set_Column_Address(0x00,0x3f);
	Set_Row_Address(0x00,0x3f);
	Set_WriteData();
	
		for(i=0;i<32;i++)
	{
		for(j=0;j<64;j++)
		{
			SPI_OLED_SendData(Data);
			SPI_OLED_SendData(Data);
		}
			for(j=0;j<64;j++)
		{
			SPI_OLED_SendData(~Data);
			SPI_OLED_SendData(~Data);
		}
	}
}

 /**
  * @brief  点间隔显示
  * @param  要点隔显示的方式
  * @retval None
  */
void Point_Interval(u8 m)
{
	u8 i;
	u8 j;
	
	Set_Column_Address(0x00,0x3f);
	Set_Row_Address(0x00,0x3f);
	Set_WriteData();
	
		for(i=0;i<32;i++)
	{
		for(j=0;j<64;j++)
		{
			if(m==0xf0)
			{
				SPI_OLED_SendData(0xf0);
				SPI_OLED_SendData(0xf0);
			}
			else
			{
				SPI_OLED_SendData(0x0f);
				SPI_OLED_SendData(0x0f);
			}
		}
		for(j=0;j<64;j++)
		{
			if(m==0xf0)
			{
				SPI_OLED_SendData(0x0f);
				SPI_OLED_SendData(0x0f);
			}
			else
			{
				SPI_OLED_SendData(0xf0);
				SPI_OLED_SendData(0xf0);
			}
		}
	}
}

 /**
  * @brief  画一条横线
  * @param  起始点的行地址
  * @param  起始点的列地址
  * @param  结束点的列地址
  * @retval None
  */
void Display_Horizontal_Line(u8 Row, u8 Start_Column, u8 End_Column)
{
	u8 i;
	
	Set_Remap_Format(0x14);                      //行扫描模式
	Set_Column_Address(Start_Column, End_Column);//设置列地址
	Set_Row_Address(Row, Row);                   //设置行地址
	
	Set_WriteData();//写使能
	
		for(i=0;i<End_Column-Start_Column+1;i++)
	{
		SPI_OLED_SendData(0xff);
		SPI_OLED_SendData(0xff);
	}
}





 /**
  * @brief  画一条竖线竖线1(因为一列有四个像素)
  * @param  起始点的列地址
  * @param  起始点的行地址
  * @param  结束点的行地址
  * @retval None
  */
void Display_Vertical_Line_1(u8 Column, u8 Start_Row, u8 End_Row)
{
	u8 i;
	
	Set_Remap_Format(0x15);                //列扫描模式
	Set_Column_Address(Column, Column);    //设置列地址
	Set_Row_Address(Start_Row, End_Row);   //设置行地址
	
	Set_WriteData();//写使能
	
	for(i=0; i<End_Row-Start_Row+1; i++)
	{
		SPI_OLED_SendData(0xf0);
		SPI_OLED_SendData(0x00);
	}
	Set_Remap_Format(0x14);                //行扫描模式
}

 /**
  * @brief  画一条竖线竖线2
  * @param  起始点的列地址
  * @param  起始点的行地址
  * @param  结束点的行地址
  * @retval None
  */
void Display_Vertical_Line_2(u8 Column, u8 Start_Row, u8 End_Row)
{
	u8 i;
	
	Set_Remap_Format(0x15);                //列扫描模式
	Set_Column_Address(Column, Column);    //设置列地址
	Set_Row_Address(Start_Row, End_Row);   //设置行地址
	
	Set_WriteData();//写使能
	
	for(i=0; i<End_Row-Start_Row+1; i++)
	{
		SPI_OLED_SendData(0x0f);
		SPI_OLED_SendData(0x00);
	}
	Set_Remap_Format(0x14);                //行扫描模式
}

 /**
  * @brief  画一条竖线竖线3
  * @param  起始点的列地址
  * @param  起始点的行地址
  * @param  结束点的行地址
  * @retval None
  */
void Display_Vertical_Line_3(u8 Column, u8 Start_Row, u8 End_Row)
{
	u8 i;
	
	Set_Remap_Format(0x15);                //列扫描模式
	Set_Column_Address(Column, Column);    //设置列地址
	Set_Row_Address(Start_Row, End_Row);   //设置行地址
	
	Set_WriteData();//写使能
	
	for(i=0; i<End_Row-Start_Row+1; i++)
	{
		SPI_OLED_SendData(0x00);
		SPI_OLED_SendData(0xf0);
	}
	Set_Remap_Format(0x14);                //行扫描模式
}

 /**
  * @brief  画一条竖线竖线4
  * @param  起始点的列地址
  * @param  起始点的行地址
  * @param  结束点的行地址
  * @retval None
  */
void Display_Vertical_Line_4(u8 Column, u8 Start_Row, u8 End_Row)
{
	u8 i;
	
	Set_Remap_Format(0x15);                //列扫描模式
	Set_Column_Address(Column, Column);    //设置列地址
	Set_Row_Address(Start_Row, End_Row);   //设置行地址
	
	Set_WriteData();//写使能
	
	for(i=0; i<End_Row-Start_Row+1; i++)
	{
		SPI_OLED_SendData(0x00);
		SPI_OLED_SendData(0x0f);
	}
	Set_Remap_Format(0x14);                //行扫描模式
}


 /**
  * @brief  画一个矩形边框
  * @param  起始点的行地址
  * @param  起始点的列地址
  * @param  结束点的行地址
  * @param  结束点的列地址
  * @retval None
  */
void Display_Rectangular(u8 Start_Row, u8 Start_Column, u8 End_Row, u8 End_Column)
{
	Display_Horizontal_Line(Start_Row, Start_Column, End_Column);
	Display_Horizontal_Line(End_Row, Start_Column, End_Column);
	Display_Vertical_Line_4(Start_Column, Start_Row, End_Row);
	Display_Vertical_Line_1(End_Column, Start_Row, End_Row);
}

 /**
  * @brief  画一个任意大小，颜色的实心矩形
  * @param  起始行、列，结束行、列，颜色
  * @retval None
  */
void Dsiplay_Solid_Rectangular(u8 Start_Row, u8 Start_Column, u8 End_Row, u8 End_Column, u8 Colour)
{
	u16 i;  //循环写入次数
	
	Set_Remap_Format(0x14);                //行扫描模式
	Set_Column_Address(Start_Column, End_Column);    //设置列地址
	Set_Row_Address(Start_Row, End_Row);   //设置行地址
	
	Set_WriteData();//写使能
	
		for(i=0; i<(End_Row-Start_Row+1)*(End_Column-Start_Column+1); i++)
	{
		SPI_OLED_SendData(Colour);
		SPI_OLED_SendData(Colour);
	}
}

 /**
  * @brief  转换函数，将一位转为四位（SD1322四位表示一个像素点）,并写入SD1322
  * @param  要转换的数据
  * @retval None
  */
void Font_1Bit_To_4Bit(u8 Data)
{
  u8 data_4byte;
  u8 i;
  u8 j;

  for(i=0,j=7; i<4; i++)
  {
   	data_4byte=*(u32 *)(0x22000000+ ((u32)(&Data)-0x20000000)*32 +j*4)*0xf0 
		           + *(u32 *)(0x22000000+ ((u32)(&Data)-0x20000000)*32 +(j-1)*4)*0x0f; //求字模的位带地址，并取内容，根据内容写入0或F
  	SPI_OLED_SendData(data_4byte);
		j--;
		j--;
  }
}

 /**
  * @brief  写一个8x16数字、字母(ASIC II)
  * @param  行，列，要写入的数字、字母
  * @retval None
  */
void Write_Single_8x16AsicII(u8 Row, u8 Column, char Data)
{
	u8 i;
	u16 k;
	
	SPI_OLED_SendCommond(Set_Re_map_and_Dual_COM_Line_mode);//设置扫描方向
	SPI_OLED_SendData(0x14);
	SPI_OLED_SendData(0x11);
	
	Set_Column_Address(Column, Column+1);    //设置列地址
	Set_Row_Address(Row, Row+15);            //设置行地址
	
	Set_WriteData();
	
	k=(Data-' ')*16;//求对空格的偏移量
	
	for(i=0;i<16;i++)
	{
		Font_1Bit_To_4Bit(Fonts_AsicII_8x16[k]);
		k++;
	}
}

 /**
  * @brief  写一行8x16数字、字母(ASIC II)
  * @param  行，列，要写入的数字、字母
  * @retval None
  */
void Write_String_8x16AsicII(u8 Row, u8 Column, char Data[])
{
	u8 i;
	u8 k;
	k=strlen(Data);
	
	for(i=0;i<k;i++)
	{
	 Write_Single_8x16AsicII(Row,Column+2*i,Data[i]);
	}
}


 /**
  * @brief  写一个16x16中文
  * @param  行，列，要写入的中文(后面要加空格)
  * @retval None
  */
void Write_Single_16x16Chinese(u8 Row, u8 Column, char Data[])
{
	u8 i;
	u16 k;
	
	k=Chinese_Pinyin_to_16x16Fonts(Data);
	k=k*32;
	
	SPI_OLED_SendCommond(Set_Re_map_and_Dual_COM_Line_mode);//设置扫描方向
	SPI_OLED_SendData(0x14);
	SPI_OLED_SendData(0x11);
	
	Set_Column_Address(Column, Column+3);    //设置列地址
	Set_Row_Address(Row, Row+15);            //设置行地址
	
	Set_WriteData();
	
	for(i=0;i<32;i++)
	{
	 Font_1Bit_To_4Bit( Fonts_16x16Chinese[k+i]);
	}
}

 /**
  * @brief  将字符串的一部分给另一个字符串,用法同Strncpy，并能消除Strncpy的缺陷
  * @param  目标字符串
  * @param  原字符串数组的起始成员
  * @param  个数
  * @retval None
  */
char *myStrncpy(char *dest, const char *src, size_t n)
{
  int size = sizeof(char)*(n+1);
  char *tmp = (char*)malloc(size);  // 开辟大小为n+1的临时内存tmp
  if(tmp)
		{
      memset(tmp, '\0', size);  // 将内存初始化为0
      memcpy(tmp, src, size-1);  // 将src的前n个字节拷贝到tmp
      memcpy(dest, tmp, size);  // 将临时空间tmp的内容拷贝到dest
      free(tmp);  // 释放内存
      return dest;
    }
		else
		{
      return NULL;
    }
 }


 /**
  * @brief  写一行16x16中文
  * @param  行，列，要写入的中文(后面要加空格)
  * @retval None
  */
void Write_String_16x16Chinese(u8 Row, u8 Column, char String_Data[])
{	
	u16 i=0;
	u16 j;
	u16 k=0;
	u16 m=0;
	
	char* Single_Data=NULL;       //定义数组指针
	
	j=strlen(String_Data);        //求整个字符串的长度
	
	for(;i<j;)                    //i<j表示还没完全输出完成
	{
	  for(k=0;String_Data[i]!=' ';i++,k++)   //k表示一个字的字符串长度
	  {
	  }
	  i=i+1;
	  m=m+1;                     //m表示字的个数
	
	  Single_Data=(char*)malloc(k+1);    //开辟一个k字节的内存空间
	  if(Single_Data)                    //判断是否开辟成功
			
	  myStrncpy(Single_Data,&String_Data[i-k-1],k+1);    //将这个字从原字符串提取出来
	
	
  	Write_Single_16x16Chinese(Row, Column+4*(m-1), Single_Data);    //写进SD1366
	
	  free(Single_Data);   //释放内存
	  Single_Data=NULL;    //指针定义
	}
}


 /**
  * @brief  写一个16x32数字、字母(ASIC II)
  * @param  行，列，要写入的数字、字母
  * @retval None
  */
void Write_Single_16x32AsicII(u8 Row, u8 Column, char Data)
{
	u8 i;
	u16 k;
	
	SPI_OLED_SendCommond(Set_Re_map_and_Dual_COM_Line_mode);//设置扫描方向
	SPI_OLED_SendData(0x14);
	SPI_OLED_SendData(0x11);
	
	Set_Column_Address(Column, Column+3);    //设置列地址
	Set_Row_Address(Row, Row+31);            //设置行地址
	
	Set_WriteData();
	
	k=(Data-' ')*64;//求对空格的偏移量
	
	for(i=0;i<64;i++)
	{
		Font_1Bit_To_4Bit(Fonts_AsicII_16x32[k]);
		k++;
	}
}

 /**
  * @brief  写一行16x32数字、字母(ASIC II)   分辨率256*64，16*24==长16，宽32
  * @param  行，列，要写入的数字、字母         Row==行的左上角   Column==列的左边（总共64）
  * @retval None
  */
void Write_String_16x32AsicII(u8 Row, u8 Column, char Data[])
{
	u8 i;
	u8 k;
	k=strlen(Data);
	
	for(i=0;i<k;i++)
	{
	 Write_Single_16x32AsicII(Row,Column+4*i,Data[i]);
	}
}

 /**
  * @brief  12x24专用的转换函数，将一位转为四位（SD1322四位表示一个像素点）,并写入SD1322
  * @param  要转换的数据
  * @retval None
  */
static void Font_1Bit_To_4Bit_12x24(u8 Data0, u8 Data1)
{
	u8 data_4byte;
  u8 i;
  u8 j;

	//取Data0
  for(i=0,j=7; i<4; i++)
  {
   	data_4byte=*(u32 *)(0x22000000+ ((u32)(&Data0)-0x20000000)*32 +j*4)*0xf0 
		           + *(u32 *)(0x22000000+ ((u32)(&Data0)-0x20000000)*32 +(j-1)*4)*0x0f; //求字模的位带地址，并取内容，根据内容写入0或F
  	SPI_OLED_SendData(data_4byte);
		j--;
		j--;
  }
	
	//取Data1的高四位
	for(i=0,j=7; i<2; i++)
	{
		data_4byte=*(u32 *)(0x22000000+ ((u32)(&Data1)-0x20000000)*32 +j*4)*0xf0 
		           + *(u32 *)(0x22000000+ ((u32)(&Data1)-0x20000000)*32 +(j-1)*4)*0x0f; //求字模的位带地址，并取内容，根据内容写入0或F
  	SPI_OLED_SendData(data_4byte);
		j--;
		j--;
	}
}



 /**
  * @brief  写一个12x24数字、字母(ASIC II)
  * @param  行，列，要写入的数字、字母
  * @retval None
  */
void Write_Single_12x24AsicII(u8 Row, u8 Column, char Data)
{
	u8 i;
	u16 k;
	
	SPI_OLED_SendCommond(Set_Re_map_and_Dual_COM_Line_mode);//设置扫描方向
	SPI_OLED_SendData(0x14);
	SPI_OLED_SendData(0x11);
	
	Set_Column_Address(Column, Column+2);    //设置列地址
	Set_Row_Address(Row, Row+23);            //设置行地址
	
	Set_WriteData();
	
	k=(Data-' ')*48;//求对空格的偏移量
	
	for(i=0;i<24;i++)
	{
		Font_1Bit_To_4Bit_12x24(Fonts_AsicII_12x24[k],Fonts_AsicII_12x24[k+1]);
		k=k+2;
	}
}

 /**
  * @brief  写一行12x24数字、字母(ASIC II)
  * @param  行，列，要写入的数字、字母
  * @retval None
  */
void Write_String_12x24AsicII(u8 Row, u8 Column, char Data[])
{
	u8 i;
	u8 k;
	k=strlen(Data);
	
	for(i=0;i<k;i++)
	{
	 Write_Single_12x24AsicII(Row,Column+3*i,Data[i]);
	}
}

/*---------------------------------------------------------显示主界面(真实值界面和设定值查看界面)-----------------------------------------------------*/

char String_Voltage[20];                       //定义一个存放转变后的电压字符串的数组,也用于后面设定电压值时的显示
float ADC_temp = 100;


 /**
  * @brief  将电压值显示出来
  * @param  电压值
  * @retval None
  */
void Display_Main_Interface_Voltage(float Voltage)
{	
	if(Voltage>=100)                             //将float转为string
	{
		sprintf(String_Voltage, "%5.1f", Voltage);
	}
	else if(Voltage>=9.995)
	{
		sprintf(String_Voltage, "%5.2f", Voltage);
	}
	else if(Voltage>=0)
	{		
	  sprintf(String_Voltage, "%5.2f", Voltage);
		String_Voltage[0] = '0';
	}
	else if(Voltage>=-10)
	{
	  sprintf(String_Voltage, "%5.2f", Voltage);
	}
	else 
	{
		sprintf(String_Voltage, "%5.1f", Voltage);
	}
	
	Write_String_16x32AsicII(0, 0, String_Voltage);
	
	Write_Single_16x32AsicII(0, 20, 'V');
	
			if((strcmp(String_Voltage, "00.00") == 0) & (ON_OFF == ON))
	{
     ADC_temp = Voltage;
	}
	
}

char String_Current[20];                       //定义一个存放转变后的电流字符串的数组,也用于后面设定电流值时的显示

 /**
  * @brief  将电流值显示出来
  * @param  电流值
  * @retval None
  */
void Display_Main_Interface_Current(float Current)
{
	if(Current>=100)                             //将float转为string
	{
		sprintf(String_Current, "%6.2f", Current);
	}
	else if(Current>=9.995)
	{
		sprintf(String_Current, "%6.2f", Current);
		String_Current[0] = '0';
	}
	else if(Current>=0)
	{
		sprintf(String_Current, "%6.2f", Current);
		String_Current[0] = '0';
    String_Current[1] = '0';		
	}
	else if(Current>=-10)
	{
		sprintf(String_Current, "%6.2f", Current);
		String_Current[0] = '-';
    String_Current[1] = '0';		
	}
	else
	{
  	sprintf(String_Current, "%6.2f", Current);
	}		
	
	Write_String_16x32AsicII(0, 36, String_Current);
	
	Write_Single_16x32AsicII(0, 60, 'A');
}

char String_Power[20];                       //定义一个存放转变后的功率字符串的数组,也用于后面设定功率值时的显示

 /**
  * @brief  将功率值显示出来
  * @param  功率值
  * @retval None
  */
void Display_Main_Interface_Power(float Power)
{
	if(Power >= 1000)
	{
		sprintf(String_Power, "%5.0f", Power);
//		String_Power[0] = '0';
//			String_Power[0] = ' ';

	}
	else if(Power >= 100)                               
	{
		sprintf(String_Power, "%5.1f", Power);
	}
	else if(Power >= 10)
	{
	  sprintf(String_Power, "%5.2f", Power);
	}
	else if(Power >= 0)
	{
		sprintf(String_Power, "%5.2f", Power);
		String_Power[0] = '0';
	}
	else if(Power >= -10)
	{
	  sprintf(String_Power, "%5.2f", Power);
	}
	else
	{
		sprintf(String_Power, "%5.1f", Power);
	}	

	
	Write_String_12x24AsicII(42, 0, String_Power);
	
	Write_Single_12x24AsicII(42, 15, 'W');
}

 /**
  * @brief  显示键盘锁定状态
  * @param  显示ON，不显示OFF
  * @retval None
  */
static void Display_Main_Interface_Lock(u8 State)
{
	if(State==LOCK)
	{
		Write_String_8x16AsicII(34, 56, "LOCK");
	}
	else
	{
		Write_String_8x16AsicII(34, 56, "    ");
	}
}

 /**
  * @brief  显示处于设定/实际值状态
  * @param  设定值状态Preset/实际值状态Actual
  * @retval None
  */
static void Display_Main_Interface_Preset(u8 State)
{
	if(State==Preset)
	{
		Write_String_8x16AsicII(50, 39, "Preset");
	}
	else
	{
		Write_String_8x16AsicII(50, 39, "      ");
	}
}

 /**
  * @brief  显示处于CV/CC/CP和DELAY状态
  * @param  CV/CC/CP
  * @param  DELAY/NOT_DELAY
  * @retval None
  */
void Display_Main_Interface_CV_CC_CP(u8 CV_CC_CP,u8 DELAY_NOT_DELAY)
{
	u8 State;
	State=CV_CC_CP | DELAY_NOT_DELAY;                                    //处于DELAY状态时，只显示DELAY
	switch(State)
	{
		case DELAY: Write_String_8x16AsicII(50, 27, "DELAY");break;
		case CV: 
			Write_String_8x16AsicII(50, 26, "  ");
		  Write_String_8x16AsicII(50, 34, "  ");
		  Write_String_8x16AsicII(50, 30, "CV");break;
		case CC: 
			Write_String_8x16AsicII(50, 26, "  ");
		  Write_String_8x16AsicII(50, 34, "  ");
		  Write_String_8x16AsicII(50, 30, "CC");break;
		case CP: 
			Write_String_8x16AsicII(50, 26, "  ");
		  Write_String_8x16AsicII(50, 34, "  ");
		  Write_String_8x16AsicII(50, 30, "CP");break;
		default: break;
	}
}

 /**
  * @brief  显示处于ON/OFF状态
  * @param  ON/OFF
  * @retval None
  */
void Display_Main_Interface_ON_OFF(u8 State)
{
	if(State==ON)
	{
		Write_String_8x16AsicII(50, 19, " ");
		Write_String_8x16AsicII(50, 25, " ");
		Write_String_8x16AsicII(50, 21, "ON");
	}
	else
	{
		Write_String_8x16AsicII(50, 20, "OFF");
	}
}

static void Display_Main_Interface_Shift(u8 State)
{
	switch(State)
	{
		case Shift: Write_String_8x16AsicII(50, 54, "Shift"); break;
		case Not_Shift: Write_String_8x16AsicII(50, 54, "     "); break;
		default: break;
	}
}

static void Display_Main_Interface_Vint_or_Vext(u8 Use_Vint_Or_Vext_Flag)
{
	switch(Use_Vint_Or_Vext_Flag)
	{
		case Use_Vint: Write_String_8x16AsicII(34, 44, "    "); break;
		case Use_Vext: Write_String_8x16AsicII(34, 44, "Vext"); break;
		default: break;
	}
}

/*---------------------------------------显示主界面------------------------------------------*/
 /**
  * @brief  显示主界面
  * @param  电压值
  * @param  电流值
  * @param  功率值
  * @param  ON/OFF
  * @param  CV/CC/CP
  * @param  Preset/Actual
  * @param  LUCK/UNLUCK
  * @param  Shift/NotShift
  * @retval None
  */
void Display_Main_Interface(float Voltage, float Current, float Power, u8 ON_OFF, 
	                          u8 CV_CC_CP,u8 DELAY_NOT_DELAY, u8 Preset_Actual, u8 LOCK_UNLOCK, u8 Shift_NotShift, u8 Use_Vint_Or_Vext_Flag)
{
	float Oled_Voltage = Voltage;
	float Oled_Current = Current;
	float Oled_Power = Power;
	
	Display_Main_Interface_Voltage(Oled_Voltage);
	Display_Main_Interface_Current(Oled_Current);
	Display_Main_Interface_Power(Oled_Power);
	Display_Main_Interface_Lock(LOCK_UNLOCK);
	Display_Main_Interface_Preset(Preset_Actual);
	Display_Main_Interface_CV_CC_CP(CV_CC_CP,DELAY_NOT_DELAY);
	Display_Main_Interface_ON_OFF(ON_OFF);
	Display_Main_Interface_Shift(Shift_NotShift);
	Display_Main_Interface_Vint_or_Vext(Use_Vint_Or_Vext_Flag);
}

/*---------------------------------------------------------显示主界面(V-set界面)-----------------------------------------------------*/

 /**
  * @brief  显示没有电压值的主界面
  * @param  电流值
  * @param  功率值
  * @param  ON/OFF
  * @param  CV/CC/CP
  * @param  Preset/Actual
  * @param  LUCK/UNLUCK
  * @retval None
  */
void Display_Main_Interface_V_set(float Current, float Power, u8 ON_OFF, 
	                                u8 CV_CC_CP,u8 DELAY_NOT_DELAY, u8 Preset_Actual, u8 LOCK_UNLOCK, u8 Use_Vint_Or_Vext_Flag)
{
	Display_Main_Interface_Current(Current);
	Display_Main_Interface_Power(Power);
	Display_Main_Interface_Lock(LOCK_UNLOCK);
	Display_Main_Interface_Preset(Preset_Actual);
	Display_Main_Interface_CV_CC_CP(CV_CC_CP,DELAY_NOT_DELAY);
	Display_Main_Interface_ON_OFF(ON_OFF);
	Display_Main_Interface_Vint_or_Vext(Use_Vint_Or_Vext_Flag);
}

/*---------------------------------------------------------显示主界面(I-set界面)-----------------------------------------------------*/

 /**
  * @brief  显示没有电流值的主界面
  * @param  电流值
  * @param  功率值
  * @param  ON/OFF
  * @param  CV/CC/CP
  * @param  Preset/Actual
  * @param  LUCK/UNLUCK
  * @retval None
  */
void Display_Main_Interface_I_set(float Voltage, float Power, u8 ON_OFF, 
	                                u8 CV_CC_CP,u8 DELAY_NOT_DELAY, u8 Preset_Actual, u8 LOCK_UNLOCK, u8 Use_Vint_Or_Vext_Flag)
{
	Display_Main_Interface_Voltage(Voltage);
	Display_Main_Interface_Power(Power);
	Display_Main_Interface_Lock(LOCK_UNLOCK);
	Display_Main_Interface_Preset(Preset_Actual);
	Display_Main_Interface_CV_CC_CP(CV_CC_CP,DELAY_NOT_DELAY);
	Display_Main_Interface_ON_OFF(ON_OFF);	
	Display_Main_Interface_Vint_or_Vext(Use_Vint_Or_Vext_Flag);
}

/*---------------------------------------------------------显示主界面(P-set界面)-----------------------------------------------------*/

 /**
  * @brief  显示没有功率值的主界面
  * @param  电压值
  * @param  电流值
  * @param  ON/OFF
  * @param  CV/CC/CP
  * @param  Preset/Actual
  * @param  LUCK/UNLUCK
  * @retval None
  */
void Display_Main_Interface_P_set(float Voltage, float Current, u8 ON_OFF, 
	                                u8 CV_CC_CP,u8 DELAY_NOT_DELAY, u8 Preset_Actual, u8 LOCK_UNLOCK, u8 Use_Vint_Or_Vext_Flag)
{
	Display_Main_Interface_Voltage(Voltage);
	Display_Main_Interface_Current(Current);
	Display_Main_Interface_Lock(LOCK_UNLOCK);
	Display_Main_Interface_Preset(Preset_Actual);
	Display_Main_Interface_CV_CC_CP(CV_CC_CP,DELAY_NOT_DELAY);
	Display_Main_Interface_ON_OFF(ON_OFF);	
	Display_Main_Interface_Vint_or_Vext(Use_Vint_Or_Vext_Flag);
}

/*---------------------------------------------------------显示存取界面-----------------------------------------------------*/

 /**
  * @brief  显示存取界面
  * @param  Recall/Save
  * @param  要Recall/Save的位置
  * @retval None
  */
static void Display_Access_Interface_Recall_Save(u8 State,float Recall_Save_Number)
{
	char a[4];
	
	if(State==Recall)
	{
		Write_String_12x24AsicII(0, 0, "Recall");
	}
	else
	{
		Write_String_12x24AsicII(0, 0, "Save  ");
	}
	
	if(Recall_Save_Number<10)
	{
		sprintf(a, "%1.0f", Recall_Save_Number);
		Write_String_12x24AsicII(0, 23, a);
		
	}
	else
	{
		sprintf(a, "%2.0f", Recall_Save_Number);
		Write_String_12x24AsicII(0, 20, a);
	}
}

 /**
  * @brief  显示存取界面功率值
  * @param  设定的功率值，范围0到99.99W
  * @retval None
  */
static void Display_Access_Interface_Power(float Power)
{
	char a[20];
	
	Write_String_12x24AsicII(0,35,"P=");
	
	if(Power >= 1000)
	{
		sprintf(a, "%5.0f", Power);
		a[0] = '0';
	}
	else if(Power >= 100)                               
	{
		sprintf(a, "%5.1f", Power);
	}
	else if(Power >= 10)
	{
	  sprintf(a, "%5.2f", Power);
	}
	else if(Power >= 0)
	{
		sprintf(a, "%5.2f", Power);
		a[0] = '0';
	}
	else if(Power >= -10)
	{
	  sprintf(a, "%5.2f", Power);
	}
	else
	{
		sprintf(a, "%5.1f", Power);
	}	
	
	Write_String_12x24AsicII(0, 42, a);
	
	Write_Single_12x24AsicII(0,58,'W');
}

 /**
  * @brief  显示存取界面电压值 
  * @param  设定的电压值，范围0到99.99V
  * @retval None
  */
static void Display_Access_Interface_Voltage(float Voltage)
{
	char a[20];
	Write_String_12x24AsicII(40,0,"V=");
	
	
		if(Voltage>=100)                             //将float转为string
	{
		sprintf(a, "%5.1f", Voltage);
	}
	else if(Voltage>=10)
	{
		sprintf(a, "%5.2f", Voltage);
	}
	else if(Voltage>=0)
	{		
	  sprintf(a, "%5.2f", Voltage);
		a[0] = '0';
	}
	else if(Voltage>=-10)
	{
	  sprintf(a, "%5.2f", Voltage);
	}
	else 
	{
	  sprintf(a, "%5.1f", Voltage);
	}
	
	Write_String_12x24AsicII(40, 7, a);
	
	Write_Single_12x24AsicII(40,23,'V');
}

 /**
  * @brief  显示存取界面电流值 
  * @param  设定的电流值，范围0到999.99A
  * @retval None
  */
static void Display_Access_Interface_Current(float Current)
{
	char a[20];
	Write_String_12x24AsicII(40,35,"I=");
	
	if(Current>=100)                             //将float转为string
	{
		sprintf(a, "%6.2f", Current);
	}
	else if(Current>=10)
	{
		sprintf(a, "%6.2f", Current);
		a[0] = '0';
	}
	else if(Current>=0)
	{
		sprintf(a, "%6.2f", Current);
		a[0] = '0';
    a[1] = '0';		
	}
	else if(Current>=-10)
	{
		sprintf(a, "%6.3f", Current);
	}
	else
	{
  	sprintf(a, "%6.2f", Current);
	}			
	
	Write_String_12x24AsicII(40, 42, a);
	
	Write_Single_12x24AsicII(40,61,'A');
}

 /**
  * @brief  显示存取界面
  * @param  Recall/Save
  * @param  Recall/Save_Number
  * @param  频率值
  * @param  电压值
  * @param  电流值
  * @retval None
  */
void Display_Access_Interface(u8 Recall_Save, u8 Recall_Save_Number, float Power, float Voltage, float Current)
{
	Display_Access_Interface_Recall_Save(Recall_Save, Recall_Save_Number);
	Display_Access_Interface_Power(Power);
	Display_Access_Interface_Voltage(Voltage);
	Display_Access_Interface_Current(Current);
}

/*---------------------------------------------------------一级菜单功能界面-----------------------------------------------------*/

 /**
  * @brief  显示一级菜单界面
  * @param  一级菜单的功能
  * @retval None
  */
void Display_Menu_Function_Interface_First_Menu(u8 Selection)
{
	Write_String_12x24AsicII(0,0,"Function");
	switch(Selection)
	{
		case System:Write_String_16x32AsicII(25,0,"     System     ");break;
		case Timer:Write_String_16x32AsicII(25,22,"Timer");
		           Dsiplay_Solid_Rectangular(25, 0, 57, 21, 0x00);
		           Dsiplay_Solid_Rectangular(25, 42, 57, 63, 0x00);  //清除上一次的显示残留
		           break;
		case Slope:Write_String_16x32AsicII(25,22,"Slope");
		           Dsiplay_Solid_Rectangular(25, 0, 57, 21, 0x00);
		           Dsiplay_Solid_Rectangular(25, 42, 57, 63, 0x00);  //清除上一次的显示残留
		           break;
		case Communication:Write_String_16x32AsicII(25,6,"Communication");break;
		case Parallel:Write_String_16x32AsicII(25,0,"    Parallel    ");break;
		case Vint_or_Vext: Write_String_16x32AsicII(25, 2,"Voltae Sampling");
		                   Dsiplay_Solid_Rectangular(25, 0, 57, 1, 0x00);
		                   Dsiplay_Solid_Rectangular(25, 62, 57, 63, 0x00);break;
		default :break;
	}
}

/*---------------------------------------------------------二级菜单功能界面-----------------------------------------------------*/

 /**
  * @brief  显示二级菜单界面
  * @param  一级菜单的功能
  * @param  二级菜单的功能
  * @retval None
  */
void Display_Menu_Function_Interface_Second_Menu(u8 First_Selection, u8 Second_Selection)
{
	//一级菜单功能，显示在左上角
	switch(First_Selection)
	{
		case System:Write_String_12x24AsicII(0,0,"System        ");break;
		case Timer:Write_String_12x24AsicII(0,0,"Timer         ");break;
		case Slope:Write_String_12x24AsicII(0,0,"Slope         ");break;
		case Communication:Write_String_12x24AsicII(0,0,"Communication");break;
		case Parallel:Write_String_12x24AsicII(0,0,"Parallel      ");break;
		case Vint_or_Vext: Write_String_12x24AsicII(0,0,"Voltae Sampling    ");break;
		default :break;
	}
	
	//显示二级菜单功能
	switch(Second_Selection)
	{
		case Reset:Write_String_16x32AsicII(25,22,"Reset");
		           Dsiplay_Solid_Rectangular(25, 0, 57, 21, 0x00);
		           Dsiplay_Solid_Rectangular(25, 42, 57, 63, 0x00);  //清除上一次的显示残留
		           break;
		case Delay:Write_String_16x32AsicII(25,22,"Delay");
		           Dsiplay_Solid_Rectangular(25, 0, 57, 21, 0x00);
		           Dsiplay_Solid_Rectangular(25, 42, 57, 63, 0x00);  //清除上一次的显示残留
		           break;
		case Cycle:Write_String_16x32AsicII(25,22,"Cycle");
		           Dsiplay_Solid_Rectangular(25, 0, 57, 21, 0x00);
		           Dsiplay_Solid_Rectangular(25, 42, 57, 63, 0x00);  //清除上一次的显示残留
		           break;
		case V_Rise:Write_String_16x32AsicII(25,0,"     V-Rise     ");break;
		case V_Fall:Write_String_16x32AsicII(25,0,"     V-Fall     ");break;
		case I_Rise:Write_String_16x32AsicII(25,0,"     I-Rise     ");break;
		case I_Fall:Write_String_16x32AsicII(25,0,"     I-Fall     ");break;
		case RS232:Write_String_16x32AsicII(25,22,"RS232");
		           Dsiplay_Solid_Rectangular(25, 0, 57, 21, 0x00);
		           Dsiplay_Solid_Rectangular(25, 42, 57, 63, 0x00);  //清除上一次的显示残留
		           break;
		case CAN:Write_String_16x32AsicII(25,26,"CAN");
		         Dsiplay_Solid_Rectangular(25, 0, 57, 25, 0x00);
		         Dsiplay_Solid_Rectangular(25, 38, 57, 63, 0x00);  //清除上一次的显示残留
		         break;
		case Menu_Vint: Write_String_16x32AsicII(25,0,"      Vint      ");break;
		case Menu_Vext: Write_String_16x32AsicII(25,0,"      Vext      ");break;
		default: break ;
	}
}


/*---------------------------------------------------------保护界面-----------------------------------------------------------------------*/

u8 First_Into_ErrorInterface_Flag = Have_Not_Into;  //定义第一次进入错误界面的标志位
 /**
  * @brief  显示保护界面
  * @param  出现的错误类型
  * @retval None
  */
void Display_Protect_Interface(u8 Error)
{
	static u16 time = 0;
	static u8 mask = 1;
	u8 temp = Error;
	u8 old_mask;
	
	Write_String_16x32AsicII(0,22,"Error");
	
	time ++;
	
	if((time >= 30) || (First_Into_ErrorInterface_Flag == Have_Not_Into))      //换模式时间到或者第一次进入错误界面
	{
		First_Into_ErrorInterface_Flag = Have_Into;
		
		time = 0;
		
		//找到是什么错误
		mask = mask << 1;
		temp = Error & mask;
		old_mask = mask;		
		
		while(temp == 0)
		{
			//左移一位
			mask = mask << 1;
			if(mask == 0x40)
			{
				mask = 1;
			}
			
			if(old_mask == mask)  //找了一遍都没找到，退出循环
			{
				break;
			}
			
			temp = Error & mask;			
		}
	}
	
	switch(temp)
	{
		case PF:break;
		case OVP:if(OVP_InputOrOutput_Flag == 0)          Write_String_16x32AsicII(32,6,"   OVP_Input     ");
						 else if (OVP_InputOrOutput_Flag == 0)    Write_String_16x32AsicII(32,6,"   OVP_Output    ");
						 else                 										Write_String_16x32AsicII(32,6,"     OVP     ");
						 break;
		case OCP:
						if(OCP_SoftOrHard_Flag == 0) 			Write_String_16x32AsicII(32,6,"   OCP_Soft   ");
						else if(OCP_SoftOrHard_Flag == 1) Write_String_16x32AsicII(32,6,"   OCP_Hard   ");
						else 															Write_String_16x32AsicII(32,6,"     OCP     ");
						break;
		case OPP:Write_String_16x32AsicII(32,6,"     OPP     ");break;
		case OTP:
						if(OTP_SensorX == 1) 			Write_String_16x32AsicII(32,6,"    OTP_1    ");
						else if(OTP_SensorX == 2) Write_String_16x32AsicII(32,6,"    OTP_2    ");
						else if(OTP_SensorX == 3) Write_String_16x32AsicII(32,6,"    OTP_3    ");
						else 											Write_String_16x32AsicII(32,6,"    OTP_U    ");
						break;
		case Sense_Reverse:Write_String_16x32AsicII(32,6,"Sense_Reverse");break;
		default: break;
	}
}

/*------------------------------------------------综合各个界面的显示函数----------------------------------------*/

 /**
  * @brief  综合各个界面的显示函数
  * @param  需要显示的界面
  * @retval None
  */
void Display_Interface(u8 Display_Interface_Mode)
{
	if(Last_Interface != Display_Interface_Mode)
	{
		Fill_RAM(0x00);
	}
	
	switch(Display_Interface_Mode)
	{
		case Display_Interface_Mode_Main_Interface_Actual: Display_Main_Interface(Voltage, 
			                                                                        Current, 
																																							Power, 
																																							ON_OFF, CV_CC_CP, DELAY_NOT_DELAY, Actual, LOCK_UNLOCK, Shift_Not_Shift, Use_Vint_Or_Vext_Flag); //显示真实值主界面
		                                                    Last_Interface = Display_Interface_Mode_Main_Interface_Actual;
		                                                    break;
		
		case Display_Interface_Mode_Main_Interface_Preset: Display_Main_Interface(Set_Voltage, Set_Current, Set_Power, ON_OFF,                   //显示设定值主界面
			                                                                        CV_CC_CP, DELAY_NOT_DELAY, Preset, LOCK_UNLOCK, Shift_Not_Shift, Use_Vint_Or_Vext_Flag);
		                                                    Last_Interface = Display_Interface_Mode_Main_Interface_Preset;
		                                                   break;
		
		case Display_Interface_Mode_Main_Interface_V_set: break;       //如果为电压设定模式，就不执行这条函数，改由TIM5执行显示功能
		
		case Display_Interface_Mode_Main_Interface_I_set: break;       //如果为电流设定模式，就不执行这条函数，改由TIM5执行显示功能
		
		case Display_Interface_Mode_Main_Interface_P_set: break;       //如果为功率设定模式，就不执行这条函数，改由TIM5执行显示功能
		
		case Display_Interface_Mode_First_Menu: Display_Menu_Function_Interface_First_Menu(First_Selection);          //显示一级菜单
		                                        Last_Interface = Display_Interface_Mode_First_Menu;
		                                        break;
		
		case Display_Interface_Mode_Second_Menu: Display_Menu_Function_Interface_Second_Menu(First_Selection, Second_Selection);     //显示二级菜单
		                                         Last_Interface = Display_Interface_Mode_Second_Menu;
		                                         break;
	  
		case Display_Interface_Mode_Third_Menu: break;  //如果为第三级菜单模式，就不执行这条函数，改由TIM5执行显示功能
		
		case Display_Interface_Mode_Protect_Interface: Display_Protect_Interface(Error);                               //显示保护界面
		                                               Last_Interface = Display_Interface_Mode_Protect_Interface;
		                                               break;
		
		case Display_Interface_Mode_Recall_Interface: Display_Access_Interface(Recall, Recall_Save_Number,              //显示取界面 
			                                            Recall_Save_Display_Power, Recall_Save_Display_Voltage, Recall_Save_Display_Current);
		                                              Last_Interface = Display_Interface_Mode_Recall_Interface;
		                                              break;
																									
		case Display_Interface_Mode_Save_Interface: Display_Access_Interface(Save, Recall_Save_Number,                  //显示存界面
			                                          Recall_Save_Display_Power, Recall_Save_Display_Voltage, Recall_Save_Display_Current);
		                                            Last_Interface = Display_Interface_Mode_Save_Interface;
																								break;
			
		default: break;
	}
}










