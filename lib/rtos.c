/*
 * rtos.c
 *
 *  Created on: 17. maj 2016
 *      Author: jernejzupan
 */

#include <stdio.h>
#include <stdlib.h>

#include "rtos.h"
#include "timer.h"
#include "cas.h"
#include "tipke.h"

void rtos_empty_slice(void);

void (*rtos_slice[]) (void) = {
	 cas_utrip
	,tipke_driver
	,rtos_empty_slice
	,tipke_driver
	,rtos_empty_slice

	,tipke_driver
	,rtos_empty_slice
	,tipke_driver
	,rtos_empty_slice
	,tipke_driver

	,rtos_empty_slice
	,tipke_driver
	,rtos_empty_slice
	,tipke_driver
	,rtos_empty_slice

	,tipke_driver
	,rtos_empty_slice
	,tipke_driver
	,rtos_empty_slice
	,tipke_driver
};

#define RTOS_NUM_OF_SLICES (sizeof(rtos_slice)/sizeof(*rtos_slice))

void rtos_init(void){
	t_set_ctc_irq(rtos_irq, 50);
	t_isr_enable();
	sei();
}

void rtos_irq(void){
	static int i=0;
	if(i<RTOS_NUM_OF_SLICES-1) i++;
	else i=0;
	rtos_slice[i](0);
}
