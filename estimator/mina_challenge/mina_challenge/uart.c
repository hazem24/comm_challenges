/*
 * uart.c
 *
 *  Created on: Jun 9, 2019
 *      Author: LENOVO
 */


#include "uart.h"

/*
 * UART init.
 */
void POL_UART_init()
{
	/*
	 * UCSRC.
	 	 * URSEL -> must be one when writing the UCSRC. --init--
	 	 * UMSEL -> USART mode select .. 0 -> ASYNC, 1 -> SYNC.--init--.
	 	 * UPM5:4   -> Parity Mode.--init--
	 	 * USBS -> Stop bit select.--init--
	 	 * UCSZ2:1 -> Helps choosing the number of data bits in your frame .. standard 8-bit 3 (Initial Value is 3 by default).--init--.
	 * UBRRL - UBRRH -> set baud rate.--init--.
	 	 * URSEL -> must be set to zero when writing UBRRH.
	 */

	SET_BIT(UCSRC, URSEL);
	CLEAR_BIT(UCSRC, UMSEL);//ASYNC Mode.

	/*
	 * Set BaudRate.
	 */
	CLEAR_BIT(UCSRC, URSEL);
	UBRRH = UBRR_VALUE >> 8;
	UBRRL = UBRR_VALUE;
}

/*
 * UART sending byte.
 */
void POL_UART_sendByte(const uint8_t byte)
{
	/*
		 * UDR.
		 	 * Data buffer register in transmitting write on it .. in receiving write on it also.--sendByte--.
		 * UCSRA.
		 	 * RXC  -> receive complete flag .. set by  CPU when there are unread data in the receive buffer.
		 	 * TXC  -> transmit complete flag .. set by CPU .. when there's no new data present in the transmit buffer.--sendByte--.
		 	 * UDRE -> flag indicate if the transmit buffer is ready to receive new data .. if is one the buffer empty.--sendByte--.
		 	 * FE   -> Frame error .. always set this bit to zero when writing to UCSRA.
		 	 * DOE  -> Data overrun error .. always set this bit to zero when writing to UCSRA.
		 	 * PE   -> Parity error .. always set this bit to zero when writing to UCSRA.
		 	 * U2X  -> Double transmission speed .. writing this bit to reduce the divisor of the baud rate divider from 16 to 8.
		 * UCSRB.
		 	 * RXCIE -> RX complete interrupt enable.
		 	 * TXCIE -> TX complete interrupt enable.
		 	 * RXEN  -> Receiver Enable.
		 	 * TXEN  -> Transmitter Enable. --sendByte--.
	*/
	SET_BIT(UCSRB, TXEN);
	UDR = byte;
	while( BIT_IS_CLEAR(UCSRA, TXC) );//Polling Operation.
	SET_BIT(UCSRA, TXC);//Reset TXC bit.
}

/*
 * UART receive byte.
 */
uint8_t POL_UART_receiveByte()
{
	/*
		* UDR.
			 * Data buffer register in transmitting write on it .. in receiving write on it also.--sendByte--, --receiveByte--.
		* UCSRA.
			 * RXC  -> receive complete flag .. set by  CPU when there are unread data in the receive buffer.--receiveByte--.
			 * TXC  -> transmit complete flag .. set by CPU .. when there's no new data present in the transmit buffer.--sendByte--.
			 * UDRE -> flag indicate if the transmit buffer is ready to receive new data .. if is one the buffer empty.--sendByte--.
			 * FE   -> Frame error .. always set this bit to zero when writing to UCSRA.
			 * DOE  -> Data overrun error .. always set this bit to zero when writing to UCSRA.
			 * PE   -> Parity error .. always set this bit to zero when writing to UCSRA.
			 * U2X  -> Double transmission speed .. writing this bit to reduce the divisor of the baud rate divider from 16 to 8.
		* UCSRB.
			 * RXCIE -> RX complete interrupt enable.
			 * TXCIE -> TX complete interrupt enable.
			 * RXEN  -> Receiver Enable.--receiveByte--.
			 * TXEN  -> Transmitter Enable. --sendByte--.
	*/
	SET_BIT(UCSRB, RXEN);
	while( BIT_IS_CLEAR(UCSRA , RXC) );
	//RXC Cleared by CPU when no data in data-in buffer.
	return UDR;
}

/*
 * UART sendString.
 */
void POL_UART_sendString(uint8_t* string)
{
	/*
	 * To send string and understandable by the receiver .. first byte will have the number of bytes coming from transmitter.
	 */
	/*
	 * Then sending byte byte to receiver.
	 */
	uint8_t i = 0;
	while( *(string + i) != '\0' )
	{
		POL_UART_sendByte( *(string + i) );
		i++;
	}
}

/*
 * UART receive string.
 */
void POL_UART_receiveString(uint8_t* string)
{
	uint8_t i = 0;
	/*
	 * First coming byte is the length of string.
	 */

	/*
	 * Loop to get all string.
	 */

	while( TRUE )
	{
		string[i] = POL_UART_receiveByte();
		if ( string[i] == '#' ) break;
		i++;
	}

	//Put Null Operator.
	*(string + i) = '\0';
}
