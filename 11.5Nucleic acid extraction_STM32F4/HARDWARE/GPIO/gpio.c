#include "gpio.h"
#include "delay.h"

//初始化电机、LED、限位开关等只使用电平控制的I/O
//IO初始化
void GPIO_init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);	 //使能PA、B、C、D、E端口时钟

    /********************************推挽输出****************************************************/
    //直流电机
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;     //无上下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA
//	  GPIO_SetBits(GPIOA,GPIO_Pin_0);
	
	  //LED1 PC10  LED2 PC11 LED3 PC12
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);		
		GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
		
		//LED4 PD 0 LED4 PD1 F103c8t6 PD0 PD1是osc    ？？？？(改成用F407了)  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //普通输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);					 //根据设定参数初始化GPIOD
		GPIO_SetBits(GPIOD,GPIO_Pin_0|GPIO_Pin_1);

		//外部中断 PB14 按键 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_4|GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 		 //普通输入模式
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOE
 
		//蜂鸣器PE0
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //普通输出模式
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;     //无上下拉
    GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA

    //PD6 转动圈数
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 				//设置成上拉输入
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //推挽输出
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO口速度为2MHz
    GPIO_Init(GPIOD, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA

}


void LEDALL_ON(void)
{
	LED1_ON;
	LED2_ON;
	LED3_ON;
	LED4_ON;
}

void LEDALL_OFF(void)
{
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
}

/*
u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;//按键按松开标志
    if(mode)key_up=1;  //支持连按
    if(key_up&&(Key_PB14==0))
    {
        delay_ms(10);//去抖动
        key_up=0;
        if(Key_PB14==0)
        {
            return KEY_PB14_PRES;
        }
    } else if(Key_PB14==1)key_up=1;
    return 0;// 无按键按下
}
*/




//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下
//4，KEY3按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!
/*u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;//按键按松开标志
    if(mode)key_up=1;  //支持连按
    if(key_up&&(KEY_U==0||KEY_D==0||KEY_L==0||KEY_R==0))
    {
        delay_ms(10);//去抖动
        key_up=0;
        if(KEY_U==0)
        {
            return KEYU_PRES;
        }
        else if(KEY_D==0)
        {
            return KEYD_PRES;
        }
        else if(KEY_L==0)
        {
            return KEYL_PRES;
        }
        else if(KEY_R==0)
        {
            //	while(!KEY_R);
            return KEYR_PRES;
        }
    } else if(KEY_U==1&&KEY_D==1&&KEY_L==1&&KEY_R==1)key_up=1;
    return 0;// 无按键按下
}
*/


