/*
 * sens.h
 *
 * Created: 20.5.2016 16:18:20
 *  Author: JernejZupan
 */ 


#ifndef SENS_H_
#define SENS_H_

void sens_init(void);
void sens_measure(void);
int16_t sens_get_temp10(uint8_t s_num);
int16_t sens_get_hum(uint8_t s_num);

#endif /* SENS_H_ */