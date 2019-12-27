/*
 * uart.h
 *
 *  Created on: Jun 9, 2019
 *      Author: LENOVO
 */

#ifndef UART_H_
#define UART_H_


#include "common/compiler.h"
#include "micro_config.h"
#include "common/macros.h"
#include "micro_registers.h"
#include <string.h>
#include <stddef.h>

#define UART_DDR DDRD
#define UART_TRANSMITTER_PIN PD1
#define UART_RECEIVER_PIN    PD0


/*
 * BAUD_RATE
 */
#define BAUD_RATE 9600

/*
 * Prescaler .. can be 8 OR 16
 * In case of 16 .. ASYNC DOUBLE MODE MUST BE ACTIVE .. (U2X = 1).
 */
#define PRESCALER 16
/*
 * PRESCALER .. 8 OR 16.
 */
#define UBRR_VALUE (((uint64_t)F_CPU/(((uint64_t)BAUD_RATE * PRESCALER))) - 1)
/*
 * UART init.
 */
void POL_UART_init();
/*
 * UART sending byte.
 */
void POL_UART_sendByte(uint8_t byte);
/*
 * UART receive byte.
 */
uint8_t POL_UART_receiveByte();

/*
 * UART sendString.
 */
void POL_UART_sendString();

/*
 * UART receive string.
 */
void POL_UART_receiveString(uint8_t*);
#endif /* UART_H_ */
