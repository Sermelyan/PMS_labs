#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f10x.h"

#include <stdint.h>

enum BUTTON {
	FIRST,
	SECOND,
	THIRD,
	FOURTH,
	NOT_SELECTED = UINT8_MAX,
};

void Init(void);

void ProcessButton(enum BUTTON b);

void EXTI15_10_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);

#endif // _MAIN_H
