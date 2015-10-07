/*
 * TestBot2.cpp
 *
 *  Created on:  Aug 13, 2012
 *      Author:  Amir Nathoo
 *
 *  Description: This file contains the entry point of the program running on the PC board.
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
