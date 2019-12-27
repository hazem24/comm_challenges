/**
 * @file DIO_Config.h
 * @brief This file is to used to configure DIO module for target.
 *
 * @author Hazem Khaled
 * @date Nov 1, 2019
 *
 */

#ifndef DIO_CONFIG_H_
#define DIO_CONFIG_H_

#include "../../micro_registers.h"
/*
 * Direction of DIO_PINs.
 */
#define INPUT  0
#define OUTPUT 1

/*
 * Number of DIO_PINs.
 * set this define to number of DIO_PINs in target.
 */
#define DIO_CONFIG_PINS_NUMBERS 32


#define DIO_PORTS_PER_PIN 8
/*
 * Number of Ports.
 * set this define to number of ports in target.
 */
#define PORTS_NUMBER 4


/*
 * Serialize DIO_PINs.
 */
enum
{
	DIO_PIN0,
	DIO_PIN1,
	DIO_PIN2,
	DIO_PIN3,
	DIO_PIN4,
	DIO_PIN5,
	DIO_PIN6,
	DIO_PIN7,
	DIO_PIN8,
	DIO_PIN9,
	DIO_PIN10,
	DIO_PIN11,
	DIO_PIN12,
	DIO_PIN13,
	DIO_PIN14,
	DIO_PIN15,
	DIO_PIN16,
	DIO_PIN17,
	DIO_PIN18,
	DIO_PIN19,
	DIO_PIN20,
	DIO_PIN21,
	DIO_PIN22,
	DIO_PIN23,
	DIO_PIN24,
	DIO_PIN25,
	DIO_PIN26,
	DIO_PIN27,
	DIO_PIN28,
	DIO_PIN29,
	DIO_PIN30,
	DIO_PIN31
};


#endif /* DIO_CONFIG_H_ */
