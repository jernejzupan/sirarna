/*
 * merilna_postaja.c
 *
 * Created: 22.4.2016 23:00:23
 *  Author: JernejZupan
 */ 

#include <avr/io.h>
#include <string.h>
#include <util/delay.h>

#include "lib/lcd.h"
#include "lib/kbd.h"
#include "lib/pos.h"
#include "lib/i2c.h"

void count_time(uint8_t* h, uint8_t* m, uint8_t* s);
char* print_time(char* data, uint8_t h, uint8_t m, uint8_t s);
char num_to_char(uint8_t num);
void pos_print_line(char* s);
void uint32_to_str(uint32_t num, char* str_ptr);

int main(void){
	uint8_t sys_cnt=0;
	char data[17]={0};
	uint8_t h=0;
	uint8_t m=0;
	uint8_t s=0;
	
	lcd_init_4d();
	kbd_init();
	pos_init();
	i2c_init();
	
	_delay_ms(500);
	DDRD |= (1<<PIND6)|(1<<PIND7);
	PORTD &= ~((1<<PIND6)|(1<<PIND7));
	uint8_t address = 0x27 << 1;
	uint8_t raw[4];
	_delay_ms(500);
	
		
	lcd_write_instruction_4d(lcd_Clear);
	lcd_write_string_4d("Merilna postaja");
	_delay_ms(1000);
	while(1)
    {	
		uint8_t refresh_lcd = 0;/*
		switch (kbd_get()) {
			case T0: s++; break;
			case T1: m++; break;
			case T2: h++; break;
			case T3: {
				pos_print_line("Hello world!\n"); break;
			}
			default: refresh_lcd=0;
		}*/
		/*
		char data[31]={0};
		static char i=224;		
		switch (kbd_get()) {
			case T0:
				for (int j=0; j<30; j++)
				{
					data[j]=i++;
				}
				data[30]='\n';
							 
				pos_print(data);
				break;			
			case T1: {
				pos_print(print_time(data, h, m, s));
				pos_print("\n");
				break;
			}
			case T2: break;
			case T3: break;
			default: refresh_lcd=0;	
			}		
		*/
		
		
		if (sys_cnt==0) {
			refresh_lcd = 1;
			count_time(&h, &m, &s);
			
			i2c_start(address | I2C_WRITE);
			_delay_ms(50);
			i2c_receive(address, raw, 4);

		}

	
			int16_t hum_raw = ((int16_t)raw[0])*256;
			hum_raw += (int16_t)raw[1];
			int16_t hum = hum_raw/164;
	
			int16_t temp_raw = (int16_t)raw[2]*64;
			temp_raw += raw[3]/4;
			int16_t temp = temp_raw/99-40;
			int16_t temp_dec = (temp_raw/10)%10;
			
		if (refresh_lcd) {
			lcd_write_instruction_4d(lcd_Clear);
			lcd_write_instruction_4d(lcd_Home);
			lcd_write_string_4d(print_time(data, h, m, s));
			lcd_write_instruction_4d(lcd_SetCursor | lcd_LineTwo);	
			char data_out[]="T1=  .  V1=     ";
			uint32_to_str((uint32_t)temp, &data_out[3]);
			uint32_to_str((uint32_t)temp_dec, &data_out[6]);
			uint32_to_str((uint32_t)hum, &data_out[11]);
			
			data_out[16]=0;
			lcd_write_string_4d(data_out);		
		}

		_delay_ms(100);
		
		if (sys_cnt < 10) sys_cnt++;
		else sys_cnt = 0;
    }
}

void count_time(uint8_t* h, uint8_t* m, uint8_t* s) {
	if(*s < 59) (*s)++;
	else { 
		*s = 0;
		if (*m < 59) (*m)++;
		else {
			*m = 0;
			if (*h < 23) (*h)++;
			else *h = 0;
		}
	}
}

char* print_time(char* data, uint8_t h, uint8_t m, uint8_t s) {
	char* p=data;
	uint8_t val=0;
	
	for(uint8_t i=0; i<3; i++) {
		p=data+3*i;
		switch(i) {
			case 0: val = h; break;
			case 1: val = m; break;
			case 2: val = s; break;
			default: val=0;
		}
		p[0]=num_to_char((val/10)%10);
		p[1]=num_to_char(val%10);
		p[2]=':';		
	}
	p[2]=0;
	return data;
}

char num_to_char(uint8_t num) {
	if (num < 10) return '0'+(char)num;
	else return ' ';
}

void uint32_to_str(uint32_t num, char* str_ptr){
	const char digit[] = "0123456789";	//define digits
	char tmp_str[11]={0};	//First char is actually stop char
	char *tmp_ptr1 = tmp_str;
	char *tmp_ptr2 = str_ptr;
	do{
        *(++tmp_ptr1) = digit[num%10];	//extract units digit
        num /= 10;					//whole num. devision right shift
    }while(num);					//loop while any nonzero digits left
	do{
		*tmp_ptr2++ = *tmp_ptr1--;		//copy in reverse order
	}while(*tmp_ptr1);				//loop until zero char detected
	//*tmp_ptr2=0;						//set zero char
}/* uart1_isTxBusy*/