//
// Copyright (c) 2000-2014, Amir Nathoo <amir.nth@gmail.com>
// All rights reserved.
//--------------------------------------------------------------------------------------------------------------
//
// RobotX.ino
// Description: Processing serial commands received from the robot's main board.
//--------------------------------------------------------------------------------------------------------------


#include <SerialCommand.h>
#include <DistanceGP2Y0A21YK.h>
#include <Servo.h>

SerialCommand sCmd;     // SerialCommand object
Servo  servo1;          // Servo motor object
#define SERVO1_PIN 4   // servo1 is connected to Digital (PWM) Pin 1 


void setup() {

  delay(2000); // Not sure the delays are needed here. But I don't want to touch it :)
  Serial.begin(115200);
  delay(2000);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // Setup callbacks for SerialCommand commands

  // Sensor Query commands
  sCmd.addCommand("TEST", sayHello);           //  Echos the string argument back
  sCmd.addCommand("P",     processCommand);    // Converts two arguments to integers and echos them back
  sCmd.addCommand("GET_IRDIST", getIRDistance);// Return distance from obstacles

  // Actuator commands
  sCmd.addCommand("SERVO_MOVE", servoMove);           // Move the servo
  sCmd.addCommand("SERVO_STOP", servoStop);           // Stop the servo
  sCmd.addCommand("SERVO_POS",  servoGetPosition);    // Get the servo position

  sCmd.setDefaultHandler(unrecognized);        // Handler for command that isn't matched  (says "What?")

  //setup the servo
  servo1.attach(SERVO1_PIN);  
}

void loop() {
  sCmd.readSerial(); // We don't do much, just process serial commands
}


void servoMove()
{
  char *arg;
  int opt;
  int i;
  arg = sCmd.next();    // Get the next argument from the SerialCommand object buffer
  opt = atoi(arg); 

  //Serial.print("SERVO_MOVE ");
  //Serial.println(arg);

  int current_pos = servo1.read();
  //Serial.print("Moving from ");
  //Serial.println(current_pos);

  if (opt == 1) 
  { 
    for(i = current_pos; i < 180; i += 1)  // goes from 0 degrees to 180 degrees 
    {                                  // in steps of 1 degree 
      servo1.write(i);               // tell servo to go to position in variable 'pos' 
      delay(15);                       // waits 15ms for the servo to reach the position 
    } 
  }
  else if(opt == 2)
  {
    for(i = current_pos; i>=1; i-=1)     // goes from 180 degrees to 0 degrees 
    {                                
      servo1.write(i);              // tell servo to go to position in variable 'pos' 
      delay(15);                       // waits 15ms for the servo to reach the position 
    }    
  } 
}

void servoStop()
{
  servo1.write(servo1.read());
}
void servoGetPosition()
{
  int i = servo1.read();
  Serial.println(i); 
}
void getIRDistance()
{
  DistanceGP2Y0A21YK Dist;
  int distance;

  Dist.begin(A2); // Make sure the Pin is not already used or reserved
  Dist.setEnabled(true);
  distance = Dist.getDistanceCentimeter();
  //Serial.print("Distance:");
  Serial.println(distance);

}
void sayHello() {
  char *arg;
  arg = sCmd.next();    // Get the next argument from the SerialCommand object buffer
  if (arg != NULL) 
  { 
    Serial.write("I say Hello ");
    Serial.write( arg );
    Serial.write('\n');    
  }
  else {
    Serial.println("Hello, whoever you are\n");
  }
}


void processCommand() {
  int aNumber;
  char *arg;

  Serial.println("We're in processCommand");
  arg = sCmd.next();
  if (arg != NULL) {
    aNumber = atoi(arg);    // Converts a char string to an integer
    Serial.print("First argument was: ");
    Serial.println(aNumber);
  }
  else {
    Serial.println("No arguments");
  }

  arg = sCmd.next();
  if (arg != NULL) {
    aNumber = atol(arg);
    Serial.print("Second argument was: ");
    Serial.println(aNumber);
  }
  else {
    Serial.println("No second argument");
  }
}

// This gets set as the default handler, and gets called when no other command matches.
void unrecognized(const char *command) {
  Serial.println("What?");
}

