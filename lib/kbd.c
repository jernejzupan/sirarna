/*
 * kbd.c
 *
 * Created: 23.4.2016 17:22:17
 *  Author: JernejZupan
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "kbd.h"

void kbd_init(void){
	DDRB &= 0x0f;
	PORTB |= 0xf0;
}

uint8_t kbd_get(void){
	return 0x0f & (~(PINB>>4));
}