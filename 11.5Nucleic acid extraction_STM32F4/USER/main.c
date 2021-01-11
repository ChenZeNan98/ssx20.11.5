#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"
#include "dac.h"
#include "gpio.h"
#include "usart3.h"
#include "stdbool.h"
#include "delay.h"
#include "timer.h"
#include "myiic.h"
#include "adc.h"
#include "dac.h"
#include "lcd16032.h"
#include "exti.h"
#include "locale.h"
#include "stdlib.h"
#include "string.h"

int Mixing_Down_a=0;
int Mixing_Down_b=0;
int Mixing_Hold_a=0;
int Mixing_Hold_b=0;
int turnnum=0;
int turnnum16[6];
u8 Global_c1;
u8 Global_c[4];
u8 tempnum_a[6];
u8 tempnum_b[6];
u8 turn_num[6]=0;
int H;
int M;
int L;
int heating_a=0; 
int heating_b=0; 
int aback=0; 
int bback=0; 
int Centrifugal_a=0;
int Centrifugal_b=0;
float T_rel_a;
float T_rel_b;
int start=0;
int Fan_On_a=0;
int Fan_On_b=0;
bool check=true;
float T_rel_a1;
float T_rel_b1;

 int main(void)
 {	
	  int BEEPCOUNT,LEDCOUNT,m;   
	 
		SystemInit();
	  GPIO_init();
	  DcMotor_Disable;
	  BEEP_OFF;
	  fan1_OFF;
	  fan2_OFF;
	  EXTIX_Init();
	  
		delay_init(168);
	  IIC_Init();
		Dac1_Init();
	  Adc_Init();
	  TIM3_Int_Init(999,7199);   //��ʱ��δ����NVIC_PriorityGroup_2
		Lcds_Config();   //pc6:sclk pc7:sid pc8:cs
	  lcd16032_init();  //lcd16032 ��ʼ��
	  Dac1_Set_Vol(330);   //3300-3.3V   3300�����ȡ3299�ͺ� �������ѹ3.3v-5v  0.1V��Ӧ��ֵ66
	  lcd16032_show_string(0,0,"��ʼ���С��������Ե�");
		DcMotor_Disable;

	 //��ʼ����� LED��˸3�Σ�������5��
//	  BEEP_OFF;   		//Ĭ������Ҫ�ص�
		for(LEDCOUNT=0;LEDCOUNT<3;LEDCOUNT++)
		{
			LED1_ON;
			LED2_ON;
			LED3_ON;
			LED4_ON;
			delay_ms(300);						
			LED1_OFF;
			LED2_OFF;
			LED3_OFF;
			LED4_OFF;
			delay_ms(300);
		}
		
		for(BEEPCOUNT=0;BEEPCOUNT<5;BEEPCOUNT++)
		{
			BEEP_ON;
			delay_ms(500);					
			BEEP_OFF;	
			delay_ms(500);
		}
		
		lcd16032_write_cmd(0x01);//����
		delay_ms(1);
		lcd16032_show_string(0,0,"��ʼ�����!");
//		NVIC_Configuration_TIM2();
//		TIM2_Int_Init(1000);            //��ʱ���жϺ������͡�T����M����A��B��ͨ��
		
		while(1) //�ȴ�����������
		{
			if(start==1)
			{
					 	usart3_init(9600);
	          uart_init(9600);
	          NVIC_Configuration_USART3();
//				while(1)
//				{
					USART_ClearITPendingBit(USART3,USART_IT_RXNE);
					while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
					USART_SendData(USART3,'A');	
					while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
					USART_SendData(USART3,'A');	
					while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
					USART_SendData(USART3,'A');	
          while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
					USART_SendData(USART3,'A');					

				  NVIC_Configuration_TIM2();
		      TIM2_Int_Init(1000);            //��ʱ���жϺ������͡�T����M����A��B��ͨ��
					lcd16032_write_cmd(0x01);//����
					lcd16032_show_string(0,0,"A��������:      ��");
					lcd16032_show_string(0,1,"B��������:      ��");
			
			
				while(1)
				{
//					lcd16032_show_string(0,0,"A��������:      ��");
//				  lcd16032_show_string(0,1,"B��������:      ��");
//					lcd16032_show_string(6,0,"      ");
//				  lcd16032_show_string(6,1,"      ");
//					if(tempnum_a>100)
					lcd16032_show_string(6,0,tempnum_a);
					lcd16032_show_string(6,1,tempnum_b);
					LED1_ON;
					delay_ms(350);					
					LED1_OFF;	
					delay_ms(350);
					if(heating_a==1&&heating_b==1)
					{
					break;}
				}
				
				while(1)
				{
					 //���ȴ���95��C//74 12 32 6f     6d 12 32 70
								if(heating_a==1&&heating_b==1)    
								{
									lcd16032_write_cmd(0x01);//����
									lcd16032_show_string(0,0,"������ɣ���");
									lcd16032_show_string(0,1,"׼�����»���");
									delay_ms(5000);  //�ȴ�5S�������
									LED1_ON;
									LED2_ON;

									
//									while(1)
//									{
//								  	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
//									  USART_SendData(USART3,'Q');	
//										delay_ms(1000);
//										while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
//								  	USART_SendData(USART3,'B');
//										delay_ms(1000);
//										// e  f
//										if(Mixing_Hold_a==1&&Mixing_Hold_b==1)
//										{
//											break;}
//									}
									TIM_Cmd(TIM2, DISABLE);
									//����ָ�ʼ���Ȼ��
											while(1)
											{
											USART_SendData(USART3,'Q');
											while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
											USART_SendData(USART3,'Q');
											while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
											USART_SendData(USART3,'Q');
											while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
											delay_ms(1000);
											
											USART_SendData(USART3,'B');
											while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
											USART_SendData(USART3,'B');
											while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
											USART_SendData(USART3,'B');
											while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
											delay_ms(1000);
							
											if(Mixing_Hold_a==1&&Mixing_Hold_b==1)
											{
												heating_a=0;
												heating_b=0;
												break;
											}
										}
								}
								
								//���յ���e�� "f"��ͷ ���»���   e  f
								if(Mixing_Hold_a==1&&Mixing_Hold_b==1)
								{
									lcd16032_write_cmd(0x01);//����
									lcd16032_show_string(0,0,"���»����С���");
									lcd16032_show_string(0,1,"A      ��");
							    lcd16032_show_string(5,1,"B      ��");
									while(1)
									{
//										lcd16032_show_string(0,1,"A      ��");
//									  lcd16032_show_string(5,1,"B      ��");
										lcd16032_show_string(1,1,tempnum_a);
										lcd16032_show_string(6,1,tempnum_b);
										if(Fan_On_a==1&&Fan_On_b==1)
										{
											Mixing_Hold_a=0;
									    Mixing_Hold_b=0;
									  	break;}
									}
								}
									
								//���յ���g�� "h"��ͷ��β �����Ƚ���//67 12 32 67    68 12 32 68
								if(Fan_On_a==1&&Fan_On_b==1)
								{
									LED1_OFF;
									LED2_OFF;
									LED3_ON;
									lcd16032_write_cmd(0x01);//����
									lcd16032_show_string(0,0,"A�彵���У�      ��");
									lcd16032_show_string(0,1,"B�彵���У�      ��");
									while(1)
									{
									  lcd16032_show_string(6,0,tempnum_a);
					          lcd16032_show_string(6,1,tempnum_b);
										//delay_ms(300);
										if(Mixing_Down_a==1&&Mixing_Down_b==1)
										{
											Fan_On_b=0;
									    Fan_On_a=0;
										  break;}
									   }
									
								}
									
								//���յ���j�� "k"��ͷ��β ���»���//6a 12 32 6a   6b 12 32 6b
									if(Mixing_Down_a==1&&Mixing_Down_b==1)
									{
									  LED1_OFF;
										LED2_ON;
										LED3_ON;
									  lcd16032_write_cmd(0x01);//����
									  lcd16032_show_string(0,0,"A���»���B���»���");
										lcd16032_show_string(0,1,"     ��");
									  lcd16032_show_string(5,1,"     ��");
										Mixing_Down_b=0;
										Mixing_Down_a=0;
										Centrifugal_a=0;
										Centrifugal_b=0;
											while(1)
											{
//									 			lcd16032_show_string(0,1,"     ��");
//									    	lcd16032_show_string(5,1,"     ��");									 		
												lcd16032_show_string(0,1,tempnum_a);
												lcd16032_show_string(5,1,tempnum_b);
												if(Centrifugal_a==1 && Centrifugal_b==1)
												{break;}
											}

								   }
										
								//���յ���y�� "z"��ͷ����   y z
										if(Centrifugal_a==1&&Centrifugal_b==1)
										{
										  lcd16032_write_cmd(0x01);//����
											delay_ms(1500);
											EXTIX1_Init();
											lcd16032_show_string(0,0,"�����С���");
											lcd16032_show_string(0,1,"Ȧ����");
											LED1_OFF;
											LED2_OFF;
											LED3_OFF;
											LED4_ON;
//�Ȳ�������				//DcMotor_Enable;
											for(m=0;m<300;m++)  
											{
												sprintf(turn_num,"%d",turnnum);
												lcd16032_show_string(3,1,turn_num);
												DcMotor_Enable;
												delay_ms(100);
												DcMotor_Disable;
												delay_ms(100);
											}

//	                  delay_ms(60000);
//�Ȳ�������					//DcMotor_Disable;
											Centrifugal_a=0;
											Centrifugal_b=0;
											
											for(BEEPCOUNT=0;BEEPCOUNT<3;BEEPCOUNT++)
												{
													BEEP_ON;
													LED1_ON;
													LED2_ON;
													LED3_ON;
													LED4_ON;
													sprintf(turn_num,"%d",turnnum);
													lcd16032_show_string(3,1,turn_num);
													delay_ms(500);					
													BEEP_OFF;	
													LED1_OFF;
													LED2_OFF;
													LED3_OFF;
													LED4_OFF;
													sprintf(turn_num,"%d",turnnum);
													lcd16032_show_string(3,1,turn_num);
													delay_ms(500);
												}
											start=0;
											EXTIX1_NVIC_DiInit();
											turn_num[6]=0;
											turnnum=0;
											lcd16032_write_cmd(0x01);//����
											lcd16032_show_string(0,0,"����ʵ�����");
											lcd16032_show_string(0,1,"�ٴ�ʵ���밴��λ��");
											break;
											}	
					}
			}
		}
 }



// int main(void)
// {	
//	 SystemInit();
//	 GPIO_init();
//	 BEEP_OFF;   		//Ĭ������Ҫ�ص�
//	 Lcds_Config();   //pc6:sclk pc7:sid pc8:cs
//	 delay_init(168);
//	 usart3_init(9600);
//	 delay_ms(50);
//	 lcd16032_init();  //lcd16032 ��ʼ��
//	 Dac1_Init();
//	 Dac1_Set_Vol(0); //3300�����
//	 Dac1_Set_Vol(3299);
// }  
// 
// 
 
//r
//q

//74 12 32 6f
//6d 12 32 70

//e
//f

//67 12 32 67
//68 12 32 68
// 
//6a 12 32 6a
//6b 12 32 6b

//y
//z

