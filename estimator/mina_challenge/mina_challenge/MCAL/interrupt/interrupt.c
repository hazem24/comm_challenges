/*
 * interrupt.c
 *
 * Created: 27-Dec-19 10:00:05 PM
 *  Author: LENOVO
 */ 


#include "interrupt.h"
#include "../../common/compiler.h"

static ptr g_callBack = NULL;
void INT2_init(void)
{
	cli();
	DDRB  &= (~(1<<2));
	GICR  |= (1<<INT2);
	MCUCSR|=(1<<ISC2);
	sei();
}

void INT2_callBack(ptr callBack)
{
    g_callBack = callBack;
}



ISR(INT2_vect)
{
    if (NULL != g_callBack)
    {
        g_callBack();
    }
}