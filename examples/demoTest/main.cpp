/*!
	@file     main.cpp
	@author   Gavin Lyons
	@brief    Example cpp file for MCP4725 DAC library , demoTest.
	@note
	This file carries our a series of tests to demonstrate features.
	Such as Power down reset wakeup and EEPROM data save.
	Output to Serial monitor and multimeter on Vout of DAC.
	
	-# Test 0 print settings.
	-# Test 1 set voltage to Vmax with power ON mode   , Vout = 3.3 V.
	-# Test 2 set voltage to 1.65  with power off mode , Vout = 0.0 V.
	-# Test 3 set voltage to 900mV with power off mode , Vout = 0.0 V.
	-# Test 4 wake up device,                            Vout = 900m V.
	-# Test 5a set voltages to Vmax with power ON mode,  Vout =  3.3 V.
	-# Test 5b print settings.
	-# Test 6a Set Voltage and power mode on in the EEPROM,  Vout = 1.0 V.
	-# Test 6b set voltage to Vmax with power ON mode        Vout = 3.3 V.
	-# Test 6c Reset device & Vout will revert EEPROM in 6a, Vout = 1.0 V.
	-# Test 7 Clear the EEPROM,                              Vout = 0.0 V.
	-# Test 7b print settings.
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
uint16_t TestCount = 0;

// Functions
void printDACSettings(void);

/*!
	@brief The MAIN loop function, demoTest example file
	@return  Program Exit code
*/
int main () {
	// Serial port setup
	busy_wait_ms(500);
	stdio_init_all(); // Initialize chosen serial port, default 38400 baud
	busy_wait_ms(1000);
	printf("MCP4725_PICO : Demo Test example.\r\n");
	
	if (!myDAC.begin(myDAC.MCP4725A0_Addr_A00, i2c0, Speed, Data, Clock, Timeout))
	{
		printf("MCP4725 : Failed to initialize DAC.!\r\n");
		while(1){};
	}else{
		printf("MCP4725 : Initialised DAC!\r\n");
	}

	// Check settings Test 0
	printDACSettings();
	
	// Set voltage test 1
	if (myDAC.setInputCode(4090, myDAC.MCP4725_FastMode, myDAC.MCP4725_PowerDown_Off))
		printf("\n 1 Set max value & normal mode, Vout = 3.3 \n");
	else
		printf("I2C error\n");

	busy_wait_ms(5000);
	
	// Power down test 2 
	if (myDAC.setInputCode(2048, myDAC.MCP4725_FastMode, myDAC.MCP4725_PowerDown_1kOhm) == true)
		printf("\n 2 Set max value & normal mode, Vout = 1.65 but off \n");
	else
		printf("I2C error\n");
	busy_wait_ms(5000);

	// Power down test 3 
	if(myDAC.setVoltage(0.9, myDAC.MCP4725_RegisterMode, myDAC.MCP4725_PowerDown_100kOhm)== true)
		printf("\n 3 Set max value & normal mode, Vout = 900 mV but off \n");
	else
		printf("I2C error\n");
	busy_wait_ms(5000);

	// Wake up the device test 4
	if (myDAC.GeneralCall(myDAC.MCP4725_GeneralCallWakeUp) == true)
		printf("\n 4 Device Wake up vout = 900mV \n");
	else 
		printf("I2C error\n");
	busy_wait_ms(5000);

	// Set voltage test 5
	if(myDAC.setInputCode(4090, myDAC.MCP4725_FastMode, myDAC.MCP4725_PowerDown_Off) == true)
		printf("\n 5 Set max value & normal mode, Vout = 3.3 \n");
	else
		printf("I2C error\n");
	busy_wait_ms(5000);

	// test 5b check settings
	printDACSettings();

	// Set Voltage and power mode EEPROM test 6 
	if(myDAC.setVoltage(1.0, myDAC.MCP4725_EEPROM_Mode, myDAC.MCP4725_PowerDown_Off) == true )
		printf("\n 6 Program EEPROM and read out 1.0 volt \n");
	else
		printf("I2C error\n");
	busy_wait_ms(5000);

	// Set Voltage to 3.3 Set Voltage test 6b 
	if(myDAC.setInputCode(4090, myDAC.MCP4725_FastMode, myDAC.MCP4725_PowerDown_Off) == true )
		printf("\n 6b set volt to 3.3  \n");
	else
		printf("I2C error\n");
	busy_wait_ms(5000);

	// reset the device with voltage = 1.0 test6c
	if (myDAC.GeneralCall(myDAC.MCP4725_GeneralCallReset) == true)
		printf("\n 6c set volt to 1.0 from EEPROM \n");
	else
		printf("I2C error\n");
	busy_wait_ms(5000);
	
	// Reset EEPROM  Test 7 
	if (myDAC.setVoltage(0.0, myDAC.MCP4725_EEPROM_Mode,  myDAC.MCP4725_PowerDown_Off) == true)
		printf("\n 7 Program EEPROM and read out 0.0 volt \n");
	else
		printf("I2C error\n");
	busy_wait_ms(1000); 

	// Check settings  Test 7b 
	printDACSettings();
	printf("\nMCP4725 : DONE!\r\n");
	return 0;
} // End of main loop

void printDACSettings(){
	printf("\nSafety Check Flag : %d \r\n", myDAC.getSafetyCheckFlag());
	printf("Serial Debug Flag : %d \r\n", myDAC.getSerialDebugFlag());
	printf("Note :  65535 is I2C error\r\n");
	printf("DAC input code Current  : %u \r\n", myDAC.getInputCode());
	printf("DAC voltage Current: %f \r\n", myDAC.getVoltage());
	printf("DAC input code in EEPROM : %u \r\n", myDAC.getStoredInputCode());
	printf("DAC Voltage in EEPROM : %f \r\n", myDAC.getStoredVoltage());
	printf("Power Type Current : %u \r\n", myDAC.getPowerType());
	printf("Power Type in EEPROM : %u \r\n", myDAC.getStoredPowerType());
}
