#include "Button.h"

Button::Button(EXTI_TypeDef * _exti, uint32_t _tr, uint8_t _scale)
: exti(_exti),
	tr(_tr),
	scale(_scale),
	st(STATE::DISABLED)
{
	this->exti->FTSR |= this->tr;
}
		
uint8_t Button::ChangeState() {
	if (this->st == STATE::DISABLED) {
		this->exti->FTSR ^= this->tr;
		this->exti->RTSR |= this->tr;
		this->st = STATE::ENABLED;
		return this->scale;
	} else {
		this->exti->FTSR |= this->tr;
		this->exti->RTSR ^= this->tr;
		this->st = STATE::DISABLED;
		return 0;
	}
}
