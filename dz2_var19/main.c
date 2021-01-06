#include "main.h"
#include "display.h"
#include "counter.h"

static void InitPorts(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPAEN;
	
	GPIOB->CRH &= ~(GPIO_CRH_MODE9 | GPIO_CRH_CNF9);
	GPIOB->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10);
	GPIOB->CRH &= ~(GPIO_CRH_MODE11 | GPIO_CRH_CNF11);
	GPIOB->CRH &= ~(GPIO_CRH_MODE12 | GPIO_CRH_CNF12);
	GPIOB->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
	GPIOB->CRH &= ~(GPIO_CRH_MODE14 | GPIO_CRH_CNF14);
	GPIOB->CRH &= ~(GPIO_CRH_MODE15 | GPIO_CRH_CNF15);
	
	GPIOB->CRH |= GPIO_CRH_MODE9;
	GPIOB->CRH |= GPIO_CRH_MODE10;
	GPIOB->CRH |= GPIO_CRH_MODE11;
	GPIOB->CRH |= GPIO_CRH_MODE12;
	GPIOB->CRH |= GPIO_CRH_MODE13;
	GPIOB->CRH |= GPIO_CRH_MODE14;
	GPIOB->CRH |= GPIO_CRH_MODE15;
	
	GPIOA->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
	GPIOA->CRH &= ~(GPIO_CRH_MODE14 | GPIO_CRH_CNF14);
	GPIOA->CRH &= ~(GPIO_CRH_MODE15 | GPIO_CRH_CNF15);
	
	GPIOA->CRH |= GPIO_CRH_MODE13;
	GPIOA->CRH |= GPIO_CRH_MODE14;
	GPIOA->CRH |= GPIO_CRH_MODE15;
}

static void InitTimer(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	
	TIM3->PSC = 36000-1;
	TIM3->ARR = 5;
	TIM3->DIER |= TIM_DIER_UIE;
	
	NVIC_EnableIRQ(TIM3_IRQn);
	NVIC_SetPriority(TIM3_IRQn, 2);
	TIM3->CR1 |= TIM_CR1_CEN;
}

//2 Pi * Radius (metres);
static float wheelLentgh = (float) 6.28 * (float) 0.27; 

void TIM3_IRQHandler(void) {
	TIM2->SR &= ~TIM_SR_UIF;
	PrintNumber((uint32_t)(wheelLentgh * GetCount()));
}
int main() {
	InitPorts();
	InitCounter();
	InitDisplay();
	InitTimer();
	
	while(1)
		;
}
