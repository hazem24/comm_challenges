/*
 * mina_challenge.c
 *
 * Created: 26-Dec-19 6:56:53 PM
 * Author : LENOVO
 */ 

#include "app/estimator.h"

int main(void)
{
   
	/* Init App. */
	ESTIMATOR_run();
    while ( TRUE ) 
    {
		ESTIMATOR_run();
    }
}

