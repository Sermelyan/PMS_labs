#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "stm32f10x.h"

#define setA() GPIOB->BSRR = GPIO_BSRR_BS15
#define setB() GPIOB->BSRR = GPIO_BSRR_BS14
#define setC() GPIOB->BSRR = GPIO_BSRR_BS13
#define setD() GPIOB->BSRR = GPIO_BSRR_BS12
#define setE() GPIOB->BSRR = GPIO_BSRR_BS11
#define setF() GPIOB->BSRR = GPIO_BSRR_BS10
#define setG() GPIOB->BSRR = GPIO_BSRR_BS9

#define resetA() GPIOB->BSRR = GPIO_BSRR_BR15
#define resetB() GPIOB->BSRR = GPIO_BSRR_BR14
#define resetC() GPIOB->BSRR = GPIO_BSRR_BR13
#define resetD() GPIOB->BSRR = GPIO_BSRR_BR12
#define resetE() GPIOB->BSRR = GPIO_BSRR_BR11
#define resetF() GPIOB->BSRR = GPIO_BSRR_BR10
#define resetG() GPIOB->BSRR = GPIO_BSRR_BR9

#define select1() GPIOA->BSRR = GPIO_BSRR_BR13
#define disSel1() GPIOA->BSRR = GPIO_BSRR_BS13
											
#define select2() GPIOA->BSRR = GPIO_BSRR_BR14
#define disSel2() GPIOA->BSRR = GPIO_BSRR_BS14
											
#define select3() GPIOA->BSRR = GPIO_BSRR_BR15
#define disSel3() GPIOA->BSRR = GPIO_BSRR_BS15

void Init(void);
void TIM2_IRQHandler(void);

#endif // _DISPLAY_H_
