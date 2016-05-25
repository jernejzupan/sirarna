/*
 * tipke.c
 *
 * Created: 14.5.2016 16:18:55
 *  Author: JernejZupan
 */ 

#include <avr/io.h>
#include "tipke.h"
//#include "kbd.h"
#include "kbd.h"

uint8_t kbd_hold_counter[4];
uint8_t kbd_status[4];

void tipke_init(void){
	kbd_init();
}

void tipke_driver(void){
	for (uint8_t i=0; i<4; i++){
		if (kbd_get()&(1<<i)){
			if (kbd_status[i]==T_UP) kbd_status[i] = T_DOWN;
			if (kbd_hold_counter[i]>T_COUNTER){
				kbd_status[i] = T_HOLD;
			}
			else kbd_hold_counter[i]++;
		}
		else {
			if (kbd_status[i]==T_DOWN) kbd_status[i]=T_TAP;
			else kbd_status[i]=T_UP;
			kbd_hold_counter[i]=0;
		}		 
	}
}

uint8_t tipke_get(uint8_t tipka){
	return kbd_status[tipka];
}