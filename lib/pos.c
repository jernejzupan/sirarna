/*
 * pos.c
 *
 * Created: 5.5.2016 22:40:42
 *  Author: JernejZupan
 */ 



#include <avr/io.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include "pos.h"

static FILE pos_stream = FDEV_SETUP_STREAM(pos_print_char, NULL, _FDEV_SETUP_WRITE);

void pos_set_stream(void){
	stdout = &pos_stream; 
}

void pos_init (void) {
	PORTB |= 0x04;
	DDRB |= 0x07;
}

void pos_print_char(char c) {
	PORTB |= 0x04;
	for (uint8_t i=0; i<8; i++) {
		PORTB = ~0x01;
		if (c & (0x80>>i)) PORTB |= 0x02;
		else PORTB &= ~0x02;
		PORTB |= 0x01;
	}
	PORTB &= ~0x01;
	_delay_ms(10);
	PORTB &= ~0x04;
	_delay_ms(10);
	PORTB |= 0x04;
	_delay_ms(10);
}

void pos_print(char* s) {
	while (*s>0) {
		pos_print_char(*s);
		s++;
	}	
}