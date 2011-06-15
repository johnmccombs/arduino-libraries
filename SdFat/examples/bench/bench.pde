/*
 * This sketch is a simple binary write/read benchmark.
 */
#include <SdFat.h>
#include <SdFatUtil.h>

// SD chip select pin
const uint8_t chipSelect = SS_PIN;

#define FILE_SIZE_MB 5
#define FILE_SIZE (1000000UL*FILE_SIZE_MB)
#define BUF_SIZE 100

uint8_t buf[BUF_SIZE];

SdFat sd;

SdFile file;

ArduinoOutStream cout(Serial);
//------------------------------------------------------------------------------
// store error strings in flash to save RAM
#define error(s) sd.errorHalt_P(PSTR(s))
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);

  // pstr stores strings in flash to save RAM
  cout << pstr("Type any character to start\n");
  while (!Serial.available());

  cout << pstr("Free RAM: ") << FreeRam() << endl;

  // initialize the SD card at SPI_FULL_SPEED for best performance.
  // try SPI_HALF_SPEED if bus errors occur.
  if (!sd.init(SPI_FULL_SPEED, chipSelect)) sd.initErrorHalt();

  cout << pstr("Type is FAT") << int(sd.vol()->fatType()) << endl;

  // open or create file - truncate existing file.
  if (!file.open("BENCH.DAT", O_CREAT | O_TRUNC | O_RDWR)) {
    error("open failed");
  }

  // fill buf with known data
  for (uint16_t i = 0; i < (BUF_SIZE-2); i++) {
    buf[i] = 'A' + (i % 26);
  }
  buf[BUF_SIZE-2] = '\r';
  buf[BUF_SIZE-1] = '\n';

  cout << pstr("File size ") << FILE_SIZE_MB << pstr("MB\n");

  cout << pstr("Starting write test.  Please wait up to a minute\n");

  // do write test
  uint32_t n = FILE_SIZE/sizeof(buf);
  uint32_t t = millis();
  for (uint32_t i = 0; i < n; i++) {
    if (file.write(buf, sizeof(buf)) != sizeof(buf)) {
      error("write failed");
    }
  }
  file.sync();
  t = millis() - t;
  double s = file.fileSize();
  cout << pstr("Write ") << s/t << pstr(" KB/sec\n\n");
  cout << pstr("Starting read test.  Please wait up to a minute\n");

  // do read test
  file.rewind();
  t = millis();
  for (uint32_t i = 0; i < n; i++) {
    if (file.read(buf, sizeof(buf)) != sizeof(buf)) {
      error("read failed");
    }
  }
  t = millis() - t;
  cout << pstr("Read ") << s/t << pstr(" KB/sec\n");
  cout << pstr("Done\n");
}
//------------------------------------------------------------------------------
void loop() { }
