/*
 * test_pos.c
 *
 * Created: 17.5.2016 21:02:25
 *  Author: JernejZupan
 */ 

#include <avr/io.h>
#include "pos.h"

int main(void)
{
	pos_init();
	pos_print("Hello world\n");
    while(1);
}