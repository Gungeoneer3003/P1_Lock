#include "init.h"
#include "keypad.h"
#include "led.h"
#include "lcd.h"
#include "stdbool.h"
#include "stm32l4xx_hal.h"
#include <stddef.h>
#include <stdint.h>
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
    INPUT,
	UNLOCKED,
    CHANGING
};

int main()
{
	init();
	welcome_message();
	LED_turn_on();

	HAL_Delay(3000);

	char password[LINE_LENGTH] = { 0 };
	char attempt[LINE_LENGTH] = { 0 };

	strcpy(password, "1234");

	int keyInput = 0;
	int keyPos = 0;

	enum State state = LOCKED;

	while (1) {
	    keyInput = get_pressed_key();
		if (keyInput == INVALID_VALUE) {
			continue;
		}

        switch (state) {
    		case LOCKED:
			    if (keyInput == INPUT_STAR || INPUT_POUND) {
				    continue;
			    } else {
                    attempt[keyPos] = '0' + keyInput;
                    keyPos++;
                    state = INPUT;
                }
            
                break;

        case INPUT:
                if (keyInput == INPUT_STAR) {
                    memset(attempt, 0, sizeof(attempt));
                    keyPos = 0;
                    state = LOCKED;

                }
                else if (keyInput == INPUT_POUND) {
                    keyPos--;
                    attempt[keyPos] = 0;
                    
                    if(keyPos == 0) {
                        state = LOCKED;
                    }

                }
                else {
                    attempt[keyPos] = '0' + keyInput;
                    keyPos++;
                    
                    if(strcmp(attempt, password) == 0) {
                        state = UNLOCKED;
                    }
                    
                }

                break;
        
        case UNLOCKED:
                if (keyInput == INPUT_STAR) {                    
                    memset(attempt, 0, sizeof(attempt));
                    keyPos = 0;
                    state = LOCKED;

                } 
                else if (keyInput == INPUT_POUND) {
                    //Enter changing   
                }               

                break;

        case CHANGING:
                //do stuff
    
        }
    }	
}
