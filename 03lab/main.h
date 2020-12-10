#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f10x.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define B_LEN 128

#define SendOK()  USARTTransmit("OK\n\r")
#define SendINV() USARTTransmit("Invalid parameter\n\r")
#define SendUNK() USARTTransmit("Unknown Command\n\r")

#define MaskAll() EXTI->IMR   &= ~(EXTI_IMR_MR10 | EXTI_IMR_MR11 | EXTI_IMR_MR12 | EXTI_IMR_MR13)
#define UnmaskAll() EXTI->IMR |= EXTI_IMR_MR10 | EXTI_IMR_MR11 | EXTI_IMR_MR12 | EXTI_IMR_MR13


enum BUTTON {
	FIRST = 1,
	SECOND,
	THIRD,
	FOURTH,
	NOT_SELECTED = UINT8_MAX,
};

void Init(void);

void HandleCommand(void);
void ProcessButton(enum BUTTON b);
void TIM2SetFreq(uint32_t freq);
void USARTTransmit(const char* buf);
void USARTSendChar(const char c);

void EXTI15_10_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void USART2_IRQHandler(void);

#endif // _MAIN_H
