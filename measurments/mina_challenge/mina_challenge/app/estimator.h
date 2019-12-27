/*
 * estimator.h
 *
 * Created: 27-Dec-19 7:57:52 PM
 *  Author: LENOVO
 */ 


#ifndef ESTIMATOR_H_
#define ESTIMATOR_H_

#include "../common/compiler.h"
#include "../MCAL/dio/dio.h"
#include "../MCAL/uart/Uart.h"
#include <util/delay.h>
#include "../lcd.h"
#include "../MCAL/spi/SPI.h"
#include "../MCAL/spi/SPI_Pb.h"

#include "../HAL/switch/switch.h"


#define SPEED_INCREASE_BUTTON DIO_PIN20
#define SPEED_DECREASE_BUTTON DIO_PIN10 
#define MAX_SPEED             100
/**
 * @brief 
 * @return void.
 */
void ESTMATIOR_init(void);


/**
 * @brief Estimator App.
 * @return void.
 */
void ESTIMATOR_run(void);


#endif /* ESTIMATOR_H_ */