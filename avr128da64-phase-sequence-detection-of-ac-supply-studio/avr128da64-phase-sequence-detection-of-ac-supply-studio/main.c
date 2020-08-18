#include <atmel_start.h>
#include <util/delay.h>
#include <stdio.h>
#include "application.h"


int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	_delay_ms(1000);                   //driver initialization blocking delay before enter in to applications
	
	threePhaseAmplitudeMax();          //Increase the amplitude of the emulator (Digital Pot)
	
	readSwToSelectFrequency();		//Read switch to get user desired frequency to generate three-phase input
	
	defaultSignalGeneration();         //Default signal generation with user desired frequency
	
	/* Replace with your application code */
	while (1)
	{
		application();
	}
}