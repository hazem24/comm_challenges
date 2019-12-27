/*
 * estimator.c
 *
 * Created: 27-Dec-19 7:57:30 PM
 *  Author: LENOVO
 */ 

#include "estimator.h"


/* Global variable to indicate that data received from PC. */
volatile static uint8_t  g_u8PcFlag = 0;

/* Car Speed. */
volatile static uint8_t  g_u8CarSpeed = 0;

/**
 * @brief rx_callBack.
 * @return void.
 */
static void rx_callBack(uint8_t speed);

/**
 * @brief 
 * @return void.
 */
void ESTMATIOR_init(void)
{
    StrUartConfig_t uart_cfg  = { NORMAL_MODE, INTERRUPT_RESPONSE, RECIEVER_FUNCTION, 9600UL,
			{_8_BIT_DATA, ONE_STOP_BIT, NO_PARITY} };
    /* Init UART. */
    UART_init(&uart_cfg);
    /* Call Back. */
    UART_callBackRx(rx_callBack);/* Consumer. */
    /* SPI init. */
    SPI_Init(&SPI_TX);

    /* Control Speed Buttons. */
    DIO_Set_Direction(SPEED_DECREASE_BUTTON, INPUT);
    DIO_Set_Direction(SPEED_INCREASE_BUTTON, INPUT);

    g_u8PcFlag = 0;
}


/**
 * @brief Estimator App.
 * @return void.
 */
void ESTIMATOR_run(void)
{
    /* Speed Received from PC. */
    if( TRUE == g_u8PcFlag )
    {
        /**
         * Main Logic. 
         * Reading Button 1 => TRUE => Speed Increase.
         * Reading Button 2 => TRUE => Speed Increase.
         * Reading Button 1 && Reading Button 2 => No variation in speed.
         */
        if( SWITCH_read(SPEED_INCREASE_BUTTON) && SWITCH_read(SPEED_DECREASE_BUTTON) )
        {
            /* NOP in this case => Speed Constant. */
        }
        else if (TRUE == SWITCH_read(SPEED_INCREASE_BUTTON))
        {
            
            if (g_u8CarSpeed + 10 <= MAX_SPEED)
            {
                g_u8CarSpeed += 10;
            }
            else
            {
                /* NOP. */
            }
            
            SPI_Send_Byte(g_u8CarSpeed);
        }
        else if (TRUE == SWITCH_read(SPEED_DECREASE_BUTTON))
        {
            
            if((sint_16_t)(g_u8CarSpeed - 10) < 0)
            {
                g_u8CarSpeed = 0;
            }
            else
            {
                /* NOP. */
            }
            
            SPI_Send_Byte(g_u8CarSpeed);
        }
        else
        {
            /*NOP*/
        }
        

    }
    else
    {
        /* NOP. */
    }
    
}

/**
 * @brief rx_callBack.
 * @return void.
 */
static void rx_callBack(uint8_t speed)
{
    /* When ISR fired, send data via SPI to Measurment ECU. */
    g_u8PcFlag = 1;
    g_u8CarSpeed = speed;
    SPI_Send_Byte(speed);
}

