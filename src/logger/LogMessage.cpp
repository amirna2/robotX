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
