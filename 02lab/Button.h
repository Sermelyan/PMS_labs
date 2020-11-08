#ifndef _BUTTON_H
#define _BUTTON_H

#include "stm32f10x.h"

#include <stdint.h>

enum class STATE : uint8_t{
	DISABLED,
	ENABLED
};

class Button {
	private:
		EXTI_TypeDef *exti;
		uint32_t tr;
		STATE st;
		uint8_t scale;
	public:
		Button(EXTI_TypeDef * _exti, uint32_t _tr, uint8_t _scale);
		
		uint8_t ChangeState();
};

#endif //_BUTTON_H
