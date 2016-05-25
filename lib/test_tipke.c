/*
 * tipke_test.c
 *
 * Created: 18.5.2016 23:17:09
 *  Author: JernejZupan
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "tipke.h"
#include "lcd.h"

int main(void)
{
	tipke_init();
	lcd_init_4d();
	char status_char[] = "UUUU";
	char line1[] = "T0=U    T1=U    ";
	char line2[] = "T2=U    T3=U    ";
	uint8_t delay_release[4]={0};
	while(1){
		tipke_driver();
		uint8_t tipka = tipka_get();
		uint8_t i = 0;
		
		if (tipka) {
			switch (tipka & T_TMASK) {
				case T1: i=0; break;
				case T2: i=1; break;
				case T3: i=2; break;
				case T4: i=3; break;
			}
			
			switch (tipke & T_SMASK){
				case T_UP:
					if(delay_release[i]>0){
						delay_release[i]--;
					}
					else{
						status_char[i]='U';
					}
					break;
				case T_DOWN:
					status_char[i]='D';
					break;
				case T_TAP:
					status_char[i]='T';
					delay_release[i]=5;
					break;
				case T_HOLD:
					status_char[i]='H';
					break;
				default:
					break;
			}
		}		
		line1[3]=status_char[0];
		line1[11]=status_char[1];
		line2[3]=status_char[2];
		line2[11]=status_char[3];
		lcd_write_instruction_4d(lcd_Home);
		lcd_write_string_4d(line1);
		lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);
		lcd_write_string_4d(line2);
		_delay_ms(100);
	}	
}