#include "init.h"
#include "keypad.h"
#include "led.h"
#include "lcd.h"

#include "stdbool.h"
#include "stm32l4xx_hal.h"
#include <stddef.h>
#include <stdint.h>

int main()
{
	gpio_init();
	display_init(true, false, true, true, true, false);

	LCD_print("Hello there", FIRST);
	LCD_print("Key is 0", SECOND);
	LED_turn_on();

	HAL_Delay(3000);

	while (1) {
		int key = get_pressed_key();

		if (key == INVALID_VALUE)
			continue;

		if (key != 0) {
			display_clear();
			HAL_Delay(2000);

			LCD_print("Kenobi!", FIRST);
			LCD_print("hi!", SECOND);
			LED_turn_on();
		}

		HAL_Delay(1000);
	}
}
