#ifndef __CFG_H__
#define __CFG_H__

sbit RV1 = P3^4;			//��������ӦIO 	zhaohq
sbit RV2 = P3^5;			//��������ӦIO 	zhaohq

#if 0
#define lock_open()	   		{	RV1 = 1; RV2 = 0;}
#define lock_close()		{   RV1 = 0; RV2 = 1;}
#define lock_keep()			{   RV1 = 0; RV2 = 0;}


#define OPEN_LOCK				1		   //����
#define CLOSE_LOCK				0		   //����

#define OP_LOCK_CRL				17		   	/*����������ѡ��*/
#define LOCK_SW_KEEP_TIME		1000		/*�������������л������籣�ֵ�ʱ��*/
#endif

#endif