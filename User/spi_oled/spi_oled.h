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


/*SPI接口定义-开头****************************/
#define      OLED_SPIx                        SPI3
#define      OLED_SPI_APBxClock_FUN           RCC_APB1PeriphClockCmd
#define      OLED_SPI_CLK                     RCC_APB1Periph_SPI3

//CS(NSS)引脚 片选选普通GPIO即可
#define      OLED_SPI_CS_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define      OLED_SPI_CS_CLK                  RCC_APB2Periph_GPIOA    
#define      OLED_SPI_CS_PORT                 GPIOA
#define      OLED_SPI_CS_PIN                  GPIO_Pin_10

//SCK引脚
#define      OLED_SPI_SCK_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      OLED_SPI_SCK_CLK                 RCC_APB2Periph_GPIOC   
#define      OLED_SPI_SCK_PORT                GPIOC   
#define      OLED_SPI_SCK_PIN                 GPIO_Pin_10

//MOSI引脚
#define      OLED_SPI_MOSI_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define      OLED_SPI_MOSI_CLK                RCC_APB2Periph_GPIOC    
#define      OLED_SPI_MOSI_PORT               GPIOC
#define      OLED_SPI_MOSI_PIN                GPIO_Pin_12

//DC引脚
#define      OLED_SPI_DC_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define      OLED_SPI_DC_CLK                  RCC_APB2Periph_GPIOA    
#define      OLED_SPI_DC_PORT                 GPIOA
#define      OLED_SPI_DC_PIN                  GPIO_Pin_11

//定义位选引脚的高低电平函数
#define  		SPI_OLED_CS_LOW()     						GPIO_ResetBits( OLED_SPI_CS_PORT, OLED_SPI_CS_PIN )
#define  		SPI_OLED_CS_HIGH()    						GPIO_SetBits( OLED_SPI_CS_PORT, OLED_SPI_CS_PIN )

//定义命令/数据引脚的高低电平函数
#define     SPI_OLED_DC_LOW()                   GPIO_ResetBits( OLED_SPI_DC_PORT, OLED_SPI_DC_PIN )
#define     SPI_OLED_DC_HIGH()                  GPIO_SetBits( OLED_SPI_DC_PORT, OLED_SPI_DC_PIN )

/*SPI接口定义-结尾****************************/

/*--------------------------------------------------标志位宏定义------------------------------------------------*/

//宏定义Shift复用标志位
#define Shift                   1
#define Not_Shift               0

//宏定义锁定/解锁状态
#define LOCK       1
#define UNLOCK     0

//宏定义设定/实际值状态
#define Preset     1
#define Actual     0

//宏定义CV/CC/CP/DELAY/NOT DELAY状态
#define CV         0X01
#define CC         0X02
#define CP         0X03
#define DELAY      0XFF
#define NOT_DELAY  0X00

////宏定义 CV之后使用的状态
//#define CV_Vint    0X04
//#define CV_Vext    0X05

//宏定义输出显示通断状态
#define ON         1
#define OFF        0

//宏定义输出通断状态
#define OUTPUT            1
#define NOT_OUTPUT        0

//宏定义存取状态
#define Save       1
#define Recall     0

//定义是否第一次进入错误界面
#define Have_Into       0
#define Have_Not_Into   1

//宏定义一级菜单
#define System          0
#define Timer           1
#define Slope           2
#define Communication   3
#define Parallel        4
#define Vint_or_Vext    5

//宏定义二级菜单
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

//宏定义保护菜单
#define PF              0x00        //proper functioning    正常工作
#define OVP             0x01
#define OCP             0x02
#define OPP             0x04
#define OTP             0x08  //0000 1000
#define Sense_Reverse   0x10

//宏定义要显示什么界面
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





/*命令定义-开头*******************************/

#define Enable_Gray_Scale_table                   0x00    //这个命令被发送来启用灰度表设置。(命令B8h)
#define Set_Column_Address_Commond                0x15    //设置列开始和结束地址
#define Write_RAM_Command                         0x5c    //使单片机能够将数据写入内存中
#define Read_RAM_Command                          0x5d    //让单片机读取内存中的数据
#define Set_Row_Address_Commond                   0x75    //设置行开始和结束地址
#define Set_Re_map_and_Dual_COM_Line_mode         0xa0    //设置重映射和双COM线模式
#define Set_Display_Start_Line                    0xa1    //在0-127之间设置显示启动线寄存器
#define Set_Display_Offset                        0xa2    //从0-127设置垂直滚动
#define Entire_Display_OFF                        0xa4    //所有像素都关闭在GS级0
#define Entire_Display_ON                         0xa5    //所有像素都在GS级别15中打开
#define Normal_Display                            0xa6    //正常显示(重置)
#define Inverse_Display                           0xa7    //逆显示
#define Enable_Partial_Display                    0xa8    //这个命令打开了部分模式
#define Exit_Partial_Display                      0xa9    //该命令被发送到退出部分显示模式
#define Function_Selection                        0xab    //选择外部VDD/启用内部VDD调整器[reset]
#define Sleep_mode_ON                             0xae    //设置睡眠模式ON
#define Sleep_mode_OFF                            0xaf    //设置睡眠模式OFF
#define Set_Phase_Length                          0xb1    //设置相位长度
#define Set_Front_Clock_Divider                   0xb3    //设置前时钟分频器/振荡器频率
#define Set_GPIO                                  0xb5    //设置GPIO
#define Set_Second_Precharge_Period               0xb6    //设置第二预充期间
#define Set_Gray_Scale_Table                      0xb8    //设置灰度表
#define Select_Default_Linear_Gray_Scale_table    0xb9    //选择默认线性灰度表
#define Set_Pre_charge_voltage                    0xbb    //设置预充电电压
#define Set_VCOMH                                 0xbe    //设置COM取消选择电压等级[reset = 04h]
#define Set_Contrast_Current                      0xc1    //设置对比度电流
#define Master_Contrast_Current_Control           0xc7    //主对比度电流控制
#define Set_MUX_Ratio                             0xca    //设置多路选择开关比值
#define Set_Command_Lock                          0xfd    //设置命令锁

/*命令定义-结尾*******************************/


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




