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
 * Serial.cpp
 *
 *  Created on:  Aug 13, 2012
 *  Author:      Amir Nathoo
 *
 *  Description: This file provides an implementation for
 *               the Serial interface defined in Serial.h
 */
 
#include <unistd.h> 
#include "Serial.h"
#include "LogMessage.h"

static LogMessage *lm = LogMessage::getInstance();

Serial *Serial::mInstance = 0;

Serial* Serial::getInstance()
{
  if (mInstance == NULL)
  {
    mInstance = new Serial();
  }
  return mInstance;   
}

Serial::Serial()
{
  mFd=-1;
}

Serial::~Serial()
{
  this->Close();
}

/**
 * Description:
 *  This method opens a serial port on a given device node and for a given baud rate
 * Params:
 *  device:    A string that represents the device node e.g /dev/ttyUSB0, /dev/ttyACM0
 *  baudRate: The serial port rate (typical values are 4800, 9600, 19200, 38400, 57600, 115200)
 * Returns:
 *
 */
tb_Error Serial::Open(const char *device, const unsigned int baudRate)
{
  struct termios options;		// Structure with the device's options
  speed_t        speed;

  // Open device
  mFd = open( device, O_RDWR | O_NOCTTY);
  if( mFd == -1 )
  {
      return SER_OPEN_FAILED;
  }
  // Set parameters:
  // Get the current options of the port
  tcgetattr( mFd, &options );
  // Clear all the options
  bzero( &options, sizeof(options) );
  // Set the speed (Bauds)
  switch (baudRate)
  {
    case 4800 :
        speed=B4800;
        break;
    case 9600 :
        speed=B9600;
        break;
    case 19200 :
        speed=B19200;
        break;
    case 38400 :
        speed=B38400;
        break;
    case 57600 :
        speed=B57600;
        break;
    case 115200 :
        speed=B115200;
        break;
    default :
        return SER_INVALID_RATE;
  }
  // Set the selected baud rate
  cfsetispeed(&options, speed);
  cfsetospeed(&options, speed);

  bzero(&options, sizeof(options));
  options.c_cflag &= ~HUPCL;   // disable hang-up-on-close to avoid reset
  options.c_cflag |= speed | CRTSCTS | CS8 | CLOCAL | CREAD;
  options.c_iflag = IGNPAR;
  options.c_oflag = 0;

  /* set input mode (non-canonical, no echo,...) */
  options.c_lflag = 0;
   
  options.c_cc[VTIME]    = 0;   // inter-character timer unused
  options.c_cc[VMIN]     = 1;   // blocking read 1 char at a time

  tcflush(mFd, TCIFLUSH);

  // Activate the settings
  if ( tcsetattr(mFd, TCSANOW, &options) < 0)
  {
    return SER_INVALID_OPT;
  }
  return SER_SUCCESS;
}

tb_Error Serial::Close(void)
{
  close(mFd);
  return SER_SUCCESS;
}

tb_Error Serial::ReadUntil(char lineTerminator, uint32_t size, char* str, uint32_t timeout)
{
  uint32_t   count=0; // max number of bytes to read
  tb_Error   err = SER_SUCCESS;
  uint32_t t = timeout;
  while(count < size) // While the buffer is not full
  {
    // read something
    int32_t nbytes = read( mFd, &str[count], 1 );

    if (nbytes == -1 || t == 0)
    {
      lm->log(ERROR,"%s: ERROR Failed to Read the output\n",__FUNCTION__);
      err = SER_READ_FAILED;
      break;
    }
    else if (nbytes == 0)  // continue reading until with have data or we timed out
    {
      t--;
      usleep( 10 * 1000 ); // wait 10 millisec try again
      continue;
    }
    else
    {
      if (str[count]==lineTerminator)  // Check if it is the final char
      {
        str[count++]=0;
        return count;
      }
      count++;
    }
  }
  return err;
}

// reads a single byte
tb_Error Serial::ReadByte(uint8_t *b)
{
  tb_Error err = SER_SUCCESS;
  int32_t n = read(mFd, b, 1);
  if ( n != 1 )
  {
    err = SER_READ_FAILED;
  }
  return err;
}

// writes a single byte
tb_Error Serial::WriteByte(uint8_t b)
{
  tb_Error err = SER_SUCCESS;
  int32_t n = write(mFd, &b, 1);
  if( n != 1 )
  {
    err = SER_WRITE_FAILED;
  }
  return err;
}

// writes a string
tb_Error Serial::Write(const char* str)
{
  tb_Error err = SER_SUCCESS;
  int32_t len = strlen( str );
  int32_t n = write(mFd, str, len);
  if( n != len )
  {
    lm->log(ERROR,"%s: ERROR Failed to Send %s\n",__FUNCTION__,str);
    err = SER_WRITE_FAILED;
  }
  return err;
}

