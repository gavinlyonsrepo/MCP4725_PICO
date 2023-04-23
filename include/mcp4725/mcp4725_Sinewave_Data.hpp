/*!
	@file     mcp4725_Sinewave_Data.hpp
	@author   Gavin Lyons
	@brief    Data header file for MCP4725 Sinewave generation. 
*/

#ifndef __MCP4725_SINEWAVE_H
#define __MCP4725_SINEWAVE_H

// Libraries
#include <stdint.h>

/*!
 Resolution of Sinewave in bits (2^bits)
 Select 512, 256, 128, 64 or 32 points table
 i.e. 2^9 =  512 , 9 bit resolution etc
*/
#define SINEWAVE_RES 128  

extern const uint16_t * pDacLookupSineWave; /**< Pointer to data which is in cpp file */

#endif // library file header guard endif
