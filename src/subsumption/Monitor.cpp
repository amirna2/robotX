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
 * Arbitrator.h
 *
 *  Created on:  Aug 13, 2012
 *  Author:      Amir Nathoo
 *
 *  Description: This file defines the Arbitrator class 
 *               as described in subsumption architecture
 */


#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "BehaviorModule.h"
#include "Thread.h"
#include "Mutex.h"
#include "Monitor.h"

#include "LogMessage.h"
static LogMessage *lm = LogMessage::getInstance();

using namespace std;

Monitor::~Monitor()
{
  delete mMutex;
};
  
Monitor::Monitor() : Thread(false, MIN_STACK_SIZE)
{
  
  mMutex = new Mutex();
  mMonitoring = true;
  
  lm->log(INFO,"Monitor::%s - created\n",__FUNCTION__);
}
  
void Monitor::run()
{ 
  lm->log(INFO,"Monitor::%s - Enter\n",__FUNCTION__);
  while(mMonitoring)
  {
    int32_t savedMod =-1;
    mMutex->lock();
    mHighestPriority = -1;
    //find highest priority behavior that asked to execute
    for (int32_t i = mMaxPriority; i >= 0; i--)
    {
      if (mModules.at(i)->requestControl())
      {
        mHighestPriority = i;
        break;
      }
    }
    savedMod = mCurrentModule;
    // Ask a subsumed module to suppress its output to allow higher module to execute.
    if (mCurrentModule != -1 && mHighestPriority > mCurrentModule)
    {
      mModules[savedMod]->suppress();
    }
    else
    {
      mCurrentModule = mHighestPriority;
    }
    mMutex->unlock();
    usleep(200 *1000); // allow main arbiter thread to run
  } //while mMonitoring
}

