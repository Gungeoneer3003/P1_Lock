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


void welcome_message() {
    LCD_print("Welcome!", FIRST);
    LCD_print("Enter key here", SECOND);
}

void init()
{
	gpio_init();
	LCD_init(true, false, true, true, true, false);
}

enum State { LOCKED, UNLOCKED, CHANGING };

void status_message(enum State state, char *input)
{
	switch (state) {
	    case (LOCKED):
		    LCD_clear();
            LCD_print("LOCKED", FIRST);
		    LCD_print(input, SECOND);
	        LED_turn_on();
            break;

        case (UNLOCKED):
		    LCD_clear();
            LCD_print("UNLOCKED", FIRST);
            LED_turn_off();
            break;

        case (CHANGING):
            LCD_clear();
            LCD_print("ENTER PASSWORD", FIRST);
            LCD_print(input, SECOND);
            break;
	}
}

void buffer_append(char *buffer, char ch)
{
	int len = strlen(buffer);
	buffer[len] = ch;
}

void buffer_backspace(char *buffer)
{
	int len = strlen(buffer);
	if (len == 0)
		return;

	buffer[len - 1] = 0;
}

void buffer_reset(char *buffer)
{
	int len = strlen(buffer);
	memset(buffer, 0, len);
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

	enum State state = LOCKED;

	while (1) {
		status_message(state, input);

		keyInput = get_pressed_key();
		while (keyInput == INVALID_VALUE) {
			keyInput = get_pressed_key();
			HAL_Delay(300);
			continue;
		}

		switch (state) {
		case LOCKED:
			if (keyInput == INPUT_STAR) {
				buffer_reset(input);
				continue;
			}

			if (keyInput == INPUT_POUND) {
				buffer_backspace(input);
				continue;
			}

			buffer_append(input, '0' + keyInput);

			if (strcmp(input, password) == 0) {
				state = UNLOCKED;
			}

			break;

		case UNLOCKED:
			// buffer is empty while unlocked
			memset(input, 0, sizeof(input));
			keyPos = 0;

			if (keyInput == INPUT_STAR) {
				state = LOCKED;
				continue;
			}

			if (keyInput == INPUT_POUND) {
				state = CHANGING;
				continue;
			}

			break;

		case CHANGING:
			// change password
			int len = strlen(input);
			if (keyInput == INPUT_POUND) {
				if (len == 0) {
					state = LOCKED;
				}

				buffer_backspace(input);
				continue;
			}

			if (keyInput == INPUT_STAR) {
				if (len < 4)
					continue;

				strcpy(password, input);
				buffer_reset(input);
				state = LOCKED;
				continue;
			}

			buffer_append(input, '0' + keyInput);
		}
	}
}
