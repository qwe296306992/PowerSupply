#include "keys.h"

u8 Shift_Not_Shift = Not_Shift;                                             //定义复用键的标志位（默认不复用）
u8 ON_OFF = OFF;                                                            //定义输出关闭标志位（默认不输出,只是用来显示）
u8 OUTPUT_NOT_OUTPUT = NOT_OUTPUT;                                          //定义是否已经输出标志位
u8 CV_CC_CP = CV;                                                           //定义输出类型标志位 (默认恒压输出)
u8 DELAY_NOT_DELAY = NOT_DELAY;                                             //定义延时标志位（默认不延时）
u8 LOCK_UNLOCK = UNLOCK;                                                    //定义锁定标志位（默认不锁定） 
u8 Recall_Save = Recall;                                                    //定义读取保存标志位（默认为读取）
int Recall_Save_Number = 0;                                                 //定义读取保存的数据组（默认第0组）
int First_Selection = 0;                                                    //定义第一菜单的选择
u8 Second_Selection = 0;                                                    //定义第二菜单的选择
u8 Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;   //定义显示界面的类型（默认显示主界面，真实值）
int Cursor_Position;                                                        //定义光标位置的全局变量
u8 Keys_Encoder_Mode = Encoder_Mode;                                        //定义用键盘输入还是用编码器输入(默认用编码器输入)
u8 RS232_BaudRate_State = 0;                                                //定义设定波特率状态
u8 CAN_BaudRate_State = 0;                                                  //定义设定波特率状态


u8 Cycle_Enter = Cycle_Enter_0;                                          //定义循环时是设定秒还是毫秒
u16 Cycle_On_s = 0;                                                         //定义循环时间的秒
u16 Cycle_On_ms = 0;                                                        //定义循环时间的毫秒
u16 Cycle_Close_s = 0;                                                      //定义循环关闭时间的秒
u16 Cycle_Close_ms = 0;                                                     //定义循环关闭时间的毫秒
char String_Cycle_On[11];                                                   //定义循环开的字符串
char String_Cycle_Close[11];                                                //定义循环关的字符串

u8 Delay_Enter = Delay_Enter_0;                                              //记录按了多少次ENTER
u8 Delay_h;                                                                  //定义延时的时
u8 Delay_m;                                                                  //定义延时的分
u8 Delay_s;                                                                  //定义延时的秒
char String_Delay[10];                                                        //定义延时的字符串


u8 V_Rise_Enter = V_Rise_Enter_0;                                             //定义V-Rise按了多少次按键
u16 V_Rise_s = 0;                                                            //定义V-Rise的秒
u16 V_Rise_ms = 0;                                                           //定义V-Rise的毫秒
char String_V_Rise[11];                                                     //定义V-Rise的字符串


u8 V_Fall_Enter = V_Fall_Enter_0;                                            //定义V_Fall设置的是秒毫秒
u16 V_Fall_s = 0;                                                            //定义V_Fall的秒
u16 V_Fall_ms = 0;                                                           //定义V_Fall的毫秒
char String_V_Fall[11];                                                     //定义V_Fall的字符串



u8 I_Rise_Enter = I_Rise_Enter_0;                                            //定义I_Rise设置的是秒毫秒
u16 I_Rise_s = 0;                                                            //定义I_Rise的秒
u16 I_Rise_ms = 0;                                                           //定义I_Rise的毫秒
char String_I_Rise[11];                                                      //定义I_Rise的字符串


u8 I_Fall_Enter = I_Fall_Enter_0;                                            //定义I_Fall设置的是秒毫秒
u16 I_Fall_s = 0;                                                            //定义I_Fall的秒
u16 I_Fall_ms = 0;                                                           //定义I_Fall的毫秒
char String_I_Fall[11];                                                      //定义I_Fall的字符串

char Use_Vint_Or_Vext_Flag = Use_Vint;                                       //定义现在是使用内部电压还是外部电压标志

//char Last_State = CV;        //定义上一次使用的状态位

//char Now_State = CV;          //定义现在使用的状态位

//char Start_State = 0;        //定义刚开机时的状态位，0代表第一次开机，1代表不是第一次开机

//u8 Flag_Sensor1 =0;
//u8 Flag_Sensor2 =0;
//u8 Flag_Sensor3 =0;


void Keys_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE , ENABLE);
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//键盘行
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//开漏输出
	GPIO_InitStructure.GPIO_Pin = KeyRow_1_Pin | KeyRow_2_Pin | KeyRow_3_Pin;
  GPIO_Init(KeyRow_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//键盘列
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStructure.GPIO_Pin = KeyCol_1_Pin | KeyCol_2_Pin | KeyCol_3_Pin | KeyCol_4_Pin | KeyCol_5_Pin | KeyCol_6_Pin | KeyCol_7_Pin;
  GPIO_Init(KeyCol_Port, &GPIO_InitStructure);	
	
	//行一开始输出低电平
	GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
	GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
	GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);	
	
	keys_EN = DISABLE;//禁用按键
}





 /**
  * @brief  将设定电压电流功率字符数组变为浮点数
  * @param	数组名
  * @retval 对应的浮点数
  */
float String_To_Float(char String[])
{
  int temp;
  float Float;
  u8 j;  //字符串长度
  u8 i;
  u8 k;  //小数点位置
	u8 l;  //输入数字的个数
	u8 m;  //位数

  j = strlen(String);             //获取字符串长度

  for(i=0, k=0, l=0; i<j; i++)
  {
	  if(String[i] == ' ')         //将字符串剩余的空格变成0
		{
			String[i] = '0';
		}
		else
		{
			l ++;                      //获取输入数字的个数
		}
		
		if(String[i] == '.')         //获取小数点的位置
		{
			k = i;
		}
  }
	
	if(k != 0)           //有小数点
	{
		m = j -1;          
	}
	else           //无小数点
	{
		m=j;
	}
	
	for(i=0, temp=0; i<j;i++)    //不计小数点相加
	{
	  if(String[i] != '.')
	  {
			temp = temp + (String[i]-48)*(pow(10,m));//'0'对应的ASCII码为48，实现了由字符转数字
			m --;
		}
	}
	
	if(k != 0)		//有小数点
	{
		Float = temp/(pow(10, (j-k)));		
	}
	else					//没有小数点
	{
		Float = temp/(pow(10, (j-l+1)));
	}

	return Float;

}


////// /**
//////  * @brief  将设定电压电流功率字符数组变为浮点数
//////  * @param	数组名
//////  * @retval 对应的浮点数
//////  */
//////float String_To_Float(char String[])
//////{
//////	float Multiple = 10;
//////	float Float = 0;
//////	int i;
//////	
//////	for(i = 0; String[i] != '\0'; i++)
//////	{
//////		if((String[i] >= '0') && (String[i] <= '9') && (Multiple == 10))
//////		{
//////			Float = Float * Multiple + (String[i] - '0');
//////		}
//////		else if((String[i] >= '0') && (String[i] <= '9') && (Multiple != 10))
//////		{
//////			Float = Float + Multiple * (String[i] - '0');
//////			Multiple *= 0.1;
//////		}
//////		else if(String[i] == '.')
//////		{
//////			Multiple = 0.1;
//////		}
//////	}
//////	
//////	return Float;
//////}





 /**
  * @brief  数字键盘输入设定电压的处理函数
  * @param	数字键的号码（1，2，3，4，5，6，7，8，9，0）
  * @retval None
  */
static void Keys_Input_Set_Voltage(char Number)
{
  if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_V_set) && (LOCK_UNLOCK == UNLOCK) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))    //处于设定电压模式和无锁定状态
  {
		if(Keys_Encoder_Mode != Keys_Mode)                         //第一次按下按键
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
				
			Cursor_Position = 0;                                     //指针设为0位
				
		  String_Voltage[0] = ' ';                                 //清空字符串
			String_Voltage[1] = ' ';
			String_Voltage[2] = ' ';
			String_Voltage[3] = ' ';
			String_Voltage[4] = ' ';
		}
		
		if(Cursor_Position == 5)
	  {
			Cursor_Position = 0;                                     //如果光标超过范围，回到启始位置
			
			String_Voltage[0] = ' ';                                 //清空字符串
			String_Voltage[1] = ' ';
			String_Voltage[2] = ' ';
			String_Voltage[3] = ' ';
			String_Voltage[4] = ' ';
	  }
		
		String_Voltage[Cursor_Position] = Number;                  //使光标位置置Number
		
		if((Cursor_Position == 1) && (String_Voltage[1] != '.'))   //如果当位置1不为小数点，会让位置2变为小数点
		{
			String_Voltage[2] = '.';
			Cursor_Position = 2;
		}
		
		Cursor_Position ++;                                        //光标位置加1      

		Write_String_16x32AsicII(0, 0, String_Voltage);           //显示输入值
		Write_Single_16x32AsicII(0, 20, 'V');    		
  }
}

 /**
  * @brief  数字键盘输入设定电流的处理函数
  * @param	数字键的号码（1，2，3，4，5，6，7，8，9，0）
  * @retval None
  */
static void Keys_Input_Set_Current(char Number)
{
  if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_I_set) && (LOCK_UNLOCK == UNLOCK) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))    //处于设定电流模式和无锁定状态
  {
		if(Keys_Encoder_Mode != Keys_Mode)                         //第一次按下按键
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
				
			Cursor_Position = 0;                                     //指针设为0位
				
		  String_Current[0] = ' ';                                 //清空字符串
			String_Current[1] = ' ';
			String_Current[2] = ' ';
			String_Current[3] = ' ';
			String_Current[4] = ' ';
			String_Current[5] = ' ';
		}
		
		if(Cursor_Position == 6)
	  {
			Cursor_Position = 0;                                     //如果光标超过范围，回到启始位置
			
			String_Current[0] = ' ';                                 //清空字符串
			String_Current[1] = ' ';
			String_Current[2] = ' ';
			String_Current[3] = ' ';
			String_Current[4] = ' ';
			String_Current[5] = ' ';
	  }
		
		String_Current[Cursor_Position] = Number;                  //使光标位置置Number
		
		if((Cursor_Position == 2) && (String_Current[1] != '.') && (String_Current[2] != '.'))   //如果当位置1和2不为小数点，会让位置2变为小数点
		{
			String_Current[3] = '.';
			Cursor_Position = 3;
		}
		
		Cursor_Position ++;                                        //光标位置加1       
		
		Write_String_16x32AsicII(0, 36, String_Current);           //显示输入值
		Write_Single_16x32AsicII(0, 60, 'A');		
  }
}

 /**
  * @brief  数字键盘输入设定功率的处理函数
  * @param	数字键的号码（1，2，3，4，5，6，7，8，9，0）
  * @retval None
  */
static void Keys_Input_Set_Power(char Number)
{
  if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_P_set) && (LOCK_UNLOCK == UNLOCK) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))    //处于设定功率模式和无锁定状态
  {
		if(Keys_Encoder_Mode != Keys_Mode)                         //第一次按下按键
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
				
			Cursor_Position = 0;                                     //指针设为0位
				
		  String_Power[0] = ' ';                                 //清空字符串
			String_Power[1] = ' ';
			String_Power[2] = ' ';
			String_Power[3] = ' ';
			String_Power[4] = ' ';
		}
		
		if(Cursor_Position == 5)
	  {
			Cursor_Position = 0;                                     //如果光标超过范围，回到启始位置
			
			String_Power[0] = ' ';                                 //清空字符串
			String_Power[1] = ' ';
			String_Power[2] = ' ';
			String_Power[3] = ' ';
			String_Power[4] = ' ';
	  }
		
		String_Power[Cursor_Position] = Number;                  //使光标位置置Number
		
		Cursor_Position ++;                                        //光标位置加1      

		Write_String_12x24AsicII(42, 0, String_Power);           //显示输入值
		Write_Single_12x24AsicII(42, 15, 'W');    		
  }
}

 /**
  * @brief  数字键盘输入设定串口ID的处理函数
  * @param	数字键的号码（1，2，3，4，5，6，7，8，9，0）
  * @retval None
  */
static void Keys_Input_RS232(char Number)
{
	if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == RS232) && (RS232_Set_ID_BaudRate == RS232_Set_ID) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //第一次按下按键
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
				
			Cursor_Position = 0;                                     //指针设为0位
				
		  String_RS232_ID[0] = ' ';                                 //清空字符串
			String_RS232_ID[1] = ' ';
		}
		
		if(Cursor_Position == 2)
	  {
			Cursor_Position = 0;                                     //如果光标超过范围，回到启始位置
			
			String_RS232_ID[0] = ' ';                                 //清空字符串
			String_RS232_ID[1] = ' ';
	  }
		
		String_RS232_ID[Cursor_Position] = Number;                  //使光标位置置Number
		
		Cursor_Position ++;                                        //光标位置加1      

		Write_String_16x32AsicII(0, 12, String_RS232_ID);           //显示输入值
	}
}

 /**
  * @brief  数字键盘输入设定串口ID的处理函数
  * @param	数字键的号码（1，2，3，4，5，6，7，8，9，0）
  * @retval None
  */
static void Keys_Input_CAN(char Number)
{
	if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == CAN) && (CAN_Set_ID_BaudRate == CAN_Set_ID) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //第一次按下按键
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
				
			Cursor_Position = 0;                                     //指针设为0位
				
		  String_CAN_ID[0] = ' ';                                 //清空字符串
			String_CAN_ID[1] = ' ';
		}
		
		if(Cursor_Position == 2)
	  {
			Cursor_Position = 0;                                     //如果光标超过范围，回到启始位置
			
			String_CAN_ID[0] = ' ';                                 //清空字符串
			String_CAN_ID[1] = ' ';
	  }
		
		String_CAN_ID[Cursor_Position] = Number;                  //使光标位置置Number
		
		Cursor_Position ++;                                        //光标位置加1      

		Write_String_16x32AsicII(0, 12, String_CAN_ID);           //显示输入值
	}
}

 /**
  * @brief  数字键盘输入设定Cycle时间的处理函数
  * @param	数字键的号码（1，2，3，4，5，6，7，8，9，0）
  * @retval None
  */
static void Keys_Input_Cycle(char Number)
{
	if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Cycle) && (Cycle_Enter == Cycle_Enter_1) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))       //设置开启秒
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //第一次按下按键
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
				
			Cursor_Position = 0;                                     //指针设为0位
				
		  String_Cycle_On[0] = ' ';                                 //清空字符串
		  String_Cycle_On[1] = ' ';
		  String_Cycle_On[2] = ' ';
		  String_Cycle_On[3] = ' ';
			String_Cycle_On[5] = ' ';
			String_Cycle_On[6] = ' ';
			String_Cycle_On[7] = ' ';
		}
		
		if(Cursor_Position == 4)
		{
			Cursor_Position = 5;
		}
		else if(Cursor_Position == 8)
	  {
			Cursor_Position = 0;                                     //指针设为0位
				
		  String_Cycle_On[0] = ' ';                                 //清空字符串
		  String_Cycle_On[1] = ' ';
		  String_Cycle_On[2] = ' ';
		  String_Cycle_On[3] = ' ';
			String_Cycle_On[5] = ' ';
			String_Cycle_On[6] = ' ';
			String_Cycle_On[7] = ' ';
	  }
		
		String_Cycle_On[Cursor_Position] = Number;                  //使光标位置置Number
		
		Cursor_Position ++;                                        //光标位置加1      

		Write_String_16x32AsicII(32, 12, String_Cycle_On);           //显示输入值
	}
	
	else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Cycle) && (Cycle_Enter == Cycle_Enter_2) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))   //设置开启毫秒
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //第一次按下按键
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
				
			Cursor_Position = 0;                                     //指针设为0位
				
		  String_Cycle_Close[0] = ' ';                                 //清空字符串
		  String_Cycle_Close[1] = ' ';
		  String_Cycle_Close[2] = ' ';
			String_Cycle_Close[3] = ' ';
			String_Cycle_Close[5] = ' ';
			String_Cycle_Close[6] = ' ';
			String_Cycle_Close[7] = ' ';
		}
		
		if(Cursor_Position == 4)
		{
			Cursor_Position = 5;
		}
		else if(Cursor_Position == 8)
	  {
			Cursor_Position = 0;                                     //指针设为0位
				
		  String_Cycle_Close[0] = ' ';                                 //清空字符串
		  String_Cycle_Close[1] = ' ';
		  String_Cycle_Close[2] = ' ';
			String_Cycle_Close[3] = ' ';
			String_Cycle_Close[5] = ' ';
			String_Cycle_Close[6] = ' ';
			String_Cycle_Close[7] = ' ';
	  }
		
		String_Cycle_Close[Cursor_Position] = Number;                  //使光标位置置Number
		
		Cursor_Position ++;                                        //光标位置加1      

		Write_String_16x32AsicII(32, 12, String_Cycle_Close);           //显示输入值
	}		
}


 /**
  * @brief  数字键盘输入设定Delay时间的处理函数
  * @param	数字键的号码（1，2，3，4，5，6，7，8，9，0）
  * @retval None
  */
static void Keys_Input_Delay(char Number)
{
	if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Delay) && (Delay_Enter == Delay_Enter_1) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))       //设置开启时
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //第一次按下按键
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
				
			Cursor_Position = 0;                                     //指针设为0位
				
		  String_Delay[0] = ' ';                                 //清空字符串
		  String_Delay[1] = ' ';
      String_Delay[3] = ' ';
			String_Delay[4] = ' ';
			String_Delay[6] = ' ';
			String_Delay[7] = ' ';
		}
		
		String_Delay[Cursor_Position] = Number;                  //使光标位置置Number
		
		Cursor_Position ++;                                        //光标位置加1  
    if (Cursor_Position == 2)
		{
			Cursor_Position = 3;
		}			
    else if (Cursor_Position == 5)
		{
			Cursor_Position = 6;
		}			
		else if (Cursor_Position == 8) 
		{
			Cursor_Position = 0;
			
			String_Delay[0] = ' ';                                 //清空字符串
		  String_Delay[1] = ' ';
      String_Delay[3] = ' ';
			String_Delay[4] = ' ';
			String_Delay[6] = ' ';
			String_Delay[7] = ' ';
		}

		Write_String_16x32AsicII(32, 14, String_Delay);           //显示输入值
	}		
}

 /**
  * @brief  数字键盘输入设定V_Rise时间的处理函数
  * @param	数字键的号码（1，2，3，4，5，6，7，8，9，0）
  * @retval None
  */
static void Keys_Input_V_Rise(char Number)
{
	if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == V_Rise) && (V_Rise_Enter == V_Rise_Enter_1) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))       //设置开启秒
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //第一次按下按键
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
				
			Cursor_Position = 0;                                     //指针设为0位
				
		  String_V_Rise[0] = ' ';                                 //清空字符串
		  String_V_Rise[1] = ' ';
		  String_V_Rise[2] = ' ';
		  String_V_Rise[3] = ' ';
			String_V_Rise[5] = ' ';
			String_V_Rise[6] = ' ';
			String_V_Rise[7] = ' ';			
		}
		
		if(Cursor_Position == 4)
		{
			Cursor_Position = 5;
		}
		else if(Cursor_Position == 8)
	  {
			Cursor_Position = 0;                                     //指针设为0位
				
		  String_V_Rise[0] = ' ';                                 //清空字符串
		  String_V_Rise[1] = ' ';
		  String_V_Rise[2] = ' ';
		  String_V_Rise[3] = ' ';
			String_V_Rise[5] = ' ';
			String_V_Rise[6] = ' ';
			String_V_Rise[7] = ' ';	
	  }
		
		String_V_Rise[Cursor_Position] = Number;                  //使光标位置置Number
		
		Cursor_Position ++;                                        //光标位置加1      

		Write_String_16x32AsicII(32, 12, String_V_Rise);           //显示输入值
	}	
}

 /**
  * @brief  数字键盘输入设定V_Fall时间的处理函数
  * @param	数字键的号码（1，2，3，4，5，6，7，8，9，0）
  * @retval None
  */
static void Keys_Input_V_Fall(char Number)
{
	if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == V_Fall) && (V_Fall_Enter == V_Fall_Enter_1) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))       //设置开启秒
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //第一次按下按键
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
				
			Cursor_Position = 0;                                     //指针设为0位
				
		  String_V_Fall[0] = ' ';                                 //清空字符串
		  String_V_Fall[1] = ' ';
		  String_V_Fall[2] = ' ';
		  String_V_Fall[3] = ' ';
			String_V_Fall[5] = ' ';
			String_V_Fall[6] = ' ';
			String_V_Fall[7] = ' ';			
		}
		
		if(Cursor_Position == 4)
		{
			Cursor_Position = 5;
		}
		else if(Cursor_Position == 8)
	  {
			Cursor_Position = 0;                                     //指针设为0位
				
		  String_V_Fall[0] = ' ';                                 //清空字符串
		  String_V_Fall[1] = ' ';
		  String_V_Fall[2] = ' ';
		  String_V_Fall[3] = ' ';
			String_V_Fall[5] = ' ';
			String_V_Fall[6] = ' ';
			String_V_Fall[7] = ' ';	
	  }
		
		String_V_Fall[Cursor_Position] = Number;                  //使光标位置置Number
		
		Cursor_Position ++;                                        //光标位置加1      

		Write_String_16x32AsicII(32, 12, String_V_Fall);           //显示输入值
	}		
}

 /**
  * @brief  数字键盘输入设定I_Rise时间的处理函数
  * @param	数字键的号码（1，2，3，4，5，6，7，8，9，0）
  * @retval None
  */
static void Keys_Input_I_Rise(char Number)
{
	if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == I_Rise) && (I_Rise_Enter == I_Rise_Enter_1) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))       //设置开启秒 
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //第一次按下按键
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
				
			Cursor_Position = 0;                                     //指针设为0位
				
		  String_I_Rise[0] = ' ';                                 //清空字符串
		  String_I_Rise[1] = ' ';
		  String_I_Rise[2] = ' ';
		  String_I_Rise[3] = ' ';
			String_I_Rise[5] = ' ';
			String_I_Rise[6] = ' ';
			String_I_Rise[7] = ' ';			
		}
		
		if(Cursor_Position == 4)
		{
			Cursor_Position = 5;
		}
		else if(Cursor_Position == 8)
	  {
			Cursor_Position = 0;                                     //指针设为0位
				
		  String_I_Rise[0] = ' ';                                 //清空字符串
		  String_I_Rise[1] = ' ';
		  String_I_Rise[2] = ' ';
		  String_I_Rise[3] = ' ';
			String_I_Rise[5] = ' ';
			String_I_Rise[6] = ' ';
			String_I_Rise[7] = ' ';	
	  }
		
		String_I_Rise[Cursor_Position] = Number;                  //使光标位置置Number
		
		Cursor_Position ++;                                        //光标位置加1      

		Write_String_16x32AsicII(32, 12, String_I_Rise);           //显示输入值
	}		
}

 /**
  * @brief  数字键盘输入设定I_Fall时间的处理函数
  * @param	数字键的号码（1，2，3，4，5，6，7，8，9，0）
  * @retval None
  */
static void Keys_Input_I_Fall(char Number)
{
	if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == I_Fall) && (I_Fall_Enter == I_Fall_Enter_1) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))       //设置开启秒
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //第一次按下按键
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
				
			Cursor_Position = 0;                                     //指针设为0位
				
		  String_I_Fall[0] = ' ';                                 //清空字符串
		  String_I_Fall[1] = ' ';
		  String_I_Fall[2] = ' ';
		  String_I_Fall[3] = ' ';
			String_I_Fall[5] = ' ';
			String_I_Fall[6] = ' ';
			String_I_Fall[7] = ' ';			
		}
		
		if(Cursor_Position == 4)
		{
			Cursor_Position = 5;
		}
		else if(Cursor_Position == 8)
	  {
			Cursor_Position = 0;                                     //指针设为0位
				
		  String_I_Fall[0] = ' ';                                 //清空字符串
		  String_I_Fall[1] = ' ';
		  String_I_Fall[2] = ' ';
		  String_I_Fall[3] = ' ';
			String_I_Fall[5] = ' ';
			String_I_Fall[6] = ' ';
			String_I_Fall[7] = ' ';	
	  }
		
		String_I_Fall[Cursor_Position] = Number;                  //使光标位置置Number
		
		Cursor_Position ++;                                        //光标位置加1      

		Write_String_16x32AsicII(32, 12, String_I_Fall);           //显示输入值
	}		
}





 /**
  * @brief  数字键盘输入选择存取界面编号的处理函数
  * @param	数字键的号码（1，2，3，4，5，6，7，8，9，0）
  * @retval None
  */
void Keys_Input_Recall_Save(u8 Number)
{
	if((Display_Interface_Mode == Display_Interface_Mode_Recall_Interface) || (Display_Interface_Mode == Display_Interface_Mode_Save_Interface))   //处于存取模式
	{
	  Recall_Save_Number = Number;
		    	
		Recall_Save_Display_Voltage = Recall_Save_Voltage[Recall_Save_Number];
		Recall_Save_Display_Current = Recall_Save_Current[Recall_Save_Number];
		Recall_Save_Display_Power = Recall_Save_Power[Recall_Save_Number];
					
		if(Display_Interface_Mode == Display_Interface_Mode_Recall_Interface)
		{
		  Display_Access_Interface(Recall, Recall_Save_Number,Recall_Save_Display_Power, Recall_Save_Display_Voltage, Recall_Save_Display_Current);
    }
		else if(Display_Interface_Mode == Display_Interface_Mode_Save_Interface)
		{
			Display_Access_Interface(Save, Recall_Save_Number,Recall_Save_Display_Power, Recall_Save_Display_Voltage, Recall_Save_Display_Current);
		}
	}
}

 /**
  * @brief  数字按键0的扫描服务函数
  * @param	None
  * @retval None
  */
void Key_0(void)
{
		Keys_Input_Set_Voltage('0');
		Keys_Input_Set_Current('0');
		Keys_Input_Set_Power('0');
		Keys_Input_RS232('0');
		Keys_Input_CAN('0');
		Keys_Input_Cycle('0');
		Keys_Input_Delay('0');
	  Keys_Input_V_Rise('0');
		Keys_Input_V_Fall('0');
		Keys_Input_I_Rise('0');
		Keys_Input_I_Fall('0');
	  Keys_Input_Recall_Save(0);
}

 /**
  * @brief  数字按键1的扫描服务函数
  * @param	None
  * @retval None
  */
void Key_1(void)
{
		Keys_Input_Set_Voltage('1');
		Keys_Input_Set_Current('1');
		Keys_Input_Set_Power('1');
		Keys_Input_RS232('1');
		Keys_Input_CAN('1');
		Keys_Input_Cycle('1');
		Keys_Input_Delay('1');
	  Keys_Input_V_Rise('1');
		Keys_Input_V_Fall('1');
		Keys_Input_I_Rise('1');
		Keys_Input_I_Fall('1');	
	  Keys_Input_Recall_Save(1);
}

 /**
  * @brief  数字按键2的扫描服务函数
  * @param	None
  * @retval None
  */
void Key_2(void)
{
		Keys_Input_Set_Voltage('2');
		Keys_Input_Set_Current('2');
		Keys_Input_Set_Power('2');
		Keys_Input_RS232('2');
		Keys_Input_CAN('2');
		Keys_Input_Cycle('2');	
    Keys_Input_Delay('2');	
	  Keys_Input_V_Rise('2');
		Keys_Input_V_Fall('2');
		Keys_Input_I_Rise('2');
		Keys_Input_I_Fall('2');	
    Keys_Input_Recall_Save(2);	
}

 /**
  * @brief  数字按键3的扫描服务函数
  * @param	None
  * @retval None
  */
void Key_3(void)
{
		Keys_Input_Set_Voltage('3');
		Keys_Input_Set_Current('3');
		Keys_Input_Set_Power('3');
		Keys_Input_RS232('3');
		Keys_Input_CAN('3');
		Keys_Input_Cycle('3');
		Keys_Input_Delay('3');
	  Keys_Input_V_Rise('3');
		Keys_Input_V_Fall('3');
		Keys_Input_I_Rise('3');
		Keys_Input_I_Fall('3');	
	  Keys_Input_Recall_Save(3);
}

 /**
  * @brief  数字按键4的扫描服务函数
  * @param	None
  * @retval None
  */
void Key_4(void)
{
		Keys_Input_Set_Voltage('4');
		Keys_Input_Set_Current('4');
		Keys_Input_Set_Power('4');
		Keys_Input_RS232('4');
		Keys_Input_CAN('4');
		Keys_Input_Cycle('4');
		Keys_Input_Delay('4');
	  Keys_Input_V_Rise('4');
		Keys_Input_V_Fall('4');
		Keys_Input_I_Rise('4');
		Keys_Input_I_Fall('4');	
	  Keys_Input_Recall_Save(4);
}

 /**
  * @brief  数字按键5的扫描服务函数
  * @param	None
  * @retval None
  */
void Key_5(void)
{
		Keys_Input_Set_Voltage('5');
		Keys_Input_Set_Current('5');
		Keys_Input_Set_Power('5');
		Keys_Input_RS232('5');
		Keys_Input_CAN('5');
		Keys_Input_Cycle('5');
		Keys_Input_Delay('5');
	  Keys_Input_V_Rise('5');
		Keys_Input_V_Fall('5');
		Keys_Input_I_Rise('5');
		Keys_Input_I_Fall('5');	
	  Keys_Input_Recall_Save(5);
}

 /**
  * @brief  数字按键6的扫描服务函数
  * @param	None
  * @retval None
  */
void Key_6(void)
{
		Keys_Input_Set_Voltage('6');
		Keys_Input_Set_Current('6');
		Keys_Input_Set_Power('6');
		Keys_Input_RS232('6');
		Keys_Input_CAN('6');
		Keys_Input_Cycle('6');
		Keys_Input_Delay('6');
	  Keys_Input_V_Rise('6');
		Keys_Input_V_Fall('6');
		Keys_Input_I_Rise('6');
		Keys_Input_I_Fall('6');	
	  Keys_Input_Recall_Save(6);
}

 /**
  * @brief  数字按键7的扫描服务函数
  * @param	None
  * @retval None
  */
void Key_7(void)
{
		Keys_Input_Set_Voltage('7');
		Keys_Input_Set_Current('7');
		Keys_Input_Set_Power('7');
		Keys_Input_RS232('7');
		Keys_Input_CAN('7');
		Keys_Input_Cycle('7');
		Keys_Input_Delay('7');
	  Keys_Input_V_Rise('7');
		Keys_Input_V_Fall('7');
		Keys_Input_I_Rise('7');
		Keys_Input_I_Fall('7');	
	  Keys_Input_Recall_Save(7);
}

 /**
  * @brief  数字按键8的扫描服务函数
  * @param	None
  * @retval None
  */
void Key_8(void)
{
		Keys_Input_Set_Voltage('8');
		Keys_Input_Set_Current('8');
		Keys_Input_Set_Power('8');
		Keys_Input_RS232('8');
		Keys_Input_CAN('8');
		Keys_Input_Cycle('8');
		Keys_Input_Delay('8');
	  Keys_Input_V_Rise('8');
		Keys_Input_V_Fall('8');
		Keys_Input_I_Rise('8');
		Keys_Input_I_Fall('8');	
	  Keys_Input_Recall_Save(8);
}

 /**
  * @brief  数字按键9的扫描服务函数
  * @param	None
  * @retval None
  */
void Key_9(void)
{
		Keys_Input_Set_Voltage('9');
		Keys_Input_Set_Current('9');
		Keys_Input_Set_Power('9');
		Keys_Input_RS232('9');
		Keys_Input_CAN('9');
		Keys_Input_Cycle('9');
		Keys_Input_Delay('9');
	  Keys_Input_V_Rise('9');
		Keys_Input_V_Fall('9');
		Keys_Input_I_Rise('9');
		Keys_Input_I_Fall('9');	
	  Keys_Input_Recall_Save(9);
}

 /**
  * @brief  小数点按键的扫描服务函数
  * @param	None
  * @retval None
  */
void Key_Decimal_Point(void)
{
  if(LOCK_UNLOCK == UNLOCK)         //按钮真的按下了且处于无锁定状态
	{
		if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_V_set) && (Keys_Encoder_Mode == Keys_Mode) && (Cursor_Position == 1) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))  //处于键盘输入的设定电压模式且光标位置为1
		{
			String_Voltage[1] = '.';       //让光标位置1置小数点
			Cursor_Position ++;            //光标增
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_I_set) && (Keys_Encoder_Mode == Keys_Mode)        //处于键盘输入的设定电流模式且光标位于位置1或者2 
			      && ((Cursor_Position == 1) || (Cursor_Position == 2)))
		{
			String_Current[Cursor_Position] = '.';    //让光标位置置小数点
			Cursor_Position ++;                       //光标增
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_P_set) && (Keys_Encoder_Mode == Keys_Mode)        //处于键盘输入的设定功率模式且光标位于位置1或者2或者3 
			      && ((Cursor_Position == 1) || (Cursor_Position == 2) || (Cursor_Position == 3)) 
		        && (String_Power[1] != '.') && (String_Power[2] != '.') )
		{
			String_Power[Cursor_Position] = '.';    //让光标位置置小数点
			Cursor_Position ++;                       //光标增
		}
	}
}

 /**
  * @brief  显示Delay的设置界面
  * @param	None
  * @retval None
  */
void Display_Oled_Delay(void)
{
  String_Delay[0] = Delay_h / 10 + '0';
	String_Delay[1] = Delay_h % 10 + '0';
	String_Delay[2] = 'h';
	String_Delay[3] = Delay_m / 10 + '0';
	String_Delay[4] = Delay_m % 10 + '0';
	String_Delay[5] = 'm';
	String_Delay[6] = Delay_s / 10 + '0';
	String_Delay[7] = Delay_s % 10 + '0';
	String_Delay[8] = 's';
					
	Write_String_16x32AsicII(0, 0, "Delay Time:");
	Write_String_16x32AsicII(32, 14, String_Delay);  //在显示屏显示
}

 /**
  * @brief  显示Cycle开启时间的设置界面
  * @param	None
  * @retval None
  */
void Display_Oled_CycleOn(void)
{
	String_Cycle_On[0] = Cycle_On_s / 1000 + '0';
	String_Cycle_On[1] = (Cycle_On_s / 100) % 10 + '0';
	String_Cycle_On[2] = (Cycle_On_s / 10) % 10 + '0';
	String_Cycle_On[3] = Cycle_On_s % 10 + '0';
	String_Cycle_On[4] = 's';
	String_Cycle_On[5] = Cycle_On_ms / 100 + '0';
	String_Cycle_On[6] = (Cycle_On_ms / 10) % 10 + '0';
	String_Cycle_On[7] = Cycle_On_ms % 10 + '0';
	String_Cycle_On[8] = 'm';
	String_Cycle_On[9] = 's';
	
  Write_String_16x32AsicII(0, 0, "Open Time:");          //在OLED屏显示				
	Write_String_16x32AsicII(32, 12, String_Cycle_On);
}
	
 /**
  * @brief  显示Cycle关闭时间的设置界面
  * @param	None
  * @retval None
  */
void Display_Oled_CycleClose(void)
{
	String_Cycle_Close[0] = Cycle_Close_s / 1000 + '0';
	String_Cycle_Close[1] = (Cycle_Close_s / 100) % 10 + '0';
	String_Cycle_Close[2] = (Cycle_Close_s / 10) % 10 + '0';
	String_Cycle_Close[3] = Cycle_Close_s % 10 + '0';
	String_Cycle_Close[4] = 's';
	String_Cycle_Close[5] = Cycle_Close_ms / 100 + '0';
	String_Cycle_Close[6] = (Cycle_Close_ms / 10) % 10 + '0';
	String_Cycle_Close[7] = Cycle_Close_ms % 10 + '0';
	String_Cycle_Close[8] = 'm';
	String_Cycle_Close[9] = 's';
	
  Write_String_16x32AsicII(0, 0, "Close Time:");          //在OLED屏显示				
	Write_String_16x32AsicII(32, 12, String_Cycle_Close);
}

 /**
  * @brief  显示V-Rise时间的设置界面
  * @param	None
  * @retval None
  */
void Display_Oled_V_Rise(void)
{
	String_V_Rise[0] = V_Rise_s / 1000 + '0';
	String_V_Rise[1] = (V_Rise_s / 100) % 10 + '0';
	String_V_Rise[2] = (V_Rise_s / 10) % 10 + '0';
	String_V_Rise[3] = V_Rise_s % 10 + '0';
	String_V_Rise[4] = 's';
	String_V_Rise[5] = V_Rise_ms / 100 + '0';
	String_V_Rise[6] = (V_Rise_ms / 10) % 10 + '0';
	String_V_Rise[7] = V_Rise_ms % 10 + '0';
	String_V_Rise[8] = 'm';
	String_V_Rise[9] = 's';
	
  Write_String_16x32AsicII(0, 0, "V-Rise:");          //在OLED屏显示				
	Write_String_16x32AsicII(32, 12, String_V_Rise);
}

 /**
  * @brief  显示V-Fall时间的设置界面
  * @param	None
  * @retval None
  */
void Display_Oled_V_Fall(void)
{
	String_V_Fall[0] = V_Fall_s / 1000 + '0';
	String_V_Fall[1] = (V_Fall_s / 100) % 10 + '0';
	String_V_Fall[2] = (V_Fall_s / 10) % 10 + '0';
	String_V_Fall[3] = V_Fall_s % 10 + '0';
	String_V_Fall[4] = 's';
	String_V_Fall[5] = V_Fall_ms / 100 + '0';
	String_V_Fall[6] = (V_Fall_ms / 10) % 10 + '0';
	String_V_Fall[7] = V_Fall_ms % 10 + '0';
	String_V_Fall[8] = 'm';
	String_V_Fall[9] = 's';
	
  Write_String_16x32AsicII(0, 0, "V-Fall:");          //在OLED屏显示				
	Write_String_16x32AsicII(32, 12, String_V_Fall);
}

 /**
  * @brief  显示V-Fall时间的设置界面
  * @param	None
  * @retval None
  */
void Display_Oled_I_Rise(void)
{
	String_I_Rise[0] = I_Rise_s / 1000 + '0';
	String_I_Rise[1] = (I_Rise_s / 100) % 10 + '0';
	String_I_Rise[2] = (I_Rise_s / 10) % 10 + '0';
	String_I_Rise[3] = I_Rise_s % 10 + '0';
	String_I_Rise[4] = 's';
	String_I_Rise[5] = I_Rise_ms / 100 + '0';
	String_I_Rise[6] = (I_Rise_ms / 10) % 10 + '0';
	String_I_Rise[7] = I_Rise_ms % 10 + '0';
	String_I_Rise[8] = 'm';
	String_I_Rise[9] = 's';
	
  Write_String_16x32AsicII(0, 0, "I-Rise:");          //在OLED屏显示				
	Write_String_16x32AsicII(32, 12, String_I_Rise);
}

 /**
  * @brief  显示V-Fall时间的设置界面
  * @param	None
  * @retval None
  */
void Display_Oled_I_Fall(void)
{
	String_I_Fall[0] = I_Fall_s / 1000 + '0';
	String_I_Fall[1] = (I_Fall_s / 100) % 10 + '0';
	String_I_Fall[2] = (I_Fall_s / 10) % 10 + '0';
	String_I_Fall[3] = I_Fall_s % 10 + '0';
	String_I_Fall[4] = 's';
	String_I_Fall[5] = I_Fall_ms / 100 + '0';
	String_I_Fall[6] = (I_Fall_ms / 10) % 10 + '0';
	String_I_Fall[7] = I_Fall_ms % 10 + '0';
	String_I_Fall[8] = 'm';
	String_I_Fall[9] = 's';
	
  Write_String_16x32AsicII(0, 0, "I-Fall:");          //在OLED屏显示				
	Write_String_16x32AsicII(32, 12, String_I_Fall);
}

 /**
  * @brief  按键Enter的扫描服务函数
  * @param	None
  * @retval None
  */
void Key_Enter(void)
{
  if((LOCK_UNLOCK == UNLOCK) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))              //确认按键已经按下了和处于无锁定状态
	{

		if(Display_Interface_Mode == Display_Interface_Mode_First_Menu)  		//当处于第一级菜单   
		{
			Display_Interface_Mode = Display_Interface_Mode_Second_Menu;      //变为第二级菜单
			switch(First_Selection)
			{
				case System: Second_Selection = Reset;break;
				case Timer: Second_Selection = Delay;break;
				case Slope: Second_Selection = V_Rise;break;
				case Communication: Second_Selection = RS232;break;
				case Parallel: Display_Interface_Mode = Display_Interface_Mode_First_Menu;break;
				case Vint_or_Vext: Second_Selection = Menu_Vint; break;
				default: break;
			}
		}
	
/*--------------------------------------------------------------------Vint_or_Vext---------------------------------------*/		
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Second_Menu) && (Second_Selection == Menu_Vint))  //当处于第二级菜单的Vint
		{
			Use_Vint_Or_Vext_Flag = Use_Vint;
					
			AT25_Save_Use_Vint_or_Vext_Flag();		
					
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual; //返回真实值界面
		}
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Second_Menu) && (Second_Selection == Menu_Vext))  //当处于第二级菜单的Vint
		{
			Use_Vint_Or_Vext_Flag = Use_Vext;
					
			AT25_Save_Use_Vint_or_Vext_Flag();		
					
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual; //返回真实值界面
		}
/*------------------------------------------------------------------------RS232-------------------------------------------*/		
		
		else if (((Display_Interface_Mode == Display_Interface_Mode_Second_Menu) || (Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == RS232))  //当处于第二级菜单的RS232
		{
			if(RS232_Set_ID_BaudRate == RS232_First_Select)                         //刚进入RS232设置
			{
			  Display_Interface_Mode = Display_Interface_Mode_Third_Menu;     //变为第三级菜单
			  if(Last_Interface != Display_Interface_Mode)                                //清屏
	      {
				  Fill_RAM(0x00);
        }
				
			  Write_String_16x32AsicII(0, 0, "ID:");
			  Write_String_16x32AsicII(32, 0, "Baud Rate:");
				Display_RS232_BaudRate(RS232_BaudRate);
				
				switch (RS232_BaudRate)
				{
					case 300 : RS232_BaudRate_State = Usart300; break;
          case 600 : RS232_BaudRate_State = Usart600;break;
          case 1200 : RS232_BaudRate_State = Usart1200;break;
          case 2400 : RS232_BaudRate_State = Usart2400;break;
          case 4800 : RS232_BaudRate_State = Usart4800;break;
          case 9600 : RS232_BaudRate_State = Usart9600;break;
          case 19200 : RS232_BaudRate_State = Usart19200;break;
          case 38400 : RS232_BaudRate_State = Usart38400;break;
          case 43000 : RS232_BaudRate_State = Usart43000;break;
          case 56000 : RS232_BaudRate_State = Usart56000;break;
          case 57600 : RS232_BaudRate_State = Usart57600;break;
          case 115200 : RS232_BaudRate_State = Usart115200;break;
					default: break;
				}
				
				Display_RS232_ID(RS232_ID);                                    //显示ID
				
			  Last_Interface = Display_Interface_Mode_Third_Menu;            
				
				RS232_Set_ID_BaudRate = RS232_Set_ID;                          //变为设置ID号模式
				
        Keys_Encoder_Mode = Encoder_Mode;                              //默认用编码器输入				
				
				Cursor_Position = 1;                                           //设定光标位置
				
			  TIM_Cmd(TIM5, ENABLE);                                         //使能定时器
			}
			
			else if(RS232_Set_ID_BaudRate == RS232_Set_ID)                   //处于设置ID模式
			{
				RS232_ID = String_To_Float(String_RS232_ID);                   //设定ID
				
				if (RS232_ID > 16)
				{
					RS232_ID = 16;
			  	sprintf(String_RS232_ID, "%2d", RS232_ID);						
				}
				else if (RS232_ID >= 10)
				{
			  	sprintf(String_RS232_ID, "%2d", RS232_ID);			
				}
				else if(RS232_ID > 0)
				{
			  	sprintf(String_RS232_ID, "%2d", RS232_ID);
          String_RS232_ID[0] = '0';				
				}
			  else 
				{
					RS232_ID = 1;
			  	sprintf(String_RS232_ID, "%2d", RS232_ID);
          String_RS232_ID[0] = '0';						
				}
				
				Write_String_16x32AsicII(0, 12, String_RS232_ID);
				
				RS232_Set_ID_BaudRate = RS232_Set_BaudRate;                    //变为设置波特率模式
				
				Keys_Encoder_Mode = Encoder_Mode;                              //变回编码器模式
			}
			
			else if(RS232_Set_ID_BaudRate == RS232_Set_BaudRate)            //处于设置波特率模式
			{
				RS232_Set_ID_BaudRate = RS232_First_Select;                   //变回第一次选择RS232
				
				switch (RS232_BaudRate_State)
				{
					case Usart300 : RS232_BaudRate = 300; break;
          case Usart600 : RS232_BaudRate = 600;break;
          case Usart1200 : RS232_BaudRate = 1200;break;
          case Usart2400 : RS232_BaudRate = 2400;break;
          case Usart4800 : RS232_BaudRate = 4800;break;
          case Usart9600 : RS232_BaudRate = 9600;break;
          case Usart19200 : RS232_BaudRate = 19200;break;
          case Usart38400 : RS232_BaudRate = 38400;break;
          case Usart43000 : RS232_BaudRate = 43000;break;
          case Usart56000 : RS232_BaudRate = 56000;break;
          case Usart57600 : RS232_BaudRate = 57600;break;
          case Usart115200 : RS232_BaudRate = 115200;break;
					default: break;
				}
				
				AT25_Save_RS232();
				
	      Usart2_Config(RS232_BaudRate);
				
				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual; //返回真实值界面
			}
		}
		
/*----------------------------------------------------------------------------------CAN--------------------------------------------------------------------------*/		
		
		else if (((Display_Interface_Mode == Display_Interface_Mode_Second_Menu) || (Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == CAN))  //当处于第二级菜单的CAN
		{
			if(CAN_Set_ID_BaudRate == CAN_First_Select)                         //刚进入CAN设置
			{
			  Display_Interface_Mode = Display_Interface_Mode_Third_Menu;     //变为第三级菜单
			  if(Last_Interface != Display_Interface_Mode)                                //清屏
	      {
				  Fill_RAM(0x00);
        }
				
			  Write_String_16x32AsicII(0, 0, "ID:");
			  Write_String_16x32AsicII(32, 0, "Baud Rate:");
				Display_CAN_BaudRate(CAN_BaudRate);
				Write_String_16x32AsicII(32, 52, "K");
				
				switch (CAN_BaudRate)
				{
					case 5 :   CAN_BaudRate_State = CAN5; break;
          case 10 :  CAN_BaudRate_State = CAN10;break;
          case 20 :  CAN_BaudRate_State = CAN20;break;
          case 25 :  CAN_BaudRate_State = CAN25;break;
          case 40 :  CAN_BaudRate_State = CAN40;break;
          case 50 :  CAN_BaudRate_State = CAN50;break;
          case 80 :  CAN_BaudRate_State = CAN80;break;
          case 100 : CAN_BaudRate_State = CAN100;break;
          case 125 : CAN_BaudRate_State = CAN125;break;
          case 200 : CAN_BaudRate_State = CAN200;break;
          case 250 : CAN_BaudRate_State = CAN250;break;
          case 400 : CAN_BaudRate_State = CAN400;break;
					case 500 : CAN_BaudRate_State = CAN500;break;
					case 800 : CAN_BaudRate_State = CAN800;break;
					default: break;
				}
				
				Display_CAN_ID(CAN_ID);                                    //显示ID
				
			  Last_Interface = Display_Interface_Mode_Third_Menu;            
				
				CAN_Set_ID_BaudRate = CAN_Set_ID;                          //变为设置ID号模式
				
        Keys_Encoder_Mode = Encoder_Mode;                              //默认用编码器输入				
				
				Cursor_Position = 1;                                           //设定光标位置
				
			  TIM_Cmd(TIM5, ENABLE);                                         //使能定时器
			}
			
			else if(CAN_Set_ID_BaudRate == CAN_Set_ID)                   //处于设置ID模式
			{
				CAN_ID = String_To_Float(String_CAN_ID);                   //设定ID
				
				if (CAN_ID > 16)
				{
					CAN_ID = 16;
			  	sprintf(String_RS232_ID, "%2d", CAN_ID);						
				}
				else if (CAN_ID >= 10)
				{
			  	sprintf(String_CAN_ID, "%2d", CAN_ID);				
				}
				else if(CAN_ID > 0)
				{
			  	sprintf(String_CAN_ID, "%2d", CAN_ID);
          String_CAN_ID[0] = '0';					
				}
				else
				{
					CAN_ID = 1;
			  	sprintf(String_CAN_ID, "%2d", CAN_ID);
          String_CAN_ID[0] = '0';						
				}
				
				Write_String_16x32AsicII(0, 12, String_CAN_ID);
				
				CAN_Set_ID_BaudRate = CAN_Set_BaudRate;                    //变为设置波特率模式
				
				Keys_Encoder_Mode = Encoder_Mode;                              //变回编码器模式
			}
			
			else if(CAN_Set_ID_BaudRate == CAN_Set_BaudRate)            //处于设置波特率模式
			{
				CAN_Set_ID_BaudRate = CAN_First_Select;                   //变回第一次选择CAN
				
				switch (CAN_BaudRate_State)
				{
					case CAN5 :   CAN_BaudRate = 5; break;
          case CAN10 :  CAN_BaudRate = 10;break;
          case CAN20 :  CAN_BaudRate = 20;break;
          case CAN25 :  CAN_BaudRate = 25;break;
          case CAN40 :  CAN_BaudRate = 40;break;
          case CAN50 :  CAN_BaudRate = 50;break;
          case CAN80 :  CAN_BaudRate = 80;break;
          case CAN100 : CAN_BaudRate = 100;break;
          case CAN125 : CAN_BaudRate = 125;break;
          case CAN200 : CAN_BaudRate = 200;break;
          case CAN250 : CAN_BaudRate = 250;break;
          case CAN400 : CAN_BaudRate = 400;break;
          case CAN500 : CAN_BaudRate = 500;break;					
          case CAN800 : CAN_BaudRate = 800;break;					
					default: break;
				}
					
				AT25_Save_CAN();
				
				CAN_Config(CAN_BaudRate);
				
				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual; //返回真实值界面
			}
		}		
		
		
/*-------------------------------------------------------------------------------------------------Cycle---------------------------------------------------------------*/		
		

		else if (((Display_Interface_Mode == Display_Interface_Mode_Second_Menu) || (Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == Cycle))  //当处于第二级菜单的Cycle
		{
			if (Cycle_Enter == Cycle_Enter_0)    //第一次选择Cycle
			{
				Keys_Encoder_Mode = Encoder_Mode;
				
			  Display_Interface_Mode = Display_Interface_Mode_Third_Menu;     //变为第三级菜单

			  if(Last_Interface != Display_Interface_Mode)                                //清屏
	      {
				  Fill_RAM(0x00);
        }				
				
				Display_Oled_CycleOn();                                        //将循环时间秒转为字符串并显示
				
				Display_Interface_Mode = Display_Interface_Mode_Third_Menu;    //将菜单变成三级菜单
				
			  Last_Interface = Display_Interface_Mode_Third_Menu;   
				
        Cycle_Enter = Cycle_Enter_1;                //记录按了一次按键
				
				Cursor_Position = 7;
				
				TIM_Cmd(TIM5, ENABLE);                                         //使能定时器
			}
			
			
			
			else if (Cycle_Enter == Cycle_Enter_1)                     //第二次按按键
			{
				Cycle_Enter = Cycle_Enter_2;       //记录按了两次按键
				
				Display_Oled_CycleClose();                                        //将循环时间秒转为字符串并显示
				
				Cursor_Position = 7;
				
				Keys_Encoder_Mode = Encoder_Mode;                              //变回编码器模式							
			}
			
			
			
			else if (Cycle_Enter == Cycle_Enter_2)                 //第三次按按键
			{			 
				u8 i;
				
				Cycle_Enter = Cycle_Enter_3;      //记录按了三次按键 

				//将字符串变回数字
				for(i = 0; i < 11; i++)
				{
					if(String_Cycle_On[i] == ' ')
					{
						String_Cycle_On[i] = '0';
					}
					if(String_Cycle_Close[i] == ' ')
					{
						String_Cycle_Close[i] = '0';
					}
				}
				
        Cycle_On_s = (String_Cycle_On[0] - '0') * 1000 + (String_Cycle_On[1] - '0') * 100 + (String_Cycle_On[2] - '0') * 10 + (String_Cycle_On[3] - '0');	
        Cycle_On_ms = (String_Cycle_On[5] - '0') * 100 + (String_Cycle_On[6] - '0') * 10 + (String_Cycle_On[7] - '0');
				Cycle_Close_s = (String_Cycle_Close[0] - '0') * 1000 + (String_Cycle_Close[1] - '0') * 100 + (String_Cycle_Close[2] - '0') * 10 + (String_Cycle_Close[3] - '0');	
        Cycle_Close_ms = (String_Cycle_Close[5] - '0') * 100 + (String_Cycle_Close[6] - '0') * 10 + (String_Cycle_Close[7] - '0');

				if (Cycle_On_s > 9999)
				{
					Cycle_On_s = 9999;
				}
				if (Cycle_On_ms > 1000)
				{
					Cycle_On_ms = 1000;				
				}
				if (Cycle_Close_s > 9999)
				{
					Cycle_Close_s = 9999;
				}
				if (Cycle_Close_ms > 1000)
				{
					Cycle_On_ms = 1000;					
				}				
				
				Keys_Encoder_Mode = Encoder_Mode;                              //变回编码器模式  

				Cycle_Enter = Cycle_Enter_0;    //变回没按过enter的状态
				
				if(((Cycle_On_s == 0) && (Cycle_On_ms == 0)) || ((Cycle_Close_s == 0) && (Cycle_Close_ms == 0)))
				{
					Cycle_Function_On_Off = Cycle_Function_Off;
				}
				else if(Cycle_Function_On_Off == Cycle_Function_Off)
				{
					Cycle_Function_On_Off = Cycle_Function_On;
				}
				
			  AT25_Save_Cycle();		

				if(OUTPUT_NOT_OUTPUT == OUTPUT)
		    {
    			Output_Zero();
					
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

				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual; //返回真实值界面
			}
			
		}
		
/*------------------------------------------------------------------Delay-----------------------------------------------------------------------------*/
		
		else if (((Display_Interface_Mode == Display_Interface_Mode_Second_Menu) || (Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == Delay))  //当处于第二级菜单的Delay
		{     
			if(Delay_Enter == Delay_Enter_0)                     //如果没按过Enter
			{
				Keys_Encoder_Mode = Encoder_Mode;                          //一开始处于编码器设置模式
			
		    if(Last_Interface != Display_Interface_Mode)                                //清屏
				{
					Fill_RAM(0x00);
				}				
				
				Display_Interface_Mode = Display_Interface_Mode_Third_Menu;			//处于选择第三层菜单模式	
				
				Display_Oled_Delay();                           //显示界面
				
				Last_Interface = Display_Interface_Mode_Third_Menu;   
				
				Cursor_Position = 7;			//一开始光标处于数组第七位
				
				Delay_Enter = Delay_Enter_1;   //记录按了一次Enter
				
				TIM_Cmd(TIM5, ENABLE);				//打开定时器显示光标
			}
			
			else if (Delay_Enter == Delay_Enter_1)       //第二次按按键
			{
				u8 i;
				
				Delay_Enter = Delay_Enter_2;
				
				//将字符串变回数字
				for(i = 0; i < 9; i++)
				{
					if(String_Delay[i] == ' ')
					{
						String_Delay[i] = '0';
					}
				}
				
				Delay_h = (String_Delay[0] - '0') * 10 + (String_Delay[1] - '0');
				Delay_m = (String_Delay[3] - '0') * 10 + (String_Delay[4] - '0');
				Delay_s = (String_Delay[6] - '0') * 10 + (String_Delay[7] - '0');
				
				if(Delay_h > 99)
				{
					Delay_h = 99;
				}
				if(Delay_m > 59)
				{
					Delay_m = 59;
				}
				if(Delay_s > 59)
				{
					Delay_s = 59;
				}

				Keys_Encoder_Mode = Encoder_Mode;                              //变回编码器模式	
				
				if((Delay_h == 0) && (Delay_m == 0) && (Delay_s == 0))      //判断是否打开功能
				{
					Delay_Function_On_Off = Delay_Function_Off;
				}
				else if(Delay_Function_On_Off == Delay_Function_Off)
				{ 					
					Delay_Function_On_Off = Delay_Function_On;
				}

        AT25_Save_Delay();		
				
				
				if(OUTPUT_NOT_OUTPUT == OUTPUT)
		    {
					Output_Zero();
					
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
				

				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual; //返回真实值界面

				Delay_Enter = Delay_Enter_0;
			}
		}			
		
/*--------------------------------------------------------------------V_RISE---------------------------------------------------------------*/
		
		else if (((Display_Interface_Mode == Display_Interface_Mode_Second_Menu) || (Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == V_Rise))  //当处于第二级菜单的V-Rise
		{
			if (V_Rise_Enter == V_Rise_Enter_0)    //第一次选择V_Rise
			{                             
				V_Rise_Enter = V_Rise_Enter_1;
				
				Keys_Encoder_Mode = Encoder_Mode;   //编码器模式
				
			  Display_Interface_Mode = Display_Interface_Mode_Third_Menu;     //变为第三级菜单
				
			  if(Last_Interface != Display_Interface_Mode)                                //清屏
	      {
				  Fill_RAM(0x00);
        }				
				
				Display_Oled_V_Rise();                                        //将循环时间秒转为字符串

				Display_Interface_Mode = Display_Interface_Mode_Third_Menu;
				
			  Last_Interface = Display_Interface_Mode_Third_Menu;   
				
				Cursor_Position = 7;
				
				TIM_Cmd(TIM5, ENABLE);                                         //使能定时器
			}
			
			else if (V_Rise_Enter== V_Rise_Enter_1)        //第二次按按键
			{
				u8 i;
				
				V_Rise_Enter = V_Rise_Enter_2;

				//将字符串变回数字
				for(i = 0; i < 11; i++)
				{
					if(String_V_Rise[i] == ' ')
					{
						String_V_Rise[i] = '0';
					}
				}
				
        V_Rise_s = (String_V_Rise[0] - '0') * 1000 + (String_V_Rise[1] - '0') * 100 + (String_V_Rise[2] - '0') * 10 + (String_V_Rise[3] - '0');	
        V_Rise_ms = (String_V_Rise[5] - '0') * 100 + (String_V_Rise[6] - '0') * 10 + (String_V_Rise[7] - '0');

				if (V_Rise_s > 9999)
				{
					V_Rise_s = 9999;
				}
				if (V_Rise_ms > 1000)
				{
					V_Rise_ms = 1000;				
				}				
				
				Keys_Encoder_Mode = Encoder_Mode;                              //变回编码器模式				
				
				if((V_Rise_s == 0) && (V_Rise_ms == 0))
				{
					V_Rise_On_Off = V_Rise_Off;
				}
				else if(V_Rise_On_Off == V_Rise_Off)
				{
					V_Rise_On_Off = V_Rise_On;
				}
				
        AT25_Save_V_Rise();			

				if(OUTPUT_NOT_OUTPUT == OUTPUT)
		    {
					Output_Zero();
					
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

				V_Rise_Enter = V_Rise_Enter_0;

				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual; //返回真实值界面				
			}
		}

/*--------------------------------------------------------------------V_FALL---------------------------------------------------------------*/
		
		else if (((Display_Interface_Mode == Display_Interface_Mode_Second_Menu) || (Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == V_Fall))  //当处于第二级菜单的V-Fall
		{
			if (V_Fall_Enter == V_Fall_Enter_0)    //第一次选择V_Fall
			{                             
				V_Fall_Enter = V_Fall_Enter_1;
				
				Keys_Encoder_Mode = Encoder_Mode;   //编码器模式
				
			  Display_Interface_Mode = Display_Interface_Mode_Third_Menu;     //变为第三级菜单
				
			  if(Last_Interface != Display_Interface_Mode)                                //清屏
	      {
				  Fill_RAM(0x00);
        }				
				
				Display_Oled_V_Fall();                                        //将循环时间秒转为字符串

				Display_Interface_Mode = Display_Interface_Mode_Third_Menu;
				
			  Last_Interface = Display_Interface_Mode_Third_Menu;   
				
				Cursor_Position = 7;
				
				TIM_Cmd(TIM5, ENABLE);                                         //使能定时器
			}
			
			else if (V_Fall_Enter== V_Fall_Enter_1)        //第二次按按键
			{
				u8 i;
				
				V_Fall_Enter = V_Fall_Enter_2;

				//将字符串变回数字
				for(i = 0; i < 11; i++)
				{
					if(String_V_Fall[i] == ' ')
					{
						String_V_Fall[i] = '0';
					}
				}
				
        V_Fall_s = (String_V_Fall[0] - '0') * 1000 + (String_V_Fall[1] - '0') * 100 + (String_V_Fall[2] - '0') * 10 + (String_V_Fall[3] - '0');	
        V_Fall_ms = (String_V_Fall[5] - '0') * 100 + (String_V_Fall[6] - '0') * 10 + (String_V_Fall[7] - '0');

				if (V_Fall_s > 9999)
				{
					V_Fall_s = 9999;
				}
				if (V_Fall_ms > 1000)
				{
					V_Fall_ms = 1000;				
				}				
				
				Keys_Encoder_Mode = Encoder_Mode;                              //变回编码器模式				
				
				if((V_Fall_s == 0) && (V_Fall_ms == 0))
				{
					V_Fall_On_Off = V_Fall_Off;
				}
				else if(V_Fall_On_Off == V_Fall_Off)
				{
					V_Fall_On_Off = V_Fall_On;
				}
				
        AT25_Save_V_Fall();

				if(OUTPUT_NOT_OUTPUT == OUTPUT)
		    {
					Output_Zero();
					
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

				V_Fall_Enter = V_Fall_Enter_0;

				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual; //返回真实值界面				
			}
		}

/*--------------------------------------------------------------------I_RISE---------------------------------------------------------------*/
		
		else if (((Display_Interface_Mode == Display_Interface_Mode_Second_Menu) || (Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == I_Rise))  //当处于第二级菜单的V-Rise
		{	
			if (I_Rise_Enter == I_Rise_Enter_0)    //第一次选择I_Rise
			{                             
				I_Rise_Enter = I_Rise_Enter_1;
				
				Keys_Encoder_Mode = Encoder_Mode;   //编码器模式
				
			  Display_Interface_Mode = Display_Interface_Mode_Third_Menu;     //变为第三级菜单
				
			  if(Last_Interface != Display_Interface_Mode)                                //清屏
	      {
				  Fill_RAM(0x00);
        }				
				
				Display_Oled_I_Rise();                                        //将循环时间秒转为字符串

				Display_Interface_Mode = Display_Interface_Mode_Third_Menu;
				
			  Last_Interface = Display_Interface_Mode_Third_Menu;   
				
				Cursor_Position = 7;
				
				TIM_Cmd(TIM5, ENABLE);                                         //使能定时器
			}
			
			else if (I_Rise_Enter== I_Rise_Enter_1)        //第二次按按键
			{
				u8 i;
				
				I_Rise_Enter = I_Rise_Enter_2;

				//将字符串变回数字
				for(i = 0; i < 11; i++)
				{
					if(String_I_Rise[i] == ' ')
					{
						String_I_Rise[i] = '0';
					}
				}
				
        I_Rise_s = (String_I_Rise[0] - '0') * 1000 + (String_I_Rise[1] - '0') * 100 + (String_I_Rise[2] - '0') * 10 + (String_I_Rise[3] - '0');	
        I_Rise_ms = (String_I_Rise[5] - '0') * 100 + (String_I_Rise[6] - '0') * 10 + (String_I_Rise[7] - '0');

				if (I_Rise_s > 9999)
				{
					I_Rise_s = 9999;
				}
				if (I_Rise_ms > 1000)
				{
					I_Rise_ms = 1000;				
				}				
				
				Keys_Encoder_Mode = Encoder_Mode;                              //变回编码器模式				
				
				if((I_Rise_s == 0) && (I_Rise_ms == 0))
				{
					I_Rise_On_Off = I_Rise_Off;
				}
				else if(I_Rise_On_Off == I_Rise_Off)
				{
					I_Rise_On_Off = I_Rise_On;
				}
				
        AT25_Save_I_Rise();				
				
				if(OUTPUT_NOT_OUTPUT == OUTPUT)
		    {
					Output_Zero();
					
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

				I_Rise_Enter = I_Rise_Enter_0;

				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual; //返回真实值界面				
			}
		}

/*--------------------------------------------------------------------I_FALL---------------------------------------------------------------*/
		
		else if (((Display_Interface_Mode == Display_Interface_Mode_Second_Menu) || (Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == I_Fall))  //当处于第二级菜单的V-Rise
		{	
			if (I_Fall_Enter == I_Fall_Enter_0)    //第一次选择I_Fall
			{                             
				I_Fall_Enter = I_Fall_Enter_1;
				
				Keys_Encoder_Mode = Encoder_Mode;   //编码器模式
				
			  Display_Interface_Mode = Display_Interface_Mode_Third_Menu;     //变为第三级菜单
				
			  if(Last_Interface != Display_Interface_Mode)                                //清屏
	      {
				  Fill_RAM(0x00);
        }				
				
				Display_Oled_I_Fall();                                        //将循环时间秒转为字符串

				Display_Interface_Mode = Display_Interface_Mode_Third_Menu;
				
			  Last_Interface = Display_Interface_Mode_Third_Menu;   
				
				Cursor_Position = 7;
				
				TIM_Cmd(TIM5, ENABLE);                                         //使能定时器
			}
			
			else if (I_Fall_Enter== I_Fall_Enter_1)        //第二次按按键
			{
				u8 i;
				
				I_Fall_Enter = I_Fall_Enter_2;

				//将字符串变回数字
				for(i = 0; i < 11; i++)
				{
					if(String_I_Fall[i] == ' ')
					{
						String_I_Fall[i] = '0';
					}
				}
				
        I_Fall_s = (String_I_Fall[0] - '0') * 1000 + (String_I_Fall[1] - '0') * 100 + (String_I_Fall[2] - '0') * 10 + (String_I_Fall[3] - '0');	
        I_Fall_ms = (String_I_Fall[5] - '0') * 100 + (String_I_Fall[6] - '0') * 10 + (String_I_Fall[7] - '0');

				if (I_Fall_s > 9999)
				{
					I_Fall_s = 9999;
				}
				if (I_Fall_ms > 1000)
				{
					I_Fall_ms = 1000;				
				}				
				
				Keys_Encoder_Mode = Encoder_Mode;                              //变回编码器模式				
				
				if((I_Fall_s == 0) && (I_Fall_ms == 0))
				{
					I_Fall_On_Off = I_Fall_Off;
				}
				else if(I_Fall_On_Off == I_Fall_Off)
				{
					I_Fall_On_Off = I_Fall_On;
				}
				
        AT25_Save_I_Fall();		

				if(OUTPUT_NOT_OUTPUT == OUTPUT)
		    {
					Output_Zero();
					
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

				I_Fall_Enter = I_Fall_Enter_0;

				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual; //返回真实值界面				
			}
		}	
		
/*--------------------------------------------------------------------Reset---------------------------------------------------------*/		
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Second_Menu) && (Second_Selection == Reset))    //处于二级菜单，选择了复位，系统复位
		{
			AT25_Reset();
			NVIC_SystemReset();
		}
		
		
		
/*--------------------------------------------------------------------设定V I P值--------------------------------------------------*/		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_V_set) && (Keys_Encoder_Mode == Encoder_Mode))   //当处于设定电压模式和编码器模式
		{
		  Set_Voltage = String_To_Float(String_Voltage);   //将字符串的设定电压变为float的设定电压
			if(Set_Voltage > 80)     //电压最大值为80
			{
				Set_Voltage = 80;
			}
			CV_CC_CP = CV;		
			AT25_Save_VIPSet();
			AT25_Save_CC_CV_CP();
			
			if((OUTPUT_NOT_OUTPUT == OUTPUT) && (DELAY_NOT_DELAY == NOT_DELAY) && (ON_OFF == ON))     //处于输出模式并且延时时间结束,当改变设定值以后要改变输出电压
			{
				Power_On();
			}
			
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;    //变回真实值主菜单显示模式
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_V_set) && (Keys_Encoder_Mode == Keys_Mode))   //当处于设定电压模式和键盘模式
		{
			Set_Voltage = String_To_Float(String_Voltage);     //将字符串的设定电压变为float的设定电压
			if(Set_Voltage > 80)     //电压最大值为80
			{
				Set_Voltage = 80;
			}
			Keys_Encoder_Mode = Encoder_Mode;
			CV_CC_CP = CV;
			AT25_Save_VIPSet();
			AT25_Save_CC_CV_CP();
			
			if((OUTPUT_NOT_OUTPUT == OUTPUT) && (DELAY_NOT_DELAY == NOT_DELAY) && (ON_OFF == ON))     //处于输出模式并且延时时间结束,当改变设定值以后要改变输出电压
			{
				Power_On();
			}
			
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_I_set) && (Keys_Encoder_Mode == Encoder_Mode))   //当处于设定电流模式和编码器模式
		{
		  Set_Current = String_To_Float(String_Current);      //将字符串的设定电流变为float的设定电流
			if(Set_Current > 100)     //电流最大值为100
			{
				Set_Current = 100;
			}
			CV_CC_CP = CC;
			AT25_Save_VIPSet();
			AT25_Save_CC_CV_CP();
			
			if((OUTPUT_NOT_OUTPUT == OUTPUT) && (DELAY_NOT_DELAY == NOT_DELAY) && (ON_OFF == ON))     //处于输出模式并且延时时间结束,当改变设定值以后要改变输出电压
			{
				Power_On();
			}
			
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;    //变回真实值主菜单显示模式
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_I_set) && (Keys_Encoder_Mode == Keys_Mode))   //当处于设定电流模式和键盘模式
		{
			Set_Current = String_To_Float(String_Current);     //将字符串的设定电压变为float的设定电压
			if(Set_Current > 100)     //电流最大值为100
			{
				Set_Current = 100;
			}
			CV_CC_CP = CC;
			Keys_Encoder_Mode = Encoder_Mode;          //变回编码器模式
			AT25_Save_VIPSet();
			AT25_Save_CC_CV_CP();
			
			if((OUTPUT_NOT_OUTPUT == OUTPUT) && (DELAY_NOT_DELAY == NOT_DELAY) && (ON_OFF == ON))     //处于输出模式并且延时时间结束,当改变设定值以后要改变输出电压
			{
				Power_On();
			}
			
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;      //变回真实值显示界面
		}
    
    else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_P_set) && (Keys_Encoder_Mode == Encoder_Mode))    //当处于设定功率模式和编码器模式
		{
		  Set_Power = String_To_Float(String_Power);      //将字符串的设定电流变为Power的设定电流
			if(Set_Power > 6000)        //功率最大值为6000
			{
				Set_Power = 6000;
			}
			CV_CC_CP = CP;
			AT25_Save_VIPSet();
			AT25_Save_CC_CV_CP();
			
			if((OUTPUT_NOT_OUTPUT == OUTPUT) && (DELAY_NOT_DELAY == NOT_DELAY) && (ON_OFF == ON))     //处于输出模式并且延时时间结束,当改变设定值以后要改变输出电压
			{
				Power_On();
			}
			
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;      //变回真实值主菜单显示模式			
		}			
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_P_set) && (Keys_Encoder_Mode == Keys_Mode))   //当处于设定功率模式和键盘模式
		{
			Set_Power = String_To_Float(String_Power);     //将字符串的设定电压变为float的设定电压
			if(Set_Power > 6000)     //电流最大值为100
			{
				Set_Power = 6000;
			}
			CV_CC_CP = CP;
			Keys_Encoder_Mode = Encoder_Mode;          //变回编码器模式
			AT25_Save_VIPSet();
			AT25_Save_CC_CV_CP();
			
			if((OUTPUT_NOT_OUTPUT == OUTPUT) && (DELAY_NOT_DELAY == NOT_DELAY) && (ON_OFF == ON))     //处于输出模式并且延时时间结束,当改变设定值以后要改变输出电压
			{
				Power_On();
			}
			
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;      //变回真实值显示界面
		}
		
		else if(Display_Interface_Mode == Display_Interface_Mode_Recall_Interface)    //处于取模式
		{
			Set_Voltage = Recall_Save_Voltage[Recall_Save_Number];
			Set_Current = Recall_Save_Current[Recall_Save_Number];
			Set_Power = Recall_Save_Power[Recall_Save_Number];
			AT25_Save_VIPSet();
			
			if((OUTPUT_NOT_OUTPUT == OUTPUT) && (DELAY_NOT_DELAY == NOT_DELAY) && (ON_OFF == ON))     //处于输出模式并且延时时间结束,当改变设定值以后要改变输出电压
			{
				Power_On();
			}
			
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;
		}
		
		else if(Display_Interface_Mode == Display_Interface_Mode_Save_Interface)      //处于存模式
		{
			Recall_Save_Voltage[Recall_Save_Number] = Set_Voltage;
			Recall_Save_Current[Recall_Save_Number] = Set_Current;
			Recall_Save_Power[Recall_Save_Number] = Set_Power;
			AT25_Save_RecallSaveSet(Recall_Save_Number);
			
			if((OUTPUT_NOT_OUTPUT == OUTPUT) && (DELAY_NOT_DELAY == NOT_DELAY) && (ON_OFF == ON))     //处于输出模式并且延时时间结束,当改变设定值以后要改变输出电压
			{
				Power_On();
			}
			
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;
		}
	}
}

 /**
  * @brief  按键Shift的扫描服务函数
  * @param	None
  * @retval None
  */
void Key_Shift(void)
{
  if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Actual) || (Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Preset))     //确认按键真的按下了,且在设定值或真实值界面
	{
		if(Shift_Not_Shift == Not_Shift)               //如果没复用，就复用
		{
			Shift_Not_Shift = Shift;
		}
		else if(Shift_Not_Shift == Shift)         //如果复用了，就取消复用
		{
			Shift_Not_Shift = Not_Shift; 
		}
	}
}

 /**
  * @brief  按键ESC的扫描服务函数
  * @param	None
  * @retval None
  */
void Key_ESC(void)
{
  if(LOCK_UNLOCK == UNLOCK)     //确定按键真的按下了和处于无锁定状态
	{
		if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_V_set) && (Keys_Encoder_Mode == Keys_Mode))       //处于设定电压模式和键盘输入模式，返回编码器模式
		{
			Keys_Encoder_Mode = Encoder_Mode;            //返回编码器模式
			Display_Main_Interface_Voltage(Set_Voltage);              //返回默认的电压字符串
			Cursor_Position = 4;
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_I_set) && (Keys_Encoder_Mode == Keys_Mode))       //处于设定电流模式和键盘输入模式，返回编码器模式
		{
			Keys_Encoder_Mode = Encoder_Mode;            //返回编码器模式
			Display_Main_Interface_Current(Set_Current);              //返回默认的电压字符串
			Cursor_Position = 5;
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_P_set) && (Keys_Encoder_Mode == Keys_Mode))       //处于设定功率模式和键盘输入模式，返回编码器模式
		{
			Keys_Encoder_Mode = Encoder_Mode;            //返回编码器模式
			Display_Main_Interface_Power(Set_Power);              //返回默认的电压字符串
			if((Set_Power >= 1000) || (Set_Power <10))
			{
				String_Power[0] = '0';
				Write_Single_12x24AsicII(42, 0, '0');
			}
			Cursor_Position = 4;
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Recall_Interface) || (Display_Interface_Mode == Display_Interface_Mode_Save_Interface))        //处于存取界面
		{
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;
		}
		
/*------------------------------------------------------------------------------------------------RS232---------------------------------------------------------------*/		
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == RS232))           //处于设置RS232参数界面
		{
			if((RS232_Set_ID_BaudRate == RS232_Set_ID) && (Keys_Encoder_Mode == Encoder_Mode))      //只有编码器模式才能返回耳机菜单
			{
				Display_Interface_Mode = Display_Interface_Mode_Second_Menu;
				RS232_Set_ID_BaudRate = RS232_First_Select;
			}	
			else if ((RS232_Set_ID_BaudRate == RS232_Set_ID) && (Keys_Encoder_Mode == Keys_Mode))
			{
				Display_RS232_ID(RS232_ID);
				
				Keys_Encoder_Mode = Encoder_Mode;
				
				Cursor_Position = 2;
			}
			else if(RS232_Set_ID_BaudRate == RS232_Set_BaudRate)
			{
				RS232_Set_ID_BaudRate = RS232_Set_ID;
				
		    switch (RS232_BaudRate_State)
		    {
		  		case Usart300 : Write_String_16x32AsicII(32, 40, "000300");break;
          case Usart600 : Write_String_16x32AsicII(32, 40, "000600");break;
          case Usart1200 : Write_String_16x32AsicII(32, 40, "001200");break;
          case Usart2400 : Write_String_16x32AsicII(32, 40, "002400");break;
          case Usart4800 : Write_String_16x32AsicII(32, 40, "004800");break;
          case Usart9600 : Write_String_16x32AsicII(32, 40, "009600");break;
          case Usart19200 : Write_String_16x32AsicII(32, 40, "019200");break;
          case Usart38400 : Write_String_16x32AsicII(32, 40, "038400");break;
          case Usart43000 : Write_String_16x32AsicII(32, 40, "043000");break;
          case Usart56000 : Write_String_16x32AsicII(32, 40, "056000");break;
          case Usart57600 : Write_String_16x32AsicII(32, 40, "057600");break;
          case Usart115200 : Write_String_16x32AsicII(32, 40, "115200");break;
					default: break;
	    	}				
				
				Keys_Encoder_Mode = Encoder_Mode;
			}
		}
		
/*-----------------------------------------------------------------------------CAN---------------------------------------------------------------------------*/		
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == CAN))           //处于设置CAN参数界面
		{
			if((CAN_Set_ID_BaudRate == CAN_Set_ID) && (Keys_Encoder_Mode == Encoder_Mode))      //只有编码器模式才能返回耳机菜单
			{
				Display_Interface_Mode = Display_Interface_Mode_Second_Menu;
				CAN_Set_ID_BaudRate = CAN_First_Select;
			}	
			else if ((CAN_Set_ID_BaudRate == CAN_Set_ID) && (Keys_Encoder_Mode == Keys_Mode))
			{
				Display_CAN_ID(CAN_ID);
				
				Keys_Encoder_Mode = Encoder_Mode;
				
				Cursor_Position = 2;
			}
			else if(CAN_Set_ID_BaudRate == CAN_Set_BaudRate)
			{
				CAN_Set_ID_BaudRate = CAN_Set_ID;
				
		    switch (CAN_BaudRate_State)
		    {
				  case CAN5 : Write_String_16x32AsicII(32, 40, "005");SysTick_Delay_Ms(10);break;				
				  case CAN10 : Write_String_16x32AsicII(32, 40, "010");SysTick_Delay_Ms(10);break;
          case CAN20 : Write_String_16x32AsicII(32, 40, "020");SysTick_Delay_Ms(10);break;
          case CAN25 : Write_String_16x32AsicII(32, 40, "025");SysTick_Delay_Ms(10);break;
          case CAN40 : Write_String_16x32AsicII(32, 40, "040");SysTick_Delay_Ms(10);break;
          case CAN50 : Write_String_16x32AsicII(32, 40, "050");SysTick_Delay_Ms(10);break;
          case CAN80 : Write_String_16x32AsicII(32, 40, "080");SysTick_Delay_Ms(10);break;
          case CAN100 : Write_String_16x32AsicII(32, 40, "100");SysTick_Delay_Ms(10);break;
          case CAN125 : Write_String_16x32AsicII(32, 40, "125");SysTick_Delay_Ms(10);break;
          case CAN200 : Write_String_16x32AsicII(32, 40, "200");SysTick_Delay_Ms(10);break;
          case CAN250 : Write_String_16x32AsicII(32, 40, "250");SysTick_Delay_Ms(10);break;
          case CAN400 : Write_String_16x32AsicII(32, 40, "400");SysTick_Delay_Ms(10);break;
          case CAN500 : Write_String_16x32AsicII(32, 40, "500");SysTick_Delay_Ms(10);break;
          case CAN800 : Write_String_16x32AsicII(32, 40, "800");SysTick_Delay_Ms(10);break;				
			    default: break;
		   	}				
				
				Keys_Encoder_Mode = Encoder_Mode;
			}
		}

/*---------------------------------------------------------------------------V-Rise--------------------------------------------------------------------*/
    else if (((Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == V_Rise))  //当处于第二级菜单的V_Rise
		{		
			if ((V_Rise_Enter == V_Rise_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //处于设置开启秒
			{
				Display_Interface_Mode = Display_Interface_Mode_Second_Menu;
				
				V_Rise_Enter = V_Rise_Enter_0;				
			}
			else if ((V_Rise_Enter == V_Rise_Enter_1) && (Keys_Encoder_Mode == Keys_Mode))
			{
				Keys_Encoder_Mode = Encoder_Mode;
				
				Cursor_Position = 7;
				
				Display_Oled_V_Rise();                                        //将循环时间秒转为字符串并显示				
			}
		}		
		
/*---------------------------------------------------------------------------V-Fall--------------------------------------------------------------------*/
    else if (((Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == V_Fall))  //当处于第二级菜单的V_Fall
		{		
			if ((V_Fall_Enter == V_Fall_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //处于设置开启秒
			{
				Display_Interface_Mode = Display_Interface_Mode_Second_Menu;
				
				V_Fall_Enter = V_Fall_Enter_0;				
			}
			else if ((V_Fall_Enter == V_Fall_Enter_1) && (Keys_Encoder_Mode == Keys_Mode))
			{
				Keys_Encoder_Mode = Encoder_Mode;
				
				Cursor_Position = 7;
				
				Display_Oled_V_Fall();                                        //将循环时间秒转为字符串并显示				
			}
		}			
		
/*---------------------------------------------------------------------------I-Rise--------------------------------------------------------------------*/
    else if (((Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == I_Rise))  //当处于第二级菜单的I_Rise
		{		
			if ((I_Rise_Enter == I_Rise_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //处于设置开启秒
			{
				Display_Interface_Mode = Display_Interface_Mode_Second_Menu;
				
				I_Rise_Enter = I_Rise_Enter_0;				
			}
			else if ((I_Rise_Enter == I_Rise_Enter_1) && (Keys_Encoder_Mode == Keys_Mode))
			{
				Keys_Encoder_Mode = Encoder_Mode;
				
				Cursor_Position = 7;
				
				Display_Oled_I_Rise();                                        //将循环时间秒转为字符串并显示				
			}
		}	

/*---------------------------------------------------------------------------I-Fall--------------------------------------------------------------------*/
    else if (((Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == I_Fall))  //当处于第二级菜单的I_Fall
		{		
			if ((I_Fall_Enter == I_Fall_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //处于设置开启秒
			{
				Display_Interface_Mode = Display_Interface_Mode_Second_Menu;
				
				I_Fall_Enter = I_Fall_Enter_0;				
			}
			else if ((I_Fall_Enter == I_Fall_Enter_1) && (Keys_Encoder_Mode == Keys_Mode))
			{
				Keys_Encoder_Mode = Encoder_Mode;
				
				Cursor_Position = 7;
				
				Display_Oled_I_Fall();                                        //将循环时间秒转为字符串并显示				
			}
		}			
		
		
/*---------------------------------------------------------------------------Cycle--------------------------------------------------------------------*/
    else if (((Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == Cycle))  //当处于第二级菜单的Cycle
		{		
			if ((Cycle_Enter == Cycle_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //处于设置开启秒
			{
				Display_Interface_Mode = Display_Interface_Mode_Second_Menu;
				
				Cycle_Enter = Cycle_Enter_0;				
			}
			else if ((Cycle_Enter == Cycle_Enter_1) && (Keys_Encoder_Mode == Keys_Mode))
			{
				Keys_Encoder_Mode = Encoder_Mode;
				
				Cursor_Position = 7;
				
				Display_Oled_CycleOn();                                        //将循环时间秒转为字符串并显示				
			}
			
			
			else if ((Cycle_Enter == Cycle_Enter_2) && (Keys_Encoder_Mode == Encoder_Mode))       //处于设置开启毫秒
			{			
				Cycle_Enter = Cycle_Enter_1;

				Display_Oled_CycleOn();		

				Cursor_Position = 7;				
			}
			else if ((Cycle_Enter == Cycle_Enter_2) && (Keys_Encoder_Mode == Keys_Mode))
			{
				Keys_Encoder_Mode = Encoder_Mode;
				
				Cursor_Position = 7;
				
				Display_Oled_CycleClose();				
			}			
		}
		
/*--------------------------------------------------------Delay----------------------------------------------------*/
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Delay))  //当处于第二级菜单的Delay
		{
			if (Delay_Enter == Delay_Enter_1)    //处于设置h模式
			{
				if (Keys_Encoder_Mode == Encoder_Mode)
				{
				  Display_Interface_Mode = Display_Interface_Mode_Second_Menu;
				
				  Delay_Enter = Delay_Enter_0;					
				}
				else if (Keys_Encoder_Mode == Keys_Mode)
				{
				  Keys_Encoder_Mode = Encoder_Mode;
				
				  Cursor_Position = 7;
				
          Display_Oled_Delay();								
				}	
			}
		}		
		
/*-------------------------------------------------------------保护模式-------------------------------------------------------*/
    else if(Display_Interface_Mode == Display_Interface_Mode_Protect_Interface)
		{
//			First_Into_ErrorInterface_Flag = Have_Not_Into;
//			Error = 0;
//			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;
			
//			Flag_Sensor1 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);//观察变量
//			Flag_Sensor2 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);
//			Flag_Sensor3 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
			
				//OTP报警消除
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0)
				{
					SysTick_Delay_Ms(5);
					if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0)
					{
						if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0)
						{
							SysTick_Delay_Ms(5);
							if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0)
							{

										Error &= ~OTP;//将错误值的OTP位置0
										OTP_SensorX = 0;//置零

							}
						}
					}
				}
				
				if(Voltage <= 10)
				{
					Error &= ~OVP;//将错误值的OTP位置0
				}
				
				if((Current <= 10) && (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11) == 1))
				{
					Error &=~ OCP;//将错误值的OCP位置0
					OCP_SoftOrHard_Flag = 2;
				}
			
				if(Power <= 10)
				{
					Error &=~ OPP;//将错误值的OCP位置0
				}			
			  
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 0)
				{
					SysTick_Delay_Ms(5);
					if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 0)
					Error &=~ Sense_Reverse;   //将错误值的Sense_Reverse位置0
				}
				
				if(Error == PF)//全部错误已消除完毕
				{
					First_Into_ErrorInterface_Flag = Have_Not_Into;
					Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;
				}
			
			
		}



		
		
/*-------------------------------------------------------------------菜单--------------------------------------------------------------------------*/		
		
		else
		{
			switch(Display_Interface_Mode)           //返回上级界面
		  {
			  case Display_Interface_Mode_Second_Menu: Display_Interface_Mode = Display_Interface_Mode_First_Menu;break;                          //处于二级菜单，返回到一级菜单
			  case Display_Interface_Mode_First_Menu: Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;break;               //处于一级菜单，返回真实值主菜单
//			  case Display_Interface_Mode_Main_Interface_Preset: Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;break;    //处于设定值显示界面，返回真实值界面
			  case Display_Interface_Mode_Main_Interface_V_set: Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;break;     //处于电压设定界面，返回真实值界面
				case Display_Interface_Mode_Main_Interface_I_set: Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;break;     //处于电流设定界面，返回真实值界面
				case Display_Interface_Mode_Main_Interface_P_set: Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;break;     //处于功率设定界面，返回真实值界面
		  }
		}
	}
}

 /**
  * @brief  按键V-set的扫描服务函数
  * @param	None
  * @retval None
  */
void Key_V_set(void)
{
  if((LOCK_UNLOCK == UNLOCK) && ((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Actual) 
	   || (Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Preset)))           //确认按键真的按下了、处于无锁定状态、处于主界面
	{
		if(Shift_Not_Shift == Not_Shift)                                              //无复用，进入电压设定模式
		{
			Keys_Encoder_Mode = Encoder_Mode;                                           //默认用编码器输入
			
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_V_set;       //进入设定电压模式
			
			Cursor_Position = 4;                                                        //令光标位于最低位
			
			if(Last_Interface != Display_Interface_Mode)                                //清屏
	    {
		    Fill_RAM(0x00);
	    }
			
			Display_Main_Interface_V_set(Set_Current, Set_Power, ON_OFF, CV_CC_CP,      //显示设定电压界面
			                             DELAY_NOT_DELAY, Preset, LOCK_UNLOCK, Use_Vint_Or_Vext_Flag);
	
				Display_Main_Interface_Voltage(Set_Voltage);                              //显示设定电压，并把它转为全局字符串
			
		  Last_Interface = Display_Interface_Mode_Main_Interface_V_set;
			
      TIM_Cmd(TIM5, ENABLE);                                                      //使能定时器，出现光标
		}
		
		
		else if(Shift_Not_Shift == Shift)                                             //复用，进入菜单设定模式
		{
			Display_Interface_Mode = Display_Interface_Mode_First_Menu;                 //显示菜单界面
			
			Shift_Not_Shift = Not_Shift;                                                //取消复用
		}
	}
}

 /**
  * @brief  按键I-set的扫描服务函数
  * @param	None
  * @retval None
  */
void Key_I_set(void)
{
  if((LOCK_UNLOCK == UNLOCK) && ((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Actual) 
	   || (Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Preset)))           //确认按键真的按下了、处于无锁定状态、处于无锁定状态、处于主界面
	{
		if(Shift_Not_Shift == Not_Shift)                                 //无复用，进入电流设置模式
		{
			Keys_Encoder_Mode = Encoder_Mode;                                           //默认用编码器输入
			
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_I_set;       //进入设定电流模式
			
			Cursor_Position = 5;                                                        //令光标位于最低位
			
			if(Last_Interface != Display_Interface_Mode)                                //清屏
	    {
		    Fill_RAM(0x00);
	    }
			
			Display_Main_Interface_I_set(Set_Voltage, Set_Power, ON_OFF, CV_CC_CP,      //显示设定电流界面
			                             DELAY_NOT_DELAY, Preset, LOCK_UNLOCK, Use_Vint_Or_Vext_Flag);
	
			Display_Main_Interface_Current(Set_Current);                                //显示设定电流，并把它转为全局字符串
			
		  Last_Interface = Display_Interface_Mode_Main_Interface_I_set;               //最后一次显示的为电流模式
			
      TIM_Cmd(TIM5, ENABLE);                                                      //使能定时器，出现光标
		}

    else if(Shift_Not_Shift == Shift)                            //复用，进入功率设置模式
		{
			Shift_Not_Shift = Not_Shift;            //取消复用
			
			Keys_Encoder_Mode = Encoder_Mode;                                           //默认用编码器输入
			
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_P_set;       //进入设定功率模式
			
			Cursor_Position = 4;                                                        //令光标位于最低位				

			if(Last_Interface != Display_Interface_Mode)                                //清屏
	    {
		    Fill_RAM(0x00);
	    }
			
			Display_Main_Interface_P_set(Set_Voltage, Set_Current, ON_OFF, CV_CC_CP,      //显示设定功率界面
			                             DELAY_NOT_DELAY, Preset, LOCK_UNLOCK, Use_Vint_Or_Vext_Flag);
	
			Display_Main_Interface_Power(Set_Power);                                    //显示设定功率，并把它转为全局字符串
			
			if((Set_Power >= 1000) || (Set_Power < 10))
			{
				String_Power[0] = '0';
				Write_Single_12x24AsicII(42, 0, '0');
			}
			
		  Last_Interface = Display_Interface_Mode_Main_Interface_P_set;               //最后一次显示的为功率模式
			
      TIM_Cmd(TIM5, ENABLE);                                                      //使能定时器，出现光标			
		}			
	}
}

 /**
  * @brief  按键Up的扫描服务函数
  * @param	None
  * @retval None
  */
void Key_Up(void)
{
  if(LOCK_UNLOCK == UNLOCK)                     //确认按键已经按下和处于无锁定状态
	{
		if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_V_set) && (Keys_Encoder_Mode == Encoder_Mode))  //处于电压设置界面和编码器模式，按一下，让指针向前加1
		{
			Write_Single_16x32AsicII(0, Cursor_Position*4, String_Voltage[Cursor_Position]);
			
			Write_Single_16x32AsicII(0, 20, 'V');
			
			Cursor_Position --;
			
			if(Cursor_Position == -1)          //光标超出范围
			{
				Cursor_Position = 4;
			}
			else if(String_Voltage[Cursor_Position] == '.')     //光标跳过小数点
			{
				Cursor_Position --;
			}
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_I_set) && (Keys_Encoder_Mode == Encoder_Mode))  //处于电流设置界面和编码器模式，按一下，让指针向前加1
		{
			Write_Single_16x32AsicII(0, Cursor_Position*4 + 36, String_Current[Cursor_Position]);
			
			Write_Single_16x32AsicII(0, 60, 'A');
			
			Cursor_Position --;
			
			if(Cursor_Position == -1)    //光标超出范围
			{
				Cursor_Position = 5;
			}
			else if(String_Current[Cursor_Position] == '.')     //光标跳过小数点
			{
				Cursor_Position --;
			}		
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_P_set) && (Keys_Encoder_Mode == Encoder_Mode))   //处于功率设置界面和编码器模式，按一下，让指针向前加1
		{
			Write_Single_12x24AsicII(42, Cursor_Position*3, String_Power[Cursor_Position]);
			
			Write_Single_12x24AsicII(42, 15, 'W');
			
			Cursor_Position --;  //光标移位
			
			if((String_To_Float(String_Power) >= 1000) && (Cursor_Position == 0))
			{
				Cursor_Position --;
			}

			if(Cursor_Position == -1)
			{
			  Cursor_Position = 4;     //光标超出范围				
			}
			
			if(String_Power[Cursor_Position] == '.')     //光标跳过小数点
			{
				Cursor_Position --;
			}
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Recall_Interface) || (Display_Interface_Mode == Display_Interface_Mode_Save_Interface))   //处于存取模式
		{
			Recall_Save_Number ++;
			if(Recall_Save_Number == 10)
			{
				Recall_Save_Number =0;
			}
			Recall_Save_Display_Voltage = Recall_Save_Voltage[Recall_Save_Number];
			Recall_Save_Display_Current = Recall_Save_Current[Recall_Save_Number];
			Recall_Save_Display_Power = Recall_Save_Power[Recall_Save_Number];
		}
		
		else if(Display_Interface_Mode == Display_Interface_Mode_First_Menu)   //处于第一个选择界面，让显示内容对应改变
		{
			First_Selection = First_Selection + 1;
			if(First_Selection == 6)
			{
				First_Selection = 0;
			}
		}
		
		else if(Display_Interface_Mode == Display_Interface_Mode_Second_Menu)   //处于第二级菜单界面，让显示内容对应改变
		{
			switch(Second_Selection)
			{
				case Delay: Second_Selection = Cycle;break;
				case Cycle: Second_Selection = Delay;break;
				case V_Rise: Second_Selection = V_Fall;break;
				case V_Fall: Second_Selection = I_Rise;break;
				case I_Rise: Second_Selection = I_Fall;break;
				case I_Fall: Second_Selection = V_Rise;break;
	      case RS232: Second_Selection = CAN;break;
				case CAN: Second_Selection = RS232;break;
				case Menu_Vint: Second_Selection = Menu_Vext;break;
				case Menu_Vext: Second_Selection = Menu_Vint;break;
				default: break;
			}
		}
		
/*--------------------------------------------------------------------------------------RS232---------------------------------------------------------*/		
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == RS232) && (RS232_Set_ID_BaudRate == RS232_Set_ID) && (Keys_Encoder_Mode == Encoder_Mode))    //设置RS232的ID
		{
			Write_Single_16x32AsicII(0, 12 + Cursor_Position*4, String_RS232_ID[Cursor_Position]);
			
			Cursor_Position --;
			
			if(Cursor_Position == -1)
			{
			  Cursor_Position = 1;     //光标超出范围				
			}
		}
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == RS232) && (RS232_Set_ID_BaudRate == RS232_Set_BaudRate))    //设置RS232的波特率
		{
			
			RS232_BaudRate_State ++;
			
			switch (RS232_BaudRate_State)                                                                    //显示设定值
			{
				  case (Usart300 - 1) : Write_String_16x32AsicII(32, 40, "115200");SysTick_Delay_Ms(10);RS232_BaudRate_State = Usart115200;break;
					case Usart300 : Write_String_16x32AsicII(32, 40, "000300");SysTick_Delay_Ms(10);break;
          case Usart600 : Write_String_16x32AsicII(32, 40, "000600");SysTick_Delay_Ms(10);break;
          case Usart1200 : Write_String_16x32AsicII(32, 40, "001200");SysTick_Delay_Ms(10);break;
          case Usart2400 : Write_String_16x32AsicII(32, 40, "002400");SysTick_Delay_Ms(10);break;
          case Usart4800 : Write_String_16x32AsicII(32, 40, "004800");SysTick_Delay_Ms(10);break;
          case Usart9600 : Write_String_16x32AsicII(32, 40, "009600");SysTick_Delay_Ms(10);break;
          case Usart19200 : Write_String_16x32AsicII(32, 40, "019200");SysTick_Delay_Ms(10);break;
          case Usart38400 : Write_String_16x32AsicII(32, 40, "038400");SysTick_Delay_Ms(10);break;
          case Usart43000 : Write_String_16x32AsicII(32, 40, "043000");SysTick_Delay_Ms(10);break;
          case Usart56000 : Write_String_16x32AsicII(32, 40, "056000");SysTick_Delay_Ms(10);break;
          case Usart57600 : Write_String_16x32AsicII(32, 40, "057600");SysTick_Delay_Ms(10);break;
          case Usart115200 : Write_String_16x32AsicII(32, 40, "115200");SysTick_Delay_Ms(10);break;
				  case (Usart115200 + 1) : Write_String_16x32AsicII(32, 40, "000300");SysTick_Delay_Ms(10);RS232_BaudRate_State = Usart300;break;
					default: break;
			}
		}
		
/*--------------------------------------------------------------------------------CAN--------------------------------------------------------------------*/				
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == CAN) && (CAN_Set_ID_BaudRate == CAN_Set_ID) && (Keys_Encoder_Mode == Encoder_Mode))    //设置CAN的ID
		{
			Write_Single_16x32AsicII(0, 12 + Cursor_Position*4, String_CAN_ID[Cursor_Position]);
			
			Cursor_Position --;
			
			if(Cursor_Position == -1)
			{
			  Cursor_Position = 1;     //光标超出范围				
			}
		}
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == CAN) && (CAN_Set_ID_BaudRate == CAN_Set_BaudRate))    //设置CAN的波特率
		{
			
			CAN_BaudRate_State ++;
			
			switch (CAN_BaudRate_State)                                                                    //显示设定值
			{
				  case (CAN5 - 1) : Write_String_16x32AsicII(32, 40, "800");SysTick_Delay_Ms(10);CAN_BaudRate_State = CAN800;break;
					case CAN5 : Write_String_16x32AsicII(32, 40, "005");SysTick_Delay_Ms(10);break;				
					case CAN10 : Write_String_16x32AsicII(32, 40, "010");SysTick_Delay_Ms(10);break;
          case CAN20 : Write_String_16x32AsicII(32, 40, "020");SysTick_Delay_Ms(10);break;
          case CAN25 : Write_String_16x32AsicII(32, 40, "025");SysTick_Delay_Ms(10);break;
          case CAN40 : Write_String_16x32AsicII(32, 40, "040");SysTick_Delay_Ms(10);break;
          case CAN50 : Write_String_16x32AsicII(32, 40, "050");SysTick_Delay_Ms(10);break;
          case CAN80 : Write_String_16x32AsicII(32, 40, "080");SysTick_Delay_Ms(10);break;
          case CAN100 : Write_String_16x32AsicII(32, 40, "100");SysTick_Delay_Ms(10);break;
          case CAN125 : Write_String_16x32AsicII(32, 40, "125");SysTick_Delay_Ms(10);break;
          case CAN200 : Write_String_16x32AsicII(32, 40, "200");SysTick_Delay_Ms(10);break;
          case CAN250 : Write_String_16x32AsicII(32, 40, "250");SysTick_Delay_Ms(10);break;
          case CAN400 : Write_String_16x32AsicII(32, 40, "400");SysTick_Delay_Ms(10);break;
          case CAN500 : Write_String_16x32AsicII(32, 40, "500");SysTick_Delay_Ms(10);break;
          case CAN800 : Write_String_16x32AsicII(32, 40, "800");SysTick_Delay_Ms(10);break;				
				  case (CAN800 + 1) : Write_String_16x32AsicII(32, 40, "005");SysTick_Delay_Ms(10);CAN_BaudRate_State = CAN5;break;
					default: break;
			}
		}
		
/*-----------------------------------------------------------------------------Cycle------------------------------------------------------------*/		
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Cycle))  //当处于第二级菜单的Cycle
		{
			if ((Cycle_Enter == Cycle_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //处于设置开启秒
			{
			  Write_Single_16x32AsicII(32, 12 + Cursor_Position*4, String_Cycle_On[Cursor_Position]);
			
		  	Cursor_Position --;
			
				if(Cursor_Position == 4)
				{
					Cursor_Position = 3;
				}
				else if(Cursor_Position == -1)
				{
					Cursor_Position = 7;
				}
			}
			
			else if ((Cycle_Enter == Cycle_Enter_2) && (Keys_Encoder_Mode == Encoder_Mode))       //处于设置开启秒
			{
			  Write_Single_16x32AsicII(32, 12 + Cursor_Position*4, String_Cycle_Close[Cursor_Position]);
			
		  	Cursor_Position --;
			
				if(Cursor_Position == 4)
				{
					Cursor_Position = 3;
				}
				else if(Cursor_Position == -1)
				{
					Cursor_Position = 7;
				}
			}
		}

/*---------------------------------------------------------------V-Rise-----------------------------------------------------------*/
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == V_Rise))  //当处于第二级菜单的V_Rise
		{
			if ((V_Rise_Enter == V_Rise_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //处于设置开启秒
			{
			  Write_Single_16x32AsicII(32, 12 + Cursor_Position*4, String_V_Rise[Cursor_Position]);
			
		  	Cursor_Position --;
			
				if(Cursor_Position == 4)
				{
					Cursor_Position = 3;
				}
				else if(Cursor_Position == -1)
				{
					Cursor_Position = 7;
				}
			}
		}
		
/*---------------------------------------------------------------V-Fall-----------------------------------------------------------*/
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == V_Fall))  //当处于第二级菜单的V_Fall
		{
			if ((V_Fall_Enter == V_Fall_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //处于设置开启秒
			{
			  Write_Single_16x32AsicII(32, 12 + Cursor_Position*4, String_V_Fall[Cursor_Position]);
			
		  	Cursor_Position --;
			
				if(Cursor_Position == 4)
				{
					Cursor_Position = 3;
				}
				else if(Cursor_Position == -1)
				{
					Cursor_Position = 7;
				}
			}
		}		
	
/*---------------------------------------------------------------I-Rise-----------------------------------------------------------*/
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == I_Rise))  //当处于第二级菜单的I_Rise
		{
			if ((I_Rise_Enter == I_Rise_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //处于设置开启秒
			{
			  Write_Single_16x32AsicII(32, 12 + Cursor_Position*4, String_I_Rise[Cursor_Position]);
			
		  	Cursor_Position --;
			
				if(Cursor_Position == 4)
				{
					Cursor_Position = 3;
				}
				else if(Cursor_Position == -1)
				{
					Cursor_Position = 7;
				}
			}
		}
		
/*---------------------------------------------------------------I-Fall-----------------------------------------------------------*/
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == I_Fall))  //当处于第二级菜单的I_Fall
		{
			if ((I_Fall_Enter == I_Fall_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //处于设置开启秒
			{
			  Write_Single_16x32AsicII(32, 12 + Cursor_Position*4, String_I_Fall[Cursor_Position]);
			
		  	Cursor_Position --;
			
				if(Cursor_Position == 4)
				{
					Cursor_Position = 3;
				}
				else if(Cursor_Position == -1)
				{
					Cursor_Position = 7;
				}
			}
		}		
		
/*--------------------------------------------------------Delay----------------------------------------------------*/
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Delay))  //当处于第二级菜单的Delay
		{			
			if ((Delay_Enter == Delay_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))     //如果按了一次按键并处于编码器模式 
			{
			  Write_Single_16x32AsicII(32, (14 + Cursor_Position * 4), String_Delay[Cursor_Position]);
			
			  Cursor_Position --;
			
			  if(Cursor_Position == 5)
		  	{
		  	  Cursor_Position = 4;     //光标超出范围				
		  	}				
				else if (Cursor_Position == 2)
				{
					Cursor_Position = 1;
				}
				else if (Cursor_Position == -1)
				{
					Cursor_Position = 7;
				}
			}			
		}
		
		
		
	}
}

 /**
  * @brief  按键Down的扫描服务函数
  * @param	None
  * @retval None
  */
void Key_Down(void)
{
  if(LOCK_UNLOCK == UNLOCK)                     //确认按键已经按下和处于无锁定状态
	{
		if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_V_set) && (Keys_Encoder_Mode == Encoder_Mode))  //处于电压设置界面和编码器模式，按一下，让指针向后移动1
		{
			Write_Single_16x32AsicII(0, Cursor_Position*4, String_Voltage[Cursor_Position]);
			
			Write_Single_16x32AsicII(0, 20, 'V');			
			
			Cursor_Position ++;

			if(Cursor_Position == 5)          //光标超出范围
			{
				Cursor_Position = 0;
			}
			else if(String_Voltage[Cursor_Position] == '.')     //光标跳过小数点
			{
				Cursor_Position ++;
			}
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_I_set) && (Keys_Encoder_Mode == Encoder_Mode))  //处于电流设置界面和编码器模式，按一下，让指针向后移动1
		{
			Write_Single_16x32AsicII(0, Cursor_Position*4 + 36, String_Current[Cursor_Position]);
			
			Write_Single_16x32AsicII(0, 60, 'A');			
			
			Cursor_Position ++;
			if(Cursor_Position == 6)    //光标超出范围
			{
				Cursor_Position = 0;
			}
			else if(String_Current[Cursor_Position] == '.')     //光标跳过小数点
			{
				Cursor_Position ++;
			}		
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_P_set) && (Keys_Encoder_Mode == Encoder_Mode))   //处于功率设置界面和编码器模式，按一下，让指针向后移动1
		{
			Write_Single_12x24AsicII(42, Cursor_Position*3, String_Power[Cursor_Position]);
			
			Write_Single_12x24AsicII(42, 15, 'W');		
			
			Cursor_Position ++;  //光标移位

			if(Cursor_Position == 5)
			{
			  Cursor_Position = 0;     //光标超出范围				
			}
			
			if((String_To_Float(String_Power) >= 1000) && (Cursor_Position == 0))
			{
				Cursor_Position ++;
			}
			
			if(String_Power[Cursor_Position] == '.')     //光标跳过小数点
			{
				Cursor_Position ++;
			}
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Recall_Interface) || (Display_Interface_Mode == Display_Interface_Mode_Save_Interface))   //处于存取模式
		{
			Recall_Save_Number --;
			if(Recall_Save_Number == -1)
			{
				Recall_Save_Number =9;
			}
			Recall_Save_Display_Voltage = Recall_Save_Voltage[Recall_Save_Number];
			Recall_Save_Display_Current = Recall_Save_Current[Recall_Save_Number];
			Recall_Save_Display_Power = Recall_Save_Power[Recall_Save_Number];
		}
		
		else if(Display_Interface_Mode == Display_Interface_Mode_First_Menu)   //处于第一个选择界面，让显示内容对应改变
		{
			First_Selection = First_Selection - 1;
			if(First_Selection == -1)
			{
				First_Selection = 5;
			}
		}
		
		else if(Display_Interface_Mode == Display_Interface_Mode_Second_Menu)   //处于第二级菜单界面，让显示内容对应改变
		{
			switch(Second_Selection)
			{
				case Delay: Second_Selection = Cycle;break;
				case Cycle: Second_Selection = Delay;break;
				case I_Fall: Second_Selection = I_Rise;break;
				case I_Rise: Second_Selection = V_Fall;break;
				case V_Fall: Second_Selection = V_Rise;break;
				case V_Rise: Second_Selection = I_Fall;break;
	      case RS232: Second_Selection = CAN;break;
				case CAN: Second_Selection = RS232;break;
				case Menu_Vint: Second_Selection = Menu_Vext; break;
				case Menu_Vext: Second_Selection = Menu_Vint; break;
				default: break;
			}
		}
		
/*------------------------------------------------RS232---------------------------------------------------*/		
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == RS232) && (RS232_Set_ID_BaudRate == RS232_Set_ID) && (Keys_Encoder_Mode == Encoder_Mode))    //设置RS232的ID
		{
			Write_Single_16x32AsicII(0, 12 + Cursor_Position*4, String_RS232_ID[Cursor_Position]);
			
			Cursor_Position ++;
			
			if(Cursor_Position == 2)
			{
			  Cursor_Position = 0;     //光标超出范围				
			}
		}

    else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == RS232) && (RS232_Set_ID_BaudRate == RS232_Set_BaudRate))    //设置RS232的波特率
		{
			
			RS232_BaudRate_State --;
			
			switch (RS232_BaudRate_State)                                                                   //显示设定值
			{
				  case (Usart300 - 1) : Write_String_16x32AsicII(32, 40, "115200");SysTick_Delay_Ms(10);RS232_BaudRate_State = Usart115200;break;
					case Usart300 : Write_String_16x32AsicII(32, 40, "000300");SysTick_Delay_Ms(10);break;
          case Usart600 : Write_String_16x32AsicII(32, 40, "000600");SysTick_Delay_Ms(10);break;
          case Usart1200 : Write_String_16x32AsicII(32, 40, "001200");SysTick_Delay_Ms(10);break;
          case Usart2400 : Write_String_16x32AsicII(32, 40, "002400");SysTick_Delay_Ms(10);break;
          case Usart4800 : Write_String_16x32AsicII(32, 40, "004800");SysTick_Delay_Ms(10);break;
          case Usart9600 : Write_String_16x32AsicII(32, 40, "009600");SysTick_Delay_Ms(10);break;
          case Usart19200 : Write_String_16x32AsicII(32, 40, "019200");SysTick_Delay_Ms(10);break;
          case Usart38400 : Write_String_16x32AsicII(32, 40, "038400");SysTick_Delay_Ms(10);break;
          case Usart43000 : Write_String_16x32AsicII(32, 40, "043000");SysTick_Delay_Ms(10);break;
          case Usart56000 : Write_String_16x32AsicII(32, 40, "056000");SysTick_Delay_Ms(10);break;
          case Usart57600 : Write_String_16x32AsicII(32, 40, "057600");SysTick_Delay_Ms(10);break;
          case Usart115200 : Write_String_16x32AsicII(32, 40, "115200");SysTick_Delay_Ms(10);break;
			  	case (Usart115200 + 1) : Write_String_16x32AsicII(32, 40, "000300");SysTick_Delay_Ms(10);RS232_BaudRate_State = Usart300;break;
					default: break;
			}
		}	

/*-----------------------------------------------------------CAN--------------------------------------------*/
		
    else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == CAN) && (CAN_Set_ID_BaudRate == CAN_Set_ID) && (Keys_Encoder_Mode == Encoder_Mode))    //设置CAN的ID
		{
			Write_Single_16x32AsicII(0, 12 + Cursor_Position*4, String_CAN_ID[Cursor_Position]);
			
			Cursor_Position ++;
			
			if(Cursor_Position == 2)
			{
			  Cursor_Position = 0;     //光标超出范围				
			}
		}

    else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == CAN) && (CAN_Set_ID_BaudRate == CAN_Set_BaudRate))    //设置CAN的波特率
		{
			
			CAN_BaudRate_State --;
			
			switch (CAN_BaudRate_State)                                                                   //显示设定值
			{
				  case (CAN5 - 1) : Write_String_16x32AsicII(32, 40, "800");SysTick_Delay_Ms(10);CAN_BaudRate_State = CAN800;break;
					case CAN5 : Write_String_16x32AsicII(32, 40, "005");SysTick_Delay_Ms(10);break;				
					case CAN10 : Write_String_16x32AsicII(32, 40, "010");SysTick_Delay_Ms(10);break;
          case CAN20 : Write_String_16x32AsicII(32, 40, "020");SysTick_Delay_Ms(10);break;
          case CAN25 : Write_String_16x32AsicII(32, 40, "025");SysTick_Delay_Ms(10);break;
          case CAN40 : Write_String_16x32AsicII(32, 40, "040");SysTick_Delay_Ms(10);break;
          case CAN50 : Write_String_16x32AsicII(32, 40, "050");SysTick_Delay_Ms(10);break;
          case CAN80 : Write_String_16x32AsicII(32, 40, "080");SysTick_Delay_Ms(10);break;
          case CAN100 : Write_String_16x32AsicII(32, 40, "100");SysTick_Delay_Ms(10);break;
          case CAN125 : Write_String_16x32AsicII(32, 40, "125");SysTick_Delay_Ms(10);break;
          case CAN200 : Write_String_16x32AsicII(32, 40, "200");SysTick_Delay_Ms(10);break;
          case CAN250 : Write_String_16x32AsicII(32, 40, "250");SysTick_Delay_Ms(10);break;
          case CAN400 : Write_String_16x32AsicII(32, 40, "400");SysTick_Delay_Ms(10);break;
          case CAN500 : Write_String_16x32AsicII(32, 40, "500");SysTick_Delay_Ms(10);break;
          case CAN800 : Write_String_16x32AsicII(32, 40, "800");SysTick_Delay_Ms(10);break;				
				  case (CAN800 + 1) : Write_String_16x32AsicII(32, 40, "005");SysTick_Delay_Ms(10);CAN_BaudRate_State = CAN5;break;
					default: break;
			}
		}
/*---------------------------------------------------------------V-Rise-----------------------------------------------------------*/
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == V_Rise))  //当处于第二级菜单的V_Rise
		{
			if ((V_Rise_Enter == V_Rise_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //处于设置开启秒
			{
			  Write_Single_16x32AsicII(32, 12 + Cursor_Position*4, String_V_Rise[Cursor_Position]);
			
		  	Cursor_Position ++;
			
				if(Cursor_Position == 4)
				{
					Cursor_Position = 5;
				}
				else if(Cursor_Position == 8)
				{
					Cursor_Position = 0;
				}		
			}
		}
		
/*---------------------------------------------------------------V-Fall-----------------------------------------------------------*/
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == V_Fall))  //当处于第二级菜单的V_Fall
		{
			if ((V_Fall_Enter == V_Fall_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //处于设置开启秒
			{
			  Write_Single_16x32AsicII(32, 12 + Cursor_Position*4, String_V_Fall[Cursor_Position]);
			
		  	Cursor_Position ++;
			
				if(Cursor_Position == 4)
				{
					Cursor_Position = 5;
				}
				else if(Cursor_Position == 8)
				{
					Cursor_Position = 0;
				}		
			}
		}

/*---------------------------------------------------------------I-Rise-----------------------------------------------------------*/
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == I_Rise))  //当处于第二级菜单的I_Rise
		{
			if ((I_Rise_Enter == I_Rise_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //处于设置开启秒
			{
			  Write_Single_16x32AsicII(32, 12 + Cursor_Position*4, String_I_Rise[Cursor_Position]);
			
		  	Cursor_Position ++;
			
				if(Cursor_Position == 4)
				{
					Cursor_Position = 5;
				}
				else if(Cursor_Position == 8)
				{
					Cursor_Position = 0;
				}		
			}
		}

/*---------------------------------------------------------------I-Fall-----------------------------------------------------------*/
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == I_Fall))  //当处于第二级菜单的I_Fall
		{
			if ((I_Fall_Enter == I_Fall_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //处于设置开启秒
			{
			  Write_Single_16x32AsicII(32, 12 + Cursor_Position*4, String_I_Fall[Cursor_Position]);
			
		  	Cursor_Position ++;
			
				if(Cursor_Position == 4)
				{
					Cursor_Position = 5;
				}
				else if(Cursor_Position == 8)
				{
					Cursor_Position = 0;
				}		
			}
		}		
		
/*-----------------------------------------------------------------------------Cycle------------------------------------------------------------*/		
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Cycle))  //当处于第二级菜单的Cycle
		{
			if ((Cycle_Enter == Cycle_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //处于设置开启秒
			{
			  Write_Single_16x32AsicII(32, 12 + Cursor_Position*4, String_Cycle_On[Cursor_Position]);
			
		  	Cursor_Position ++;
			
				if(Cursor_Position == 4)
				{
					Cursor_Position = 5;
				}
				else if(Cursor_Position == 8)
				{
					Cursor_Position = 0;
				}
			}
			
			
			if ((Cycle_Enter == Cycle_Enter_2) && (Keys_Encoder_Mode == Encoder_Mode))       //处于设置开启秒
			{
			  Write_Single_16x32AsicII(32, 12 + Cursor_Position*4, String_Cycle_Close[Cursor_Position]);
			
		  	Cursor_Position ++;
			
				if(Cursor_Position == 4)
				{
					Cursor_Position = 5;
				}
				else if(Cursor_Position == 8)
				{
					Cursor_Position = 0;
				}
			}
		}		
/*--------------------------------------------------------Delay----------------------------------------------------*/
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Delay))  //当处于第二级菜单的Delay
		{			
			if ((Delay_Enter == Delay_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))      
			{
			  Write_Single_16x32AsicII(32, (14 + Cursor_Position * 4), String_Delay[Cursor_Position]);
			
				//改光标位置
			  Cursor_Position ++;
			
			  if(Cursor_Position == 2)
		  	{
		  	  Cursor_Position = 3;     //光标超出范围				
		  	}					
				else if(Cursor_Position == 5)
				{
					Cursor_Position = 6;
				}
				else if(Cursor_Position == 8)
				{
					Cursor_Position = 0;
				}
			}
		}

		
	}
}

 /**
  * @brief  按键ON/OFF的扫描服务函数
  * @param	None
  * @retval None
  */
void Key_ON_OFF(void)
{
	if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Actual) || (Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Preset))    //确认按键已经按下,且在真实值或设定值界面
	{
		if(OUTPUT_NOT_OUTPUT == NOT_OUTPUT)
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
		else if(OUTPUT_NOT_OUTPUT == OUTPUT)
		{
			OUTPUT_NOT_OUTPUT = NOT_OUTPUT;
			TIM_DELAY_OFF();
			TIM_CYCLE_OFF();
			Power_Off();
		}
	}
}

 /**
  * @brief  按键Preset/Lock的扫描服务函数
  * @param	None
  * @retval None
  */
void Key_Preset_Lock(void)
{
	if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Actual) 
	   || (Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Preset))                     //确认按键已经按下、并处于主界面
	{
		if(Shift_Not_Shift == Not_Shift)                     //无复用，为预设值真实值设定按钮
		{
			if(Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Preset)                //之前是设定值
			{
				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;   //显示真实值
			}
			else if(Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Actual)          //之前是真实值
			{
				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Preset;                 //显示设定值
			}
			else
			{
				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Preset;
			}
		}
		
		else if(Shift_Not_Shift == Shift)           //复用，为锁定按钮
		{
			if(LOCK_UNLOCK == UNLOCK)         //如果没锁定，就锁定,并返回主界面
			{
				LOCK_UNLOCK = LOCK;
				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;
			}
			else if(LOCK_UNLOCK == LOCK)      //如果锁定了，就解锁
			{
				LOCK_UNLOCK = UNLOCK;
			}
			Shift_Not_Shift = Not_Shift;
		}
	}
}

 /**
  * @brief  按键Recall/Save的扫描服务函数
  * @param	None
  * @retval None
  */
void Key_Recall_Save(void)
{
	if((LOCK_UNLOCK == UNLOCK) && ((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Actual) 
	   || (Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Preset)))                     //确认按键已经按下、处于无锁定状态、处于主界面
	{
		if(Shift_Not_Shift == Not_Shift)          //无复用，处于取界面
		{
			Recall_Save_Number = 0;
			Recall_Save_Display_Voltage = Recall_Save_Voltage[Recall_Save_Number];
			Recall_Save_Display_Current = Recall_Save_Current[Recall_Save_Number];
			Recall_Save_Display_Power = Recall_Save_Power[Recall_Save_Number];
			Display_Interface_Mode = Display_Interface_Mode_Recall_Interface;			
		}
		
		else if(Shift_Not_Shift == Shift)         //复用，处于存界面
		{
			Shift_Not_Shift = Not_Shift;              //取消复用
			Recall_Save_Number = 0;
			Recall_Save_Display_Voltage = Recall_Save_Voltage[Recall_Save_Number];
			Recall_Save_Display_Current = Recall_Save_Current[Recall_Save_Number];
			Recall_Save_Display_Power = Recall_Save_Power[Recall_Save_Number];
			Display_Interface_Mode = Display_Interface_Mode_Save_Interface;				
		}
	}		
}

//////u16 text;
//////u8 Key_Flag;    //防止连点的变量
////// /**
//////  * @brief  按键处理函数（先判断行列，再调用按键函数）
//////  * @param	None
//////  * @retval None
//////  */
//////void Keys_Handler(void)
//////{
//////	SysTick_Delay_Ms(10);    //延时让寄存器有足够的时间反应过来
//////	
//////	if((KeyCol_Status == 0x7f) && (Key_Flag == 1))
//////	{
//////		Key_Flag = 0;
//////	}

//////	if((KeyCol_Status != 0x7f) && (Key_Flag == 0))    //检测到有按键按下了
//////	{
//////		SysTick_Delay_Ms(15);    //延时确定真的按下了
//////		if(KeyCol_Status != 0x7F)
//////		{
//////			Key_Flag = 1;
//////			switch(KeyCol_Status)
//////			{
//////				case 0x3f:           //第一列按下了
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_1_Pin);     //检查第一行
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_1_Pin) == 1)    //1行-1列按键按下了
//////					{
//////					  Key_1();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////					
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_2_Pin);     //检查第二行
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_1_Pin) == 1)    //2行-1列按键按下了
//////					{
//////					  Key_4();					
//////            GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);						
//////						return;	
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////				
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_3_Pin);     //检查第三行
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_1_Pin) == 1)    //3行-1列按键按下了
//////					{
//////						Key_7();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////					break;
//////					
//////				case 0x5f:           //第二列按下了
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_1_Pin);     //检查第一行
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_2_Pin) == 1)    //1行-2列按键按下了
//////					{
//////						Key_2();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////					
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_2_Pin);     //检查第二行
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_2_Pin) == 1)    //2行-2列按键按下了
//////					{
//////						Key_5();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////						return;	
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////				
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_3_Pin);     //检查第三行
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_3_Pin) == 1)    //3行-3列按键按下了
//////					{
//////						Key_8();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);	
//////					break;

//////				case 0x6f:           //第三列按下了
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_1_Pin);     //检查第一行
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_3_Pin) == 1)    //1行-3列按键按下了
//////					{
//////						Key_3();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////					
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_2_Pin);     //检查第二行
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_3_Pin) == 1)    //2行-3列按键按下了
//////					{
//////						Key_6();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////						return;	
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////				
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_3_Pin);     //检查第三行
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_3_Pin) == 1)    //3行-3列按键按下了
//////					{
//////						Key_9();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////					break;

//////				case 0x77:           //第四列按下了
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_1_Pin);     //检查第一行
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_4_Pin) == 1)    //1行-4列按键按下了
//////					{
//////						Key_ESC();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////					
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_2_Pin);     //检查第二行
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_4_Pin) == 1)    //2行-4列按键按下了
//////					{
//////						Key_0();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////						return;	
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////				
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_3_Pin);     //检查第三行
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_4_Pin) == 1)    //3行-4列按键按下了
//////					{
//////						Key_Decimal_Point();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////					break;

//////				case 0x7b:           //第五列按下了
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_1_Pin);     //检查第一行
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_5_Pin) == 1)    //1行-5列按键按下了
//////					{
//////						Key_V_set();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////					
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_2_Pin);     //检查第二行
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_5_Pin) == 1)    //2行-5列按键按下了
//////					{
//////						Key_Recall_Save();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////						return;	
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////				
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_3_Pin);     //检查第三行
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_5_Pin) == 1)    //3行-5列按键按下了
//////					{
//////						Key_Shift();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////					break;
//////					
//////				case 0x7d:           //第六列按下了
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_1_Pin);     //检查第一行
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_6_Pin) == 1)    //1行-6列按键按下了
//////					{
//////						Key_I_set();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////					
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_2_Pin);     //检查第二行
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_6_Pin) == 1)    //2行-6列按键按下了
//////					{
//////						Key_Preset_Lock();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////						return;	
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////				
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_3_Pin);     //检查第三行
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_6_Pin) == 1)    //3行-6列按键按下了
//////					{
//////						Key_ON_OFF();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////          break;					
//////					
//////				case 0x7e:           //第七列按下了
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_1_Pin);     //检查第一行
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_7_Pin) == 1)    //1行-7列按键按下了
//////					{
//////						Key_Up();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////					
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_2_Pin);     //检查第二行
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_7_Pin) == 1)    //2行-7列按键按下了
//////					{
//////						Key_Enter();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////						return;	
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////				
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_3_Pin);     //检查第三行
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_7_Pin) == 1)    //3行-7列按键按下了
//////					{
//////						Key_Down();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////          break;					
//////									
//////				default :break;	
//////			}
//////		}
//////	}
//////}



u16 text;
u8 Key_Flag;    //防止连点的变量
 /**
  * @brief  按键处理函数（先判断行列，再调用按键函数）
  * @param	None
  * @retval None
  */
void Keys_Handler(FunctionalState keys_EN)
{
	if(keys_EN != ENABLE) return;
	GPIO_SetBits(KeyRow_Port, KeyRow_1_Pin);    //让三行高
	GPIO_SetBits(KeyRow_Port, KeyRow_2_Pin);
	GPIO_SetBits(KeyRow_Port, KeyRow_3_Pin);	
	
	
	GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);     //第一行
	SysTick_Delay_Ms(5);
	
	if(KeyCol_Status == 0x7f && Key_Flag == 1)    
	{
		Key_Flag = 0;                //按键标志位复位             
	}
	
	if(Key_Flag == 0)     //只有标志位为0才能处理
	{
		switch(KeyCol_Status)
	  {         
		  case 0x3f:                        //第一列按下了 “1”
	    {
		    Key_1();
			  Key_Flag = 1;
			  break;
		  }
		  case 0x5f:                   //第二列按下了“2”
		  {
			  Key_2();
			  Key_Flag = 1;
			  break;			
		  }
		  case 0x6f:                 //第三列按下了“3”
		  {
			  Key_3();
			  Key_Flag = 1;
			  break;				
		  }
		  case 0x77:                 //第四列按下了“ESC”
		  {
			  Key_ESC();
			  Key_Flag = 1;
			  break;				
		  }
		  case 0x7b:            //第五列按下了“V-set”
		  {
			  Key_V_set();
			  Key_Flag = 1;
			  break;				
		  }
		  case 0x7d:              //第六列按下了“I-set”
		  {
			  Key_I_set();
			  Key_Flag = 1;
			  break;			
		  }
		  case 0x7e:               //第七列按下了“UP”
		  {
			  Key_Up();
			  Key_Flag = 1;
			  break;			
		  }
		  default: 			
        break;                 //没有按下
	  }
	}
	GPIO_SetBits(KeyRow_Port, KeyRow_1_Pin);	

	
	
	
	GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);     //第二行
	SysTick_Delay_Ms(5);
	
	if(KeyCol_Status == 0x7f && Key_Flag == 2)    
	{
		Key_Flag = 0;                //按键标志位复位             
	}	
	if(Key_Flag == 0)
	{
		switch(KeyCol_Status)
	  {              	
		  case 0x3f:                        //第一列按下了 “4”
	    {
		    Key_4();
			  Key_Flag = 2;
			  break;
		  }
		  case 0x5f:                   //第二列按下了“5”
		  {
			  Key_5();
		  	Key_Flag = 2;
			  break;			
		  }
		  case 0x6f:                 //第三列按下了“6”
		  {
			  Key_6();
			  Key_Flag = 2;
			  break;				
		  }
		  case 0x77:                 //第四列按下了“0”
		  {
			  Key_0();
			  Key_Flag = 2;
			  break;				
		  }
		  case 0x7b:            //第五列按下了“RECALL”
		  {
			  Key_Recall_Save();
			  Key_Flag = 2;
			  break;				
		  }
		  case 0x7d:              //第六列按下了“Preset”
		  {
			  Key_Preset_Lock();
			  Key_Flag = 2;
			  break;			
		  }
		  case 0x7e:               //第七列按下了“ENTER”
		  {
			  Key_Enter();
			  Key_Flag = 2;
			  break;			
		  }
		  default: 
        break;                 //没有按下			
	  }
	}
	GPIO_SetBits(KeyRow_Port, KeyRow_2_Pin);	

	
	
	
	GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);     //第三行
	SysTick_Delay_Ms(5);
	
	if(KeyCol_Status == 0x7f && Key_Flag == 3)    
	{
		Key_Flag = 0;                //按键标志位复位             
	}	
	if(Key_Flag == 0)
	{
		switch(KeyCol_Status)
	  {
		  case 0x3f:                        //第一列按下了 “7”
	    {
		    Key_7();
				Key_Flag = 3;
			  break;
		  }
		  case 0x5f:                   //第二列按下了“8”
		  {
			  Key_8();
				Key_Flag = 3;
			  break;			
		  }
		  case 0x6f:                 //第三列按下了“9”
		  {
			  Key_9();
				Key_Flag = 3;
			  break;				
		  }
		  case 0x77:                 //第四列按下了“.”
		  {
			  Key_Decimal_Point();
				Key_Flag = 3;
			  break;				
		  }
		  case 0x7b:            //第五列按下了“shift”
		  {
			  Key_Shift();
				Key_Flag = 3;
			  break;				
		  }
		  case 0x7d:              //第六列按下了“on”
		  {
			  Key_ON_OFF();
				Key_Flag = 3;
			  break;			
		  }
		  case 0x7e:               //第七列按下了“down”
		  {
			  Key_Down();
				Key_Flag = 3;
			  break;			
		  }
		  default:
        break;                 //没有按下			
	  }	
	}	
  GPIO_SetBits(KeyRow_Port, KeyRow_3_Pin);
}





