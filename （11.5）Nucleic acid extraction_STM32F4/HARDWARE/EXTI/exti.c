#include "exti.h"
#include "delay.h" 
#include "gpio.h"
#include "delay.h"


extern int start;
extern int turnnum;
u8 EXIT_flag=0;
 
//外部中断初始化程序
void EXTIX_Init(void)
{

		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);      //中断优先级的设置(确定先占优先级和从占优先级的分配)
	
	  //GPIO配置在GPIO_init()中已经进行了，这边不重复配置 
	  //PB4按键外部中断
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;	//KEYL
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键KEY0所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//原程序DISABLE？？			//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
		
		
		
		//////////////////////////
//		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource7);
//	  EXTI_InitStructure.EXTI_Line=EXTI_Line7;	//KEYR
//  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
//		
//	  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键KEY0所在的外部中断通道
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//抢占优先级2 
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0 
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
//  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

//	  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource6);
//	  EXTI_InitStructure.EXTI_Line=EXTI_Line6;	//KEYR
//  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
//		
//	  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键KEY0所在的外部中断通道
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//抢占优先级2 
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//子优先级0 
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
//  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

//外部中断初始化程序
void EXTIX1_Init(void)
{

		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
		
		//PD6 转速
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource6);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line6;	//KEYL
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键KEY0所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
		

}

void EXTIX1_NVIC_DiInit(void)
{
	  NVIC_InitTypeDef NVIC_InitStructure;
	
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//使能按键KEY0所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;	
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}


//中断函数  PB4 
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)//OPTICAL_SEN
    {
			delay_ms(100);
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)==0)   //看看高电平有效还是低电平有效？
			{
           start=1;
			}
      EXTI_ClearITPendingBit(EXTI_Line4);
    } 
}



//中断函数  
void EXTI9_5_IRQHandler(void)
{

				if(EXTI_GetITStatus(EXTI_Line6) != RESET)//OPTICAL_SEN
				{ 

					if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6)==0)   //看看高电平有效还是低电平有效？
					{
						turnnum++;
					}
						EXTI_ClearITPendingBit(EXTI_Line6);  //清除LINE4上的中断标志位  
				}
				
				
				//		if(EXTI_GetITStatus(EXTI_Line7) != RESET)//OPTICAL_SEN
//				{ 
//					delay_ms(10);
//					if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)==0)   //看看高电平有效还是低电平有效？
//					{
//						EXIT_flag=!EXIT_flag;
//					}
//					while(EXIT_flag);
//				 	EXTI_ClearITPendingBit(EXTI_Line7);  //清除LINE4上的中断标志位 
//				}

}

