/*
 * display.c
 *
 * Created: 20.5.2016 16:16:26
 *  Author: JernejZupan
 */ 

 #include <avr/io.h>
 #include <string.h>
 #include <stdio.h>
 #include <util/atomic.h>
 #include "lcd.h"
 #include "display.h"

static FILE display_stream = FDEV_SETUP_STREAM(display_putc, NULL, _FDEV_SETUP_WRITE);

#define DISPLAY_BUFFER_SIZE 40
#define DISPLAY_ROW_LEN		16
	
char display_buffer[DISPLAY_BUFFER_SIZE+1];
uint8_t buffer_index;

void display_init(){
	lcd_init_4d();
	display_reset_buffer();
}

void display_reset_buffer(void){
	stdout = &display_stream; 
	buffer_index = 0;
}
void display_putc(char c){
	if (buffer_index<DISPLAY_BUFFER_SIZE) {
		display_buffer[buffer_index++]=c;
		display_buffer[buffer_index]='\0';
	}		
}

void display_print(void){
	char line1[] = "                ";
	char line2[] = "                ";
	uint8_t ib=0;
	uint8_t il1=0;
	uint8_t il2=0;
	uint8_t line_select = 1;
	
	while (display_buffer[ib]>0  ) {
		char sym = display_buffer[ib++];
		if (sym == '#') {
			sym = display_buffer[ib++];
			if (sym = '1') line_select=1;
			if (sym = '2') line_select=2;
			if (sym = '\0') break;
		}		
		else {
			if (line_select == 1 && il1 < DISPLAY_ROW_LEN) line1[il1++]=sym;
			if (line_select == 2 && il2 < DISPLAY_ROW_LEN) line2[il2++]=sym;
		}	
	}
	
	lcd_write_instruction_4d(lcd_Clear);
	lcd_write_string_4d(line1);
	lcd_write_instruction_4d(lcd_SetCursor|lcd_LineTwo);
	lcd_write_string_4d(line2);
}