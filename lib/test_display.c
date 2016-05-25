/*
 * test_display.c
 *
 * Created: 20.5.2016 17:54:21
 *  Author: JernejZupan
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "display.h"

int main(void)
{
	display_init();
	uint8_t var=0;
    while(1)
    {
		uint16_t square = (uint16_t)var * (uint16_t)var;	
	    display_reset_buffer();
		printf("**DISPLAY*TEST**#2%u * %u =%u", var, var, square);
		display_print();
		_delay_ms(500);
		var++;
    }
}