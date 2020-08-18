/*
 * Communication.c
 *
 * Created: 12/13/2019 1:58:18 PM
 *  Author: I20946
 */ 

/******************************************************************************/
/* Include Files                                                              */
/******************************************************************************/
#include "Communication.h"
#include "spi_basic.h"
#include "spi_basic_example.h"
#include "atomic.h"


void drive_slave_select_low();
void drive_slave_select_high();
/***************************************************************************//**
 * @brief Initializes the SPI communication peripheral.
 *
 * @param lsbFirst - Transfer format (0 or 1).
 *                   Example: 0x0 - MSB first.
 *                            0x1 - LSB first.
 * @param clockFreq - SPI clock frequency (Hz).
 *                    Example: 1000 - SPI clock frequency is 1 kHz.
 * @param clockPol - SPI clock polarity (0 or 1).
 *                   Example: 0x0 - idle state for SPI clock is low.
 *	                          0x1 - idle state for SPI clock is high.
 * @param clockPha - SPI clock phase (0 or 1).
 *                   Example: 0x0 - data is latched on the leading edge of SPI
 *                                  clock and data changes on trailing edge.
 *                            0x1 - data is latched on the trailing edge of SPI
 *                                  clock and data changes on the leading edge.
 *
 * @return 0 - Initialization failed, 1 - Initialization succeeded.
*******************************************************************************/
unsigned char SPI_Init()
{
         SPI_0_init();
		 
		 return 0;
}

/***************************************************************************//**
 * @brief Writes data to SPI.
 *
 * @param data - Write data buffer:
 *               - first byte is the chip select number;
 *               - from the second byte onwards are located data bytes to write.
 * @param bytesNumber - Number of bytes to write.
 *
 * @return Number of written bytes.
*******************************************************************************/
unsigned char SPI_Write(unsigned char* data,
                        unsigned char bytesNumber)
{
           SPI_0_write_block(data,bytesNumber);
	 return 0;
}

/***************************************************************************//**
 * @brief Reads data from SPI.
 *
 * @param data - As an input parameter, data represents the write buffer:
 *               - first byte is the chip select number;
 *               - from the second byte onwards are located data bytes to write.
 *               As an output parameter, data represents the read buffer:
 *               - from the first byte onwards are located the read data bytes. 
 * @param bytesNumber - Number of bytes to write.
 *
 * @return Number of written bytes.
*******************************************************************************/
unsigned char SPI_Read(unsigned char* data,
                       unsigned char bytesNumber)
{
       SPI_0_read_block(data,bytesNumber);
	   return 0;
}




void AD9833_chipSelect (uint8_t selAD9833, uint8_t level)
{
 
       switch(selAD9833)
	   {
	     case 1:
		  AD9833_FSN1_set_level(level);
		 break;
		 case 2:
		  AD9833_FSN2_set_level(level);
		 break;
		 case 3:
		  AD9833_FSN3_set_level(level);
		 break;

	   }
	   
}

void AD5227_chipSelect (uint8_t selAD5227, uint8_t level)
{
	switch(selAD5227)
	{
		case 1:
		AD5227_CS1_set_level(level);
		break;
		case 2:
		AD5227_CS2_set_level(level);
		break;
		case 3:
		AD5227_CS3_set_level(level);
		break;
	}
}