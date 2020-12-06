#include "main.h"

static volatile uint8_t State = NOT_SELECTED;
static volatile uint8_t prev = NOT_SELECTED;

void TIM2SetFreq(uint16_t freq) {
	TIM2->PSC = (72000000/freq) - 1;
}

void ProcessButton(enum BUTTON b) {
	if (State == NOT_SELECTED) {
		TIM2SetFreq((b+1)*1000);
		State = b;
		prev = b;
		TIM2->CR1 |= TIM_CR1_CEN;
	} else if (b == State) {
		State = NOT_SELECTED;
		prev = b;
		TIM2->CR1 ^= TIM_CR1_CEN;
		GPIOC->BSRR |= GPIO_BSRR_BR13;
	}
	TIM3->CR1 |= TIM_CR1_CEN;
}

void EXTI15_10_IRQHandler(void) {
	if (EXTI->PR & EXTI_PR_PR10) {
		EXTI->IMR ^= EXTI_IMR_MR10;
		EXTI->PR = EXTI_PR_PR10;
		ProcessButton(FIRST);
	} else if (EXTI->PR & EXTI_PR_PR11) {
		EXTI->IMR ^= EXTI_IMR_MR11;
		EXTI->PR = EXTI_PR_PR11;
		ProcessButton(SECOND);
	} else if (EXTI->PR & EXTI_PR_PR12) {
		EXTI->IMR ^= EXTI_IMR_MR12;
		EXTI->PR = EXTI_PR_PR12;
		ProcessButton(THIRD);
	} else if (EXTI->PR & EXTI_PR_PR13) {
		EXTI->IMR ^= EXTI_IMR_MR13;
		EXTI->PR = EXTI_PR_PR13;
		ProcessButton(FOURTH);
	}
}

void TIM2_IRQHandler(void) {
	TIM2->SR &= ~TIM_SR_UIF;
	GPIOC->ODR ^= GPIO_ODR_ODR13;
}

void TIM3_IRQHandler(void) {
	TIM3->SR &= ~TIM_SR_UIF;
	switch (prev) {
		case FIRST:
			EXTI->IMR |= EXTI_IMR_MR10;
		break;
		case SECOND:
			EXTI->IMR |= EXTI_IMR_MR11;
		break;
		case THIRD:
			EXTI->IMR |= EXTI_IMR_MR12;
		break;
		case FOURTH:
			EXTI->IMR |= EXTI_IMR_MR13;
		break;
		case NOT_SELECTED:
		break;
	}
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
	
	EXTI->FTSR |= EXTI_FTSR_TR10;
	EXTI->RTSR |= EXTI_RTSR_TR10;
	EXTI->IMR  |= EXTI_IMR_MR10;
	EXTI->FTSR |= EXTI_FTSR_TR11;
	EXTI->RTSR |= EXTI_RTSR_TR11;
	EXTI->IMR  |= EXTI_IMR_MR11;
	EXTI->FTSR |= EXTI_FTSR_TR12;
	EXTI->RTSR |= EXTI_RTSR_TR12;
	EXTI->IMR  |= EXTI_IMR_MR12;
	EXTI->FTSR |= EXTI_FTSR_TR13;
	EXTI->RTSR |= EXTI_RTSR_TR13;
	EXTI->IMR  |= EXTI_IMR_MR13;
	
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	NVIC_SetPriority(EXTI15_10_IRQn, 2);

  //Включаем таймеры
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM3EN;
	
	TIM2->ARR = 1;
	TIM2->DIER |= TIM_DIER_UIE;
	
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn, 1);
	
	TIM3->PSC = 36000-1;
	TIM3->ARR = 50-1;
	TIM3->DIER |= TIM_DIER_UIE;
	TIM3->CR1 |= TIM_CR1_OPM;
	
	NVIC_EnableIRQ(TIM3_IRQn);
	NVIC_SetPriority(TIM3_IRQn, 0);
	
	//Включаем динамик
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	
	GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
	GPIOC->CRH |= GPIO_CRH_MODE13;
}

int main(void) {
	Init();
	while (1)
		;
}
