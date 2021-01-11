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
extern u8 Global_c[4];//���յ����ֽ�
extern int H;
extern int M;
extern int L;


void TIM2_Int_Init(u16 time_ms)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
		time_ms=time_ms*2-1;
    TIM_TimeBaseStructure.TIM_Period = time_ms; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
    TIM_TimeBaseStructure.TIM_Prescaler =41999; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM2�ж�,��������ж�

    TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����
}


void NVIC_Configuration_TIM2(void)//��ʱ���ж�����
{ 
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�����ж�
	
  NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;    //�ж�ͨ����
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;   //��ռ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;    //��Ӧ���ȼ�
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;     //������Ӧͨ�����ж�
  NVIC_Init(&NVIC_InitStructure);    //�жϳ�ʼ��
}


////��ʱ��2�жϷ������
void TIM2_IRQHandler(void)
{
   //TIM_IT_Update����ĳ���Ĵ���������
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���ָ����TIM�жϷ������:TIM �ж�Դ    
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



///////////////////////////////////////////////////��ʱ��3�����¶���Ϣ��ʾ/////////////////////////////////////////////////////////


//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
							 
}
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
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




//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //ʹ��GPIO�����AFIO���ù���ģ��ʱ��
	
	//GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3������ӳ��  TIM3_CH2->PB5    
 
   //���ø�����Ϊ�����������,���TIM3 CH1��PWM���岨��	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM3 OC2
	
	//��ͨ��Ҫ��TIM_OCxIint��TIM_SetComparex     TIM3 CH1  PA6
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM3,ENABLE);//ARPEʹ�� 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
	

}
