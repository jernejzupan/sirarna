 /*
 * test_rtos.c
 *
 * Created: 25.5.2016
 *  Author: JernejZupan
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "rtos.h"
#include "cas.h"
#include "tipke.h"
#include "display.h"
#include "sens.h"
#include "pos.h"

int main(void) {
	display_init();
	pos_init();
	display_reset_buffer();
	printf("***RTOS*TEST****#2***RTOS*TEST****");
	display_print();
	pos_set_stream();
	printf("***RTOS*TEST*******RTOS*TEST****\n");
	_delay_ms(2000);
	
	cas_set(cas_dconv(2016,26,5),cas_uconv(2,36,0));
	cas_start();
	tipke_init();	
	sens_init();
	rtos_init();
	
	uint8_t ure_meritve=3;
	uint8_t view_switch_cnt=0;
	
	while (1) {
		sens_measure();
				
		Datum d=cas_dget();
		Ura u=cas_uget();		
		
		if(cas_ueq(u,cas_uconv(ure_meritve,0,0))){
				pos_set_stream();
				printf("%02u.%02u %02u:%02u T1=%02d.%d V1=%02d T2=%02d.%d V2=%02d\n"
				, d.d, d.m, u.u, u.m
				,sens_get_temp10(0)/10, sens_get_temp10(0)%10, sens_get_hum(0)
				,sens_get_temp10(1)/10, sens_get_temp10(1)%10, sens_get_hum(1));
				ure_meritve=(ure_meritve+1)%24;
		}
				
		display_reset_buffer();
		if (view_switch_cnt<20) {
			printf("%02u.%02u %02u:%02u:%02u#2T1=%02d.%d V1=%02d"
				, d.d, d.m, u.u, u.m, u.s
				,sens_get_temp10(0)/10, sens_get_temp10(0)%10, sens_get_hum(0));
			view_switch_cnt++;
		}
		else if (view_switch_cnt<40) {
			printf("%02u.%02u %02u:%02u:%02u#2T2=%02d.%d V2=%02d"
				, d.d, d.m, u.u, u.m, u.s
				,sens_get_temp10(1)/10, sens_get_temp10(1)%10, sens_get_hum(1));
			view_switch_cnt++;
		}
		else view_switch_cnt=0;	
		display_print();			
	}
	
	return 0;
}
