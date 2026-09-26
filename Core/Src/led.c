// led.c
#include "led.h"
#include "stdbool.h"
#include "stm32l476xx.h"
#include <stdint.h>

void LED_turn_off()
{
	GPIOA->ODR &= ~GPIO_ODR_OD4;
}

void LED_turn_on() {
    GPIOA->ODR |= GPIO_ODR_OD4;
}
