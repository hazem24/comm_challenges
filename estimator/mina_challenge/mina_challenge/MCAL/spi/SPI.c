/*
 * SPI_Pb.h
 *
 * Created: 12/20/2019 3:25:15 PM
 *  Author: Zeyad
 */ 


#include "SPI.h"


/* Global variable used by APP to check for data sent */
uint8_t gu8_Data_SentFlag = 1;		   
/* Global variable to store if using Interrupt or Polling */								   
uint8_t gu8_Interrupt_Polling_Mode=0;  


/* Description : SPI Initialization                                              */
/* Input       : Str_SPIConfiguration_t* "Pointer to Struct of SPI Configration" */
/* Output      : Error Checking                                                  */
uint8_t SPI_Init(Str_SPIConfiguration_t* pstr_SPI_Confg)
{
	uint8_t retval = ERROR_OK;
	/* Check for MASTER OR SLAVE  */
	if( pstr_SPI_Confg -> u8_Master_Mode == MASTER )
	{
		/* Set Maser Mode */
		SETBIT(SPCR,MSTR);
		/* Set PINS of SPI */
	     DIO_Set_Direction(DIO_PIN12,OUTPUT); 	//DIO_Init_Pin(B4,OUT);   /*   SS(PB4)   --> Output */
		 DIO_Set_Direction(DIO_PIN13,OUTPUT); 	//DIO_Init_Pin(B5,OUT);   /*   MOSI(PB5) --> Output */
		 DIO_Set_Direction(DIO_PIN14,INPUT); 	//DIO_Init_Pin(B6,IN);    /*   MISO(PB6) --> Input  */
		 DIO_Set_Direction(DIO_PIN15,OUTPUT); 	// DIO_Init_Pin(B7,OUT);   /*   SCK(PB7) --> Output  */

		/* Check for Normal Speed or Double speed Mode */
		if (pstr_SPI_Confg ->u8_Speed_Mode == NORMAL_SPEED)
		{
			CLRBIT(SPSR,SPI2X);
			/* Check for prescaler of SPI */
			switch( pstr_SPI_Confg -> u8_Pre_Scaler )
			{
				case PRE_SCALER_F_4   :  CLRBIT(SPCR,SPR0);  /* set prescaler 0 , 0 */
										 CLRBIT(SPCR,SPR1);					 
										 break;
				case PRE_SCALER_F_16  :  SETBIT(SPCR,SPR0);  /* set prescaler 0 , 1 */
										 CLRBIT(SPCR,SPR1);
										 break;
				case PRE_SCALER_F_64  :  CLRBIT(SPCR,SPR0);  /* set prescaler 1 , 0 */
										 SETBIT(SPCR,SPR1); 
										 break;
				case PRE_SCALER_F_128 :  SETBIT(SPCR,SPR0);  /* set prescaler 1 , 1 */
										 SETBIT(SPCR,SPR1);
										 break;
				default: retval = ERROR_PRE_SCALER;           /* Error on prescaler input */
								  break;
			}
		}
		else if (pstr_SPI_Confg -> u8_Speed_Mode == DOUBLE_SPEED )
		{
			/* Set Double Speed Mode */
			SETBIT(SPSR,SPI2X);
			/* Check for prescaler of SPI */
			switch( pstr_SPI_Confg -> u8_Pre_Scaler )
			{
				case PRE_SCALER_F_2   : CLRBIT(SPCR,SPR0);
										CLRBIT(SPCR,SPR1);
										break;
				case PRE_SCALER_F_8   : SETBIT(SPCR,SPR0);
										CLRBIT(SPCR,SPR1);
										break;
				case PRE_SCALER_F_32  : CLRBIT(SPCR,SPR0);
										SETBIT(SPCR,SPR1);
										break;
				/* Last Case of F_64 not handled in this mode you can use it from Normal Mode */
				default: retval = ERROR_PRE_SCALER;
				break;
			}
		}
		else
		{
			retval = ERROR_SPI_SPEED_MODE ;     /* wrong SPI speed mode */
		}

		/*** Check if Interrupt or Polling ***/
		if(pstr_SPI_Confg -> u8_Interrupt_Mode == INTERRUPT) 
		{
			SETBIT(SPCR,SPIE);                       /* Enable Interrupt of Module */
			SETBIT(SREG,GLOBAL_INTERRUPT_ENABLE);    /* Enable Global Interrupt    */
			gu8_Interrupt_Polling_Mode = INTERRUPT;   /* Global variable to store if using Interrupt or Polling */
		}
			else if (pstr_SPI_Confg -> u8_Interrupt_Mode == POLLING)
			{
				gu8_Interrupt_Polling_Mode = POLLING;
			}
				else 
				{
					retval = ERROR_INTERRUPT_POLLING ;				 /* Wrong SPI Not Interrupt or Polling */
				}

		/*** Check for Data Order of SPI ***/
		if(pstr_SPI_Confg -> u8_Data_Order  == MSB) 
		{
			CLRBIT(SPCR,DORD);
		}
			else if (pstr_SPI_Confg -> u8_Data_Order  == LSB)
			{
				SETBIT(SPCR,DORD);
			}
				else 
				{
					retval = ERROR_SPI_DATA_ORDER ;		     /* Wrong SPI Not MSB or LSB */
				}

		/*** Check for Clock Polarity ***/
		if(pstr_SPI_Confg -> u8_clk_Polarity  == POLARITY_LOW ) 
		{
			CLRBIT(SPCR,CPOL);
		}
			else if (pstr_SPI_Confg -> u8_clk_Polarity  == POLARITY_HIGH)
			{
				SETBIT(SPCR,CPOL);
			}
				else 
				{
					retval = ERROR_SPI_CLK_POLARITY ;		 /* Wrong SPI CLK Not HIGH or LOW Clock */
				}

		/*** Check for Clock Sampling ***/
		if(pstr_SPI_Confg -> u8_clk_Phase  == SAMPLE_LEAD ) 
		{
			CLRBIT(SPCR,CPHA);
		}
			else if (pstr_SPI_Confg -> u8_clk_Phase  == SAMPLE_TRAIL)
			{
				SETBIT(SPCR,CPHA);
			}
				else 
				{
					retval = ERROR_SPI_CLK_PHASE ;		    /* Wrong SPI CLK Not in lead or trail */
				}
	}

	else if ( pstr_SPI_Confg ->u8_Master_Mode == SLAVE )
	{
		/* Clear Maser Mode */
		CLRBIT(SPCR,MSTR);	
	 	 DIO_Set_Direction(DIO_PIN12,INPUT); 	//DIO_Init_Pin(B4,IN);  /*  SS(PB4)   --> Input   */
	 	 DIO_Set_Direction(DIO_PIN13,INPUT); 	//DIO_Init_Pin(B5,IN);  /*  MOSI(PB5) --> Input   */
	 	 DIO_Set_Direction(DIO_PIN14,OUTPUT); 	//DIO_Init_Pin(B6,OUT); /*  MISO(PB6) --> Output  */
	 	 DIO_Set_Direction(DIO_PIN15,INPUT); 	//DIO_Init_Pin(B7,IN);  /*  SCK(PB7) --> Input    */

		/*** Check if Interrupt or Polling ***/
		if(pstr_SPI_Confg -> u8_Interrupt_Mode == INTERRUPT) 
		{
			SETBIT(SPCR,SPIE);                     /* Enable Interrupt of Module */
			SETBIT(SREG,GLOBAL_INTERRUPT_ENABLE);  /* Enable Global Interrupt    */
			gu8_Interrupt_Polling_Mode = INTERRUPT;   /* Global variable to store if using Interrupt or Polling */
		}
			else if (pstr_SPI_Confg -> u8_Interrupt_Mode == POLLING)
			{
				gu8_Interrupt_Polling_Mode = POLLING;
			}
				else 
				{
					retval = ERROR_INTERRUPT_POLLING ;	   /* Wrong SPI Not Interrupt or Polling */
				}

		/*** Check for Data Order of SPI ***/
		if(pstr_SPI_Confg -> u8_Data_Order  == MSB) 
		{
			CLRBIT(SPCR,DORD);
		}
			else if (pstr_SPI_Confg -> u8_Data_Order  == LSB)
			{
				SETBIT(SPCR,DORD);
			}
				else 
				{
					retval = ERROR_SPI_DATA_ORDER ;				 /* Wrong SPI Not MSB or LSB */
				}

		/*** Check for Clock Polarity ***/
		if(pstr_SPI_Confg -> u8_clk_Polarity  == POLARITY_LOW ) 
		{
			CLRBIT(SPCR,CPOL);
		}
			else if (pstr_SPI_Confg -> u8_clk_Polarity  == POLARITY_HIGH)
			{
				SETBIT(SPCR,CPOL);
			}
				else 
				{
					retval = ERROR_SPI_CLK_POLARITY ;				 /* Wrong SPI CLK Not HIGH or LOW Clock */
				}

		/*** Check for Clock Sampling ***/
		if(pstr_SPI_Confg -> u8_clk_Phase  == SAMPLE_LEAD ) 
		{
			CLRBIT(SPCR,CPHA);
		}
			else if (pstr_SPI_Confg -> u8_clk_Phase  == SAMPLE_TRAIL)
			{
				SETBIT(SPCR,CPHA);
			}
				else 
				{
					retval = ERROR_SPI_CLK_PHASE ;		    /* Wrong SPI CLK Not in lead or trail */
				}
		
	}
	else
	{
		retval = ERROR_SPI_MASTER_SLAVE ;		/* Wrong SPI Not Master or Slave */
	}

	/* Enable SPI Module */
	SETBIT(SPCR,SPE);
	
	return retval;								/* Return Error Value */
}

/* Description : SPI Send Byte                                                   */
/* Input       : u8_Data "const u8 Data"                                         */
/* Output      : Error Checking                                                  */
uint8_t SPI_Send_Byte(uint8_t const u8_Data)
{
	uint8_t retval = ERROR_OK;
	/* Check if Polling or Interrupt */
	if ( gu8_Interrupt_Polling_Mode == INTERRUPT )
	{
		gu8_Data_SentFlag=0;			/* Clear Global Flag of ISR */
		SPDR = u8_Data;					/* Send DATA */
	}
	else if ( gu8_Interrupt_Polling_Mode == POLLING )
	{
		SPDR = u8_Data;					/* sen DATA */
		while( ! GETBIT(SPSR,SPIF) );	/* Loop until getting Flag */
	}
	else
	{
		retval = ERROR_INTERRUPT_POLLING;		/* Wrong SPI Not Interrupt or Polling  */
	}

	return retval;
}

/* Description : SPI Receive Byte                                                */
/* Input       : u8_Data "pointer to u8 Data"                                    */
/* Output      : Error Checking                                                  */
uint8_t SPI_Receive_Byte(uint8_t *pu8_Data)
{
	uint8_t retval = ERROR_OK;
	/* Check if Polling or Interrupt */
	if ( gu8_Interrupt_Polling_Mode == INTERRUPT )
	{
		gu8_Data_SentFlag=0;			/* Clear Global Flag of ISR */
		*pu8_Data = SPDR;				/* Read DATA from SPI */
	}
	else if ( gu8_Interrupt_Polling_Mode == POLLING )
	{
		while( ! GETBIT(SPSR,SPIF) );	/* Loop until getting Flag */
		*pu8_Data = SPDR;				/* Read DATA from SPI */
	}
	else 
	{
		retval = ERROR_INTERRUPT_POLLING;		/* Wrong SPI Not Interrupt or Polling  */
	}
	return retval;
}

/* ISR of SPI */
void __vector_12(void)
{
	gu8_Data_SentFlag = 1;    /* Set the Global variable to ensure of ISR data sent successfully. */
}