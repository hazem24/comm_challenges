/*
 * meaurmentECU.h
 *
 * Created: 27-Dec-19 9:48:07 PM
 *  Author: LENOVO
 */ 


#ifndef MEAURMENTECU_H_
#define MEAURMENTECU_H_

#include "../common/compiler.h"
#include "../MCAL/dio/dio.h"
#include "../MCAL/uart/Uart.h"
#include <util/delay.h>
#include "../lcd.h"
#include "../MCAL/spi/SPI.h"
#include "../MCAL/spi/SPI_Pb.h"
#include "../HAL/switch/switch.h"
#include "../MCAL/timer/Timer0.h"


#define SEND_DATA_TO_PC_BUTTON   DIO_PIN11
#define TICK_TIME                128U


static uint16_t  g_u16Timer_ovf           = 0;
static uint32_t  g_u32Distance            = 0; 
static uint8_t   g_u8CarSpeed             = 0;
static uint8_t   g_u8PrevCarSpeed         = 0;
static uint8_t   g_u8Timer_started         = 0;

/**
 * @brief init measurment unit.
 * @return void.
 */
void MEASURMENT_init(void)
{
    StrUartConfig_t uart_cfg  = { NORMAL_MODE, INTERRUPT_RESPONSE,  , 9600UL,
		{_8_BIT_DATA, ONE_STOP_BIT, NO_PARITY} };
    /* Init UART. */
    UART_init(&uart_cfg);
    /* Init Timer. */
    timer0_init();
    timer0_start();
    /* SPI init. */
    SPI_Init(&SPI_RX);
    /* init Interrupt. */
    INT2_init();
    INT2_callBack(sendToPc);
    /* Control Speed Buttons. */
    DIO_Set_Direction(SEND_DATA_TO_PC_BUTTON, INPUT);
}

/**
 * @brief 
 * reset_timer()
 */
void MEASURMENT_run(void)
{
    /**
     * main Logic.
     */
    g_u8PrevCarSpeed = g_u8CarSpeed;
    SPI_Receive_Byte(&g_u8CarSpeed);

    if(g_u8PrevCarSpeed != g_u8CarSpeed)
    {
        reset_timer(&g_u16Timer_ovf);
    }
    else
    {
        /* NOP. */
    }
}
/**
 * @brief send data to pc.
 * 
 */
static void sendToPc(void)
{
    g_u32Distance = g_u32Distance + ( (get_ticks() * TICK_TIME) + g_u16Timer_ovf * ( TICK_TIME ) ) * g_u8CarSpeed;
    UART_sendByte((g_u32Distance/10));
    g_u32Distance = 0;
    reset_timer(&g_u16_Timer_ovf);
}


ISR(TIMER0_OVF_vect)
{
    g_u16Timer_ovf++;
}
#endif /* MEAURMENTECU_H_ */