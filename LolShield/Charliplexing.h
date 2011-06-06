/*
  Charliplexing.h - Library for controlling the charliplexed led board
  from JimmiePRodgers.com
  Created by Alex Wenger, December 30, 2009.
  Modified by Matt Mets, May 28, 2010.
  Released into the public domain.
*/

#ifndef Charliplexing_h
#define Charliplexing_h

#include <inttypes.h>

#define SINGLE_BUFFER 0
#define DOUBLE_BUFFER 1

namespace LedSign
{
    extern void Init(uint8_t mode = SINGLE_BUFFER);
    extern void Set(uint8_t x, uint8_t y, uint8_t c);
    extern volatile unsigned int tcnt2;
    extern void Flip(bool blocking = false);
    extern void Clear(int set=0);
    extern void Horizontal(int y, int set=0);
    extern void Vertical(int x, int set=0);
};

#endif
