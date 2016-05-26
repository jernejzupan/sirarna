/*
 * merilna_postaja.c
 *
 * Created: 22.4.2016 23:00:23
 *  Author: JernejZupan
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "lib/rtos.h"
#include "lib/cas.h"
#include "lib/tipke.h"
#include "lib/display.h"
#include "lib/sens.h"
#include "lib/pos.h"

#define VIEW_MASK	0xf0
#define SUBVIEW_MASK	0x0f

#define UI_TIME			0x10
 #define UI_TIME_NORMAL		0x01
#define UI_SET_TIME		0x20
 #define UI_SET_TIME_YEAR     	0x02
 #define UI_SET_TIME_MONTH    	0x03
 #define UI_SET_TIME_DAY      	0x04
 #define UI_SET_TIME_HOUR     	0x05
 #define UI_SET_TIME_MIN      	0x06
 #define UI_SET_TIME_SEC      	0x07
#define UI_MEAS			0x30
 #define UI_MEAS_NORMAL		0x01

 //MODULAR INCREMENT/DECREMENT WITH UPPER AND LOWER BOUNDRY
#define M_INCR(X,MIN,MAX) ((X+MAX-MIN+1)%MAX+MIN)
#define M_DECR(X,MIN,MAX) ((X+MAX-MIN+1)%MAX+MIN)
 
int main(void) {
	display_init();
	pos_init();
	display_reset_buffer();
	printf("MERILNA POSTAJA#2SIRARNA");
	display_print();
	_delay_ms(2000);
	
	cas_set(cas_dconv(2016,26,5),cas_uconv(2,36,0));
	cas_start();
	tipke_init();	
	sens_init();
	rtos_init();
	
	uint16_t view_switch_cnt=0;
	uint8_t ure_meritve=0;
	uint8_t view_mode=UI_TIME&UI_TIME_NORMAL;
	uint8_t tipka_hold[5]={0};
	
	while (1) {
		sens_measure();		
		Datum d=cas_dget();
		Ura u=cas_uget();
		tipke=get_tipke();
		
		if (T1|T_HOLD) 	tipka_hold[1]=1;
		if (T1|T_UP) 	tipka_hold[1]=0;
		if (T2|T_HOLD) 	tipka_hold[2]=1;
		if (T2|T_UP) 	tipka_hold[2]=0;
		if (T3|T_HOLD) 	tipka_hold[3]=1;
		if (T3|T_UP) 	tipka_hold[3]=0;
		if (T4|T_HOLD) 	tipka_hold[4]=1;
		if (T4|T_UP) 	tipka_hold[4]=0;
		
		// USER INTERFACE
		display_reset_buffer();
		switch (VIEW_MASK&view_mode) {
			case UI_TIME:
				if (tipke==T1|T_HOLD) {
					view_mode=SET_TIME&UI_SET_TIME_YEAR;
					view_switch_cnt=0;
					break;
				}			
				if (view_switch_cnt>50 || tipke==T1|T_TAP){
					view_mode=UI_MEAS&UI_MEAS_NORMAL;
					view_switch_cnt=0;
					break;
				}
				printf("%02u.%02u.%4u #2%02u:%02u:%02u"
					, d.d, d.m, d.l, u.u, u.m, u.s);
				view_switch_cnt++;
				break;
			case UI_MEAS:
				if (tipke==T1|T_HOLD) {
					view_mode=SET_TIME&UI_SET_TIME_YEAR;
					view_switch_cnt=0;
					break;
				}
				if (view_switch_cnt>50 || tipke==T1|T_TAP){
					view_mode=UI_TIME&UI_TIME_NORMAL;
					view_switch_cnt=0;
					break;
				}			
				printf("T1=%02d.%d V1=%02d#2T2=%02d.%d V2=%02d"
					,sens_get_temp10(0)/10, sens_get_temp10(0)%10, sens_get_hum(0)
					,sens_get_temp10(1)/10, sens_get_temp10(1)%10, sens_get_hum(1));
				view_switch_cnt++;
				break;
			case UI_SET_TIME:
				if (tipke==T1|T_TAP) {
					view_switch_cnt=0;
					switch (SUBVIEW_MASK&view_mode) {
						case UI_SET_TIME_YEAR:
							view_mode=UI_SET_TIME&UI_SET_TIME_MONTH;
							break;
						case UI_SET_TIME_MONTH:
							view_mode=UI_SET_TIME&UI_SET_TIME_DAY;
							break;
						case UI_SET_TIME_DAY: 
							view_mode=UI_SET_TIME&UI_SET_TIME_HOUR;
							break;
						case UI_SET_TIME_HOUR:
							view_mode=UI_SET_TIME&UI_SET_TIME_MIN;
							break;
						case UI_SET_TIME_MIN:
							view_mode=UI_SET_TIME&UI_SET_TIME_SEC;
							break;
						case UI_SET_TIME_SEC:  
							view_mode=UI_TIME&UI_TIME_NORMAL;
							break;
						default: 
							view_mode=UI_TIME&UI_TIME_NORMAL;
							break;							
					}
					break;
				}				
				if (tipke==T3|T_TAP || tipka_hold[3]) {
					view_switch_cnt=0;
					switch (SUBVIEW_MASK&view_mode) {
						case UI_SET_TIME_YEAR:
							cas_set(cas_dconv(d.l+1,d.m,d.d),cas_uconv(u.u,u.m,u.s));
							break;
						case UI_SET_TIME_MONTH:
							cas_set(cas_dconv(d.l,M_INCR(d.m,1,12),d.d),cas_uconv(u.u,u.m,u.s));
							break;
						case UI_SET_TIME_DAY: 
							cas_set(cas_dconv(d.l,d.m,M_INCR(d.d,1,31),cas_uconv(u.u,u.m,u.s));
							break;
						case UI_SET_TIME_HOUR:
							cas_set(cas_dconv(d.l,d.m,d.d),cas_uconv(M_INCR(u.u,0,23),u.m,u.s));
							break;
						case UI_SET_TIME_MIN:
							cas_set(cas_dconv(d.l,d.m,d.d),cas_uconv(u.u,M_INCR(u.m,0,60),u.s));
							break;
						case UI_SET_TIME_SEC:  
							cas_set(cas_dconv(d.l,d.m,d.d),cas_uconv(u.u,u.m,M_INCR(u.s,0,60)));
							break;
						default: 
							break;							
					}
					break;
				}				
				if (tipke==T4|T_TAP || tipka_hold[4]) {
					view_switch_cnt=0;
					switch (SUBVIEW_MASK&view_mode) {
						case UI_SET_TIME_YEAR:
							cas_set(cas_dconv(d.l+1,d.m,d.d),cas_uconv(u.u,u.m,u.s));
							break;
						case UI_SET_TIME_MONTH:
							cas_set(cas_dconv(d.l,M_DECR(d.m,1,12),d.d),cas_uconv(u.u,u.m,u.s));
							break;
						case UI_SET_TIME_DAY: 
							cas_set(cas_dconv(d.l,d.m,M_DECR(d.d,1,31),cas_uconv(u.u,u.m,u.s));
							break;
						case UI_SET_TIME_HOUR:
							cas_set(cas_dconv(d.l,d.m,d.d),cas_uconv(M_DECR(u.u,0,23),u.m,u.s));
							break;
						case UI_SET_TIME_MIN:
							cas_set(cas_dconv(d.l,d.m,d.d),cas_uconv(u.u,M_DECR(u.m,0,60),u.s));
							break;
						case UI_SET_TIME_SEC:  
							cas_set(cas_dconv(d.l,d.m,d.d),cas_uconv(u.u,u.m,M_DECR(u.s,0,60)));
							break;
						default: 
							break;							
					}
					break;
				}				
				if (view_switch_cnt>200){
					view_mode=UI_TIME&UI_TIME_NORMAL;
					view_switch_cnt=0;
					break;
				}
				printf("%02u.%02u.%4u #2%02u:%02u:%02u"
					, d.d, d.m, d.l, u.u, u.m, u.s);
				// MODIFIY DISPLAY OUTPUT BUFFER BEFORE PRINTING 10.10.2016
				if ((view_switch_cnt%10)<3){
					switch (SUBVIEW_MASK&view_mode) {
						case UI_SET_TIME_YEAR:
							display_mod(0,6,' ');
							display_mod(0,7,' ');
							display_mod(0,8,' ');
							display_mod(0,9,' ');
							break;
						case UI_SET_TIME_MONTH:
							display_mod(0,3,' ');
							display_mod(0,4,' ');
							break;
						case UI_SET_TIME_DAY: 
							display_mod(0,0,' ');
							display_mod(0,1,' ');
							break;
						case UI_SET_TIME_HOUR:
							display_mod(1,0,' ');
							display_mod(1,1,' ');
							break;
						case UI_SET_TIME_MIN:
							display_mod(1,3,' ');
							display_mod(1,4,' ');
							break;
						case UI_SET_TIME_SEC:  
							display_mod(1,6,' ');
							display_mod(1,7,' ');
							break;
						default: 
							break;						
					}					
				}//MODIFIY DISPLAY	
				view_switch_cnt++;			
		}
		display_print();//END USER INTERFACE

		// PRINT EVENTS
		if(cas_ueq(u,cas_uconv(ure_meritve,0,0))){
				pos_set_stream();
				printf("%02u.%02u %02u:%02u T1=%02d.%d V1=%02d T2=%02d.%d V2=%02d\n"
				, d.d, d.m, u.u, u.m
				,sens_get_temp10(0)/10, sens_get_temp10(0)%10, sens_get_hum(0)
				,sens_get_temp10(1)/10, sens_get_temp10(1)%10, sens_get_hum(1));
				ure_meritve=(ure_meritve+1)%24;
		}
			
	}
	
	return 0;
}
