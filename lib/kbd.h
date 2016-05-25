/*
 * kbd.h
 *
 * Created: 23.4.2016 17:22:06
 *  Author: JernejZupan
 */ 

#ifndef KBD_H_
#define KBD_H_

#define KBD_T0 0
#define KBD_T1 1
#define KBD_T2 2
#define KBD_T3 3

void kbd_init(void);
uint8_t kbd_get(void);

#endif /* KBD_H_ */