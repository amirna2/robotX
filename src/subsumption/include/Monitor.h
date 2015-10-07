/*
 * Arbitrator.h
 *
 *  Created on:  Aug 13, 2012
 *  Author:      Amir Nathoo
 *
 *  Description: This file defines the Arbitrator class 
 *               as described in subsumption architecture
 */

#ifndef MONITOR_H_
#define MONITOR_H_

#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "BehaviorModule.h"
#include "Thread.h"
#include "Mutex.h"

using namespace std;

//Aggregate class of Arbitrator to monitor for behaviors
class Monitor : public Thread
{ 
public:
  bool mMonitoring;
  vector <BehaviorModule*> mModules;
  
  // This arbitrator is priority based. The highest priority gets to execute its engage method
  // Highest priority: First behavior in Behavior array
  // Lowest priority : Last behavior in Behavior array
  int32_t mHighestPriority;
  int32_t mMaxPriority;
  int32_t mCurrentModule; //  active behavior; set by monitor, used by arbitrate();
  Mutex* mMutex;
    
  ~Monitor();
  Monitor();   
  virtual void run();

};


#endif /* MONITOR_H_ */
