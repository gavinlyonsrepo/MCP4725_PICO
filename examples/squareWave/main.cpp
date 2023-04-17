/*
 * Project Name: MCP4725_PICO
 * File: main.cpp
 * Description: example cpp file for MCP4725 DAC library
 * Generate a square wave 100Hz
 * See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/MCP4725_PICO
 */

// Libraries
#include <stdio.h>
#include "pico/stdlib.h"
#include "mcp4725/mcp4725.hpp"

// Definations
#define DAC_REF_VOLTAGE 3.3  // Volts DAC supply-reference voltage

// Setup
MCP4725 myDAC(DAC_REF_VOLTAGE);
int TestCount = 0;
int Period = 10; // mS Period of square wave, F = 1/T 100Hz

int main () {
	// local variable declaration:
	busy_wait_ms(500);
	stdio_init_all(); // Initialize chosen serial port, default 38400 baud
	busy_wait_ms(1000);
	printf("MCP4725_PICO : Square wave example.\r\n");
	
	if (!myDAC.begin(MCP4725A0_Addr_A00 , i2c1, 400, 18 , 19))
	{
		printf("MCP4725 : Failed to initialize DAC.!\r\n");
		while(1){};
	}

	while(1)
	{
		myDAC.setInputCode(4095, MCP4725_FastMode, MCP4725_PowerDown_Off);
		busy_wait_ms(Period/2);  
		myDAC.setInputCode(0, MCP4725_FastMode, MCP4725_PowerDown_Off);
		busy_wait_ms(Period/2);
	}
	return 0;
}


