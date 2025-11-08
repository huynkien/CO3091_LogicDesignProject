///*
// * keyboard.c
// *
// *  Created on: Nov 4, 2025
// *      Author: Dell
// */
//
//#include "keyboard.h"
//
//// PROCESS INPUT FROM USER
//
//void keyboard_number() {
//	// edge detection to avoid missing presses between scans
//	static uint8_t prev_pressed[16] = {0};
//	uint8_t pressed[16];
//	for (int i = 0; i < 16; ++i) {
//		pressed[i] = (button_count[i] > 0) ? 1 : 0;
//	}
//
//	// map only on rising edge (pressed now, not pressed before)
//	if (pressed[0] && !prev_pressed[0]) keyboard_butbuf = NUMBER_1;
//	if (pressed[1] && !prev_pressed[1]) keyboard_butbuf = NUMBER_2;
//	if (pressed[2] && !prev_pressed[2]) keyboard_butbuf = NUMBER_3;
//	if (pressed[3] && !prev_pressed[3]) keyboard_butbuf = DELETE;
//	if (pressed[4] && !prev_pressed[4]) keyboard_butbuf = NUMBER_4;
//	if (pressed[5] && !prev_pressed[5]) keyboard_butbuf = NUMBER_5;
//	if (pressed[6] && !prev_pressed[6]) keyboard_butbuf = NUMBER_6;
//	if (pressed[7] && !prev_pressed[7]) keyboard_butbuf = DELETE_ALL;
//	if (pressed[8] && !prev_pressed[8]) keyboard_butbuf = NUMBER_7;
//	if (pressed[9] && !prev_pressed[9]) keyboard_butbuf = NUMBER_8;
//	if (pressed[10] && !prev_pressed[10]) keyboard_butbuf = NUMBER_9;
//	if (pressed[11] && !prev_pressed[11]) keyboard_butbuf = UNLOCK;
//	if (pressed[12] && !prev_pressed[12]) keyboard_butbuf = CHAR_A_F;
//	if (pressed[13] && !prev_pressed[13]) keyboard_butbuf = NUMBER_0;
//	if (pressed[14] && !prev_pressed[14]) keyboard_butbuf = CP;
//	if (pressed[15] && !prev_pressed[15]) keyboard_butbuf = OPEN_DOOR;
//
//	for (int i = 0; i < 16; ++i) prev_pressed[i] = pressed[i];
//}
//
//void keyboard_character() {
//	static uint8_t prev_pressed[16] = {0};
//	uint8_t pressed[16];
//	for (int i = 0; i < 16; ++i) {
//		pressed[i] = (button_count[i] > 0) ? 1 : 0;
//	}
//
//	if (pressed[0] && !prev_pressed[0]) keyboard_butbuf = CHARACTER_A;
//	if (pressed[1] && !prev_pressed[1]) keyboard_butbuf = CHARACTER_B;
//	if (pressed[2] && !prev_pressed[2]) keyboard_butbuf = CHARACTER_C;
//	if (pressed[3] && !prev_pressed[3]) keyboard_butbuf = DELETE;
//	if (pressed[4] && !prev_pressed[4]) keyboard_butbuf = CHARACTER_D;
//	if (pressed[5] && !prev_pressed[5]) keyboard_butbuf = CHARACTER_E;
//	if (pressed[6] && !prev_pressed[6]) keyboard_butbuf = CHARACTER_F;
//	if (pressed[7] && !prev_pressed[7]) keyboard_butbuf = DELETE_ALL;
//	// keep indices 8..10 unused here if not mapped
//	if (pressed[11] && !prev_pressed[11]) keyboard_butbuf = UNLOCK;
//	if (pressed[12] && !prev_pressed[12]) keyboard_butbuf = NUM_0_9; // toggle back to number
//	if (pressed[14] && !prev_pressed[14]) keyboard_butbuf = CP;
//	if (pressed[15] && !prev_pressed[15]) keyboard_butbuf = OPEN_DOOR;
//
//	for (int i = 0; i < 16; ++i) prev_pressed[i] = pressed[i];
//}
////
////void keyboard_cp_number() {
////
////}
////
////void keyboard_cp_character() {
////
////}
