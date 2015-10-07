/*
 * BehaviorAvoid.cpp
 *
 *  Created on:  Aug 13, 2012
 *      Author:  Amir Nathoo
 *
 *  Description: This file implements the User Control Behavior class as defined in BehaviorUserControl.h
 */

#include <iostream>
#include "BehaviorUserControl.h"
#include "LogMessage.h"

using namespace std;

static LogMessage *lm = LogMessage::getInstance();

BehaviorUserControl::BehaviorUserControl()
{
    triggered = false;
}

BehaviorUserControl::~BehaviorUserControl()
{
    // TODO Auto-generated destructor stub
}

bool BehaviorUserControl::requestControl()
{
     
    lm->log(INFO,"BehaviorUserControl::%s requestControl Enter\n",__FUNCTION__);
    triggered = false;
    lm->log(INFO,"BehaviorUserControl::%s requestControl Exit (%d)\n",__FUNCTION__,triggered);
    return triggered;
}

void BehaviorUserControl::action()
{    

  //NOTE: It is best to implememt this as a state machine
  lm->log(INFO,"BehaviorUserControl::%s - Enter\n",__FUNCTION__);
  if(triggered)
  {
    lm->log(INFO,"BehaviorUserControl::%s - started\n",__FUNCTION__);
    
    //Release control of the robot here to allow other behaviors to take control

  }
  else
  {
   lm->log(INFO,"BehaviorUserControl::%s - suppressed\n",__FUNCTION__);
  }
  lm->log(INFO,"BehaviorUserControl::%s - Exit\n",__FUNCTION__);
}

// Invoked when this behavior is being suppressed by a higher priority behavior module
void BehaviorUserControl::suppress()
{
  lm->log(INFO,"BehaviorUserControl::%s - being suppressed\n",__FUNCTION__);
  triggered = false;
}
