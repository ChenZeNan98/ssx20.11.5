#include "timer.h"
#include "usart.h"
#include "stdbool.h"
#include "usart3.h"

extern vu16 USART3_RX_STA;
extern bool check;
extern float T_rel_a;
extern float T_rel_b;
extern u8 tempnum_a[6];
extern u8 tempnum_b[6];
extern u8 Global_c[4];//接收到的字节
extern int H;
extern int M;
extern int L;


void TIM2_Int_Init(u16 time_ms)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
		time_ms=time_ms*2-1;
    TIM_TimeBaseStructure.TIM_Period = time_ms; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
    TIM_TimeBaseStructure.TIM_Prescaler =41999; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //使能指定的TIM2中断,允许更新中断

    TIM_Cmd(TIM2, ENABLE);  //使能TIMx外设
}


void NVIC_Configuration_TIM2(void)//定时器中断配置
{ 
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //开启中断
	
  NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;    //中断通道名
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;   //抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;    //响应优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;     //启动响应通道的中断
  NVIC_Init(&NVIC_InitStructure);    //中断初始化
}


////定时器2中断服务程序
void TIM2_IRQHandler(void)
{
   //TIM_IT_Update：有某个寄存器被更新
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查指定的TIM中断发生与否:TIM 中断源    
		{

			if (check == true)
			{
				USART_SendData(USART3, 'T');
				while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);  
				USART_SendData(USART3, 'T');
				while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
				USART_SendData(USART3, 'T');
				while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
			}
			else
			{
				USART_SendData(USART3, 'M');
				while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
				USART_SendData(USART3, 'M');
				while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
				USART_SendData(USART3, 'M');
				while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
			}
			check = !check;

			//if(T_rel_a>=100||T_rel_b>=100)
			TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
		}
}



///////////////////////////////////////////////////定时器3接收温度信息显示/////////////////////////////////////////////////////////


//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
							 
}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			if(T_rel_a<100)
			{sprintf(tempnum_a,"%.2f",T_rel_a);}
			else if(T_rel_a>100)
			{sprintf(tempnum_a,"%.1f",T_rel_a);}
			if(T_rel_b<100)
			{sprintf(tempnum_b,"%.2f",T_rel_b);}
			else if(T_rel_b>100)
			{sprintf(tempnum_b,"%.1f",T_rel_b);}
			
			if(T_rel_a>110)
			{
				L = Global_c[1];
				H = Global_c[2];
				T_rel_a = (H * 256 + L) / 100.0;
				if(T_rel_a<100)
				{sprintf(tempnum_a,"%.2f",T_rel_a);}
				else if(T_rel_a>100)
				{sprintf(tempnum_a,"%.1f",T_rel_a);}
			}
			
			if(T_rel_b>110)
			{
				L = Global_c[1];
				H = Global_c[2];
				T_rel_b = (H * 256 + L) / 100.0;
				if(T_rel_b<100)
				{sprintf(tempnum_b,"%.2f",T_rel_b);}
				else if(T_rel_b>100)
				{sprintf(tempnum_b,"%.1f",T_rel_b);}
			}
				
			 TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
		}
}




//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5    
 
   //设置该引脚为复用输出功能,输出TIM3 CH1的PWM脉冲波形	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2
	
	//改通道要改TIM_OCxIint和TIM_SetComparex     TIM3 CH1  PA6
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
	TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPE使能 
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
	

}
