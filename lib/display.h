/*
 * display.h
 *
 * Created: 20.5.2016 16:16:19
 *  Author: JernejZupan
 */ 

#ifndef CAS_H_
#define CAS_H_

void display_init();
void display_reset_buffer(void);
void display_putc(char c);
void display_print(void);
void display_mod(uint8_t line,uint8_t column,char c);

#endif /* CAS_H_ */