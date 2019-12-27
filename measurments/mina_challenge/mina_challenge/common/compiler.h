/*
 * data_type.h
 *
 *  Created on: Nov 1, 2019
 *      Author: LENOVO
 */

#ifndef DATA_TYPE_H_
#define DATA_TYPE_H_

/*
 * Define Most Data Types.
 */
#define FALSE 0U
#define TRUE  1U
typedef unsigned char uint8_t;
typedef char sint8_t;
/*
 * Integer in AVR (16-bits).
 */
typedef unsigned int uint16_t;
typedef signed   int sint_16_t;

typedef unsigned long   int  uint32_t;
typedef signed   long   int  sint32_t;


typedef unsigned long long   int  uint64_t;
typedef signed   long long   int  sint64_t;

typedef uint8_t* ptr_8u_t; 
typedef unsigned int* u16_ptr_t;
#define NULL 0U

#endif /* DATA_TYPE_H_ */
