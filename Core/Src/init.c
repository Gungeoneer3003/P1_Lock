//init_c
#include "init.h"
#include "stdbool.h"
#include "stm32l476xx.h"
#include "stm32l4xx_hal.h"
#include <stdint.h>

// Function prototypes
void lcd_gpio_init();
void keypad_gpio_init();
void led_gpio_init();
void bus_init();

// Init every pin for this project
void gpio_init() 
{ 
	HAL_Init();
    bus_init();
    lcd_gpio_init();
    keypad_gpio_init();
    led_gpio_init();
}

// Start the heart
void bus_init()
{
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOCEN);
}

// LCD panel will use pines GPIOC 0-9
void lcd_gpio_init()
{
    //LCD Panel
	//Set PC0-9 as input mode
	GPIOC->MODER &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1 | GPIO_MODER_MODE2 | GPIO_MODER_MODE3);
	GPIOC->MODER &= ~(GPIO_MODER_MODE4 | GPIO_MODER_MODE5 | GPIO_MODER_MODE6);
	GPIOC->MODER &= ~(GPIO_MODER_MODE7 | GPIO_MODER_MODE8 | GPIO_MODER_MODE9);

	GPIOC->MODER |= (GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0 | GPIO_MODER_MODE2_0 | GPIO_MODER_MODE3_0);
	GPIOC->MODER |= (GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0);
	GPIOC->MODER |= (GPIO_MODER_MODE7_0 | GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0);

	return;
}

// Keypad will use pins GPIOA 5-8 and GPIOC 10-12
void keypad_gpio_init()
{

	// set rows as input
	// PC0-3
	GPIOA->MODER &= ~(GPIO_MODER_MODE5 | GPIO_MODER_MODE6 | GPIO_MODER_MODE7 | GPIO_MODER_MODE8);

	// set rows as pull-down
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD5 | GPIO_PUPDR_PUPD6 | GPIO_PUPDR_PUPD7 | GPIO_PUPDR_PUPD8);
	GPIOA->PUPDR |= (GPIO_PUPDR_PUPD5_1 | GPIO_PUPDR_PUPD6_1 | GPIO_PUPDR_PUPD7_1 | GPIO_PUPDR_PUPD8_1);

	// set cols as output
	// PC10-12
	GPIOC->MODER &= ~(GPIO_MODER_MODE10 | GPIO_MODER_MODE11 | GPIO_MODER_MODE12);
	GPIOC->MODER |= (GPIO_MODER_MODE10_0 | GPIO_MODER_MODE11_0 | GPIO_MODER_MODE12_0);
}

//LED will use pin GPIOB pin 0 
void led_gpio_init()
{ 
	// set LED as output
	GPIOB->MODER &= ~(GPIO_MODER_MODE0);
	GPIOB->MODER |= (GPIO_MODER_MODE0_0);
}


