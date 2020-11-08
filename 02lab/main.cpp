#include <vector>

#include "main.h"
#include "Button.h"

static std::vector<Button> Buttons;
static uint8_t Prescaler = 0;

void EXTI0_IRQHandler() {
	Prescaler = Buttons[0].ChangeState();
	EXTI->PR |= EXTI_PR_PR0;
}

void EXTI1_IRQHandler() {
	Prescaler = Buttons[1].ChangeState();
	EXTI->PR |= EXTI_PR_PR1;
}

void EXTI2_IRQHandler() {
	Prescaler = Buttons[2].ChangeState();
	EXTI->PR |= EXTI_PR_PR2;
}

void EXTI3_IRQHandler() {
	Prescaler = Buttons[3].ChangeState();
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
	
	Buttons.emplace_back(EXTI, EXTI_FTSR_TR0, 1);
	EXTI->IMR  |= EXTI_IMR_MR0;
	Buttons.emplace_back(EXTI, EXTI_FTSR_TR1, 2);
	EXTI->IMR  |= EXTI_IMR_MR1;
	Buttons.emplace_back(EXTI, EXTI_FTSR_TR2, 3);
	EXTI->IMR  |= EXTI_IMR_MR2;
	Buttons.emplace_back(EXTI, EXTI_FTSR_TR3, 4);
	EXTI->IMR  |= EXTI_IMR_MR3;
	
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_SetPriority(EXTI0_IRQn, 0);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_SetPriority(EXTI1_IRQn, 1);
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_SetPriority(EXTI2_IRQn, 2);
	NVIC_EnableIRQ(EXTI3_IRQn);
	NVIC_SetPriority(EXTI3_IRQn, 3);
	
	//Включаем динамик
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	GPIOA->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1);
	GPIOA->CRL |= GPIO_CRL_MODE1;
}

int main(void) {
	Init();
	while (1) {
		if (Prescaler) {
			delay(DIANA*Prescaler);
			SPEAKER_ON();
			delay(DIANA*Prescaler);
			SPEAKER_OFF();
		}
	}
}
