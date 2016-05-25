 /*
 * test_rtos.c
 *
 * Created: 25.5.2016
 *  Author: JernejZupan
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include "rtos.h"
#include "cas.h"
#include "tipke.h"
#include "display.h"
#include "sens.h"

int main(void) {
	display_init();
	display_reset_buffer();
	printf("***RTOS*TEST****#2***RTOS*TEST****");
	display_print();
	_delay_ms(2000);
	
	cas_set(cas_dconv(2016,5,5),cas_uconv(10,0,0));
	cas_start();
	
	tipke_init();	
	sens_init();
	
	rtos_init();
	
	while (1) {
		sens_measure();
				
		Datum d=cas_dget();
		Ura u=cas_uget();
		
		display_reset_buffer();
		printf("%02u.%02u.%04u %02u:%02u:%02u#2T1=%02d.%d V1=%02d"
			, d.d, d.m, d.l, u.u, u.m);
			,sens_get_temp10(0)/10, sens_get_temp10(0)%10, sens_get_hum(0));
		display_print();		
		_delay_ms(200);		
	}
	
	return EXIT_SUCCESS;
}
