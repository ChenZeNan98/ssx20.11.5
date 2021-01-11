#ifndef __CFG_H__
#define __CFG_H__

sbit RV1 = P3^4;			//电子锁对应IO 	zhaohq
sbit RV2 = P3^5;			//电子锁对应IO 	zhaohq

#if 0
#define lock_open()	   		{	RV1 = 1; RV2 = 0;}
#define lock_close()		{   RV1 = 0; RV2 = 1;}
#define lock_keep()			{   RV1 = 0; RV2 = 0;}


#define OPEN_LOCK				1		   //开锁
#define CLOSE_LOCK				0		   //关锁

#define OP_LOCK_CRL				17		   	/*电子锁控制选项*/
#define LOCK_SW_KEEP_TIME		1000		/*电子锁动作到切换到掉电保持的时间*/
#endif

#endif