// Fluorescence Reader for Isothermal Device ---------
//---Microcontroller: ADuC847, V1.0 
 //Xianbo Qiu, 2018.4.15

        #include <ADuC847.h>       
        #include <math.h>
		   sbit AD5621_cs	= P3^3;	   //AD5621 CS control
			 sbit CON2=P0^0;		//��ŷ�	 
			 sbit CON1=P0^1;   //FAN control
		   sbit DIR=P0^2;
			 sbit EN=P0^3;
			 
       #define NULL 	0
     
       unsigned char con_kinds=0x00; 
			 unsigned char heat_status=0;
       float Temperature_Signal;	
       float temperature;			 //task type
	     unsigned char rece[3];                 //receive buffer
			 unsigned int mode;
       float Tem_rel;
			 unsigned int dutycycle;
			 unsigned int X0MODE=0;
			 bit dir_control=0;
			 unsigned int highbit=0x05;
			 unsigned int du=0;
			 float Ttest=0;
			 int tempi=0;
			 	int restart=0;
//			 unsigned int sys_status=0;
			 unsigned char cnt,flages=0;

void delay(unsigned long i)//12.582912MHZ/12=1048576=0.95367��s;ʵ��5��s
{ 
    while (i--);
}


                       


//-------AD Converter-------------------------------------------------------------------------------------------
 float Temperature_convert(void)                         
        { 
           unsigned long test_R;					
           unsigned int sample[3];
					 double tmp1=0;
					 double tmp2=0;
	 				 double tmp3=0;
	         double temper=0;
           extern  float Temperature_Signal;	
								
					
					 ADC0CON2 = 0x4A;	              // Ref+/- enabled, Primay ADC come from: Ain1 -> Ain2
           RDY0=0;
           ADCMODE=0X22;					
           while(RDY0==0){}                                    //Wait for AD conversion
           RDY0=0;
           sample[2]=ADC0L; 
           sample[1]=ADC0M;
           sample[0]=ADC0H;                                    
           ADCMODE=0X00;
						 
           test_R=sample[0]*65536+sample[1]*256+sample[2];     //long    
				 	 tmp1=2.5*test_R/16777216.0;
					 tmp2=18000*tmp1/(2.5-tmp1)-1000;
				 	 tmp3=0.9984*tmp2-3.15;//95�ȷ�Χ���0.2~0.4
					 temper=-26.71*log(tmp3) + 271.95;//ʵ�ʲ����¶�			
					 Temperature_Signal=(float) temper;//   (int)(temper*100);//double temper �Ŵ�100����ǿ��ת��Ϊint 
           Ttest=Temperature_Signal;
			     return Temperature_Signal;
         }
 
//=======================AD Converter(get voltage)=========================================
int Tmp_control(float target)
{ 
  extern float Temperature_Signal;   
  extern float temperature;

  extern unsigned int dutycycle;

  temperature=Temperature_Signal;   //????

   if(temperature>target)           
  { 
		dutycycle=0xff;
   PWM0H=dutycycle;//dutycycle=0;
    return dutycycle;
  }
	else if(target<=(temperature+1))
	{
		dutycycle=0X7f;
		PWM0H=dutycycle;
    return dutycycle;		
	}
	else 
		{ 
		dutycycle=0;
		PWM0H=dutycycle;
		}
  return dutycycle;  
}

////���²��� �ش�ȷ����Ϣ
//void send_data_w(void) {
//	 ES=0;//�ش����ж�
//     TI=0;                                                                                       
//  	 RI=0;                  
//  
//	   SBUF='w';         //Module number of the fluoresence detector (#1/#2) 
//     while(TI==0){}                                   
//     TI=0;

//	   SBUF='w';         //Special bit first 
//     while(TI==0){}                                   
//     TI=0;			 
//			 
//     SBUF='w';        //low 8 bit
//     while(TI==0){}                                   
//     TI=0;

//     SBUF='w';          //High 8 bit
//     while(TI==0){}                                    
//     TI=0;    

//	   ES=1;
//	
//}

//A����յ�A��ʼ���� �ش�ȷ����Ϣ
void send_data_aback(void) {
	 ES=0;//�ش����ж�
     TI=0;                                                                                       
  	 RI=0;                  
  
	   SBUF='q';         //Module number of the fluoresence detector (#1/#2) 
     while(TI==0){}                                   
     TI=0;

	   SBUF='q';         //Special bit first 
     while(TI==0){}                                   
     TI=0;			 
			 
     SBUF='q';        //low 8 bit
     while(TI==0){}                                   
     TI=0;

     SBUF='q';          //High 8 bit
     while(TI==0){}                                    
     TI=0;    

	   ES=1;
	
}

//�¶Ȼش���Ϣ
void send_data(void) 
// Send Temperature reading (mV) to the main controller
{
	 extern float Temperature_Signal; 
	 extern unsigned char rece[3]; 
	 unsigned int temp_rel_H;
   unsigned int temp_rel_L;
	 unsigned int temp_rel;
	    
     temp_rel=(int)(Temperature_Signal*100);//float temper �Ŵ�100����ǿ��ת��Ϊint 
     temp_rel_H =temp_rel/256;	               //High 8 bit
     temp_rel_L =temp_rel-256*temp_rel_H;      //low 8 bit                      //low 8 bit
    
     ES=0;//�ش����ж�
     TI=0;                                                                                       
  	 RI=0;                  
  
	   SBUF='t';         //Module number of the fluoresence detector (#1/#2) 
     while(TI==0){}                                   
     TI=0;

	   SBUF=temp_rel_L;         //Special bit first 
     while(TI==0){}                                   
     TI=0;			 
			 
     SBUF=temp_rel_H;        //low 8 bit
     while(TI==0){}							               
     TI=0;

     SBUF='o';          //High 8 bit
     while(TI==0){}                                    
     TI=0;    

	   ES=1;
}

//A���Ļش���Ϣ
void send_data_y(void) {
	 ES=0;//�ش����ж�
     TI=0;                                                                                       
  	 RI=0;                  
  
	   SBUF='y';         //Module number of the fluoresence detector (#1/#2) 
     while(TI==0){}                                   
     TI=0;

	   SBUF='y';         //Special bit first 
     while(TI==0){}                                   
     TI=0;			 
			 
     SBUF='y';        //low 8 bit
     while(TI==0){}                                   
     TI=0;

     SBUF='y';          //High 8 bit
     while(TI==0){}                                    
     TI=0;    

	   ES=1;
	
}
	
//���»��Ȼش���Ϣ
void send_data_mixinghold(void) 
	{
				 extern float Temperature_Signal; 
	 extern unsigned char rece[3]; 
	 unsigned int temp_rel_H;
   unsigned int temp_rel_L;
	 unsigned int temp_rel;
	    
     temp_rel=(int)(Temperature_Signal*100);//float temper �Ŵ�100����ǿ��ת��Ϊint 
     temp_rel_H =temp_rel/256;	               //High 8 bit
     temp_rel_L =temp_rel-256*temp_rel_H;      //low 8 bit   
		
		 ES=0;//�ش����ж�
     TI=0;                                                                                       
  	 RI=0;                  
  
	   SBUF='e';         //Module number of the fluoresence detector (#1/#2) 
     while(TI==0){}                                   
     TI=0;

	   SBUF=temp_rel_L;         //Special bit first 
     while(TI==0){}                                   
     TI=0;			 
			 
     SBUF=temp_rel_H;        //low 8 bit
     while(TI==0){}                                   
     TI=0;

     SBUF='e';          //High 8 bit
     while(TI==0){}                                    
     TI=0;    

	   ES=1;
	
}
	
//�����Ƚ��»ش���Ϣ
void send_data_tempdown(void) 
	{
		 extern float Temperature_Signal; 
		 extern unsigned char rece[3]; 
		 unsigned int temp_rel_H;
		 unsigned int temp_rel_L;
		 unsigned int temp_rel;
	    
     temp_rel=(int)(Temperature_Signal*100);//float temper �Ŵ�100����ǿ��ת��Ϊint 
     temp_rel_H =temp_rel/256;	               //High 8 bit
     temp_rel_L =temp_rel-256*temp_rel_H;      //low 8 bit                      //low 8 bit
    
     ES=0;//�ش����ж�
     TI=0;                                                                                       
  	 RI=0;                  
  
	   SBUF='g';         //Module number of the fluoresence detector (#1/#2) 
     while(TI==0){}                                   
     TI=0;

	   SBUF=temp_rel_L;         //Special bit first 
     while(TI==0){}                                   
     TI=0;			 
			 
     SBUF=temp_rel_H;        //low 8 bit
     while(TI==0){}							               
     TI=0;

     SBUF='g';          //High 8 bit
     while(TI==0){}                                    
     TI=0;    

	   ES=1;
	
}
	
//���»��Ȼش���Ϣ
void send_data_mixingdown(void) 
	{
		 extern float Temperature_Signal; 
		 extern unsigned char rece[3]; 
		 unsigned int temp_rel_H;
		 unsigned int temp_rel_L;
		 unsigned int temp_rel;
	    
     temp_rel=(int)(Temperature_Signal*100);//float temper �Ŵ�100����ǿ��ת��Ϊint 
     temp_rel_H =temp_rel/256;	               //High 8 bit
     temp_rel_L =temp_rel-256*temp_rel_H;      //low 8 bit                      //low 8 bit
    
     ES=0;//�ش����ж�
     TI=0;                                                                                       
  	 RI=0;  
		
	   SBUF='j';         //Module number of the fluoresence detector (#1/#2) 
     while(TI==0){}                                   
     TI=0;

	   SBUF=temp_rel_L;         //Special bit first 
     while(TI==0){}                                   
     TI=0;			 
			 
     SBUF=temp_rel_H;        //low 8 bit
     while(TI==0){}							               
     TI=0;

     SBUF='j';          //High 8 bit
     while(TI==0){}                                    
     TI=0;    

	   ES=1;
	
}
//-----------------


//-----------------System Initialization  ----------------------------------------------------      

     void sys_int(void)		  //System Initialization 
     {
			    extern double LED_Current;  //(mA)
			 
					PWMCON=0X51;//PWM MODE 5 Dual 8-bit outputs  32.768 KHz/12=2.73KHz
					PWM1H=0XFF;   
					PWM0H=0XFF;
          PWM1L=0X0F;
					PWM0L=0X01;	

          delay(100);
          EA=0;              //Disable interruption

	        PLLCON=0X00;       //12.58MHz    
          CFG847=0X00;       //��ջ����

          RDY0 = 0;         
          SF=13;             // 105.3Hz ADC data rate
          
          ADCMODE = 0x24;	   // Internal Zero-Scale Calibration,Chop Mode Enable
          ADCMODE = 0x25;	   // Internal Full-Scale Calibration, Chop Mode Enable
          ADC0CON1 = 0x27;   // Buf enabled, Unipolar, Range +2.56V

		      EADC = 0;	  	


					
         // TMOD=0X11;          
 //�йض�ʱ��
          TL1=0X3D;                                   
          TH1=0X0A;             
          TR1=0;             
	
          TL0=0X3D;      // (65536-A3E)*/12582912=50 ms                            
          TH0=0X0A;            
          TR0=0;                

		      ET0=0;             
		      ET1=0;                 
			//�йش���
          ES=0;               //ES ���ϴ����ж�  1enable 0 disable
    	
          SCON=0X50;          //8λuart �����ʿɱ�  ����ʹ��
          T3CON = 0x86;       //0x86:9600 Baud (CD=0)----0x84:38400 Baud (CD=0)
          T3FD = 0x12; 
					CON2=0;            //p0^0

		      delay(100);
					
		      PS=1;   		//�����ж����ȼ� 1���� 0����
					EA=1;                 //ʹ�������ж�Դ  
		      ES=1;				//�������ж�
				
				
          EN=0;					 //p0^3 
}

void uart_sendstring(unsigned char pt[],unsigned int num)
{
	unsigned int i;
	ES=0;//�ش����ж�
	for(i=0;i<num;i++)
	{
			SBUF=pt[i];
		while(!TI);   //TI transfer���ͱ�־   TI�����������0  0�����Ϳ���  1���������
		TI=0;
			
	}
	ES=1;//�������ж�
}

//-----------motor_remix ���»���----------//		 
void motor_remix_stay(int cycle,int T)    //remix with heat ;cycle=10;
{
	   int i;
	   int tempi=0;
       for (i=1;i<cycle;i++){
				 	Temperature_convert();
          Tmp_control(T);
//					send_data();
          tempi++;					
	        dir_control=~dir_control;
			    DIR=dir_control;
				  PWM1L=0X09;
    			PWM0L=0X08;
				  EN=1;
	        delay(27000);/////////0.135
					send_data_mixinghold();  //���»��Ȼش���Ϣ		
				  PWM1L=0X09;
    			PWM0L=0X08;

					if(tempi==1)
					{
						send_data_mixinghold();  //���»��Ȼش���Ϣ
						delay(80000);//0.4s
					}
					else
					{
						send_data_mixinghold();  //���»��Ȼش���Ϣ
								delay(106000);
//				    delay(300000); //1.5S
					}
				  EN=0;
					delay(200000);//1s
					delay(200000);//1s
					send_data_mixinghold();  //���»��Ȼش���Ϣ
			}
				  dir_control=~dir_control;
			    DIR=dir_control;
				  PWM1L=0X09;
    			PWM0L=0X08;
				  EN=1;
	        delay(40000);//0.2s
							
				  PWM1L=highbit;
    			PWM0L=0X01;
					delay(80000);//0.4s
			    send_data_mixinghold();  //���»��Ȼش���Ϣ
			tempi=0;
			EN=0;
			
}

//-----------motor_remix-���»���---------//		 
void motor_remix_down(int cycle,int T)    //remix with heat ;cycle=10;
{
	   int i,k;
	   int tempi=0;
       for (i=1;i<cycle;i++){
				 	Temperature_convert();
          Tmp_control(T);
//					send_data();
          tempi++;					
	        dir_control=~dir_control;
			    DIR=dir_control;
				  PWM1L=0X09;
    			PWM0L=0X01;
				  EN=1;
	        delay(40000);//0.2s
					send_data_mixingdown();
				  PWM1L=highbit;
    			PWM0L=0X01;

					if(tempi==1)
					{
						send_data_mixingdown();
						delay(80000);//0.4s
					}
					else
					{
						send_data_mixingdown();
						delay(106000);
//				    delay(300000); //1.5S
					}
				  EN=0;
					delay(200000);//1s
					delay(200000);//1s
					send_data_mixingdown();//����ɢ�� led1��led2/led3����
	
				  

			}
				  dir_control=~dir_control;
			    DIR=dir_control;
				  PWM1L=0X09;
    			PWM0L=0X01;
				  EN=1;
	        delay(40000);//0.2s
							
				  PWM1L=highbit;
    			PWM0L=0X01;
					delay(80000);//0.4s
			send_data_mixingdown();//���ȿ���������ɢ�� led1��led2/led3����
			tempi=0;
			EN=0;
			
}
	
//----------stop heat and star fan-------------//
//period=10*1s;
void fan_en(int cycle){
	
	int i,j;
	CON1=0;
	
	for(i=0;i<cycle;i++){
		Temperature_convert();
    Tmp_control(20.0);
//	  send_data();
		//1sһ��
		for(j=0;j<200;j++)
				{
					delay(1000);    				
				}
		send_data_tempdown(); //���ȿ���������ɢ�� led1��led2/led3����v
//	  delay(200000);//1s
	}
	CON1=1;//stop fan
}


	

	


//------------Main Function-----------
void main(void)
{
 	 extern unsigned char con_kinds;
	 extern unsigned char rece[3]; 
	 extern double LED_Current;
	 extern unsigned char heat_status;

	CON1=1;
	CON2=0;


  sys_int();
//-------	RESET-------//

  	IT0=1;   //T0�½��ش���	
	  EX0=1;   //�ⲿ�ж�0ʹ��
	  delay(1000);
	  X0MODE=1; //	T0MODE=1 �����λ
	
    PWM1L=0X0F;
		PWM0L=0X01;	
		EN=1;
	  DIR=1;
	  delay(20000);//1s
    
		EN=0;
		delay(20000);//100ms
		
		 if(X0MODE==1)
		 {
		EN=1;
		DIR=0;
	  PWM1L=0X0f;
		PWM0L=0X01;

		delay(20000);//1s
//	  delay(200000);//1s
		 }

		dir_control=1;
		delay(200000);//1s
		 
		//�����ж�  ENʹ�ܣ�DIR�����򣿣�
		PWM1L=0X0F;
		PWM0L=0X01;	
		EN=1;
	  DIR=1;
	  delay(20000);//1s
    
		EN=0;
		delay(20000);//100ms
		
		 if(X0MODE==1)
		 {
		EN=1;
		DIR=1;
	  PWM1L=0X0f;
		PWM0L=0X01;
			 
    delay(20000);//1s
//	  delay(200000);//1s
		 }

		dir_control=1;
		delay(200000);//1s
		 
		 
//		 while(X0MODE)   //�����ж�
//		 {
//			  uart_sendstring("waittingkey",11);
//			 if(X0MODE==0x0f){
//				 X0MODE=0;
//				 break;
//			 }
//			 delay(10);
//		 }
//--------RESET-------//		 
//		  ET0=1;
//		  TR0=1;
		 

     delay(10);
	   while(1)                 
      { 
				delay(100);
				
			switch(con_kinds)            //�жϽ���stm32���͵�����
			{

				case 1:   //  A           //��ʼ����	
						delay(100);	 
					Temperature_convert(); 
				    Tmp_control(100.0);
				
					if(heat_status==1)
				    {
						send_data();      //A�巢�͡�t����ͷ���� �ж��¶� ����3�Σ���uart�ж��п��Կ�����Ҫ3�Σ���ʼһֱ��t��ͷ�¶�����
						heat_status=0;					
				    }
				    delay(100);
				    break;			
				
				case 2:              //Q  star remix
					delay(300000);//0.5s  ��B���1s��Ϊ�ܵ�Q��Bָ��ʱ���
				  send_data_mixinghold();  //���»��Ȼش���Ϣ
//			  motor_remix_stay(126,100); // ��ϰ������5min;
				  motor_remix_stay(10,100);
				
				  send_data_tempdown(); //��ǰ��һ�䲻Ȼ��Ļ��ʾ�����ӳ�
					delay(600000);//2s
					fan_en(120);			// ���ȿ�ʼɢ��2min 2min��ط��� ԭ120
					
				  send_data_mixingdown();//��ǰ��һ�䲻Ȼ��Ļ��ʾ�����ӳ�
				  delay(600000);//0.5s	
					CON1 = 0;				//1s�󿪷���
  				delay(600000);  //2s
//					motor_remix_down(60, 20);	 //��������ͬ������2min
				  motor_remix_down(30, 20);	 //��������ͬ������1min
				
					CON1 = 1;				 //�ط��ȹػ��
					delay(200000);//1s	/1s
					delay(200000);
					send_data_y();          //����ָ����32��ʼ����  //�������Ŀ�ʼ��־λ��c����stm32
					send_data_y();
					con_kinds=5;  
					break;	
				
				case 3:  //C  stop remix
					delay(100);
				    EX0=1;   //�ⲿ�ж�0ʹ��
					EN=0;
				    delay(1000);
					X0MODE=2;
   
					PWM1L=0X09;
					PWM0L=0X01;
					DIR=1;
					EN=1;
					break;

				  case 4:		 // D               	                
					delay(100);
					CON1=0;
					Temperature_convert();
				    Tmp_control(20.0);
					send_data();				
  				  break;
				
				  case 5:	
							IT0=1;   //T0�½��ش���	
							EX0=1;   //�ⲿ�ж�0ʹ��
							delay(1000);
							X0MODE=1; //	T0MODE=1 �����λ
						
							PWM1L=0X0F;
							PWM0L=0X01;	
							EN=1;
							DIR=1;
							delay(200000);//1s
							
							EN=0;
							delay(20000);//100ms
							
							 if(X0MODE==1)
							 {
							EN=1;
							DIR=0;
							PWM1L=0X0f;
							PWM0L=0X01;

							delay(200000);//1s
							 }
							dir_control=1;
							delay(200000);//1s
              con_kinds=6;							 
  				 break;
				
				  case 6:// F
				  break;

				  default:
				  break;
	        }
		}
		
} 
 

		  


//INT0 interruption
void InterruptINT0(void) interrupt 0
{
      switch(X0MODE)
				{
				case 1://��λ
					EX0=0;
					X0MODE=0x0F;//RESTAR
				  EN=0;				 			
				  break;			

	      case 2:
					EX0=0;
					X0MODE=0x0f;					
				  EN=0;		
  		    break;

		    default:
	
				  break;
	    }
				  
  		 


}
//---------------T0 Interrupter------------
/*
void InterruptTimer0(void) interrupt 1
{

TL0 = 0x3D; //
TH0 = 0x0A;
	cnt++;
	if(cnt>=200);
	{
    cnt=0;
				if (flages==1)
				{					
					Temperature_convert();
          Tmp_control(100.0);
					send_data();
				}
  }

}
*/

//-----Serial communication interruption 
void commun(void) interrupt 4 using 1 
{
	
    extern unsigned char rece[3];       
	  extern unsigned char con_kinds;
		extern unsigned char heat_status;
    extern int restart;
    TI=0;                                                        
    ES=0;                          	
	  
	//Seiral comm protocl (STM32 to ADuC847)
	// Bit[0]: Module number for the Fluor reader (#1(FAM)/#2(ROX))
	// Bit[1]: Speical bit, (1)S --Set LED current, (2)F ---Measure and return fluor value 
  //                      (3)L --LED ON,          (4)M ---LED OFF
	// Bit[2]: LED current (1.0 precision) or 0 for other purposes
	
    rece[0]=SBUF;   //   Module number 
  	RI=0;	    ////RI riceive���ձ�־   RI�����������0	
	  while(RI==0){}
    rece[1]=SBUF;    //  Speical bit
  	RI=0;	 
			
	  while(RI==0){}	
    rece[2]=SBUF;       
  	RI=0;	 
		
			if(rece[0]=='A'||rece[1]=='A'||rece[2]=='A')
		{
			con_kinds=1;
		}
		if(rece[0]=='Q'||rece[1]=='Q'||rece[2]=='Q')
		{
			con_kinds=2;
		}
		
			switch(rece[0])
			{
				case 'A':		               
				  con_kinds=1; //Star heat, Measure Temprature
				  break;			
				
				case 'Q':		              
   				  con_kinds=2; //Star remix
 //         dir_control=1;		
				  break;	

	            case 'C':	
   				  con_kinds=3; //stop remix
				  break;	

	            case 'D':		               	
				  con_kinds=4; //stop heat
  		          break;

				case 'x':		    				
				  restart=1;
  		    break;
							
							
//				case 'E':		    				
//				  con_kinds=5;
//  		          break;

				case 'T':	//����T����A��					
                   heat_status=1;
  		           break;

		          default:
				  break;
	        }
//		}
	  ES=1;              //Enable Comm interruption 
}

