[![Website](https://img.shields.io/badge/Website-Link-blue.svg)](https://gavinlyonsrepo.github.io/)  [![Rss](https://img.shields.io/badge/Subscribe-RSS-yellow.svg)](https://gavinlyonsrepo.github.io//feed.xml)  [![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)


# MCP4725_PICO

![image](https://github.com/gavinlyonsrepo/MCP4725_PICO/blob/main/extra/images/device.jpg)


Table of contents
---------------------------

  * [Overview](#overview)
  * [Examples](#examples)
  * [Software](#software)  
  * [Hardware](#hardware)
  * [Output](#output)

Overview
-----------------
* Name: MCP4725_PICO
* Description:

Library Driver for  DAC  sensor,  MCP4725 modules,
for Raspberry pi PICO RP2040. 

MCP4725 I2C , 12-Bit Digital-to-Analog Converter with EEPROM Memory.
The MCP4725 is a low-power, high accuracy, single
channel, 12-bit buffered voltage output Digital-to-
Analog Converter (DAC) with nonvolatile memory
(EEPROM). Its on-board precision output amplifier
allows it to achieve rail-to-rail analog output swing.

* Toolchain
	1. Raspberry pi PICO RP2040
	2. SDK C++, compiler G++ for arm-none-eabi
	3. CMAKE , VScode

* By adjusting the value of External A0 address pin the I2C address can be changed allowing 2 of these devices on same I2C bus. With The PICO's two I2C buses this allows a total of 4 devices to be connected to PICO.


Examples
---------------------
There are eight example files.
The example files are in example folder. To build the one you want, edit the Cmaketxt file add_executable(${PROJECT_NAME} section, comment in one example file path and one ONLY. 


| Path Name | Function |
| --- | --- |
| isConnected | Check I2C connection over and over  |
| demoTest | Carries out Test on various features, EEPROM, power mode's etc |
| setVoltage | Carries out Tests on  voltage and input code methods |
| squareWave | Generates a square waveform  100 Hz  |
| triangleWave | Generates a triangle waveform  75 Hz  |
| sawToothWave | Generates a saw tooth waveform 150 Hz   |
| sineWave | Generates  a sinusoidal waveform at various values, see file header |
| random | Random data generation |

The example files output Data to the PC serial port using printf,  default settings.(baud rate 38400). 

Software
---------------------
TODO

Hardware
---------------------
TODO

Output
--------------------

Saw tooth waveform.
![image](https://github.com/gavinlyonsrepo/MCP4725_PICO/blob/main/extra/images/sawtooth.jpg)

Sine waveform.
![image](https://github.com/gavinlyonsrepo/MCP4725_PICO/blob/main/extra/images/sine32.jpg)
