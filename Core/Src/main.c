#include "init.h"
#include "keypad.h"
#include "led.h"
#include "lcd.h"
#include "stdbool.h"
#include "stm32l4xx_hal.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define INPUT_STAR 10
#define INPUT_POUND 15

void init()
{
	gpio_init();
	display_init(true, false, true, true, true, false);
}

void welcome_message()
{
	LCD_print("Welcome!", FIRST);
	LCD_print("Enter key here", SECOND);
}



enum State {
	LOCKED,
	UNLOCKED
};

int main()
{
	init();
	welcome_message();
	LED_turn_on();

	HAL_Delay(3000);

	char password[16] = { 0 };
	char attempt[16] = { 0 };

	strcpy(password, "1234");

	int keyInput = 0;
	int i = 0;

	enum State state = LOCKED;

	while (1) {
		switch (state) {

		case LOCKED:

			keyInput = get_pressed_key();
			if (keyInput == INVALID_VALUE) {
				continue;
			}

			if (keyInput == INPUT_STAR) {
				
			}



	}

		keyInput = get_pressed_key();
		if (keyInput == INVALID_VALUE)
			continue;

		if (keyInput == 10) {
			memset(attempt, 0, sizeof(attempt));
			i = 0;

			display_clear();

			LCD_print("Welcome!", FIRST);
			LCD_print("Enter key here", SECOND);

			continue;
		}

		if (keyInput == 15) {
			attempt[i - 1] = 0;
			i--;

			if (strcmp(password, attempt) == 0) {
				while (1) {
					keyInput = get_pressed_key();
					if (keyInput == INVALID_VALUE)
						continue;
				}
			}
		}
	}
}
