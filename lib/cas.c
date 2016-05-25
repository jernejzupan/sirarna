/*
 * cas.c
 *
 * Created: 14.5.2016 16:16:26
 *  Author: JernejZupan
 */ 

 #include <avr/io.h>
 #include "cas.h"
 
uint8_t e = 0;
Ura u;
Datum d;

void cas_set(Datum ds, Ura us){
	u = us;
	d = ds;
}

void cas_start(void){
	e = 1;
}

void cas_stop(void){
	e = 0;
}

void cas_utrip(void){
	if (e) {
		// SEKUNDE
		if (u.s<59)u.s++;
		else {
			u.s = 0;
			// MINUTE
			if (u.m<59)u.m++;
			else {
				// URA
				u.m = 0;
				if (u.u<23)u.u++;
				else {
					u.u=0;
					// MESECI
					switch (d.m) {
						// APRIL, JUNIJ, SEPTEMBER, NOVEMBER
						case 4:
						case 6:
						case 9:
						case 11:
							if (d.d<30)d.d++;
							else {
								d.d=1;
								d.m++;
							}
							break;
						// FEBRUAR
						case 2:
							if (d.d<28)d.d++;
							else {
								// PRESTOPNA LETA
								if ((d.l%4==0 && d.l%100!=0)||d.l%400==0) {
									if (d.d<29)d.d++;
									else {
										d.d=1;
										d.m++;
									}
								}
								else {
									d.d=1;
									d.m++;
								}
							}
							break;
						default:					
							if(d.d<31)d.d++;
							else{
								d.d=1;
								if(d.m<12)d.m++;
								else d.m = 1;
							}	
					}					
				}
			}
		}
	}
}

Datum cas_dget(void){
	return d;
}

Datum cas_dconv(uint16_t l, uint8_t m, uint8_t d){
	Datum dout;
	dout.l=l;
	dout.m=m;
	dout.d=d;	
	return dout;
}

uint8_t cas_deq(Datum d1, Datum d2){
	if ((d1.l == d2.l) && (d1.m == d2.m) && (d1.d == d2.d)) return 1;
	else return 0;
}

Ura cas_uget(void){
	return u;
}

Ura cas_uconv(uint8_t u, uint8_t m, uint8_t s){
	Ura uout;
	uout.u=u;
	uout.m=m;
	uout.s=s;
	return uout;
}

uint8_t cas_ueq(Ura u1, Ura u2){
	if ((u1.s == u2.s) && (u1.m == u2.m) && (u1.u == u2.u)) return 1;
	else return 0;
}

Ura cas_usum(Ura u1, Ura u2){
	u1.s += u2.s;
	if (u1.s > 59) {
		u1.s %= 60;
		u1.m++;
	}
	u1.m += u2.m;
	if (u1.m > 59) {
		u1.m %= 60;
		u1.u++;
	}
	u1.u += u2.u;
	if (u1.u > 23){
		u1.u %= 24;
	}
	return u1;
}
