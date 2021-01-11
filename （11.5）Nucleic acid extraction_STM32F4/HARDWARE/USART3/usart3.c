#include "delay.h"
#include "usart3.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "timer.h"

extern u8 Global_c[4];//接收到的字节
extern float T_rel_a;
extern float T_rel_b;
extern int H;
extern int M;
extern int L;
extern int heating_a;
extern int heating_b;
extern int aback;
extern int bback;
extern int Fan_On_a;
extern int Fan_On_b;
extern int Mixing_Down_a;
extern int Mixing_Down_b;
extern int Mixing_Hold_a;
extern int Mixing_Hold_b;
extern int Centrifugal_a;
extern int Centrifugal_b;

//串口接收缓存区
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//接收缓冲,最大USART3_MAX_RECV_LEN个字节.
u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			//发送缓冲,最大USART3_MAX_SEND_LEN字节

//通过判断接收连续2个字符之间的时间差不大于10ms来决定是不是一次连续的数据.
//如果2个字符接收间隔超过10ms,则认为不是1次连续数据.也就是超过10ms没有接收到
//任何数据,则表示此次接收完毕.
//接收到的数据状态
//[15]:0,没有接收到数据;1,接收到了一批数据.
//[14:0]:接收到的数据长度
vu16 USART3_RX_STA = 0;

int GetKey3(void)
{
u32 Wait_T=2000000;   //对应的延时等待时间为50ms（龚计算结果）-100ms(吴杰计算结果)
while(!(USART3->SR&USART_FLAG_RXNE))
{
  Wait_T--;
  if(Wait_T==0) break;
}
return((int)(USART3->DR&0x1FF));
}


void USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)//接收中断
		{
			USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//关闭USART3串口中断
			USART_ClearFlag(USART3, USART_FLAG_TC); //清发送移位寄存器标志位

			Global_c[0] = GetKey3();
			Global_c[1] = GetKey3();
			Global_c[2] = GetKey3();	
			Global_c[3] = GetKey3();

						
			//加热回传
			if (Global_c[0] == 'q'||Global_c[3] == 'q')
			{
				aback = 1;
			}
			if (Global_c[0] == 'r'||Global_c[3] == 'r')
			{
				bback = 1;
			}
			
			//温度
			if (Global_c[0] == 't'&&Global_c[3] == 'o')
			{
				L = Global_c[1];
				H = Global_c[2];

				T_rel_a = (H * 256 + L) / 100.0;
				if (T_rel_a >= 95)
				{
					heating_a = 1;
					//       GPIO_SetBits(GPIOC,GPIO_Pin_11);
					//       TIM_Cmd(TIM2,DISABLE); 
				}
			}
			if (Global_c[0] == 'm'&&Global_c[3] == 'p' )
			{
				L = Global_c[1];
				H = Global_c[2];

				T_rel_b = (H * 256 + L) / 100.0;
				if (T_rel_b >= 95)
				{
					heating_b = 1;
					//       GPIO_SetBits(GPIOC,GPIO_Pin_11);
					//       TIM_Cmd(TIM2,DISABLE); 
				}
			}
			
			//离心
			if (Global_c[0] == 'y')
			{
				Centrifugal_a = 1;
			}
			if (Global_c[0] == 'z')
			{
				Centrifugal_b = 1;
			}
			
			
			//恒温混匀
			if (Global_c[0] == 'e'&&Global_c[3] == 'e')
			{
				L = Global_c[1];
				H = Global_c[2];
				T_rel_a = (H * 256 + L) / 100.0;
				Mixing_Hold_a = 1;
			}
			if (Global_c[0] == 'f'&&Global_c[3] == 'f')
			{
				L = Global_c[1];
				H = Global_c[2];
				T_rel_b = (H * 256 + L) / 100.0;
				Mixing_Hold_b = 1;
			}
			
			//降温
			if (Global_c[0] == 'g'&&Global_c[3] == 'g')
			{
				L = Global_c[1];
				H = Global_c[2];
				T_rel_a = (H * 256 + L) / 100.0;
				Fan_On_a = 1;
			}
			if (Global_c[0] == 'h'&&Global_c[3] == 'h')
			{
				L = Global_c[1];
				H = Global_c[2];
				T_rel_b = (H * 256 + L) / 100.0;
				Fan_On_b = 1;
			}
			
			//降温混匀
			if (Global_c[0] == 'j'&&Global_c[3] == 'j')
			{
				L = Global_c[1];
				H = Global_c[2];
				T_rel_a = (H * 256 + L) / 100.0;
				Mixing_Down_a = 1;
			}
			if (Global_c[0] == 'k'&&Global_c[3] == 'k')
			{
				L = Global_c[1];
				H = Global_c[2];
				T_rel_b = (H * 256 + L) / 100.0;
				Mixing_Down_b = 1;
			}
			
			USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//Enable USART6 interrupt
		}

		if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) == SET)//接受中断
		{
			USART_ClearFlag(USART3, USART_FLAG_ORE);          //读SR其实就是清除标志 
			USART_ReceiveData(USART3);                       //读DR 
		}
}


//初始化IO 串口3
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率
void usart3_init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	// GPIOB时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //串口3时钟使能
	
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART1); //GPIOA10复用为USART1
	
    USART_DeInit(USART3);  //复位串口3
    //USART3_TX   复用RX  PD9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
    GPIO_Init(GPIOD, &GPIO_InitStructure); //初始化PB10
	
    USART_InitStructure.USART_BaudRate = bound;//波特率一般设置为9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART3, &USART_InitStructure); //初始化串口	3

    USART_Cmd(USART3, ENABLE);                    //使能串口
    //使能接收中断
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断


//    TIM2_Int_Init(1000 - 1, 7200 - 1);		//10ms中断
//	  TIM_Cmd(TIM2, DISABLE);			//关闭定时器4
//    USART3_RX_STA = 0;		//清零
}

//uasrt3中断配置
void NVIC_Configuration_USART3(void)
{ 
		NVIC_InitTypeDef NVIC_InitStructure;          
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   //中断优先级的设置
	
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//在这里改中断类型就好
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //子优先级0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //IRP通道使能
		NVIC_Init(&NVIC_InitStructure);  
}



//串口3,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void u3_printf(char* fmt, ...)
{
    u16 i, j;
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char*)USART3_TX_BUF, fmt, ap);
    va_end(ap);
    i = strlen((const char*)USART3_TX_BUF);		//此次发送数据的长度
    for (j = 0; j < i; j++)							//循环发送数据
    {
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); //循环发送,直到发送完毕
        USART_SendData(USART3, USART3_TX_BUF[j]);
    }

}
