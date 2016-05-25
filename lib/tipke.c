/*
 * tipke.c
 *
 * Created: 14.5.2016 16:18:55
 *  Author: JernejZupan
 */ 

#include <avr/io.h>
#include <util/atomic.h>
#include "tipke.h"
#include "kbd.h"

uint8_t kbd_hold_counter[4];
uint8_t kbd_status[4];
uint8_t tipke_buffer[T_BUFFER_SIZE];

volatile uint8_t buffer_head;
volatile uint8_t buffer_tail;


static void tipke_write_buffer(uint8_t val){
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		uint8_t head_tmp = (buffer_head+1)%T_BUFFER_SIZE;
		if (head_tmp!=buffer_tail) {
			tipke_buffer[buffer_head]=val;
			buffer_head=head_tmp;
		}
	}
}

static uint8_t val tipke_read_buffer(volatile){
	uint8_t val=T_NOKEY;
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		if (buffer_head!=buffer_tail) {
			val=tipke_buffer[buffer_tail];
			buffer_tail=(buffer_tail+1)%T_BUFFER_SIZE;
		}
	}
	return val;
}


void tipke_init(void){
	kbd_init();
	buffer_head=0;
	buffer_tail=0;
}

void tipke_driver(void){
	uint8_t kbd_current = kbd_get();
	for (uint8_t i=0; i<4; i++){
		if (kbd_current&(1<<i)){
			if (kbd_status[i]==T_UP){
				kbd_status[i] = T_DOWN;
				tipke_write_buffer((i+1)<<4 | T_DOWN);
			}
			if (kbd_hold_counter[i]>T_COUNTER){
				kbd_status[i] = T_HOLD;
				tipke_write_buffer((i+1)<<4 | T_HOLD);
			}
			else kbd_hold_counter[i]++;
		}
		else {
			if (kbd_status[i]==T_DOWN){
				kbd_status[i]=T_TAP;
				tipke_write_buffer((i+1)<<4 | T_TAP);
			}
			else 
			{
				if (kbd_status[i]!=T_UP){
					kbd_status[i]=T_UP;
					kbd_hold_counter[i]=0;
					tipke_write_buffer((i+1)<<4 | T_UP);
				}
			}
		}		 
	}
}

uint8_t tipke_get(void){
	return tipke_read_buffer();
}