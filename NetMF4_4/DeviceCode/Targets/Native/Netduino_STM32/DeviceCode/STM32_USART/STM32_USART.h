#ifndef STM32_USART_H
#define STM32_USART_H

struct STM32USARTPins
{
	GPIO_PIN rx;
	GPIO_PIN tx;
	GPIO_PIN cts;
	GPIO_PIN rts;
};

struct STM32USART
{
	USART_TypeDef* usart;
	UINT32 irq;
	STM32USARTPins pins;
};

#endif
