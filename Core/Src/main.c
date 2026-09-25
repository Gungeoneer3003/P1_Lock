#include "lcd.h"
#include "init.h"
#include "keypad.h"
#include "led.h"
#include "stdbool.h"
#include "stm32l4xx_hal.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int main() {
    gpio_init();
	display_init(true, false, true, true, true, false);

    
    LCD_print("Hello there", FIRST);
	LCD_print("Key is 0", SECOND);
    LED_turn_on();

    HAL_Delay(3000);

    char str[12];
    int key = 0;

    while (1) {
		key = get_pressed_key();
        if (key == INVALID_VALUE)
			continue;
		
		if (key == 50) 
        {
            display_clear();
            HAL_Delay(2000);

            snprintf(str, sizeof(str), "%d", key);

            LCD_print("Kenobi!", FIRST);
	        LCD_print(str, SECOND);
        }     

		HAL_Delay(1000);
	}
}
