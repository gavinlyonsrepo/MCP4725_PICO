/*
 * Project Name: MCP4725_PICO
 * File: main.cpp
 * Description: example cpp file for MCP4725 DAC library
 * This file carries out some set voltage tests
 * See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/MCP4725_PICO
 */

//----------------------------------------------------------------
// Test 1 setInputCode function 4096 vout = 3.3V
// Test 2 setInputCode function  2048 Vout = 1.65V
// Test 3 setVoltage function 2.0 , vout 2.0V
// Test 4 setVoltage function 0.800 , vout = 800mV
//----------------------------------------------------------------

// Example input code calculation 
// PICO Vref = 3.3 , MCP4725A0 , resolution = 2^12 = 4096
// Note Vout = (Vref X input code) / Resolution 
// Vout = (3.3 X  inputcode)/ 4096
// eg for input code  2048 -> Vout = (3.3 X 2048) /4096 = 1.65 V

// Libraries
#include <stdio.h>
#include "pico/stdlib.h"
#include "mcp4725/mcp4725.hpp"


// Definations
#define DAC_REF_VOLTAGE 3.3  // Volts DAC supply-reference voltage

// Setup
MCP4725 myDAC(DAC_REF_VOLTAGE);

int main () {
	// local variable declaration:
	busy_wait_ms(500);
	stdio_init_all(); // Initialize chosen serial port, default 38400 baud
	busy_wait_ms(1000);
	printf("MCP4725_PICO : Set Voltage example.\r\n");
	
	if (!myDAC.begin(MCP4725A0_Addr_A00 , i2c1, 100, 18 , 19))
	{
		printf("MCP4725 : Failed to initialize DAC.!\r\n");
		while(1){};
	}

	// Test 1  setInputCode function 4090
	
	if(myDAC.setInputCode(4090, MCP4725_FastMode, MCP4725_PowerDown_Off) == true)
		printf("Set max value & normal mode, Vout = ~3.3\n");
	else 
		printf("I2C error\n");
	busy_wait_ms(5000);

	// Test 2 setInputCode function  2048 
	
	if(myDAC.setInputCode(2048, MCP4725_FastMode, MCP4725_PowerDown_Off)== true)
		printf("Set 50% value & normal mode, Vout = 1.65\n");
	else
		printf("I2C error\n");
	busy_wait_ms(5000);

	// Test 3 SetVoltage function 2.5V
	if(myDAC.setVoltage(2.5, MCP4725_FastMode, MCP4725_PowerDown_Off) == true)
		printf("Set 2.5V & normal mode, Vout = 2.5\n");
	else
		printf("I2C error\n");
	busy_wait_ms(5000);

	// Test 4 SetVoltage 800mV
	if(myDAC.setVoltage(.800, MCP4725_FastMode, MCP4725_PowerDown_Off)== true)
		printf("Set 800mV & normal mode, Vout = 800mV\n");
	else
		printf("I2C error\n");
	busy_wait_ms(5000);
	return 0;
}


