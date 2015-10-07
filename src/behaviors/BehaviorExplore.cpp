/*
 * BehaviorExplore.cpp
 *
 *  Created on:  February 6, 2013
 *      Author:  Amir Nathoo
 *
 *  Description: This file implements the Explore behavior class as defined in BehaviorExplore.h
 */

#include <iostream>
#include "Serial.h"
#include "BehaviorExplore.h"
#include "LogMessage.h"

using namespace std;

static LogMessage *lm = LogMessage::getInstance();

BehaviorExplore::BehaviorExplore()
{
    triggered = false;
}

BehaviorExplore::~BehaviorExplore()
{
    // TODO Auto-generated destructor stub
}

bool BehaviorExplore::requestControl()
{
     
  //What could be triggering exploration?
  // - Maybe some noises capured from the microphones (explore in direction of the noise)
  // - Some interesting objects (a colored ball?)
  // - Could explore (randomly) if nothing has happened within a certain time.
  char str[255];
  uint32_t size = 255;
  memset(str,0,size);
  uint8_t ir_dist=0;
  Serial *s = Serial::getInstance();
  
  lm->log(INFO,"BehaviorExplore::%s - Sending command GET_IRDIST\n",__FUNCTION__);
  if (s->Write("GET_IRDIST\n") != SER_SUCCESS)
  {
    lm->log(ERROR,"ERROR Sending GET_IRDIST command\n");
  }
  
  s->ReadUntil('\n', size, str,64);
  ir_dist = atoi(str);
  lm->log(INFO,"IR Distance - %dcm\n",ir_dist);
  
  if(ir_dist > 10) // more than 10cm away triggers the exploration action 
  {
    lm->log(INFO,"CLEAR PATH - Explore\n");
    triggered = true;
  }
  return triggered;
}

void BehaviorExplore::action()
{    

  //NOTE: It is best to implememt this as a state machine
  if(triggered)
  {
    lm->log(INFO,"BehaviorExplore::%s - Starting\n",__FUNCTION__);
    
    lm->log(INFO,"Sending SERVO MOVE commands\n");
    
    if (Serial::getInstance()->Write("SERVO_MOVE 1\n") != SER_SUCCESS)
    {
      lm->log(ERROR,"ERROR Sending SERVO_MOVE 1 command\n");
    }
    if (Serial::getInstance()->Write("SERVO_MOVE 2\n") != SER_SUCCESS)
    {
      lm->log(ERROR,"ERROR Sending SERVO_MOVE 2 command\n");
    }
    triggered = false;
    lm->log(INFO,"BehaviorExplore::%s - Done\n",__FUNCTION__);
  }
}

// Invoked when this behavior is being suppressed by a higher priority behavior module
void BehaviorExplore::suppress()
{
  lm->log(INFO,"BehaviorExplore::%s - being suppressed\n",__FUNCTION__);
  triggered = false;
}
