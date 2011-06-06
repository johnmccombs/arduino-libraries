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
/* Arduino pins used for Commands
 * default assignment uses the first five analog pins
 */

#define CSEL1				14		// CS1 Bit   // swap pin assignments with CSEL2 if left/right image is reversed
#define CSEL2				15		// CS2 Bit

#define R_W					16		// R/W Bit
#define D_I					17		// D/I Bit 
#define EN					18		// EN Bit
//#define RES 				19		// Reset Bit  // uncomment this to contol LCD reset on this pin

/* option: uncomment the next line if all command pins are on the same port for slight speed & code size improvement */
#define LCD_CMD_PORT		PORTC		// Command Output Register for pins 14-19

/* Arduino pins used for LCD Data 
 * un-comment ONE of the following pin options that corresponds to the wiring of data bits 0-3 
 */
#define dataPins8to11   // bits 0-3 assigned to arduino pins 8-11, bits 4-7 assigned to arduino pins 4-7
//#define dataPins14to17 //bits 0-3 assigned to arduino pins 14-17, bits 4-7 assigned to arduino pins 4-7. (note command pins must be changed)
//#define dataPins0to3  // bits 0-3 assigned to arduino pins 0-3 , bits 4-7 assigned to arduino pins 4-7, this is marginally  the fastest option but  its only available on runtime board without hardware rs232.

/* NOTE: all above options assume LCD data bits 4-7 are connected to arduino pins 4-7 */

/*******************************************************/
/*     end of Arduino configuration                    */
/*******************************************************/

#ifndef dataPins0to3                     // this is the only option on standard arduino where all data bits are on same port 
#define LCD_DATA_NIBBLES                // if this is defined then data i/o is split into two operations
#endif 

// these macros  map pins to ports using the defines above
// the following should not be changed unless you really know what your doing 
#ifdef dataPins0to3
#define LCD_DATA_LOW_NBL   D   // port for low nibble: D=pins 0-3  
#endif
#ifdef dataPins14to17 
#define LCD_DATA_LOW_NBL   C   // port for low nibble: C=pins 14-17 (using this requires reasignment of command pins) 
#endif
#ifdef dataPins8to11            // the following is the defualt setting
#define LCD_DATA_LOW_NBL   B   // port for low nibble, B=pins 8-11
#endif

#define LCD_DATA_HIGH_NBL  D   // port for high nibble: D=pins 4-7, B & C not available on std arduino  

// macros to fast write data to pins known at compile time, this is over 30 times faster than digitalWrite
#define fastWriteHigh(_pin_) ( _pin_ < 8 ?  PORTD |= 1 << (_pin_ & 0x07) : ( _pin_ < 14 ?  PORTB |= 1 << ((_pin_ -8) & 0x07) : PORTC |= 1 << ((_pin_ -14) & 0x07)  ) ) 
#define fastWriteLow(_pin_) ( _pin_ < 8 ?   PORTD &= ~(1 << (_pin_  & 0x07)) : ( _pin_ < 14 ?  PORTB &= ~(1 << ((_pin_ -8) & 0x07) )  :  PORTC &= ~(1 << ((_pin_ -14) & 0x07) )  ) )


#endif
