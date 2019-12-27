/*
 * uart.c
 *
 *  Created on: Dec 22, 2019
 *      Author: LENOVO
 */


/*- INCLUDES ----------------------------------------------*/
#include "Uart.h"
#include "../../common/macros.h"
/*- LOCAL MACROS ------------------------------------------*/
/*- LOCAL Dataypes ----------------------------------------*/
/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/

#if TX_INTERRUPT || RX_INTERRUPT || UDRE_INTERRUPT
/* Enable interrupt. */
static void interrupt_enable();
#endif

static UART_Status setupFrame(const StrUartFrame_t*);

static void setupBaudRate(const uint32_t, const uint8_t);

/*- GLOBAL STATIC VARIABLES -------------------------------*/
#if UDRE_INTERRUPT || TX_INTERRUPT
volatile static funcPtr g_funcPtr_TransmitterCallBack = NULL;/*Pointer to function used inside ISR of transmitter.*/
#if UDRE_INTERRUPT
volatile static uint8_t gu8_byte_was_sent = FALSE;/* because UDRE 1 at first time. */
#endif
#if TX_INTERRUPT
volatile static uint8_t gu8_byte_was_sent = TRUE;/* because UDRE 1 at first time. */
#endif
#endif

#if RX_INTERRUPT
volatile static rxCallBack g_funcPtr_RecieverCallBack    = NULL;/*Pointer to function used inside ISR of receiver.*/
#endif

volatile static uint8_t gu8_uart_init = FALSE;/* global variable to indicate if uart is init or not. */
volatile static uint8_t gu8_response;/*global variable to indicate type of response POLLING/INTERRUPT.*/
volatile static uint8_t data = 0;
/*- GLOBAL EXTERN VARIABLES -------------------------------*/
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/
/*- APIs IMPLEMENTATION -----------------------------------*/


/**
 * @brief  : Init UART Module.
 * @return : <uint8_t> CODE STATUS
 */
UART_Status UART_init(const StrUartConfig_t* ptr_StrUartConfig_t)
{
	/**
	 * 1 - check module not init. before.
	 	 * if => init => error
	 	 * else =>
	 	 	 *
	 */
	uint8_t au8_response = UART_SUCCESS;

	gu8_response = POLLING_RESPONSE;/* POILLING OR INTERRUPT BE DEFAULT POLLING.  */
	if(TRUE == gu8_uart_init)
	{
		au8_response = UART_REINIT_NOT_ALLOWED;
	}
	else
	{
		if (NULL != ptr_StrUartConfig_t)
		{
			/* safe to use this pointer. */
			
			SET_BIT_WITH_VALUE(UCSRA, U2X ,(ptr_StrUartConfig_t->mode - 1));
			au8_response = setupFrame(&ptr_StrUartConfig_t->frame);
			if (UART_SUCCESS == au8_response)
			{
				/*- setup BAUDRATE. -*/
				setupBaudRate(ptr_StrUartConfig_t->baudrate, ptr_StrUartConfig_t->mode);
				gu8_response = ptr_StrUartConfig_t->response;/* It used inside sendByte && receive byte. */
#if TX_INTERRUPT || RX_INTERRUPT || UDRE_INTERRUPT
			/* Enable interrupt. */
			interrupt_enable();
#endif
			/* active module. */
			/* Active channels TX/RX/BOTH. */
			//SET_BIT(UCSRB, RXEN);
			
			if (ptr_StrUartConfig_t->operation == TX_AND_RX_FUNCTION)
			{
				SET_BIT(UCSRB, TXEN);
				SET_BIT(UCSRB, RXEN);
			}
			else if(ptr_StrUartConfig_t->operation == TRASMITTER_FUNCTION)
			{
				SET_BIT(UCSRB, TXEN);
			}
			else if (ptr_StrUartConfig_t->operation == RECIEVER_FUNCTION)
			{
				SET_BIT(UCSRB, RXEN);
			}
			else
			{
				/* NOP. */
			}
			gu8_uart_init = TRUE;/* Active module. */
		}
		else
		{
			/* error invalid configuration. */
			au8_response = UART_INVAILED_CONFG;
		}
	}
	}
	return au8_response;
}


/**
 * @brief  : UART Send Byte.
 * @param  : uint8_t au8Byte.
 * @return : <uint8_t> CODE STATUS
 */
UART_Status UART_sendByte(const uint8_t au8Byte)
{
	uint8_t au8_response = UART_SUCCESS;
	if (FALSE == gu8_uart_init)
	{
		au8_response = UART_NO_INIT;
	}
	else
	{
		if(INTERRUPT_RESPONSE == gu8_response)
		{
			/* logic of interrupt. */
#if !TX_ISR_CUSTOM && (UDRE_INTERRUPT || TX_INTERRUPT)
/* Not Custom Interrupt. */
	if(TRUE == gu8_byte_was_sent)/* Locker. */
	{
		/* Put Another Byte. */
		gu8_byte_was_sent = FALSE;
		UDR = au8Byte;
	}
/* user can control it via callBackFunction. */
#endif
		}
		else
		{
			/*- POLLING. -*/
#if TX_INTERRUPT
	while(BIT_IS_CLEAR(UCSRA, TXC));
	UDR = au8Byte;
	SET_BIT(UCSRA, TXC);
#endif

#if URDE_INTERRUPT
	while(BIT_IS_CLEAR(UCSRA, UDRE));
	UDR = au8Byte;
	SET_BIT(UCSRA, UDRE);
#endif
		}
	}
	return au8_response;
}

/**
 * @brief  : UART Receive Byte.
 * @param  : uint8_t* pointer to data.
 * @return : <uint8_t> CODE STATUS.
 */
UART_Status UART_receiveByte(uint8_t* aptr_u8data)
{
	uint8_t au8_response = UART_SUCCESS;
	if (FALSE == gu8_uart_init)
	{
		au8_response = UART_NO_INIT;
	}
	else
	{
		if(INTERRUPT_RESPONSE == gu8_response)
		{
			/* logic of interrupt. */
#if !RX_ISR_CUSTOM
	/* Not Custom Interrupt. */
	/* user can control it via callBackFunction. */
#endif
		}
		else
		{
			/*- POLLING. -*/
			while(BIT_IS_CLEAR(UCSRA, RXC));
			*aptr_u8data = UDR;
			SET_BIT(UCSRA, RXC);
		}
	}
	return au8_response;
}
/**
 * @brief  : DeInit UART Module.
 * @return : <uint8_t> CODE STATUS.
 */
UART_Status UART_deInit(void)
{
	CLEAR_BIT(UCSRC, URSEL);
	UBRRH = 0;
	UBRRL = 0;
	//CLEAR_BIT(); clear TXEN,RXEN
	return UART_SUCCESS;
}


#if !TX_ISR_CUSTOM && (TX_INTERRUPT || UDRE_INTERRUPT)
/**
 * @brief  : Set CallBack For Transmitter Interrupt.
 * @return : void
 */

void UART_callBackTx(funcPtr aptr_func)
{
	g_funcPtr_TransmitterCallBack = aptr_func;
}
#endif

#if RX_INTERRUPT && !RX_ISR_CUSTOM
/**
 * @brief  : Set CallBack For Receiver Interrupt.
 * @return : void
 */
void UART_callBackRx(const rxCallBack aptr_func)
{
	/* TODO: Check for pointer NULL. */
	g_funcPtr_RecieverCallBack = aptr_func;
}
#endif
/**
 * @brief  : setupFrame
 * @return : void
 */
static UART_Status setupFrame(const StrUartFrame_t* aptr_strFrame)
{
	uint8_t au8_response = UART_SUCCESS;
	if (NULL != aptr_strFrame)
	{
		//SET_BIT(UCSRC, URSEL);
		UCSRC |= (aptr_strFrame->data_size << 1);
		UCSRC |= (aptr_strFrame->stop_bit << USBS);
		UCSRC |= (aptr_strFrame->parity_mode << UPM0);
		CLEAR_BIT(UCSRC, URSEL);
	}
	else
	{
		au8_response = UART_SETUP_FRAME_ERROR;
	}

	return au8_response;
}

/**
 * @brief  : setupBaudRate.
 * @return : void.
 */
static void setupBaudRate(const uint32_t baudRate, const uint8_t mode)
{
	uint8_t auMode = (mode * 8) + (8);
	uint16_t au16_UDR_value = 0;
	au16_UDR_value = (uint16_t)(((uint64_t)F_CPU)/(auMode * baudRate)) - 1U;
	CLEAR_BIT(UCSRC, URSEL);
	UBRRH = (uint8_t)au16_UDR_value >> 8;
	UBRRL = (uint8_t)au16_UDR_value;
}
/**
 * @brief  : Enable Interrupt.
 * @return : void
 */
#if TX_INTERRUPT || RX_INTERRUPT || UDRE_INTERRUPT
/* Enable interrupt. */
static void interrupt_enable()
{
#if RX_INTERRUPT
	/* ENABLE RX_INTERRUPT. */
	SET_BIT(UCSRB, RXCIE);
#endif
#if TX_INTERRUPT
	/* ENABLE TXIE. */
	SET_BIT(UCSRB, TXCIE);
#endif
#if UDRE_INTERRUPT
	/* ENABLE UDRE. */
	SET_BIT(UCSRB, UDRE);
#endif
sei();/* global I-BIT. */
}
#endif

#ifdef TX_ISR_CUSTOM
#if TX_ISR_CUSTOM == DISABLE
#if TX_INTERRUPT
ISR(USART_TXC_vect)
{
	if(NULL != g_funcPtr_TransmitterCallBack)
	{
		g_funcPtr_TransmitterCallBack();/* call callBack Function. */
	}
	gu8_byte_was_sent = TRUE;/* Flag to indicate buffer is ready for new byte. */
}
#elif UDRE_INTERRUPT
ISR(USART_UDRE_vect)
{
	if(NULL != g_funcPtr_TransmitterCallBack)
	{
		g_funcPtr_TransmitterCallBack();/* call callBack Function. */
	}
	gu8_byte_was_sent = TRUE;/* Flag to indicate buffer is ready for new byte. */
}
#else
#error "Error wrong configuration in interrupt"
#endif
#endif
#endif

#if RX_ISR_CUSTOM == DISABLE && RX_INTERRUPT
ISR(USART_RXC_vect)
{
	if(NULL != g_funcPtr_RecieverCallBack)
	{
		g_funcPtr_RecieverCallBack(UDR);/* call callBack Function. */
	}
}

#endif
