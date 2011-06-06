Installation
--------------------------------------------------------------------------------

To install this library, just place this entire folder as a subfolder in your
Arduino hardware/libraries folder.

When installed, this library should look like:

Arduino/hardware/libraries/ks0108                                (this library's folder)
Arduino/hardware/libraries/ks0108/ks0108.cpp             (the library implementation file)
Arduino/hardware/libraries/ks0108/ks0108.h                 (the library header file)
Arduino/hardware/libraries/ks0108/ks0108_Panel.h       (the Panel configuration header)
Arduino/hardware/libraries/ks0108/ks0108_Arduino.h   (header used to define pins for Atmega168/328) 
Arduino/hardware/libraries/ks0108/ks0108_Mega.h      (header used to define pins for Mega) 
Arduino/hardware/libraries/ks0108/ks0108_Sanguino.h (header used to define pins for Sanguino) - untested
Arduino/hardware/libraries/ks0108/SystemFont5x7.h     (definition for 5x7 system fonmt)
Arduino/hardware/libraries/ks0108/Arial14.h                 (the definition for 14 point Arial Font)
Arduino/hardware/libraries/ks0108/keywords.txt           (the syntax coloring file)
Arduino/hardware/libraries/ks0108/examples                 (diectory containing the example test sketch)
Arduino/hardware/libraries/ks0108/readme.txt               (this file)

Building
--------------------------------------------------------------------------------

After the files are copied into your library directory, check the wiring matches the pin configuration
for your board. The defualt wiring is different for each of the three chip types:
(see http://www.arduino.cc/playground/Code/GLCDks0108 for more info)

 
ATmega168/328  uses configuration file: ks0108_Arduino.h 
LCD                     ARDUINO
 CSEL1		14		// CS1 Bit   // swap with CSEL2 if left/right is reversed
 CSEL2		15		// CS2 Bit
 R_W		16		// R/W Bit
 D_I		17		// D/I Bit 
 EN		18		// EN Bit

 LCD data 0-3             8-11 
 LCD data 4-7             4-7

Mega (ATmega1280)  uses configuration file: ks0108_Mega.h 
 
 CSEL1		33		// CS1 Bit   // swap with CSEL2 if left/right is reversed
 CSEL2		34		// CS2 Bit
 R_W		35		// R/W Bit
 D_I		36		// D/I Bit 
 EN		37		// EN Bit

 LCD data 0-7             22-29 

Sanguino (ATmega644) uses configuration file: ks0108_Sanguino.h
CSEL1		24 		// CS1 Bit   // swap pin assignments with CSEL2 if left/right image is reversed
CSEL2		25 		// CS2 Bit
 R_W		26 		// R/W Bit
 D_I		27 		// D/I Bit 
 EN		28 		// EN Bit

 LCD data 0-7             16-23 

Slow panels may need delays added to the panel panel configuraion file: ks0108_Panel.h 
change the number 6 to a higher number to make th delay longer :
#define EN_DELAY_VALUE 6 // this is the delay value that may need to be hand tuned for slow panels
If your panel has a different width or height you can change these two constants in this file:
  #define DISPLAY_WIDTH 128
  #define DISPLAY_HEIGHT 64

After a successful build of this library, a new file named 'ks0108.o' will appear
in ks0108 library directory. If you make any changes to any of the files in the
ks0108 directory you must delete ks0108.o before recompiling your sketch for the changes to be
recognized by your sketch. The new "Test.o" with your code will appear after the next
verify or compile (If there are no syntax errors in the changed code).

To use the library in a sketch, go to the Sketch | Import Library menu and
select ks0108.  This will add a corresponding line to the top of your sketch:
#include <ks0108.h>. It will also add lines for all the font definitions you have
in the ks0108 library directory. You should remove the includes for any fonts you
don't use in a sketch, they use a lot of program memory.

