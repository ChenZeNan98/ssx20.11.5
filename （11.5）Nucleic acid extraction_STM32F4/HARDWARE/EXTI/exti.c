#include "exti.h"
#include "delay.h" 
#include "gpio.h"
#include "delay.h"


extern int start;
extern int turnnum;
u8 EXIT_flag=0;
 
//�ⲿ�жϳ�ʼ������
void EXTIX_Init(void)
{

		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);      //�ж����ȼ�������(ȷ����ռ���ȼ��ʹ�ռ���ȼ��ķ���)
	
	  //GPIO������GPIO_init()���Ѿ������ˣ���߲��ظ����� 
	  //PB4�����ⲿ�ж�
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;	//KEYL
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ�ܰ���KEY0���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//ԭ����DISABLE����			//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
		
		
		
		//////////////////////////
//		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource7);
//	  EXTI_InitStructure.EXTI_Line=EXTI_Line7;	//KEYR
//  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
//		
//	  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ���KEY0���ڵ��ⲿ�ж�ͨ��
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//��ռ���ȼ�2 
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�0 
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
//  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

//	  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource6);
//	  EXTI_InitStructure.EXTI_Line=EXTI_Line6;	//KEYR
//  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
//		
//	  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ���KEY0���ڵ��ⲿ�ж�ͨ��
//  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;	//��ռ���ȼ�2 
//  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�0 
//  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
//  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

//�ⲿ�жϳ�ʼ������
void EXTIX1_Init(void)
{

		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	
		
		//PD6 ת��
		SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource6);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line6;	//KEYL
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ���KEY0���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
		

}

void EXTIX1_NVIC_DiInit(void)
{
	  NVIC_InitTypeDef NVIC_InitStructure;
	
		NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			//ʹ�ܰ���KEY0���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;	
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}


//�жϺ���  PB4 
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)//OPTICAL_SEN
    {
			delay_ms(100);
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)==0)   //�����ߵ�ƽ��Ч���ǵ͵�ƽ��Ч��
			{
           start=1;
			}
      EXTI_ClearITPendingBit(EXTI_Line4);
    } 
}



//�жϺ���  
void EXTI9_5_IRQHandler(void)
{

				if(EXTI_GetITStatus(EXTI_Line6) != RESET)//OPTICAL_SEN
				{ 

					if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6)==0)   //�����ߵ�ƽ��Ч���ǵ͵�ƽ��Ч��
					{
						turnnum++;
					}
						EXTI_ClearITPendingBit(EXTI_Line6);  //���LINE4�ϵ��жϱ�־λ  
				}
				
				
				//		if(EXTI_GetITStatus(EXTI_Line7) != RESET)//OPTICAL_SEN
//				{ 
//					delay_ms(10);
//					if(GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_7)==0)   //�����ߵ�ƽ��Ч���ǵ͵�ƽ��Ч��
//					{
//						EXIT_flag=!EXIT_flag;
//					}
//					while(EXIT_flag);
//				 	EXTI_ClearITPendingBit(EXTI_Line7);  //���LINE4�ϵ��жϱ�־λ 
//				}

}

