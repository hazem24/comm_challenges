/*
 * dio.c
 *
 *  Created on: Nov 1, 2019
 *      Author: LENOVO
 */

#include "dio.h"

/**
 * @brief this function used to calculate port number.
 * @param <integer> pin Number.
 * @return PORT number.
 */
static uint8_t DIO_PortNumber(uint8_t pinNumber);


/**
 * @brief this function used to get value of real pin in port.
 * @param  <integer> pin Number.
 * @return <integer>
 */
static uint8_t realPinNumber(uint8_t pinNumber);

/**
 * @brief this function used to check pin status against error.
 * @param  <integer> pin Number.
 * @return <integer>
 */
static uint8_t DIO_PinStatus(uint8_t pinNumber);


/**
 * @brief this array used to track pins (Status for pins =>
 * 											2 => output.
 * 											1 => input.
 * @index of this array indicate pin number.
 * @variable track_pins
 */
static uint8_t track_pins[DIO_CONFIG_PINS_NUMBERS];

/**
 * @brief this array used to store DDR ports address.
 * @index of this array indicate port number.
 * @variable DDR_Address
 */
volatile uint8_t* DDR_Address[ PORTS_NUMBER ] = {&DDRA, &DDRB, &DDRC, &DDRD};// This Should be at DIO_Config.h.

/**
 * @brief this array used to store PORT address.
 * @index of this array indicate port number.
 * @variable PORT_Address
 */
volatile uint8_t* PORT_Address[ PORTS_NUMBER ] ={&PORTA, &PORTB, &PORTC, &PORTD};//This Should be at DIO_Config.h.

/**
 * @brief this array used to store PIN address.
 * @index of this array indicate port number.
 * @variable PIN_Address
 */

volatile uint8_t* PIN_Address[ PORTS_NUMBER ] = {&PINA, &PINB, &PINC, &PIND};//// This Should be at DIO_Config.h.

/**
 * @brief this array used to track ports (Status for ports =>
 * 											2 => output.
 * 											1 => input.
 * @index of this array indicate port number.
 * @variable track_ports
 * Not used In version 1.
 */
//static uint8_t track_ports[PORTS_NUMBER];



/**
 * @brief this function used to INIT DIO Module.
 * @return void
 */
/*void DIO_INIT(0)
{
	//DDR_Address[ PORTS_NUMBER ];
}*/

/**
 * @brief this function used to set direction for specific pin.
 * @param <integer> pin Number.
 * @param <integer> input/output .. output = 0, input = 1.
 * @return TRUE in case pin is set correctly,
 *	 	 	Error number in case it's not setted.
 */
//DIO_Set_Direction(28, OUTPUT)
uint8_t DIO_Set_Direction(uint8_t pinNumber, uint8_t direction)
{
	/*
	 * check if pinNumber less than or equal total number of pins.
	 	 * if TRUE
	 	 	 * Check Pin in track_pins array.
	 	 	 	 * if pin not found.
	 	 	 	 	 * set it and set it in track_pins.
	 	 	 	 	 * return TRUE.
	 	 	 	 * if founded.
	 	 	 	 	 * return error PIN_REDEFINED_NOT_ALLOWED
	 	 * if FALSE
	 	 	 * return ERROR PIN_NUBMER_EXCEEDS_PINS_RANGE
	 */
	uint8_t return_value = DIO_PinStatus(pinNumber);
	if (return_value == TRUE)
	{
		uint8_t pin = pinNumber;
		//Meaning pin not defined before.
		track_pins[pin] = direction + 1;
		//get real number of pin.
		pinNumber = realPinNumber(pinNumber);
		SET_BIT(*(DDR_Address[DIO_PortNumber(pin)]), pinNumber);
	}
	return return_value;
}

/**
 * @brief this function used to set specific pin.
 * @param <integer> pin Number.
 * @param <integer> value .. HIGH = 1, LOW = 0.
 * @return TRUE in case pin is set correctly,
 *	 	 	Error number in case it's not set.
 */

// i assumed value is HIGH or LOW.
uint8_t DIO_Set_Pin(uint8_t pinNumber, uint8_t value)
{
	uint8_t return_value = DIO_PinStatus(pinNumber);
	uint8_t pin = pinNumber;
	if ( return_value == PIN_REDEFINED_NOT_ALLOWED )//Meaning bit is founded.
	{
		/*
		 * check if pin is output.
		 */
		if ( track_pins[ pinNumber ]  == 2 )
		{
			// i assumed value is HIGH or LOW.
			pinNumber = realPinNumber(pinNumber);
			switch(value)
			{
				case HIGH:
					//set pin.
					SET_BIT(*(PORT_Address[DIO_PortNumber(pin)]), pinNumber);
					break;
				case LOW :
					//clear pin
					CLEAR_BIT(*(PORT_Address[DIO_PortNumber(pin)]), pinNumber);
					break;
			}
		}else
		{
			return_value = PIN_DEFINED_AS_INPUT;
		}
	}
	return return_value;
}


/**
 * @brief this function used to read specific pin.
 * @param <integer> pin Number.
 * @return HIGH  = 1 or LOW = 0,
 *	 	 	Error number in case it's not cleared.
 */
uint8_t DIO_Read_Pin(uint8_t pinNumber)
{
	uint8_t return_value = DIO_PinStatus(pinNumber);
	uint8_t pin = pinNumber;
	if ( return_value == PIN_REDEFINED_NOT_ALLOWED )//Meaning bit is founded.
	{
		/*
		 * check if pin is input.
		 */
		if ( track_pins[ pinNumber ]  == 1 )
		{
			pinNumber = realPinNumber(pinNumber);
			return_value = READ_BIT(*(PIN_Address[DIO_PortNumber(pin)]), pinNumber);
		}else
		{
			return_value = PIN_DEFINED_AS_OUTPUT;
		}
	}
	return return_value;

}


/**
 * @brief this function used to set value for specific port.
 * @param <integer> PORT Number .. started from 1.
 * @return TRUE in case if it's set correctly,
 *	 	 Error number in case it's not set.
 */
uint8_t DIO_Set_Port(uint8_t portNumber, uint8_t value)
{
	/*
	 * check if port in target.
	 	 * TRUE
	 	 	 * check if port in track ports.
	 	 	 	 * TRUE
	 	 	 	    * set value return TRUE.
	 	 	 	 * FALSE
	 	 	 	 	* return PORT_NOT_DEFINED.
	 * else
	 	 *  return PORT_NUBMER_EXCEEDS_PORTS_RANGE.
	 */
	uint8_t return_result;
	if (portNumber <= PORTS_NUMBER)
	{
		*(PORT_Address[ portNumber ]) = value;
		return_result = TRUE;
	}else
	{
		return_result = PORT_NUBMER_EXCEEDS_PORTS_RANGE;
	}
	return return_result;
}

/**
 * @brief this function used to read specific port.
 * @param <integer> PORT Number.
 * @return value of this port.
 */
volatile uint8_t* DIO_Read_Port(uint8_t portNumber)
{
	return (PIN_Address[portNumber]);
}
/**
 * @brief this function used to toggle pin.
 * @param <integer> pin Number.
 * @return void.
 * @note need some modification to handle error. 
 */
void DIO_Toggle_Pin(uint8_t pinNumber)
{
	uint8_t copyPin = realPinNumber(pinNumber);
	TOGGLE_BIT(*(PORT_Address[DIO_PortNumber(pinNumber)]), copyPin);
}

/**
 * @brief this function used to calculate port number.
 * @param  <integer> pin Number.
 * @return <integer> PORT number.
 */
static uint8_t DIO_PortNumber(uint8_t pinNumber)
{
	return (pinNumber/DIO_PORTS_PER_PIN);
}

/**
 * @brief this function used to check pin status against error.
 * @param  <integer> pin Number.
 * @return <integer>
 * @note this function will need more modification to handle more error cases.
 */
static uint8_t DIO_PinStatus(uint8_t pinNumber)
{
	uint8_t return_value;
	if (pinNumber <= (DIO_CONFIG_PINS_NUMBERS - 1))
	{
		if(track_pins[pinNumber] == FALSE)
		{
			//Meaning pin not defined before.
			return_value = TRUE;
		}else
		{
			//Pin defined before.
			return_value = PIN_REDEFINED_NOT_ALLOWED;
		}
	}else
	{
		return_value = PIN_NUBMER_EXCEEDS_PINS_RANGE;
	}
	return return_value;
}

/**
 * @brief this function used to get value of real pin in port.
 * @param  <integer> pin Number.
 * @return <integer>
 * 3amalt el function de w ana msd3333.
 */
static uint8_t realPinNumber(uint8_t pinNumber)
{
	if ( pinNumber > 23 ) pinNumber = pinNumber - 23;
	else if ( pinNumber > 15 ) pinNumber = pinNumber - 15;
	else if ( pinNumber > 7  ) pinNumber = pinNumber - 7;
	return pinNumber;
}
