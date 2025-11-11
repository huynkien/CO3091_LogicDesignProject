/*
 * electronic_lock.h
 *
 *  Created on: Nov 4, 2025
 *      Author: Dell
 */

#ifndef INC_ELECTRONIC_LOCK_H_
#define INC_ELECTRONIC_LOCK_H_

#include "main.h"
#include "button.h"
#include "global.h"
#include "lcd.h"
#include "picture.h"
//#include "keyboard.h"
#include "software_timer.h"
#include "led_7seg.h"

extern uint8_t correct_password[4];
extern uint8_t entered_password[4];

uint8_t check_password();

void fsm_electronic_lock_run();

void init_idle();
void init_receive_password_number();
void init_receive_password_character();
void init_process_and_control();
void init_unlock_door();
void init_door_open();
void init_door_close();
void init_alert();
void init_lock_door();
void init_update_password_number();
void init_update_password_character();

void idle_receive_password_number();
void idle_receive_password_character();

void init();
void idle();
void receive_password_number();
void receive_password_character();
void process_and_control();
void unlock_door();
void door_open();
void door_close();
void alert();
void lock_door();
void password_incorrect();
void update_password_number();
void update_password_character();

//Thử trường hợp lockout
void init_lockout(uint32_t ms);
void lockout();
void Show_7SEGMENT(uint16_t seconds);

//update password
void init_update_password();


#endif /* INC_ELECTRONIC_LOCK_H_ */
