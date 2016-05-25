/*
 * test_lcd.c
 *
 * Created: 17.5.2016 21:00:21
 *  Author: JernejZupan
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

char* circular_coppy(char* si, char* so, int* i){
	int j=*i;
	while(*so!=0){
		*so++=si[j];
		if(si[++j]==0)j=0;
	}
	*i+=1;
	if(si[*i]==0)*i=0;	
}

int main(void){
	lcd_init_4d();
	_delay_ms(100);
	lcd_write_string_4d("****LCD*TEST****");
	lcd_write_instruction_4d(lcd_SetCursor|lcd_LineTwo);
	lcd_write_string_4d("****LCD*TEST****");
	_delay_ms(3000);
	char text_ln1[]="This is a lcd test. Please keep calm and carry on.                                ";
	int index1 = 0;
	int index2 = 16;
	char out1[]="xxxxxxxxxxxxxxxx";
	char out2[]="xxxxxxxxxxxxxxxx";
    while(1) {
		circular_coppy(text_ln1, out1, &index1);
		circular_coppy(text_ln1, out2, &index2);
		lcd_write_instruction_4d(lcd_Clear);
		lcd_write_string_4d(out1);
		lcd_write_instruction_4d(lcd_SetCursor|lcd_LineTwo);
		lcd_write_string_4d(out2);
		_delay_ms(500);		
    }
}