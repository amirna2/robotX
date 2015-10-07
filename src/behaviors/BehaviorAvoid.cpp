/*
 * BehaviorAvoid.cpp
 *
 *  Created on:  Aug 13, 2012
 *      Author:  Amir Nathoo
 *
 *  Description: This file implements the Obstacle Avoidance Behavior class as defined in BehaviorAvoid.h
 */

#include <iostream>
#include "Serial.h"
#include "BehaviorAvoid.h"
#include "LogMessage.h"

using namespace std;

static LogMessage *lm = LogMessage::getInstance();

BehaviorAvoid::BehaviorAvoid()
{
  triggered = false;
    
}

BehaviorAvoid::~BehaviorAvoid()
{
    // TODO Auto-generated destructor stub
}

bool BehaviorAvoid::requestControl()
{
     // 1. Read relevant sensor inputs
     // 2. Evaluate inputs and trigger the control request
     
  char str[255];
  uint32_t size = 255;
  memset(str,0,size);
  uint8_t ir_dist=0;
  Serial *s = Serial::getInstance();
  
  lm->log(INFO,"BehaviorAvoid::%s - Sending command GET_IRDIST\n",__FUNCTION__);
  if (s->Write("GET_IRDIST\n") != SER_SUCCESS)
  {
    lm->log(ERROR,"ERROR Sending GET_IRDIST command\n");
  }
  
  s->ReadUntil('\n', size, str,64);
  ir_dist = atoi(str);
  lm->log(INFO,"IR Distance - %dcm\n",ir_dist);
  
  if(ir_dist <= 10) // 10 cm or less triggers the avoidance action 
  {
    lm->log(INFO,"OBSTACLE DETECTED at %d cm STOP SERVO\n",ir_dist);
    triggered = true;
  }

  return triggered;
}

void BehaviorAvoid::action()
{    
  //NOTE: It is best to implememt this as a state machine
  //Avoidance is simply stopping the robot here.
  if(triggered)
  {
    lm->log(INFO,"BehaviorAvoid::%s - Starting\n",__FUNCTION__);
    lm->log(INFO,"AVOIDING Obstacle - STOP the ROBOT\n");
    if (Serial::getInstance()->Write("SERVO_STOP\n") != SER_SUCCESS)
    {
      lm->log(ERROR,"ERROR Sending SERVO_STOP command\n");
    }
    triggered = false;
    lm->log(INFO,"BehaviorAvoid::%s - Done\n",__FUNCTION__);
  }
}

// Invoked when this behavior is being suppressed by a higher priority behavior module
void BehaviorAvoid::suppress()
{
  lm->log(INFO,"BehaviorAvoid::%s - being suppressed\n",__FUNCTION__);
  triggered = false;
}
