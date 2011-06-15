/*
 * This sketch demonstrates use of SdFile::rename() 
 * and SdFat::rename().
 */
#include <SdFat.h>

// SD chip select pin
const uint8_t chipSelect = SS_PIN;

SdFat sd;

//------------------------------------------------------------------------------
// store error strings in flash to save RAM
#define error(s) sd.errorHalt_P(PSTR(s))
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  Serial.println("Insert an empty SD.  Type any character to start.");
  while (!Serial.available());

  // initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
  // breadboards.  use SPI_FULL_SPEED for better performance.
  if (!sd.init(SPI_HALF_SPEED, chipSelect)) sd.initErrorHalt();

  // create a file and write one line to the file
  SdFile file("NAME1.TXT", O_WRITE | O_CREAT);
  if (!file.isOpen()) error("NAME1");
  file.println("A test line for NAME1.TXT");

  // rename the file NAME2.TXT and add a line.
  // sd.cwd() is the current working directory, root.
  if (!file.rename(sd.cwd(), "NAME2.TXT")) error("NAME2");
  file.println("A test line for NAME2.TXT");

  // list files
  Serial.println("------");
  sd.ls(LS_R);

  // make a new directory - "DIR1"
  if (!sd.mkdir("DIR1")) error("DIR1");

  // move file into DIR1, rename it NAME3.TXT and add a line
  if (!file.rename(sd.cwd(), "DIR1/NAME3.TXT")) error("NAME3");
  file.println("A line for DIR1/NAME3.TXT");

  // list files
  Serial.println("------");
  sd.ls(LS_R);

  // make directory "DIR2"
  if (!sd.mkdir("DIR2")) error("DIR2");

  // close file before rename(oldPath, newPath)
  file.close();

  // move DIR1 into DIR2 and rename it DIR3
  if (!sd.rename("DIR1", "DIR2/DIR3")) error("DIR2/DIR3");

  // open file for append in new location and add a line
  if (!file.open("DIR2/DIR3/NAME3.TXT", O_WRITE | O_APPEND)) {
    error("DIR2/DIR3/NAME3.TXT");
  }
  file.println("A line for DIR2/DIR3/NAME3.TXT");

  // list files
  Serial.println("------");
  sd.ls(LS_R);

  Serial.println("Done");
}
void loop() {}
