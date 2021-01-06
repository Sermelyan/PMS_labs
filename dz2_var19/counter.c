#include "counter.h"

void InitCounter(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;
	
	GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);
	GPIOA->CRL |= GPIO_CRL_CNF1_0;
	
	AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI1_PA;
	
	EXTI->RTSR |= EXTI_RTSR_TR1;
	EXTI->IMR  |= EXTI_IMR_MR1;
	
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_SetPriority(EXTI1_IRQn, 0);
}

static uint32_t counter = 0;

void EXTI1_IRQHandler(void) {
	EXTI->PR = EXTI_PR_PR1;
	counter++;
}

uint32_t GetCount(void) {
	return counter;
}
