/*
 * sens.c
 *
 * Created: 20.5.2016 16:18:25
 *  Author: JernejZupan
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
 
int16_t hum[4];		//vlaga
int16_t temp[4];	//temperatura*10 za izpis desetink

void sens_init(void){
	for (uint8_t i=0; i<4; i++){
		hum[i]=0;	
		temp[i]=0;
	}

	i2c_init();
	DDRD |= (1<<PIND6)|(1<<PIND7);
}
	
void sens_measure(void){	
	// +-4.5 % HUM, +- 0.5 C TEMP
	uint8_t address = 0x27 << 1;	// must shift because of the way i2c lib was wiriten
	
	for (uint8_t i=0; i<4; i++){
		uint8_t raw[4]={0};
		PORTD &= 0x3f;
		PORTD |= i<<6;	// select sensor with mux (PIN6, PIN7)
			
		i2c_start(address | I2C_WRITE);
		_delay_ms(50);
		i2c_receive(address, raw, 4);
		
		// HUMIDITY CONVERSION
		int32_t hum_raw = ((int32_t)raw[0])*256;
		hum_raw += (int32_t)raw[1];
		hum[i] = (int16_t)(hum_raw/164);
		// TEMPERATURE CONVERSION (*10 for decimal value)
		int32_t temp_raw = (int32_t)raw[2]*64;
		temp_raw += raw[3]/4;
		temp[i] = (int16_t)(10*((int32_t)temp_raw)/99-400);
		_delay_ms(5);
	}
}

uint16_t sens_get_temp10(uint8_t s_num){	
	// ATOMIC
	return temp[s_num];
}

uint16_t sens_get_hum(uint8_t s_num){	
	// ATOMIC
	return hum[s_num];
}
