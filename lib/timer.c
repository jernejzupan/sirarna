/*
 * timer.c
 *
 * Created: 14.5.2016 16:15:22
 *  Author: JernejZupan
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "timer.h"

void (*ctc_irq)(void) = 0;	//timer interrupt routine
uint32_t _period_ms = 0;

void t_set_ctc_irq(void (*f)(void), uint32_t period_ms){
	ctc_irq = f;
	_period_ms = period_ms;
	TCCR1A = 0;					//clear timer control register 
	TIMSK1 = (1 << OCIE1A);		//enable CTC interrupt
	OCR1A = 124;				//interrupt period 8*(124+1)/1e6=1ms
				
}

void t_isr_enable(void){
	TIMSK1 = (1 << OCIE1A);		//enable CTC interrupt
	TCCR1B = (1 << CS11) | (1 << WGM12);
}
void t_isr_disable(void){
	TIMSK1 = 0;
	TCCR1B = 0;
}

ISR(TIMER1_COMPA_vect){
	static i=0;
	if(i>_period_ms){
		i=0;
		ctc_irq();
	}
	else i++;
}
