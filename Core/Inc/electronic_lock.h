/*
 * electronic_lock.h
 *
 *  Created on: Nov 4, 2025
 *      Author: Dell
 */

#ifndef INC_ELECTRONIC_LOCK_H_
#define INC_ELECTRONIC_LOCK_H_

#include "main.h"
#include "global.h"
#include "lcd.h"
#include "picture.h"
#include "keyboard.h"
#include "software_timer.h"
#include "led_7seg.h"

void fsm_electronic_lock_run();

void ilde();
void receive_password();
void process_and_control();
void unlock_door();
void alert();
void lock_door();
void update_password();

#endif /* INC_ELECTRONIC_LOCK_H_ */
