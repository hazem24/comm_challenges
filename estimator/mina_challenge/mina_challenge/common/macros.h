/**
 * @file macros.h
 * @brief This file contains some useful macros.
 *
 * @author Hazem Khaled
 * @date Nov 1, 2019
 *
 */

#ifndef _MACROS_H_
#define _MACROS_H_
#include "compiler.h"



#define SET_BIT(REG, BIT_NUMBER)    REG = (REG | (1 << BIT_NUMBER))

#define CLEAR_BIT(REG, BIT_NUMBER)  REG = REG & (~(1 << BIT_NUMBER))

#define READ_BIT(REG, BIT_NUMBER)   REG = REG & ((1 << BIT_NUMBER))

#define TOGGLE_BIT(REG, BIT_NUMBER) REG = REG ^ (1 << BIT_NUMBER)

#define SET_BITS(REG, BITS_NUMBERS) REG = REG | BITS_NUMBERS

#define BIT_IS_CLEAR(REG, BIT_NUMBERS) !(REG & (1 << BIT_NUMBERS))

#define SET_BIT_WITH_VALUE(REG, BIT_NUMBER, VALUE) REG = REG | (VALUE << BIT_NUMBER)
#endif /* MACROS_H_ */
