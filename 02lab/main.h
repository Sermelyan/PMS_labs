#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f10x.h"

#include <stdint.h>

#define	SPEAKER_ON()		GPIOA->BSRR = GPIO_BSRR_BS1
#define	SPEAKER_OFF()		GPIOA->BSRR = GPIO_BSRR_BR1

#define DIANA 1000

void Init(void);
void delay(uint64_t tiks);

void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);

#endif //_MAIN_H
