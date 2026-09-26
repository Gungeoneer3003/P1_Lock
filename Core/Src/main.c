#include "init.h"
#include "keypad.h"
#include "led.h"
#include "lcd.h"
#include "stdbool.h"
#include "stm32l4xx_hal.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int main() {
    gpio_init();
	display_init(true, false, true, true, true, false);

    char* password = "12345678";
    
    LCD_print("Welcome!", FIRST);
	LCD_print("Enter key here", SECOND);
    LED_turn_on();

    HAL_Delay(3000);

    char attempt[16];
    
    char* keyBuffer;
    int keyInput = 0;
    int currentIndex = 0;

    while (1) {
		keyInput = get_pressed_key();
        if (keyInput == INVALID_VALUE)
			continue;
        
        if (keyInput == 10) {
            memset(attempt, 0, sizeof(attempt));
            currentIndex = 0;

            display_clear();

            LCD_print("Welcome!", FIRST);
	        LCD_print("Enter key here", SECOND);
            
            continue;
        }

        
        if (keyInput == 15) {
            attempt[c-1] = 0;
            currentIndex--;
            
        
		

        if (strcmp(password, attempt) == 0) {
            while(1) {         
		        keyInput = get_pressed_key();
                if (keyInput == INVALID_VALUE)
			        continue;
	            
                if(keyInput == 15) {
                    LCD_PRINT("
                    while(1) {
                        

		HAL_Delay(500);
	}
}
