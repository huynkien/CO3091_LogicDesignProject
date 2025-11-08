/*
 * led_7seg.c
 */

/* Includes */
#include <led_7seg.h>
#include "spi.h"

/* Variables */
static uint8_t led_7seg[4] = { 0, 1, 2, 3 };
// NOTE: Segment mapping depends on board wiring. Existing digits 0-9 kept.
// Add hex A-F patterns (common cathode assumption) to allow display of A-F.
// Original mapping indexes 0..9. We'll extend array to 16 entries.
// Patterns for A B C D E F chosen for readability; adjust if wiring differs.
// 0x77 A, 0x7C b, 0x39 C, 0x5E d, 0x79 E, 0x71 F (typical). We may choose uppercase style approximations.
static uint8_t led_7seg_map_of_output[16] = {
	0x03, // 0
	0x9f, // 1
	0x25, // 2
	0x0d, // 3
	0x99, // 4
	0x49, // 5
	0x41, // 6
	0x1f, // 7
	0x01, // 8
	0x09, // 9
	0x11, // A
	0xC1, // b (lowercase)
	0x63, // C
	0x85, // d (lowercase)
	0x61, // E
	0x71  // F
};
static uint16_t led_7seg_index = 0;
static uint16_t spi_buffer = 0xffff;

/* Functions */
/**
 * @brief  	Init led 7 segment
 * @param  	None
 * @retval 	None
 */
void led_7seg_init() {
	HAL_GPIO_WritePin(LD_LATCH_GPIO_Port, LD_LATCH_Pin, 1);
}

/**
 * @brief	Scan led 7 segment
 * @param	None
 * @note	Call in 1ms interrupt
 * @retval 	None
 */
void led_7seg_display() {
	spi_buffer &= 0x00ff;
	spi_buffer |= led_7seg[led_7seg_index] << 8;

	switch (led_7seg_index) {
	case 0:
		spi_buffer |= 0x00b0;
		spi_buffer &= 0xffbf;
		break;
	case 1:
		spi_buffer |= 0x00d0;
		spi_buffer &= 0xffdf;
		break;
	case 2:
		spi_buffer |= 0x00e0;
		spi_buffer &= 0xffef;
		break;
	case 3:
		spi_buffer |= 0x0070;
		spi_buffer &= 0xff7f;
		break;
	default:
		break;
	}

	led_7seg_index = (led_7seg_index + 1) % 4;

	HAL_GPIO_WritePin(LD_LATCH_GPIO_Port, LD_LATCH_Pin, 0);
	HAL_SPI_Transmit(&hspi1, (void*) &spi_buffer, 2, 1);
	HAL_GPIO_WritePin(LD_LATCH_GPIO_Port, LD_LATCH_Pin, 1);
}

/**
 * @brief  	Display a digit at a position of led 7-segment
 * @param  	num	Number displayed
 * @param  	pos	The position displayed (index from 0)
 * @param  	show_dot Show dot in the led or not
 * @retval 	None
 */
void led_7seg_set_digit(int num, int position, uint8_t show_dot) {
	if (position < 0 || position > 3) return;
	if (num < 0) num = 0;
	if (num > 15) num = num % 16; // wrap
	uint8_t seg = led_7seg_map_of_output[num];
	if (show_dot) {
		// assume dot is LSB cleared when active like previous code using '- show_dot'
		if (seg > 0) seg -= 1; // replicate prior behavior
	}
	led_7seg[position] = seg;
}

void led_7seg_clear_pos(int position) {
	if (position < 0 || position > 3) return;
	// Set all segments off for this position (blank). Assuming 0xFF turns all segments off in this wiring.
	// If your wiring is active-low for segments, 0xFF will turn them off (no segment lit).
	// Adjust if necessary after visual test.
	led_7seg[position] = 0xFF;
}

/**
 * @brief	Control the colon led
 * @param	status Status applied to the colon (1: turn on, 0: turn off)
 *          This parameter can be one of the following values:
 *				@arg 0: Turn off
 *				@arg 1: Turn on
 * @retval None
 */
void led_7seg_set_colon(uint8_t status) {
	if (status == 1)
		spi_buffer &= ~(1 << 3);
	else
		spi_buffer |= (1 << 3);
}

/**
 * @brief  	Turn on one of three led at top-right corner of the board
 * @param  	index Index of the led
 *          This parameter can be one of the following values:
 *            	@arg 6
 *            	@arg 7
 *            	@arg 8
 * @retval None
 */
void led_7seg_debug_turn_on(uint8_t index) {
	if (index >= 6 && index <= 8) {
		spi_buffer |= 1 << (index - 6);
	}
}

/**
 * @brief  	Turn off one of three led at top-right corner of the board
 * @param  	index Index of the led
 *			This parameter can be one of the following values:
 *            	@arg 6
 *            	@arg 7
 *            	@arg 8
 * @retval None
 */
void led_7seg_debug_turn_off(uint8_t index) {
	if (index >= 6 && index <= 8) {
		spi_buffer &= ~(1 << (index - 6));
	}
}
