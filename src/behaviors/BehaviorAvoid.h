/*
 * BehaviorAvoid.h
 *
 *  Created on:  Aug 13, 2012
 *      Author:  Amir Nathoo
 *
 *  Description: This file defines the Obstacle Avoidance Behavior class based on the Subsumption model
 */

#ifndef BEHAVIORAVOID_H_
#define BEHAVIORAVOID_H_

#include "BehaviorModule.h"

class BehaviorAvoid : public BehaviorModule
{
public:
  BehaviorAvoid();
  ~BehaviorAvoid();

  // BehaviorAvoid::requestControl
  // Description:  
  //   Circumstances under which the behavior is taking control of the robot
  // Parameters:   
  //   None
  // Returns:
  //   true is taking control, false otheriwise  
  virtual bool requestControl();
  
  // BehaviorAvoid::action
  // Description: 
  //   Represents the action implementing this behavior.
  //   This can be implemeted as an Augmented Finite State Machine.
  //   A behavior action returns as soon as the action is completed.
  //   It also must return if the behavior is being suppressed in favor
  //   of a higher priority behavior. The suppression is indicated by the 
  //   suppress() method below
  // Returns:        
  //   When we exit this method, the robot is in a known good state for another behavior action.   
  virtual void action();

  // Invoked when this behavior is being suppressed by a higher level behavior module
  virtual void suppress();

private:
  bool triggered;
};

#endif /* BEHAVIORAVOID_H_ */
