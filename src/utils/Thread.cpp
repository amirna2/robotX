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

