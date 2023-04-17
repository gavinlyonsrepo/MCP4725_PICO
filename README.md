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

* Supports MCP4725 features including EEPROM r/w, Power modes and general call. 

* Toolchain
	1. Raspberry pi PICO RP2040
	2. SDK C++, compiler G++ for arm-none-eabi
	3. CMAKE , VScode


Examples
---------------------
There are eight example files.
The example files are in example folder. To build the one you want, edit the Cmaketxt file add_executable(${PROJECT_NAME} section, comment in one example file path and one ONLY. 


| Path Name | Function |
| --- | --- |
| isConnected | Check I2C connection continuously, useful for debugging I2C bus issues |
| demoTest | Carries out Test on various features, EEPROM, power mode's etc |
| setVoltage | Carries out Tests on voltage and input code methods |
| squareWave | Generates a square waveform  100 Hz  |
| triangleWave | Generates a triangle waveform  75 Hz  |
| sawToothWave | Generates a saw tooth waveform 150 Hz   |
| sineWave | Generates  a sinusoidal waveform at various values, see file header |
| random | Random data generation |

The example files output Data to the PC serial port using printf,  default settings.(baud rate 38400). 

Software
---------------------
**Constructor**
The constructor has one parameter. The reference  voltage of the DAC.

**I2C settings**

In the begin method the user can pass in a number of I2C related arguments.
In the header file it is also possible to turn on I2C debugging messages(_serialDebug flag) and
adjust the timeout of the I2C functions if necessary.

| Function | Default |
| --- |  --- | 
| I2C address 8-bit,  0x6? 6 possible values see hardware section and enum  | 0x60 |
| I2C instance of port IC20 or I2C1 | I2C1 |
| I2C Clk speed mode ikn  kbits, 100, 400 or 3400(not tested) | 100 |
| I2C data line | GPIO 18 |
| I2C clock Line | GPIO 19 |


Hardware
---------------------

**Pinout**

| Pin | Function | 
| --- | --- |
| OUT | Analog Voltage output of DAC | 
| GND | Ground Reference |
| SDA |Digital I/O Serial data. Transmits and receives data |
| SCL | Digital input Serial clock input. Clocks data on SDA |
| GND | Ground |
| VDD  | Reference + supply voltage 2.7v - 5.5v,  In PICO case connect to 3.3v |
| A0 | I2C Address Bit Selection pin (A0 bit)^^ |

^^This pin can be tied to VSS or VDD,
or can be actively driven by the digital logic levels. The logic state of this
pin determines what the A0 bit of the I2C address bits should be. 
The address is also effected  which type of chip. There are 3 variant's thus
6 choice's of I2C address.

The module tested has 2 X 4.7Kohm  pull up resistors on I2C bus and a bypass capacitor.
An additional 10 µF capacitor (tantalum) in parallel is also recommended to
further attenuate high frequency noise.

Vpor(Power-On-Reset (POR)) = 2v. 
If Vdd fails below 2v, all circuits & no DAC output disabled, 
when Vdd increases above Vpor device "resets" and gets data from EEPROM(stored voltage and power on conditions)


Output
--------------------

Saw tooth waveform.
![image](https://github.com/gavinlyonsrepo/MCP4725_PICO/blob/main/extra/images/sawtooth.jpg)

Sine waveform.
![image](https://github.com/gavinlyonsrepo/MCP4725_PICO/blob/main/extra/images/sine32.jpg)
