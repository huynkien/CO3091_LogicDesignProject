/*
 * global.h
 *
 *  Created on: Nov 4, 2025
 *      Author: Dell
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "main.h"

// ELECTRONIC LOCK SYSTEM
#define INIT					0
#define IDLE					1
#define RECEIVE_PASSWORD		2
#define PROCESS_AND_CONTROL		3
#define UNLOCK_DOOR				4
#define ALERT					5
#define LOCK_DOOR				6
#define UPDATE_PASSWORD			7

// KEYBOARD
#define NUMBER					10
#define CHARACTER				11

// TIMER
#define NORMAL_TIMER			0

extern uint8_t electronic_lock_state;
extern uint8_t keyboard_state;

#endif /* INC_GLOBAL_H_ */
