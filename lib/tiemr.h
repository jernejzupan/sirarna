/*
 * tiemr.h
 *
 * Created: 14.5.2016 16:15:31
 *  Author: JernejZupan
 */ 

#ifndef TIMER_H_
#define TIMER_H_

// Timer1 ctc interrupt routine function pointer type
typedef void(*t_f_ctc_isr)(void);

// PUBLIC FUNCTIONS
void t_set_ctc_irq(t_f_ctc_isr f, uint32_t period_ms);
void t_isr_enable(void);
void t_isr_disable(void);
// TODO:
//uint8_t t_start_timeout(uint32_t time_ms);
//uint8_t t_is_timeout(uint32_t time_ms);
#endif /* TIMER_H_ */