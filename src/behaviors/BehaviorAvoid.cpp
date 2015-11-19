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
 *  Author:      Amir Nathoo
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
