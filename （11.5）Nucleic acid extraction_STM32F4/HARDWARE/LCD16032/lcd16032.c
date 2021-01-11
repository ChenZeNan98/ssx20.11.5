
///************************************************************************************************************
//�ʼǣ�
//1�����ô������
//2������ [16 NC��PSB��]������Ҫ�ӵ�, H������ͨ�� L������ͨ��

//3��LCD_CS_Pin    LCD_CS_GPIO_Port    //Ƭѡ����,0,��Ч 1��Ч
//   LCD_SID_Pin   LCD_SID_GPIO_Port   //��������,
//   LCD_SCLK_Pin  LCD_SCLK_GPIO_Port  //ʱ������,0,��Ч,1��Ч

//4������ͨ�Ÿ�ʽ:
//��ʼ֡(1�ֽ�)--11111 R1/W0(��д) RS(����1/ָ��0) 0
//����֡(2�ֽ�)--[D7 D6 D5 D4 0000] [D3 D2 D1 D0 0000]

//5��ָ��ο�[16032Һ����ʾģ��ʹ��˵����.pdf--9ҳ]

//6��Һ�������縴λ�ǣ���Ҫ�ȴ���Դ�ȶ��󣬳�ʼ�������������С�
//   ������ֵ���������Һ������ʾ�쳣���϶�����ʱ�ò�����
//*************************************************************************************************************/
#include "LCD16032.h"
#include "string.h"
#include "usart3.h"
#include "stdlib.h"
#include "delay.h"

//#define lcd16032_cs(N) N==1?HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)
//#define lcd16032_sclk(N) N==1?HAL_GPIO_WritePin(LCD_SCLK_GPIO_Port, LCD_SCLK_Pin, GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_SCLK_GPIO_Port, LCD_SCLK_Pin, GPIO_PIN_RESET)
//#define lcd16032_sid(N) N==1?HAL_GPIO_WritePin(LCD_SID_GPIO_Port, LCD_SID_Pin, GPIO_PIN_SET):HAL_GPIO_WritePin(LCD_SID_GPIO_Port, LCD_SID_Pin, GPIO_PIN_RESET)
//#define lcd16032_sid_input HAL_GPIO_ReadPin(LCD_SID_GPIO_Port, LCD_SID_Pin)

#define lcd16032_cs(N) N==1?CS_H:CS_L
#define lcd16032_sclk(N) N==1?SCLK_H:SCLK_L
#define lcd16032_sid(N) N==1?SID_H:SID_L
#define lcd16032_sid_input GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)

#define LCD16032_CMD_READ     0xFC //������
#define LCD16032_CMD_WRITE    0xF8 //д����
#define LCD16032_DATA_READ    0xFE //������
#define LCD16032_DATA_WRITE   0xFA //д����

#define LCD16032_BYTE_X       20   //X�����֧��20���ַ�,10������
#define LCD16032_BYTE_Y       2    //Y�����֧��2��


/******************************
��������lcd16032_send_byte
��  �ܣ�����������һ֡�ֽ�
��  �Σ�byte--һ֡
����ֵ��
��  ע���������ֲ�ʱ��ͼ
3��LCD_CS_Pin    LCD_CS_GPIO_Port    //Ƭѡ����,0,��Ч 1��Ч
   LCD_SID_Pin   LCD_SID_GPIO_Port   //��������,
   LCD_SCLK_Pin  LCD_SCLK_GPIO_Port  //ʱ������,0,��Ч,1��Ч
*******************************/
void lcd16032_send_byte(uint8_t byte)
{
    uint8_t i = 0;
    lcd16032_cs(1);
    lcd16032_sclk(1);

    //printf("ing send byte\r\n");
    delay_ms(1);
    //u3_printf("cs-%d,scl-%d\r\n", GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8), GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6));

    //u3_printf("byte-0x%x\r\n", byte);
    for(i = 0; i < 8; i++)
    {
        lcd16032_sclk(0);
        delay_us(300);;
        (byte & 0x80) == 0x80 ? lcd16032_sid(1) : lcd16032_sid(0); //ѡ�����ݸߵ͵�ƽ
				//	u3_printf("okkkk");
        //u3_printf("bit-%d,sid-%d\r\n", byte & 0x80, GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7));
        /*��sclk�����½���,��������һ��λ*/
        lcd16032_sclk(1);
        delay_us(300);;

        byte <<= 1; //׼����һλ
        //delay_ms(1));
    }
}


//pc6:sclk pc7:sid pc8:cs
void Lcds_Config(void)
{

  GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/******************************
��������lcd16032_read_byte
��  �ܣ��������ж�ȡһ���ֽ�
��  �Σ�
����ֵ��һ���ֽ�
��  ע��
*******************************/
uint8_t lcd16032_read_byte(void)
{
    uint8_t byte = 0;
    lcd16032_cs(1);
    lcd16032_sclk(1);
    delay_us(300);;
    for(uint8_t i = 0; i < 8; i++)
    {
        if(lcd16032_sid_input)
        {
            byte |= 0x01;
        }
        /*��sclk�����½���,��������һ��λ*/

        byte <<= 1; //׼����һλ
        delay_us(300);;
    }
    //u3_printf("byte:%x\r\n",byte);
    return byte;
}

/******************************
��������lcd16032_read_busy
��  �ܣ���ȡ��ʾ���Ƿ���æ״̬
��  �Σ�
����ֵ��1æ 0��æ
��  ע��
*******************************/
uint8_t lcd16032_read_busy(void)
{
    uint16_t getd;
    lcd16032_cs(1);//����
    lcd16032_send_byte(LCD16032_CMD_READ);
    getd = lcd16032_read_byte();//���ֽ�
    //u3_printf("busy-getdh-0x%x\r\n", getd);
    getd |= (lcd16032_read_byte() >> 4);//���ֽ�+���ֽ�
    //u3_printf("busy-getd-0x%x\r\n", getd);
    return (getd & 0x80) ?  1 : 0;
}

/******************************
��������lcd16032_write_cmd
��  �ܣ�������д����
��  �Σ�cmd--����
����ֵ��0--�ɹ� 1--ʧ��
��  ע������ֵ�ο������ֲ�
*******************************/
uint8_t lcd16032_write_cmd(uint8_t cmd)
{
    uint8_t i = 0, busy = 1;
    i = 5;
    while(i-- && busy)
    {
        busy = lcd16032_read_busy();
			  //u3_printf("asdsfsdfsdfsdfs:%d",busy);
        delay_us(300);;//1ms
    }
    if(i > 0 && busy == 0)
    {
        lcd16032_send_byte(LCD16032_CMD_WRITE);
    }
    else
    {
        //printf("err\r\n");
        return 1;
    }
    delay_us(300);;

    i = 5;
    while(i-- && busy)
    {
        busy = lcd16032_read_busy();
        delay_us(300);;//1ms
    }
    if(i > 0 && busy == 0)
    {
        lcd16032_send_byte(0xf0 & cmd); //high 4bits
        delay_us(300);;
        lcd16032_send_byte(0xf0 & (cmd << 4)); //low 4bits
        //u3_printf("cmd-0x%x-0x%x-0x%x\r\n", cmd, 0xf0 & cmd, 0xf0 & (cmd << 4));
    }
    else
    {
        return 1;
    }
    return 0;
}

/******************************
��������lcd16032_write_data
��  �ܣ�������д��һ���ֽ�
��  �Σ�byte--�ֽ�
����ֵ��0--�ɹ� 1--ʧ��
��  ע��
*******************************/
uint8_t lcd16032_write_data(uint8_t byte)
{
    uint8_t i = 0, busy = 1;
    i = 5;
    while(i-- && busy)
    {
        busy = lcd16032_read_busy();
       delay_us(300);;//1ms
    }
    if(i > 0 && busy == 0)
    {
        lcd16032_send_byte(LCD16032_DATA_WRITE);
    }
    else
    {
        return 1;
    }
   delay_us(300);;//1ms
    i = 5;
    while(i-- && busy)
    {
        busy = lcd16032_read_busy();
       delay_us(300);;//1ms
    }
    if(i > 0 && busy == 0)
    {
        lcd16032_send_byte(0xf0 & byte);
        delay_us(300);;
        lcd16032_send_byte(0xf0 & (byte << 4));
    }
    else
    {
        return 1;
    }
    return 0;
}

/******************************
��������lcd16032_set_xy
��  �ܣ������ַ���ʾλ��
��  �Σ�x��,y��
����ֵ��0--�ɹ� 1--ʧ��
��  ע������  X��:80H-89H   Y��:80H-9FH
��  �ǣ�һ���ַ� ռx-8λ,y-16λ
*******************************/
uint8_t lcd16032_set_xy(uint8_t x, uint8_t y)
{
    uint8_t err;
    if((x > LCD16032_BYTE_X) || (y > LCD16032_BYTE_Y+1))
    {
        x = 0;
        y = 1;
    }
    y == 0 ? (err = lcd16032_write_cmd(0x80 + x)) : (err = lcd16032_write_cmd(0x90 + x)); //�����ַ���ʾλ�� ����72us
    return err;
}




/******************************
��������lcd16032_init
��  �ܣ�������ʼ��
��  �Σ�
����ֵ��
��  ע��00110000  ��Ҫ����RTOS�����ܳ�ʼ��Һ����
*******************************/
void lcd16032_init(void)
{
//    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); //Ƭѡ����,ʹ��ʱ����
//    HAL_GPIO_WritePin(LCD_SCLK_GPIO_Port, LCD_SCLK_Pin, GPIO_PIN_SET); //ʱ������,������ʱ��Ч
	  CS_L;
	  SCLK_H;
    lcd16032_write_cmd(0x30);//�����趨����00110000[8-BIT ���ƽӿ�][����ָ�����]����72us
    delay_us(300);;//100us
    lcd16032_write_cmd(0x0c);//�����趨����00110000[8-BIT ���ƽӿ�][����ָ�����]����72us
    delay_us(300);;//100us
    lcd16032_write_cmd(0x01);//��ʾ���� ����1.6ms
}

/***************************Ӧ���ຯ��********************************/
/******************************
��������lcd16032_send_char
��  �ܣ���ʾ�����ַ�
��  �Σ�byte--�ֽ�
����ֵ��0--�ɹ� 1--ʧ��
��  ע��
*******************************/
uint8_t lcd16032_send_char(uint8_t byte)
{
    return lcd16032_write_data(byte);
}
/******************************
��������lcd16032_show_char
��  �ܣ���ʾ�����ַ�
��  �Σ�x,y byte--�ֽ�
����ֵ��0--�ɹ� 1--ʧ��
��  ע��
*******************************/
uint8_t lcd16032_show_char(uint8_t x, uint8_t y, uint8_t byte)
{
    lcd16032_set_xy(x, y);
    return lcd16032_write_data(byte);
}

/******************************
��������lcd16032_send_string
��  �ܣ���ʾ����ַ�
��  �Σ�str--�ַ���
����ֵ��0--�ɹ� >1--ʧ��
��  ע��
*******************************/
uint8_t lcd16032_send_string(uint8_t *str)
{
    uint8_t i, len, err = 0;
    len = strlen((const char*) str);
    for(i = 0; i < len; i++)
    {
        err |= lcd16032_write_data(str[i]);
    }
    return err;
}

/******************************
��������lcd16032_show_string
��  �ܣ���ʾ����ַ�,��ָ����ʾλ��
��  �Σ�col���У�ȡֵ0-9��,row���У�0|1�� str
����ֵ��0--�ɹ� >1--ʧ��
��  ע��һ�����������ֽ�,Һ����һ����ַֻ�ܴ��һ�����ֻ��������ַ�.
*******************************/
uint8_t lcd16032_show_string(uint8_t col, uint8_t row, uint8_t *str)
{
    uint8_t err, game_x, game_y, i = 0, len=0, flag = 0;
	  
//	  uint8_t TxBuffer[32];
    err = lcd16032_set_xy(col, row);
    //delay_ms(1);
   delay_us(300);;
    game_x = col;
    game_y = row;
    len = strlen((const char *)str);

    //printf("TextLen-%d\r\n", len);

    while(*str != '\r' && i < len)
    {

        if(*str > 127)//����
        {
            if(flag == 1) //�ж��ϸ��ֽڣ��Ƿ�ռ��һ����ַ��û��ռ����λ ! = 0
            {
                game_x++;
                flag = 0;
                lcd16032_send_char(' ');//��λ,ʹ����������ʾ
                //lcd16032_set_xy(game_x, game_y);
                
            }
           delay_us(300);;
            //delay_ms(1));
            lcd16032_send_char(*str++);
           delay_us(300);;
            //delay_ms(1));
            lcd16032_send_char(*str++);
            game_x ++;   //��¼���λ��
            i += 2;      //��¼�ַ���Ŀ��λ
        }
        else //��ĸ
        {
            if(flag == 1)
            {
                game_x++;
                flag = 0;
               
            }
            else
            {
                flag++;
            }
            i++;
            err |= lcd16032_send_char(*str++);
           delay_us(300);;
            //delay_ms(1));
        }

//        if(*str == '&' && game_y < 2) //����л��з������л�����һ��
//        {
//            if(game_y == 1)
//            {
//                lcd16032_set_xy(game_x, 1);
//                delay_us(300);;
//                game_y = 1;
//                i++;
//                flag = 0;
//            }
//            else
//            {
//                lcd16032_set_xy(0, 1);
//                //delay_ms(1));
//                delay_us(300);;
//                game_x = 0;
//                game_y = 1;
//                flag = 0;

//            }
//            //printf("game_y-%d\r\n", game_y);
//            *str++;

//            continue;
//        }
//        else if((col + game_x > (LCD16032_BYTE_X/2) - 1) && game_y == 0) //��һ�й������Զ��л����ڶ���
//        {
//            lcd16032_set_xy(0, 1);
//            //delay_ms(1));
//            delay_us(300);;
//            game_x = 0;
//            game_y = 1;
//            flag = 0;
//            //printf("game_y2-%d\r\n", game_y);
//            continue;
//        }
    }
    return err;
}



