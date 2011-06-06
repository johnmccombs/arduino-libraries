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
#include <util/twi.h>
#include "WProgram.h"

#ifndef HMC_h
#define HMC_h

class HMC5843
{
  public:
    HMC5843();
    void init();
    void getValues(int *x, int *y, int *z);
  private:
    void sendStart(void);
    void sendStop(void);
    void sendByte(unsigned char data);
    unsigned char receiveByte(void);
    unsigned char readStatus(void);
    void waitForReady(void);
};

extern HMC5843 HMC;

#endif // HMC_h
