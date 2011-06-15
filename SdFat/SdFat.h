/* Arduino SdFat Library
 * Copyright (C) 2009 by William Greiman
 *
 * This file is part of the Arduino SdFat Library
 *
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Arduino SdFat Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#ifndef SdFat_h
#define SdFat_h
/**
 * \file
 * \brief SdFat class
 */
#include <SdStream.h>
#include <ArduinoStream.h>
//------------------------------------------------------------------------------
/**
 * \class SdFat
 * \brief Integration class for the %SdFat library.
 */
class SdFat {
 public:
  SdFat() {}
  /** \return a pointer to the Sd2Card object. */
  Sd2Card* card() {return &card_;}
  /** \return a pointer to the current working directory. */
  SdFile* cwd() {return &cwd_;}
  bool chdir();
  bool chdir(const char* path);
  void errorHalt();
  void errorHalt_P(PGM_P msg);
  void errorHalt(char const *msg);
  void errorPrint();
  void errorPrint_P(PGM_P msg);
  void errorPrint(char const *msg);
  bool exists(const char* name);
  bool init(uint8_t sckRateID = SPI_FULL_SPEED,
    uint8_t chipSelectPin = SD_CHIP_SELECT_PIN);
  void initErrorHalt();
  void initErrorPrint();
  void ls(uint8_t flags = 0);
  void ls(Print* pr, uint8_t flags = 0);
  bool mkdir(const char* path, bool pFlag = true);
  bool remove(const char* path);
  bool rename(const char *oldPath, const char *newPath);
  bool rmdir(const char* path);
  bool truncate(const char* path, uint32_t length);
  /** \return a pointer to the SdVolume object. */
  SdVolume* vol() {return &vol_;}
 private:
  Sd2Card card_;
  SdVolume vol_;
  SdFile cwd_;
};
#endif  // SdFat_h
