/*
 * mina_challenge.c
 *
 * Created: 26-Dec-19 6:56:53 PM
 * Author : LENOVO
 */ 

#include "app/measurment_ecu.h"
#include "common/compiler.h"
int main(void)
{
   
	/* Init App. */
	  MEASURMENT_init();
    while ( TRUE ) 
    {
		    MEASURMENT_run();
    }
}

