#ifndef __MAIN_H
#define	__MAIN_H

#include "stm32f10x.h"  

// Величина задержки между вкл/выкл светодиодов
#define DELAY_VAL	500000

#define	LED_ON()	GPIOC->BSRR = GPIO_BSRR_BS13
#define	LED_OFF()	GPIOC->BSRR = GPIO_BSRR_BR13

/* Прототипы функций */
void delay(unsigned long takts);

#endif
