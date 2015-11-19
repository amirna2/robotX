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
