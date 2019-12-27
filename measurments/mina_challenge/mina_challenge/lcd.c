/*
 * lcd.c
 *
 *  Created on: Apr 29, 2019
 *      Author: LENOVO
 */


#include "lcd.h"



static void LCD_pinEPulse();

/*
 * Init LCD.
 */
void LCD_init()
{
	/*
	 * init pins.
	 * RS = 0 To send CMD for initialization LCD.
	 * RW = 0 To Write CMD To LCD.
	 * E = high to low pulses.
	 * CMD .. for 8-bit Mode.
	 	 * 0x38 for 5 x 8 Matrix (2 Line .. 8 bit).
	 	 * 0x0E display on, Cursor on.
	 	 * 0x01 Clear display screen.
	 */
	#if LCD_MODE == 8
		LCD_DDR = 0XFF;//Output PINS.
	#else
		#if UPPER_PINS == True
			LCD_DDR |= 0x0F;
		#else
			LCD_DDR |= 0xF0;
		#endif
	#endif
	SET_BIT(LCD_CONFIG_DDR, RS);//output.
	SET_BIT(LCD_CONFIG_DDR, RW);//output.
	SET_BIT(LCD_CONFIG_DDR, E);//output.
	/*
	 * Init CMD.
	 */
	#if LCD_MODE == 8
		LCD_sendCommand(0x38);
	#else
		LCD_sendCommand(0x02);
		LCD_sendCommand(0x28);
	#endif
	LCD_sendCommand(0x0E);
	LCD_sendCommand(0x01);
}



/*
 * send command to LCD.
 */
void LCD_sendCommand(const uint8_t cmd)
{
	CLEAR_BIT(LCD_CONFIG_PORT, RS);
	CLEAR_BIT(LCD_CONFIG_PORT ,RW);
	/*
	 * After each CMD I must wait about (100 Ms).
	 */
	//Changed in 4-bit.
	#if LCD_MODE == 8
		LCD_PORT = cmd;
		LCD_pinEPulse();
	#else
		#if UPPER_PINS == True

			SET_BIT(LCD_CONFIG_PORT, E);
			_delay_ms(2);
			LCD_PORT = ( LCD_PORT & 0xF0 ) | (( cmd >> 4 ));
			CLEAR_BIT(LCD_CONFIG_PORT, E);
			_delay_ms(2);
			SET_BIT(LCD_CONFIG_PORT, E);
			_delay_ms(2);
			LCD_PORT = ( LCD_PORT & 0xF0 ) | ( ( cmd & 0x0F ));//Passed.
			CLEAR_BIT(LCD_CONFIG_PORT, E);
			_delay_ms(2);

		#else

			SET_BIT(LCD_CONFIG_PORT, E);
			_delay_ms(2);
			//( LCD_PORT & 0x0F ) ﬂ‰  ‰«”ÌÂ« Œ«·’ „Õÿ Â«‘ ›Ì «·Õ”»«‰
			LCD_PORT = ( LCD_PORT & 0x0F )|( ( cmd & 0xF0 ));//Passed.
			CLEAR_BIT(LCD_CONFIG_PORT, E);
			_delay_ms(2);
			SET_BIT(LCD_CONFIG_PORT, E);
			_delay_ms(2);
			LCD_PORT = ( LCD_PORT & 0x0F ) | (( cmd << 4 ));
			CLEAR_BIT(LCD_CONFIG_PORT, E);
			_delay_ms(2);
		#endif
	#endif
	/*
	 * High to low pulse to on PIN "E".
	 */
}

/*
 * sendingData.
 */
void LCD_displayCharacter(const uint8_t data)
{
	SET_BIT(LCD_CONFIG_PORT, RS);
	CLEAR_BIT(LCD_CONFIG_PORT, RW);
	//Changed in 4-bit.
	#if LCD_MODE == 8
		LCD_PORT = data;
		LCD_pinEPulse();
	#else
		#if UPPER_PINS == True
			SET_BIT(LCD_CONFIG_PORT, E);
			_delay_ms(2);
			LCD_PORT = ( LCD_PORT & 0xF0 ) | (( data >> 4 ));
			CLEAR_BIT(LCD_CONFIG_PORT, E);
			_delay_ms(2);
			SET_BIT(LCD_CONFIG_PORT, E);
			_delay_ms(2);
			LCD_PORT = ( LCD_PORT & 0xF0 ) | ( ( data & 0x0F ));//Passed.
			CLEAR_BIT(LCD_CONFIG_PORT, E);
			_delay_ms(2);

		#else
			SET_BIT(LCD_CONFIG_PORT, E);
			_delay_ms(2);
			LCD_PORT = ( LCD_PORT & 0x0F ) | ( ( data & 0xF0 ));//Passed.
			CLEAR_BIT(LCD_CONFIG_PORT, E);
			_delay_ms(2);
			SET_BIT(LCD_CONFIG_PORT, E);
			_delay_ms(2);
			LCD_PORT = ( LCD_PORT & 0x0F ) | (( data << 4 ));
			CLEAR_BIT(LCD_CONFIG_PORT, E);
			_delay_ms(2);

		#endif
	#endif
}


void LCD_displayString(uint8_t* string)
{
	uint8_t i = 0;

	while( string[i] != '\0' )
	{
		LCD_displayCharacter( string[i] );
		i++;
	}

}


void LCD_goToRowColumn(uint8_t row, uint8_t col)
{
	uint8_t address;
	switch(row)
	{
		case 1:
			address = 0x80 + col;
			//OR LCD_sendCommand(0x80).
			break;
		case 2:
			address = 0xC0 + col;
			//OR LCD_sendCommand(0xC0);
			break;
		default:
			break;
			//Nothing.
	}
	/*Another solution.
	 * uint8 i;
	for(i = 0;i<col;i++)
	{
		LCD_sendCommand(0x14);//right shift cursor to the wanted col.
	}*/

	/*
	 * Another solution
	 * cmd = address | 0b100000000
	 */
	LCD_sendCommand(address | 0b100000000);

}

/*
 * Put string in ROW-COL.
 */
void LCD_stringRowCol(uint8_t* string, uint8_t row, uint8_t col)
{
	LCD_goToRowColumn(row, col);
	LCD_displayString(string);

}


/*
 * Clear Screen.
 */

void LCD_clearScreen()
{
	LCD_sendCommand(0x01);
}
/*
 * integerToString.
 */
void LCD_integerToString(uint32_t value)
{
	uint32_t buffer[20];
	itoa(value, buffer , 10);
	LCD_displayString(buffer);
}
/*
 * LCD_pinEPulse.
 */
static void LCD_pinEPulse()
{
	//High Pulse.
	TOGGLE_BIT(LCD_CONFIG_PORT,E);
	_delay_ms(2);
	//Low Pulse.
	TOGGLE_BIT(LCD_CONFIG_PORT,E);
	_delay_ms(2);
}
