/*
 * test_cas.c
 *
 * Created: 17.5.2016 23:50:56
 *  Author: JernejZupan
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "cas.h"
#include "lcd.h"
						//|____LINE1_____||____LINE2_____|
char lcd_string_buffer[]="****CAS*TEST********CAS*TEST****";
uint8_t lcd_index;
void reset_lcd(void){
	lcd_index = 0;
}
void putchar_to_lcd(char c){
	if(lcd_index<32)lcd_string_buffer[lcd_index++]=c;	
}

void lcd_print(void){
	char line1[17] = {0};
	char line2[17] = {0};
	for (uint8_t i=0; i<16; i++) line1[i]=lcd_string_buffer[i];
	for (uint8_t i=0; i<16; i++) line2[i]=lcd_string_buffer[i+16];
	lcd_write_instruction_4d(lcd_Clear);
	lcd_write_string_4d(line1);
	lcd_write_instruction_4d(lcd_SetCursor|lcd_LineTwo);
	lcd_write_string_4d(line2);
}

static FILE mystdout = FDEV_SETUP_STREAM(putchar_to_lcd, NULL, _FDEV_SETUP_WRITE);

void test_10sec(char* test_name, uint8_t sec, uint8_t min, uint8_t ura, uint8_t dan, uint8_t mesec, uint16_t leto){
	reset_lcd();
	printf(test_name);
	lcd_print();
	_delay_ms(2000);
	cas_set(cas_dconv(leto,mesec,dan),cas_uconv(ura,min,sec));
	cas_start();
	for (uint8_t i=0; i<10; i++){
		Datum d=cas_dget();
		Ura u=cas_uget();
		reset_lcd();
		printf("%02u.%02u.%04u      %02u:%02u:%02u         ", d.d, d.m, d.l, u.u, u.m, u.s);
		lcd_print();
		_delay_ms(1000);
		cas_utrip();
	}			
}

int main(void)
{
	stdout = &mystdout;
	lcd_init_4d();
	_delay_ms(100);
	lcd_print();
	_delay_ms(3000);	
    while(1)
    {
		//JANUAR
		test_10sec("JANUAR1 BREZ PREHODA            ",55,59,23,30,1,2016);
		test_10sec("JANUAR2 PREHOD                  ",55,59,23,31,1,2016);
		//FEBRUAR
		test_10sec("FEBRUAR1 NORMALNO BREZ PREHODA  ",55,59,23,27,2,2015);
		test_10sec("FEBRUAR2 NORMALNO PREHOD        ",55,59,23,28,2,2015);
		test_10sec("FEBRUAR3 PRESTOPNO BREZ PREHODA ",55,59,23,28,2,2016);
		test_10sec("FEBRUAR4 PRESTOPNO PREHOD       ",55,59,23,29,2,2016);
		test_10sec("FEBRUAR5 100 LET BREZ PREHODA   ",55,59,23,27,2,2100);
		test_10sec("FEBRUAR6 100 LET PREHOD         ",55,59,23,28,2,2100);
		test_10sec("FEBRUAR7 400 LET BREZ PREHODA   ",55,59,23,28,2,2400);
		test_10sec("FEBRUAR8 400 LET PREHOD         ",55,59,23,29,2,2400);
		//MAREC
		test_10sec("MAREC1 BREZ PREHODA             ",55,59,23,30,3,2016);
		test_10sec("MAREC2 PREHOD                   ",55,59,23,31,3,2016);
		//APRIL
		test_10sec("APRIL1 BREZ PREHODA             ",55,59,23,29,4,2016);
		test_10sec("APRIL2 PREHOD                   ",55,59,23,30,4,2016);
		//MAJ
		test_10sec("MAJ1 BREZ PREHODA               ",55,59,23,30,5,2016);
		test_10sec("MAJ2 PREHOD                     ",55,59,23,31,5,2016);
    }
}