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

void Init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	
	TIM2->PSC = 36000-1;
	TIM2->ARR = 20;
	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->CR1 |= TIM_CR1_OPM;
	
	NVIC_EnableIRQ(TIM2_IRQn);
	TIM2->CR1 |= TIM_CR1_CEN;
	
	disSel1();
	disSel2();
	disSel3();
}

static volatile char dp = 1;

void TIM2_IRQHandler(void) {
	TIM2->SR &= ~TIM_SR_UIF;
	resetAll();
	TIM2->CR1 |= TIM_CR1_CEN;
	switch(dp) {
		case 1:
			disSel3();
			select1();
			print3();
			dp = 2;
			return;
		case 2:
			disSel1();
			select2();
			print2();
			dp = 3;
			return;
		case 3:
			disSel2();
			select3();
			print1();
			dp = 1;
			return;
	}	
}
