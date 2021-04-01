#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x.h"
#include "systick.h"
#include "fonts.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "keys.h"
#include "adc.h"


/*SPI�ӿڶ���-��ͷ****************************/
#define      OLED_SPIx                        SPI3
#define      OLED_SPI_APBxClock_FUN           RCC_APB1PeriphClockCmd
#define      OLED_SPI_CLK                     RCC_APB1Periph_SPI3

//CS(NSS)���� Ƭѡѡ��ͨGPIO����
#define      OLED_SPI_CS_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define      OLED_SPI_CS_CLK                  RCC_APB2Periph_GPIOA    
#define      OLED_SPI_CS_PORT                 GPIOA
#define      OLED_SPI_CS_PIN                  GPIO_Pin_10

//SCK����
#define      OLED_SPI_SCK_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      OLED_SPI_SCK_CLK                 RCC_APB2Periph_GPIOC   
#define      OLED_SPI_SCK_PORT                GPIOC   
#define      OLED_SPI_SCK_PIN                 GPIO_Pin_10

//MOSI����
#define      OLED_SPI_MOSI_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define      OLED_SPI_MOSI_CLK                RCC_APB2Periph_GPIOC    
#define      OLED_SPI_MOSI_PORT               GPIOC
#define      OLED_SPI_MOSI_PIN                GPIO_Pin_12

//DC����
#define      OLED_SPI_DC_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define      OLED_SPI_DC_CLK                  RCC_APB2Periph_GPIOA    
#define      OLED_SPI_DC_PORT                 GPIOA
#define      OLED_SPI_DC_PIN                  GPIO_Pin_11

//����λѡ���ŵĸߵ͵�ƽ����
#define  		SPI_OLED_CS_LOW()     						GPIO_ResetBits( OLED_SPI_CS_PORT, OLED_SPI_CS_PIN )
#define  		SPI_OLED_CS_HIGH()    						GPIO_SetBits( OLED_SPI_CS_PORT, OLED_SPI_CS_PIN )

//��������/�������ŵĸߵ͵�ƽ����
#define     SPI_OLED_DC_LOW()                   GPIO_ResetBits( OLED_SPI_DC_PORT, OLED_SPI_DC_PIN )
#define     SPI_OLED_DC_HIGH()                  GPIO_SetBits( OLED_SPI_DC_PORT, OLED_SPI_DC_PIN )

/*SPI�ӿڶ���-��β****************************/

/*--------------------------------------------------��־λ�궨��------------------------------------------------*/

//�궨��Shift���ñ�־λ
#define Shift                   1
#define Not_Shift               0

//�궨������/����״̬
#define LOCK       1
#define UNLOCK     0

//�궨���趨/ʵ��ֵ״̬
#define Preset     1
#define Actual     0

//�궨��CV/CC/CP/DELAY/NOT DELAY״̬
#define CV         0X01
#define CC         0X02
#define CP         0X03
#define DELAY      0XFF
#define NOT_DELAY  0X00

////�궨�� CV֮��ʹ�õ�״̬
//#define CV_Vint    0X04
//#define CV_Vext    0X05

//�궨�������ʾͨ��״̬
#define ON         1
#define OFF        0

//�궨�����ͨ��״̬
#define OUTPUT            1
#define NOT_OUTPUT        0

//�궨���ȡ״̬
#define Save       1
#define Recall     0

//�����Ƿ��һ�ν���������
#define Have_Into       0
#define Have_Not_Into   1

//�궨��һ���˵�
#define System          0
#define Timer           1
#define Slope           2
#define Communication   3
#define Parallel        4
#define Vint_or_Vext    5

//�궨������˵�
#define Reset           0
#define Delay           1
#define Cycle           2
#define V_Rise          3
#define V_Fall          4
#define I_Rise          5
#define I_Fall          6
#define RS232           7
#define CAN             8
#define Menu_Vint       9
#define Menu_Vext       10

//�궨�屣���˵�
#define PF              0x00        //proper functioning    ��������
#define OVP             0x01
#define OCP             0x02
#define OPP             0x04
#define OTP             0x08  //0000 1000
#define Sense_Reverse   0x10

//�궨��Ҫ��ʾʲô����
#define Display_Interface_Mode_Main_Interface_Actual          0
#define Display_Interface_Mode_Main_Interface_Preset          1
#define Display_Interface_Mode_Main_Interface_V_set           2
#define Display_Interface_Mode_Main_Interface_I_set           3
#define Display_Interface_Mode_Main_Interface_P_set           4
#define Display_Interface_Mode_Access_Interface               5
#define Display_Interface_Mode_First_Menu                     6
#define Display_Interface_Mode_Second_Menu                    7
#define Display_Interface_Mode_Third_Menu                     8
#define Display_Interface_Mode_Protect_Interface              9   
#define Display_Interface_Mode_Recall_Interface               10 
#define Display_Interface_Mode_Save_Interface                 11





/*�����-��ͷ*******************************/

#define Enable_Gray_Scale_table                   0x00    //���������������ûҶȱ����á�(����B8h)
#define Set_Column_Address_Commond                0x15    //�����п�ʼ�ͽ�����ַ
#define Write_RAM_Command                         0x5c    //ʹ��Ƭ���ܹ�������д���ڴ���
#define Read_RAM_Command                          0x5d    //�õ�Ƭ����ȡ�ڴ��е�����
#define Set_Row_Address_Commond                   0x75    //�����п�ʼ�ͽ�����ַ
#define Set_Re_map_and_Dual_COM_Line_mode         0xa0    //������ӳ���˫COM��ģʽ
#define Set_Display_Start_Line                    0xa1    //��0-127֮��������ʾ�����߼Ĵ���
#define Set_Display_Offset                        0xa2    //��0-127���ô�ֱ����
#define Entire_Display_OFF                        0xa4    //�������ض��ر���GS��0
#define Entire_Display_ON                         0xa5    //�������ض���GS����15�д�
#define Normal_Display                            0xa6    //������ʾ(����)
#define Inverse_Display                           0xa7    //����ʾ
#define Enable_Partial_Display                    0xa8    //���������˲���ģʽ
#define Exit_Partial_Display                      0xa9    //��������͵��˳�������ʾģʽ
#define Function_Selection                        0xab    //ѡ���ⲿVDD/�����ڲ�VDD������[reset]
#define Sleep_mode_ON                             0xae    //����˯��ģʽON
#define Sleep_mode_OFF                            0xaf    //����˯��ģʽOFF
#define Set_Phase_Length                          0xb1    //������λ����
#define Set_Front_Clock_Divider                   0xb3    //����ǰʱ�ӷ�Ƶ��/����Ƶ��
#define Set_GPIO                                  0xb5    //����GPIO
#define Set_Second_Precharge_Period               0xb6    //���õڶ�Ԥ���ڼ�
#define Set_Gray_Scale_Table                      0xb8    //���ûҶȱ�
#define Select_Default_Linear_Gray_Scale_table    0xb9    //ѡ��Ĭ�����ԻҶȱ�
#define Set_Pre_charge_voltage                    0xbb    //����Ԥ����ѹ
#define Set_VCOMH                                 0xbe    //����COMȡ��ѡ���ѹ�ȼ�[reset = 04h]
#define Set_Contrast_Current                      0xc1    //���öԱȶȵ���
#define Master_Contrast_Current_Control           0xc7    //���Աȶȵ�������
#define Set_MUX_Ratio                             0xca    //���ö�·ѡ�񿪹ر�ֵ
#define Set_Command_Lock                          0xfd    //����������

/*�����-��β*******************************/


extern u8 Error;
extern u8 Last_Interface;
extern char String_Voltage[20]; 
extern char String_Current[20];
extern char String_Power[20];

extern float Recall_Save_Display_Voltage;  
extern float Recall_Save_Display_Current;  
extern float Recall_Save_Display_Power;   

extern float Recall_Save_Voltage[10];
extern float Recall_Save_Current[10];
extern float Recall_Save_Power[10];

extern float Set_Voltage;
extern float Set_Current;
extern float Set_Power;

extern float Voltage;
extern float Current;
extern float Power;

extern float Vint;
extern float Vext;

extern u8 First_Into_ErrorInterface_Flag;

extern void SPI_FUN(void);
extern void SPI_OLED_SendCommond(u8 byte);
extern void SPI_OLED_SendData(u8 byte);
extern void OLED_Init(void);
extern void Set_Column_Address(u8 a, u8 b);
extern void Set_Row_Address(u8 a, u8 b);
extern void Set_WriteData(void);
extern void Set_Remap_Format(unsigned char A);
extern void Fill_RAM(u8 Data);
extern void Row_Interval(u8 Data);
extern void Point_Interval(u8 m);
extern void Display_Horizontal_Line(u8 Row, u8 Start_Column, u8 End_Column);
extern void Display_Vertical_Line_1(u8 Column, u8 Start_Row, u8 End_Row);
extern void Display_Vertical_Line_2(u8 Column, u8 Start_Row, u8 End_Row);
extern void Display_Vertical_Line_3(u8 Column, u8 Start_Row, u8 End_Row);
extern void Display_Vertical_Line_4(u8 Column, u8 Start_Row, u8 End_Row);
extern void Display_Rectangular(u8 Start_Row, u8 Start_Column, u8 End_Row, u8 End_Column);
extern void Dsiplay_Solid_Rectangular(u8 Start_Row, u8 Start_Column, u8 End_Row, u8 End_Column, u8 Colour);
extern void Font_1Bit_To_4Bit(u8 Data);
extern void Write_Single_8x16AsicII(u8 Row, u8 Column, char Data);
extern void Write_String_8x16AsicII(u8 Row, u8 Column, char Data[]);
extern void Write_Single_12x24AsicII(u8 Row, u8 Column, char Data);
extern void Write_String_12x24AsicII(u8 Row, u8 Column, char Data[]);
extern void Write_Single_16x16Chinese(u8 Row, u8 Column, char Data[]);
extern void Write_String_16x16Chinese(u8 Row, u8 Column, char String_Data[]);
extern void Write_Single_16x32AsicII(u8 Row, u8 Column, char Data);
extern void Write_String_16x32AsicII(u8 Row, u8 Column, char Data[]);
extern void Display_Main_Interface(float Voltage, float Current, float Power, u8 ON_OFF, u8 CV_CC_CP,u8 DELAY_NOT_DELAY, u8 Preset_Actual, u8 LOCK_UNLOCK, u8 Shift_NotShift, u8 Use_Vint_Or_Vext_Flag);
extern void Display_Access_Interface(u8 Recall_Save, u8 Recall_Save_Number, float Power, float Voltage, float Current);
extern void Display_Menu_Function_Interface_First_Menu(u8 Selection);
extern void Display_Menu_Function_Interface_Second_Menu(u8 First_Selection, u8 Second_Selection);
extern void Display_Protect_Interface(u8 Error);
extern void Display_Interface(u8 Display_Interface_Mode);
extern void Display_Main_Interface_CV_CC_CP(u8 CV_CC_CP,u8 DELAY_NOT_DELAY);
extern void Display_Main_Interface_Voltage(float Voltage);
extern void Display_Main_Interface_Current(float Current);
extern void Display_Main_Interface_Power(float Power);
extern void Display_Main_Interface_V_set(float Current, float Power, u8 ON_OFF, u8 CV_CC_CP,u8 DELAY_NOT_DELAY, u8 Preset_Actual, u8 LOCK_UNLOCK, u8 Use_Vint_Or_Vext_Flag);
extern void Display_Main_Interface_I_set(float Voltage, float Power, u8 ON_OFF, u8 CV_CC_CP,u8 DELAY_NOT_DELAY, u8 Preset_Actual, u8 LOCK_UNLOCK, u8 Use_Vint_Or_Vext_Flag);
extern void Display_Main_Interface_P_set(float Voltage, float Current, u8 ON_OFF, u8 CV_CC_CP,u8 DELAY_NOT_DELAY, u8 Preset_Actual, u8 LOCK_UNLOCK, u8 Use_Vint_Or_Vext_Flag);
extern void Display_Main_Interface_ON_OFF(u8 State);


#endif /* __SPI_H */




