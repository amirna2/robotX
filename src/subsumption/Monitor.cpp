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

