#include "display.h"

static inline void setAll(void) {
	setA();
	setB();
	setC();
	setD();
	setE();
	setF();
	setG();
}

static inline void resetAll(void) {
	resetA();
	resetB();
	resetC();
	resetD();
	resetE();
	resetF();
	resetG();
}

static inline void print0(void) {
	setA();
	setB();
	setC();
	setD();
	setE();
	setF();
}

static inline void print1(void) {
	setB();
	setC();
}

static inline void print2(void) {
	setA();
	setB();
	setD();
	setE();
	setG();
}

static inline void print3(void) {
	setA();
	setB();
	setC();
	setD();
	setG();
}

static inline void print4(void) {
	setB();
	setC();
	setF();
	setG();
}

static inline void print5(void) {
	setA();
	setC();
	setD();
	setF();
	setG();
}

static inline void print6(void) {
	setA();
	setC();
	setD();
	setE();
	setF();
	setG();
}

static inline void print7(void) {
	setA();
	setB();
	setC();
}

static inline void print8(void) {
	setAll();
}

static inline void print9(void) {
	setA();
	setB();
	setC();
	setD();
	setF();
	setG();
}

void InitDisplay(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
	TIM2->PSC = 36000-1;
	TIM2->ARR = 1;
	TIM2->DIER |= TIM_DIER_UIE;
	
	NVIC_EnableIRQ(TIM2_IRQn);
	NVIC_SetPriority(TIM2_IRQn, 1);
	TIM2->CR1 |= TIM_CR1_CEN;
	
	disSel1();
	disSel2();
	disSel3();
}

static inline void printDigit(uint8_t d) {
	switch (d) {
		case 0:
			print0();
			return;
		case 1:
			print1();
			return;
		case 2:
			print2();
			return;
		case 3:
			print3();
			return;
		case 4:
			print4();
			return;
		case 5:
			print5();
			return;
		case 6:
			print6();
			return;
		case 7:
			print7();
			return;
		case 8:
			print8();
			return;
		case 9:
			print9();
			return;
		default:
			setA();
			setD();
			setE();
			setF();
			setG();
			return;
	}
}

static uint8_t firstD = 0;
static uint8_t seconD = 0;
static uint8_t thirdD = 0;

void PrintNumber(uint32_t number) {
	firstD = number % 10;
	seconD = (number / 10) % 10;
	thirdD = (number / 100) % 10;
}

static volatile uint8_t dp = 1;

void TIM2_IRQHandler(void) {
	TIM2->SR &= ~TIM_SR_UIF;
	resetAll();
	switch(dp) {
		case 1:
			disSel3();
			select1();
			printDigit(firstD);
			dp = 2;
			return;
		case 2:
			disSel1();
			select2();
			printDigit(seconD);
			dp = 3;
			return;
		case 3:
			disSel2();
			select3();
			printDigit(thirdD);
			dp = 1;
			return;
	}	
}
