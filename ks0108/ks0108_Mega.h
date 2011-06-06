/*
  ks0108_Arduino.h - Arduino library support for ks0108 and compatable graphic LCDs
  Copyright (c)2008 Michael Margolis All right reserved

  This is the configuration file for mapping Arduino (ATmega168) pins to the ks0108 Graphics LCD library

*/

#ifndef	KS0108_CONFIG_H
#define KS0108_CONFIG_H

/*********************************************************/
/*  Configuration for assigning LCD bits to Arduino Pins */
/*********************************************************/
/* Arduino pins used for Commands - these must be within the range of 30-37
 */

#define CSEL1				33		// CS1 Bit   // swap pin assignments with CSEL2 if left/right image is reversed
#define CSEL2				34		// CS2 Bit
#define R_W					35		// R/W Bit
#define D_I					36		// D/I Bit 
#define EN					37		// EN Bit

#define LCD_CMD_PORT		PORTC		// Command Output Register for pins 30-37

/* This version uses pins 22-29 for LCD Data */

/*******************************************************/
/*     end of Arduino configuration                    */
/*******************************************************/


// these macros  map pins to ports using the defines above	
// the following should not be changed unless you really know what your doing 
#define LCD_DATA_LOW_NBL   A   // port for low nibble:  A=pins 22-25  
#define LCD_DATA_HIGH_NBL  A   // port for high nibble: A=pins 26-29 

// macros to fast write data to pins 30 - 37
#define fastWriteHigh(_pin) (PORTC |= 1   << ((7-(_pin -30)) & 0x07)) 
#define fastWriteLow(_pin)  (PORTC &= ~(1 << ((7-(_pin -30)) & 0x07)))


#endif
