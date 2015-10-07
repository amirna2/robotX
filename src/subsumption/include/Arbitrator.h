/*
 * Arbitrator.h
 *
 *  Created on:  Aug 13, 2012
 *  Author:      Amir Nathoo
 *
 *  Description: This file defines the Arbitrator class 
 *               as described in subsumption architecture
 */

#ifndef ARBITRATOR_H_
#define ARBITRATOR_H_

#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "BehaviorModule.h"
#include "Monitor.h"
#include "Thread.h"
#include "Mutex.h"

using namespace std;

class Arbitrator
{
  
  public:
    Arbitrator(vector<BehaviorModule*>bm, bool flag);
    virtual ~Arbitrator();

    void start();

private:
  void arbitrate();
  bool mExitFlag;
  Monitor *mMonitor;
};

#endif /* ARBITRATOR_H_ */
