#include "lcd.h"
#include "stdbool.h"
#include "stm32l476xx.h"
#include "stm32l4xx_hal.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h> // For strlen();

/* PC1-8 = DB0-7 */
void DB_set(uint8_t val)
{
	GPIOC->ODR &= ~(0b11111111 << GPIO_ODR_OD1_Pos);
	GPIOC->ODR |= (val << GPIO_ODR_OD1_Pos);
}

/* PC9 = RS */
void RS_set(bool val)
{
	GPIOC->ODR &= ~GPIO_ODR_OD9_Msk;
	GPIOC->ODR |= (val << GPIO_ODR_OD9_Pos);
}

/* PC0 = E */
void E_set(bool val)
{
	GPIOC->ODR &= ~GPIO_ODR_OD0_Msk;
	GPIOC->ODR |= val << GPIO_ODR_OD0_Pos;
}

void bus_write(uint8_t val, bool set_RS)
{
	E_set(false);
	HAL_Delay(1);
	RS_set(set_RS);
	HAL_Delay(1);
	E_set(true);
	HAL_Delay(DELAY_SETUP_TIME);
	DB_set(val);
	HAL_Delay(DELAY_SETUP_TIME);
	E_set(false);
	HAL_Delay(DELAY_SETUP_TIME);
}

void instruction_send(uint8_t val)
{
	bus_write(val, false);
}

void data_send(uint8_t val)
{
	bus_write(val, true);
}

uint8_t instruction_mask_create(uint8_t offset, uint8_t argument_count, bool *arguments)
{
	uint8_t mask = 0;
	mask |= (1 << offset);

	for (uint8_t i = 0; i < argument_count; i++) {
		uint8_t argument_offset = offset - 1 - i;
		mask |= (arguments[i] << argument_offset);
	}

	return mask;
}

void display_clear()
{
	instruction_send(INSTRUCTION_DISPLAY_CLEAR_Msk);
}

void entry_mode_set(bool increment, bool display_shift_on)
{
	static const uint8_t argument_count = 2;
	bool arguments[argument_count];

	arguments[0] = increment;
	arguments[1] = display_shift_on;

	uint8_t mask = instruction_mask_create(INSTRUCTION_ENTRY_MODE_SET_Pos, argument_count, arguments);

	instruction_send(mask);
}

void display_on_off(bool display_on, bool cursor_on, bool cursor_blink)
{
	static const uint8_t argument_count = 3;
	bool arguments[argument_count];

	arguments[0] = display_on;
	arguments[1] = cursor_on;
	arguments[2] = cursor_blink;

	uint8_t mask = instruction_mask_create(INSTRUCTION_DISPLAY_ON_OFF_Pos, argument_count, arguments);

	instruction_send(mask);
}

void display_cursor_shift(bool shift_display, bool right)
{
}

void function_set(bool byte_mode, bool dual_line, bool font)
{
	static const uint8_t argument_count = 3;
	bool arguments[argument_count];

	arguments[0] = byte_mode;
	arguments[1] = dual_line;
	arguments[2] = font;

	uint8_t mask = instruction_mask_create(INSTRUCTION_FUNCTION_SET_Pos, argument_count, arguments);

	instruction_send(mask);
}

void ddram_address_set(uint8_t addr) {
	uint8_t mask = INSTRUCTION_DDRAM_ADDRESS_SET_Msk | addr;

	instruction_send(mask);
}

void line_set(bool second_line) {
	if (second_line)
		ddram_address_set(DDRAM_ADDRESS_SECOND_LINE);
	else
		ddram_address_set(DDRAM_ADDRESS_FIRST_LINE);
}

// TODO: need to have display ON/OFF configured
void LCD_write_char(uint8_t letter)
{
	data_send(letter);
}

void LCD_print(const char *message, enum Line line)
{
	size_t length = strlen(message);
	if (length > 16) {
		message = "BAD LENGTH";
	}

	if (line == FIRST)
	 	line_set(false);
	else
		line_set(true);
	
	for (int i = 0; i < length; i++) {
		LCD_write_char(message[i]);
	}

	return;
}

void bus_init()
{
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOCEN);
}

void display_init(bool dual_line, bool large_font, bool cursor_on, bool cursor_blink, bool increment_mode, bool display_shift_on)
{
	function_set(true, dual_line, large_font);
	HAL_Delay(100);
	function_set(true, dual_line, large_font);
	HAL_Delay(100);
	function_set(true, dual_line, large_font);
	HAL_Delay(100);
	display_on_off(true, cursor_on, cursor_blink);
	HAL_Delay(100);
	display_clear();
	HAL_Delay(100);
	entry_mode_set(increment_mode, display_shift_on);
}

// TODO: check validity
void gpio_init()
{
	bus_init();

	//Set PC0-9 as input mode
	GPIOC->MODER &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1 | GPIO_MODER_MODE2 | GPIO_MODER_MODE3);
	GPIOC->MODER &= ~(GPIO_MODER_MODE4 | GPIO_MODER_MODE5 | GPIO_MODER_MODE6);
	GPIOC->MODER &= ~(GPIO_MODER_MODE7 | GPIO_MODER_MODE8 | GPIO_MODER_MODE9);

	GPIOC->MODER |= (GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0 | GPIO_MODER_MODE2_0 | GPIO_MODER_MODE3_0);
	GPIOC->MODER |= (GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0);
	GPIOC->MODER |= (GPIO_MODER_MODE7_0 | GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0);

	return;
}

