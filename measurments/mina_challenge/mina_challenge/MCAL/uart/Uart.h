/*
 * uart.h
 *
 *  Created on: Dec 22, 2019
 *      Author: LENOVO
 */

#ifndef _UART_H_
#define _UART_H_

/*- INCLUDES -----------------------------------------------*/
#include "../../common/compiler.h"
#include "../../micro_registers.h"
#include "../../micro_config.h"
#include "Uart_Preconfg.h"

/*- CONSTANTS ----------------------------------------------*/
#if TRASMITTER_ENABLE == DISABLED && RECIEVER_ENABLE == DISABLED
#error "You disabled transmitter and receiver function of UART"
#endif
/*- Error Section */
#define UART_BASE_STATUS_CODE   (uint8_t)(50)
#define UART_SUCCESS            (uint8_t)UART_BASE_STATUS_CODE
#define UART_PARTIY_ERROR       (uint8_t)(UART_BASE_STATUS_CODE + 1)
#define UART_FRAME_ERROR        (uint8_t)(UART_BASE_STATUS_CODE + 2)
#define UART_OVERRUN_ERROR      (uint8_t)(UART_BASE_STATUS_CODE + 3)
#define UART_BASE_CODE_ERROR    (uint8_t)(UART_BASE_STATUS_CODE + 4)
#define UART_REINIT_NOT_ALLOWED (uint8_t)(UART_BASE_STATUS_CODE + 5)
#define UART_INVAILED_CONFG     (uint8_t)(UART_BASE_STATUS_CODE + 6)
#define UART_SETUP_FRAME_ERROR  (uint8_t)(UART_BASE_STATUS_CODE + 7)
#define UART_NO_INIT 			(uint8_t)(UART_BASE_STATUS_CODE + 7)
/*-End Error Section */

#define NORMAL_MODE        (uint8_t)1
#define DOUBLE_MODE        (uint8_t)2

#define INTERRUPT_RESPONSE (uint8_t)0
#define POLLING_RESPONSE   (uint8_t)1

#define ONE_STOP_BIT 	   (uint8_t)0
#define TWO_STOP_BIT       (uint8_t)1


#define NO_PARITY          (uint8_t)0
#define EVEN_PARITY        (uint8_t)1
#define ODD_PARITY         (uint8_t)2

#define _5_BIT_DATA  0U
#define _6_BIT_DATA  1U
#define _7_BIT_DATA  2U
#define _8_BIT_DATA  3U

/*- PRIMITIVE TYPES ----------------------------------------*/
typedef uint8_t UART_Status;


/*- STRUCTS AND UNIONS -------------------------------------*/
typedef void(*funcPtr)(void);
typedef void(*rxCallBack)(uint8_t);


typedef struct
{
	uint8_t data_size;/*5-6-7-8*/
	uint8_t stop_bit;/*1 or 2*/
	uint8_t parity_mode;/* inactive/odd/even */
}StrUartFrame_t;

typedef struct
{
	uint8_t mode;/* double/normal. */
	uint8_t response;/* interrupt/polling. */
	uint8_t operation;/*TX(only)/RX(only)/Both of them.*/
	uint32_t baudrate;
	StrUartFrame_t frame;
}StrUartConfig_t;




/*- FUNCTION-LIKE MACROS -----------------------------------*/
/*- FUNCTION DECLARATIONS ----------------------------------*/


/**
 * @brief  : Init UART Module.
 * @return : <uint8_t> CODE STATUS
 */
UART_Status UART_init(const StrUartConfig_t* strUartConfig_t);

/**
 * @brief  : UART Send Byte.
 * @param  : uint8_t au8Byte.
 * @return : <uint8_t> CODE STATUS
 */
UART_Status UART_sendByte(const uint8_t au8Byte);

/**
 * @brief  : UART Receive Byte.
 * @param  : uint8_t* pointer to data.
 * @return : <uint8_t> CODE STATUS.
 */
UART_Status UART_receiveByte(uint8_t* aptr_u8data);
/**
 * @brief  : DeInit UART Module.
 * @return : <uint8_t> CODE STATUS.
 */
UART_Status UART_deInit(void);

/**
 * @brief  : Set CallBack For Transmitter Interrupt.
 * @return : void
 */
void UART_callBackTx(funcPtr aptr_func);

/**
 * @brief  : Set CallBack For Receiver Interrupt.
 * @return : void
 */
void UART_callBackRx(const rxCallBack aptr_func);
#endif /* UART_H_ */
