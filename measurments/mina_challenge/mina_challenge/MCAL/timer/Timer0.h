/*
 * MACROS.h
 *
 *  Created on: Dec 2, 2017
 *      Author: morphios
 */

#ifndef __TIMER0__
#define __TIMER0__

/**include register file */
#include "../../micro_registers.h"

extern void timer0_init(void);

extern void start_timer(void);

extern void reset_timer(uint16_t* ovf);
extern uint8_t get_ticks(void);

#endif /* MACROS_H_ */
