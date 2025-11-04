/*
 * electronic_lock.c
 *
 *  Created on: Nov 4, 2025
 *      Author: Dell
 */

#include "electronic_lock.h"

uint8_t electronic_lock_state = INIT;

void fsm_electronic_lock_run() {
	switch(electronic_lock_state) {
		case INIT:

			break;
		case IDLE:

			break;
		case RECEIVE_PASSWORD:

			break;
		case PROCESS_AND_CONTROL:

			break;
		case UNLOCK_DOOR:

			break;
		case ALERT:

			break;
		case LOCK_DOOR:

			break;
		case UPDATE_PASSWORD:

			break;
		default:
			break;
	}
}

void ilde() {

}

void receive_password();

void process_and_control();

void unlock_door();

void alert();

void lock_door();

void update_password();

