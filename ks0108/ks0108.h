/*
  ks0108.h - Arduino library support for ks0108 and compatable graphic LCDs
  Copyright (c)2008 Michael Margolis All right reserved
  mailto:memargolis@hotmail.com?subject=KS0108_Library 

  The high level functions of this library are based on version 1.1 of ks0108 graphics routines
  written and copyright by Fabian Maximilian Thiele. His sitelink is dead but
  you can obtain a copy of his original work here:
  http://www.scienceprog.com/wp-content/uploads/2007/07/glcd_ks0108.zip

  Code changes include conversion to an Arduino C++ library, rewriting the low level routines 
  to read busy status flag and support a wider range of displays, adding more flexibility
  in port addressing and improvements in I/O speed. The interface has been made more Arduino friendly
  and some convenience functions added. 

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 

  Version:   1.0  - May  8 2008  - initial release
  Version:   1.0a - Sep  1 2008  - simplified command pin defines  
  Version:   1.0b - Sep 18 2008  - replaced <wiring.h> with boolean typedef for rel 0012  
  Version:   1.1  - Nov  7 2008  - restructured low level code to adapt to panel speed
                                 - moved chip and panel configuration into seperate header files    
							     - added fixed width system font
  Version:   2   - May 26 2009   - second release
                                 - added support for Mega and Sanguino, improved panel speed tolerance, added bitmap support
     
*/

#include <inttypes.h>
typedef uint8_t boolean;
typedef uint8_t byte;
#include <avr/pgmspace.h>

#ifndef	KS0108_H
#define KS0108_H

#define GLCD_VERSION 2 // software version of this library

// Chip specific includes
#if defined(__AVR_ATmega1280__)
#include "ks0108_Mega.h"  // include this for the Arduino Mega other ATmega1280 boards
#elif defined (__AVR_ATmega644__)  // TODO - check this define 
#include "ks0108_Sanguino.h"   // include this for Sanguino or ATmega644 boards
#else
#include "ks0108_Arduino.h"  // include this for the Arduino or other ATmega168 boards
#endif

#include "ks0108_Panel.h"      // this contains LCD panel specific configuration


// macros for pasting port defines
#define GLUE(a, b)     a##b 
#define PORT(x)        GLUE(PORT, x)
#define PIN(x)         GLUE(PIN, x)
#define DDR(x)         GLUE(DDR, x)

// paste together the port definitions if using nibbles
#define LCD_DATA_IN_LOW		PIN(LCD_DATA_LOW_NBL)	// Data I/O Register, low nibble
#define LCD_DATA_OUT_LOW	PORT(LCD_DATA_LOW_NBL)  // Data Output Register - low nibble
#define LCD_DATA_DIR_LOW	DDR(LCD_DATA_LOW_NBL)	// Data Direction Register for Data Port, low nibble

#define LCD_DATA_IN_HIGH	PIN(LCD_DATA_HIGH_NBL)	// Data Input Register  high nibble
#define LCD_DATA_OUT_HIGH	PORT(LCD_DATA_HIGH_NBL)	// Data Output Register - high nibble
#define LCD_DATA_DIR_HIGH	DDR(LCD_DATA_HIGH_NBL)	// Data Direction Register for Data Port, high nibble

#define lcdDataOut(_val_) LCD_DATA_OUT(_val_) 
#define lcdDataDir(_val_) LCD_DATA_DIR(_val_) 

// macros to handle data output
#ifdef LCD_DATA_NIBBLES  // data is split over two ports 
#define LCD_DATA_OUT(_val_) \
    LCD_DATA_OUT_LOW =  (LCD_DATA_OUT_LOW & 0xF0)| (_val_ & 0x0F); LCD_DATA_OUT_HIGH = (LCD_DATA_OUT_HIGH & 0x0F)| (_val_ & 0xF0); 

#define LCD_DATA_DIR(_val_)\
    LCD_DATA_DIR_LOW =  (LCD_DATA_DIR_LOW & 0xF0)| (_val_ & 0x0F); LCD_DATA_DIR_HIGH = (LCD_DATA_DIR_HIGH & 0x0F)| (_val_ & 0xF0);
#else  // all data on same port (low equals high)
#define LCD_DATA_OUT(_val_) LCD_DATA_OUT_LOW = (_val_);		
#define LCD_DATA_DIR(_val_) LCD_DATA_DIR_LOW = (_val_);
#endif


// Commands
#ifdef HD44102 
#define LCD_ON				0x39
#define LCD_OFF				0x38
#define LCD_DISP_START		0x3E   // Display start page 0
#else
#define LCD_ON				0x3F
#define LCD_OFF				0x3E
#define LCD_DISP_START		0xC0
#endif

#define LCD_SET_ADD			0x40
#define LCD_SET_PAGE		0xB8

#define LCD_BUSY_FLAG		0x80 

// Colors
#define BLACK				0xFF
#define WHITE				0x00

// useful user contants
#define NON_INVERTED false
#define INVERTED     true

// Font Indices
#define FONT_LENGTH			0
#define FONT_FIXED_WIDTH	2
#define FONT_HEIGHT			3
#define FONT_FIRST_CHAR		4
#define FONT_CHAR_COUNT		5
#define FONT_WIDTH_TABLE	6


// Uncomment for slow drawing
// #define DEBUG

typedef struct {
	uint8_t x;
	uint8_t y;
	uint8_t page;
} lcdCoord;

typedef uint8_t (*FontCallback)(const uint8_t*);

uint8_t ReadPgmData(const uint8_t* ptr);	//Standard Read Callback

#define DrawVertLine(x, y, length, color) FillRect(x, y, 0, length, color)
#define DrawHoriLine(x, y, length, color) FillRect(x, y, length, 0, color)
#define DrawCircle(xCenter, yCenter, radius, color) DrawRoundRect(xCenter-radius, yCenter-radius, 2*radius, 2*radius, radius, color)
#define ClearScreenX() FillRect(0, 0, (DISPLAY_WIDTH-1), (DISPLAY_HEIGHT-1), WHITE)
#define ClearSysTextLine(_line) FillRect(0, (line*8), (DISPLAY_WIDTH-1), ((line*8)+ 7), WHITE )

class ks0108  // shell class for ks0108 glcd code
{
  private:
    lcdCoord			Coord;
    boolean				Inverted; 
    FontCallback	    FontRead;
    uint8_t				FontColor;
    const uint8_t*		Font;
	uint8_t ReadData(void); 
	uint8_t DoReadData(uint8_t first);
	void WriteCommand(uint8_t cmd, uint8_t chip);
	void WriteData(uint8_t data); // experts can make this public but the functionality is not documented
	inline void Enable(void);
	inline void SelectChip(uint8_t chip); 
	void WaitReady( uint8_t chip);
  public:
    ks0108();
	// Control functions
	void Init(boolean invert);
    void GotoXY(uint8_t x, uint8_t y);
	// Graphic Functions
	void ClearPage(uint8_t page, uint8_t color);
	void ClearScreen(uint8_t color = WHITE);
    void DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t color);
    void DrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
	void DrawRoundRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t radius, uint8_t color);
    void FillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
    void InvertRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
    void SetInverted(boolean invert);
    void SetDot(uint8_t x, uint8_t y, uint8_t color);
	void DrawBitmap(const uint8_t * bitmap, uint8_t x, uint8_t y, uint8_t color);
	
    // Font Functions
    void SelectFont(const uint8_t* font, uint8_t color=BLACK, FontCallback callback=ReadPgmData); // defualt arguments added, callback now last arg
    int PutChar(char c);
    void Puts(char* str);
    void Puts_P(PGM_P str);
	void PrintNumber(long n);
	void CursorTo( uint8_t x, uint8_t y); // 0 based coordinates for fixed width fonts (i.e. systemFont5x7)

    uint8_t CharWidth(char c);
    uint16_t StringWidth(char* str);
    uint16_t StringWidth_P(PGM_P str);
};

extern ks0108 GLCD;    
#endif
