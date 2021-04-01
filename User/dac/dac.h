#ifndef __DAC_H
#define __DAC_H

#include "stm32f10x.h"
#include "systick.h"

//写入输入寄存器n
#define DAC_CMD_WriteREG_n          0
//更新DAC寄存器n
#define DAC_CMD_UpDAC_n             1
//写入输入寄存器n，更新全部
#define DAC_CMD_WriteREGn_UpDAC_all 2
//写入并更新DAC通道n
#define DAC_CMD_WriteREGn_UpDAC_n   3
//复位
#define DAC_CMD_Reset               5
//LDAC寄存器设置
#define DAC_CMD_RegConfig           6

#define DAC_CH1 0				//000
#define DAC_CH2 1				//001
#define DAC_CH3 2				//010
#define DAC_CH4 3				//011
#define DAC_CHall 7			//111

#define DAC_Vext DAC_CH1
#define DAC_Vint DAC_CH4
#define DAC_P DAC_CH2
#define DAC_I DAC_CH3


#define Havent_Below     0
#define Already_Below    1


extern u8 Error_Stop_Flag;
extern u8 Wait_Below_Set_Value_Flag;


extern void SPI1_DAC_Config(void);
extern void DAC_Send(u8 cmd, u8 ch, u16 data);
extern void Power_On(void);
extern void Power_Off(void);
extern void Output_Zero(void);
//extern void Converter_Flag_Or_Not(void);



extern u8 SPI1_SendByte(u8 byte);


  



// 				DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_Vint, 0.0018 * Set_Voltage * Set_Voltage + 164.21 * Set_Voltage - 4.0316);
//		    DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_I, Set_Current * Set_Current * 0.0007 + 131.53 * Set_Current - 22.161);
// 		    DAC_Send(DAC_CMD_WriteREGn_UpDAC_all, DAC_P, -0.00000009 * Set_Power * Set_Power + 2.1895 * Set_Power + 4.6661);





#endif /* __DAC_H */
