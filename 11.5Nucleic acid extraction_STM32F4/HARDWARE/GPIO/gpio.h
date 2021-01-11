#ifndef __GPIO_H
#define __GPIO_H
#include "sys.h"
/*管脚分配						如果是对称部分，以1结尾的就是右边，以11结尾的是左边
直流电机 PA0
LED1  PC10
LED2  PC11 
LED3  PC12
LED4  PD0
LED5  PD1
按键外部中断  PB14
*/

extern int Mixing;



/********************************推挽输出****************************************************/
#define DcMotor_Disable GPIO_ResetBits(GPIOA,GPIO_Pin_0);//直流电机
#define DcMotor_Enable GPIO_SetBits(GPIOA,GPIO_Pin_0);//直流电机

#define LED1_ON GPIO_ResetBits(GPIOC,GPIO_Pin_10);//LED1
#define LED1_OFF GPIO_SetBits(GPIOC,GPIO_Pin_10);//LED1

#define LED2_ON GPIO_ResetBits(GPIOC,GPIO_Pin_11);//LED2
#define LED2_OFF GPIO_SetBits(GPIOC,GPIO_Pin_11);//LED2

#define LED3_ON GPIO_ResetBits(GPIOC,GPIO_Pin_12);//LED3
#define LED3_OFF GPIO_SetBits(GPIOC,GPIO_Pin_12);//LED3

#define LED4_ON GPIO_ResetBits(GPIOD,GPIO_Pin_0);//LED4
#define LED4_OFF GPIO_SetBits(GPIOD,GPIO_Pin_0);//LED4

#define BEEP_ON GPIO_ResetBits(GPIOE,GPIO_Pin_0);
#define BEEP_OFF GPIO_SetBits(GPIOE,GPIO_Pin_0);

#define fan1_ON GPIO_ResetBits(GPIOB,GPIO_Pin_7);//直流电机
#define fan1_OFF GPIO_SetBits(GPIOB,GPIO_Pin_7);//直流电机

#define fan2_ON GPIO_ResetBits(GPIOB,GPIO_Pin_8);//直流电机
#define fan2_OFF GPIO_SetBits(GPIOB,GPIO_Pin_8);//直流电机
/********************************************************************************************/

/********************************上拉输入****************************************************/
#define	Key_PB14			PBin(14)   //加热触控开关
/********************************************************************************************/

#define KEY_PB14_PRES 	1	


void GPIO_init(void);//初始化
u8   KEY_Scan(u8 mode);
void KEY_Func(void);

#endif
