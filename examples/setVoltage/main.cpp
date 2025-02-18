/*!
	@file     main.cpp
	@author   Gavin Lyons
	@brief   Example cpp file for MCP4725 DAC library, SetVoltage.
	@note
	This file carries out some set voltage tests.
	-# Test 1 setInputCode function 4096 vout = 3.3V.
	-# Test 2 setInputCode function  2048 Vout = 1.65V.
	-# Test 3 setVoltage function 2.0 , vout 2.0V.
	-# Test 4 setVoltage function 0.800 , vout = 800mV.
	
	Example input code calculation .
	PICO Vref = 3.3 , MCP4725A0 , resolution = 2^12 = 4096.
	Note Vout = (Vref X input code) / Resolution .
	Vout = (3.3 X  inputcode)/ 4096.
	eg for input code  2048 -> Vout = (3.3 X 2048) /4096 = 1.65 V.
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
MCP4725_PICO myDAC(DAC_REF_VOLTAGE);

/*!
	@brief The MAIN loop function, setVoltage example file
	@return  Program Exit code
*/
int main () {
	// local variable declaration:
	busy_wait_ms(500);
	stdio_init_all(); // Initialize chosen serial port, default 38400 baud
	busy_wait_ms(1000);
	printf("MCP4725_PICO : Set Voltage example.\r\n");
	
	if (!myDAC.begin(myDAC.MCP4725A0_Addr_A00, i2c0, Speed, Data, Clock, Timeout))
	{
		printf("MCP4725 : Failed to initialize DAC.!\r\n");
		while(1){};
	}

	// Test 1  setInputCode function 4090
	
	if(myDAC.setInputCode(4090, myDAC.MCP4725_FastMode, myDAC.MCP4725_PowerDown_Off) == true)
		printf("Set max value & normal mode, Vout = ~3.3\n");
	else 
		printf("I2C error\n");
	busy_wait_ms(5000);

	// Test 2 setInputCode function  2048 
	
	if(myDAC.setInputCode(2048, myDAC.MCP4725_FastMode, myDAC.MCP4725_PowerDown_Off)== true)
		printf("Set 50%% value & normal mode, Vout = 1.65\n");
	else
		printf("I2C error\n");
	busy_wait_ms(5000);

	// Test 3 SetVoltage function 2.5V
	if(myDAC.setVoltage(2.5, myDAC.MCP4725_FastMode, myDAC.MCP4725_PowerDown_Off) == true)
		printf("Set 2.5V & normal mode, Vout = 2.5\n");
	else
		printf("I2C error\n");
	busy_wait_ms(5000);

	// Test 4 SetVoltage 800mV
	if(myDAC.setVoltage(.800, myDAC.MCP4725_FastMode, myDAC.MCP4725_PowerDown_Off)== true)
		printf("Set 800mV & normal mode, Vout = 800mV\n");
	else
		printf("I2C error\n");
	busy_wait_ms(5000);

	// turn off voltgae
	if(myDAC.setVoltage(0, myDAC.MCP4725_FastMode, myDAC.MCP4725_PowerDown_Off)== true)
		printf("zero volts\n");
	else
		printf("I2C error\n");
	busy_wait_ms(1000);

	printf("Test Over\n");
	myDAC.deinitI2C();
	return 0;
}


