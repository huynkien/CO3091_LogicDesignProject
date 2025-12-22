/*
 * button.c
 *
 *  Created on: Nov 4, 2025
 *      Author: Dell
 */

/* Includes */
#include "button.h"

#include "spi.h"
#include "gpio.h"
#include "touch.h"
#include "lcd.h"

/* Variables */
uint16_t button_count[16] = {0};
static uint16_t button_spi_buffer = 0x0000;

/* Functions */
/**
 * @brief  	Init matrix button
 * @param  	None
 * @retval 	None
 */
void button_init() {
	HAL_GPIO_WritePin(BTN_LOAD_GPIO_Port, BTN_LOAD_Pin, 1);
}

/**
 * @brief  	Scan matrix button
 * @param  	None
 * @note  	Call every 50ms
 * @retval 	None
 */
void button_scan() {
	HAL_GPIO_WritePin(BTN_LOAD_GPIO_Port, BTN_LOAD_Pin, 0);
	HAL_GPIO_WritePin(BTN_LOAD_GPIO_Port, BTN_LOAD_Pin, 1);
	__disable_irq();
	HAL_SPI_Receive(&hspi1, (void*) &button_spi_buffer, 2, 10);
	__enable_irq();

	touch_Scan();
	int touch_index = -1;
	if (touch_IsTouched()) {
		uint16_t x = touch_GetX();
		uint16_t y = touch_GetY();

		if (y >= lcddev.height / 2 && x < lcddev.width) {
			int row = (y - lcddev.height / 2) / (lcddev.height / 8);
			int col = x / (lcddev.width / 4);
			if (row >= 0 && row < 4 && col >= 0 && col < 4) {
				touch_index = row * 4 + col;
			}
		}
	}

	int button_index = 0;
	uint16_t mask = 0x8000;
	for (int i = 0; i < 16; i++) {
		if (i >= 0 && i <= 3) {
			button_index = i + 4;
		} else if (i >= 4 && i <= 7) {
			button_index = 7 - i;
		} else if (i >= 8 && i <= 11) {
			button_index = i + 4;
		} else {
			button_index = 23 - i;
		}

		int is_pressed = !(button_spi_buffer & mask);
		if (touch_index == button_index) {
			is_pressed = 1;
		}

		if (is_pressed)
			button_count[button_index]++;
		else
			button_count[button_index] = 0;
		mask = mask >> 1;
	}
}

