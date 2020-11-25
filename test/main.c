#include "main.h"

void delay(unsigned long takts) {
	for (unsigned long i = 0; i < takts; i++) 
		;
}

int main(void) {
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	GPIOC->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);
	GPIOC->CRH |= GPIO_CRH_MODE13_1;
	
	while (1) {
		LED_ON();
		delay(500000);
		LED_OFF();
		delay(500000);
	}
}
