/*
 * electronic_lock.c
 *
 *  Created on: Nov 4, 2025
 *      Author: Dell
 */

#include "electronic_lock.h"
#include "led_7seg.h"
#include "buzzer.h"

uint8_t electronic_lock_state = INIT;
uint8_t keyboard_state = KEYBOARD_NUMBER;

uint8_t correct_password[4] = {1, 2, 3, 4};

uint8_t entered_index = 0;
uint8_t entered_password[4] = {0};

uint8_t idle_first_input = 1;

//update password
uint8_t update_stage = 0;
uint8_t new_password[4] = {0};

void fsm_electronic_lock_run() {
	switch(electronic_lock_state) {
		case INIT:
			init();
			if (1) {
				init_idle();
				electronic_lock_state = IDLE;
			}
			break;
		case IDLE:
			idle();

			break;
		case RECEIVE_PASSWORD_NUMBER:
			receive_password_number();

			break;
		case RECEIVE_PASSWORD_CHARACTER:
			receive_password_character();

			break;
		case PROCESS_AND_CONTROL:
			process_and_control();
			break;
		case UNLOCK_DOOR:
			unlock_door();
			break;
		case DOOR_OPEN:
			door_open();
			break;
		case DOOR_CLOSE:
			door_close();
			break;
		case ALERT:
			alert();
			break;
		case LOCK_DOOR:
			lock_door();
			break;
		case PASSWORD_INCORRECT:
			password_incorrect();
			break;
			//lockout
		case LOCK_OUT:
			lockout();
			break;
//		case UPDATE_PASSWORD_NUMBER:
//			update_password_number();
//			break;
//		case UPDATE_PASSWORD_CHARACTER:
//			update_password_character();
//			break;
		case CHANGE_PASSWORD_NUMBER:
			change_password_number();
			break;
		case CHANGE_PASSWORD_CHARACTER:
			change_password_character();
			break;
		default:
			break;
	}
}

static void reset_inputs(void) {
	entered_index = 0;
	for (int i = 0; i < 4; ++i) {
		entered_password[i] = 0;
		// Blank the display instead of showing '0' for cleared positions
		led_7seg_clear_pos(i);
	}


}

static uint8_t prev_pressed[16] = {0};

static void read_edges(uint8_t edges[16]) {
	for (int i = 0; i < 16; ++i) {
		uint8_t now = (button_count[i] > 0) ? 1 : 0;
		edges[i] = (now && !prev_pressed[i]) ? 1 : 0;
		prev_pressed[i] = now;
	}
}

uint8_t check_password() {
	for(uint8_t i = 0; i < 4; i++) {
		if (correct_password[i] != entered_password[i]) {
			return 0;
		}
	}
	return 1;
}

void init_idle() {
	lcd_fill(0, 0, 240, 20, BLACK);
	lcd_fill(0, 150, 240, 170, BLACK);
	lcd_fill(0, 20, 240, 172, BLACK);
	lcd_show_string_center(0, 0, "IDLE", LIGHTBLUE, BLACK, 16, 0);
	lcd_show_picture(84, 30, 72, 120, gImage_door_close);
	lcd_show_picture(0, 172, 240, 148, gImage_ini_key_num);
	lcd_show_picture(195, 75, 30, 30, gImage_locked);

	keyboard_state = KEYBOARD_NUMBER;
	idle_first_input = 1;
}
void init_receive_password_number() {
	lcd_fill(0, 0, 240, 20, BLACK);
	lcd_show_string_center(0, 0, "RECEIVE PASSWORD NUMBER", LIGHTBLUE, BLACK, 16, 0);
	lcd_show_picture(84, 30, 72, 120, gImage_door_close);
	lcd_show_picture(0, 172, 240, 148, gImage_ini_key_num);
}
void init_receive_password_character() {
	lcd_fill(0, 0, 240, 20, BLACK);
	lcd_show_string_center(0, 0, "RECEIVE PASSWORD CHARACTER", LIGHTBLUE, BLACK, 16, 0);
	lcd_show_picture(84, 30, 72, 120, gImage_door_close);
	lcd_show_picture(0, 172, 240, 148, gImage_ini_key_char);
}
void init_process_and_control() {
	lcd_fill(0, 0, 240, 20, BLACK);
	lcd_show_string_center(0, 0, "PROCESS AND CONTROL", LIGHTBLUE, BLACK, 16, 0);
	lcd_show_picture(195, 75, 30, 30, gImage_locked);

	electronic_lock_state = PROCESS_AND_CONTROL;
	setTimer(SYSTEM_TIMER, 1000);
}

void init_unlock_door() {
	lcd_fill(0, 0, 240, 20, BLACK);
	lcd_fill(84, 30, 177, 153, BLACK);
	lcd_show_string_center(0, 0, "UNLOCK DOOR", LIGHTBLUE, BLACK, 16, 0);
	lcd_show_picture(195, 75, 30, 30, gImage_unlocked);
	lcd_show_picture(84, 30, 72, 120, gImage_door_close);
}

void init_door_open() {
	lcd_fill(0, 0, 240, 20, BLACK);
	lcd_fill(84, 30, 177, 153, BLACK);
	lcd_fill(0, 150, 240, 170, BLACK);
	lcd_show_string_center(0, 0, "DOOR OPEN", LIGHTBLUE, BLACK, 16, 0);
	lcd_show_picture(195, 75, 30, 30, gImage_unlocked);
	lcd_show_picture(84, 30, 72, 120, gImage_door_open);
}

void init_door_close() {
	lcd_fill(0, 0, 240, 20, BLACK);
	lcd_fill(84, 30, 177, 153, BLACK);
	lcd_fill(30, 75, 60, 105, BLACK);
	lcd_show_string_center(0, 0, "DOOR CLOSE", LIGHTBLUE, BLACK, 16, 0);
	lcd_show_picture(195, 75, 30, 30, gImage_unlocked);
	lcd_show_picture(84, 30, 72, 120, gImage_door_close);
}

void init_alert() {
	lcd_fill(0, 0, 240, 20, BLACK);
	lcd_fill(84, 30, 177, 153, BLACK);
	lcd_show_string_center(0, 0, "ALERT! DOOR NOT CLOSED", RED, BLACK, 16, 0);
	lcd_show_picture(30, 75, 30, 30, gImage_alert);
	lcd_show_picture(195, 75, 30, 30, gImage_unlocked);
	lcd_show_picture(84, 30, 72, 120, gImage_door_open);
	setTimer(4, 500);
	buzzer_init();
}

void init_lock_door() {
	lcd_fill(0, 0, 240, 20, BLACK);
	lcd_fill(84, 30, 177, 153, BLACK);
	lcd_show_string_center(0, 0, "LOCKING DOOR", LIGHTBLUE, BLACK, 16, 0);
	lcd_show_picture(195, 75, 30, 30, gImage_locked);
	lcd_show_picture(84, 30, 72, 120, gImage_door_close);
}

void init_update_password_number();
void init_update_password_character();

void init_update_password(){
//	if(electronic_lock_state == LOCK_OUT) return;
//	update_stage = 0;
//	reset_inputs();
//	lcd_fill(0, 0, 240, 20, WHITE);
//	lcd_fill(0, 150, 240, 170, WHITE);
//	lcd_show_string_center(0, 0, "CHANGE PASSWORD", BLACK, WHITE, 16, 0);
//	lcd_show_string_center(0, 150, "ENTER OLD PASSWORD", BLACK, WHITE, 16, 0);
//	keyboard_state = KEYBOARD_NUMBER;
//	electronic_lock_state = UPDATE_PASSWORD_NUMBER;
//	setTimer(SYSTEM_TIMER, 100);
}

void idle_receive_password_number() {

}

void idle_receive_password_character() {

}

void init() {
	// DO NOTHING
}

void idle() {
//	lcd_fill(0, 150, 240, 20, WHITE);

	if (keyboard_state == KEYBOARD_NUMBER) {
		lcd_show_picture(0, 172, 240, 148, gImage_ini_key_num);
	} else {
		lcd_show_picture(0, 172, 240, 148, gImage_ini_key_char);
	}

	if (!isTimerExpired(SYSTEM_TIMER)) return;

	uint8_t e[16];
	read_edges(e);

	if(e[14]){
		super_init_change_password_number();
		electronic_lock_state = CHANGE_PASSWORD_NUMBER;
		setTimer(SYSTEM_TIMER, 100);
		return;
	}

	if (e[12]) {
		keyboard_state = (keyboard_state == KEYBOARD_NUMBER) ? KEYBOARD_CHARACTER : KEYBOARD_NUMBER;
		setTimer(SYSTEM_TIMER, 100);
		return;
	}

	if (keyboard_state == KEYBOARD_NUMBER) {
		int digit = -1;
		if (e[13] == 1) digit = 0;
		else if (e[0] == 1) digit = 1;
		else if (e[1] == 1) digit = 2;
		else if (e[2] == 1) digit = 3;
		else if (e[4] == 1) digit = 4;
		else if (e[5] == 1) digit = 5;
		else if (e[6] == 1) digit = 6;
		else if (e[8] == 1) digit = 7;
		else if (e[9] == 1) digit = 8;
		else if (e[10] == 1) digit = 9;

		if (digit >= 0) {
			init_receive_password_number();
			entered_index = 0;

			for (int i = 0; i < 4; ++i) {
				entered_password[i] = 0;
				led_7seg_clear_pos(i);
			}

			entered_password[0] = digit;
			led_7seg_set_digit(digit, 0, 0);
			entered_index = 1;
			electronic_lock_state = RECEIVE_PASSWORD_NUMBER;
			setTimer(SYSTEM_TIMER, 100);
			setTimer(TIMER_15S, TIME_15S);
			return;
		}
	} else { // KEYBOARD_CHARACTER
		int character = -1; // A-F -> 10..15
		if (e[0] == 1) character = 10; // A
		else if (e[1] == 1) character = 11; // B
		else if (e[2] == 1) character = 12; // C
		else if (e[4] == 1) character = 13; // D
		else if (e[5] == 1) character = 14; // E
		else if (e[6] == 1) character = 15; // F

		if (character >= 0) {
			init_receive_password_character();
			entered_index = 0;

			for (int i = 0; i < 4; ++i) {
				entered_password[i] = 0;
				led_7seg_clear_pos(i);
			}

			entered_password[0] = character;
			led_7seg_set_digit(character, 0, 0);
			entered_index = 1;
			electronic_lock_state = RECEIVE_PASSWORD_CHARACTER;
			setTimer(SYSTEM_TIMER, 10);
			setTimer(TIMER_15S, TIME_15S);
			return;
		}
	}

	setTimer(SYSTEM_TIMER, 100);
}

void receive_password_number() {
	if (isTimerExpired(TIMER_15S)) {
		reset_inputs();
		init_idle();
		electronic_lock_state = IDLE;
		return;
	}

	if (!isTimerExpired(SYSTEM_TIMER)) return;

	uint8_t e[16];
	read_edges(e);

	// toggle to character
	if (e[12]) {
		init_receive_password_character();
		keyboard_state = KEYBOARD_CHARACTER;
		electronic_lock_state = RECEIVE_PASSWORD_CHARACTER;
		setTimer(SYSTEM_TIMER, 100);
		setTimer(TIMER_15S, TIME_15S);
		return;
	}

	uint8_t processed = 0;
	// digits
	int digit = -1;
	if (e[13] == 1) digit = 0;
	else if (e[0] == 1) digit = 1;
	else if (e[1] == 1) digit = 2;
	else if (e[2] == 1) digit = 3;
	else if (e[4] == 1) digit = 4;
	else if (e[5] == 1) digit = 5;
	else if (e[6] == 1) digit = 6;
	else if (e[8] == 1) digit = 7;
	else if (e[9] == 1) digit = 8;
	else if (e[10] == 1) digit = 9;
	if (digit >= 0) {
		if (entered_index < 4) {
			entered_password[entered_index] = digit;
			led_7seg_set_digit(digit, entered_index, 0);
			entered_index++;
		}
		processed = 1;
	}

	if (e[3] && !processed) { // DELETE
		if (entered_index > 0) {
			entered_index--;
			entered_password[entered_index] = 0;
			led_7seg_clear_pos(entered_index);
		}
		processed = 1;
	}
	if (!processed && e[7]) { // DELETE_ALL
		reset_inputs();
		processed = 1;
	}

	if (processed) {
		setTimer(TIMER_15S, TIME_15S);
	}

	// enough 4 character
	if (entered_index >= 4) {
		init_process_and_control();
		electronic_lock_state = PROCESS_AND_CONTROL;
		return;
	}

	setTimer(SYSTEM_TIMER, 100);
}

void receive_password_character() {
	if (isTimerExpired(TIMER_15S)) {
		reset_inputs();
		init_idle();
		electronic_lock_state = IDLE;
		return;
	}

	if (!isTimerExpired(SYSTEM_TIMER)) return;

	uint8_t e[16];
	read_edges(e);

	// toggle back to number
	if (e[12]) {
		init_receive_password_number();
		keyboard_state = KEYBOARD_NUMBER;
		electronic_lock_state = RECEIVE_PASSWORD_NUMBER;
		setTimer(SYSTEM_TIMER, 100);
		setTimer(TIMER_15S, TIME_15S);
		return;
	}

	uint8_t processed = 0;
	int character = -1; // A-F
	if (e[0] == 1) character = 10; // A
	else if (e[1] == 1) character = 11; // B
	else if (e[2] == 1) character = 12; // C
	else if (e[4] == 1) character = 13; // D
	else if (e[5] == 1) character = 14; // E
	else if (e[6] == 1) character = 15; // F
	if (character >= 0) {
		if (entered_index < 4) {
			entered_password[entered_index] = character;
			led_7seg_set_digit(character, entered_index, 0);
			entered_index++;
		}
		processed = 1;
	}

	if (e[3] && processed) { // DELETE
		if (entered_index > 0) {
			entered_index--;
			entered_password[entered_index] = 0;
			led_7seg_clear_pos(entered_index);
		}
		processed = 1;
	}
	if (!processed && e[7]) { // DELETE_ALL
		reset_inputs();
		processed = 1;
	}

	if (processed) {
		setTimer(TIMER_15S, TIME_15S);
	}

	if (entered_index >= 4) {
		init_process_and_control();
		electronic_lock_state = PROCESS_AND_CONTROL;
		return;
	}

	setTimer(SYSTEM_TIMER, 100);
}

//count incorrect time
static uint8_t wrong_counter = 0;
uint8_t lock_level = 0;
uint32_t lockout_remaining_ms = 0;

// Variables for change password flow
static uint8_t change_stage = 0; // 0: old password, 1: new password, 2: confirm password
static uint8_t old_password_input[4] = {0};
static uint8_t new_password_input[4] = {0};
static uint8_t confirm_password_input[4] = {0};
static uint8_t change_password_index = 0;
static uint8_t change_password_wrong_count = 0; // Count wrong old password attempts

void show_7SEGMENT(uint16_t seconds){
	int d0 = seconds%10;
	seconds /= 10;
	int d1 = seconds%10;
	seconds /= 10;
	int d2 = seconds%10;
	seconds /= 10;
	int d3 = seconds;
	if(d3 > 0) led_7seg_set_digit(d3, 0, 0);
	else led_7seg_clear_pos(0);
	if(d3 > 0 || d2 > 0) led_7seg_set_digit(d2, 1, 0);
	else led_7seg_clear_pos(1);
	if(d3>0 || d2>0 || d1>0) led_7seg_set_digit(d1, 2, 0);
	else led_7seg_clear_pos(2);
	led_7seg_set_digit(d0, 3, 0);
}

void process_and_control() {
	if (!isTimerExpired(SYSTEM_TIMER)) return;
	
	if (check_password()) {
		// Password correct
		lcd_fill(0, 150, 240, 20, BLACK);
		lcd_show_string_center(0, 150, "PASSWORD CORRECT", GREEN, BLACK, 16, 0);
		wrong_counter = 0;
		lock_level = 0;
		setTimer(SYSTEM_TIMER, 1500);
		electronic_lock_state = UNLOCK_DOOR;
		init_unlock_door();
		setTimer(TIMER_10S, TIME_10S);
		setTimer(TIMER_30S, TIME_30S);
	} else {
		lcd_fill(0, 150, 240, 20, BLACK);
		lcd_show_string_center(0, 150, "PASSWORD INCORRECT", RED, BLACK, 16, 0);
		wrong_counter++;
		setTimer(SYSTEM_TIMER, 1500); // Wait 1.5s before returning to IDLE
		electronic_lock_state = PASSWORD_INCORRECT;
	}
}

void password_incorrect() {
	// Wait 1.5s then return to IDLE or reset change password form
	if (!isTimerExpired(SYSTEM_TIMER)) return;
	
	// Check if coming from change password with too many wrong attempts
	if (change_password_wrong_count >= 3) {
		// Go to IDLE first, then trigger lockout
		reset_inputs();
		init_idle();
		electronic_lock_state = IDLE;
		
		// Prepare lockout parameters
		uint32_t duration_ms = 60000;
		duration_ms <<= lock_level;
		init_lockout(duration_ms);
		electronic_lock_state = LOCK_OUT;
		lock_level++;
		change_password_wrong_count = 0; // Reset for next time
		return;
	}
	
	// Check if this was from change password (wrong_counter not incremented)
	if (change_password_wrong_count > 0 && change_password_wrong_count < 3) {
		// Reset change password form and stay in change password state
		reset_change_password_form();
		electronic_lock_state = CHANGE_PASSWORD_NUMBER;
		setTimer(SYSTEM_TIMER, 100);
		return;
	}
	
	// Normal password incorrect flow
	if(wrong_counter >= 3){
		uint32_t duration_ms = 60000;
		duration_ms <<= lock_level;
		init_lockout(duration_ms);
		electronic_lock_state = LOCK_OUT;
		lock_level++;
		return;
	}
	
	reset_inputs();
	init_idle();
	electronic_lock_state = IDLE;
	setTimer(SYSTEM_TIMER, 100);
}

void init_lockout(uint32_t ms){
	lockout_remaining_ms = ms;
	uint16_t seconds = (lockout_remaining_ms + 999) / 1000;
	show_7SEGMENT(seconds);
	lcd_fill(0, 0, 240, 20, BLACK);
    lcd_fill(0, 150, 240, 170, BLACK);
    lcd_show_string_center(0, 0,  "LOCKED", RED, BLACK, 16, 0);
    lcd_show_string_center(0, 150, "TRY AGAIN LATER", YELLOW, BLACK, 16, 0);
    setTimer(SYSTEM_TIMER, 1000);
}

void lockout(){
	if(!isTimerExpired(SYSTEM_TIMER)) return;
	if(lockout_remaining_ms >= 1000){
		lockout_remaining_ms -= 1000;
		uint16_t seconds = (lockout_remaining_ms + 999) / 1000;
		show_7SEGMENT(seconds);
		setTimer(SYSTEM_TIMER, 1000);
	}
	else{
		lockout_remaining_ms = 0;
		reset_inputs();
		init_idle();
		electronic_lock_state = IDLE;
		setTimer(SYSTEM_TIMER, 100);
	}
}

void unlock_door() {
	if (!isTimerExpired(SYSTEM_TIMER)) return;
	
	uint8_t e[16];
	read_edges(e);
	
	// Check if user pressed OPEN_DOOR button
	if (e[15]) {
		// User pressed open door button
		init_door_open();
		electronic_lock_state = DOOR_OPEN;
		setTimer(TIMER_30S, TIME_30S); // Reset 30s timer for door closing
		setTimer(SYSTEM_TIMER, 100);
		return;
	}
	
	// Check if 10s expired without pressing open door button
	if (isTimerExpired(TIMER_10S)) {
		// Lock door again and return to IDLE
		init_lock_door();
		electronic_lock_state = LOCK_DOOR;
		setTimer(SYSTEM_TIMER, 2000); // Stay in LOCK_DOOR for 2s
		return;
	}
	
	setTimer(SYSTEM_TIMER, 100);
}

void door_open() {
	if (!isTimerExpired(SYSTEM_TIMER)) return;
	
	uint8_t e[16];
	read_edges(e);
	
	if (e[15]) {
		// User closed door
		init_door_close();
		electronic_lock_state = DOOR_CLOSE;
		setTimer(SYSTEM_TIMER, 100); // Poll for button[11] press
		return;
	}
	
	// Check if 30s expired without closing door
	if (isTimerExpired(TIMER_30S)) {
		// Alert: door not closed in time
		init_alert();
		electronic_lock_state = ALERT;
		setTimer(SYSTEM_TIMER, 100);
		return;
	}
	
	setTimer(SYSTEM_TIMER, 100);
}

void door_close() {
	if (!isTimerExpired(SYSTEM_TIMER)) return;
	
	uint8_t e[16];
	read_edges(e);
	
	// Wait for user to press button[11] to lock the door
	if (e[11]) {
		init_lock_door();
		electronic_lock_state = LOCK_DOOR;
		setTimer(SYSTEM_TIMER, 2000);
		return;
	}
	
	setTimer(SYSTEM_TIMER, 100);
}

void alert() {
	if (!isTimerExpired(SYSTEM_TIMER)) return;
	
	uint8_t e[16];
	read_edges(e);
	if(isTimerExpired(4)){
		HAL_GPIO_TogglePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin);
		setTimer(4, 500);
	}
	buzzer_SetVolume(50);
	// Wait for user to close door
	if (e[15]) {
		// User closed door
		buzzer_Stop();
		init_door_close();
		electronic_lock_state = DOOR_CLOSE;
		setTimer(SYSTEM_TIMER, 100); // Poll for button[11] press
		HAL_GPIO_WritePin(OUTPUT_Y0_GPIO_Port, OUTPUT_Y0_Pin, RESET);
		return;
	}
	
	setTimer(SYSTEM_TIMER, 100);
}

void lock_door() {
	if (!isTimerExpired(SYSTEM_TIMER)) return;
	
	reset_inputs();
	init_idle();
	electronic_lock_state = IDLE;
	setTimer(SYSTEM_TIMER, 100);
}

static int get_digit_from_edges(uint8_t e[16]){
    if (e[0]) return 1;
    if (e[1]) return 2;
    if (e[2]) return 3;
    if (e[4]) return 4;
    if (e[5]) return 5;
    if (e[6]) return 6;
    if (e[8]) return 7;
    if (e[9]) return 8;
    if (e[10]) return 9;
    if (e[13]) return 0;
    return -1;
}

void update_password_number(){

}

void update_password_character() {

}

// Called when first entering change password from IDLE - resets everything including counter
void super_init_change_password_number() {
	lcd_fill(0, 0, 240, 20, BLACK);
	lcd_show_string_center(0, 0, "CHANGE PASSWORD", LIGHTBLUE, BLACK, 16, 0);
	lcd_fill(0, 20, 240, 172, BLACK);
	
	lcd_show_string(20, 50, "OLD PASSWORD:", LIGHTBLUE, BLACK, 16, 0);
	lcd_show_string(20, 80, "NEW PASSWORD:", LIGHTBLUE, BLACK, 16, 0);
	lcd_show_string(20, 110, "CONFIRM:", LIGHTBLUE, BLACK, 16, 0);
	
	lcd_show_picture(0, 172, 240, 148, gImage_ini_key_num);
	
	// Reset variables
	change_stage = 0;
	change_password_index = 0;
	change_password_wrong_count = 0; // Reset wrong count when first entering change password
	for (int i = 0; i < 4; i++) {
		old_password_input[i] = 0;
		new_password_input[i] = 0;
		confirm_password_input[i] = 0;
	}
	
	keyboard_state = KEYBOARD_NUMBER;
}

// Called when resetting form after wrong password - does NOT reset counter
void reset_change_password_form() {
	lcd_fill(0, 0, 240, 20, BLACK);
	lcd_show_string_center(0, 0, "CHANGE PASSWORD", LIGHTBLUE, BLACK, 16, 0);
	lcd_fill(0, 20, 240, 172, BLACK);
	
	lcd_show_string(20, 50, "OLD PASSWORD:", LIGHTBLUE, BLACK, 16, 0);
	lcd_show_string(20, 80, "NEW PASSWORD:", LIGHTBLUE, BLACK, 16, 0);
	lcd_show_string(20, 110, "CONFIRM:", LIGHTBLUE, BLACK, 16, 0);
	
	lcd_show_picture(0, 172, 240, 148, gImage_ini_key_num);
	
	// Reset variables but keep change_password_wrong_count
	change_stage = 0;
	change_password_index = 0;
	for (int i = 0; i < 4; i++) {
		old_password_input[i] = 0;
		new_password_input[i] = 0;
		confirm_password_input[i] = 0;
	}
	
	keyboard_state = KEYBOARD_NUMBER;
}

void init_change_password_number() {
	lcd_fill(0, 0, 240, 20, BLACK);
	lcd_show_string_center(0, 0, "CHANGE PASSWORD", LIGHTBLUE, BLACK, 16, 0);
	lcd_show_picture(0, 172, 240, 148, gImage_ini_key_num);


	keyboard_state = KEYBOARD_NUMBER;
}

void init_change_password_character() {
	lcd_fill(0, 0, 240, 20, BLACK);
	lcd_show_string_center(0, 0, "CHANGE PASSWORD", LIGHTBLUE, BLACK, 16, 0);
	lcd_show_picture(0, 172, 240, 148, gImage_ini_key_char);
	
	keyboard_state = KEYBOARD_CHARACTER;
}

void change_password_number() {
	if (!isTimerExpired(SYSTEM_TIMER)) return;
	
	uint8_t e[16];
	read_edges(e);
	
	// Return to IDLE if button[14] pressed
	if (e[14]) {
		reset_inputs();
		init_idle();
		electronic_lock_state = IDLE;
		setTimer(SYSTEM_TIMER, 100);
		return;
	}
	
	// Toggle to character mode
	if (e[12]) {
		init_change_password_character();
		electronic_lock_state = CHANGE_PASSWORD_CHARACTER;
		setTimer(SYSTEM_TIMER, 100);
		return;
	}
	
	// DELETE one character
	if (e[3]) {
		if (change_password_index > 0) {
			change_password_index--;
			// Clear the last character by drawing white rectangle
			uint16_t y_pos = (change_stage == 0) ? 50 : (change_stage == 1) ? 80 : 110;
			lcd_fill(140 + change_password_index * 20, y_pos, 140 + change_password_index * 20 + 16, y_pos + 16, BLACK);

			// Clear from array
			if (change_stage == 0) {
				old_password_input[change_password_index] = 0;
			} else if (change_stage == 1) {
				new_password_input[change_password_index] = 0;
			} else if (change_stage == 2) {
				confirm_password_input[change_password_index] = 0;
			}
		}
		setTimer(SYSTEM_TIMER, 100);
		return;
	}
	
	// DELETE ALL characters
	if (e[7]) {
		// Clear all characters in current stage
		uint16_t y_pos = (change_stage == 0) ? 50 : (change_stage == 1) ? 80 : 110;
		lcd_fill(140, y_pos, 240, y_pos + 20, BLACK);
		
		// Clear from array and reset index
		change_password_index = 0;
		if (change_stage == 0) {
			for (int i = 0; i < 4; i++) old_password_input[i] = 0;
		} else if (change_stage == 1) {
			for (int i = 0; i < 4; i++) new_password_input[i] = 0;
		} else if (change_stage == 2) {
			for (int i = 0; i < 4; i++) confirm_password_input[i] = 0;
		}
		setTimer(SYSTEM_TIMER, 100);
		return;
	}
	
	// Get digit input
	int digit = get_digit_from_edges(e);
	
	if (digit >= 0 && change_password_index < 4) {
		if (change_stage == 0) {
			old_password_input[change_password_index] = (uint8_t)digit;
			lcd_show_int_num(140 + change_password_index * 20, 50, digit, 1, LIGHTBLUE, BLACK, 16);
		} else if (change_stage == 1) {
			new_password_input[change_password_index] = (uint8_t)digit;
			lcd_show_int_num(140 + change_password_index * 20, 80, digit, 1, LIGHTBLUE, BLACK, 16);
		} else if (change_stage == 2) {
			confirm_password_input[change_password_index] = (uint8_t)digit;
			lcd_show_int_num(140 + change_password_index * 20, 110, digit, 1, LIGHTBLUE, BLACK, 16);
		}
		
		change_password_index++;
		
		if (change_password_index >= 4) {
			change_password_index = 0;
			change_stage++;
			
			if (change_stage >= 3) {
				// Verify old password
				uint8_t old_correct = 1;
				for (int i = 0; i < 4; i++) {
					if (old_password_input[i] != correct_password[i]) {
						old_correct = 0;
						break;
					}
				}
				
				// Verify new password matches confirm
				uint8_t new_matches = 1;
				for (int i = 0; i < 4; i++) {
					if (new_password_input[i] != confirm_password_input[i]) {
						new_matches = 0;
						break;
					}
				}
				
				if (old_correct && new_matches) {
					// Save new password - SUCCESS
					for (int i = 0; i < 4; i++) {
						correct_password[i] = new_password_input[i];
					}
					lcd_fill(0, 140, 240, 20, BLACK);
					lcd_show_string_center(0, 140, "PASSWORD CHANGED!", GREEN, BLACK, 16, 0);
					change_password_wrong_count = 0; // Reset counter on success
					setTimer(SYSTEM_TIMER, 2000);
					electronic_lock_state = LOCK_DOOR;
				} else {
					lcd_fill(0, 140, 240, 20, BLACK);
					if (!old_correct) {
						change_password_wrong_count++; // Increase wrong counter
						
						// Check if exceeded 3 attempts
						if (change_password_wrong_count >= 3) {
							lcd_show_string_center(0, 140, "TOO MANY ATTEMPTS!", RED, BLACK, 16, 0);
							setTimer(SYSTEM_TIMER, 1500);
							electronic_lock_state = PASSWORD_INCORRECT; // Trigger lockout
							return;
						} else {
							lcd_show_string_center(0, 140, "OLD PASSWORD WRONG!", RED, BLACK, 16, 0);
						}
					} else {
						lcd_show_string_center(0, 140, "PASSWORDS DON'T MATCH!", RED, BLACK, 16, 0);
					}
					
					// Reset change password form and try again
					setTimer(SYSTEM_TIMER, 1500);
					electronic_lock_state = PASSWORD_INCORRECT; // Reset change password
				}
				
				return;
			}
		}
	}
	
	setTimer(SYSTEM_TIMER, 100);
}

void change_password_character() {
	if (!isTimerExpired(SYSTEM_TIMER)) return;
	
	uint8_t e[16];
	read_edges(e);
	
	if (e[14]) {
		reset_inputs();
		init_idle();
		electronic_lock_state = IDLE;
		setTimer(SYSTEM_TIMER, 100);
		return;
	}
	
	// Toggle back to number mode
	if (e[12]) {
		init_change_password_number();
		electronic_lock_state = CHANGE_PASSWORD_NUMBER;
		setTimer(SYSTEM_TIMER, 100);
		return;
	}
	
	// DELETE one character
	if (e[3]) {
		if (change_password_index > 0) {
			change_password_index--;
			// Clear the last character
			uint16_t y_pos = (change_stage == 0) ? 50 : (change_stage == 1) ? 80 : 110;
			lcd_fill(140 + change_password_index * 20, y_pos, 140 + change_password_index * 20 + 16, y_pos + 16, BLACK);

			// Clear from array
			if (change_stage == 0) {
				old_password_input[change_password_index] = 0;
			} else if (change_stage == 1) {
				new_password_input[change_password_index] = 0;
			} else if (change_stage == 2) {
				confirm_password_input[change_password_index] = 0;
			}
		}
		setTimer(SYSTEM_TIMER, 100);
		return;
	}
	
	// DELETE ALL characters
	if (e[7]) {
		// Clear all characters in current stage
		uint16_t y_pos = (change_stage == 0) ? 50 : (change_stage == 1) ? 80 : 110;
		lcd_fill(140, y_pos, 240, y_pos + 20, BLACK);

		// Clear from array and reset index
		change_password_index = 0;
		if (change_stage == 0) {
			for (int i = 0; i < 4; i++) old_password_input[i] = 0;
		} else if (change_stage == 1) {
			for (int i = 0; i < 4; i++) new_password_input[i] = 0;
		} else if (change_stage == 2) {
			for (int i = 0; i < 4; i++) confirm_password_input[i] = 0;
		}
		setTimer(SYSTEM_TIMER, 100);
		return;
	}
	
	// Get character input (A-F)
	int character = -1;
	if (e[0]) character = 10; // A
	else if (e[1]) character = 11; // B
	else if (e[2]) character = 12; // C
	else if (e[4]) character = 13; // D
	else if (e[5]) character = 14; // E
	else if (e[6]) character = 15; // F
	
	if (character >= 0 && change_password_index < 4) {
		char display_char = 'A' + (character - 10);
		
		if (change_stage == 0) {
			old_password_input[change_password_index] = (uint8_t)character;
			lcd_show_char(140 + change_password_index * 20, 50, display_char, LIGHTBLUE, BLACK, 16, 0);
		} else if (change_stage == 1) {
			new_password_input[change_password_index] = (uint8_t)character;
			lcd_show_char(140 + change_password_index * 20, 80, display_char, LIGHTBLUE, BLACK, 16, 0);
		} else if (change_stage == 2) {
			confirm_password_input[change_password_index] = (uint8_t)character;
			lcd_show_char(140 + change_password_index * 20, 110, display_char, LIGHTBLUE, BLACK, 16, 0);
		}
		
		change_password_index++;
		
		if (change_password_index >= 4) {
			change_password_index = 0;
			change_stage++;
			
			if (change_stage >= 3) {
				// Verify old password
				uint8_t old_correct = 1;
				for (int i = 0; i < 4; i++) {
					if (old_password_input[i] != correct_password[i]) {
						old_correct = 0;
						break;
					}
				}
				
				// Verify new password matches confirm
				uint8_t new_matches = 1;
				for (int i = 0; i < 4; i++) {
					if (new_password_input[i] != confirm_password_input[i]) {
						new_matches = 0;
						break;
					}
				}
				
				if (old_correct && new_matches) {
					// Save new password - SUCCESS
					for (int i = 0; i < 4; i++) {
						correct_password[i] = new_password_input[i];
					}
					lcd_fill(0, 140, 240, 20, BLACK);
					lcd_show_string_center(0, 140, "PASSWORD CHANGED!", GREEN, BLACK, 16, 0);
					change_password_wrong_count = 0; // Reset counter on success
					setTimer(SYSTEM_TIMER, 2000);
					electronic_lock_state = LOCK_DOOR;
				} else {
					// FAILED - handle differently based on error type
					lcd_fill(0, 140, 240, 20, BLACK);
					if (!old_correct) {
						change_password_wrong_count++; // Increment wrong counter
						
						// Check if exceeded 3 attempts
						if (change_password_wrong_count >= 3) {
							lcd_show_string_center(0, 140, "TOO MANY ATTEMPTS!", RED, BLACK, 16, 0);
							setTimer(SYSTEM_TIMER, 1500);
							electronic_lock_state = PASSWORD_INCORRECT; // Trigger lockout
							return;
						} else {
							lcd_show_string_center(0, 140, "OLD PASSWORD WRONG!", RED, BLACK, 16, 0);
						}
					} else {
						lcd_show_string_center(0, 140, "PASSWORDS DON'T MATCH!", RED, BLACK, 16, 0);
					}
					
					// Reset change password form and try again
					setTimer(SYSTEM_TIMER, 1500);
					electronic_lock_state = PASSWORD_INCORRECT;
				}
				
				return;
			}
		}
	}
	
	setTimer(SYSTEM_TIMER, 100);
}

