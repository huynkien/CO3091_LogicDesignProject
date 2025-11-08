/* * global.h
 *
 *  Created on: Nov 4, 2025
 *      Author: Dell
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "main.h"

// ELECTRONIC LOCK SYSTEM
#define INIT						0
#define IDLE						1
#define RECEIVE_PASSWORD_NUMBER		2
#define RECEIVE_PASSWORD_CHARACTER	3
#define PROCESS_AND_CONTROL			4
#define UNLOCK_DOOR					5
#define ALERT						6
#define LOCK_DOOR					7
#define UPDATE_PASSWORD_NUMBER		8
#define UPDATE_PASSWORD_CHARACTER	9

// KEYBOARD
#define KEYBOARD_NUMBER				20
#define KEYBOARD_CHARACTER			21
#define KEYBOARD_CP_NUMBER			22
#define KEYBOARD_CP_CHARACTER 		23

// KEYBOARD BUTTON
#define NUMBER_0					30
#define NUMBER_1					31
#define NUMBER_2					32
#define NUMBER_3					33
#define NUMBER_4					34
#define NUMBER_5					35
#define NUMBER_6					36
#define NUMBER_7					37
#define NUMBER_8					38
#define NUMBER_9					39

// NO BUTTON / invalid buffer value
#define NO_SIGNAL                   255

// KEYBOARD CHARACTER
#define CHARACTER_A					40
#define CHARACTER_B					41
#define CHARACTER_C					42
#define CHARACTER_D					43
#define CHARACTER_E					44
#define CHARACTER_F					45

//KEYBOARD FUNCTION
#define DELETE						50
#define DELETE_ALL					51
#define UNLOCK						52
#define OPEN_DOOR					53
#define CP							54
#define VALIDATE					55
#define CHAR_A_F					56
#define NUM_0_9						57


// TIMER
#define SYSTEM_TIMER			0
#define LED_7SEG				1
#define TIMER_15S				2
#define TIMER_30S				3

#define TIME_15S				15000
#define TIME_30S 				30000

extern uint8_t electronic_lock_state;
extern uint8_t keyboard_state;
extern uint8_t keyboard_butbuf;

#endif /* INC_GLOBAL_H_ */
