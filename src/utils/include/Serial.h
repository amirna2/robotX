/*
Copyright (c) 2012, Amir Nathoo <amir.nth@gmail.com>
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    1.Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    2.Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    3.Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.
      
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * Serial.h
 *
 *  Created on:  Aug 12, 2012
 *      Author:  Amir Nathoo
 *
 *  Description: This file defines Serial interface class used to 
 *               communicate with the robot m-controller board.
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <termios.h>
#include <string.h>
#include <iostream>
#include <fcntl.h>

#include "TestBotErrors.h"
#include "TestBotTypes.h"

const uint32_t SER_SUCCESS      =  TB_SUCCESS;
const uint32_t SER_OPEN_FAILED  = 100;
const uint32_t SER_INVALID_RATE = 101;
const uint32_t SER_READ_FAILED  = 102;
const uint32_t SER_WRITE_FAILED = 103;
const uint32_t SER_INVALID_OPT  = 104;


class Serial
{
public:

  Serial();
  ~Serial();

  static Serial* getInstance(); // make this a singleton
   
  // open the serial port
  tb_Error Open(const char* device, const uint32_t bauds);
  // close serial port
  tb_Error Close(void);

  // reads a string until lineTerminator character is found
  tb_Error ReadUntil(char lineTerminator, uint32_t size, char* str, uint32_t timeout);

  // reads a single byte
  tb_Error ReadByte(uint8_t *b);

  // writes a single byte
  tb_Error WriteByte(uint8_t b);

  // writes a string
  tb_Error Write(const char* str);
private:

  static Serial* mInstance;
  // serial port file descriptor
  int32_t mFd;
};

#endif /* SERIAL_H_ */
