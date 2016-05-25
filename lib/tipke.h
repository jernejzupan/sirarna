/*
 * tipke.h
 *
 * Created: 14.5.2016 16:19:02
 *  Author: JernejZupan
 */ 


#ifndef TIPKE_H_
#define TIPKE_H_

#define T_DRIVER_PERIOD 100
#define T_HOLD_TIME		1000
#define T_COUNTER		(T_HOLD_TIME/T_DRIVER_PERIOD)

#define TIPKA0	0
#define TIPKA1	1
#define TIPKA2	2
#define TIPKA3	3

#define T_UP	0
#define T_DOWN	1
#define T_TAP	2
#define T_HOLD	3

void tipke_init(void);
void tipke_driver(void);
uint8_t tipke_get(uint8_t tipka);

#endif /* TIPKE_H_ */