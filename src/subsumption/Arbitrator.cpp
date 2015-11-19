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
* Arbitrator.cpp
*
*  Created on:  Aug 13, 2012
*      Author:  Amir Nathoo
*
*  Description: This file implements the subsumption Arbitrator class as defined in Arbitrator.h
*/

#include "Arbitrator.h"
#include "LogMessage.h"
static LogMessage *lm = LogMessage::getInstance();

using namespace std;

Arbitrator::Arbitrator(vector<BehaviorModule*>bm, bool flag)
{
  lm->log(INFO,"Arbitrator::%s - Enter\n",__FUNCTION__);

  mMonitor = new Monitor();
  mExitFlag = flag;
  mMonitor->mModules = bm;
  mMonitor->mMaxPriority = (mMonitor->mModules.size() -1);
  mMonitor->mHighestPriority = -1;
  mMonitor->mCurrentModule = -1;
  lm->log(INFO,"Arbitrator::%s - Exit (%p)\n",__FUNCTION__,this);
}

Arbitrator::~Arbitrator()
{
  lm->log(INFO,"Arbitrator::%s - Enter\n",__FUNCTION__);
  delete mMonitor;
  lm->log(INFO,"Arbitrator::%s - Exit (%p)\n",__FUNCTION__,this);
} 

// This method will be executed by the Thread::exec method
void Arbitrator::start()
{
  printf("Arbitrator::%s - Enter\n",__FUNCTION__);
  //find the highest priority behavior requesting control of the robot
  mMonitor->start(); 
  printf("Arbitrator::%s - monitor started\n",__FUNCTION__);
  while(mMonitor->mHighestPriority == -1)
  {
    //allow other behaviors to request control
    usleep(200 * 1000); 
  }
  // allow choosen behavior to perform its action
  // eventually subsumming the currectly running behavior
  arbitrate();
  
  printf("Arbitrator::%s - Exit\n",__FUNCTION__);
     
}

void Arbitrator::arbitrate()
{
  // execute the selected behavior module
  lm->log(INFO,"Arbitrator::%s Enter\n",__FUNCTION__);
  while (true)
  {    
    mMonitor->mMutex->lock();
    
    if (mMonitor->mHighestPriority != -1)
    {
      mMonitor->mCurrentModule = mMonitor->mHighestPriority;
    }
    else if (mExitFlag) // we can exit if there are no more behaviors to check.
    {
      lm->log(DEBUG,"Arbitrator::%s no more behaviors to check\n",__FUNCTION__);
      mMonitor->mMonitoring = false;
      mMonitor->mMutex->unlock();
      lm->log(INFO,"Arbitrator::%s - Exit\n",__FUNCTION__);
      return;
    }

    mMonitor->mMutex->unlock();
    
    if (mMonitor->mCurrentModule != -1)
    {
      // selected module is started
      lm->log(INFO,"Arbitrator::%s starting behavior module: %d\n",__FUNCTION__,mMonitor->mHighestPriority);
      mMonitor->mModules[mMonitor->mCurrentModule]->action();
      mMonitor->mCurrentModule = -1;
    }
    usleep(200*1000); //wait 200ms
  }
}
