/**
 * @file DIO.h
 * @brief This file is to use DIO module for any micro controller.
 *
 * @author Hazem Khaled
 * @date Nov 1, 2019
 *
 */
#ifndef DIO_H_
#define DIO_H_

#include "../../common/compiler.h"
#include "../../common/macros.h"
#include "../../micro_config.h"
#include "dio_config.h"

/*
 * values of pin.
 */
#define HIGH 1
#define LOW  0

/*
 * ERRORS Table.
 * This ENUM Contains all error that may be happen in DIO driver.
 */
enum
{
	PIN_NOT_DEFINED = 2,
	PIN_DEFINED_AS_INPUT,
	PIN_DEFINED_AS_OUTPUT,
	PIN_REDEFINED_NOT_ALLOWED,
	PIN_NUBMER_EXCEEDS_PINS_RANGE,
	PORT_NOT_DEFINED,
	PORT_NUBMER_EXCEEDS_PORTS_RANGE
};

/**
 * @brief this function used to INIT DIO Module.
 * @return void
 */
//void DIO_INIT(void);


/**
 * @brief this function used to set direction for specific pin.
 * @param <integer> pin Number.
 * @param <integer> input/output .. input = 0, output = 1.
 * @return true in case pin is set correctly,
 *	 	 	Error number in case it's not setted.
 */
uint8_t DIO_Set_Direction(uint8_t pinNumber, uint8_t direction);

/**
 * @brief this function used to set specific pin.
 * @param <integer> pin Number.
 * @param <integer> value .. HIGH = 1, LOW = 0.
 * @return true in case pin is set correctly,
 *	 	 	Error number in case it's not setted.
 */
uint8_t DIO_Set_Pin(uint8_t pinNumber, uint8_t value);

/**
 * @brief this function used to read specific port.
 * @param <integer> PORT Number.
 * @return value of this port.
 */
volatile uint8_t* DIO_Read_Port(uint8_t portNumber);
/**
 * @brief this function used to toggle pin.
 * @param <integer> pin Number.
 * @return true in success,
 *	 	 	Error number.
 */
void DIO_Toggle_Pin(uint8_t pinNumber);

/**
 * @brief this function used to toggle pin.
 * @param <integer> pin Number.
 * @return void
 */
uint8_t DIO_Set_Port(uint8_t portNumber, uint8_t value);
/**
 * @brief this function used to read specific pin.
 * @param <integer> pin Number.
 * @return HIGH  = 1 or LOW = 0,
 *	 	 	Error number in case it's not cleared.
 */
uint8_t DIO_Read_Pin(uint8_t pinNumber);

/**
 * @brief this function used to read specific port.
 * @param <integer> PORT Number.
 * @return value of this port.
 */
volatile uint8_t* DIO_Read_Port(uint8_t portNumber);
#endif /* DIO_H_ */
