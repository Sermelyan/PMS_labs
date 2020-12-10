#include "main.h"

static volatile uint8_t State = NOT_SELECTED;
static volatile uint8_t prev = NOT_SELECTED;
static const uint32_t CpuFreq = 36000;

static char RxBuffer[B_LEN];

void HandleCommand(void) {
	uint32_t d = 0;
	if (strncmp(RxBuffer, "*IDN?", 5) == 0) {
		USARTTransmit("Zakharova-Melikyan-IU4-73\n\r");
		return;
	}
	if (strncmp(RxBuffer, "TONE", 4) == 0) {
		if (sscanf(RxBuffer, "TONE %u", &d) == 1 && d >= 1 && d <= 4) {
			TIM2SetFreq(d * 1000);
			TIM4->CR1 |= TIM_CR1_CEN;
			MaskAll();
			SendOK();
		} else {
			SendINV();
		}
		return;
	}
	if (strncmp(RxBuffer, "FREQ", 4) == 0) {
		if (strcmp(RxBuffer, "FREQ OFF") == 0) {
			TIM2->CR1 &= ~TIM_CR1_CEN;
			UnmaskAll();
			SendOK();
		} else if (sscanf(RxBuffer, "FREQ %u", &d) == 1 && d >= 1 && d <= 9999) {
			TIM2SetFreq(d);
			MaskAll();
			SendOK();
		} else {
			SendINV();
		}
		return;
	}
	SendUNK();
}

void TIM2SetFreq(uint32_t freq) {
	uint16_t p = (uint16_t)(CpuFreq/freq);
	TIM2->PSC = p - 1;
	TIM2->CR1 |= TIM_CR1_CEN;
}

void ProcessButton(enum BUTTON b) {
	if (State == NOT_SELECTED) {
		TIM2SetFreq(b * 1000);
		State = b;
		prev = b;
	} else if (b == State) {
		State = NOT_SELECTED;
		prev = b;
		TIM2->CR1 &= ~TIM_CR1_CEN;
		GPIOC->BSRR |= GPIO_BSRR_BR13;
	}
	TIM3->CR1 |= TIM_CR1_CEN;
}

void EXTI15_10_IRQHandler(void) {
	if (EXTI->PR & EXTI_PR_PR10) {
		EXTI->IMR &= ~EXTI_IMR_MR10;
		EXTI->PR = EXTI_PR_PR10;
		ProcessButton(FIRST);
	} else if (EXTI->PR & EXTI_PR_PR11) {
		EXTI->IMR &= ~EXTI_IMR_MR11;
		EXTI->PR = EXTI_PR_PR11;
		ProcessButton(SECOND);
	} else if (EXTI->PR & EXTI_PR_PR12) {
		EXTI->IMR &= ~EXTI_IMR_MR12;
		EXTI->PR = EXTI_PR_PR12;
		ProcessButton(THIRD);
	} else if (EXTI->PR & EXTI_PR_PR13) {
		EXTI->IMR &= ~EXTI_IMR_MR13;
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

void TIM4_IRQHandler(void) {
	TIM4->SR &= ~TIM_SR_UIF;
	TIM2->CR1 &= ~TIM_CR1_CEN;
	GPIOC->BSRR |= GPIO_BSRR_BR13;
	UnmaskAll();
}

void USART2_IRQHandler(void) {
	if (USART2->SR & USART_SR_RXNE) {
		uint32_t len = strlen(RxBuffer);
		char c = USART2->DR;
		if (c == '\r' || len == B_LEN) {
			USARTTransmit("\n\r");
			HandleCommand();
			memset(RxBuffer, 0, B_LEN);
			return;
		}
		RxBuffer[len] = c;
		USARTSendChar(c);
	}
}

void USARTTransmit(const char* buf) {
	for (uint32_t i = 0; i < strlen(buf); i++) {
		USARTSendChar(buf[i]);
	}
}

void USARTSendChar(const char c) {
	USART2->DR = c;
	while (!(USART2->SR & USART_SR_TC))
		;
}

void Init() {
	//Включаем порты PB10-PB13 на которых висят кнопки
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

  //Включаем таймеры
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM3EN | RCC_APB1ENR_TIM4EN;
	
	TIM2->ARR = 2000;
	TIM2->DIER |= TIM_DIER_UIE;
	
	NVIC_EnableIRQ(TIM2_IRQn);
	
	TIM3->PSC = 36000-1;
	TIM3->ARR = 80;
	TIM3->DIER |= TIM_DIER_UIE;
	TIM3->CR1 |= TIM_CR1_OPM;
	
	NVIC_EnableIRQ(TIM3_IRQn);
	
	TIM4->PSC = 36000-1;
	TIM4->ARR = 6000;
	TIM4->DIER |= TIM_DIER_UIE;
	TIM4->CR1 |= TIM_CR1_OPM;
	
	NVIC_EnableIRQ(TIM4_IRQn);
	
	//Включаем динамик
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	
	GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
	GPIOC->CRH |= GPIO_CRH_MODE13;
	
	//Инициализируем usart
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	GPIOA->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2 | GPIO_CRL_MODE3 | GPIO_CRL_CNF3);
  GPIOA->CRL |= (GPIO_CRL_MODE2 | GPIO_CRL_CNF2_1 | GPIO_CRL_CNF3_0);
	
	USART2->BRR = 0x753;
	
	USART2->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
	USART2->CR1 |= USART_CR1_RXNEIE; //разрешить прерывание по приему байта данных
	
	NVIC_EnableIRQ(USART2_IRQn);
	memset(RxBuffer, 0, B_LEN);
}

int main(void) {
	Init();
	while (1)
		;
}
