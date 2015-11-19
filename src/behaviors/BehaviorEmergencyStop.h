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
 * BehaviorEmergencyStop.h
 *
 *  Created on:  Jan 21, 2014
 *      Author:  Amir Nathoo
 *
 *  Description: This file defines the Emergency Stop Behavior class based on the Subsumption model
 */

#ifndef BEHAVIORSTOP_H_
#define BEHAVIORSTOP_H_

#include "BehaviorModule.h"

class BehaviorEmergencyStop : public BehaviorModule
{
public:
  BehaviorEmergencyStop();
  ~BehaviorEmergencyStop();

  // BehaviorEmergencyStop::requestControl
  // Description:  
  //   Circumstances under which the behavior is taking control of the robot
  // Parameters:   
  //   None
  // Returns:
  //   true is taking control, false otheriwise  
  virtual bool requestControl();
  
  // BehaviorEmergencyStop::action
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

#endif /* BEHAVIORSTOP_H_ */
