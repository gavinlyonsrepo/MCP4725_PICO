/*!
	@file     mcp4725.hpp
	@author   Gavin Lyons
	@brief    Library header file for MCP4725 PICO DAC library.

	@note  See URL for full details. https://github.com/gavinlyonsrepo/MCP4725_PICO
		
*/

#ifndef __MCP4725_DAC_H
#define __MCP4725_DAC_H

// Libraries
#include <stdio.h> // optional for printf debug error messages
#include "pico/stdlib.h"
#include "hardware/i2c.h"


/*!
	@brief Class for MCP4725_PIC0 DAC
*/
class MCP4725_PICO
{
 public:
// Section Enums

	/*! 8-bit i2c address. */
	enum MCP4725_I2C_Addr_e : uint8_t
	{
		MCP4725A0_Addr_A00 = 0x60, /**< MCP4725A0 with A0 = GND */
		MCP4725A0_Addr_A01 = 0x61, /**< MCP4725A0 with A0 = VCC */
		MCP4725A1_Addr_A00 = 0x62, /**< MCP4725A1 with A0 = GND */
		MCP4725A1_Addr_A01 = 0x63, /**< MCP4725A1 with A0 = VCC */
		MCP4725A2_Addr_A00 = 0x64, /**< MCP4725A2 with A0 = GND */
		MCP4725A2_Addr_A01 = 0x65  /**< MCP4725A2 with A0 = VCC */
	};

	/*! DAC register, command bits C2C1C0 */
	enum MCP4725_CmdType_e : uint8_t
	{
		MCP4725_FastMode     = 0x00, /**< Writes data to DAC register */
		MCP4725_RegisterMode = 0x40, /**< Writes data & config bits to DAC register */
		MCP4725_EEPROM_Mode  = 0x60  /**< Writes data & config bits to DAC register & EEPROM */
	};

	/*! DAC register, power down bits PD1 PD0 , BSY,POR,xx,xx,xx,PD1,PD0,xx */
	enum MCP4725_PowerDownType_e: uint8_t
	{
		MCP4725_PowerDown_Off     = 0x00, /**< Power down off draws 0.40mA no load & 0.29mA max load */
		MCP4725_PowerDown_1kOhm   = 0x01, /**< Power down on, with 1.0 kOhm to GND, draws ~60nA */
		MCP4725_PowerDown_100kOhm = 0x02, /**< Power down on, with 100 kOhm to GND */
		MCP4725_PowerDown_500kOhm = 0x03  /**< Power down on, with 500 kOhm to GND */
	}; 

	/*! DAC library read register type  */
	enum MCP4725_ReadType_e: uint8_t
	{
		MCP4725_ReadSettings = 1, /**< Read 1 byte,  Settings data  */
		MCP4725_ReadDACReg   = 3, /**< Read 3 bytes, DAC register data */
		MCP4725_ReadEEPROM   = 5  /**< Read 5 bytes, EEPROM data */
	};

	/*! DAC general call command datasheet 7.3 */
	enum MCP4725_GeneralCallType_e: uint8_t
	{
		MCP4725_GeneralCallAddress = 0x00, /**< General call address */ 
		MCP4725_GeneralCallReset  =  0x06, /**< General call reset command */
		MCP4725_GeneralCallWakeUp =  0x09  /**< General call wake-up command */
	}; 

	MCP4725_PICO(float refV = _MCP4725_REFERENCE_VOLTAGE);

	bool begin(MCP4725_I2C_Addr_e addr, i2c_inst_t* type, uint16_t speed, uint8_t SDA, uint8_t SCLK, uint32_t I2C_Timeout);
	bool isConnected();
	bool GeneralCall(MCP4725_GeneralCallType_e);
	void deinitI2C();

	void setReferenceVoltage(float value);
	float getReferenceVoltage(void);

	bool     setInputCode(uint16_t inputCode, MCP4725_CmdType_e = MCP4725_FastMode, MCP4725_PowerDownType_e = MCP4725_PowerDown_Off);
	uint16_t getInputCode(void);
	
	bool  setVoltage(float voltage, MCP4725_CmdType_e = MCP4725_FastMode, MCP4725_PowerDownType_e = MCP4725_PowerDown_Off);
	float getVoltage(void);

	uint16_t getStoredInputCode(void);
	float    getStoredVoltage(void);

	uint16_t getPowerType(void);
	uint16_t getStoredPowerType(void);

	void setSerialDebugFlag(bool onOff);
	bool getSerialDebugFlag(void);

	void setSafetyCheckFlag(bool onOff);
	bool getSafetyCheckFlag(void);

	uint16_t getLibraryVersionNumber(void);

	uint16_t getEEPROMwriteTime(void);
	void setEEPROMwriteTime(uint16_t );

 private:
	//I2c related 
	MCP4725_I2C_Addr_e _i2cAddress;
	i2c_inst_t *_i2c; // i2C port number, i2c0 or i2c1
	uint8_t _SDataPin;
	uint8_t _SClkPin;
	uint16_t _CLKSpeed = 100; // I2C bus speed in khz
	uint32_t _I2C_Delay  = 50000;  /**<  uS delay , I2C timeout */

	
	float    _refVoltage;
	uint16_t _bitsPerVolt;
	uint16_t  _eepromWriteTime = 25;  /**<  mSec Memory write time, maximum 50 mSec */

	/**< Constants for MCP4725 DAC voltage levels  */
	constexpr static float _MCP4725_REFERENCE_VOLTAGE = 3.3; /**<  default supply-reference Voltage in volts */
	constexpr static auto _MCP4725_RESOLUTION = 12; /**< Resolution in bits, 12-bit */
	constexpr static auto _MCP4725_STEPS = (1 << _MCP4725_RESOLUTION); /**< Quantity of DAC steps: 2^12 = 4096 */
	constexpr static auto _MCP4725_MAX_VALUE = _MCP4725_STEPS - 1; /**< Max value = 4096 - 1, range 0 to 4095 */

	bool _serialDebug = false; // Outputs Messages for debugging to serial port true = on
	bool _safetyCheck = true;  // Safety check for voltage level's , true  = on
	constexpr static uint16_t _MCP4725_ERROR = 0xFFFF; /**<  returns this value if I2C bus error from some methods */
	
	uint16_t _LibVersion = 101; /**< Library version number er 101 = 1.0.1 */

	bool     getEEPROMBusyFlag(void);
	bool     writeCommand(uint16_t value, MCP4725_CmdType_e mode, MCP4725_PowerDownType_e powerType);
	uint16_t readRegister(MCP4725_ReadType_e dataType);
};

#endif // library file header guard endif
