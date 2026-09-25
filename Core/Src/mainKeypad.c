/*
Assignment:	A2
Names: 		Chris Amey & Alan Kusparmakov
Professor: 	Dr. John Oliver 
Date: 		September 9th, 2026
Class: 		CPE-3160
*/

#include "main.h"

#include "stdbool.h"
#include "stm32l476xx.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_gpio.h"
#include <stdint.h>

/* On the Keypad, from left to right: Row1, Row2, Row3, Row4, Col.1, Col.2, Col.3
*/

#define ROW_COUNT 4
#define COL_COUNT 3
#define LED_COUNT 4

#define PC_ROW_START 0
#define PC_COLUMN_START 10
#define PA_LED_START 5

#define INVALID_VALUE -1

void bus_init() {
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOCEN);
}

void gpio_init()
{

	// set rows as input
	// PC0-3
	GPIOC->MODER &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1 | GPIO_MODER_MODE2 | GPIO_MODER_MODE3);

	// set rows as pull-down
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1 | GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD3);
	GPIOC->PUPDR |= (GPIO_PUPDR_PUPD0_1 | GPIO_PUPDR_PUPD1_1 | GPIO_PUPDR_PUPD2_1 | GPIO_PUPDR_PUPD3_1);

	// set cols as output
	// PC10-12
	GPIOC->MODER &= ~(GPIO_MODER_MODE10 | GPIO_MODER_MODE11 | GPIO_MODER_MODE12);
	GPIOC->MODER |= (GPIO_MODER_MODE10_0 | GPIO_MODER_MODE11_0 | GPIO_MODER_MODE12_0);

	// set LEDS as output
	// PA5-8
	GPIOA->MODER &= ~(GPIO_MODER_MODE5 | GPIO_MODER_MODE6 | GPIO_MODER_MODE7 | GPIO_MODER_MODE8);

	GPIOA->MODER |= (GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0 | GPIO_MODER_MODE8_0);
}

bool row_is_high(uint8_t i)
{
	return GPIOC->IDR & (1 << (PC_ROW_START + i));
}

void col_set(uint8_t j, bool high)
{
	GPIOC->ODR &= ~(1 << (PC_COLUMN_START + j));
	if (high)
		GPIOC->ODR |= (1 << (PC_COLUMN_START + j));
}

void cols_set_low()
{
	for (uint8_t j = 0; j < COL_COUNT; j++) {
		col_set(j, false);
	}
}

void col_isolate(uint8_t j)
{
	cols_set_low();
	col_set(j, true);
}

bool key_is_pressed(uint8_t row, uint8_t col)
{
	col_isolate(col);
	return row_is_high(row);
}

int get_key_value(uint8_t row, uint8_t col)
{
	// treat all rows but the last one as a regularly ordered numerical grid
	if (row < ROW_COUNT - 1)
		return (row * COL_COUNT) + col + 1;

	// treat the last row with special hardcoded values
	if (col == 0)
		return 10;

	if (col == 1)
		return 0;

	if (col == 2)
		return 15;

	return INVALID_VALUE;
}

int get_pressed_key()
{
	for (uint8_t i = 0; i < ROW_COUNT; i++)
		for (uint8_t j = 0; j < COL_COUNT; j++)
			if (key_is_pressed(i, j))
				return get_key_value(i, j);

	return INVALID_VALUE;
}

void LED_turn_off()
{
	GPIOA->ODR &= ~GPIO_ODR_OD5;
	GPIOA->ODR &= ~GPIO_ODR_OD6;
	GPIOA->ODR &= ~GPIO_ODR_OD7;
	GPIOA->ODR &= ~GPIO_ODR_OD8;
}

void count_display(uint8_t val)
{
	LED_turn_off();
	GPIOA->ODR |= ((val) << GPIO_ODR_OD5_Pos);
	// HAL_Delay(DISPLAY_DELAY);
}

int main(void)
{
	HAL_Init();
	
	bus_init();
	gpio_init();

	LED_turn_off();
	
	while (1) {
		int key = get_pressed_key();
		if (key == INVALID_VALUE)
			continue;
		
		count_display(key);

		HAL_Delay(1000);
	}
}
