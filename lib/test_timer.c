/*
 * test_timer.c
 *
 * Created: 17.5.2016 19:27:17
 *  Author: JernejZupan
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"

#define led_on() 	{DDRD|=1<<PIND0; PORTD|=1<<PIND0;}
#define led_off()	{PORTD&=~(1<<PIND0);}	

void blin_isr(void){
	static uint8_t i = 0;
	if(i>9){
		led_on();
		i = 0;
	}
	else {
		led_off();
		i++;
	}
}

int main(void)
{
	t_set_ctc_irq(blin_isr, 100);
	t_isr_enable();
	sei();
    while(1);
}