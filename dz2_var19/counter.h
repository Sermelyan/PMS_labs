#ifndef _COUNTER_H_
#define _COUNTER_H_

#include <stdint.h>

#include "stm32f10x.h"

void InitCounter(void);
uint32_t GetCount(void);
void EXTI1_IRQHandler(void);

#endif //_COUNTER_H_
