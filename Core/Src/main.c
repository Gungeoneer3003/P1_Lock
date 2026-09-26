#include "init.h"
#include "keypad.h"
#include "led.h"
#include "lcd.h"
#include "stdbool.h"
#include "stm32l476xx.h"
#include "stm32l4xx_hal.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define INPUT_STAR 10
#define INPUT_POUND 15

#define LINE_WIDTH 16

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

enum State { LOCKED, UNLOCKED, CHANGING };

void status_message(enum State state, char *input)
{
	switch (state) {
	case (LOCKED):
		LCD_print("LOCKED", FIRST);
		LCD_print(input, SECOND);
	case (UNLOCKED):
		LCD_print("UNLOCKED", FIRST);
	}
}

int main()
{
	init();
	welcome_message();
	LED_turn_on();

	HAL_Delay(3000);

	// enough space for longest string and a null terminator
	char password[LINE_WIDTH + 1] = { 0 };
	char input[LINE_WIDTH + 1] = { 0 };

	strcpy(password, "1234");

	int keyInput = 0;
	int keyPos = 0;

	enum State state = LOCKED;

	while (1) {
		status_message(state, input);
		keyInput = get_pressed_key();
		// int input_length = strlen(keyInput);

		if (keyInput == INVALID_VALUE) {
			continue;
		}

		switch (state) {
		case LOCKED:
			if (keyInput == INPUT_STAR) {
				memset(input, 0, sizeof(input));
				keyPos = 0;
				continue;
			}

			if (keyInput == INPUT_POUND) {
				if (keyInput == 0)
					continue;

				keyPos--;
				input[keyPos] = 0;
				continue;
			}

			input[keyPos] = '0' + keyInput;
			keyPos++;

			if (strcmp(input, password) == 0) {
				memset(input, 0, sizeof(input));
				keyPos = 0;
				state = UNLOCKED;
			}

			break;

		case UNLOCKED:
			if (keyInput == INPUT_STAR) {
				state = LOCKED;
				continue;
			}

			if (keyInput == INPUT_POUND) {
				state = CHANGING;
				continue;
			}

		case CHANGING:
			// change password
			if (keyInput == INPUT_STAR) {
				memset(input, 0, sizeof(input));
				keyPos = 0;
				continue;
			}

			if (keyInput == INPUT_POUND) {
				if (keyInput == 0)
					continue;

				keyPos--;
				input[keyPos] = 0;
				continue;
			}

			input[keyPos] = '0' + keyInput;
			keyPos++;
			
		}
	}
}
