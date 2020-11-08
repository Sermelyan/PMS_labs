#include "main.h"

static uint8_t ButtonStates[4] = {DISABLED, DISABLED, DISABLED, DISABLED};
static uint8_t Prescaler = NOT_SELECTED;

void ProcessButton(enum BUTTON b) {
	if (ButtonStates[b] == DISABLED) {
		ButtonStates[b] = ENABLED;
		Prescaler = b + 1;
		switch (b) {
			case FIRST:
				EXTI->FTSR |= ~EXTI_FTSR_TR0;
				EXTI->RTSR |= EXTI_RTSR_TR0;
				break;
			case SECOND:
				EXTI->FTSR |= ~EXTI_FTSR_TR1;
				EXTI->RTSR |= EXTI_RTSR_TR1;
				break;
			case THIRD:
				EXTI->FTSR |= ~EXTI_FTSR_TR2;
				EXTI->RTSR |= EXTI_RTSR_TR2;
				break;
			case FOURTH:
				EXTI->FTSR |= ~EXTI_FTSR_TR3;
				EXTI->RTSR |= EXTI_RTSR_TR3;
				break;
			case NOT_SELECTED:
				//Такое значение сюда попадать не должно
				break;
		}
	} else {
		ButtonStates[b] = DISABLED;
		Prescaler = NOT_SELECTED;
		switch (b) {
			case FIRST:
				EXTI->FTSR |= EXTI_FTSR_TR0;
				EXTI->RTSR |= ~EXTI_RTSR_TR0;
				break;
			case SECOND:
				EXTI->FTSR |= EXTI_FTSR_TR1;
				EXTI->RTSR |= ~EXTI_RTSR_TR1;
				break;
			case THIRD:
				EXTI->FTSR |= EXTI_FTSR_TR2;
				EXTI->RTSR |= ~EXTI_RTSR_TR2;
				break;
			case FOURTH:
				EXTI->FTSR |= EXTI_FTSR_TR3;
				EXTI->RTSR |= ~EXTI_RTSR_TR3;
				break;
			case NOT_SELECTED:
				//Такое значение сюда попадать не должно
				break;
		}
	}
}

void EXTI0_IRQHandler() {
	ProcessButton(FIRST);
	EXTI->PR |= EXTI_PR_PR0;
}

void EXTI1_IRQHandler() {
	ProcessButton(SECOND);
	EXTI->PR |= EXTI_PR_PR1;
}

void EXTI2_IRQHandler() {
	ProcessButton(THIRD);
	EXTI->PR |= EXTI_PR_PR2;
}

void EXTI3_IRQHandler() {
	ProcessButton(FOURTH);
	EXTI->PR |= EXTI_PR_PR3;
}

void delay(uint64_t tiks) {
	for (uint64_t i = 0; i < tiks; i++)
		;
}

void Init() {
	//Включаем порты PC0-PC3 на которых висят кнопки
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN;
	
	GPIOC->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
	GPIOC->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);
	GPIOC->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF3);
	GPIOC->CRL &= ~(GPIO_CRL_MODE3 | GPIO_CRL_CNF3);
	
	GPIOC->CRL  |= GPIO_CRL_CNF0_1;
	GPIOC->BSRR |= GPIO_BSRR_BS0;
	GPIOC->CRL  |= GPIO_CRL_CNF1_1;
	GPIOC->BSRR |= GPIO_BSRR_BS1;
	GPIOC->CRL  |= GPIO_CRL_CNF2_1;
	GPIOC->BSRR |= GPIO_BSRR_BS2;
	GPIOC->CRL  |= GPIO_CRL_CNF3_1;
	GPIOC->BSRR |= GPIO_BSRR_BS3;
	
	//Настравиваем прерывания по нажатию на кнопку
	AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI0_PC;
	AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI1_PC;
	AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI2_PC;
	AFIO->EXTICR[0] |= AFIO_EXTICR1_EXTI3_PC;
	
	EXTI->FTSR |= EXTI_FTSR_TR0;
	EXTI->IMR  |= EXTI_IMR_MR0;
	EXTI->FTSR |= EXTI_FTSR_TR1;
	EXTI->IMR  |= EXTI_IMR_MR1;
	EXTI->FTSR |= EXTI_FTSR_TR2;
	EXTI->IMR  |= EXTI_IMR_MR2;
	EXTI->FTSR |= EXTI_FTSR_TR3;
	EXTI->IMR  |= EXTI_IMR_MR3;
	
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_SetPriority(EXTI0_IRQn, 0);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_SetPriority(EXTI1_IRQn, 1);
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_SetPriority(EXTI2_IRQn, 2);
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_SetPriority(EXTI3_IRQn, 3);
	
	//Включаем динамик
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);
	GPIOA->CRL |= GPIO_CRL_MODE1;
}

int main(void) {
	Init();
	while (1) {
		if (Prescaler != NOT_SELECTED) {
			delay(DIANA*Prescaler);
			SPEAKER_ON();
			delay(DIANA*Prescaler);
			SPEAKER_OFF();
		}
	}
}
