/*
 * switch.h
 *
 * Created: 27-Dec-19 8:42:52 PM
 *  Author: LENOVO
 */ 


#ifndef SWITCH_H_
#define SWITCH_H_

#include "../../MCAL/dio/dio.h"
#include "../../micro_config.h"
#include "../../common/compiler.h"
#include "util/delay.h"


/**
 * @brief 
 * 
 * @param switch_number 
 * @return uint8_t 
 */
uint8_t SWITCH_read(uint8_t switch_number);

#endif /* SWITCH_H_ */