#include "hardware.h"


uint8_t RO_Bit[RO_Bit_Num];
uint8_t RW_Bit[RW_Bit_Num];
uint16_t RO_Word[RO_Word_Num];
uint16_t RW_Word[RW_Word_Num];


/**
  * @brief  ����Դ�����ֵ��MODBUS��Ȼ�󷢳�ȥ
  * @param  ��
  * @retval ��
  */
void Power_To_PC(void)
{
//	
//	ROBit(0) = ON_OFF;
//	ROBit(1) = ����״̬;
//	ROBit(2) = ��ѹ״̬;
//	ROBit(3) = �ȹ�״̬;
//	ROBit(4) = �����ѹ����;
//	ROBit(5) = �����ѹ����;
//	ROBit(6) = �����������;
//	ROBit(7) = ���±���;
//	ROBit(8) = ȱ�ౣ��;
//	ROBit(9) = �����ʱ���;
//	ROBit(10) = IGBT���ϱ���;
//	ROBit(11) = ON_OFF;	
//	ROBit(12) = ON_OFF;
	
	//RWBit(0)= �������
	//RWBit(1)= ����ͨ�Ͽ���
	//RWBit(2)=��������
	//RWBit(3)=�����ѹ��������
	//RWBit(4)=�����ѹ��������
	//RWBit(5)=���������������
	//RWBit(6)=���±�������
	//RWBit(7)=ȱ�ౣ������
	//RWBit(8)=�����ʱ�������
	//RWBit(9)=IGBT���ϱ�������
	//RWBit(10)=���������

	
    RW_Word[0] = (uint16_t)Set_Current*100;//�����趨ֵ
	RW_Word[1] = (uint16_t)Set_Voltage*100;//��ѹ�趨ֵ
	RW_Word[2] = (uint16_t)Set_Power*100;//�����趨ֵ
	
//	RW_Word[5] = Set_soft_start_time; //����ʱ��
//	RW_Word[6] = Set_minute;//�趨����
//	RW_Word[7] = Set_second;//�趨��
//	RW_Word[8] = Set_soft_stop_time;//����ʱ��
//    RW_Word[9] = ControlMode;//���ģʽ 1-����  2-��ѹ  3-�㹦��

	
	
	
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
  * @brief  ��Modbus�յ���ֵ����Դ
  * @param  ��
  * @retval ��
  */
void PC_To_Modbus(void)
{
//	ON_OFF = ROBit(0);
	
    Set_Current= (float)RW_Word[0]/100 ;
	Set_Voltage = (float)RW_Word[1]/100;
	Set_Power = (float)RW_Word[2]/100;
}









