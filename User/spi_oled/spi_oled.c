#include "spi_oled.h"

   
u8 Error;
u8 Last_Interface;

#define set_OLED GPIO_SetBits(GPIOA, GPIO_Pin_12)
#define res_OLED GPIO_ResetBits(GPIOA, GPIO_Pin_12)



extern u8 OCP_SoftOrHard_Flag;//OCPӲ�����������������
extern u8 OTP_SensorX;				//OTP�������ĸ�������

float Recall_Save_Display_Voltage;  //��ʾ�Ĵ�ȡ��ѹ
float Recall_Save_Display_Current;  //��ʾ�Ĵ�ȡ����
float Recall_Save_Display_Power;    //��ʾ�Ĵ�ȡ����

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
  * @brief  SPI_OLED��ʼ��
  * @param  ��
  * @retval ��
  */
void SPI_FUN(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	
	/*- ����GPIOB�Լ�SPI3ʱ�� -*/
  OLED_SPI_CS_APBxClock_FUN(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
  OLED_SPI_APBxClock_FUN(OLED_SPI_CLK, ENABLE);
	
	//��JTAG
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SPI3,ENABLE);
	
	/*- SPI3���� - SCLK��MOSI -*/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	
	GPIO_InitStructure.GPIO_Pin =OLED_SPI_SCK_PIN;
  GPIO_Init(OLED_SPI_SCK_PORT, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = OLED_SPI_MOSI_PIN;
	GPIO_Init(OLED_SPI_MOSI_PORT, &GPIO_InitStructure);
	
	/*- Ƭѡ����->PB10�����������л�->PB11 -*/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	
	GPIO_InitStructure.GPIO_Pin = OLED_SPI_CS_PIN;
  GPIO_Init(OLED_SPI_CS_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = OLED_SPI_DC_PIN;
	GPIO_Init(OLED_SPI_DC_PORT, &GPIO_InitStructure);
	
	/*AT25:CS PD2����*/
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//CS
  GPIO_Init(GPIOD, &GPIO_InitStructure);

	/*- SPI3���� -*/
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;	//ȫ˫��ģʽ
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	 										  //��ģʽ
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	 								  //���ݴ�С8λ
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		 										  //ʱ�Ӽ��ԣ�����ʱΪ��
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;											  //��2��������Ч��������Ϊ����ʱ��
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		   										  //NSS�ź����������
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; 	//���÷�Ƶ������Ƶ��=36M/��Ƶ��
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  							  //��λ��ǰ
	SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(OLED_SPIx, &SPI_InitStructure);														//��ʼ���ṹ��

  /* ʹ�� SPI  */
  SPI_Cmd(OLED_SPIx , ENABLE);
	
}

 /**
  * @brief  ʹ��SPI����һ���ֽڵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
  */
static u8 SPI_SendByte(u8 byte)
{
	
  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
  while (SPI_I2S_GetFlagStatus(OLED_SPIx , SPI_I2S_FLAG_TXE) == RESET)
	{
		
	}

  /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
  SPI_I2S_SendData(OLED_SPIx , byte);
	
	/*�ȴ����ܻ������ǿ�*/
	while (SPI_I2S_GetFlagStatus(OLED_SPIx, SPI_I2S_FLAG_RXNE) == RESET)
	{
		
	}
	
	return SPI_I2S_ReceiveData(OLED_SPIx);	
}


 /**
  * @brief  ��OLED��������
  * @param  byte��Ҫ���͵�����
  * @retval None
  */
void SPI_OLED_SendData(u8 byte)
{
	/* ѡ�� OLED: CS �� */
  SPI_OLED_CS_LOW();
	
	//ѡ��д����ģʽ
	SPI_OLED_DC_HIGH();
	
	//д������
	SPI_SendByte(byte);
	
	//ѡ��д����ģʽ
	SPI_OLED_DC_HIGH();
	
	/* ֹͣ�ź� OLED: CS �ߵ�ƽ */
  SPI_OLED_CS_HIGH();
}


 /**
  * @brief  ��OLED��������
  * @param  byte��Ҫ���͵�����
  * @retval None
  */
void SPI_OLED_SendCommond(uint8_t byte)
{
	/* ѡ�� OLED: CS �� */
  SPI_OLED_CS_LOW();
	
	//ѡ��д����ģʽ
	SPI_OLED_DC_LOW();
	
	//д������
	SPI_SendByte(byte);
	
	//ѡ��д����ģʽ
	SPI_OLED_DC_HIGH();
	
	/* ֹͣ�ź� OLED: CS �ߵ�ƽ */
  SPI_OLED_CS_HIGH();
}


 /**
  * @brief  OLED��ʼ��
  */
void OLED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//������ų�ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	res_OLED;
	SysTick_Delay_Ms(1);
	set_OLED;
	SysTick_Delay_Ms(5);
	
	SPI_OLED_SendCommond(Set_Command_Lock);//����
	SPI_OLED_SendData(0x12);
	
	SPI_OLED_SendCommond(Set_Column_Address_Commond);//�����е�ַ
	SPI_OLED_SendData(0x1c);
	SPI_OLED_SendData(0x5b);
	
	SPI_OLED_SendCommond(Set_Row_Address_Commond);//�����е�ַ
	SPI_OLED_SendData(0x00);
	SPI_OLED_SendData(0x3f);
	
	SPI_OLED_SendCommond(Sleep_mode_ON);//����

	SPI_OLED_SendCommond(Set_Front_Clock_Divider);//����ǰʱ�ӷ�Ƶ��/����Ƶ��
	SPI_OLED_SendData(0x91);
	
	SPI_OLED_SendCommond(Set_MUX_Ratio);//���ö�·ѡ�񿪹ر�ֵ
	SPI_OLED_SendData(0x3f);
	
	SPI_OLED_SendCommond(Set_Display_Offset);//��0-127���ô�ֱ����
	SPI_OLED_SendData(0x00);
	
	SPI_OLED_SendCommond(Set_Display_Start_Line);//����������
	SPI_OLED_SendData(0x00);
	
	SPI_OLED_SendCommond(Normal_Display);//����������ʾ
	
	SPI_OLED_SendCommond(Set_Re_map_and_Dual_COM_Line_mode);//����ɨ�跽��
	SPI_OLED_SendData(0x14);
	SPI_OLED_SendData(0x11);
	
	SPI_OLED_SendCommond(Set_GPIO);//�����趨
	SPI_OLED_SendData(0x00);
	
	SPI_OLED_SendCommond(Function_Selection);//�ڲ���ѹ����
	SPI_OLED_SendData(0x01);
	
	SPI_OLED_SendCommond(0xb4);
	SPI_OLED_SendData(0xa0);
	SPI_OLED_SendData(0xfd);
	
	SPI_OLED_SendCommond(Set_Contrast_Current);//���öԱȶȵ���
	SPI_OLED_SendData(0xaf);
	
	SPI_OLED_SendCommond(Master_Contrast_Current_Control);//���Աȶȵ�������
	SPI_OLED_SendData(0x0f);
	
	SPI_OLED_SendCommond(Set_Gray_Scale_Table);//���ûҶȱ�
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
	
	SPI_OLED_SendCommond(Enable_Gray_Scale_table);//�����Ҷȱ�
	
	SPI_OLED_SendCommond(Set_Phase_Length);//������λ����
	SPI_OLED_SendData(0xE2);
	
	SPI_OLED_SendCommond(0xD1); 								//Display Enhancement B
	SPI_OLED_SendData(0xa2);
	SPI_OLED_SendData(0x20);										//����
	
	SPI_OLED_SendCommond(Set_Pre_charge_voltage);//����Ԥ����ѹ
	SPI_OLED_SendData(0x1F);
	
	SPI_OLED_SendCommond(Set_Second_Precharge_Period);//���õڶ�Ԥ���ڼ�
	SPI_OLED_SendData(0x08);
	
	SPI_OLED_SendCommond(Set_VCOMH );//����COMȡ��ѡ���ѹ�ȼ�
	SPI_OLED_SendData(0x07);
	
	SPI_OLED_SendCommond(Normal_Display);//������ʾ
	
	SPI_OLED_SendCommond(Exit_Partial_Display);//��������͵��˳�������ʾģʽ
	
	SPI_OLED_SendCommond(Write_RAM_Command);
	
	 Fill_RAM(0x00);   //����
	
	SPI_OLED_SendCommond(Sleep_mode_OFF);//������
}

/*----------------------------------------------��ʾ����-----------------------------------------------*/

 /**
  * @brief  �����е�ַ
  * @param  byte����ʼ��ַ
  * @param  byte��������ַ
  * @retval None
  */
void Set_Column_Address(u8 a, u8 b)
{
	SPI_OLED_SendCommond(Set_Column_Address_Commond);			// Set Column Address
	SPI_OLED_SendData(0x1c+a);					                  //0x1c��ַƫ����
	SPI_OLED_SendData(0x1c+b);				                   
}

 /**
  * @brief  �����е�ַ
  * @param  byte����ʼ��ַ
  * @param  byte��������ַ
  * @retval None
  */
void Set_Row_Address(u8 a, u8 b)
{
	SPI_OLED_SendCommond(Set_Row_Address_Commond);			// Set Row Address
	SPI_OLED_SendData(a);					//   Default => 0x00
	SPI_OLED_SendData(b);					//   Default => 0x7F
}

 /**
  * @brief  д����
  * @param  None
  * @retval None
  */
void Set_WriteData(void)
{
	SPI_OLED_SendCommond(Write_RAM_Command);			// Enable MCU to Write into RAM
}

 /**
  * @brief  д˳��
  * @param  A[0]=0 ˮƽɨ�裻A[0]=1 ��ֱɨ��
            A[1]����������λ�����˳��
            A[2]����һ��������λ��˳��
            A[5]����COM����ż����
            �������һ������Ϊ0x14
  * @retval None
  */
void Set_Remap_Format(unsigned char A)
{
	SPI_OLED_SendCommond(Set_Re_map_and_Dual_COM_Line_mode);			// Set Re-Map / Dual COM Line Mode
	SPI_OLED_SendData(A);					                                //   Default => 0x40
	SPI_OLED_SendData(0x11);	   	                               	//   Default => 0x01 (Disable Dual COM Mode)
}

 /**
  * @brief  ������ʾ
  * @param  Ҫ������ʾ����ɫ
  * @retval None
  */
void Fill_RAM(u8 Data)
{
	unsigned char i,j;

	Set_Column_Address(0x00,0x3f);//0��63��    
	Set_Row_Address(0x00,0x3f);   //0��63��
	Set_WriteData();

	for(i=0;i<64;i++)             //һ��64��
	{
		for(j=0;j<64;j++)           //һ��64��
		{
			SPI_OLED_SendData(Data);  //һ���������ص�
			SPI_OLED_SendData(Data);
		}
	}
}

 /**
  * @brief  �м����ʾ
  * @param  Ҫ�м����ʾ����ɫ
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
  * @brief  ������ʾ
  * @param  Ҫ�����ʾ�ķ�ʽ
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
  * @brief  ��һ������
  * @param  ��ʼ����е�ַ
  * @param  ��ʼ����е�ַ
  * @param  ��������е�ַ
  * @retval None
  */
void Display_Horizontal_Line(u8 Row, u8 Start_Column, u8 End_Column)
{
	u8 i;
	
	Set_Remap_Format(0x14);                      //��ɨ��ģʽ
	Set_Column_Address(Start_Column, End_Column);//�����е�ַ
	Set_Row_Address(Row, Row);                   //�����е�ַ
	
	Set_WriteData();//дʹ��
	
		for(i=0;i<End_Column-Start_Column+1;i++)
	{
		SPI_OLED_SendData(0xff);
		SPI_OLED_SendData(0xff);
	}
}





 /**
  * @brief  ��һ����������1(��Ϊһ�����ĸ�����)
  * @param  ��ʼ����е�ַ
  * @param  ��ʼ����е�ַ
  * @param  ��������е�ַ
  * @retval None
  */
void Display_Vertical_Line_1(u8 Column, u8 Start_Row, u8 End_Row)
{
	u8 i;
	
	Set_Remap_Format(0x15);                //��ɨ��ģʽ
	Set_Column_Address(Column, Column);    //�����е�ַ
	Set_Row_Address(Start_Row, End_Row);   //�����е�ַ
	
	Set_WriteData();//дʹ��
	
	for(i=0; i<End_Row-Start_Row+1; i++)
	{
		SPI_OLED_SendData(0xf0);
		SPI_OLED_SendData(0x00);
	}
	Set_Remap_Format(0x14);                //��ɨ��ģʽ
}

 /**
  * @brief  ��һ����������2
  * @param  ��ʼ����е�ַ
  * @param  ��ʼ����е�ַ
  * @param  ��������е�ַ
  * @retval None
  */
void Display_Vertical_Line_2(u8 Column, u8 Start_Row, u8 End_Row)
{
	u8 i;
	
	Set_Remap_Format(0x15);                //��ɨ��ģʽ
	Set_Column_Address(Column, Column);    //�����е�ַ
	Set_Row_Address(Start_Row, End_Row);   //�����е�ַ
	
	Set_WriteData();//дʹ��
	
	for(i=0; i<End_Row-Start_Row+1; i++)
	{
		SPI_OLED_SendData(0x0f);
		SPI_OLED_SendData(0x00);
	}
	Set_Remap_Format(0x14);                //��ɨ��ģʽ
}

 /**
  * @brief  ��һ����������3
  * @param  ��ʼ����е�ַ
  * @param  ��ʼ����е�ַ
  * @param  ��������е�ַ
  * @retval None
  */
void Display_Vertical_Line_3(u8 Column, u8 Start_Row, u8 End_Row)
{
	u8 i;
	
	Set_Remap_Format(0x15);                //��ɨ��ģʽ
	Set_Column_Address(Column, Column);    //�����е�ַ
	Set_Row_Address(Start_Row, End_Row);   //�����е�ַ
	
	Set_WriteData();//дʹ��
	
	for(i=0; i<End_Row-Start_Row+1; i++)
	{
		SPI_OLED_SendData(0x00);
		SPI_OLED_SendData(0xf0);
	}
	Set_Remap_Format(0x14);                //��ɨ��ģʽ
}

 /**
  * @brief  ��һ����������4
  * @param  ��ʼ����е�ַ
  * @param  ��ʼ����е�ַ
  * @param  ��������е�ַ
  * @retval None
  */
void Display_Vertical_Line_4(u8 Column, u8 Start_Row, u8 End_Row)
{
	u8 i;
	
	Set_Remap_Format(0x15);                //��ɨ��ģʽ
	Set_Column_Address(Column, Column);    //�����е�ַ
	Set_Row_Address(Start_Row, End_Row);   //�����е�ַ
	
	Set_WriteData();//дʹ��
	
	for(i=0; i<End_Row-Start_Row+1; i++)
	{
		SPI_OLED_SendData(0x00);
		SPI_OLED_SendData(0x0f);
	}
	Set_Remap_Format(0x14);                //��ɨ��ģʽ
}


 /**
  * @brief  ��һ�����α߿�
  * @param  ��ʼ����е�ַ
  * @param  ��ʼ����е�ַ
  * @param  ��������е�ַ
  * @param  ��������е�ַ
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
  * @brief  ��һ�������С����ɫ��ʵ�ľ���
  * @param  ��ʼ�С��У������С��У���ɫ
  * @retval None
  */
void Dsiplay_Solid_Rectangular(u8 Start_Row, u8 Start_Column, u8 End_Row, u8 End_Column, u8 Colour)
{
	u16 i;  //ѭ��д�����
	
	Set_Remap_Format(0x14);                //��ɨ��ģʽ
	Set_Column_Address(Start_Column, End_Column);    //�����е�ַ
	Set_Row_Address(Start_Row, End_Row);   //�����е�ַ
	
	Set_WriteData();//дʹ��
	
		for(i=0; i<(End_Row-Start_Row+1)*(End_Column-Start_Column+1); i++)
	{
		SPI_OLED_SendData(Colour);
		SPI_OLED_SendData(Colour);
	}
}

 /**
  * @brief  ת����������һλתΪ��λ��SD1322��λ��ʾһ�����ص㣩,��д��SD1322
  * @param  Ҫת��������
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
		           + *(u32 *)(0x22000000+ ((u32)(&Data)-0x20000000)*32 +(j-1)*4)*0x0f; //����ģ��λ����ַ����ȡ���ݣ���������д��0��F
  	SPI_OLED_SendData(data_4byte);
		j--;
		j--;
  }
}

 /**
  * @brief  дһ��8x16���֡���ĸ(ASIC II)
  * @param  �У��У�Ҫд������֡���ĸ
  * @retval None
  */
void Write_Single_8x16AsicII(u8 Row, u8 Column, char Data)
{
	u8 i;
	u16 k;
	
	SPI_OLED_SendCommond(Set_Re_map_and_Dual_COM_Line_mode);//����ɨ�跽��
	SPI_OLED_SendData(0x14);
	SPI_OLED_SendData(0x11);
	
	Set_Column_Address(Column, Column+1);    //�����е�ַ
	Set_Row_Address(Row, Row+15);            //�����е�ַ
	
	Set_WriteData();
	
	k=(Data-' ')*16;//��Կո��ƫ����
	
	for(i=0;i<16;i++)
	{
		Font_1Bit_To_4Bit(Fonts_AsicII_8x16[k]);
		k++;
	}
}

 /**
  * @brief  дһ��8x16���֡���ĸ(ASIC II)
  * @param  �У��У�Ҫд������֡���ĸ
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
  * @brief  дһ��16x16����
  * @param  �У��У�Ҫд�������(����Ҫ�ӿո�)
  * @retval None
  */
void Write_Single_16x16Chinese(u8 Row, u8 Column, char Data[])
{
	u8 i;
	u16 k;
	
	k=Chinese_Pinyin_to_16x16Fonts(Data);
	k=k*32;
	
	SPI_OLED_SendCommond(Set_Re_map_and_Dual_COM_Line_mode);//����ɨ�跽��
	SPI_OLED_SendData(0x14);
	SPI_OLED_SendData(0x11);
	
	Set_Column_Address(Column, Column+3);    //�����е�ַ
	Set_Row_Address(Row, Row+15);            //�����е�ַ
	
	Set_WriteData();
	
	for(i=0;i<32;i++)
	{
	 Font_1Bit_To_4Bit( Fonts_16x16Chinese[k+i]);
	}
}

 /**
  * @brief  ���ַ�����һ���ָ���һ���ַ���,�÷�ͬStrncpy����������Strncpy��ȱ��
  * @param  Ŀ���ַ���
  * @param  ԭ�ַ����������ʼ��Ա
  * @param  ����
  * @retval None
  */
char *myStrncpy(char *dest, const char *src, size_t n)
{
  int size = sizeof(char)*(n+1);
  char *tmp = (char*)malloc(size);  // ���ٴ�СΪn+1����ʱ�ڴ�tmp
  if(tmp)
		{
      memset(tmp, '\0', size);  // ���ڴ��ʼ��Ϊ0
      memcpy(tmp, src, size-1);  // ��src��ǰn���ֽڿ�����tmp
      memcpy(dest, tmp, size);  // ����ʱ�ռ�tmp�����ݿ�����dest
      free(tmp);  // �ͷ��ڴ�
      return dest;
    }
		else
		{
      return NULL;
    }
 }


 /**
  * @brief  дһ��16x16����
  * @param  �У��У�Ҫд�������(����Ҫ�ӿո�)
  * @retval None
  */
void Write_String_16x16Chinese(u8 Row, u8 Column, char String_Data[])
{	
	u16 i=0;
	u16 j;
	u16 k=0;
	u16 m=0;
	
	char* Single_Data=NULL;       //��������ָ��
	
	j=strlen(String_Data);        //�������ַ����ĳ���
	
	for(;i<j;)                    //i<j��ʾ��û��ȫ������
	{
	  for(k=0;String_Data[i]!=' ';i++,k++)   //k��ʾһ���ֵ��ַ�������
	  {
	  }
	  i=i+1;
	  m=m+1;                     //m��ʾ�ֵĸ���
	
	  Single_Data=(char*)malloc(k+1);    //����һ��k�ֽڵ��ڴ�ռ�
	  if(Single_Data)                    //�ж��Ƿ񿪱ٳɹ�
			
	  myStrncpy(Single_Data,&String_Data[i-k-1],k+1);    //������ִ�ԭ�ַ�����ȡ����
	
	
  	Write_Single_16x16Chinese(Row, Column+4*(m-1), Single_Data);    //д��SD1366
	
	  free(Single_Data);   //�ͷ��ڴ�
	  Single_Data=NULL;    //ָ�붨��
	}
}


 /**
  * @brief  дһ��16x32���֡���ĸ(ASIC II)
  * @param  �У��У�Ҫд������֡���ĸ
  * @retval None
  */
void Write_Single_16x32AsicII(u8 Row, u8 Column, char Data)
{
	u8 i;
	u16 k;
	
	SPI_OLED_SendCommond(Set_Re_map_and_Dual_COM_Line_mode);//����ɨ�跽��
	SPI_OLED_SendData(0x14);
	SPI_OLED_SendData(0x11);
	
	Set_Column_Address(Column, Column+3);    //�����е�ַ
	Set_Row_Address(Row, Row+31);            //�����е�ַ
	
	Set_WriteData();
	
	k=(Data-' ')*64;//��Կո��ƫ����
	
	for(i=0;i<64;i++)
	{
		Font_1Bit_To_4Bit(Fonts_AsicII_16x32[k]);
		k++;
	}
}

 /**
  * @brief  дһ��16x32���֡���ĸ(ASIC II)   �ֱ���256*64��16*24==��16����32
  * @param  �У��У�Ҫд������֡���ĸ         Row==�е����Ͻ�   Column==�е���ߣ��ܹ�64��
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
  * @brief  12x24ר�õ�ת����������һλתΪ��λ��SD1322��λ��ʾһ�����ص㣩,��д��SD1322
  * @param  Ҫת��������
  * @retval None
  */
static void Font_1Bit_To_4Bit_12x24(u8 Data0, u8 Data1)
{
	u8 data_4byte;
  u8 i;
  u8 j;

	//ȡData0
  for(i=0,j=7; i<4; i++)
  {
   	data_4byte=*(u32 *)(0x22000000+ ((u32)(&Data0)-0x20000000)*32 +j*4)*0xf0 
		           + *(u32 *)(0x22000000+ ((u32)(&Data0)-0x20000000)*32 +(j-1)*4)*0x0f; //����ģ��λ����ַ����ȡ���ݣ���������д��0��F
  	SPI_OLED_SendData(data_4byte);
		j--;
		j--;
  }
	
	//ȡData1�ĸ���λ
	for(i=0,j=7; i<2; i++)
	{
		data_4byte=*(u32 *)(0x22000000+ ((u32)(&Data1)-0x20000000)*32 +j*4)*0xf0 
		           + *(u32 *)(0x22000000+ ((u32)(&Data1)-0x20000000)*32 +(j-1)*4)*0x0f; //����ģ��λ����ַ����ȡ���ݣ���������д��0��F
  	SPI_OLED_SendData(data_4byte);
		j--;
		j--;
	}
}



 /**
  * @brief  дһ��12x24���֡���ĸ(ASIC II)
  * @param  �У��У�Ҫд������֡���ĸ
  * @retval None
  */
void Write_Single_12x24AsicII(u8 Row, u8 Column, char Data)
{
	u8 i;
	u16 k;
	
	SPI_OLED_SendCommond(Set_Re_map_and_Dual_COM_Line_mode);//����ɨ�跽��
	SPI_OLED_SendData(0x14);
	SPI_OLED_SendData(0x11);
	
	Set_Column_Address(Column, Column+2);    //�����е�ַ
	Set_Row_Address(Row, Row+23);            //�����е�ַ
	
	Set_WriteData();
	
	k=(Data-' ')*48;//��Կո��ƫ����
	
	for(i=0;i<24;i++)
	{
		Font_1Bit_To_4Bit_12x24(Fonts_AsicII_12x24[k],Fonts_AsicII_12x24[k+1]);
		k=k+2;
	}
}

 /**
  * @brief  дһ��12x24���֡���ĸ(ASIC II)
  * @param  �У��У�Ҫд������֡���ĸ
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

/*---------------------------------------------------------��ʾ������(��ʵֵ������趨ֵ�鿴����)-----------------------------------------------------*/

char String_Voltage[20];                       //����һ�����ת���ĵ�ѹ�ַ���������,Ҳ���ں����趨��ѹֵʱ����ʾ
float ADC_temp = 100;


 /**
  * @brief  ����ѹֵ��ʾ����
  * @param  ��ѹֵ
  * @retval None
  */
void Display_Main_Interface_Voltage(float Voltage)
{	
	if(Voltage>=100)                             //��floatתΪstring
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

char String_Current[20];                       //����һ�����ת���ĵ����ַ���������,Ҳ���ں����趨����ֵʱ����ʾ

 /**
  * @brief  ������ֵ��ʾ����
  * @param  ����ֵ
  * @retval None
  */
void Display_Main_Interface_Current(float Current)
{
	if(Current>=100)                             //��floatתΪstring
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

char String_Power[20];                       //����һ�����ת���Ĺ����ַ���������,Ҳ���ں����趨����ֵʱ����ʾ

 /**
  * @brief  ������ֵ��ʾ����
  * @param  ����ֵ
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
  * @brief  ��ʾ��������״̬
  * @param  ��ʾON������ʾOFF
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
  * @brief  ��ʾ�����趨/ʵ��ֵ״̬
  * @param  �趨ֵ״̬Preset/ʵ��ֵ״̬Actual
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
  * @brief  ��ʾ����CV/CC/CP��DELAY״̬
  * @param  CV/CC/CP
  * @param  DELAY/NOT_DELAY
  * @retval None
  */
void Display_Main_Interface_CV_CC_CP(u8 CV_CC_CP,u8 DELAY_NOT_DELAY)
{
	u8 State;
	State=CV_CC_CP | DELAY_NOT_DELAY;                                    //����DELAY״̬ʱ��ֻ��ʾDELAY
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
  * @brief  ��ʾ����ON/OFF״̬
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

/*---------------------------------------��ʾ������------------------------------------------*/
 /**
  * @brief  ��ʾ������
  * @param  ��ѹֵ
  * @param  ����ֵ
  * @param  ����ֵ
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

/*---------------------------------------------------------��ʾ������(V-set����)-----------------------------------------------------*/

 /**
  * @brief  ��ʾû�е�ѹֵ��������
  * @param  ����ֵ
  * @param  ����ֵ
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

/*---------------------------------------------------------��ʾ������(I-set����)-----------------------------------------------------*/

 /**
  * @brief  ��ʾû�е���ֵ��������
  * @param  ����ֵ
  * @param  ����ֵ
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

/*---------------------------------------------------------��ʾ������(P-set����)-----------------------------------------------------*/

 /**
  * @brief  ��ʾû�й���ֵ��������
  * @param  ��ѹֵ
  * @param  ����ֵ
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

/*---------------------------------------------------------��ʾ��ȡ����-----------------------------------------------------*/

 /**
  * @brief  ��ʾ��ȡ����
  * @param  Recall/Save
  * @param  ҪRecall/Save��λ��
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
  * @brief  ��ʾ��ȡ���湦��ֵ
  * @param  �趨�Ĺ���ֵ����Χ0��99.99W
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
  * @brief  ��ʾ��ȡ�����ѹֵ 
  * @param  �趨�ĵ�ѹֵ����Χ0��99.99V
  * @retval None
  */
static void Display_Access_Interface_Voltage(float Voltage)
{
	char a[20];
	Write_String_12x24AsicII(40,0,"V=");
	
	
		if(Voltage>=100)                             //��floatתΪstring
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
  * @brief  ��ʾ��ȡ�������ֵ 
  * @param  �趨�ĵ���ֵ����Χ0��999.99A
  * @retval None
  */
static void Display_Access_Interface_Current(float Current)
{
	char a[20];
	Write_String_12x24AsicII(40,35,"I=");
	
	if(Current>=100)                             //��floatתΪstring
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
  * @brief  ��ʾ��ȡ����
  * @param  Recall/Save
  * @param  Recall/Save_Number
  * @param  Ƶ��ֵ
  * @param  ��ѹֵ
  * @param  ����ֵ
  * @retval None
  */
void Display_Access_Interface(u8 Recall_Save, u8 Recall_Save_Number, float Power, float Voltage, float Current)
{
	Display_Access_Interface_Recall_Save(Recall_Save, Recall_Save_Number);
	Display_Access_Interface_Power(Power);
	Display_Access_Interface_Voltage(Voltage);
	Display_Access_Interface_Current(Current);
}

/*---------------------------------------------------------һ���˵����ܽ���-----------------------------------------------------*/

 /**
  * @brief  ��ʾһ���˵�����
  * @param  һ���˵��Ĺ���
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
		           Dsiplay_Solid_Rectangular(25, 42, 57, 63, 0x00);  //�����һ�ε���ʾ����
		           break;
		case Slope:Write_String_16x32AsicII(25,22,"Slope");
		           Dsiplay_Solid_Rectangular(25, 0, 57, 21, 0x00);
		           Dsiplay_Solid_Rectangular(25, 42, 57, 63, 0x00);  //�����һ�ε���ʾ����
		           break;
		case Communication:Write_String_16x32AsicII(25,6,"Communication");break;
		case Parallel:Write_String_16x32AsicII(25,0,"    Parallel    ");break;
		case Vint_or_Vext: Write_String_16x32AsicII(25, 2,"Voltae Sampling");
		                   Dsiplay_Solid_Rectangular(25, 0, 57, 1, 0x00);
		                   Dsiplay_Solid_Rectangular(25, 62, 57, 63, 0x00);break;
		default :break;
	}
}

/*---------------------------------------------------------�����˵����ܽ���-----------------------------------------------------*/

 /**
  * @brief  ��ʾ�����˵�����
  * @param  һ���˵��Ĺ���
  * @param  �����˵��Ĺ���
  * @retval None
  */
void Display_Menu_Function_Interface_Second_Menu(u8 First_Selection, u8 Second_Selection)
{
	//һ���˵����ܣ���ʾ�����Ͻ�
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
	
	//��ʾ�����˵�����
	switch(Second_Selection)
	{
		case Reset:Write_String_16x32AsicII(25,22,"Reset");
		           Dsiplay_Solid_Rectangular(25, 0, 57, 21, 0x00);
		           Dsiplay_Solid_Rectangular(25, 42, 57, 63, 0x00);  //�����һ�ε���ʾ����
		           break;
		case Delay:Write_String_16x32AsicII(25,22,"Delay");
		           Dsiplay_Solid_Rectangular(25, 0, 57, 21, 0x00);
		           Dsiplay_Solid_Rectangular(25, 42, 57, 63, 0x00);  //�����һ�ε���ʾ����
		           break;
		case Cycle:Write_String_16x32AsicII(25,22,"Cycle");
		           Dsiplay_Solid_Rectangular(25, 0, 57, 21, 0x00);
		           Dsiplay_Solid_Rectangular(25, 42, 57, 63, 0x00);  //�����һ�ε���ʾ����
		           break;
		case V_Rise:Write_String_16x32AsicII(25,0,"     V-Rise     ");break;
		case V_Fall:Write_String_16x32AsicII(25,0,"     V-Fall     ");break;
		case I_Rise:Write_String_16x32AsicII(25,0,"     I-Rise     ");break;
		case I_Fall:Write_String_16x32AsicII(25,0,"     I-Fall     ");break;
		case RS232:Write_String_16x32AsicII(25,22,"RS232");
		           Dsiplay_Solid_Rectangular(25, 0, 57, 21, 0x00);
		           Dsiplay_Solid_Rectangular(25, 42, 57, 63, 0x00);  //�����һ�ε���ʾ����
		           break;
		case CAN:Write_String_16x32AsicII(25,26,"CAN");
		         Dsiplay_Solid_Rectangular(25, 0, 57, 25, 0x00);
		         Dsiplay_Solid_Rectangular(25, 38, 57, 63, 0x00);  //�����һ�ε���ʾ����
		         break;
		case Menu_Vint: Write_String_16x32AsicII(25,0,"      Vint      ");break;
		case Menu_Vext: Write_String_16x32AsicII(25,0,"      Vext      ");break;
		default: break ;
	}
}


/*---------------------------------------------------------��������-----------------------------------------------------------------------*/

u8 First_Into_ErrorInterface_Flag = Have_Not_Into;  //�����һ�ν���������ı�־λ
 /**
  * @brief  ��ʾ��������
  * @param  ���ֵĴ�������
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
	
	if((time >= 30) || (First_Into_ErrorInterface_Flag == Have_Not_Into))      //��ģʽʱ�䵽���ߵ�һ�ν���������
	{
		First_Into_ErrorInterface_Flag = Have_Into;
		
		time = 0;
		
		//�ҵ���ʲô����
		mask = mask << 1;
		temp = Error & mask;
		old_mask = mask;		
		
		while(temp == 0)
		{
			//����һλ
			mask = mask << 1;
			if(mask == 0x40)
			{
				mask = 1;
			}
			
			if(old_mask == mask)  //����һ�鶼û�ҵ����˳�ѭ��
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

/*------------------------------------------------�ۺϸ����������ʾ����----------------------------------------*/

 /**
  * @brief  �ۺϸ����������ʾ����
  * @param  ��Ҫ��ʾ�Ľ���
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
																																							ON_OFF, CV_CC_CP, DELAY_NOT_DELAY, Actual, LOCK_UNLOCK, Shift_Not_Shift, Use_Vint_Or_Vext_Flag); //��ʾ��ʵֵ������
		                                                    Last_Interface = Display_Interface_Mode_Main_Interface_Actual;
		                                                    break;
		
		case Display_Interface_Mode_Main_Interface_Preset: Display_Main_Interface(Set_Voltage, Set_Current, Set_Power, ON_OFF,                   //��ʾ�趨ֵ������
			                                                                        CV_CC_CP, DELAY_NOT_DELAY, Preset, LOCK_UNLOCK, Shift_Not_Shift, Use_Vint_Or_Vext_Flag);
		                                                    Last_Interface = Display_Interface_Mode_Main_Interface_Preset;
		                                                   break;
		
		case Display_Interface_Mode_Main_Interface_V_set: break;       //���Ϊ��ѹ�趨ģʽ���Ͳ�ִ����������������TIM5ִ����ʾ����
		
		case Display_Interface_Mode_Main_Interface_I_set: break;       //���Ϊ�����趨ģʽ���Ͳ�ִ����������������TIM5ִ����ʾ����
		
		case Display_Interface_Mode_Main_Interface_P_set: break;       //���Ϊ�����趨ģʽ���Ͳ�ִ����������������TIM5ִ����ʾ����
		
		case Display_Interface_Mode_First_Menu: Display_Menu_Function_Interface_First_Menu(First_Selection);          //��ʾһ���˵�
		                                        Last_Interface = Display_Interface_Mode_First_Menu;
		                                        break;
		
		case Display_Interface_Mode_Second_Menu: Display_Menu_Function_Interface_Second_Menu(First_Selection, Second_Selection);     //��ʾ�����˵�
		                                         Last_Interface = Display_Interface_Mode_Second_Menu;
		                                         break;
	  
		case Display_Interface_Mode_Third_Menu: break;  //���Ϊ�������˵�ģʽ���Ͳ�ִ����������������TIM5ִ����ʾ����
		
		case Display_Interface_Mode_Protect_Interface: Display_Protect_Interface(Error);                               //��ʾ��������
		                                               Last_Interface = Display_Interface_Mode_Protect_Interface;
		                                               break;
		
		case Display_Interface_Mode_Recall_Interface: Display_Access_Interface(Recall, Recall_Save_Number,              //��ʾȡ���� 
			                                            Recall_Save_Display_Power, Recall_Save_Display_Voltage, Recall_Save_Display_Current);
		                                              Last_Interface = Display_Interface_Mode_Recall_Interface;
		                                              break;
																									
		case Display_Interface_Mode_Save_Interface: Display_Access_Interface(Save, Recall_Save_Number,                  //��ʾ�����
			                                          Recall_Save_Display_Power, Recall_Save_Display_Voltage, Recall_Save_Display_Current);
		                                            Last_Interface = Display_Interface_Mode_Save_Interface;
																								break;
			
		default: break;
	}
}










