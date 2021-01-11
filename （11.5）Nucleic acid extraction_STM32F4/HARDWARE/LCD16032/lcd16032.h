

//////////lcm16032 HAL¿â Í·ÎÄ¼þ/////////

#ifndef _LCD16032_H
#define _LCD16032_H
#include "sys.h"

#define SID_H	GPIO_SetBits(GPIOC,GPIO_Pin_7)   
#define SID_L	GPIO_ResetBits(GPIOC,GPIO_Pin_7)

#define CS_H	GPIO_SetBits(GPIOC,GPIO_Pin_8)
#define CS_L	GPIO_ResetBits(GPIOC,GPIO_Pin_8)

#define SCLK_H	GPIO_SetBits(GPIOC,GPIO_Pin_6)
#define SCLK_L	GPIO_ResetBits(GPIOC,GPIO_Pin_6)

#define BLA_H GPIO_SetBits(GPIOC,GPIO_Pin_9)
#define BLA_L GPIO_ResetBits(GPIOC,GPIO_Pin_9)

void lcd16032_send_byte(uint8_t byte);
uint8_t lcd16032_read_byte(void);
uint8_t lcd16032_read_busy(void);
uint8_t lcd16032_write_cmd(uint8_t cmd);
uint8_t lcd16032_write_data(uint8_t byte);
void lcd16032_init(void);
void Lcds_Config(void);

uint8_t lcd16032_send_char(uint8_t byte);
uint8_t lcd16032_show_char(uint8_t x, uint8_t y, uint8_t byte);
uint8_t lcd16032_send_string(uint8_t *str);
uint8_t lcd16032_set_xy(uint8_t x, uint8_t y);

uint8_t lcd16032_show_string(uint8_t x, uint8_t y, uint8_t *str);

char* split(char * p, char * str);
#endif

