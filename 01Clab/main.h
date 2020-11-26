#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f10x.h"

#include <stdint.h>

#define	SPEAKER_ON()		GPIOC->BSRR = GPIO_BSRR_BS13
#define	SPEAKER_OFF()		GPIOC->BSRR = GPIO_BSRR_BR13

#define DIANA 256

void Init(void);
void delay(uint64_t tiks);
uint8_t getButton(void);

#endif // _MAIN_H
