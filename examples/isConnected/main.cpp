/*!
	@file     main.cpp
	@author   Gavin Lyons
	@brief MCP4725 DAC library example file. IsConnected.
	@note
	This file carries out Connection test to see if DAC on the I2C bus.
	Output to Serial monitor 38400 baud rate 

*/

// Libraries
#include "mcp4725/mcp4725.hpp"

// Definations
#define DAC_REF_VOLTAGE 3.3  // Volts DAC supply-reference voltage

// Setup
MCP4725_PICO myDAC(DAC_REF_VOLTAGE);
uint16_t TestCount = 0;

/*!
	@brief The MAIN loop function, isConnected example file
	@return  Program Exit code
*/
int main () {
	// Serial port setup
	busy_wait_ms(500);
	stdio_init_all(); // Initialize chosen serial port, default 38400 baud
	busy_wait_ms(1000);
	printf("MCP4725_PICO : isConnected example.\r\n");
	
	if (!myDAC.begin(MCP4725A0_Addr_A00 , i2c0, 100, 12 , 13))
	{
		printf("MCP4725 : Failed to initialize DAC.!\r\n");
		while(1){};
	}else{
		printf("MCP4725 : Initialised DAC!\r\n");
	}

	while(1)
	{
		printf("Test Count :: %u \r\n", TestCount);
		TestCount++;
		if (myDAC.isConnected())
		{
			printf("DAC Connected\r\n");
		}else{
			printf("DAC Disconnected\r\n");
		}
		busy_wait_ms(5000);
	}
	return 0;
} // End of main loop


