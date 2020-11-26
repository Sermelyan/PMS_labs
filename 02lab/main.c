#include "main.h"

static uint8_t Prescaler = NOT_SELECTED;

void ProcessButton(enum BUTTON b) {
	if (Prescaler == NOT_SELECTED) {
		switch (b) {
			case FIRST:
				EXTI->FTSR |= EXTI_FTSR_TR10;
				EXTI->RTSR ^= EXTI_RTSR_TR10;
			break;
			case SECOND:
				EXTI->FTSR |= EXTI_FTSR_TR11;
				EXTI->RTSR ^= EXTI_RTSR_TR11;
			break;
			case THIRD:
				EXTI->FTSR |= EXTI_FTSR_TR12;
				EXTI->RTSR ^= EXTI_RTSR_TR12;
			break;
			case FOURTH:
				EXTI->FTSR |= EXTI_FTSR_TR13;
				EXTI->RTSR ^= EXTI_RTSR_TR13;
			break;
			case NOT_SELECTED:
			break;
		}
		Prescaler = b;
	} else if (b == Prescaler) {
		switch (b) {
			case FIRST:
				EXTI->FTSR ^= EXTI_FTSR_TR10;
				EXTI->RTSR |= EXTI_RTSR_TR10;
			break;
			case SECOND:
				EXTI->FTSR ^= EXTI_FTSR_TR11;
				EXTI->RTSR |= EXTI_RTSR_TR11;
			break;
			case THIRD:
				EXTI->FTSR ^= EXTI_FTSR_TR12;
				EXTI->RTSR |= EXTI_RTSR_TR12;
			break;
			case FOURTH:
				EXTI->FTSR ^= EXTI_FTSR_TR13;
				EXTI->RTSR |= EXTI_RTSR_TR13;
			break;
			case NOT_SELECTED:
			break;
		}
		Prescaler = NOT_SELECTED;
	}
}

void EXTI15_10_IRQHandler(void) {
	if (EXTI->PR & EXTI_PR_PR10) {
		EXTI->PR = EXTI_PR_PR10;
		ProcessButton(FIRST);
	} else if (EXTI->PR & EXTI_PR_PR11) {
		EXTI->PR = EXTI_PR_PR11;
		ProcessButton(SECOND);
	} else if (EXTI->PR & EXTI_PR_PR12) {
		EXTI->PR = EXTI_PR_PR12;
		ProcessButton(THIRD);
	} else if (EXTI->PR & EXTI_PR_PR13) {
		EXTI->PR = EXTI_PR_PR13;
		ProcessButton(FOURTH);
	}
	delay(100);
}

void delay(uint64_t tiks) {
	for (uint64_t i = 0; i < tiks; i++)
		;
}

void Init() {
	//Включаем порты PC0-PC3 на которых висят кнопки
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
	
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
	
	//Настравиваем прерывания по нажатию на кнопку
	AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI10_PB;
	AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI11_PB;
	AFIO->EXTICR[3] |= AFIO_EXTICR4_EXTI12_PB;
	AFIO->EXTICR[3] |= AFIO_EXTICR4_EXTI13_PB;
	
	EXTI->RTSR |= EXTI_RTSR_TR10;
	EXTI->IMR  |= EXTI_IMR_MR10;
	EXTI->RTSR |= EXTI_RTSR_TR11;
	EXTI->IMR  |= EXTI_IMR_MR11;
	EXTI->RTSR |= EXTI_RTSR_TR12;
	EXTI->IMR  |= EXTI_IMR_MR12;
	EXTI->RTSR |= EXTI_RTSR_TR13;
	EXTI->IMR  |= EXTI_IMR_MR13;
	
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	NVIC_SetPriority(EXTI15_10_IRQn, 0);
	
	//Включаем динамик
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	
	GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
	GPIOC->CRH |= GPIO_CRH_MODE13;
}

int main(void) {
	Init();
	while (1) {
		if (Prescaler != NOT_SELECTED) {
			delay(DIANA*(Prescaler+1));
			SPEAKER_ON();
			delay(DIANA*(Prescaler+1));
			SPEAKER_OFF();
		}
	}
}
