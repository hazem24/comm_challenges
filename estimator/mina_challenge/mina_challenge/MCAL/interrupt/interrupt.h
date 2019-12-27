/*
 * interrupt.h
 *
 * Created: 27-Dec-19 10:00:20 PM
 *  Author: LENOVO
 */ 


#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "../../micro_registers.h"

typedef void (*ptr)(void);


void INT2_init();

void INT2_callBack(ptr callBack);


#endif /* INTERRUPT_H_ */