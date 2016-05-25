/*
 * test_kbd.c
 *
 * Created: 17.5.2016 21:38:40
 *  Author: JernejZupan
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "kbd.h"

#define led_on() 	{DDRD|=1<<PIND0; PORTD|=1<<PIND0;}
#define led_off()	{PORTD&=~(1<<PIND0);}	

int main(void)
{
	uint16_t patern = 0b0000000000000000;
	uint8_t i = 0;
	
	kbd_init();
    while(1)
    {
        uint8_t kbd_value = kbd_get();
		if (kbd_value & (1<<KBD_T0)) patern = 0b1111111100000000;
		if (kbd_value & (1<<KBD_T1)) patern = 0b1111000011110000;
		if (kbd_value & (1<<KBD_T2)) patern = 0b1100110011001100;
		if (kbd_value & (1<<KBD_T3)) patern = 0b1010101010101010;
		if(patern & (1<<i++)) {led_on();}
		else {led_off();}
		if(i>15) i=0;
		_delay_ms(100);	
    }
	
}