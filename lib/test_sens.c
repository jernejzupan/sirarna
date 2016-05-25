/*
 * test_sens.c
 *
 * Created: 20.5.2016 21:38:40
 *  Author: JernejZupan
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "sens.h"
#include "display.h"

int main(void)
{
	display_init();
	sens_init();
	
	display_reset_buffer();
	printf("***SENS*TEST****#2***SENS*TEST****");
	display_print();
	_delay_ms(1000);
	
	while (1) {		
		sens_measure();
		display_reset_buffer();
		printf("T1=%02d.%d V1=%02d #2T2=%02d.%d V2=%02d"
			,sens_get_temp10(0)/10, sens_get_temp10(0)%10, sens_get_hum(0)
			,sens_get_temp10(1)/10, sens_get_temp10(1)%10, sens_get_hum(1));
		display_print();
		_delay_ms(200);
	}
	
	return 0;	
}