/*
 * test_pos.c
 *
 * Created: 17.5.2016 21:02:25
 *  Author: JernejZupan
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include "pos.h"

int main(void)
{
	pos_init();
	pos_set_stream();
	printf("Hello world %02d:%02d\n", 24,17);
    while(1);
}