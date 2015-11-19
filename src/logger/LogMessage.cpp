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
 * LogMessage.h
 *
 *  Created on:  Dec 28, 2013
 *      Author:  Amir Nathoo
 *
 *  Description: Simple C++ logger - Based on example from Bill Blunden's book 
 *               "Software Exorcism:A handbook for Debugging and Optimizing Legacy Code"
 *               ISBN-13 (pbk): 978-1-4302-5107-1
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>        /* for syscall() */
#include <sys/syscall.h>   /* For SYS_xxx definitions */
#include <iostream>
#include "LogMessage.h"

using namespace std;

LogMessage *LogMessage::mInstance = 0;

LogMessage::LogMessage() 
{
  mLogFilter = NULL;
  mBuffer = new char[MAX_BUFFER_SIZE];
  
  mMutex = new Mutex();
}

LogMessage* LogMessage::getInstance()
{
  if (mInstance == NULL)
  {
    mInstance = new LogMessage();
  }
  return mInstance;   
}

void LogMessage::registerFilter(LogFilter *logFilter)
{
  if (mLogFilter != NULL)
  {
    mLogFilter = logFilter;
  }
}
void LogMessage::log(char const* lvl, char const* msg,...)
{
  timeval tp;
  char header[48];
  time_t curtime;
  tm *t = NULL;
  va_list args;
  mMutex->lock();
  gettimeofday(&tp, 0);
  curtime = tp.tv_sec;
  t = localtime(&curtime);
  sprintf(header,"[%02d:%02d:%02d.%06ld][%s][tid=%ld] ",
                t->tm_hour, t->tm_min, t->tm_sec, tp.tv_usec,
                lvl,
                syscall(SYS_gettid));
  va_start(args, msg);
  vsprintf(mBuffer,msg,args);
  va_end(args);
  
  mMutex->unlock();
  cout << header << mBuffer;
  
}
