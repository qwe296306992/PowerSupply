#include "keys.h"

u8 Shift_Not_Shift = Not_Shift;                                             //���帴�ü��ı�־λ��Ĭ�ϲ����ã�
u8 ON_OFF = OFF;                                                            //��������رձ�־λ��Ĭ�ϲ����,ֻ��������ʾ��
u8 OUTPUT_NOT_OUTPUT = NOT_OUTPUT;                                          //�����Ƿ��Ѿ������־λ
u8 CV_CC_CP = CV;                                                           //����������ͱ�־λ (Ĭ�Ϻ�ѹ���)
u8 DELAY_NOT_DELAY = NOT_DELAY;                                             //������ʱ��־λ��Ĭ�ϲ���ʱ��
u8 LOCK_UNLOCK = UNLOCK;                                                    //����������־λ��Ĭ�ϲ������� 
u8 Recall_Save = Recall;                                                    //�����ȡ�����־λ��Ĭ��Ϊ��ȡ��
int Recall_Save_Number = 0;                                                 //�����ȡ����������飨Ĭ�ϵ�0�飩
int First_Selection = 0;                                                    //�����һ�˵���ѡ��
u8 Second_Selection = 0;                                                    //����ڶ��˵���ѡ��
u8 Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;   //������ʾ��������ͣ�Ĭ����ʾ�����棬��ʵֵ��
int Cursor_Position;                                                        //������λ�õ�ȫ�ֱ���
u8 Keys_Encoder_Mode = Encoder_Mode;                                        //�����ü������뻹���ñ���������(Ĭ���ñ���������)
u8 RS232_BaudRate_State = 0;                                                //�����趨������״̬
u8 CAN_BaudRate_State = 0;                                                  //�����趨������״̬


u8 Cycle_Enter = Cycle_Enter_0;                                          //����ѭ��ʱ���趨�뻹�Ǻ���
u16 Cycle_On_s = 0;                                                         //����ѭ��ʱ�����
u16 Cycle_On_ms = 0;                                                        //����ѭ��ʱ��ĺ���
u16 Cycle_Close_s = 0;                                                      //����ѭ���ر�ʱ�����
u16 Cycle_Close_ms = 0;                                                     //����ѭ���ر�ʱ��ĺ���
char String_Cycle_On[11];                                                   //����ѭ�������ַ���
char String_Cycle_Close[11];                                                //����ѭ���ص��ַ���

u8 Delay_Enter = Delay_Enter_0;                                              //��¼���˶��ٴ�ENTER
u8 Delay_h;                                                                  //������ʱ��ʱ
u8 Delay_m;                                                                  //������ʱ�ķ�
u8 Delay_s;                                                                  //������ʱ����
char String_Delay[10];                                                        //������ʱ���ַ���


u8 V_Rise_Enter = V_Rise_Enter_0;                                             //����V-Rise���˶��ٴΰ���
u16 V_Rise_s = 0;                                                            //����V-Rise����
u16 V_Rise_ms = 0;                                                           //����V-Rise�ĺ���
char String_V_Rise[11];                                                     //����V-Rise���ַ���


u8 V_Fall_Enter = V_Fall_Enter_0;                                            //����V_Fall���õ��������
u16 V_Fall_s = 0;                                                            //����V_Fall����
u16 V_Fall_ms = 0;                                                           //����V_Fall�ĺ���
char String_V_Fall[11];                                                     //����V_Fall���ַ���



u8 I_Rise_Enter = I_Rise_Enter_0;                                            //����I_Rise���õ��������
u16 I_Rise_s = 0;                                                            //����I_Rise����
u16 I_Rise_ms = 0;                                                           //����I_Rise�ĺ���
char String_I_Rise[11];                                                      //����I_Rise���ַ���


u8 I_Fall_Enter = I_Fall_Enter_0;                                            //����I_Fall���õ��������
u16 I_Fall_s = 0;                                                            //����I_Fall����
u16 I_Fall_ms = 0;                                                           //����I_Fall�ĺ���
char String_I_Fall[11];                                                      //����I_Fall���ַ���

char Use_Vint_Or_Vext_Flag = Use_Vint;                                       //����������ʹ���ڲ���ѹ�����ⲿ��ѹ��־

//char Last_State = CV;        //������һ��ʹ�õ�״̬λ

//char Now_State = CV;          //��������ʹ�õ�״̬λ

//char Start_State = 0;        //����տ���ʱ��״̬λ��0�����һ�ο�����1�����ǵ�һ�ο���

//u8 Flag_Sensor1 =0;
//u8 Flag_Sensor2 =0;
//u8 Flag_Sensor3 =0;


void Keys_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE , ENABLE);
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//��©���
	GPIO_InitStructure.GPIO_Pin = KeyRow_1_Pin | KeyRow_2_Pin | KeyRow_3_Pin;
  GPIO_Init(KeyRow_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//������
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
	GPIO_InitStructure.GPIO_Pin = KeyCol_1_Pin | KeyCol_2_Pin | KeyCol_3_Pin | KeyCol_4_Pin | KeyCol_5_Pin | KeyCol_6_Pin | KeyCol_7_Pin;
  GPIO_Init(KeyCol_Port, &GPIO_InitStructure);	
	
	//��һ��ʼ����͵�ƽ
	GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
	GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
	GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);	
	
	keys_EN = DISABLE;//���ð���
}





 /**
  * @brief  ���趨��ѹ���������ַ������Ϊ������
  * @param	������
  * @retval ��Ӧ�ĸ�����
  */
float String_To_Float(char String[])
{
  int temp;
  float Float;
  u8 j;  //�ַ�������
  u8 i;
  u8 k;  //С����λ��
	u8 l;  //�������ֵĸ���
	u8 m;  //λ��

  j = strlen(String);             //��ȡ�ַ�������

  for(i=0, k=0, l=0; i<j; i++)
  {
	  if(String[i] == ' ')         //���ַ���ʣ��Ŀո���0
		{
			String[i] = '0';
		}
		else
		{
			l ++;                      //��ȡ�������ֵĸ���
		}
		
		if(String[i] == '.')         //��ȡС�����λ��
		{
			k = i;
		}
  }
	
	if(k != 0)           //��С����
	{
		m = j -1;          
	}
	else           //��С����
	{
		m=j;
	}
	
	for(i=0, temp=0; i<j;i++)    //����С�������
	{
	  if(String[i] != '.')
	  {
			temp = temp + (String[i]-48)*(pow(10,m));//'0'��Ӧ��ASCII��Ϊ48��ʵ�������ַ�ת����
			m --;
		}
	}
	
	if(k != 0)		//��С����
	{
		Float = temp/(pow(10, (j-k)));		
	}
	else					//û��С����
	{
		Float = temp/(pow(10, (j-l+1)));
	}

	return Float;

}


////// /**
//////  * @brief  ���趨��ѹ���������ַ������Ϊ������
//////  * @param	������
//////  * @retval ��Ӧ�ĸ�����
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
  * @brief  ���ּ��������趨��ѹ�Ĵ�����
  * @param	���ּ��ĺ��루1��2��3��4��5��6��7��8��9��0��
  * @retval None
  */
static void Keys_Input_Set_Voltage(char Number)
{
  if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_V_set) && (LOCK_UNLOCK == UNLOCK) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))    //�����趨��ѹģʽ��������״̬
  {
		if(Keys_Encoder_Mode != Keys_Mode)                         //��һ�ΰ��°���
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //�л�������ģʽ
				
			Cursor_Position = 0;                                     //ָ����Ϊ0λ
				
		  String_Voltage[0] = ' ';                                 //����ַ���
			String_Voltage[1] = ' ';
			String_Voltage[2] = ' ';
			String_Voltage[3] = ' ';
			String_Voltage[4] = ' ';
		}
		
		if(Cursor_Position == 5)
	  {
			Cursor_Position = 0;                                     //�����곬����Χ���ص���ʼλ��
			
			String_Voltage[0] = ' ';                                 //����ַ���
			String_Voltage[1] = ' ';
			String_Voltage[2] = ' ';
			String_Voltage[3] = ' ';
			String_Voltage[4] = ' ';
	  }
		
		String_Voltage[Cursor_Position] = Number;                  //ʹ���λ����Number
		
		if((Cursor_Position == 1) && (String_Voltage[1] != '.'))   //�����λ��1��ΪС���㣬����λ��2��ΪС����
		{
			String_Voltage[2] = '.';
			Cursor_Position = 2;
		}
		
		Cursor_Position ++;                                        //���λ�ü�1      

		Write_String_16x32AsicII(0, 0, String_Voltage);           //��ʾ����ֵ
		Write_Single_16x32AsicII(0, 20, 'V');    		
  }
}

 /**
  * @brief  ���ּ��������趨�����Ĵ�����
  * @param	���ּ��ĺ��루1��2��3��4��5��6��7��8��9��0��
  * @retval None
  */
static void Keys_Input_Set_Current(char Number)
{
  if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_I_set) && (LOCK_UNLOCK == UNLOCK) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))    //�����趨����ģʽ��������״̬
  {
		if(Keys_Encoder_Mode != Keys_Mode)                         //��һ�ΰ��°���
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //�л�������ģʽ
				
			Cursor_Position = 0;                                     //ָ����Ϊ0λ
				
		  String_Current[0] = ' ';                                 //����ַ���
			String_Current[1] = ' ';
			String_Current[2] = ' ';
			String_Current[3] = ' ';
			String_Current[4] = ' ';
			String_Current[5] = ' ';
		}
		
		if(Cursor_Position == 6)
	  {
			Cursor_Position = 0;                                     //�����곬����Χ���ص���ʼλ��
			
			String_Current[0] = ' ';                                 //����ַ���
			String_Current[1] = ' ';
			String_Current[2] = ' ';
			String_Current[3] = ' ';
			String_Current[4] = ' ';
			String_Current[5] = ' ';
	  }
		
		String_Current[Cursor_Position] = Number;                  //ʹ���λ����Number
		
		if((Cursor_Position == 2) && (String_Current[1] != '.') && (String_Current[2] != '.'))   //�����λ��1��2��ΪС���㣬����λ��2��ΪС����
		{
			String_Current[3] = '.';
			Cursor_Position = 3;
		}
		
		Cursor_Position ++;                                        //���λ�ü�1       
		
		Write_String_16x32AsicII(0, 36, String_Current);           //��ʾ����ֵ
		Write_Single_16x32AsicII(0, 60, 'A');		
  }
}

 /**
  * @brief  ���ּ��������趨���ʵĴ�����
  * @param	���ּ��ĺ��루1��2��3��4��5��6��7��8��9��0��
  * @retval None
  */
static void Keys_Input_Set_Power(char Number)
{
  if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_P_set) && (LOCK_UNLOCK == UNLOCK) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))    //�����趨����ģʽ��������״̬
  {
		if(Keys_Encoder_Mode != Keys_Mode)                         //��һ�ΰ��°���
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //�л�������ģʽ
				
			Cursor_Position = 0;                                     //ָ����Ϊ0λ
				
		  String_Power[0] = ' ';                                 //����ַ���
			String_Power[1] = ' ';
			String_Power[2] = ' ';
			String_Power[3] = ' ';
			String_Power[4] = ' ';
		}
		
		if(Cursor_Position == 5)
	  {
			Cursor_Position = 0;                                     //�����곬����Χ���ص���ʼλ��
			
			String_Power[0] = ' ';                                 //����ַ���
			String_Power[1] = ' ';
			String_Power[2] = ' ';
			String_Power[3] = ' ';
			String_Power[4] = ' ';
	  }
		
		String_Power[Cursor_Position] = Number;                  //ʹ���λ����Number
		
		Cursor_Position ++;                                        //���λ�ü�1      

		Write_String_12x24AsicII(42, 0, String_Power);           //��ʾ����ֵ
		Write_Single_12x24AsicII(42, 15, 'W');    		
  }
}

 /**
  * @brief  ���ּ��������趨����ID�Ĵ�����
  * @param	���ּ��ĺ��루1��2��3��4��5��6��7��8��9��0��
  * @retval None
  */
static void Keys_Input_RS232(char Number)
{
	if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == RS232) && (RS232_Set_ID_BaudRate == RS232_Set_ID) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //��һ�ΰ��°���
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //�л�������ģʽ
				
			Cursor_Position = 0;                                     //ָ����Ϊ0λ
				
		  String_RS232_ID[0] = ' ';                                 //����ַ���
			String_RS232_ID[1] = ' ';
		}
		
		if(Cursor_Position == 2)
	  {
			Cursor_Position = 0;                                     //�����곬����Χ���ص���ʼλ��
			
			String_RS232_ID[0] = ' ';                                 //����ַ���
			String_RS232_ID[1] = ' ';
	  }
		
		String_RS232_ID[Cursor_Position] = Number;                  //ʹ���λ����Number
		
		Cursor_Position ++;                                        //���λ�ü�1      

		Write_String_16x32AsicII(0, 12, String_RS232_ID);           //��ʾ����ֵ
	}
}

 /**
  * @brief  ���ּ��������趨����ID�Ĵ�����
  * @param	���ּ��ĺ��루1��2��3��4��5��6��7��8��9��0��
  * @retval None
  */
static void Keys_Input_CAN(char Number)
{
	if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == CAN) && (CAN_Set_ID_BaudRate == CAN_Set_ID) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //��һ�ΰ��°���
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //�л�������ģʽ
				
			Cursor_Position = 0;                                     //ָ����Ϊ0λ
				
		  String_CAN_ID[0] = ' ';                                 //����ַ���
			String_CAN_ID[1] = ' ';
		}
		
		if(Cursor_Position == 2)
	  {
			Cursor_Position = 0;                                     //�����곬����Χ���ص���ʼλ��
			
			String_CAN_ID[0] = ' ';                                 //����ַ���
			String_CAN_ID[1] = ' ';
	  }
		
		String_CAN_ID[Cursor_Position] = Number;                  //ʹ���λ����Number
		
		Cursor_Position ++;                                        //���λ�ü�1      

		Write_String_16x32AsicII(0, 12, String_CAN_ID);           //��ʾ����ֵ
	}
}

 /**
  * @brief  ���ּ��������趨Cycleʱ��Ĵ�����
  * @param	���ּ��ĺ��루1��2��3��4��5��6��7��8��9��0��
  * @retval None
  */
static void Keys_Input_Cycle(char Number)
{
	if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Cycle) && (Cycle_Enter == Cycle_Enter_1) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))       //���ÿ�����
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //��һ�ΰ��°���
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //�л�������ģʽ
				
			Cursor_Position = 0;                                     //ָ����Ϊ0λ
				
		  String_Cycle_On[0] = ' ';                                 //����ַ���
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
			Cursor_Position = 0;                                     //ָ����Ϊ0λ
				
		  String_Cycle_On[0] = ' ';                                 //����ַ���
		  String_Cycle_On[1] = ' ';
		  String_Cycle_On[2] = ' ';
		  String_Cycle_On[3] = ' ';
			String_Cycle_On[5] = ' ';
			String_Cycle_On[6] = ' ';
			String_Cycle_On[7] = ' ';
	  }
		
		String_Cycle_On[Cursor_Position] = Number;                  //ʹ���λ����Number
		
		Cursor_Position ++;                                        //���λ�ü�1      

		Write_String_16x32AsicII(32, 12, String_Cycle_On);           //��ʾ����ֵ
	}
	
	else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Cycle) && (Cycle_Enter == Cycle_Enter_2) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))   //���ÿ�������
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //��һ�ΰ��°���
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //�л�������ģʽ
				
			Cursor_Position = 0;                                     //ָ����Ϊ0λ
				
		  String_Cycle_Close[0] = ' ';                                 //����ַ���
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
			Cursor_Position = 0;                                     //ָ����Ϊ0λ
				
		  String_Cycle_Close[0] = ' ';                                 //����ַ���
		  String_Cycle_Close[1] = ' ';
		  String_Cycle_Close[2] = ' ';
			String_Cycle_Close[3] = ' ';
			String_Cycle_Close[5] = ' ';
			String_Cycle_Close[6] = ' ';
			String_Cycle_Close[7] = ' ';
	  }
		
		String_Cycle_Close[Cursor_Position] = Number;                  //ʹ���λ����Number
		
		Cursor_Position ++;                                        //���λ�ü�1      

		Write_String_16x32AsicII(32, 12, String_Cycle_Close);           //��ʾ����ֵ
	}		
}


 /**
  * @brief  ���ּ��������趨Delayʱ��Ĵ�����
  * @param	���ּ��ĺ��루1��2��3��4��5��6��7��8��9��0��
  * @retval None
  */
static void Keys_Input_Delay(char Number)
{
	if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Delay) && (Delay_Enter == Delay_Enter_1) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))       //���ÿ���ʱ
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //��һ�ΰ��°���
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //�л�������ģʽ
				
			Cursor_Position = 0;                                     //ָ����Ϊ0λ
				
		  String_Delay[0] = ' ';                                 //����ַ���
		  String_Delay[1] = ' ';
      String_Delay[3] = ' ';
			String_Delay[4] = ' ';
			String_Delay[6] = ' ';
			String_Delay[7] = ' ';
		}
		
		String_Delay[Cursor_Position] = Number;                  //ʹ���λ����Number
		
		Cursor_Position ++;                                        //���λ�ü�1  
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
			
			String_Delay[0] = ' ';                                 //����ַ���
		  String_Delay[1] = ' ';
      String_Delay[3] = ' ';
			String_Delay[4] = ' ';
			String_Delay[6] = ' ';
			String_Delay[7] = ' ';
		}

		Write_String_16x32AsicII(32, 14, String_Delay);           //��ʾ����ֵ
	}		
}

 /**
  * @brief  ���ּ��������趨V_Riseʱ��Ĵ�����
  * @param	���ּ��ĺ��루1��2��3��4��5��6��7��8��9��0��
  * @retval None
  */
static void Keys_Input_V_Rise(char Number)
{
	if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == V_Rise) && (V_Rise_Enter == V_Rise_Enter_1) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))       //���ÿ�����
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //��һ�ΰ��°���
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //�л�������ģʽ
				
			Cursor_Position = 0;                                     //ָ����Ϊ0λ
				
		  String_V_Rise[0] = ' ';                                 //����ַ���
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
			Cursor_Position = 0;                                     //ָ����Ϊ0λ
				
		  String_V_Rise[0] = ' ';                                 //����ַ���
		  String_V_Rise[1] = ' ';
		  String_V_Rise[2] = ' ';
		  String_V_Rise[3] = ' ';
			String_V_Rise[5] = ' ';
			String_V_Rise[6] = ' ';
			String_V_Rise[7] = ' ';	
	  }
		
		String_V_Rise[Cursor_Position] = Number;                  //ʹ���λ����Number
		
		Cursor_Position ++;                                        //���λ�ü�1      

		Write_String_16x32AsicII(32, 12, String_V_Rise);           //��ʾ����ֵ
	}	
}

 /**
  * @brief  ���ּ��������趨V_Fallʱ��Ĵ�����
  * @param	���ּ��ĺ��루1��2��3��4��5��6��7��8��9��0��
  * @retval None
  */
static void Keys_Input_V_Fall(char Number)
{
	if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == V_Fall) && (V_Fall_Enter == V_Fall_Enter_1) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))       //���ÿ�����
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //��һ�ΰ��°���
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //�л�������ģʽ
				
			Cursor_Position = 0;                                     //ָ����Ϊ0λ
				
		  String_V_Fall[0] = ' ';                                 //����ַ���
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
			Cursor_Position = 0;                                     //ָ����Ϊ0λ
				
		  String_V_Fall[0] = ' ';                                 //����ַ���
		  String_V_Fall[1] = ' ';
		  String_V_Fall[2] = ' ';
		  String_V_Fall[3] = ' ';
			String_V_Fall[5] = ' ';
			String_V_Fall[6] = ' ';
			String_V_Fall[7] = ' ';	
	  }
		
		String_V_Fall[Cursor_Position] = Number;                  //ʹ���λ����Number
		
		Cursor_Position ++;                                        //���λ�ü�1      

		Write_String_16x32AsicII(32, 12, String_V_Fall);           //��ʾ����ֵ
	}		
}

 /**
  * @brief  ���ּ��������趨I_Riseʱ��Ĵ�����
  * @param	���ּ��ĺ��루1��2��3��4��5��6��7��8��9��0��
  * @retval None
  */
static void Keys_Input_I_Rise(char Number)
{
	if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == I_Rise) && (I_Rise_Enter == I_Rise_Enter_1) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))       //���ÿ����� 
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //��һ�ΰ��°���
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //�л�������ģʽ
				
			Cursor_Position = 0;                                     //ָ����Ϊ0λ
				
		  String_I_Rise[0] = ' ';                                 //����ַ���
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
			Cursor_Position = 0;                                     //ָ����Ϊ0λ
				
		  String_I_Rise[0] = ' ';                                 //����ַ���
		  String_I_Rise[1] = ' ';
		  String_I_Rise[2] = ' ';
		  String_I_Rise[3] = ' ';
			String_I_Rise[5] = ' ';
			String_I_Rise[6] = ' ';
			String_I_Rise[7] = ' ';	
	  }
		
		String_I_Rise[Cursor_Position] = Number;                  //ʹ���λ����Number
		
		Cursor_Position ++;                                        //���λ�ü�1      

		Write_String_16x32AsicII(32, 12, String_I_Rise);           //��ʾ����ֵ
	}		
}

 /**
  * @brief  ���ּ��������趨I_Fallʱ��Ĵ�����
  * @param	���ּ��ĺ��루1��2��3��4��5��6��7��8��9��0��
  * @retval None
  */
static void Keys_Input_I_Fall(char Number)
{
	if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == I_Fall) && (I_Fall_Enter == I_Fall_Enter_1) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))       //���ÿ�����
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //��һ�ΰ��°���
		{
		  Keys_Encoder_Mode = Keys_Mode;                           //�л�������ģʽ
				
			Cursor_Position = 0;                                     //ָ����Ϊ0λ
				
		  String_I_Fall[0] = ' ';                                 //����ַ���
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
			Cursor_Position = 0;                                     //ָ����Ϊ0λ
				
		  String_I_Fall[0] = ' ';                                 //����ַ���
		  String_I_Fall[1] = ' ';
		  String_I_Fall[2] = ' ';
		  String_I_Fall[3] = ' ';
			String_I_Fall[5] = ' ';
			String_I_Fall[6] = ' ';
			String_I_Fall[7] = ' ';	
	  }
		
		String_I_Fall[Cursor_Position] = Number;                  //ʹ���λ����Number
		
		Cursor_Position ++;                                        //���λ�ü�1      

		Write_String_16x32AsicII(32, 12, String_I_Fall);           //��ʾ����ֵ
	}		
}





 /**
  * @brief  ���ּ�������ѡ���ȡ�����ŵĴ�����
  * @param	���ּ��ĺ��루1��2��3��4��5��6��7��8��9��0��
  * @retval None
  */
void Keys_Input_Recall_Save(u8 Number)
{
	if((Display_Interface_Mode == Display_Interface_Mode_Recall_Interface) || (Display_Interface_Mode == Display_Interface_Mode_Save_Interface))   //���ڴ�ȡģʽ
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
  * @brief  ���ְ���0��ɨ�������
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
  * @brief  ���ְ���1��ɨ�������
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
  * @brief  ���ְ���2��ɨ�������
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
  * @brief  ���ְ���3��ɨ�������
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
  * @brief  ���ְ���4��ɨ�������
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
  * @brief  ���ְ���5��ɨ�������
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
  * @brief  ���ְ���6��ɨ�������
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
  * @brief  ���ְ���7��ɨ�������
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
  * @brief  ���ְ���8��ɨ�������
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
  * @brief  ���ְ���9��ɨ�������
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
  * @brief  С���㰴����ɨ�������
  * @param	None
  * @retval None
  */
void Key_Decimal_Point(void)
{
  if(LOCK_UNLOCK == UNLOCK)         //��ť��İ������Ҵ���������״̬
	{
		if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_V_set) && (Keys_Encoder_Mode == Keys_Mode) && (Cursor_Position == 1) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))  //���ڼ���������趨��ѹģʽ�ҹ��λ��Ϊ1
		{
			String_Voltage[1] = '.';       //�ù��λ��1��С����
			Cursor_Position ++;            //�����
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_I_set) && (Keys_Encoder_Mode == Keys_Mode)        //���ڼ���������趨����ģʽ�ҹ��λ��λ��1����2 
			      && ((Cursor_Position == 1) || (Cursor_Position == 2)))
		{
			String_Current[Cursor_Position] = '.';    //�ù��λ����С����
			Cursor_Position ++;                       //�����
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_P_set) && (Keys_Encoder_Mode == Keys_Mode)        //���ڼ���������趨����ģʽ�ҹ��λ��λ��1����2����3 
			      && ((Cursor_Position == 1) || (Cursor_Position == 2) || (Cursor_Position == 3)) 
		        && (String_Power[1] != '.') && (String_Power[2] != '.') )
		{
			String_Power[Cursor_Position] = '.';    //�ù��λ����С����
			Cursor_Position ++;                       //�����
		}
	}
}

 /**
  * @brief  ��ʾDelay�����ý���
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
	Write_String_16x32AsicII(32, 14, String_Delay);  //����ʾ����ʾ
}

 /**
  * @brief  ��ʾCycle����ʱ������ý���
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
	
  Write_String_16x32AsicII(0, 0, "Open Time:");          //��OLED����ʾ				
	Write_String_16x32AsicII(32, 12, String_Cycle_On);
}
	
 /**
  * @brief  ��ʾCycle�ر�ʱ������ý���
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
	
  Write_String_16x32AsicII(0, 0, "Close Time:");          //��OLED����ʾ				
	Write_String_16x32AsicII(32, 12, String_Cycle_Close);
}

 /**
  * @brief  ��ʾV-Riseʱ������ý���
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
	
  Write_String_16x32AsicII(0, 0, "V-Rise:");          //��OLED����ʾ				
	Write_String_16x32AsicII(32, 12, String_V_Rise);
}

 /**
  * @brief  ��ʾV-Fallʱ������ý���
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
	
  Write_String_16x32AsicII(0, 0, "V-Fall:");          //��OLED����ʾ				
	Write_String_16x32AsicII(32, 12, String_V_Fall);
}

 /**
  * @brief  ��ʾV-Fallʱ������ý���
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
	
  Write_String_16x32AsicII(0, 0, "I-Rise:");          //��OLED����ʾ				
	Write_String_16x32AsicII(32, 12, String_I_Rise);
}

 /**
  * @brief  ��ʾV-Fallʱ������ý���
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
	
  Write_String_16x32AsicII(0, 0, "I-Fall:");          //��OLED����ʾ				
	Write_String_16x32AsicII(32, 12, String_I_Fall);
}

 /**
  * @brief  ����Enter��ɨ�������
  * @param	None
  * @retval None
  */
void Key_Enter(void)
{
  if((LOCK_UNLOCK == UNLOCK) && (Display_Interface_Mode != Display_Interface_Mode_Protect_Interface))              //ȷ�ϰ����Ѿ������˺ʹ���������״̬
	{

		if(Display_Interface_Mode == Display_Interface_Mode_First_Menu)  		//�����ڵ�һ���˵�   
		{
			Display_Interface_Mode = Display_Interface_Mode_Second_Menu;      //��Ϊ�ڶ����˵�
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
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Second_Menu) && (Second_Selection == Menu_Vint))  //�����ڵڶ����˵���Vint
		{
			Use_Vint_Or_Vext_Flag = Use_Vint;
					
			AT25_Save_Use_Vint_or_Vext_Flag();		
					
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual; //������ʵֵ����
		}
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Second_Menu) && (Second_Selection == Menu_Vext))  //�����ڵڶ����˵���Vint
		{
			Use_Vint_Or_Vext_Flag = Use_Vext;
					
			AT25_Save_Use_Vint_or_Vext_Flag();		
					
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual; //������ʵֵ����
		}
/*------------------------------------------------------------------------RS232-------------------------------------------*/		
		
		else if (((Display_Interface_Mode == Display_Interface_Mode_Second_Menu) || (Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == RS232))  //�����ڵڶ����˵���RS232
		{
			if(RS232_Set_ID_BaudRate == RS232_First_Select)                         //�ս���RS232����
			{
			  Display_Interface_Mode = Display_Interface_Mode_Third_Menu;     //��Ϊ�������˵�
			  if(Last_Interface != Display_Interface_Mode)                                //����
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
				
				Display_RS232_ID(RS232_ID);                                    //��ʾID
				
			  Last_Interface = Display_Interface_Mode_Third_Menu;            
				
				RS232_Set_ID_BaudRate = RS232_Set_ID;                          //��Ϊ����ID��ģʽ
				
        Keys_Encoder_Mode = Encoder_Mode;                              //Ĭ���ñ���������				
				
				Cursor_Position = 1;                                           //�趨���λ��
				
			  TIM_Cmd(TIM5, ENABLE);                                         //ʹ�ܶ�ʱ��
			}
			
			else if(RS232_Set_ID_BaudRate == RS232_Set_ID)                   //��������IDģʽ
			{
				RS232_ID = String_To_Float(String_RS232_ID);                   //�趨ID
				
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
				
				RS232_Set_ID_BaudRate = RS232_Set_BaudRate;                    //��Ϊ���ò�����ģʽ
				
				Keys_Encoder_Mode = Encoder_Mode;                              //��ر�����ģʽ
			}
			
			else if(RS232_Set_ID_BaudRate == RS232_Set_BaudRate)            //�������ò�����ģʽ
			{
				RS232_Set_ID_BaudRate = RS232_First_Select;                   //��ص�һ��ѡ��RS232
				
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
				
				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual; //������ʵֵ����
			}
		}
		
/*----------------------------------------------------------------------------------CAN--------------------------------------------------------------------------*/		
		
		else if (((Display_Interface_Mode == Display_Interface_Mode_Second_Menu) || (Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == CAN))  //�����ڵڶ����˵���CAN
		{
			if(CAN_Set_ID_BaudRate == CAN_First_Select)                         //�ս���CAN����
			{
			  Display_Interface_Mode = Display_Interface_Mode_Third_Menu;     //��Ϊ�������˵�
			  if(Last_Interface != Display_Interface_Mode)                                //����
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
				
				Display_CAN_ID(CAN_ID);                                    //��ʾID
				
			  Last_Interface = Display_Interface_Mode_Third_Menu;            
				
				CAN_Set_ID_BaudRate = CAN_Set_ID;                          //��Ϊ����ID��ģʽ
				
        Keys_Encoder_Mode = Encoder_Mode;                              //Ĭ���ñ���������				
				
				Cursor_Position = 1;                                           //�趨���λ��
				
			  TIM_Cmd(TIM5, ENABLE);                                         //ʹ�ܶ�ʱ��
			}
			
			else if(CAN_Set_ID_BaudRate == CAN_Set_ID)                   //��������IDģʽ
			{
				CAN_ID = String_To_Float(String_CAN_ID);                   //�趨ID
				
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
				
				CAN_Set_ID_BaudRate = CAN_Set_BaudRate;                    //��Ϊ���ò�����ģʽ
				
				Keys_Encoder_Mode = Encoder_Mode;                              //��ر�����ģʽ
			}
			
			else if(CAN_Set_ID_BaudRate == CAN_Set_BaudRate)            //�������ò�����ģʽ
			{
				CAN_Set_ID_BaudRate = CAN_First_Select;                   //��ص�һ��ѡ��CAN
				
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
				
				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual; //������ʵֵ����
			}
		}		
		
		
/*-------------------------------------------------------------------------------------------------Cycle---------------------------------------------------------------*/		
		

		else if (((Display_Interface_Mode == Display_Interface_Mode_Second_Menu) || (Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == Cycle))  //�����ڵڶ����˵���Cycle
		{
			if (Cycle_Enter == Cycle_Enter_0)    //��һ��ѡ��Cycle
			{
				Keys_Encoder_Mode = Encoder_Mode;
				
			  Display_Interface_Mode = Display_Interface_Mode_Third_Menu;     //��Ϊ�������˵�

			  if(Last_Interface != Display_Interface_Mode)                                //����
	      {
				  Fill_RAM(0x00);
        }				
				
				Display_Oled_CycleOn();                                        //��ѭ��ʱ����תΪ�ַ�������ʾ
				
				Display_Interface_Mode = Display_Interface_Mode_Third_Menu;    //���˵���������˵�
				
			  Last_Interface = Display_Interface_Mode_Third_Menu;   
				
        Cycle_Enter = Cycle_Enter_1;                //��¼����һ�ΰ���
				
				Cursor_Position = 7;
				
				TIM_Cmd(TIM5, ENABLE);                                         //ʹ�ܶ�ʱ��
			}
			
			
			
			else if (Cycle_Enter == Cycle_Enter_1)                     //�ڶ��ΰ�����
			{
				Cycle_Enter = Cycle_Enter_2;       //��¼�������ΰ���
				
				Display_Oled_CycleClose();                                        //��ѭ��ʱ����תΪ�ַ�������ʾ
				
				Cursor_Position = 7;
				
				Keys_Encoder_Mode = Encoder_Mode;                              //��ر�����ģʽ							
			}
			
			
			
			else if (Cycle_Enter == Cycle_Enter_2)                 //�����ΰ�����
			{			 
				u8 i;
				
				Cycle_Enter = Cycle_Enter_3;      //��¼�������ΰ��� 

				//���ַ����������
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
				
				Keys_Encoder_Mode = Encoder_Mode;                              //��ر�����ģʽ  

				Cycle_Enter = Cycle_Enter_0;    //���û����enter��״̬
				
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
					
					if(Delay_Function_On_Off == Delay_Function_On)               //������ʱ����
					{
						TIM_DELAY_ON();
					}
					else if(Cycle_Function_On_Off == Cycle_Function_On)          //����ѭ������
					{
						TIM_CYCLE_ON();
					}
					else
					{
						Power_On();
					}
				}

				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual; //������ʵֵ����
			}
			
		}
		
/*------------------------------------------------------------------Delay-----------------------------------------------------------------------------*/
		
		else if (((Display_Interface_Mode == Display_Interface_Mode_Second_Menu) || (Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == Delay))  //�����ڵڶ����˵���Delay
		{     
			if(Delay_Enter == Delay_Enter_0)                     //���û����Enter
			{
				Keys_Encoder_Mode = Encoder_Mode;                          //һ��ʼ���ڱ���������ģʽ
			
		    if(Last_Interface != Display_Interface_Mode)                                //����
				{
					Fill_RAM(0x00);
				}				
				
				Display_Interface_Mode = Display_Interface_Mode_Third_Menu;			//����ѡ�������˵�ģʽ	
				
				Display_Oled_Delay();                           //��ʾ����
				
				Last_Interface = Display_Interface_Mode_Third_Menu;   
				
				Cursor_Position = 7;			//һ��ʼ��괦���������λ
				
				Delay_Enter = Delay_Enter_1;   //��¼����һ��Enter
				
				TIM_Cmd(TIM5, ENABLE);				//�򿪶�ʱ����ʾ���
			}
			
			else if (Delay_Enter == Delay_Enter_1)       //�ڶ��ΰ�����
			{
				u8 i;
				
				Delay_Enter = Delay_Enter_2;
				
				//���ַ����������
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

				Keys_Encoder_Mode = Encoder_Mode;                              //��ر�����ģʽ	
				
				if((Delay_h == 0) && (Delay_m == 0) && (Delay_s == 0))      //�ж��Ƿ�򿪹���
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
					
					if(Delay_Function_On_Off == Delay_Function_On)               //������ʱ����
					{
						TIM_DELAY_ON();
					}
					else if(Cycle_Function_On_Off == Cycle_Function_On)          //����ѭ������
					{
						TIM_CYCLE_ON();
					}
					else
					{
						Power_On();
					}
				}
				

				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual; //������ʵֵ����

				Delay_Enter = Delay_Enter_0;
			}
		}			
		
/*--------------------------------------------------------------------V_RISE---------------------------------------------------------------*/
		
		else if (((Display_Interface_Mode == Display_Interface_Mode_Second_Menu) || (Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == V_Rise))  //�����ڵڶ����˵���V-Rise
		{
			if (V_Rise_Enter == V_Rise_Enter_0)    //��һ��ѡ��V_Rise
			{                             
				V_Rise_Enter = V_Rise_Enter_1;
				
				Keys_Encoder_Mode = Encoder_Mode;   //������ģʽ
				
			  Display_Interface_Mode = Display_Interface_Mode_Third_Menu;     //��Ϊ�������˵�
				
			  if(Last_Interface != Display_Interface_Mode)                                //����
	      {
				  Fill_RAM(0x00);
        }				
				
				Display_Oled_V_Rise();                                        //��ѭ��ʱ����תΪ�ַ���

				Display_Interface_Mode = Display_Interface_Mode_Third_Menu;
				
			  Last_Interface = Display_Interface_Mode_Third_Menu;   
				
				Cursor_Position = 7;
				
				TIM_Cmd(TIM5, ENABLE);                                         //ʹ�ܶ�ʱ��
			}
			
			else if (V_Rise_Enter== V_Rise_Enter_1)        //�ڶ��ΰ�����
			{
				u8 i;
				
				V_Rise_Enter = V_Rise_Enter_2;

				//���ַ����������
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
				
				Keys_Encoder_Mode = Encoder_Mode;                              //��ر�����ģʽ				
				
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
					
					if(Delay_Function_On_Off == Delay_Function_On)               //������ʱ����
					{
						TIM_DELAY_ON();
					}
					else if(Cycle_Function_On_Off == Cycle_Function_On)          //����ѭ������
					{
						TIM_CYCLE_ON();
					}
					else
					{
						Power_On();
					}
				}				

				V_Rise_Enter = V_Rise_Enter_0;

				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual; //������ʵֵ����				
			}
		}

/*--------------------------------------------------------------------V_FALL---------------------------------------------------------------*/
		
		else if (((Display_Interface_Mode == Display_Interface_Mode_Second_Menu) || (Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == V_Fall))  //�����ڵڶ����˵���V-Fall
		{
			if (V_Fall_Enter == V_Fall_Enter_0)    //��һ��ѡ��V_Fall
			{                             
				V_Fall_Enter = V_Fall_Enter_1;
				
				Keys_Encoder_Mode = Encoder_Mode;   //������ģʽ
				
			  Display_Interface_Mode = Display_Interface_Mode_Third_Menu;     //��Ϊ�������˵�
				
			  if(Last_Interface != Display_Interface_Mode)                                //����
	      {
				  Fill_RAM(0x00);
        }				
				
				Display_Oled_V_Fall();                                        //��ѭ��ʱ����תΪ�ַ���

				Display_Interface_Mode = Display_Interface_Mode_Third_Menu;
				
			  Last_Interface = Display_Interface_Mode_Third_Menu;   
				
				Cursor_Position = 7;
				
				TIM_Cmd(TIM5, ENABLE);                                         //ʹ�ܶ�ʱ��
			}
			
			else if (V_Fall_Enter== V_Fall_Enter_1)        //�ڶ��ΰ�����
			{
				u8 i;
				
				V_Fall_Enter = V_Fall_Enter_2;

				//���ַ����������
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
				
				Keys_Encoder_Mode = Encoder_Mode;                              //��ر�����ģʽ				
				
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
					
					if(Delay_Function_On_Off == Delay_Function_On)               //������ʱ����
					{
						TIM_DELAY_ON();
					}
					else if(Cycle_Function_On_Off == Cycle_Function_On)          //����ѭ������
					{
						TIM_CYCLE_ON();
					}
					else
					{
						Power_On();
					}
				}				

				V_Fall_Enter = V_Fall_Enter_0;

				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual; //������ʵֵ����				
			}
		}

/*--------------------------------------------------------------------I_RISE---------------------------------------------------------------*/
		
		else if (((Display_Interface_Mode == Display_Interface_Mode_Second_Menu) || (Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == I_Rise))  //�����ڵڶ����˵���V-Rise
		{	
			if (I_Rise_Enter == I_Rise_Enter_0)    //��һ��ѡ��I_Rise
			{                             
				I_Rise_Enter = I_Rise_Enter_1;
				
				Keys_Encoder_Mode = Encoder_Mode;   //������ģʽ
				
			  Display_Interface_Mode = Display_Interface_Mode_Third_Menu;     //��Ϊ�������˵�
				
			  if(Last_Interface != Display_Interface_Mode)                                //����
	      {
				  Fill_RAM(0x00);
        }				
				
				Display_Oled_I_Rise();                                        //��ѭ��ʱ����תΪ�ַ���

				Display_Interface_Mode = Display_Interface_Mode_Third_Menu;
				
			  Last_Interface = Display_Interface_Mode_Third_Menu;   
				
				Cursor_Position = 7;
				
				TIM_Cmd(TIM5, ENABLE);                                         //ʹ�ܶ�ʱ��
			}
			
			else if (I_Rise_Enter== I_Rise_Enter_1)        //�ڶ��ΰ�����
			{
				u8 i;
				
				I_Rise_Enter = I_Rise_Enter_2;

				//���ַ����������
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
				
				Keys_Encoder_Mode = Encoder_Mode;                              //��ر�����ģʽ				
				
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
					
					if(Delay_Function_On_Off == Delay_Function_On)               //������ʱ����
					{
						TIM_DELAY_ON();
					}
					else if(Cycle_Function_On_Off == Cycle_Function_On)          //����ѭ������
					{
						TIM_CYCLE_ON();
					}
					else
					{
						Power_On();
					}
				}				

				I_Rise_Enter = I_Rise_Enter_0;

				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual; //������ʵֵ����				
			}
		}

/*--------------------------------------------------------------------I_FALL---------------------------------------------------------------*/
		
		else if (((Display_Interface_Mode == Display_Interface_Mode_Second_Menu) || (Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == I_Fall))  //�����ڵڶ����˵���V-Rise
		{	
			if (I_Fall_Enter == I_Fall_Enter_0)    //��һ��ѡ��I_Fall
			{                             
				I_Fall_Enter = I_Fall_Enter_1;
				
				Keys_Encoder_Mode = Encoder_Mode;   //������ģʽ
				
			  Display_Interface_Mode = Display_Interface_Mode_Third_Menu;     //��Ϊ�������˵�
				
			  if(Last_Interface != Display_Interface_Mode)                                //����
	      {
				  Fill_RAM(0x00);
        }				
				
				Display_Oled_I_Fall();                                        //��ѭ��ʱ����תΪ�ַ���

				Display_Interface_Mode = Display_Interface_Mode_Third_Menu;
				
			  Last_Interface = Display_Interface_Mode_Third_Menu;   
				
				Cursor_Position = 7;
				
				TIM_Cmd(TIM5, ENABLE);                                         //ʹ�ܶ�ʱ��
			}
			
			else if (I_Fall_Enter== I_Fall_Enter_1)        //�ڶ��ΰ�����
			{
				u8 i;
				
				I_Fall_Enter = I_Fall_Enter_2;

				//���ַ����������
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
				
				Keys_Encoder_Mode = Encoder_Mode;                              //��ر�����ģʽ				
				
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
					
					if(Delay_Function_On_Off == Delay_Function_On)               //������ʱ����
					{
						TIM_DELAY_ON();
					}
					else if(Cycle_Function_On_Off == Cycle_Function_On)          //����ѭ������
					{
						TIM_CYCLE_ON();
					}
					else
					{
						Power_On();
					}
				}				

				I_Fall_Enter = I_Fall_Enter_0;

				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual; //������ʵֵ����				
			}
		}	
		
/*--------------------------------------------------------------------Reset---------------------------------------------------------*/		
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Second_Menu) && (Second_Selection == Reset))    //���ڶ����˵���ѡ���˸�λ��ϵͳ��λ
		{
			AT25_Reset();
			NVIC_SystemReset();
		}
		
		
		
/*--------------------------------------------------------------------�趨V I Pֵ--------------------------------------------------*/		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_V_set) && (Keys_Encoder_Mode == Encoder_Mode))   //�������趨��ѹģʽ�ͱ�����ģʽ
		{
		  Set_Voltage = String_To_Float(String_Voltage);   //���ַ������趨��ѹ��Ϊfloat���趨��ѹ
			if(Set_Voltage > 80)     //��ѹ���ֵΪ80
			{
				Set_Voltage = 80;
			}
			CV_CC_CP = CV;		
			AT25_Save_VIPSet();
			AT25_Save_CC_CV_CP();
			
			if((OUTPUT_NOT_OUTPUT == OUTPUT) && (DELAY_NOT_DELAY == NOT_DELAY) && (ON_OFF == ON))     //�������ģʽ������ʱʱ�����,���ı��趨ֵ�Ժ�Ҫ�ı������ѹ
			{
				Power_On();
			}
			
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;    //�����ʵֵ���˵���ʾģʽ
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_V_set) && (Keys_Encoder_Mode == Keys_Mode))   //�������趨��ѹģʽ�ͼ���ģʽ
		{
			Set_Voltage = String_To_Float(String_Voltage);     //���ַ������趨��ѹ��Ϊfloat���趨��ѹ
			if(Set_Voltage > 80)     //��ѹ���ֵΪ80
			{
				Set_Voltage = 80;
			}
			Keys_Encoder_Mode = Encoder_Mode;
			CV_CC_CP = CV;
			AT25_Save_VIPSet();
			AT25_Save_CC_CV_CP();
			
			if((OUTPUT_NOT_OUTPUT == OUTPUT) && (DELAY_NOT_DELAY == NOT_DELAY) && (ON_OFF == ON))     //�������ģʽ������ʱʱ�����,���ı��趨ֵ�Ժ�Ҫ�ı������ѹ
			{
				Power_On();
			}
			
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_I_set) && (Keys_Encoder_Mode == Encoder_Mode))   //�������趨����ģʽ�ͱ�����ģʽ
		{
		  Set_Current = String_To_Float(String_Current);      //���ַ������趨������Ϊfloat���趨����
			if(Set_Current > 100)     //�������ֵΪ100
			{
				Set_Current = 100;
			}
			CV_CC_CP = CC;
			AT25_Save_VIPSet();
			AT25_Save_CC_CV_CP();
			
			if((OUTPUT_NOT_OUTPUT == OUTPUT) && (DELAY_NOT_DELAY == NOT_DELAY) && (ON_OFF == ON))     //�������ģʽ������ʱʱ�����,���ı��趨ֵ�Ժ�Ҫ�ı������ѹ
			{
				Power_On();
			}
			
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;    //�����ʵֵ���˵���ʾģʽ
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_I_set) && (Keys_Encoder_Mode == Keys_Mode))   //�������趨����ģʽ�ͼ���ģʽ
		{
			Set_Current = String_To_Float(String_Current);     //���ַ������趨��ѹ��Ϊfloat���趨��ѹ
			if(Set_Current > 100)     //�������ֵΪ100
			{
				Set_Current = 100;
			}
			CV_CC_CP = CC;
			Keys_Encoder_Mode = Encoder_Mode;          //��ر�����ģʽ
			AT25_Save_VIPSet();
			AT25_Save_CC_CV_CP();
			
			if((OUTPUT_NOT_OUTPUT == OUTPUT) && (DELAY_NOT_DELAY == NOT_DELAY) && (ON_OFF == ON))     //�������ģʽ������ʱʱ�����,���ı��趨ֵ�Ժ�Ҫ�ı������ѹ
			{
				Power_On();
			}
			
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;      //�����ʵֵ��ʾ����
		}
    
    else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_P_set) && (Keys_Encoder_Mode == Encoder_Mode))    //�������趨����ģʽ�ͱ�����ģʽ
		{
		  Set_Power = String_To_Float(String_Power);      //���ַ������趨������ΪPower���趨����
			if(Set_Power > 6000)        //�������ֵΪ6000
			{
				Set_Power = 6000;
			}
			CV_CC_CP = CP;
			AT25_Save_VIPSet();
			AT25_Save_CC_CV_CP();
			
			if((OUTPUT_NOT_OUTPUT == OUTPUT) && (DELAY_NOT_DELAY == NOT_DELAY) && (ON_OFF == ON))     //�������ģʽ������ʱʱ�����,���ı��趨ֵ�Ժ�Ҫ�ı������ѹ
			{
				Power_On();
			}
			
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;      //�����ʵֵ���˵���ʾģʽ			
		}			
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_P_set) && (Keys_Encoder_Mode == Keys_Mode))   //�������趨����ģʽ�ͼ���ģʽ
		{
			Set_Power = String_To_Float(String_Power);     //���ַ������趨��ѹ��Ϊfloat���趨��ѹ
			if(Set_Power > 6000)     //�������ֵΪ100
			{
				Set_Power = 6000;
			}
			CV_CC_CP = CP;
			Keys_Encoder_Mode = Encoder_Mode;          //��ر�����ģʽ
			AT25_Save_VIPSet();
			AT25_Save_CC_CV_CP();
			
			if((OUTPUT_NOT_OUTPUT == OUTPUT) && (DELAY_NOT_DELAY == NOT_DELAY) && (ON_OFF == ON))     //�������ģʽ������ʱʱ�����,���ı��趨ֵ�Ժ�Ҫ�ı������ѹ
			{
				Power_On();
			}
			
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;      //�����ʵֵ��ʾ����
		}
		
		else if(Display_Interface_Mode == Display_Interface_Mode_Recall_Interface)    //����ȡģʽ
		{
			Set_Voltage = Recall_Save_Voltage[Recall_Save_Number];
			Set_Current = Recall_Save_Current[Recall_Save_Number];
			Set_Power = Recall_Save_Power[Recall_Save_Number];
			AT25_Save_VIPSet();
			
			if((OUTPUT_NOT_OUTPUT == OUTPUT) && (DELAY_NOT_DELAY == NOT_DELAY) && (ON_OFF == ON))     //�������ģʽ������ʱʱ�����,���ı��趨ֵ�Ժ�Ҫ�ı������ѹ
			{
				Power_On();
			}
			
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;
		}
		
		else if(Display_Interface_Mode == Display_Interface_Mode_Save_Interface)      //���ڴ�ģʽ
		{
			Recall_Save_Voltage[Recall_Save_Number] = Set_Voltage;
			Recall_Save_Current[Recall_Save_Number] = Set_Current;
			Recall_Save_Power[Recall_Save_Number] = Set_Power;
			AT25_Save_RecallSaveSet(Recall_Save_Number);
			
			if((OUTPUT_NOT_OUTPUT == OUTPUT) && (DELAY_NOT_DELAY == NOT_DELAY) && (ON_OFF == ON))     //�������ģʽ������ʱʱ�����,���ı��趨ֵ�Ժ�Ҫ�ı������ѹ
			{
				Power_On();
			}
			
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;
		}
	}
}

 /**
  * @brief  ����Shift��ɨ�������
  * @param	None
  * @retval None
  */
void Key_Shift(void)
{
  if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Actual) || (Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Preset))     //ȷ�ϰ�����İ�����,�����趨ֵ����ʵֵ����
	{
		if(Shift_Not_Shift == Not_Shift)               //���û���ã��͸���
		{
			Shift_Not_Shift = Shift;
		}
		else if(Shift_Not_Shift == Shift)         //��������ˣ���ȡ������
		{
			Shift_Not_Shift = Not_Shift; 
		}
	}
}

 /**
  * @brief  ����ESC��ɨ�������
  * @param	None
  * @retval None
  */
void Key_ESC(void)
{
  if(LOCK_UNLOCK == UNLOCK)     //ȷ��������İ����˺ʹ���������״̬
	{
		if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_V_set) && (Keys_Encoder_Mode == Keys_Mode))       //�����趨��ѹģʽ�ͼ�������ģʽ�����ر�����ģʽ
		{
			Keys_Encoder_Mode = Encoder_Mode;            //���ر�����ģʽ
			Display_Main_Interface_Voltage(Set_Voltage);              //����Ĭ�ϵĵ�ѹ�ַ���
			Cursor_Position = 4;
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_I_set) && (Keys_Encoder_Mode == Keys_Mode))       //�����趨����ģʽ�ͼ�������ģʽ�����ر�����ģʽ
		{
			Keys_Encoder_Mode = Encoder_Mode;            //���ر�����ģʽ
			Display_Main_Interface_Current(Set_Current);              //����Ĭ�ϵĵ�ѹ�ַ���
			Cursor_Position = 5;
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_P_set) && (Keys_Encoder_Mode == Keys_Mode))       //�����趨����ģʽ�ͼ�������ģʽ�����ر�����ģʽ
		{
			Keys_Encoder_Mode = Encoder_Mode;            //���ر�����ģʽ
			Display_Main_Interface_Power(Set_Power);              //����Ĭ�ϵĵ�ѹ�ַ���
			if((Set_Power >= 1000) || (Set_Power <10))
			{
				String_Power[0] = '0';
				Write_Single_12x24AsicII(42, 0, '0');
			}
			Cursor_Position = 4;
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Recall_Interface) || (Display_Interface_Mode == Display_Interface_Mode_Save_Interface))        //���ڴ�ȡ����
		{
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;
		}
		
/*------------------------------------------------------------------------------------------------RS232---------------------------------------------------------------*/		
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == RS232))           //��������RS232��������
		{
			if((RS232_Set_ID_BaudRate == RS232_Set_ID) && (Keys_Encoder_Mode == Encoder_Mode))      //ֻ�б�����ģʽ���ܷ��ض����˵�
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
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == CAN))           //��������CAN��������
		{
			if((CAN_Set_ID_BaudRate == CAN_Set_ID) && (Keys_Encoder_Mode == Encoder_Mode))      //ֻ�б�����ģʽ���ܷ��ض����˵�
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
    else if (((Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == V_Rise))  //�����ڵڶ����˵���V_Rise
		{		
			if ((V_Rise_Enter == V_Rise_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //�������ÿ�����
			{
				Display_Interface_Mode = Display_Interface_Mode_Second_Menu;
				
				V_Rise_Enter = V_Rise_Enter_0;				
			}
			else if ((V_Rise_Enter == V_Rise_Enter_1) && (Keys_Encoder_Mode == Keys_Mode))
			{
				Keys_Encoder_Mode = Encoder_Mode;
				
				Cursor_Position = 7;
				
				Display_Oled_V_Rise();                                        //��ѭ��ʱ����תΪ�ַ�������ʾ				
			}
		}		
		
/*---------------------------------------------------------------------------V-Fall--------------------------------------------------------------------*/
    else if (((Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == V_Fall))  //�����ڵڶ����˵���V_Fall
		{		
			if ((V_Fall_Enter == V_Fall_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //�������ÿ�����
			{
				Display_Interface_Mode = Display_Interface_Mode_Second_Menu;
				
				V_Fall_Enter = V_Fall_Enter_0;				
			}
			else if ((V_Fall_Enter == V_Fall_Enter_1) && (Keys_Encoder_Mode == Keys_Mode))
			{
				Keys_Encoder_Mode = Encoder_Mode;
				
				Cursor_Position = 7;
				
				Display_Oled_V_Fall();                                        //��ѭ��ʱ����תΪ�ַ�������ʾ				
			}
		}			
		
/*---------------------------------------------------------------------------I-Rise--------------------------------------------------------------------*/
    else if (((Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == I_Rise))  //�����ڵڶ����˵���I_Rise
		{		
			if ((I_Rise_Enter == I_Rise_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //�������ÿ�����
			{
				Display_Interface_Mode = Display_Interface_Mode_Second_Menu;
				
				I_Rise_Enter = I_Rise_Enter_0;				
			}
			else if ((I_Rise_Enter == I_Rise_Enter_1) && (Keys_Encoder_Mode == Keys_Mode))
			{
				Keys_Encoder_Mode = Encoder_Mode;
				
				Cursor_Position = 7;
				
				Display_Oled_I_Rise();                                        //��ѭ��ʱ����תΪ�ַ�������ʾ				
			}
		}	

/*---------------------------------------------------------------------------I-Fall--------------------------------------------------------------------*/
    else if (((Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == I_Fall))  //�����ڵڶ����˵���I_Fall
		{		
			if ((I_Fall_Enter == I_Fall_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //�������ÿ�����
			{
				Display_Interface_Mode = Display_Interface_Mode_Second_Menu;
				
				I_Fall_Enter = I_Fall_Enter_0;				
			}
			else if ((I_Fall_Enter == I_Fall_Enter_1) && (Keys_Encoder_Mode == Keys_Mode))
			{
				Keys_Encoder_Mode = Encoder_Mode;
				
				Cursor_Position = 7;
				
				Display_Oled_I_Fall();                                        //��ѭ��ʱ����תΪ�ַ�������ʾ				
			}
		}			
		
		
/*---------------------------------------------------------------------------Cycle--------------------------------------------------------------------*/
    else if (((Display_Interface_Mode == Display_Interface_Mode_Third_Menu)) && (Second_Selection == Cycle))  //�����ڵڶ����˵���Cycle
		{		
			if ((Cycle_Enter == Cycle_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //�������ÿ�����
			{
				Display_Interface_Mode = Display_Interface_Mode_Second_Menu;
				
				Cycle_Enter = Cycle_Enter_0;				
			}
			else if ((Cycle_Enter == Cycle_Enter_1) && (Keys_Encoder_Mode == Keys_Mode))
			{
				Keys_Encoder_Mode = Encoder_Mode;
				
				Cursor_Position = 7;
				
				Display_Oled_CycleOn();                                        //��ѭ��ʱ����תΪ�ַ�������ʾ				
			}
			
			
			else if ((Cycle_Enter == Cycle_Enter_2) && (Keys_Encoder_Mode == Encoder_Mode))       //�������ÿ�������
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
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Delay))  //�����ڵڶ����˵���Delay
		{
			if (Delay_Enter == Delay_Enter_1)    //��������hģʽ
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
		
/*-------------------------------------------------------------����ģʽ-------------------------------------------------------*/
    else if(Display_Interface_Mode == Display_Interface_Mode_Protect_Interface)
		{
//			First_Into_ErrorInterface_Flag = Have_Not_Into;
//			Error = 0;
//			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;
			
//			Flag_Sensor1 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);//�۲����
//			Flag_Sensor2 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);
//			Flag_Sensor3 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7);
			
				//OTP��������
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

										Error &= ~OTP;//������ֵ��OTPλ��0
										OTP_SensorX = 0;//����

							}
						}
					}
				}
				
				if(Voltage <= 10)
				{
					Error &= ~OVP;//������ֵ��OTPλ��0
				}
				
				if((Current <= 10) && (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11) == 1))
				{
					Error &=~ OCP;//������ֵ��OCPλ��0
					OCP_SoftOrHard_Flag = 2;
				}
			
				if(Power <= 10)
				{
					Error &=~ OPP;//������ֵ��OCPλ��0
				}			
			  
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 0)
				{
					SysTick_Delay_Ms(5);
					if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 0)
					Error &=~ Sense_Reverse;   //������ֵ��Sense_Reverseλ��0
				}
				
				if(Error == PF)//ȫ���������������
				{
					First_Into_ErrorInterface_Flag = Have_Not_Into;
					Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;
				}
			
			
		}



		
		
/*-------------------------------------------------------------------�˵�--------------------------------------------------------------------------*/		
		
		else
		{
			switch(Display_Interface_Mode)           //�����ϼ�����
		  {
			  case Display_Interface_Mode_Second_Menu: Display_Interface_Mode = Display_Interface_Mode_First_Menu;break;                          //���ڶ����˵������ص�һ���˵�
			  case Display_Interface_Mode_First_Menu: Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;break;               //����һ���˵���������ʵֵ���˵�
//			  case Display_Interface_Mode_Main_Interface_Preset: Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;break;    //�����趨ֵ��ʾ���棬������ʵֵ����
			  case Display_Interface_Mode_Main_Interface_V_set: Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;break;     //���ڵ�ѹ�趨���棬������ʵֵ����
				case Display_Interface_Mode_Main_Interface_I_set: Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;break;     //���ڵ����趨���棬������ʵֵ����
				case Display_Interface_Mode_Main_Interface_P_set: Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;break;     //���ڹ����趨���棬������ʵֵ����
		  }
		}
	}
}

 /**
  * @brief  ����V-set��ɨ�������
  * @param	None
  * @retval None
  */
void Key_V_set(void)
{
  if((LOCK_UNLOCK == UNLOCK) && ((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Actual) 
	   || (Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Preset)))           //ȷ�ϰ�����İ����ˡ�����������״̬������������
	{
		if(Shift_Not_Shift == Not_Shift)                                              //�޸��ã������ѹ�趨ģʽ
		{
			Keys_Encoder_Mode = Encoder_Mode;                                           //Ĭ���ñ���������
			
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_V_set;       //�����趨��ѹģʽ
			
			Cursor_Position = 4;                                                        //����λ�����λ
			
			if(Last_Interface != Display_Interface_Mode)                                //����
	    {
		    Fill_RAM(0x00);
	    }
			
			Display_Main_Interface_V_set(Set_Current, Set_Power, ON_OFF, CV_CC_CP,      //��ʾ�趨��ѹ����
			                             DELAY_NOT_DELAY, Preset, LOCK_UNLOCK, Use_Vint_Or_Vext_Flag);
	
				Display_Main_Interface_Voltage(Set_Voltage);                              //��ʾ�趨��ѹ��������תΪȫ���ַ���
			
		  Last_Interface = Display_Interface_Mode_Main_Interface_V_set;
			
      TIM_Cmd(TIM5, ENABLE);                                                      //ʹ�ܶ�ʱ�������ֹ��
		}
		
		
		else if(Shift_Not_Shift == Shift)                                             //���ã�����˵��趨ģʽ
		{
			Display_Interface_Mode = Display_Interface_Mode_First_Menu;                 //��ʾ�˵�����
			
			Shift_Not_Shift = Not_Shift;                                                //ȡ������
		}
	}
}

 /**
  * @brief  ����I-set��ɨ�������
  * @param	None
  * @retval None
  */
void Key_I_set(void)
{
  if((LOCK_UNLOCK == UNLOCK) && ((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Actual) 
	   || (Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Preset)))           //ȷ�ϰ�����İ����ˡ�����������״̬������������״̬������������
	{
		if(Shift_Not_Shift == Not_Shift)                                 //�޸��ã������������ģʽ
		{
			Keys_Encoder_Mode = Encoder_Mode;                                           //Ĭ���ñ���������
			
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_I_set;       //�����趨����ģʽ
			
			Cursor_Position = 5;                                                        //����λ�����λ
			
			if(Last_Interface != Display_Interface_Mode)                                //����
	    {
		    Fill_RAM(0x00);
	    }
			
			Display_Main_Interface_I_set(Set_Voltage, Set_Power, ON_OFF, CV_CC_CP,      //��ʾ�趨��������
			                             DELAY_NOT_DELAY, Preset, LOCK_UNLOCK, Use_Vint_Or_Vext_Flag);
	
			Display_Main_Interface_Current(Set_Current);                                //��ʾ�趨������������תΪȫ���ַ���
			
		  Last_Interface = Display_Interface_Mode_Main_Interface_I_set;               //���һ����ʾ��Ϊ����ģʽ
			
      TIM_Cmd(TIM5, ENABLE);                                                      //ʹ�ܶ�ʱ�������ֹ��
		}

    else if(Shift_Not_Shift == Shift)                            //���ã����빦������ģʽ
		{
			Shift_Not_Shift = Not_Shift;            //ȡ������
			
			Keys_Encoder_Mode = Encoder_Mode;                                           //Ĭ���ñ���������
			
			Display_Interface_Mode = Display_Interface_Mode_Main_Interface_P_set;       //�����趨����ģʽ
			
			Cursor_Position = 4;                                                        //����λ�����λ				

			if(Last_Interface != Display_Interface_Mode)                                //����
	    {
		    Fill_RAM(0x00);
	    }
			
			Display_Main_Interface_P_set(Set_Voltage, Set_Current, ON_OFF, CV_CC_CP,      //��ʾ�趨���ʽ���
			                             DELAY_NOT_DELAY, Preset, LOCK_UNLOCK, Use_Vint_Or_Vext_Flag);
	
			Display_Main_Interface_Power(Set_Power);                                    //��ʾ�趨���ʣ�������תΪȫ���ַ���
			
			if((Set_Power >= 1000) || (Set_Power < 10))
			{
				String_Power[0] = '0';
				Write_Single_12x24AsicII(42, 0, '0');
			}
			
		  Last_Interface = Display_Interface_Mode_Main_Interface_P_set;               //���һ����ʾ��Ϊ����ģʽ
			
      TIM_Cmd(TIM5, ENABLE);                                                      //ʹ�ܶ�ʱ�������ֹ��			
		}			
	}
}

 /**
  * @brief  ����Up��ɨ�������
  * @param	None
  * @retval None
  */
void Key_Up(void)
{
  if(LOCK_UNLOCK == UNLOCK)                     //ȷ�ϰ����Ѿ����ºʹ���������״̬
	{
		if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_V_set) && (Keys_Encoder_Mode == Encoder_Mode))  //���ڵ�ѹ���ý���ͱ�����ģʽ����һ�£���ָ����ǰ��1
		{
			Write_Single_16x32AsicII(0, Cursor_Position*4, String_Voltage[Cursor_Position]);
			
			Write_Single_16x32AsicII(0, 20, 'V');
			
			Cursor_Position --;
			
			if(Cursor_Position == -1)          //��곬����Χ
			{
				Cursor_Position = 4;
			}
			else if(String_Voltage[Cursor_Position] == '.')     //�������С����
			{
				Cursor_Position --;
			}
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_I_set) && (Keys_Encoder_Mode == Encoder_Mode))  //���ڵ������ý���ͱ�����ģʽ����һ�£���ָ����ǰ��1
		{
			Write_Single_16x32AsicII(0, Cursor_Position*4 + 36, String_Current[Cursor_Position]);
			
			Write_Single_16x32AsicII(0, 60, 'A');
			
			Cursor_Position --;
			
			if(Cursor_Position == -1)    //��곬����Χ
			{
				Cursor_Position = 5;
			}
			else if(String_Current[Cursor_Position] == '.')     //�������С����
			{
				Cursor_Position --;
			}		
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_P_set) && (Keys_Encoder_Mode == Encoder_Mode))   //���ڹ������ý���ͱ�����ģʽ����һ�£���ָ����ǰ��1
		{
			Write_Single_12x24AsicII(42, Cursor_Position*3, String_Power[Cursor_Position]);
			
			Write_Single_12x24AsicII(42, 15, 'W');
			
			Cursor_Position --;  //�����λ
			
			if((String_To_Float(String_Power) >= 1000) && (Cursor_Position == 0))
			{
				Cursor_Position --;
			}

			if(Cursor_Position == -1)
			{
			  Cursor_Position = 4;     //��곬����Χ				
			}
			
			if(String_Power[Cursor_Position] == '.')     //�������С����
			{
				Cursor_Position --;
			}
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Recall_Interface) || (Display_Interface_Mode == Display_Interface_Mode_Save_Interface))   //���ڴ�ȡģʽ
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
		
		else if(Display_Interface_Mode == Display_Interface_Mode_First_Menu)   //���ڵ�һ��ѡ����棬����ʾ���ݶ�Ӧ�ı�
		{
			First_Selection = First_Selection + 1;
			if(First_Selection == 6)
			{
				First_Selection = 0;
			}
		}
		
		else if(Display_Interface_Mode == Display_Interface_Mode_Second_Menu)   //���ڵڶ����˵����棬����ʾ���ݶ�Ӧ�ı�
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
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == RS232) && (RS232_Set_ID_BaudRate == RS232_Set_ID) && (Keys_Encoder_Mode == Encoder_Mode))    //����RS232��ID
		{
			Write_Single_16x32AsicII(0, 12 + Cursor_Position*4, String_RS232_ID[Cursor_Position]);
			
			Cursor_Position --;
			
			if(Cursor_Position == -1)
			{
			  Cursor_Position = 1;     //��곬����Χ				
			}
		}
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == RS232) && (RS232_Set_ID_BaudRate == RS232_Set_BaudRate))    //����RS232�Ĳ�����
		{
			
			RS232_BaudRate_State ++;
			
			switch (RS232_BaudRate_State)                                                                    //��ʾ�趨ֵ
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
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == CAN) && (CAN_Set_ID_BaudRate == CAN_Set_ID) && (Keys_Encoder_Mode == Encoder_Mode))    //����CAN��ID
		{
			Write_Single_16x32AsicII(0, 12 + Cursor_Position*4, String_CAN_ID[Cursor_Position]);
			
			Cursor_Position --;
			
			if(Cursor_Position == -1)
			{
			  Cursor_Position = 1;     //��곬����Χ				
			}
		}
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == CAN) && (CAN_Set_ID_BaudRate == CAN_Set_BaudRate))    //����CAN�Ĳ�����
		{
			
			CAN_BaudRate_State ++;
			
			switch (CAN_BaudRate_State)                                                                    //��ʾ�趨ֵ
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
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Cycle))  //�����ڵڶ����˵���Cycle
		{
			if ((Cycle_Enter == Cycle_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //�������ÿ�����
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
			
			else if ((Cycle_Enter == Cycle_Enter_2) && (Keys_Encoder_Mode == Encoder_Mode))       //�������ÿ�����
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
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == V_Rise))  //�����ڵڶ����˵���V_Rise
		{
			if ((V_Rise_Enter == V_Rise_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //�������ÿ�����
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
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == V_Fall))  //�����ڵڶ����˵���V_Fall
		{
			if ((V_Fall_Enter == V_Fall_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //�������ÿ�����
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
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == I_Rise))  //�����ڵڶ����˵���I_Rise
		{
			if ((I_Rise_Enter == I_Rise_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //�������ÿ�����
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
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == I_Fall))  //�����ڵڶ����˵���I_Fall
		{
			if ((I_Fall_Enter == I_Fall_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //�������ÿ�����
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
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Delay))  //�����ڵڶ����˵���Delay
		{			
			if ((Delay_Enter == Delay_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))     //�������һ�ΰ��������ڱ�����ģʽ 
			{
			  Write_Single_16x32AsicII(32, (14 + Cursor_Position * 4), String_Delay[Cursor_Position]);
			
			  Cursor_Position --;
			
			  if(Cursor_Position == 5)
		  	{
		  	  Cursor_Position = 4;     //��곬����Χ				
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
  * @brief  ����Down��ɨ�������
  * @param	None
  * @retval None
  */
void Key_Down(void)
{
  if(LOCK_UNLOCK == UNLOCK)                     //ȷ�ϰ����Ѿ����ºʹ���������״̬
	{
		if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_V_set) && (Keys_Encoder_Mode == Encoder_Mode))  //���ڵ�ѹ���ý���ͱ�����ģʽ����һ�£���ָ������ƶ�1
		{
			Write_Single_16x32AsicII(0, Cursor_Position*4, String_Voltage[Cursor_Position]);
			
			Write_Single_16x32AsicII(0, 20, 'V');			
			
			Cursor_Position ++;

			if(Cursor_Position == 5)          //��곬����Χ
			{
				Cursor_Position = 0;
			}
			else if(String_Voltage[Cursor_Position] == '.')     //�������С����
			{
				Cursor_Position ++;
			}
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_I_set) && (Keys_Encoder_Mode == Encoder_Mode))  //���ڵ������ý���ͱ�����ģʽ����һ�£���ָ������ƶ�1
		{
			Write_Single_16x32AsicII(0, Cursor_Position*4 + 36, String_Current[Cursor_Position]);
			
			Write_Single_16x32AsicII(0, 60, 'A');			
			
			Cursor_Position ++;
			if(Cursor_Position == 6)    //��곬����Χ
			{
				Cursor_Position = 0;
			}
			else if(String_Current[Cursor_Position] == '.')     //�������С����
			{
				Cursor_Position ++;
			}		
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_P_set) && (Keys_Encoder_Mode == Encoder_Mode))   //���ڹ������ý���ͱ�����ģʽ����һ�£���ָ������ƶ�1
		{
			Write_Single_12x24AsicII(42, Cursor_Position*3, String_Power[Cursor_Position]);
			
			Write_Single_12x24AsicII(42, 15, 'W');		
			
			Cursor_Position ++;  //�����λ

			if(Cursor_Position == 5)
			{
			  Cursor_Position = 0;     //��곬����Χ				
			}
			
			if((String_To_Float(String_Power) >= 1000) && (Cursor_Position == 0))
			{
				Cursor_Position ++;
			}
			
			if(String_Power[Cursor_Position] == '.')     //�������С����
			{
				Cursor_Position ++;
			}
		}
		
		else if((Display_Interface_Mode == Display_Interface_Mode_Recall_Interface) || (Display_Interface_Mode == Display_Interface_Mode_Save_Interface))   //���ڴ�ȡģʽ
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
		
		else if(Display_Interface_Mode == Display_Interface_Mode_First_Menu)   //���ڵ�һ��ѡ����棬����ʾ���ݶ�Ӧ�ı�
		{
			First_Selection = First_Selection - 1;
			if(First_Selection == -1)
			{
				First_Selection = 5;
			}
		}
		
		else if(Display_Interface_Mode == Display_Interface_Mode_Second_Menu)   //���ڵڶ����˵����棬����ʾ���ݶ�Ӧ�ı�
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
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == RS232) && (RS232_Set_ID_BaudRate == RS232_Set_ID) && (Keys_Encoder_Mode == Encoder_Mode))    //����RS232��ID
		{
			Write_Single_16x32AsicII(0, 12 + Cursor_Position*4, String_RS232_ID[Cursor_Position]);
			
			Cursor_Position ++;
			
			if(Cursor_Position == 2)
			{
			  Cursor_Position = 0;     //��곬����Χ				
			}
		}

    else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == RS232) && (RS232_Set_ID_BaudRate == RS232_Set_BaudRate))    //����RS232�Ĳ�����
		{
			
			RS232_BaudRate_State --;
			
			switch (RS232_BaudRate_State)                                                                   //��ʾ�趨ֵ
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
		
    else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == CAN) && (CAN_Set_ID_BaudRate == CAN_Set_ID) && (Keys_Encoder_Mode == Encoder_Mode))    //����CAN��ID
		{
			Write_Single_16x32AsicII(0, 12 + Cursor_Position*4, String_CAN_ID[Cursor_Position]);
			
			Cursor_Position ++;
			
			if(Cursor_Position == 2)
			{
			  Cursor_Position = 0;     //��곬����Χ				
			}
		}

    else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == CAN) && (CAN_Set_ID_BaudRate == CAN_Set_BaudRate))    //����CAN�Ĳ�����
		{
			
			CAN_BaudRate_State --;
			
			switch (CAN_BaudRate_State)                                                                   //��ʾ�趨ֵ
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
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == V_Rise))  //�����ڵڶ����˵���V_Rise
		{
			if ((V_Rise_Enter == V_Rise_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //�������ÿ�����
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
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == V_Fall))  //�����ڵڶ����˵���V_Fall
		{
			if ((V_Fall_Enter == V_Fall_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //�������ÿ�����
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
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == I_Rise))  //�����ڵڶ����˵���I_Rise
		{
			if ((I_Rise_Enter == I_Rise_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //�������ÿ�����
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
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == I_Fall))  //�����ڵڶ����˵���I_Fall
		{
			if ((I_Fall_Enter == I_Fall_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //�������ÿ�����
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
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Cycle))  //�����ڵڶ����˵���Cycle
		{
			if ((Cycle_Enter == Cycle_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))       //�������ÿ�����
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
			
			
			if ((Cycle_Enter == Cycle_Enter_2) && (Keys_Encoder_Mode == Encoder_Mode))       //�������ÿ�����
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
		
		else if ((Display_Interface_Mode == Display_Interface_Mode_Third_Menu) && (Second_Selection == Delay))  //�����ڵڶ����˵���Delay
		{			
			if ((Delay_Enter == Delay_Enter_1) && (Keys_Encoder_Mode == Encoder_Mode))      
			{
			  Write_Single_16x32AsicII(32, (14 + Cursor_Position * 4), String_Delay[Cursor_Position]);
			
				//�Ĺ��λ��
			  Cursor_Position ++;
			
			  if(Cursor_Position == 2)
		  	{
		  	  Cursor_Position = 3;     //��곬����Χ				
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
  * @brief  ����ON/OFF��ɨ�������
  * @param	None
  * @retval None
  */
void Key_ON_OFF(void)
{
	if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Actual) || (Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Preset))    //ȷ�ϰ����Ѿ�����,������ʵֵ���趨ֵ����
	{
		if(OUTPUT_NOT_OUTPUT == NOT_OUTPUT)
		{
			OUTPUT_NOT_OUTPUT = OUTPUT;
			if(Delay_Function_On_Off == Delay_Function_On)               //������ʱ����
			{
				TIM_DELAY_ON();
			}
			else if(Cycle_Function_On_Off == Cycle_Function_On)          //����ѭ������
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
  * @brief  ����Preset/Lock��ɨ�������
  * @param	None
  * @retval None
  */
void Key_Preset_Lock(void)
{
	if((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Actual) 
	   || (Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Preset))                     //ȷ�ϰ����Ѿ����¡�������������
	{
		if(Shift_Not_Shift == Not_Shift)                     //�޸��ã�ΪԤ��ֵ��ʵֵ�趨��ť
		{
			if(Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Preset)                //֮ǰ���趨ֵ
			{
				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;   //��ʾ��ʵֵ
			}
			else if(Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Actual)          //֮ǰ����ʵֵ
			{
				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Preset;                 //��ʾ�趨ֵ
			}
			else
			{
				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Preset;
			}
		}
		
		else if(Shift_Not_Shift == Shift)           //���ã�Ϊ������ť
		{
			if(LOCK_UNLOCK == UNLOCK)         //���û������������,������������
			{
				LOCK_UNLOCK = LOCK;
				Display_Interface_Mode = Display_Interface_Mode_Main_Interface_Actual;
			}
			else if(LOCK_UNLOCK == LOCK)      //��������ˣ��ͽ���
			{
				LOCK_UNLOCK = UNLOCK;
			}
			Shift_Not_Shift = Not_Shift;
		}
	}
}

 /**
  * @brief  ����Recall/Save��ɨ�������
  * @param	None
  * @retval None
  */
void Key_Recall_Save(void)
{
	if((LOCK_UNLOCK == UNLOCK) && ((Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Actual) 
	   || (Display_Interface_Mode == Display_Interface_Mode_Main_Interface_Preset)))                     //ȷ�ϰ����Ѿ����¡�����������״̬������������
	{
		if(Shift_Not_Shift == Not_Shift)          //�޸��ã�����ȡ����
		{
			Recall_Save_Number = 0;
			Recall_Save_Display_Voltage = Recall_Save_Voltage[Recall_Save_Number];
			Recall_Save_Display_Current = Recall_Save_Current[Recall_Save_Number];
			Recall_Save_Display_Power = Recall_Save_Power[Recall_Save_Number];
			Display_Interface_Mode = Display_Interface_Mode_Recall_Interface;			
		}
		
		else if(Shift_Not_Shift == Shift)         //���ã����ڴ����
		{
			Shift_Not_Shift = Not_Shift;              //ȡ������
			Recall_Save_Number = 0;
			Recall_Save_Display_Voltage = Recall_Save_Voltage[Recall_Save_Number];
			Recall_Save_Display_Current = Recall_Save_Current[Recall_Save_Number];
			Recall_Save_Display_Power = Recall_Save_Power[Recall_Save_Number];
			Display_Interface_Mode = Display_Interface_Mode_Save_Interface;				
		}
	}		
}

//////u16 text;
//////u8 Key_Flag;    //��ֹ����ı���
////// /**
//////  * @brief  ���������������ж����У��ٵ��ð���������
//////  * @param	None
//////  * @retval None
//////  */
//////void Keys_Handler(void)
//////{
//////	SysTick_Delay_Ms(10);    //��ʱ�üĴ������㹻��ʱ�䷴Ӧ����
//////	
//////	if((KeyCol_Status == 0x7f) && (Key_Flag == 1))
//////	{
//////		Key_Flag = 0;
//////	}

//////	if((KeyCol_Status != 0x7f) && (Key_Flag == 0))    //��⵽�а���������
//////	{
//////		SysTick_Delay_Ms(15);    //��ʱȷ����İ�����
//////		if(KeyCol_Status != 0x7F)
//////		{
//////			Key_Flag = 1;
//////			switch(KeyCol_Status)
//////			{
//////				case 0x3f:           //��һ�а�����
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_1_Pin);     //����һ��
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_1_Pin) == 1)    //1��-1�а���������
//////					{
//////					  Key_1();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////					
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_2_Pin);     //���ڶ���
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_1_Pin) == 1)    //2��-1�а���������
//////					{
//////					  Key_4();					
//////            GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);						
//////						return;	
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////				
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_3_Pin);     //��������
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_1_Pin) == 1)    //3��-1�а���������
//////					{
//////						Key_7();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////					break;
//////					
//////				case 0x5f:           //�ڶ��а�����
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_1_Pin);     //����һ��
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_2_Pin) == 1)    //1��-2�а���������
//////					{
//////						Key_2();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////					
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_2_Pin);     //���ڶ���
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_2_Pin) == 1)    //2��-2�а���������
//////					{
//////						Key_5();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////						return;	
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////				
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_3_Pin);     //��������
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_3_Pin) == 1)    //3��-3�а���������
//////					{
//////						Key_8();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);	
//////					break;

//////				case 0x6f:           //�����а�����
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_1_Pin);     //����һ��
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_3_Pin) == 1)    //1��-3�а���������
//////					{
//////						Key_3();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////					
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_2_Pin);     //���ڶ���
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_3_Pin) == 1)    //2��-3�а���������
//////					{
//////						Key_6();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////						return;	
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////				
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_3_Pin);     //��������
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_3_Pin) == 1)    //3��-3�а���������
//////					{
//////						Key_9();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////					break;

//////				case 0x77:           //�����а�����
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_1_Pin);     //����һ��
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_4_Pin) == 1)    //1��-4�а���������
//////					{
//////						Key_ESC();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////					
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_2_Pin);     //���ڶ���
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_4_Pin) == 1)    //2��-4�а���������
//////					{
//////						Key_0();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////						return;	
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////				
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_3_Pin);     //��������
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_4_Pin) == 1)    //3��-4�а���������
//////					{
//////						Key_Decimal_Point();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////					break;

//////				case 0x7b:           //�����а�����
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_1_Pin);     //����һ��
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_5_Pin) == 1)    //1��-5�а���������
//////					{
//////						Key_V_set();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////					
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_2_Pin);     //���ڶ���
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_5_Pin) == 1)    //2��-5�а���������
//////					{
//////						Key_Recall_Save();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////						return;	
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////				
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_3_Pin);     //��������
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_5_Pin) == 1)    //3��-5�а���������
//////					{
//////						Key_Shift();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////					break;
//////					
//////				case 0x7d:           //�����а�����
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_1_Pin);     //����һ��
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_6_Pin) == 1)    //1��-6�а���������
//////					{
//////						Key_I_set();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////					
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_2_Pin);     //���ڶ���
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_6_Pin) == 1)    //2��-6�а���������
//////					{
//////						Key_Preset_Lock();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////						return;	
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////				
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_3_Pin);     //��������
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_6_Pin) == 1)    //3��-6�а���������
//////					{
//////						Key_ON_OFF();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);
//////          break;					
//////					
//////				case 0x7e:           //�����а�����
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_1_Pin);     //����һ��
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_7_Pin) == 1)    //1��-7�а���������
//////					{
//////						Key_Up();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////						return;
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);
//////					
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_2_Pin);     //���ڶ���
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_7_Pin) == 1)    //2��-7�а���������
//////					{
//////						Key_Enter();
//////						GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////						return;	
//////					}
//////					GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);
//////				
//////				  GPIO_SetBits(KeyRow_Port, KeyRow_3_Pin);     //��������
//////				  SysTick_Delay_Ms(5);
//////				  if(GPIO_ReadInputDataBit(KeyCol_Port, KeyCol_7_Pin) == 1)    //3��-7�а���������
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
u8 Key_Flag;    //��ֹ����ı���
 /**
  * @brief  ���������������ж����У��ٵ��ð���������
  * @param	None
  * @retval None
  */
void Keys_Handler(FunctionalState keys_EN)
{
	if(keys_EN != ENABLE) return;
	GPIO_SetBits(KeyRow_Port, KeyRow_1_Pin);    //�����и�
	GPIO_SetBits(KeyRow_Port, KeyRow_2_Pin);
	GPIO_SetBits(KeyRow_Port, KeyRow_3_Pin);	
	
	
	GPIO_ResetBits(KeyRow_Port, KeyRow_1_Pin);     //��һ��
	SysTick_Delay_Ms(5);
	
	if(KeyCol_Status == 0x7f && Key_Flag == 1)    
	{
		Key_Flag = 0;                //������־λ��λ             
	}
	
	if(Key_Flag == 0)     //ֻ�б�־λΪ0���ܴ���
	{
		switch(KeyCol_Status)
	  {         
		  case 0x3f:                        //��һ�а����� ��1��
	    {
		    Key_1();
			  Key_Flag = 1;
			  break;
		  }
		  case 0x5f:                   //�ڶ��а����ˡ�2��
		  {
			  Key_2();
			  Key_Flag = 1;
			  break;			
		  }
		  case 0x6f:                 //�����а����ˡ�3��
		  {
			  Key_3();
			  Key_Flag = 1;
			  break;				
		  }
		  case 0x77:                 //�����а����ˡ�ESC��
		  {
			  Key_ESC();
			  Key_Flag = 1;
			  break;				
		  }
		  case 0x7b:            //�����а����ˡ�V-set��
		  {
			  Key_V_set();
			  Key_Flag = 1;
			  break;				
		  }
		  case 0x7d:              //�����а����ˡ�I-set��
		  {
			  Key_I_set();
			  Key_Flag = 1;
			  break;			
		  }
		  case 0x7e:               //�����а����ˡ�UP��
		  {
			  Key_Up();
			  Key_Flag = 1;
			  break;			
		  }
		  default: 			
        break;                 //û�а���
	  }
	}
	GPIO_SetBits(KeyRow_Port, KeyRow_1_Pin);	

	
	
	
	GPIO_ResetBits(KeyRow_Port, KeyRow_2_Pin);     //�ڶ���
	SysTick_Delay_Ms(5);
	
	if(KeyCol_Status == 0x7f && Key_Flag == 2)    
	{
		Key_Flag = 0;                //������־λ��λ             
	}	
	if(Key_Flag == 0)
	{
		switch(KeyCol_Status)
	  {              	
		  case 0x3f:                        //��һ�а����� ��4��
	    {
		    Key_4();
			  Key_Flag = 2;
			  break;
		  }
		  case 0x5f:                   //�ڶ��а����ˡ�5��
		  {
			  Key_5();
		  	Key_Flag = 2;
			  break;			
		  }
		  case 0x6f:                 //�����а����ˡ�6��
		  {
			  Key_6();
			  Key_Flag = 2;
			  break;				
		  }
		  case 0x77:                 //�����а����ˡ�0��
		  {
			  Key_0();
			  Key_Flag = 2;
			  break;				
		  }
		  case 0x7b:            //�����а����ˡ�RECALL��
		  {
			  Key_Recall_Save();
			  Key_Flag = 2;
			  break;				
		  }
		  case 0x7d:              //�����а����ˡ�Preset��
		  {
			  Key_Preset_Lock();
			  Key_Flag = 2;
			  break;			
		  }
		  case 0x7e:               //�����а����ˡ�ENTER��
		  {
			  Key_Enter();
			  Key_Flag = 2;
			  break;			
		  }
		  default: 
        break;                 //û�а���			
	  }
	}
	GPIO_SetBits(KeyRow_Port, KeyRow_2_Pin);	

	
	
	
	GPIO_ResetBits(KeyRow_Port, KeyRow_3_Pin);     //������
	SysTick_Delay_Ms(5);
	
	if(KeyCol_Status == 0x7f && Key_Flag == 3)    
	{
		Key_Flag = 0;                //������־λ��λ             
	}	
	if(Key_Flag == 0)
	{
		switch(KeyCol_Status)
	  {
		  case 0x3f:                        //��һ�а����� ��7��
	    {
		    Key_7();
				Key_Flag = 3;
			  break;
		  }
		  case 0x5f:                   //�ڶ��а����ˡ�8��
		  {
			  Key_8();
				Key_Flag = 3;
			  break;			
		  }
		  case 0x6f:                 //�����а����ˡ�9��
		  {
			  Key_9();
				Key_Flag = 3;
			  break;				
		  }
		  case 0x77:                 //�����а����ˡ�.��
		  {
			  Key_Decimal_Point();
				Key_Flag = 3;
			  break;				
		  }
		  case 0x7b:            //�����а����ˡ�shift��
		  {
			  Key_Shift();
				Key_Flag = 3;
			  break;				
		  }
		  case 0x7d:              //�����а����ˡ�on��
		  {
			  Key_ON_OFF();
				Key_Flag = 3;
			  break;			
		  }
		  case 0x7e:               //�����а����ˡ�down��
		  {
			  Key_Down();
				Key_Flag = 3;
			  break;			
		  }
		  default:
        break;                 //û�а���			
	  }	
	}	
  GPIO_SetBits(KeyRow_Port, KeyRow_3_Pin);
}





