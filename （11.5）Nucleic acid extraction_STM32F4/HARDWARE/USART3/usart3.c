#include "delay.h"
#include "usart3.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "timer.h"

extern u8 Global_c[4];//���յ����ֽ�
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

//���ڽ��ջ�����
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.
u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�

//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
vu16 USART3_RX_STA = 0;

int GetKey3(void)
{
u32 Wait_T=2000000;   //��Ӧ����ʱ�ȴ�ʱ��Ϊ50ms������������-100ms(��ܼ�����)
while(!(USART3->SR&USART_FLAG_RXNE))
{
  Wait_T--;
  if(Wait_T==0) break;
}
return((int)(USART3->DR&0x1FF));
}


void USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)//�����ж�
		{
			USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//�ر�USART3�����ж�
			USART_ClearFlag(USART3, USART_FLAG_TC); //�巢����λ�Ĵ�����־λ

			Global_c[0] = GetKey3();
			Global_c[1] = GetKey3();
			Global_c[2] = GetKey3();	
			Global_c[3] = GetKey3();

						
			//���Ȼش�
			if (Global_c[0] == 'q'||Global_c[3] == 'q')
			{
				aback = 1;
			}
			if (Global_c[0] == 'r'||Global_c[3] == 'r')
			{
				bback = 1;
			}
			
			//�¶�
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
			
			//����
			if (Global_c[0] == 'y')
			{
				Centrifugal_a = 1;
			}
			if (Global_c[0] == 'z')
			{
				Centrifugal_b = 1;
			}
			
			
			//���»���
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
			
			//����
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
			
			//���»���
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

		if (USART_GetFlagStatus(USART3, USART_FLAG_ORE) == SET)//�����ж�
		{
			USART_ClearFlag(USART3, USART_FLAG_ORE);          //��SR��ʵ���������־ 
			USART_ReceiveData(USART3);                       //��DR 
		}
}


//��ʼ��IO ����3
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������
void usart3_init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);	// GPIOBʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //����3ʱ��ʹ��
	
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
		GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
    USART_DeInit(USART3);  //��λ����3
    //USART3_TX   ����RX  PD9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8; 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
    GPIO_Init(GPIOD, &GPIO_InitStructure); //��ʼ��PB10
	
    USART_InitStructure.USART_BaudRate = bound;//������һ������Ϊ9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART3, &USART_InitStructure); //��ʼ������	3

    USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���
    //ʹ�ܽ����ж�
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�


//    TIM2_Int_Init(1000 - 1, 7200 - 1);		//10ms�ж�
//	  TIM_Cmd(TIM2, DISABLE);			//�رն�ʱ��4
//    USART3_RX_STA = 0;		//����
}

//uasrt3�ж�����
void NVIC_Configuration_USART3(void)
{ 
		NVIC_InitTypeDef NVIC_InitStructure;          
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   //�ж����ȼ�������
	
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//��������ж����;ͺ�
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //�����ȼ�0
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //IRPͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);  
}



//����3,printf ����
//ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
void u3_printf(char* fmt, ...)
{
    u16 i, j;
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char*)USART3_TX_BUF, fmt, ap);
    va_end(ap);
    i = strlen((const char*)USART3_TX_BUF);		//�˴η������ݵĳ���
    for (j = 0; j < i; j++)							//ѭ����������
    {
        while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); //ѭ������,ֱ���������
        USART_SendData(USART3, USART3_TX_BUF[j]);
    }

}
