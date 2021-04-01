#include "stm32f10x.h"
#include "can.h"

uint8_t CAN1_ReceiveData[10];
uint8_t CAN1_SendData[10];
uint8_t CAN1_Need_To_Send_Num;
//uint16_t  WireRES;
//uint8_t  WireRES_OnOFF;

 /**
  * @brief  CANͨ�ų�ʼ��
  * @param  BraudRate-������ ��λkbps
  * @retval ��
  */
void CAN_Config(uint16_t BraudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	uint8_t CAN_SJW;         
	uint8_t CAN_BS1;          
	uint8_t CAN_BS2; 		
	uint16_t CAN_Prescaler;	    // ����һ��Tq��ʱ�䳤��

	 switch(BraudRate)           // ������  1s�������λ
	{
		case 5:                       //��λΪkbps     Ӧ����5000bps��д��5kbps    ����Ӧ�ö���Ҫ����1000   Ӧ���д�T1bit=11���������������
//			CAN_SJW = CAN_SJW_2tq;
//		    CAN_BS1= CAN_BS1_6tq;
//		    CAN_BS2= CAN_BS2_4tq;
//		    CAN_Prescaler = 600;	
//		
				CAN_SJW = CAN_SJW_1tq;
		    CAN_BS1= CAN_BS1_3tq;
		    CAN_BS2= CAN_BS2_2tq;
		    CAN_Prescaler = 1200;	
			break;
		case 10:
			CAN_SJW = CAN_SJW_1tq;
		    CAN_BS1= CAN_BS1_3tq;
		    CAN_BS2= CAN_BS2_2tq;
            CAN_Prescaler = 600;		
		    break;
		case 20:
			CAN_SJW = CAN_SJW_1tq;
		    CAN_BS1= CAN_BS1_3tq;
		    CAN_BS2= CAN_BS2_2tq;
            CAN_Prescaler = 300;		
		    break;
		case 25:
			CAN_SJW = CAN_SJW_1tq;
		    CAN_BS1= CAN_BS1_3tq;
		    CAN_BS2= CAN_BS2_2tq;
            CAN_Prescaler = 240;		
			break;
		case 40:
			CAN_SJW = CAN_SJW_1tq;
		    CAN_BS1= CAN_BS1_3tq;
		    CAN_BS2= CAN_BS2_2tq;
		    CAN_Prescaler =150;
			break;
		case 50:
			CAN_SJW = CAN_SJW_1tq;
		    CAN_BS1= CAN_BS1_3tq;
		    CAN_BS2= CAN_BS2_2tq;
		    CAN_Prescaler =120;
			break;
		case 80:
			CAN_SJW = CAN_SJW_1tq;
		    CAN_BS1= CAN_BS1_3tq;
		    CAN_BS2= CAN_BS2_2tq;
		    CAN_Prescaler =75;
			break;
		case 100:
			CAN_SJW = CAN_SJW_1tq;
		    CAN_BS1= CAN_BS1_3tq;
		    CAN_BS2= CAN_BS2_2tq;
		    CAN_Prescaler =60;
			break;
		case 125:
			  CAN_SJW = CAN_SJW_1tq;
		    CAN_BS1= CAN_BS1_3tq;
		    CAN_BS2= CAN_BS2_2tq;
		    CAN_Prescaler =48;
			break;
		case 200:
			CAN_SJW = CAN_SJW_1tq;
		    CAN_BS1= CAN_BS1_3tq;
		    CAN_BS2= CAN_BS2_2tq;
		    CAN_Prescaler =30;
			break;
		case 250:
			CAN_SJW = CAN_SJW_1tq;
		    CAN_BS1= CAN_BS1_3tq;
		    CAN_BS2= CAN_BS2_2tq;
		    CAN_Prescaler =24;
			break;
		case 400:
			CAN_SJW = CAN_SJW_1tq;
		    CAN_BS1= CAN_BS1_3tq;
		    CAN_BS2= CAN_BS2_2tq;
		    CAN_Prescaler =10;
			break;
		case 500:
			CAN_SJW = CAN_SJW_1tq;
		    CAN_BS1= CAN_BS1_3tq;
		    CAN_BS2= CAN_BS2_2tq;
		    CAN_Prescaler =12;
			break;
		case 800:CAN_SJW = CAN_SJW_1tq;
		    CAN_BS1= CAN_BS1_3tq;
		    CAN_BS2= CAN_BS2_2tq;
		    CAN_Prescaler =5;
			break;
	}
	
	
	
	
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	//ʹ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig( GPIO_Remap2_CAN1,ENABLE);     //CAN1���ú���ӳ��
	
	
	//CAN1_RX
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;
	GPIO_InitStructure .GPIO_Speed =GPIO_Speed_50MHz ;
	GPIO_InitStructure .GPIO_Mode =GPIO_Mode_IPU;  //����,���ε�ƽ
	GPIO_Init(GPIOD,&GPIO_InitStructure );
	
	//CAN1_TX
	GPIO_InitStructure .GPIO_Pin =GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz ;
	GPIO_InitStructure .GPIO_Mode =GPIO_Mode_AF_PP ;
	GPIO_Init(GPIOD ,&GPIO_InitStructure );


		CAN_InitStructure.CAN_TTCM=DISABLE;	//?��ֹʱ�䴥��ͨ��ģʽ  
  	CAN_InitStructure.CAN_ABOM=ENABLE;	//�Զ����߹���
  	CAN_InitStructure.CAN_AWUM=ENABLE;//�Զ�����
  	CAN_InitStructure.CAN_NART=DISABLE;	//�ر��Զ��ش�     �����Ҫע�⣿��    //ʹ���Զ��ش�����һֱ���ͱ���ֱ���ɹ�Ϊֹ
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//�������ʱ����
  	CAN_InitStructure.CAN_TXFP=DISABLE;	  //���ձ���ID�����ȼ�������
  	CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;    //����ģʽ  

  	CAN_InitStructure.CAN_SJW=CAN_SJW ;	
  	CAN_InitStructure.CAN_BS1=CAN_BS1;
  	CAN_InitStructure.CAN_BS2=CAN_BS2;
  	CAN_InitStructure.CAN_Prescaler=CAN_Prescaler;  //100k
  	CAN_Init(CAN1, &CAN_InitStructure); 
	
	
	//����������	
	CAN_FilterInitStructure.CAN_FilterNumber=0;	     //һ��0-27��28��ɸѡ����ѡ���0��
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; //����λģʽ    ����������ģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit; ////ɸѡ���ĳ߶ȣ�  ���16λ
	CAN_FilterInitStructure.CAN_FilterIdHigh=0;   //���ڴ���Ҫɸѡ��ID��16λģʽ�������������Ҫɸѡ��ID
	CAN_FilterInitStructure.CAN_FilterIdLow=0;       //���ڴ���Ҫɸѡ��ID��16λ�������������Ҫɸѡ��ID
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;    //�������IdHigh��Ա��Ӧ�����룬�� CAN_FilterIdHigh ���һ��ɸѡ��
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0; //����ɸѡ�����ݴ��浽�ĸ����ܵ�FIFO��ɸѡ����������FIFIO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //�Ƿ񼤻����ɸѡ��
	CAN_FilterInit(&CAN_FilterInitStructure);//
	
	
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn ;//CAN1 �����ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;    
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;       
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	CAN_ITConfig (CAN1,CAN_IT_FMP0,ENABLE);//�����ж�ʹ��       FMP0   FIFO 0��Ϣ�����жϣ���FIFO0���ܵ�����ʱ�������ж�

}


//	uint8_t state = 5;
 /**
  * @brief  CAN���ͱ���
  * @param ��
  * @retval ��
  */
void CAN1_Send(void)
{
	uint8_t i;
//	uint8_t TransmitMailbox;
	
	CanTxMsg TxMessage; 
	TxMessage.StdId =0;      // ������Ǳ��ĵ�11λ��׼��ʶ��
	TxMessage.IDE =CAN_ID_STD;     //�������Ǳ�׼֡��ʹ��StdId��Ա���汨��ID
	TxMessage.RTR =CAN_RTR_DATA;     //����֡
	TxMessage.DLC =CAN1_Need_To_Send_Num;  //���汨�����ݶεĳ���
	for(i=0;i < CAN1_Need_To_Send_Num;i++)
	{
		TxMessage.Data[i]=CAN1_SendData[i];
	}
	
	   
	//��ʼ�������� û�п����䷵��CAN_TxStatus_NoMailBox
	while(CAN_Transmit (CAN1 ,&TxMessage)==CAN_TxStatus_NoMailBox);

//	state = CAN_TransmitStatus(CAN1, TransmitMailbox);
}


/**
  * @brief  ����������0x20
  * @param  ��
  * @retval ��
  */
static void CAN_function20(void)
{

	CAN1_SendData [0]=(u8)CAN_ID;
	CAN1_SendData [1]=0x20;     //0010 0000
	CAN1_SendData [2]=0x01;     //0000 0001


	if(CAN1_ReceiveData[3]==ON) 
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
	else
	{
		OUTPUT_NOT_OUTPUT = NOT_OUTPUT;
		TIM_DELAY_OFF();
		TIM_CYCLE_OFF();
		Power_Off();
	}
	
	CAN1_SendData [3]=CAN1_ReceiveData[3];
    
	CAN1_Need_To_Send_Num =4;
	CAN1_Send ();		
}

/**
  * @brief  �趨�����ѹ���0x21
  * @param  ��
  * @retval ��
  */
static void CAN_function21(void)
{

	CAN1_SendData [0]=(u8)CAN_ID;//����ID
	CAN1_SendData [1]=0x21;//ָ������    //0010 0001
	CAN1_SendData [2]=0x02;      //0000 0010
	
	//�趨�����ѹ
	Set_Voltage=(CAN1_ReceiveData[3]<<8|CAN1_ReceiveData[4])/100.0;    
	CV_CC_CP = CV;
	AT25_Save_VIPSet();
	AT25_Save_CC_CV_CP();
	
	CAN1_SendData [3]=CAN1_ReceiveData[3];
	CAN1_SendData [4]=CAN1_ReceiveData[4];
	

	CAN1_Need_To_Send_Num =5;	
	CAN1_Send ();			


}


/**
  * @brief  �趨����������0x22
  * @param  ��
  * @retval ��
  */
static void CAN_function22(void)
{

	CAN1_SendData [0]=(u8)CAN_ID ;
	CAN1_SendData [1]=0x22;
	CAN1_SendData [2]=0x02;
	
	//�趨�������
    Set_Current=(CAN1_ReceiveData[3]<<8|CAN1_ReceiveData[4])/100.0;
	CV_CC_CP = CC;
	AT25_Save_VIPSet();
	AT25_Save_CC_CV_CP();
	
	
	CAN1_SendData [3]=CAN1_ReceiveData[3];
	CAN1_SendData [4]=CAN1_ReceiveData[4];

	CAN1_Need_To_Send_Num =5;
	CAN1_Send ();	
}

/**
  * @brief  �趨����������0x23
  * @param  ��
  * @retval ��
  */
static void CAN_function23(void)
{

	CAN1_SendData [0]=(u8)CAN_ID ;
	CAN1_SendData [1]=0x23;
	CAN1_SendData [2]=0x02;    //�������0x02 
	
	
	//�趨��������͵�ѹ
	Set_Power=(CAN1_ReceiveData[3]<<8|CAN1_ReceiveData[4]);
	CV_CC_CP = CP;
	AT25_Save_VIPSet();
	AT25_Save_CC_CV_CP();	
	
	
	CAN1_SendData [3]=CAN1_ReceiveData[3];
	CAN1_SendData [4]=CAN1_ReceiveData[4];
	


	CAN1_Need_To_Send_Num = 5;
	CAN1_Send ();	

}

/**
  * @brief  ��ȡ�������ֵ���ݣ�0x24
  * @param  ��
  * @retval ��
  */
static void CAN_function24(void)
{
	CAN1_SendData [0]=(u8)CAN_ID;
	CAN1_SendData [1]=0x24;
	
	CAN1_SendData [2]=0x03;

	CAN1_SendData [3]=0x17;
	CAN1_SendData [4]=0x70;   //6000W
	CAN1_SendData [5]=0;

	CAN1_Need_To_Send_Num =6;
	CAN1_Send ();
}

/**
  * @brief  ��ȡ�������ֵ���ݣ�0x25
  * @param  ��
  * @retval ��
  */
static void CAN_function25(void)
{
	CAN1_SendData [0]=(u8)CAN_ID;
	CAN1_SendData [1]=0x25;
	
	CAN1_SendData [2]=3;

	CAN1_SendData [3]=0x27;
	CAN1_SendData [4]=0x10;//10000
	CAN1_SendData [5]=2;    //С���㳤��2�����е�������/10��n�η�

	CAN1_Need_To_Send_Num =6;
	CAN1_Send ();
}





/**
  * @brief ��ȡ��ѹ�������������ݣ�0x26
  * @param  ��
  * @retval ��
  */
static void CAN_function26(void)
{

	CAN1_SendData [0]=(u8)CAN_ID;
	CAN1_SendData [1]=0x26;
	CAN1_SendData [2]=0x04;
	
	CAN1_SendData [3]=((u16)(Voltage*100))>>8;     //16λ����8λ���������8λ���ʴ�����0110 0000
	CAN1_SendData [4]=(u8)(Voltage*100);
	CAN1_SendData [5]=((u16)(Current*100))>>8;
	CAN1_SendData [6]=(u8)(Current*100);
	
	CAN1_Need_To_Send_Num =7;
	CAN1_Send ();
	
}


/**
  * @brief  ��ȡ��ѹ���ֵ���ݣ�0x27
  * @param  ��
  * @retval ��
  */
static void CAN_function27(void)
{
	CAN1_SendData [0]=(u8)CAN_ID;
	CAN1_SendData [1]=0x27;
	CAN1_SendData [2]=0x03;
	
	CAN1_SendData [3]=0x1f;
	CAN1_SendData [4]=0x40;    //8000V
	CAN1_SendData [5]=2;


	CAN1_Need_To_Send_Num =6;
	CAN1_Send ();
}


/**
  * @brief  ��ȡ��ѹ�������趨������ݣ�0x28
  * @param  ��
  * @retval ��
  */
static void CAN_function28(void)
{
	
	CAN1_SendData [0]=(u8)CAN_ID;
	CAN1_SendData [1]=0x28;
	CAN1_SendData [2]=0x05;
	
	CAN1_SendData [3]=ON_OFF;
	CAN1_SendData [4]=(u16)(Set_Voltage*100)>>8;
	CAN1_SendData [5]=(u16)(Set_Voltage*100);
	CAN1_SendData [6]=(u16)(Set_Current*100)>>8;
	CAN1_SendData [7]=(u16)(Set_Current*100);
		
	CAN1_Need_To_Send_Num =8;
	CAN1_Send ();	
}
	
/**
  * @brief  �趨������ţ�0x29
  * @param  ��
  * @retval ��
  */
static void CAN_function29(void)
{
	CAN1_SendData[0]=(u8)CAN_ID ;
	CAN1_SendData[1]=0x29;
	CAN1_SendData[2]=0x01;
	
	CAN_ID = (u16)CAN1_ReceiveData[3];
	CAN1_SendData[3]=CAN1_ReceiveData[3];

	CAN1_Need_To_Send_Num =4;
	CAN1_Send ();				
}

/**
  * @brief  ��ȡ��ѹ�������������ݣ�0x2a
  * @param  ��
  * @retval ��
  */
static void CAN_function2a(void)
{
	CAN1_SendData[0]=(u8)CAN_ID ;
	CAN1_SendData[1]=0x2a;
	CAN1_SendData[2]=0x05;
	
	CAN1_SendData [3]=ON_OFF ;
	CAN1_SendData [4]=(u16)(Voltage*100)>>8;
	CAN1_SendData [5]=(u16)(Voltage*100);
	CAN1_SendData [6]=(u16)(Current*100)>>8;
	CAN1_SendData [7]=(u16)(Current*100);

	CAN1_Need_To_Send_Num =8;
	CAN1_Send ();				
}

/**
  * @brief  ��ȡ��ѹ�������������ݣ�0x2e
  * @param  ��
  * @retval ��
  */
static void CAN_function2e(void)
{
	CAN1_SendData[0]=(u8)CAN_ID ;
	CAN1_SendData[1]=0x2e;
	CAN1_SendData[2]=0x05;
	
	CAN1_SendData [3]=ON_OFF ;
	CAN1_SendData [4]=(u16)(Voltage*100)>>8;
	CAN1_SendData [5]=(u16)(Voltage*100);
	CAN1_SendData [6]=(u16)(Current*100)>>8;
	CAN1_SendData [7]=(u16)(Current*100);

	CAN1_Need_To_Send_Num =8;
	CAN1_Send ();				
}


///**
//  * @brief  ʹ�����蹦�ܣ�0x2c
//  * @param  ��
//  * @retval ��
//  */
//static void CAN_function2c(void)
//{
//	CAN1_SendData[0]=(u8)CAN_ID ;
//	CAN1_SendData[1]=0x2c;
//	CAN1_SendData[2]=0x01;
//	
//	//���Ƹ�ʹ�����蹦��
//	if(CAN1_ReceiveData[3]==0x01) WireRES_OnOFF=1;
//	else WireRES_OnOFF=0;
//	
//	CAN1_SendData [3]=CAN1_ReceiveData[3] ;
//	
//	CAN1_Need_To_Send_Num =4;
//	CAN1_Send ();				
//}


///**
//  * @brief  ����������ֵ��0x2d
//  * @param  ��
//  * @retval ��
//  */
//static void CAN_function2d(void)
//{
//	CAN1_SendData[0]=(u8)CAN_ID ;
//	CAN1_SendData[1]=0x2d;
//	CAN1_SendData[2]=0x02;
//	
//	//���Ƹ��������蹦��
//	WireRES=(CAN1_ReceiveData[3]<<8|CAN1_ReceiveData[4]);
//	
//	CAN1_SendData [3]=CAN1_ReceiveData[3] ;
//	CAN1_SendData [4]=CAN1_ReceiveData[4] ;

//	CAN1_Need_To_Send_Num =5;
//	CAN1_Send ();				
//}

 /**
  * @brief  CAN�����жϴ���
  * @param  receive-
  * @retval ��
  */
void CAN1_Receive_Handler(void)
{
	if((CAN1_ReceiveData [0]==(u8)CAN_ID )||(CAN1_ReceiveData [0]==0))
	{
		switch(CAN1_ReceiveData [1])
		{
			case 0x20:CAN_function20();break; 
			case 0x21:CAN_function21();break; 
			case 0x22:CAN_function22();break; 
			case 0x23:CAN_function23();break; 
			case 0x24:CAN_function24();break;
			case 0x25:CAN_function25();break;
			case 0x26:CAN_function26();break; 
			case 0x27:CAN_function27();break; 
			case 0x28:CAN_function28();break;
			case 0x29:CAN_function29();break;
			case 0x2a:CAN_function2a();break;
			case 0x2e:CAN_function2e();break;

//			case 0x2c:CAN_function2c();break;
//			case 0x2d:CAN_function2d();break;
			default: break;
		}			
	}
	else return;	
}


/**
  * @brief  CAN�жϷ�����
  * @param  None
  * @retval None
  */
void CAN1_IT(void)
{
	CanRxMsg RxMessage;
	uint8_t i=0;
	CAN_Receive (CAN1,CAN_FIFO0 ,&RxMessage);
	for(i=0;i < RxMessage.DLC;i++)
	{
		CAN1_ReceiveData[i]=RxMessage.Data[i];
	}	
	CAN1_Receive_Handler();
	CAN_ClearITPendingBit (CAN1,CAN_IT_FMP0 );//ʹFIFO0���յ�һ���±���	
	
}




