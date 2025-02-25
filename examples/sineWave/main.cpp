/*!
	@file     main.cpp
	@author   Gavin Lyons
	@brief Example cpp file for MCP4725 DAC library, SineWave.
	@note
	Generate a Sine waveform for MCP4725. 
	The data to create Sine wave  is the MCP4725 Sinewave_Data.cpp file.
	In the MCP4725_Sinewave_Data.hpp user can pick Resolution "SINEWAVE_RES".
	Resolution of Sinewave in bits (2^bits).
	Select 512, 256, 128, 64 or 32 points table.
	i.e. 2^9 =  512 , 9 bit resolution etc.
	default 128 100Hz.
*/


// Libraries
#include <stdio.h>
#include "pico/stdlib.h"
#include "mcp4725/mcp4725.hpp"
#include "mcp4725/mcp4725_Sinewave_Data.hpp" // Sine wave data structure's

// test setup
#define DAC_REF_VOLTAGE 3.3  // Volts DAC supply-reference voltage
uint16_t Speed = 100;		 // I2C speed in Khz
uint8_t Data = 12;			 // I2C GPIO for data line
uint8_t Clock = 13;			 // I2C GPIO for Clock line
uint32_t Timeout = 50000; // I2C timeout delay in uS.

// Setup
MCP4725_PICO myDAC(DAC_REF_VOLTAGE);

/*!
	@brief The MAIN loop function, sineWave example file
	@return  Program Exit code
*/
int main () {
	// local variable declaration:
	busy_wait_ms(500);
	stdio_init_all(); // Initialize chosen serial port, default 38400 baud
	busy_wait_ms(1000);
	printf("MCP4725_PICO : Sine wave example.\r\n");
	
	if (!myDAC.begin(myDAC.MCP4725A0_Addr_A00, i2c0, Speed, Data, Clock, Timeout))
	{
		printf("MCP4725 : Failed to initialize DAC.!\r\n");
		while(1){};
	}

	while(1)
	{
			for (uint16_t i = 0; i < SINEWAVE_RES; i++)
			{
				myDAC.setInputCode(pDacLookupSineWave[i], myDAC.MCP4725_FastMode, myDAC.MCP4725_PowerDown_Off);
			}
	}
	return 0;
}


