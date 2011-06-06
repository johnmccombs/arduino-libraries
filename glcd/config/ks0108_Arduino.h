/*
 * ks0108_Arduino.h - User specific configuration for Arduino GLCD library
 *
 * Use this file to set io pins
 * This version is for a standard ks0108 display
 * connected using the default Arduino wiring
 *
*/

#ifndef GLCD_PIN_CONFIG_H
#define GLCD_PIN_CONFIG_H

/*
 * define name for pin configuration
 */
#define glcd_PinConfigName "ks0108-Arduino"

/*********************************************************/
/*  Configuration for assigning LCD bits to Arduino Pins */
/*********************************************************/
/* Data pin definitions
 */

// alternate wiring starting from pin 2
#define glcdData0Pin        6
#define glcdData1Pin        7
#define glcdData2Pin        8
#define glcdData3Pin        9
#define glcdData4Pin        2
#define glcdData5Pin        3
#define glcdData6Pin        4
#define glcdData7Pin        5


#ifdef junk

#define glcdData0Pin        8
#define glcdData1Pin        9
#define glcdData2Pin        10
#define glcdData3Pin        11
#define glcdData4Pin        4
#define glcdData5Pin        5
#define glcdData6Pin        6
#define glcdData7Pin        7

#endif


/* Arduino pins used for Commands
 * default assignment uses the first five analog pins
 */

#define glcdCSEL1        14
#define glcdCSEL2        15

#if NBR_CHIP_SELECT_PINS > 2
#define glcdCSEL3         3   // third chip select if needed
#endif

#if NBR_CHIP_SELECT_PINS > 3
#define glcdCSEL4         2   // fourth chip select if needed
#endif

#define glcdRW           16
#define glcdDI           17
#define glcdEN           18
// Reset Bit  - uncomment the next line if reset is connected to an output pin
//#define glcdRES          19    // Reset Bit

#endif //GLCD_PIN_CONFIG_H
