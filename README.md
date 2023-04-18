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

Library Driver for  MCP4725 DAC modules,
for Raspberry pi PICO RP2040. 

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

The constructor has one parameter. The reference voltage of the DAC(Vref)
This setting can be read or changed later on.

**Begin method & I2C**

In the begin method the user can pass in a number of I2C related arguments.
In the header file it is also possible to turn on I2C debugging messages(_serialDebug flag) and
adjust the timeout of the I2C functions if necessary.

| Function | Default |
| --- |  --- | 
| I2C address 8-bit,  0x6? 6 possible values see hardware section and enum  | 0x60 |
| I2C instance of port IC20 or I2C1 | I2C1 |
| I2C Clk speed mode in  kbits, 100, 400 or 3400(not tested) | 100 |
| I2C data line | GPIO 18 |
| I2C clock Line | GPIO 19 |

**Set Voltage Input code**
The user can set the Voltage output using a voltage or an input code.
The input code is calculated using following. Example :: 

0. Required voltage output of 1.65 V
1. Voltage reference of 3.3 Volts 
2. DAC resolution of 2^12 (4096) 
3. The Din or input code would be  2048.

![image](https://github.com/gavinlyonsrepo/MCP4725_PICO/blob/main/extra/images/inputcode.jpg)

**EEPROM**

The Device can store a voltage output value and power mode status in its onboard EEPROM.
In the event of a software reset or power loss, The device will read EEPROM and restore these settings. If the power supply voltage is less than the POR(Power-On-Reset) thresh-
old (VPOR = 2V, typical), all circuits are disabled and there will be no DAC output. . 
When Vdd increases above Vpor device "resets" and gets data from EEPROM(stored voltage and power on conditions).

![image](https://github.com/gavinlyonsrepo/MCP4725_PICO/blob/main/extra/images/eeprom.jpg)

**Power modes**

The device has 4  modes. Normal and 3 power down modes. During the power-down mode, the device draws about 60 nA (typical).

![image](https://github.com/gavinlyonsrepo/MCP4725_PICO/blob/main/extra/images/powerdown.jpg)

**General call method**

The general call address (0x00) is for addressing every device connected to the I2C-bus at the same time. This command may potentially cause issues with other slave devices on the I2C bus.
The MCP4725 has two possible arguments here , Reset and wakeup. Reset is a software reset and will result in EEPROM values being set. Wakeup is to exit a power down mode and return to normal.

![image](https://github.com/gavinlyonsrepo/MCP4725_PICO/blob/main/extra/images/call.jpg)

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
The address is also effected by which variant of chip. There are 3 variant's thus
6 choice's of I2C address

![image](https://github.com/gavinlyonsrepo/MCP4725_PICO/blob/main/extra/images/hardware.jpg)

The module tested has 2 X 4.7 kOhm  pull up resistors on I2C bus and a bypass capacitor.
An additional 10 µF capacitor (tantalum) in parallel is also recommended to
further attenuate high frequency noise.


Output
--------------------

Saw tooth waveform.
![image](https://github.com/gavinlyonsrepo/MCP4725_PICO/blob/main/extra/images/sawtooth.jpg)

Sine waveform.
![image](https://github.com/gavinlyonsrepo/MCP4725_PICO/blob/main/extra/images/sine32.jpg)
