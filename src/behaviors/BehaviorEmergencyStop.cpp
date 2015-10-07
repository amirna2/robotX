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
