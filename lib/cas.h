/*
 * cas.h
 *
 * Created: 14.5.2016 16:16:19
 *  Author: JernejZupan
 */ 


#ifndef CAS_H_
#define CAS_H_

typedef struct{
	uint8_t u;
	uint8_t m;
	uint8_t s;
} Ura;

typedef struct{
	uint16_t l;
	uint8_t d;
	uint8_t m;
} Datum;

void cas_set(Datum ds, Ura us);
void cas_start(void);
void cas_stop(void);
void cas_utrip(void);

Datum cas_dget(void);
Datum cas_dconv(uint16_t l, uint8_t m, uint8_t d);
uint8_t cas_deq(Datum d1, Datum d2);

Ura cas_uget(void);
Ura cas_uconv(uint8_t u, uint8_t m, uint8_t s);
uint8_t cas_ueq(Ura u1, Ura u2);
Ura cas_usum(Ura u1, Ura u2);

#endif /* CAS_H_ */