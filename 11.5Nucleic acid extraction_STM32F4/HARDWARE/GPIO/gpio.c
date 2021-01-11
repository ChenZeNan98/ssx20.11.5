#include "gpio.h"
#include "delay.h"

//��ʼ�������LED����λ���ص�ֻʹ�õ�ƽ���Ƶ�I/O
//IO��ʼ��
void GPIO_init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);	 //ʹ��PA��B��C��D��E�˿�ʱ��

    /********************************�������****************************************************/
    //ֱ�����
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //��ͨ���ģʽ
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;     //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA
//	  GPIO_SetBits(GPIOA,GPIO_Pin_0);
	
	  //LED1 PC10  LED2 PC11 LED3 PC12
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //��ͨ���ģʽ
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOC, &GPIO_InitStructure);		
		GPIO_SetBits(GPIOC,GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12);
		
		//LED4 PD 0 LED4 PD1 F103c8t6 PD0 PD1��osc    ��������(�ĳ���F407��)  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //��ͨ���ģʽ
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOD, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOD
		GPIO_SetBits(GPIOD,GPIO_Pin_0|GPIO_Pin_1);

		//�ⲿ�ж� PB14 ���� 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_4|GPIO_Pin_5;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 		 //��ͨ����ģʽ
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOE
 
		//������PE0
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 		 //��ͨ���ģʽ
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;     //��������
    GPIO_Init(GPIOE, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA

    //PD6 ת��Ȧ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 				//���ó���������
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   //�������
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO���ٶ�Ϊ2MHz
    GPIO_Init(GPIOD, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA

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
    static u8 key_up=1;//�������ɿ���־
    if(mode)key_up=1;  //֧������
    if(key_up&&(Key_PB14==0))
    {
        delay_ms(10);//ȥ����
        key_up=0;
        if(Key_PB14==0)
        {
            return KEY_PB14_PRES;
        }
    } else if(Key_PB14==1)key_up=1;
    return 0;// �ް�������
}
*/




//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2����
//4��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!
/*u8 KEY_Scan(u8 mode)
{
    static u8 key_up=1;//�������ɿ���־
    if(mode)key_up=1;  //֧������
    if(key_up&&(KEY_U==0||KEY_D==0||KEY_L==0||KEY_R==0))
    {
        delay_ms(10);//ȥ����
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
    return 0;// �ް�������
}
*/


