/*!
	@file     main.cpp
	@author   Gavin Lyons
	@brief    Example cpp file for MCP4725 DAC library, sawToothWave.
	@note     Generated a sawtooth waveform for MCP4725 146 Hz.
*/


// Libraries
#include <stdio.h>
#include "pico/stdlib.h"
#include "mcp4725/mcp4725.hpp"

// Definations
#define DAC_REF_VOLTAGE 3.3  // Volts DAC supply-reference voltage

// Setup
MCP4725_PICO myDAC(DAC_REF_VOLTAGE);
int16_t smoothing = 50;
int16_t counter = 0;

/*!
	@brief The MAIN loop function, sawTooth example file
	@return  Program Exit code
*/
int main () {
	// local variable declaration:
	busy_wait_ms(500);
	stdio_init_all(); // Initialize chosen serial port, default 38400 baud
	busy_wait_ms(1000);
	printf("MCP4725_PICO : SawTooth wave example 150 Hz.\r\n");
	
	if (!myDAC.begin(MCP4725A0_Addr_A00 , i2c1, 400, 18 , 19))
	{
		printf("MCP4725 : Failed to initialize DAC.!\r\n");
		while(1){};
	}

	while(1)
	{
		counter = 0;
		for (counter = 0; counter <= 4095; counter = counter + smoothing)
		{
			myDAC.setInputCode(counter, MCP4725_FastMode, MCP4725_PowerDown_Off);
		}
		myDAC.setInputCode(0, MCP4725_FastMode, MCP4725_PowerDown_Off);
	}
	return 0;
}


