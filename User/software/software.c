#include "software.h"

struct
{
	u8 Function_Switch;
	u8 Number_0;
	u8 Number_1;
	u8 Number_2;
	u8 Number_3;
	u8 Number_4;
	u8 Number_5;
	u8 Number_6;
	u8 Number_7;
	u8 Number_8;
	u8 Number_9;
	u8 Decimal_Point;
	u8 Esc;
	u8 V_set;
	u8 I_set;
	u8 Recall_;
	u8 Preset_;
	u8 Shift_;
	u8 On_Off;
	u8 up;
	u8 Enter;
	u8 Down;
} Software_Flag;



void Computer_Control_Device(void)
{
	if(Software_Flag.Function_Switch == 1)
	{
		if(Software_Flag.Number_0 == 1)
		{
			Software_Flag.Number_0 = 0;
			Key_0();
		}
		else if(Software_Flag.Number_1 == 1)
		{
			Software_Flag.Number_1 = 0;
			Key_1();
		}
		else if(Software_Flag.Number_2 == 1)
		{
			Software_Flag.Number_2 = 0;
			Key_2();
		}
		else if(Software_Flag.Number_3 == 1)
		{
			Software_Flag.Number_3 = 0;
			Key_3();
		}
		else if(Software_Flag.Number_4 == 1)
		{
			Software_Flag.Number_4 = 0;
			Key_4();
		}
		else if(Software_Flag.Number_5 == 1)
		{
			Software_Flag.Number_5 = 0;
			Key_5();
		}
		else if(Software_Flag.Number_6 == 1)
		{
			Software_Flag.Number_6 = 0;
			Key_6();
		}
		else if(Software_Flag.Number_7 == 1)
		{
			Software_Flag.Number_7 = 0;
			Key_7();
		}
		else if(Software_Flag.Number_8 == 1)
		{
			Software_Flag.Number_8 = 0;
			Key_8();
		}
		else if(Software_Flag.Number_9 == 1)
		{
			Software_Flag.Number_9 = 0;
			Key_9();
		}
		else if(Software_Flag.Decimal_Point == 1)
		{
			Software_Flag.Decimal_Point = 0;
			Key_Decimal_Point();
		}
		else if(Software_Flag.Esc == 1)
		{
			Software_Flag.Esc = 0;
			Key_ESC();
		}
		else if(Software_Flag.V_set == 1)
		{
			Software_Flag.V_set = 0;
			Key_V_set();
		}
		else if(Software_Flag.I_set == 1)
		{
			Software_Flag.I_set = 0;
			Key_I_set();
		}
		else if(Software_Flag.Recall_== 1)
		{
			Software_Flag.Recall_ = 0;
			Key_Recall_Save();
		}
		else if(Software_Flag.Preset_ == 1)
		{
			Software_Flag.Preset_ = 0;
			Key_Preset_Lock();
		}
		else if(Software_Flag.Shift_ == 1)
		{
			Software_Flag.Shift_ = 0;
			Key_Shift();
		}
		else if(Software_Flag.On_Off == 1)
		{
			Software_Flag.On_Off = 0;
			Key_ON_OFF();
		}
		else if(Software_Flag.up == 1)
		{
			Software_Flag.up = 0;
			Key_Up();
		}
		else if(Software_Flag.Enter == 1)
		{
			Software_Flag.Enter = 0;
			Key_Enter();
		}
		else if(Software_Flag.Down)
		{
			Software_Flag.Down = 0;
			Key_Down();
		}
	}
}




















