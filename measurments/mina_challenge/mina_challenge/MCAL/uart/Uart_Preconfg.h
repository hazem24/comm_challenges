/*
 * uart_confg.h
 *
 *  Created on: Dec 22, 2019
 *      Author: LENOVO
 */

#ifndef _UART_CONFG_H_
#define _UART_CONFG_H_


/*- INCLUDES -----------------------------------------------*/
#include "../../common/compiler.h"
/*- CONSTANTS ----------------------------------------------*/
#define ENABLE  1U
#define DISABLE 0U

#define TX_AND_RX_FUNCTION   	  0U
#define TRASMITTER_FUNCTION 	  1U
#define RECIEVER_FUNCTION         2U

#define TRASMITTER_ENABLE DISABLE
#define RECIEVER_ENABLE   ENABLE

#if TRASMITTER_ENABLE
#define TX_INTERRUPT      ENABLE
#define UDRE_INTERRUPT    DISABLE
#endif

#if RECIEVER_ENABLE
#define RX_INTERRUPT      ENABLE
#endif


#if TX_INTERRUPT || UDRE_INTERRUPT
#define TX_ISR_CUSTOM     DISABLE /* Enable it if you want to declare it otherwise you can use callBack. */
#endif


#if RX_INTERRUPT
#define RX_ISR_CUSTOM     DISABLE /* Enable it if you want to declare it otherwise you can use callBack. */
#endif
/*- PRIMITIVE TYPES ----------------------------------------*/
/*- ENUMS --------------------------------------------------*/
/*- STRUCTS AND UNIONS -------------------------------------*/
/*- FUNCTION-LIKE MACROS -----------------------------------*/
/*- FUNCTION DECLARATIONS ----------------------------------*/



#endif /* UART_CONFG_H_ */
