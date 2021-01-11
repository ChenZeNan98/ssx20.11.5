
///************************************************************************************************************
//笔记：
//1、采用串行输出
//2、器件 [16 NC（PSB）]引脚需要接地, H：并口通信 L：串口通信

//3、LCD_CS_Pin    LCD_CS_GPIO_Port    //片选控制,0,无效 1有效
//   LCD_SID_Pin   LCD_SID_GPIO_Port   //数据引脚,
//   LCD_SCLK_Pin  LCD_SCLK_GPIO_Port  //时钟引脚,0,有效,1无效

//4、串口通信格式:
//起始帧(1字节)--11111 R1/W0(读写) RS(数据1/指令0) 0
//数据帧(2字节)--[D7 D6 D5 D4 0000] [D3 D2 D1 D0 0000]

//5、指令集参考[16032液晶显示模块使用说明书.pdf--9页]

//6、液晶屏掉电复位是，需要等待电源稳定后，初始化才能正常运行。
//   如果出现掉电重启后，液晶屏显示异常，肯定是延时得不够。
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

#define LCD16032_CMD_READ     0xFC //读命令
#define LCD16032_CMD_WRITE    0xF8 //写命令
#define LCD16032_DATA_READ    0xFE //读数据
#define LCD16032_DATA_WRITE   0xFA //写数据

#define LCD16032_BYTE_X       20   //X轴最大支持20个字符,10个汉字
#define LCD16032_BYTE_Y       2    //Y轴最大支持2行


/******************************
函数名：lcd16032_send_byte
功  能：向器件发送一帧字节
形  参：byte--一帧
返回值：
备  注：看器件手册时序图
3、LCD_CS_Pin    LCD_CS_GPIO_Port    //片选控制,0,无效 1有效
   LCD_SID_Pin   LCD_SID_GPIO_Port   //数据引脚,
   LCD_SCLK_Pin  LCD_SCLK_GPIO_Port  //时钟引脚,0,有效,1无效
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
        (byte & 0x80) == 0x80 ? lcd16032_sid(1) : lcd16032_sid(0); //选择数据高低电平
				//	u3_printf("okkkk");
        //u3_printf("bit-%d,sid-%d\r\n", byte & 0x80, GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7));
        /*但sclk出现下降沿,器件接收一个位*/
        lcd16032_sclk(1);
        delay_us(300);;

        byte <<= 1; //准备下一位
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/******************************
函数名：lcd16032_read_byte
功  能：从器件中读取一个字节
形  参：
返回值：一个字节
备  注：
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
        /*但sclk出现下降沿,器件发送一个位*/

        byte <<= 1; //准备下一位
        delay_us(300);;
    }
    //u3_printf("byte:%x\r\n",byte);
    return byte;
}

/******************************
函数名：lcd16032_read_busy
功  能：读取显示屏是否处于忙状态
形  参：
返回值：1忙 0不忙
备  注：
*******************************/
uint8_t lcd16032_read_busy(void)
{
    uint16_t getd;
    lcd16032_cs(1);//拉高
    lcd16032_send_byte(LCD16032_CMD_READ);
    getd = lcd16032_read_byte();//高字节
    //u3_printf("busy-getdh-0x%x\r\n", getd);
    getd |= (lcd16032_read_byte() >> 4);//高字节+低字节
    //u3_printf("busy-getd-0x%x\r\n", getd);
    return (getd & 0x80) ?  1 : 0;
}

/******************************
函数名：lcd16032_write_cmd
功  能：向器件写命令
形  参：cmd--命令
返回值：0--成功 1--失败
备  注：命令值参考器件手册
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
函数名：lcd16032_write_data
功  能：向器件写入一个字节
形  参：byte--字节
返回值：0--成功 1--失败
备  注：
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
函数名：lcd16032_set_xy
功  能：设置字符显示位置
形  参：x列,y行
返回值：0--成功 1--失败
备  注：器件  X轴:80H-89H   Y轴:80H-9FH
笔  记：一个字符 占x-8位,y-16位
*******************************/
uint8_t lcd16032_set_xy(uint8_t x, uint8_t y)
{
    uint8_t err;
    if((x > LCD16032_BYTE_X) || (y > LCD16032_BYTE_Y+1))
    {
        x = 0;
        y = 1;
    }
    y == 0 ? (err = lcd16032_write_cmd(0x80 + x)) : (err = lcd16032_write_cmd(0x90 + x)); //设置字符显示位置 至少72us
    return err;
}




/******************************
函数名：lcd16032_init
功  能：器件初始化
形  参：
返回值：
备  注：00110000  需要启动RTOS，才能初始化液晶屏
*******************************/
void lcd16032_init(void)
{
//    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET); //片选拉低,使用时拉高
//    HAL_GPIO_WritePin(LCD_SCLK_GPIO_Port, LCD_SCLK_Pin, GPIO_PIN_SET); //时钟拉高,当拉低时有效
	  CS_L;
	  SCLK_H;
    lcd16032_write_cmd(0x30);//功能设定设置00110000[8-BIT 控制接口][基本指令集动作]至少72us
    delay_us(300);;//100us
    lcd16032_write_cmd(0x0c);//功能设定设置00110000[8-BIT 控制接口][基本指令集动作]至少72us
    delay_us(300);;//100us
    lcd16032_write_cmd(0x01);//显示清屏 至少1.6ms
}

/***************************应用类函数********************************/
/******************************
函数名：lcd16032_send_char
功  能：显示单个字符
形  参：byte--字节
返回值：0--成功 1--失败
备  注：
*******************************/
uint8_t lcd16032_send_char(uint8_t byte)
{
    return lcd16032_write_data(byte);
}
/******************************
函数名：lcd16032_show_char
功  能：显示单个字符
形  参：x,y byte--字节
返回值：0--成功 1--失败
备  注：
*******************************/
uint8_t lcd16032_show_char(uint8_t x, uint8_t y, uint8_t byte)
{
    lcd16032_set_xy(x, y);
    return lcd16032_write_data(byte);
}

/******************************
函数名：lcd16032_send_string
功  能：显示多个字符
形  参：str--字符串
返回值：0--成功 >1--失败
备  注：
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
函数名：lcd16032_show_string
功  能：显示多个字符,并指定显示位置
形  参：col：列（取值0-9）,row：行（0|1） str
返回值：0--成功 >1--失败
备  注：一个汉字两个字节,液晶屏一个地址只能存放一个汉字或者两个字符.
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

        if(*str > 127)//汉字
        {
            if(flag == 1) //判断上个字节，是否占满一个地址。没有占满补位 ! = 0
            {
                game_x++;
                flag = 0;
                lcd16032_send_char(' ');//补位,使汉字正常显示
                //lcd16032_set_xy(game_x, game_y);
                
            }
           delay_us(300);;
            //delay_ms(1));
            lcd16032_send_char(*str++);
           delay_us(300);;
            //delay_ms(1));
            lcd16032_send_char(*str++);
            game_x ++;   //记录光标位置
            i += 2;      //记录字符串目标位
        }
        else //字母
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

//        if(*str == '&' && game_y < 2) //如果有换行符，就切换到下一行
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
//        else if((col + game_x > (LCD16032_BYTE_X/2) - 1) && game_y == 0) //第一行过长，自动切换到第二行
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



