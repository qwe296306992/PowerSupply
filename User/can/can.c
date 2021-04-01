#include "stm32f10x.h"
#include "can.h"

uint8_t CAN1_ReceiveData[10];
uint8_t CAN1_SendData[10];
uint8_t CAN1_Need_To_Send_Num;
//uint16_t  WireRES;
//uint8_t  WireRES_OnOFF;

 /**
  * @brief  CAN通信初始化
  * @param  BraudRate-波特率 单位kbps
  * @retval 无
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
	uint16_t CAN_Prescaler;	    // 控制一个Tq的时间长度

	 switch(BraudRate)           // 波特率  1s传输多少位
	{
		case 5:                       //单位为kbps     应该是5000bps，写成5kbps    后面应该都是要乘以1000   应该有错T1bit=11，算出来不是整数
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
	
	
	
	
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	//使能时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig( GPIO_Remap2_CAN1,ENABLE);     //CAN1备用函数映射
	
	
	//CAN1_RX
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;
	GPIO_InitStructure .GPIO_Speed =GPIO_Speed_50MHz ;
	GPIO_InitStructure .GPIO_Mode =GPIO_Mode_IPU;  //上拉,隐形电平
	GPIO_Init(GPIOD,&GPIO_InitStructure );
	
	//CAN1_TX
	GPIO_InitStructure .GPIO_Pin =GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz ;
	GPIO_InitStructure .GPIO_Mode =GPIO_Mode_AF_PP ;
	GPIO_Init(GPIOD ,&GPIO_InitStructure );


		CAN_InitStructure.CAN_TTCM=DISABLE;	//?禁止时间触发通信模式  
  	CAN_InitStructure.CAN_ABOM=ENABLE;	//自动离线管理
  	CAN_InitStructure.CAN_AWUM=ENABLE;//自动唤醒
  	CAN_InitStructure.CAN_NART=DISABLE;	//关闭自动重传     这边需要注意？？    //使用自动重传，会一直发送报文直到成功为止
  	CAN_InitStructure.CAN_RFLM=DISABLE;	//接收溢出时覆盖
  	CAN_InitStructure.CAN_TXFP=DISABLE;	  //按照报文ID的优先级来发送
  	CAN_InitStructure.CAN_Mode= CAN_Mode_Normal;    //正常模式  

  	CAN_InitStructure.CAN_SJW=CAN_SJW ;	
  	CAN_InitStructure.CAN_BS1=CAN_BS1;
  	CAN_InitStructure.CAN_BS2=CAN_BS2;
  	CAN_InitStructure.CAN_Prescaler=CAN_Prescaler;  //100k
  	CAN_Init(CAN1, &CAN_InitStructure); 
	
	
	//过滤器配置	
	CAN_FilterInitStructure.CAN_FilterNumber=0;	     //一共0-27共28个筛选器，选择第0个
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; //屏蔽位模式    工作在掩码模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit; ////筛选器的尺度，  宽度16位
	CAN_FilterInitStructure.CAN_FilterIdHigh=0;   //用于储存要筛选的ID，16位模式储存就是完整的要筛选的ID
	CAN_FilterInitStructure.CAN_FilterIdLow=0;       //用于储存要筛选的ID，16位储存就是完整的要筛选的ID
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;    //储存的是IdHigh成员对应的掩码，与 CAN_FilterIdHigh 组成一组筛选器
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0; //设置筛选后数据储存到哪个接受的FIFO，筛选器被关联到FIFIO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //是否激活这个筛选器
	CAN_FilterInit(&CAN_FilterInitStructure);//
	
	
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn ;//CAN1 接受中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;    
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;       
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	CAN_ITConfig (CAN1,CAN_IT_FMP0,ENABLE);//接受中断使能       FMP0   FIFO 0信息挂起中断，当FIFO0接受到数据时会引起中断

}


//	uint8_t state = 5;
 /**
  * @brief  CAN发送报文
  * @param 无
  * @retval 无
  */
void CAN1_Send(void)
{
	uint8_t i;
//	uint8_t TransmitMailbox;
	
	CanTxMsg TxMessage; 
	TxMessage.StdId =0;      // 储存的是报文的11位标准标识符
	TxMessage.IDE =CAN_ID_STD;     //本报文是标准帧，使用StdId成员储存报文ID
	TxMessage.RTR =CAN_RTR_DATA;     //数据帧
	TxMessage.DLC =CAN1_Need_To_Send_Num;  //储存报文数据段的长度
	for(i=0;i < CAN1_Need_To_Send_Num;i++)
	{
		TxMessage.Data[i]=CAN1_SendData[i];
	}
	
	   
	//开始传输数据 没有空邮箱返回CAN_TxStatus_NoMailBox
	while(CAN_Transmit (CAN1 ,&TxMessage)==CAN_TxStatus_NoMailBox);

//	state = CAN_TransmitStatus(CAN1, TransmitMailbox);
}


/**
  * @brief  输出控制命令：0x20
  * @param  无
  * @retval 无
  */
static void CAN_function20(void)
{

	CAN1_SendData [0]=(u8)CAN_ID;
	CAN1_SendData [1]=0x20;     //0010 0000
	CAN1_SendData [2]=0x01;     //0000 0001


	if(CAN1_ReceiveData[3]==ON) 
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
  * @brief  设定输出电压命令：0x21
  * @param  无
  * @retval 无
  */
static void CAN_function21(void)
{

	CAN1_SendData [0]=(u8)CAN_ID;//本机ID
	CAN1_SendData [1]=0x21;//指令类型    //0010 0001
	CAN1_SendData [2]=0x02;      //0000 0010
	
	//设定输出电压
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
  * @brief  设定输出电流命令：0x22
  * @param  无
  * @retval 无
  */
static void CAN_function22(void)
{

	CAN1_SendData [0]=(u8)CAN_ID ;
	CAN1_SendData [1]=0x22;
	CAN1_SendData [2]=0x02;
	
	//设定输出电流
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
  * @brief  设定输出功率命令：0x23
  * @param  无
  * @retval 无
  */
static void CAN_function23(void)
{

	CAN1_SendData [0]=(u8)CAN_ID ;
	CAN1_SendData [1]=0x23;
	CAN1_SendData [2]=0x02;    //规格书是0x02 
	
	
	//设定输出电流和电压
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
  * @brief  读取功率最大值数据：0x24
  * @param  无
  * @retval 无
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
  * @brief  读取电流最大值数据：0x25
  * @param  无
  * @retval 无
  */
static void CAN_function25(void)
{
	CAN1_SendData [0]=(u8)CAN_ID;
	CAN1_SendData [1]=0x25;
	
	CAN1_SendData [2]=3;

	CAN1_SendData [3]=0x27;
	CAN1_SendData [4]=0x10;//10000
	CAN1_SendData [5]=2;    //小数点长度2，所有电流数据/10的n次方

	CAN1_Need_To_Send_Num =6;
	CAN1_Send ();
}





/**
  * @brief 读取电压及电流反馈数据：0x26
  * @param  无
  * @retval 无
  */
static void CAN_function26(void)
{

	CAN1_SendData [0]=(u8)CAN_ID;
	CAN1_SendData [1]=0x26;
	CAN1_SendData [2]=0x04;
	
	CAN1_SendData [3]=((u16)(Voltage*100))>>8;     //16位传给8位，传后面的8位，故传的是0110 0000
	CAN1_SendData [4]=(u8)(Voltage*100);
	CAN1_SendData [5]=((u16)(Current*100))>>8;
	CAN1_SendData [6]=(u8)(Current*100);
	
	CAN1_Need_To_Send_Num =7;
	CAN1_Send ();
	
}


/**
  * @brief  读取电压最大值数据：0x27
  * @param  无
  * @retval 无
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
  * @brief  读取电压及电流设定输出数据：0x28
  * @param  无
  * @retval 无
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
  * @brief  设定机器编号：0x29
  * @param  无
  * @retval 无
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
  * @brief  读取电压及电流反馈数据：0x2a
  * @param  无
  * @retval 无
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
  * @brief  读取电压及电流反馈数据：0x2e
  * @param  无
  * @retval 无
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
//  * @brief  使能线阻功能：0x2c
//  * @param  无
//  * @retval 无
//  */
//static void CAN_function2c(void)
//{
//	CAN1_SendData[0]=(u8)CAN_ID ;
//	CAN1_SendData[1]=0x2c;
//	CAN1_SendData[2]=0x01;
//	
//	//立浩改使能线阻功能
//	if(CAN1_ReceiveData[3]==0x01) WireRES_OnOFF=1;
//	else WireRES_OnOFF=0;
//	
//	CAN1_SendData [3]=CAN1_ReceiveData[3] ;
//	
//	CAN1_Need_To_Send_Num =4;
//	CAN1_Send ();				
//}


///**
//  * @brief  设置线阻阻值：0x2d
//  * @param  无
//  * @retval 无
//  */
//static void CAN_function2d(void)
//{
//	CAN1_SendData[0]=(u8)CAN_ID ;
//	CAN1_SendData[1]=0x2d;
//	CAN1_SendData[2]=0x02;
//	
//	//立浩改设置线阻功能
//	WireRES=(CAN1_ReceiveData[3]<<8|CAN1_ReceiveData[4]);
//	
//	CAN1_SendData [3]=CAN1_ReceiveData[3] ;
//	CAN1_SendData [4]=CAN1_ReceiveData[4] ;

//	CAN1_Need_To_Send_Num =5;
//	CAN1_Send ();				
//}

 /**
  * @brief  CAN接受中断处理
  * @param  receive-
  * @retval 无
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
  * @brief  CAN中断服务函数
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
	CAN_ClearITPendingBit (CAN1,CAN_IT_FMP0 );//使FIFO0接收到一个新报文	
	
}




