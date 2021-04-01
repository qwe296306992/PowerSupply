#include "hardware.h"


uint8_t RO_Bit[RO_Bit_Num];
uint8_t RW_Bit[RW_Bit_Num];
uint16_t RO_Word[RO_Word_Num];
uint16_t RW_Word[RW_Word_Num];


/**
  * @brief  将电源里面的值给MODBUS，然后发出去
  * @param  无
  * @retval 无
  */
void Power_To_PC(void)
{
//	
//	ROBit(0) = ON_OFF;
//	ROBit(1) = 稳流状态;
//	ROBit(2) = 稳压状态;
//	ROBit(3) = 稳功状态;
//	ROBit(4) = 输入过压保护;
//	ROBit(5) = 输出过压保护;
//	ROBit(6) = 输出过流保护;
//	ROBit(7) = 过温保护;
//	ROBit(8) = 缺相保护;
//	ROBit(9) = 过功率保护;
//	ROBit(10) = IGBT故障保护;
//	ROBit(11) = ON_OFF;	
//	ROBit(12) = ON_OFF;
	
	//RWBit(0)= 输出开关
	//RWBit(1)= 快速通断开关
	//RWBit(2)=锁定操作
	//RWBit(3)=输入过压保护允许
	//RWBit(4)=输出过压保护允许
	//RWBit(5)=输出过流保护允许
	//RWBit(6)=过温保护允许
	//RWBit(7)=缺相保护允许
	//RWBit(8)=过功率保护允许
	//RWBit(9)=IGBT故障保护允许
	//RWBit(10)=允许开机输出

	
    RW_Word[0] = (uint16_t)Set_Current*100;//电流设定值
	RW_Word[1] = (uint16_t)Set_Voltage*100;//电压设定值
	RW_Word[2] = (uint16_t)Set_Power*100;//功率设定值
	
//	RW_Word[5] = Set_soft_start_time; //缓升时间
//	RW_Word[6] = Set_minute;//设定分钟
//	RW_Word[7] = Set_second;//设定秒
//	RW_Word[8] = Set_soft_stop_time;//缓降时间
//    RW_Word[9] = ControlMode;//输出模式 1-恒流  2-恒压  3-恒功率

	
	
	
//	RO_Word[0] = (uint16_t)Current*100;
//	RO_Word[1] = (uint16_t)Voltage*100;
//	RO_Word[2] = (uint16_t)Power*100;
//	
//	RO_Word[3] = (uint16_t)Recall_Save_Current[0]*100;
//	RO_Word[4] = (uint16_t)Recall_Save_Current[1]*100;
//	RO_Word[5] = (uint16_t)Recall_Save_Current[2]*100;
//	RO_Word[6] = (uint16_t)Recall_Save_Current[3]*100;
//	RO_Word[7] = (uint16_t)Recall_Save_Current[4]*100;
//	RO_Word[8] = (uint16_t)Recall_Save_Current[5]*100;
//	RO_Word[9] = (uint16_t)Recall_Save_Current[6]*100;
//	RO_Word[10] = (uint16_t)Recall_Save_Current[7]*100;
//	RO_Word[11] = (uint16_t)Recall_Save_Current[8]*100;
//	RO_Word[12] = (uint16_t)Recall_Save_Current[9]*100;
//	
//	RO_Word[13] = (uint16_t)Recall_Save_Voltage[0]*100;
//	RO_Word[14] = (uint16_t)Recall_Save_Voltage[1]*100;
//	RO_Word[15] = (uint16_t)Recall_Save_Voltage[2]*100;
//	RO_Word[16] = (uint16_t)Recall_Save_Voltage[3]*100;
//	RO_Word[17] = (uint16_t)Recall_Save_Voltage[4]*100;
//	RO_Word[18] = (uint16_t)Recall_Save_Voltage[5]*100;
//	RO_Word[19] = (uint16_t)Recall_Save_Voltage[6]*100;
//	RO_Word[20] = (uint16_t)Recall_Save_Voltage[7]*100;
//	RO_Word[21] = (uint16_t)Recall_Save_Voltage[8]*100;
//	RO_Word[22] = (uint16_t)Recall_Save_Voltage[9]*100;
//	
//	RO_Word[23] = Recall_Save_Power[0]*100;
//	RO_Word[24] = Recall_Save_Power[1]*100;
//	RO_Word[25] = Recall_Save_Power[2]*100;
//	RO_Word[26] = Recall_Save_Power[3]*100;
//	RO_Word[27] = Recall_Save_Power[4]*100;
//	RO_Word[28] = Recall_Save_Power[5]*100;
//	RO_Word[29] = Recall_Save_Power[6]*100;
//	RO_Word[30] = Recall_Save_Power[7]*100;
//	RO_Word[31] = Recall_Save_Power[8]*100;
//	RO_Word[32] = Recall_Save_Power[9]*100;
	
}

/**
  * @brief  将Modbus收到的值给电源
  * @param  无
  * @retval 无
  */
void PC_To_Modbus(void)
{
//	ON_OFF = ROBit(0);
	
    Set_Current= (float)RW_Word[0]/100 ;
	Set_Voltage = (float)RW_Word[1]/100;
	Set_Power = (float)RW_Word[2]/100;
}









