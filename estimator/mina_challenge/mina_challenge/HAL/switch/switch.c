/*
 * switch.c
 *
 * Created: 27-Dec-19 8:42:38 PM
 *  Author: LENOVO
 */ 
#include "switch.h"


/**
 * @brief 
 * 
 * @param switch_number 
 * @return uint8_t 
 */

uint8_t SWITCH_read(uint8_t switch_number)
{
    uint8_t au8_response = LOW;
    if ( HIGH == DIO_Read_Pin(switch_number))
    {
        _delay_ms(100);
        if(HIGH == DIO_Read_Pin(switch_number))
        {
           au8_response = HIGH;     
        }
    }
    return au8_response;
}