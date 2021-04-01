#include "stm32f10x.h"   
#include "spi_oled.h"
#include "systick.h"
#include "keys.h"
#include "encoder.h"
#include "tim_oled.h"
#include "at25.h"
#include "tim_delay.h"
#include "tim_cycle.h"
#include "adc.h"
#include "protect.h"
#include "usart.h"
#include "can.h"
#include "tim_slope.h"
#include "dac.h"
#include "softstart.h"
#include "software.h"
#include "SCR_Controller.h"


int main(void)
{

//////////////////////////////////////////////////////////////////////	//读保护
//////////////////////////////////////////////////////////////////////	FLASH_Unlock();
//////////////////////////////////////////////////////////////////////	FLASH_ReadOutProtection(ENABLE);
	SoftStart_Init();//软启动引脚初始化
	TIM_PROTECT_Init();//设置保护参数，如电压电流功率报警阀值，软启动定时也在里面
	//可控硅初始化
	SCR_Controller_Init();
	//各种初始化
	SysTick_Delay_Ms(10);
	SPI_FUN();
	OLED_Init();
  Keys_Init();
  Encode_Init();//中断分组已经在此初始化一次	
	
  TIM_OLED_Init();
	TIM_DELAY_Init();
	TIM_CYCLE_Init();
	
//	while(1);  		没漏
	
//	TIM_PROTECT_Init();//设置保护参数，如电压电流功率报警阀值，软启动定时也在里面
	TIM_SLOPE_Init();//
//	while(1);			没漏
  SPI1_DAC_Config();		
//	CP_Slope_Init();//恒功率CP模式下的定时器初始化程序
//	SoftStart_Init();//软启动应该放在最前面
//	while(1);     漏
	SS_DISADBE_Init();
	
	TestCurrent_GPIO_Config();//OC1输出过流保护
	
	//检查开关是否接上了
	Check_T_I_Switch();

	//解锁AT25
	SPI_AT25_Unlock();
	
	//从AT25加载数据
	AT25_Load_Data();
	
  //初始化通信
	CAN_Config(CAN_BaudRate);
	Usart2_Config(RS232_BaudRate);
	
		//ADC初始化
	SPI2_ADC_config();

//0.053s(53ms)一次while循环
	while(1)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_12);//拉高CS电平
		SysTick_Delay_Ms(1);		
		Display_Interface(Display_Interface_Mode);
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);//最后拉低CS电平，选中芯片
		SysTick_Delay_Ms(1);
		Keys_Handler(keys_EN);//软启动期间DISABLE按键
		Computer_Control_Device();
		TestCurrent();//过流保护检测	
		
//		if( (TIM3->CR1 & TIM_CR1_CEN) == 0 && ((EXTI->IMR & EXTI_Line9) == 0))  //发生错误
//		{
//				EXTI->IMR |= EXTI_Line9;  //开放中断
//		}
		
//		DISADBE_On_Or_Off();
	}

}



