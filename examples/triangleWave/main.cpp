/*!
	@file     main.cpp
	@author   Gavin Lyons
	@brief Example cpp file for MCP4725 DAC library, triangleWave.
	@note Generated a triangle waveform for MCP4725 75 Hz.

*/

// Libraries
#include <stdio.h>
#include "pico/stdlib.h"
#include "mcp4725/mcp4725.hpp"

// test setup
#define DAC_REF_VOLTAGE 3.3  // Volts DAC supply-reference voltage
uint16_t Speed = 100;		 // I2C speed in Khz
uint8_t Data = 12;			 // I2C GPIO for data line
uint8_t Clock = 13;			 // I2C GPIO for Clock line
uint32_t Timeout = 50000; // I2C timeout delay in uS.

// Setup
MCP4725_PICO  myDAC(DAC_REF_VOLTAGE);
int16_t smoothing = 50;
int16_t counter = 0;

/*!
	@brief The MAIN loop function, squareWave example file
	@return  Program Exit code
*/
int main () {
	// local variable declaration:
	busy_wait_ms(500);
	stdio_init_all(); // Initialize chosen serial port, default 38400 baud
	busy_wait_ms(1000);
	printf("MCP4725_PICO : Triangle wave example 75 Hz.\r\n");
	
	if (!myDAC.begin(myDAC.MCP4725A0_Addr_A00, i2c0, Speed, Data, Clock, Timeout))
	{
		printf("MCP4725 : Failed to initialize DAC.!\r\n");
		while(1){};
	}

	while(1)
	{
		counter = 0;
		for (counter = 0; counter <= 4095; counter = counter + smoothing)
		{
			myDAC.setInputCode(counter, myDAC.MCP4725_FastMode, myDAC.MCP4725_PowerDown_Off);
		}
		counter = 4095;
		for (counter = 4095; counter >= 0; counter = counter - smoothing)
		{
			myDAC.setInputCode(counter, myDAC.MCP4725_FastMode, myDAC.MCP4725_PowerDown_Off);
		}
		
	}
	return 0;
}


