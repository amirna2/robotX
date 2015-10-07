/*
 * Thread.cpp
  *
 *  Created on:  Aug 13, 2012
 *  Author:      Amir Nathoo
 *
 *  Description: This file provides an implementation of Thread class define in Thread.h
 */

#include <stdio.h>
#include <string.h>
#include "TestBotErrors.h"
#include "TestBotTypes.h"
#include "Thread.h"
#include "LogMessage.h"
static LogMessage *lm = LogMessage::getInstance();

using namespace std;

Thread::Thread()
{
  mStarted = false;
  mArg = NULL;
  mId = 0;
  setAttributes(false, MIN_STACK_SIZE);
}
Thread::Thread(bool detach, int size)
{
  mStarted = false;
  mArg = NULL;
  mId = 0;
  setAttributes(detach, size);
}
Thread::~Thread()
{
}

void Thread::setAttributes(bool detach, int32_t size)
{
  int32_t ret;
  
  lm->log(INFO,"Thread::setAttributes - Enter\n");
  
  if ((ret = pthread_attr_init(&mAttr)) != 0) 
  {
    lm->log(ERROR,"ERROR pthread_attr_init\n");
    throw "Error";
  }
  if (detach) 
  {
    if ((ret = pthread_attr_setdetachstate(&mAttr,PTHREAD_CREATE_DETACHED)) != 0)
    {
      lm->log(ERROR,"ERROR pthread_attr_setdetachstate\n");
      throw "Error";
    }
  }
  if ((ret = pthread_attr_setstacksize(&mAttr, (size_t)size)) != 0) 
  {
    lm->log(ERROR,"ERROR pthread_attr_setstacksize\n");
    throw "Error";
  }
  
  lm->log(INFO,"Thread::setAttributes - Exit\n");
}

void Thread::start(void *arg)
{
  /*
  * Since pthread_create is a C library function, the 3rd argument is
  * a global function that will be executed by the thread. In C++, we
  * emulate the global function using the static member function that
  * is called exec. The 4th argument is the actual argument passed to
  * the function exec. Here we use this pointer, which is an instance
  * of the Thread class.
  */
  lm->log(INFO,"Thread::start - Enter\n");
  if(!mStarted)
  {
    int32_t ret;
    this->mArg = arg;
    mStarted = true;
    if ((ret = pthread_create(&mId, &mAttr, &Thread::exec, this)) != 0)
    {
      lm->log(ERROR,"ERROR pthread_create\n");
      throw "Error";
    }
    lm->log(INFO,"Thread::start - Exit (tid=%lx)\n", mId);
  }
}

int32_t Thread::yield()
{
  return pthread_yield();
}
void Thread::join()
{
  lm->log(INFO,"Thread::join (tid=%lx)\n", mId);
  pthread_join(mId, NULL);
}

// Function that is to be executed by the thread
void *Thread::exec(void *thr)
{
  reinterpret_cast<Thread *> (thr)->run();
  return NULL; //avoiding compiler warning - return value is not checked.
}

