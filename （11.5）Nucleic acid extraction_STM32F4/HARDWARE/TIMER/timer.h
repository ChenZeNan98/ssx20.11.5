#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"



void TIM2_Int_Init(u16 time_ms);
void NVIC_Configuration_TIM2(void);
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_IRQHandler(void) ;
void TIM3_PWM_Init(u16 arr,u16 psc);
	
//void TIM1_Int_Init(u16 arr,u16 psc);
//void TIM2_PWM_Init(u16 arr, u16 psc);
//void TIM4_PWM_Init(u16 arr,u16 psc);
//void TIM1_CAP_Init(u16 arr,u16 psc);
//void TIM3_Int_Init(void);
//void TIM4_Int_Init(u16 arr,u16 psc);
//void TIM5_Int_Init(u16 arr,u16 psc);
//void PWM_rest(void);

#endif
