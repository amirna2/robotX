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
MainBot.cpp
Created on:  Aug 13, 2012
Author:  Amir Nathoo
Description: This file contains the entry point of the program running on the PC board.
*/

#include <iostream>
#include <vector>

#include "TestBotTypes.h"
#include "Serial.h"
#include "Arbitrator.h"
#include "BehaviorUserControl.h"
#include "BehaviorEmergencyStop.h"
#include "BehaviorExplore.h"
#include "BehaviorAvoid.h"
#include "LogMessage.h"

using namespace std;

int main()
{
  tb_Error err = SER_SUCCESS;
  vector<BehaviorModule*> bm;

  LogMessage *lm = LogMessage::getInstance();
  Serial *serial = Serial::getInstance();
  
  lm->log(INFO,"main: This is RobotX\n");
  
  lm->log(INFO,"main: opening Serial Port\n");
	err = serial->Open("/dev/ttyACM1", 115200);
  if(err != SER_SUCCESS)
  {
    err = serial->Open("/dev/ttyACM0", 115200);
  }
  
  if(err != SER_SUCCESS)
  {
    lm->log(ERROR,"main: ERROR - Cannot open serial port\n");
    delete serial;
    return -1;
  }
  lm->log(INFO,"main: Serial Port is opened\n");
	 
  // this is required due to Arduino auto reset after a connection is established.
  // For this to work you also have to close the Arduino IDE
  sleep(5); 

  lm->log(INFO,"main: Adding behaviors\n");

  bm.push_back(new BehaviorUserControl); // high priority evaluated first
  bm.push_back(new BehaviorExplore); 
  bm.push_back(new BehaviorAvoid);
  bm.push_back(new BehaviorEmergencyStop); // lowest priority evaluated last
  
  lm->log(INFO,"main: Start the arbitrator\n");
  Arbitrator *arb = new Arbitrator(bm, false);
  arb->start();
  
  serial->Close();
  
  lm->log(INFO,"main: Bye bye!\n");
        
	return 0;
}
