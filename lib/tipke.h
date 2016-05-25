/*
 * tipke.h
 *
 * Created: 14.5.2016 16:19:02
 *  Author: JernejZupan
 */ 


#ifndef TIPKE_H_
#define TIPKE_H_

#define T_DRIVER_PERIOD 100
#define T_HOLD_TIME	1000
#define T_COUNTER	(T_HOLD_TIME/T_DRIVER_PERIOD)
#define T_BUFFER_SIZE	16

#define T_NOKEY 	0

#define T_TMASK 0xf0
#define T1	0x10
#define T2	0x20
#define T3	0x30
#define T4	0x40

#define T_SMASK 0x0f
#define T_UP	0x00
#define T_DOWN	0x01
#define T_TAP	0x02
#define T_HOLD	0x03



void tipke_init(void);
void tipke_driver(void);
uint8_t tipke_get(void);

#endif /* TIPKE_H_ */