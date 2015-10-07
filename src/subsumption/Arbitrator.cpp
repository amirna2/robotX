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
