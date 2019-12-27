


#include "Timer0.h"


void timer0_init(void)
{
	TCNT0 = 0; //timer initial value
	TIMSK |= (1<<TOIE0); //enable overflow interrupt

	TCCR0 = (1<<FOC0) ;
}
void start_timer(void)
{
	/* start timer with clock = F_CPU/1024 CS00=1 CS01=0 CS02=1 */
	TCCR0 |=  (1<<CS02) ;
	TCCR0 |=  (1<<CS00);
}

uint8_t get_ticks(void)
{
	/* start timer with clock = F_CPU/1024 CS00=1 CS01=0 CS02=1 */
	return TCNT0;
}
void reset_timer(uint16_t* ovf)
{
	TCNT0 = 0; //timer initial value
	*ovf  = 0;
}




