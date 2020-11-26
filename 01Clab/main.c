#include "main.h"

void delay(uint64_t tiks) {
	for (uint64_t i = 0; i < tiks; i++)
		;
}

void Init() {
	//Включаем порты PC0-PC3 на которых висят кнопки
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	
	GPIOB->CRH &= ~(GPIO_CRH_MODE10 | GPIO_CRH_CNF10);
	GPIOB->CRH &= ~(GPIO_CRH_MODE11 | GPIO_CRH_CNF11);
	GPIOB->CRH &= ~(GPIO_CRH_MODE12 | GPIO_CRH_CNF12);
	GPIOB->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
	
	GPIOB->CRH  |= GPIO_CRH_CNF10_1;
	GPIOB->BSRR |= GPIO_BSRR_BR10;
	GPIOB->CRH  |= GPIO_CRH_CNF11_1;
	GPIOB->BSRR |= GPIO_BSRR_BR11;
	GPIOB->CRH  |= GPIO_CRH_CNF12_1;
	GPIOB->BSRR |= GPIO_BSRR_BR12;
	GPIOB->CRH  |= GPIO_CRH_CNF13_1;
	GPIOB->BSRR |= GPIO_BSRR_BR13;
	
	//Включаем динамик
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	
	GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
	GPIOC->CRH |= GPIO_CRH_MODE13;
}

uint8_t getButton(void) {
	uint32_t i = GPIOB->IDR;
	if (i & GPIO_IDR_IDR10) {
		return 1;
	}
	if (i & GPIO_IDR_IDR11) {
		return 2;
	}
	if (i & GPIO_IDR_IDR12) {
		return 3;
	}
	if (i & GPIO_IDR_IDR13) {
		return 4;
	}
	return 0;
} 

int main(void) {
	Init();
	while (1) {
		uint8_t b = getButton();
		if (b > 0) {
			delay(DIANA*b);
			SPEAKER_ON();
			delay(DIANA*b);
			SPEAKER_OFF();
		}
	}
}
