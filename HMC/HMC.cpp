/*
 * HMC.cpp - Interface a Honeywell HMC5843 magnetometer to an AVR via i2c
 * Version 0.1 - http://eclecti.cc
 * Copyright (c) 2009 Nirav Patel
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Code based in part on information from the following:
 * http://www.sparkfun.com/datasheets/Sensors/Magneto/HMC5843-v11.c
 * http://www.atmel.com/dyn/resources/prod_documents/doc2545.pdf
 */
 
#include "HMC.h"

/* PUBLIC METHODS */

HMC5843::HMC5843()
{
}

// note that you need to wait at least 5ms after power on to initialize
void HMC5843::init()
{
  PORTC = 0b00110000; // Use the internal pull up resistors
  
  // Choose 100KHz for the bus.  Formula from 21.5.2 in ATmega168 datasheet.
  TWSR &= ~((1<<TWPS1)&(1<<TWPS0));
	TWBR = (unsigned char)(F_CPU/200000l-8);
	
	// Put the HMC5843 into continuous mode
	sendStart();
	sendByte(0x3C);
	sendByte(0x02);
	sendByte(0x00);
	sendStop();
	// note that you need to wait 100ms after this before first calling recieve
}

// This can be called at 100ms intervals to get new data
void HMC5843::getValues(int *x, int *y, int *z)
{
  unsigned char xin, yin, zin;
  // start the reading
  sendStart();
  sendByte(0x3D);
  // read out the 3 values, 2 bytes each.  lsb first, then msb.
  xin = receiveByte();
  *x = (xin<<8)|receiveByte();
  yin = receiveByte();
  *y = (yin<<8)|receiveByte();
  zin = receiveByte();
  *z = (zin<<8)|receiveByte();
  // wrap back around for the next set of reads and close
  sendByte(0x3D);
  sendStop();
}

/* PRIVATE METHODS */

// start i2c as the master
void HMC5843::sendStart(void)
{
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
}

// close i2c
void HMC5843::sendStop(void)
{
  waitForReady();
  TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

// send a byte when the channel is ready
void HMC5843::sendByte(unsigned char data)
{
  waitForReady();
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
}

// ask for a byte, wait for it arrive, and return it
unsigned char HMC5843::receiveByte(void)
{
  waitForReady();
  TWCR = ((TWCR&0x0F)|(1<<TWINT)|(1<<TWEA));
	waitForReady();
	return(TWDR);
}

// get status register. the bits 0 and 1 are zeroed in init. see datasheet
unsigned char HMC5843::readStatus()
{
  waitForReady();
  return(TWSR);
}

// wait for TWINT to be set before touching the other registers.
void HMC5843::waitForReady(void)
{
  // timeout after some time to avoid locking up if something goes wrong
  int timeout = 200;
  while ((!(TWCR & (1<<TWINT))) && (timeout--));
}

// Set the default object
HMC5843 HMC = HMC5843();
