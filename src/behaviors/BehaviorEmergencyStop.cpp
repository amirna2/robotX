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
 * BehaviorAvoid.cpp
 *
 *  Created on:  Aug 13, 2012
 *      Author:  Amir Nathoo
 *
 *  Description: This file implements the EmergencyStop Behavior class as defined in BehaviorEmergencyStop.h
 */

#include <iostream>
#include "BehaviorEmergencyStop.h"
#include "LogMessage.h"

using namespace std;

static LogMessage *lm = LogMessage::getInstance();

BehaviorEmergencyStop::BehaviorEmergencyStop()
{
    triggered = false;
}

BehaviorEmergencyStop::~BehaviorEmergencyStop()
{
    // TODO Auto-generated destructor stub
}

bool BehaviorEmergencyStop::requestControl()
{
     
    lm->log(INFO,"BehaviorEmergencyStop::%s requestControl Enter\n",__FUNCTION__);
    triggered = false;
    lm->log(INFO,"BehaviorEmergencyStop::%s requestControl Exit (%d)\n",__FUNCTION__,triggered);
    return triggered;
}

void BehaviorEmergencyStop::action()
{    
  //NOTE: It is best to implememt this as a state machine
  if(triggered)
  {
    lm->log(INFO,"BehaviorEmergencyStop::%s - Starting\n",__FUNCTION__);
    
    //Release control of the robot here to allow other behaviors to take control
    triggered = false;
    lm->log(INFO,"BehaviorEmergencyStop::%s - Done\n",__FUNCTION__);
  }
}

// Invoked when this behavior is being suppressed by a higher priority behavior module
void BehaviorEmergencyStop::suppress()
{
  lm->log(INFO,"BehaviorEmergencyStop::%s - being suppressed\n",__FUNCTION__);
  triggered = false;
}
