/*
 * LogMessage.h
 *
 *  Created on:  Dec 28, 2013
 *      Author:  Amir Nathoo
 *
 *  Description: Simple C++ logger - Loosely based on example from Bill Blunden's book 
 *               "Software Exorcism:A handbook for Debugging and Optimizing Legacy Code"
 *               ISBN-13 (pbk): 978-1-4302-5107-1
 */
 
#ifndef LOGMESSAGE_H_
#define LOGMESSAGE_H_

#include <stdlib.h>
#include "Mutex.h"
#include "LogFilter.h"

#define MAX_BUFFER_SIZE 1024
#define ERROR "ERROR"
#define WARN  "WARN"
#define INFO  "INFO"
#define DEBUG "DEBUG"
#define TRACE "TRACE"

class LogMessage
{
public:  
  static LogMessage* getInstance();
  void registerFilter(LogFilter *logFilter); 
  void log(char const* lvl, char const* msg,...);
  
private:
  static LogMessage* mInstance;
  char*  mBuffer;
  Mutex *mMutex;
  LogFilter* mLogFilter;
  
  LogMessage();
  
};


#endif /* LOGMESSAGE_H_ */

