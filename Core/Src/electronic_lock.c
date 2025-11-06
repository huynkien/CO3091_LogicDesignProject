/*
 * electronic_lock.c
 *
 *  Created on: Nov 4, 2025
 *      Author: Dell
 */

#include "electronic_lock.h"

uint8_t electronic_lock_state = INIT;
uint8_t keyboard_state = KEYBOARD_NUMBER;

void fsm_electronic_lock_run() {
	switch(electronic_lock_state) {
		case INIT:
			init();
			if (1) {
				init_idle();
				electronic_lock_state = IDLE;
				keyboard_state = KEYBOARD_NUMBER;
				setTimer(SYSTEM_TIMER, 100);
			}
			break;
		case IDLE:
			idle();

			break;
		case RECEIVE_PASSWORD_NUMBER:

			break;

		case RECEIVE_PASSWORD_CHARACTER:

			break;
		case PROCESS_AND_CONTROL:

			break;
		case UNLOCK_DOOR:

			break;
		case ALERT:

			break;
		case LOCK_DOOR:

			break;
		case UPDATE_PASSWORD_NUMBER:

			break;
		case UPDATE_PASSWORD_CHARACTER:

			break;
		default:
			break;
	}
}

void init_idle() {
	// DO NOTHING
}
void init_receive_password_number();
void init_receive_password_character();
void init_process_and_control();
void init_unlock_door();
void init_alert();
void init_lock_door();
void init_update_password_number();
void init_update_password_character();

void idle_receive_password_number();
void idle_receive_password_character();

void init() {
	// DO NOTHING
}
void idle();
void receive_password_number();
void receive_password_character();
void process_and_control();
void unlock_door();
void alert();
void lock_door();
void update_password_number();
void update_password_character();