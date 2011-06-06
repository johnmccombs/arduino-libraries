/*
 Basic LoL Shield Test
 
 Writen for the LoL Shield, designed by Jimmie Rodgers:
 http://jimmieprodgers.com/kits/lolshield/
 
 This needs the Charliplexing library, which you can get at the
 LoL Shield project page: http://code.google.com/p/lolshield/
 
 Created by Jimmie Rodgers on 12/30/2009.
 Adapted from: http://www.arduino.cc/playground/Code/BitMath
 
 History:
  	December 30, 2009 - V1.0 first version written at 26C3/Berlin

  This is free software; you can redistribute it and/or
  modify it under the terms of the GNU Version 3 General Public
  License as published by the Free Software Foundation; 
  or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <avr/pgmspace.h>  //AVR library for writing to ROM
#include <Charliplexing.h> //Imports the library, which needs to be
                           //Initialized in setup.

int blinkdelay = 100;      //Sets the time each frame is shown

/*
The BitMap array is what contains the frame data. Each line is one full frame.
Since each number is 16 bits, we can easily fit all 14 LEDs per row into it.
The number is calculated by adding up all the bits, starting with lowest on
the left of each row. 18000 was chosen as the kill number, so make sure that
is at the end of the matrix, or the program will continue to read into memory.

Here PROGMEM is called, which stores the array into ROM, which leaves us
with our RAM. You cannot change the array during run-time, only when you
upload to the Arduino. You will need to pull it out of ROM, which is covered
below. If you want it to stay in RAM, just delete PROGMEM
*/
uint16_t BitMap[][9] PROGMEM = {
//Diaganal swipe across the screen
{1, 0, 0, 0, 0, 0, 0, 0, 0},
{3, 1, 0, 0, 0, 0, 0, 0, 0},
{7, 3, 1, 0, 0, 0, 0, 0, 0},
{15, 7, 3, 1, 0, 0, 0, 0, 0},
{31, 15, 7, 3, 1, 0, 0, 0, 0},
{63, 31, 15, 7, 3, 1, 0, 0, 0},
{127, 63, 31, 15, 7, 3, 1, 0, 0},
{255, 127, 63, 31, 15, 7, 3, 1, 0},
{511, 255, 127, 63, 31, 15, 7, 3, 1},
{1023, 511, 255, 127, 63, 31, 15, 7, 3},
{2047, 1023, 511, 255, 127, 63, 31, 15, 7},
{4095, 2047, 1023, 511, 255, 127, 63, 31, 15},
{8191, 4095, 2047, 1023, 511, 255, 127, 63, 31},
{16383, 8191, 4095, 2047, 1023, 511, 255, 127, 63},
{16383, 16383, 8191, 4095, 2047, 1023, 511, 255, 127},
{16383, 16383, 16383, 8191, 4095, 2047, 1023, 511, 255},
{16383, 16383, 16383, 16383, 8191, 4095, 2047, 1023, 511},
{16383, 16383, 16383, 16383, 16383, 8191, 4095, 2047, 1023},
{16383, 16383, 16383, 16383, 16383, 16383, 8191, 4095, 2047},
{16383, 16383, 16383, 16383, 16383, 16383, 16383, 8191, 4095},
{16383, 16383, 16383, 16383, 16383, 16383, 16383, 16383, 8191},
{16383, 16383, 16383, 16383, 16383, 16383, 16383, 16383, 16383},
{16383, 16383, 16383, 16383, 16383, 16383, 16383, 16383, 16383},
{16382, 16383, 16383, 16383, 16383, 16383, 16383, 16383, 16383},
{16380, 16382, 16383, 16383, 16383, 16383, 16383, 16383, 16383},
{16376, 16380, 16382, 16383, 16383, 16383, 16383, 16383, 16383},
{16368, 16376, 16380, 16382, 16383, 16383, 16383, 16383, 16383},
{16352, 16368, 16376, 16380, 16382, 16383, 16383, 16383, 16383},
{16320, 16352, 16368, 16376, 16380, 16382, 16383, 16383, 16383},
{16256, 16320, 16352, 16368, 16376, 16380, 16382, 16383, 16383},
{16128, 16256, 16320, 16352, 16368, 16376, 16380, 16382, 16383},
{15872, 16128, 16256, 16320, 16352, 16368, 16376, 16380, 16382},
{15360, 15872, 16128, 16256, 16320, 16352, 16368, 16376, 16380},
{14336, 15360, 15872, 16128, 16256, 16320, 16352, 16368, 16376},
{12288, 14336, 15360, 15872, 16128, 16256, 16320, 16352, 16368},
{8192, 12288, 14336, 15360, 15872, 16128, 16256, 16320, 16352},
{0, 8192, 12288, 14336, 15360, 15872, 16128, 16256, 16320},
{0, 0, 8192, 12288, 14336, 15360, 15872, 16128, 16256},
{0, 0, 0, 8192, 12288, 14336, 15360, 15872, 16128},
{0, 0, 0, 0, 8192, 12288, 14336, 15360, 15872},
{0, 0, 0, 0, 0, 8192, 12288, 14336, 15360},
{0, 0, 0, 0, 0, 0, 8192, 12288, 14336},
{0, 0, 0, 0, 0, 0, 0, 8192, 12288},
{0, 0, 0, 0, 0, 0, 0, 0, 8192},
{0, 0, 0, 0, 0, 0, 0, 0, 0}, 

//Horizontal swipe
{1, 1, 1, 1, 1, 1, 1, 1, 1} ,
{3, 3, 3, 3, 3, 3, 3, 3, 3},
{7, 7, 7, 7, 7, 7, 7, 7, 7},
{15, 15, 15, 15, 15, 15, 15, 15, 15},
{31, 31, 31, 31, 31, 31, 31, 31, 31},
{63, 63, 63, 63, 63, 63, 63, 63, 63},
{127, 127, 127, 127, 127, 127, 127, 127, 127},
{255, 255, 255, 255, 255, 255, 255, 255, 255},
{511, 511, 511, 511, 511, 511, 511, 511, 511},
{1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023},
{2047, 2047, 2047, 2047, 2047, 2047, 2047, 2047, 2047},
{4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095, 4095},
{8191, 8191, 8191, 8191, 8191, 8191, 8191, 8191, 8191},
{16383, 16383, 16383, 16383, 16383, 16383, 16383, 16383, 16383},
{16382, 16382, 16382, 16382, 16382, 16382, 16382, 16382, 16382},
{16380, 16380, 16380, 16380, 16380, 16380, 16380, 16380, 16380},
{16376, 16376, 16376, 16376, 16376, 16376, 16376, 16376, 16376},
{16368, 16368, 16368, 16368, 16368, 16368, 16368, 16368, 16368},
{16352, 16352, 16352, 16352, 16352, 16352, 16352, 16352, 16352},
{16320, 16320, 16320, 16320, 16320, 16320, 16320, 16320, 16320},
{16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256, 16256},
{16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128, 16128},
{15872, 15872, 15872, 15872, 15872, 15872, 15872, 15872, 15872},
{15360, 15360, 15360, 15360, 15360, 15360, 15360, 15360, 15360},
{14336, 14336, 14336, 14336, 14336, 14336, 14336, 14336, 14336},
{12288, 12288, 12288, 12288, 12288, 12288, 12288, 12288, 12288},
{8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192, 8192},
{0, 0, 0, 0, 0, 0, 0, 0, 0}, 
{18000}
};

void setup() {
  LedSign::Init();  //Initializes the screen
}
void loop() {
  DisplayBitMap();  //Displays the bitmap
}

void DisplayBitMap()
{
  boolean run=true;    //While this is true, the screen updates
  byte frame = 0;      //Frame counter
  byte line = 0;       //Row counter
  unsigned long data;  //Temporary storage of the row data

  while(run == true) {
    for(line = 0; line < 9; line++) {

      //Here we fetch data from program memory with a pointer.
      data = pgm_read_word_near (&BitMap[frame][line]);
      
      //Kills the loop if the kill number is found
      if (data==18000){
        run=false;
      }
      
      //This is where the bit-shifting happens to pull out
      //each LED from a row. If the bit is 1, then the LED
      //is turned on, otherwise it is turned off.
      else for (byte led=0; led<14; ++led) {
        if (data & (1<<led)) {
          LedSign::Set(led, line, 1);
        }
        else {
          LedSign::Set(led, line, 0);
        }
      }

    }
    
    //Delays the next update
    delay(blinkdelay);        
    frame++;  
  }
}
