/*
 * SPI_Pb.h
 *
 * Created: 12/20/2019 3:25:15 PM
 *  Author: Zeyad
 */ 



#ifndef SPI_H_
#define SPI_H_

#include "MACROS.h"
#include "../../common/compiler.h"
#include "SPI_REG.h"
#include "../dio/dio.h"

typedef struct {
uint8_t u8_Master_Mode;      
uint8_t u8_Speed_Mode;      
uint8_t u8_Pre_Scaler;       
uint8_t u8_Interrupt_Mode;
uint8_t u8_clk_Phase;
uint8_t u8_Data_Order;
uint8_t u8_clk_Polarity;
}Str_SPIConfiguration_t;


/* NORMAL SPEED */
#define PRE_SCALER_F_4   0
#define PRE_SCALER_F_16  1
#define PRE_SCALER_F_64  2
#define PRE_SCALER_F_128 3
/* DOUBLE SPEED */
#define PRE_SCALER_F_2   0
#define PRE_SCALER_F_8   1
#define PRE_SCALER_F_32  2
/* YOU CAN USE PRE_SCALER_F_64 FROM NORMAL SPEED ! */

/* ERROR FLAGS */
#define ERROR_OK                 0   /* OK No Error */
#define ERROR_SPI_SPEED_MODE    -1   /* wrong SPI speed mode                */
#define ERROR_PRE_SCALER        -2   /* wrong SPI Prescaler                 */
#define ERROR_SPI_MASTER_SLAVE  -3   /* Wrong SPI Not Master or Slave       */
#define ERROR_INTERRUPT_POLLING -4   /* Wrong SPI Not Interrupt or Polling  */
#define ERROR_SPI_DATA_ORDER	-5	 /* Wrong SPI Not MSB or LSB            */
#define ERROR_SPI_CLK_POLARITY  -6   /* Wrong SPI CLK Not HIGH or LOW Clock */
#define ERROR_SPI_CLK_PHASE     -7   /* Wrong SPI CLK Not in lead or trail  */
/* Polling or Interrupt */
#define POLLING         0
#define INTERRUPT       1
/* Master or Slave */
#define SLAVE           0
#define MASTER          1
/* Most SB or Least SB */
#define MSB             0
#define LSB             1
/* polarity LOW or HIGH */
#define POLARITY_LOW    0
#define POLARITY_HIGH   1
/* Samples on LEAD or TRAIL */
#define SAMPLE_LEAD     0
#define SAMPLE_TRAIL    1
/* NORMAL Speed or DOUBLE Speed  */
#define NORMAL_SPEED    0
#define DOUBLE_SPEED    1


/* ISR FOR SPI */
void __vector_12(void)__attribute((signal,used));   /* Serial Transfer Complete */

/* Description : SPI Initialization                                              */
/* Input       : Str_SPIConfiguration_t* "Pointer to Struct of SPI Configration" */
/* Output      : Error Checking                                                  */
uint8_t SPI_Init(Str_SPIConfiguration_t* pstr_SPI_Confg);

/* Description : SPI Send Byte                                                   */
/* Input       : u8_Data "const u8 Data"                                         */
/* Output      : Error Checking                                                  */
uint8_t SPI_Send_Byte(uint8_t const u8_Data);

/* Description : SPI Receive Byte                                                */
/* Input       : u8_Data "pointer to u8 Data"                                    */
/* Output      : Error Checking                                                  */
uint8_t SPI_Receive_Byte(uint8_t *pu8_Data);


#endif /* SPI_H_ */