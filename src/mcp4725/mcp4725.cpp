/*
 * Project Name: MCP4725_PICO
 * File: MCP4725.cpp
 * Description: cpp file for MCP4725 DAC library
 * See URL for full details.
 * URL: https://github.com/gavinlyonsrepo/MCP4725_PICO
 */

#include "../include/mcp4725/mcp4725.hpp"

// Constructor MCP4725()

MCP4725::MCP4725(float refV)
{
	setReferenceVoltage(refV);
}

// Desc : Begin() Init & config i2c
// Param1 : enum MCP4725_I2C_Addr_e : I2C address 8 bit address 0x6?
// Param2 : i2c_inst_t* : I2C instance of port, IC20 or I2C1
// Param3 : uint16_t : I2C Bus Clock speed in Kbit/s. see 7.1 datasheet
// Param4 : uint8_t : I2C Data pin
// Param5 : uint8_t : I2C Clock pin
// Returns : bool :true if success , false for failure
bool MCP4725::begin(MCP4725_I2C_Addr_e addr, i2c_inst_t* i2c_type, uint16_t CLKspeed, uint8_t SDApin, uint8_t SCLKpin)
{
	uint8_t rxData = 0;

	 // init I2c pins and interface
	_i2cAddress  = addr;
	_i2c = i2c_type;
	_SClkPin = SCLKpin;
	_SDataPin = SDApin;
	_CLKSpeed = CLKspeed;
	gpio_set_function(_SDataPin, GPIO_FUNC_I2C);
	gpio_set_function(_SClkPin, GPIO_FUNC_I2C);
	gpio_pull_up(_SDataPin);
	gpio_pull_up(_SClkPin);
	i2c_init(_i2c, _CLKSpeed * 1000);
	busy_wait_ms(50);
	// check connection?
	return isConnected();
}

// Desc :: Checks if DAC is connected
// return  true if DAC is connected , false if not
bool MCP4725::isConnected()
{
	int ReturnCode = 0;
	uint8_t rxData = 0;
	// check connection?
	ReturnCode = i2c_read_timeout_us(_i2c, _i2cAddress , &rxData, 1, false, MCP4725_I2C_DELAY);
	if (ReturnCode < 1){ // no bytes read back from device or error issued
		if (_serialDebug == true)
		{
			printf("1202  PICO_MCP4725::is connected: \r\n");
			printf("Check Connection, Return code :: %d ,RX data :: %u \r\n", ReturnCode , rxData);
		}
		return false;
	}
	return true;
}

// Desc set reference voltage
// Parameters 1 :: float value to set reference voltage called from constructor
void MCP4725::setReferenceVoltage(float voltage)
{
	if (voltage == 0)
		_refVoltage = MCP4725_REFERENCE_VOLTAGE;
	else
		_refVoltage = voltage;

	_bitsPerVolt = (float)MCP4725_STEPS / _refVoltage;
}

// Desc : Return reference voltage
float MCP4725::getReferenceVoltage(){return _refVoltage;}

// Desc setInputCode : Set voltage out based on DAC input code
// Parameter 1 : inputcode : uint16_t : 0 to MCP4725_MAX_VALUE voltage out
// Parameter 2 : mode :  enum  MCP4725_CmdType_e
//	see enum descriptions
// Parameter 3 : power type : enum  : MCP4725_PowerDownType_e
//	see enum descriptions
// return bool : output of writeCommand true for success, false for failure
bool MCP4725::setInputCode(uint16_t InputCode, MCP4725_CmdType_e mode, MCP4725_PowerDownType_e powerType)
{
	if (_safetyCheck  == true)
	{
		if (InputCode > MCP4725_MAX_VALUE)
			InputCode = MCP4725_MAX_VALUE;
	}

	return writeCommand(InputCode, mode, powerType);
}

// Desc setInputCode : Set voltage out based on voltage input
// Parameter 1 : voltage: float : 0 to_MCP4725_REFERENCE_VOLTAGE, voltage out
// Parameter 2 : mode :  enum  MCP4725_CmdType_e
//	-see enum descriptions
// Parameter 3 : power type : enum  : MCP4725_PowerDownType_e
//	-see enum descriptions
// return bool : output of writeCommand true for success, false for failure
bool MCP4725::setVoltage(float voltage, MCP4725_CmdType_e mode, MCP4725_PowerDownType_e powerType)
{
	uint16_t voltageValue = 0;

	// Convert voltage to DAC bits
	//xx,xx,xx,xx,D11,D10,D9,D8 ,D7,D6,D4,D3,D2,D9,D1,D0
	if (_safetyCheck  == true)
	{
		if (voltage >= _refVoltage)
			voltageValue = MCP4725_MAX_VALUE;
		else if (voltage <= 0)
			voltageValue = 0; //make sure value never below zero
		else
			voltageValue = voltage * _bitsPerVolt;
	}
	else if (_safetyCheck ==  false)
	{
		voltageValue = voltage * _bitsPerVolt;
	}

	return writeCommand(voltageValue, mode, powerType);
}


// Desc :: get current DAC InputCode from DAC register
// Returns : uint16_t  : DAC InputCode :or 0xFFFF if I2C error
uint16_t MCP4725::getInputCode()
{
	uint16_t inputCode = readRegister(MCP4725_ReadDACReg);
	// InputCode = D11,D10,D9,D8,D7,D6,D5,D4, D3,D2,D1,D0,x,x,x,x

	if (inputCode != MCP4725_ERROR)
		return inputCode >> 4; //0,0,0,0,D11,D10,D9,D8,  D7,D6,D5,D4,D3,D2,D1,D0
	else 
		return inputCode; // i2c Error return 0xFFFF
}

// Desc :: get DAC inputCode from DAC register & convert to volts
// Returns : float : voltage or 0xFFFF if I2C error
float MCP4725::getVoltage()
{
	float InputCode = getInputCode();
	if (InputCode != MCP4725_ERROR) 
		return InputCode / _bitsPerVolt;
	else
		return InputCode; // i2c Error return 0xFFFF
}

// Desc :: Read DAC inputCode from EEPROM
// Returns : uint16_t : stored EEPROM value or 0xFFFF if I2C error
uint16_t MCP4725::getStoredInputCode()
{
	uint16_t inputCode = readRegister(MCP4725_ReadEEPROM); 
	//InputCode = x,PD1,PD0,x,D11,D10,D9,D8, D7,D6,D5,D4,D3,D2,D1,D0

	if (inputCode != MCP4725_ERROR) 
		return inputCode & 0x0FFF;  //0,0,0,0,D11,D10,D9,D8, D7,D6,D5,D4,D3,D2,D1,D0
	else													
		return inputCode; // i2c Error return 0xFFFF
}

// Desc :: Read stored DAC InputCode from EEPROM & convert to voltage
// Returns : float : stored EEPROM voltage  or 0xFFFF if I2C error
float MCP4725::getStoredVoltage()
{
	float InputCode = getStoredInputCode();

	if (InputCode != MCP4725_ERROR) 
		return InputCode / _bitsPerVolt;
	else
		return InputCode;
}


// Desc Return current power type from DAC register
// Returns : uint16_t : power type or 0xFFFF if I2C error
// Power type corresponds to enum MCP4725_PowerDownType_e
uint16_t MCP4725::getPowerType()
{
	uint16_t powerTypeValue = readRegister(MCP4725_ReadSettings); 
	//powerTypeValue = BSY,POR,xx,xx,xx,PD1,PD0,xx

	if (powerTypeValue != MCP4725_ERROR){
		powerTypeValue &= 0x0006;   //00,00,00,00,00,PD1,PD0,00
		return powerTypeValue >> 1; //00,00,00,00,00,00,PD1,PD0
	}else{
		return powerTypeValue;
	}
}

// Desc Return stored power type from EEPROM
// Returns : uint16_t : power type or 0xFFFF if I2C error
// Power type corresponds to enum MCP4725_PowerDownType_e
uint16_t MCP4725::getStoredPowerType()
{
	uint16_t powerTypeValue = readRegister(MCP4725_ReadEEPROM); 
	//powerTypeValue = x,PD1,PD0,xx,D11,D10,D9,D8,D7,D6,D5,D4,D3,D2,D1,D0

	if (powerTypeValue != MCP4725_ERROR)
	{
		powerTypeValue = powerTypeValue << 1;  //PD1,PD0,xx,D11,D10,D9,D8,D7,D6,D5,D4,D3,D2,D1,D0,00
		return  powerTypeValue >> 14; //00,00,00,00,00,00,00,00,00,00,00,00,00,00,PD1,PD0
	}else
	{
		return powerTypeValue;
	}
}

// Desc Return EEPROM writing status from DAC register
// Returns 1 for completed or 0( busy or I2C error)
// Note The BSY bit is low (during the EEPROM writing)
bool MCP4725::getEEPROMBusyFlag()
{
	uint16_t registerValue = readRegister(MCP4725_ReadSettings); 
	//register value = BSY,POR,xx,xx,xx,PD1,PD0,xx
	bool ReturnValue = false;
	if (registerValue != MCP4725_ERROR)
	{
		ReturnValue = ((registerValue >> 7) & 0x01);
		return ReturnValue; //1 - Not Busy, 0 - Busy
	}else
	{
		return ReturnValue; // I2C error
	}
}

// Desc Writes value to DAC register or EEPROM
// Parameter 1 : inputCode : uint16_t : 0 to MCP4725_MAX_VALUE voltage out
// Parameter 2 : mode :  enum  MCP4725_CmdType_e
//	see enum descriptions
// Parameter 3 : power type : enum  : MCP4725_PowerDownType_e
//	see enum descriptions
// return bool : output of writeCommand, true for success, false for failure
bool MCP4725::writeCommand(uint16_t inputCode, MCP4725_CmdType_e mode, MCP4725_PowerDownType_e powerType)
{
	uint8_t dataBuffer[3];
	uint8_t lowByte = 0;
	uint8_t highByte = 0;
	int ReturnCode = 0;

	switch (mode)
	{
		case MCP4725_FastMode:  
			//C2=0,C1=0,PD1,PD0,D11,D10,D9,D8,D7,D6,D5,D4,D3,D2,D1,D0
			lowByte  = (uint8_t)(inputCode & 0x00FF);
			highByte = (uint8_t)((inputCode >> 8) & 0x00FF);
			dataBuffer[0] = mode | (powerType << 4) | highByte; //C2,C1,PD1,PD0,D11,D10,D9,D8
  			dataBuffer[1] = lowByte;                            //D7,D6,D5,D4,D3,D2,D1,D0
			ReturnCode = i2c_write_timeout_us(_i2c, _i2cAddress, dataBuffer, 2 , false, MCP4725_I2C_DELAY);
			if (ReturnCode < 1)
			{
				if (_serialDebug == true)
				{
					printf("1203 : I2C error :: WriteCommand 1 \r\n");
					printf("Tranmission code : %d \r\n", ReturnCode );
					busy_wait_ms(100);
				}
			return false;
			}
			
			break;

		case MCP4725_RegisterMode: 
		//C2=0,C1=1,C0=0,x,x,PD1,PD0,x,D11,D10,D9,D8,D7,D6,D5,D4,D3,D2,D1,D0,x,x,x,x
		case MCP4725_EEPROM_Mode: 
		//C2=0,C1=1,C0=1,x,x,PD1,PD0,x,D11,D10,D9,D8,D7,D6,D5,D4,D3,D2,D1,D0,x,x,x,x	
			inputCode = inputCode << 4; //D11,D10,D9,D8,D7,D6,D5,D4,D3,D2,D1,D0,x,x,x,x
			lowByte  = (uint8_t)(inputCode & 0x00FF);
			highByte = (uint8_t)((inputCode >> 8) & 0x00FF);
			dataBuffer[0] = mode | (powerType << 1); // C2,C1,C0,x,x,PD1,PD0,x
  			dataBuffer[1] = highByte;               // D11,D10,D9,D8,D7,D6,D5,D4
			dataBuffer[2] = lowByte;                // D3,D2,D1,D0,x,x,x,x
			ReturnCode = i2c_write_timeout_us(_i2c, _i2cAddress, dataBuffer, 3 , false, MCP4725_I2C_DELAY);
			if (ReturnCode < 1)
			{
				if (_serialDebug == true)
				{
					printf("1204 : I2C error :: writeCommand 2 \r\n");
					printf("Tranmission code : %d \r\n", ReturnCode );
					busy_wait_ms(100);
				}
				return false;
			}
			break;
	}

	if (mode == MCP4725_EEPROM_Mode)
	{
		if (getEEPROMBusyFlag() == true)
			return true;
		busy_wait_ms(MCP4725_EEPROM_WRITE_TIME); //typical EEPROM write time 25 mSec
		if (getEEPROMBusyFlag() == true)
			 return true;
		busy_wait_ms(MCP4725_EEPROM_WRITE_TIME); //maximum EEPROM write time 25*2 mSec
	}

	return true;
}

// Desc Read DAC register
// Parameters: enum Datatype value 1 3 or 5
// Return Value: uint16_t : requested value of read or type 0XFFFF if I2c error
uint16_t MCP4725::readRegister(MCP4725_ReadType_e readType)
{
	uint16_t dataWord = readType;
	uint8_t dataBuffer[6];
	int ReturnCode = 0;

	/*Format of read data :
	== Settings data one byte
	BSY,POR,xx,xx,xx,PD1,PD0,xx, 
	== DAC register data 3 byte(1st 1 don't care)
	D11,D10,D9,D8,D7,D6,D5,D4, D3,D2,D1,D0,xx,xx,xx,xx, 
	== EEPROM data 5 byte (1st 3 don't care)
	xx,PD1,PD0,xx,D11,D10,D9,D8, D7,D6,D5,D4,D3,D2,D1,D0
	*/
	switch (readType)
	{
		case MCP4725_ReadSettings: // Read one byte settings
			ReturnCode = i2c_read_timeout_us(_i2c, _i2cAddress, dataBuffer, 1, false, MCP4725_I2C_DELAY);
			dataWord = dataBuffer[0];
			break;

		case MCP4725_ReadDACReg: // Read 3 bytes  DAC register data, skip first 1 don't care
			ReturnCode = i2c_read_timeout_us(_i2c, _i2cAddress, dataBuffer, 3, false, MCP4725_I2C_DELAY);
			dataWord = dataBuffer[1];
			dataWord = (dataWord << 8) | dataBuffer[2];
			break;

		case MCP4725_ReadEEPROM: // Read 5 bytes EEPROM data , first 3 don't care
			ReturnCode = i2c_read_timeout_us(_i2c, _i2cAddress, dataBuffer, 5, false, MCP4725_I2C_DELAY);
			dataWord = dataBuffer[3];
			dataWord = (dataWord << 8) | dataBuffer[4];
			break;
	}

	if (ReturnCode < 1)
	{ // no bytes read back from device or error issued
		if (_serialDebug == true)
		{
			printf("1205 I2C Error readRegister : \r\n");
			printf("Tranmission Code :: %d\r\n", ReturnCode);
			busy_wait_ms(100);
		}
	 	return MCP4725_ERROR;
 	 }else{
		return dataWord;
 	}
}

// Setter for _serialDebug flag
void MCP4725::setSerialDebugFlag(bool onOff){_serialDebug = onOff;}
// Getter for _serialDebug flag
bool MCP4725::getSerialDebugFlag(void){return _serialDebug;}
// Setter for _safetyCheck flag
void MCP4725::setSafetyCheckFlag(bool onOff){_safetyCheck = onOff;}
// Getter for _safetyCheck flag
bool MCP4725::getSafetyCheckFlag(){return _safetyCheck;}

// Desc :: General Call, name from datasheet section 7.3
// --Reset MCP4725 & upload data from EEPROM to DAC register
//   Immediately after reset event, uploads contents of EEPROM into the DAC reg.
// --Wake up & upload value from DAC register, 
//   Current power-down bits are set to normal, EEPROM power-down bit are not affected
// Parameters :: enum MCP4725_GeneralCallType_e tow inputs , reset OR wakup
// Return :: true on success, false on I2c error OR wrong input(GeneralCallAddress)
// Notes General Call command may affect other I2C slaves, sends out 0x00 on I2c Bus

bool MCP4725::GeneralCall(MCP4725_GeneralCallType_e typeCall){

	if (typeCall == MCP4725_GeneralCallAddress) {return false;}
	
	int ReturnCode = 0;
	uint8_t dataBuffer[1];
	
	dataBuffer[0] = (uint8_t)typeCall;
	// Note I2c address is MCP4725_GENERAL_CALL_ADDRESS
	ReturnCode = i2c_write_timeout_us(_i2c, (uint8_t)MCP4725_GeneralCallAddress, dataBuffer, 1 , false, MCP4725_I2C_DELAY);
	
	if (ReturnCode < 1)
	{ // no bytes read back from device or error issued
		if (_serialDebug == true)
		{
			printf("1206 I2C Error General Call : \r\n");
			printf("Tranmission Code :: %d\r\n", ReturnCode);
			busy_wait_ms(100);
		}
	 	return false;
 	 }else{
		return true;
 	}
}
// EOF