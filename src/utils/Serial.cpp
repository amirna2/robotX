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

