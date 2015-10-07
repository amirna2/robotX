/*
 * BehaviorModule.h
 * 
 *  Created on:  Aug 13, 2012
 *      Author:  Amir Nathoo
 *
 *  Description: This file defines the abstract Behavior Module class 
 *  based on the subsumption architecture
 */

#ifndef BEHAVIORMODULE_H_
#define BEHAVIORMODULE_H_

#include "TestBotTypes.h"

class BehaviorModule
{
public:
  virtual ~BehaviorModule();

  // Indicates if the behavior is requesting control, due to positive trigger.
  virtual bool requestControl()=0;

  // Represents the action implementing this behavior.
  virtual void action()=0;

  // Invoked when this behavior is being suppressed by a higher priority behavior module
  virtual void suppress()=0;

};

#endif /* BEHAVIORMODULE_H_ */
