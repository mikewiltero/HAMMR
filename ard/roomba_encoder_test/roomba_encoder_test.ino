// some code taken from Dr. Hessmer. see hessmer_quad.ino located in /ard/

// changes for r2: implementing digitalWriteFast and encoder reading
// changes for r1: adding abillity to use potentiometer for input. this
// will help with control system modeling later.

// including this instead of Wprogram.h because Wprogram.h is from the pre 1.00 arduino IDE.
// instructions here: http://makezine.com/2011/12/01/arduino-1-0-is-out-heres-what-you-need-to-know/
//#include "Arduino.h"

// not sure if this works but lets keep it here for later
// reference: https://gist.github.com/jrmedd/5516863
//#if defined(ARDUINO) && ARDUINO >= 100
  //#include "Arduino.h"
  //#else
  //#include "WProgram.h"
  //#endif


//#include <digitalWriteFast.h> // Dr. Hessmer's digitalwritefast library
                              // http://code.google.com/p/digitalwritefast/

#define motorPinA 7
#define motorPinB 8
#define motorSpeedPin 6
#define commandPin A0

// define encoder inputs
#define encoderPin 2
volatile unsigned int encoderPos = 0;


void setup() {
  // serial setup
  // 9600 is so last year
  Serial.begin(115200);

  // encoder setups
  pinMode(encoderPin, INPUT);

  attachInterrupt(0, doEncoder, RISING);
  
  // motor setups
  pinMode(motorPinA, OUTPUT);
  pinMode(motorPinB, OUTPUT);
  pinMode(motorSpeedPin, OUTPUT);
  Serial.println("Starting...");
  

}

void loop() {

  // NOTE: minium useful speed is about 90. anything less than that
  // and nothing will really happen. (roomba motor). i.e the deadband
  // of the motors is +/-90.

  // get input value from pot and map it to something useful
  
  //int commandValue = analogRead(commandPin);
  // Serial.print(commandValue);
  //commandValue = map(commandValue, 0, 1023, -255, 255);
  int commandValue = 90;

  // this was a good value (1630changes/rev) for the speed of about 120, but after speeding it up, its very wrong.
  // running at 90, ~11.1RPM, and using RISING, there were 942p/rot
  int destinationPos = 942;

  while (destinationPos >= encoderPos) {
    runMotor(commandValue);
    
  }

  
  runMotor(0);
  while(true);
  //runMotor(commandValue);


  




}

void doEncoder() {

  encoderPos++;
  Serial.println(encoderPos);
  //Serial.println(encoderPos, DEC);
  
}

void runMotor(int cmd)
{
  // cmd will be a value between -255 and 255

  // get speed
  int motorSpeed = abs(cmd);
  analogWrite(motorSpeedPin, motorSpeed);
  //Serial.print(" Motor Speed Command: ");
  //Serial.print(motorSpeed);

  if (cmd > 0 && cmd <= 255) {
    digitalWrite(motorPinA, HIGH);
    digitalWrite(motorPinB, LOW);
    //Serial.print(" Rotating CW ");
  }

  else if (cmd < 0 && cmd >= -255) {
    digitalWrite(motorPinA, LOW);
    digitalWrite(motorPinB, HIGH);
    Serial.println(" Rotating CCW");
  }

  else if (cmd == 0) {
    digitalWrite(motorPinA, LOW);
    digitalWrite(motorPinB, LOW);
    Serial.println(" stopped, idle");
  }

  else {
    digitalWrite(motorPinA, LOW);
    digitalWrite(motorPinB, LOW);
    Serial.println(" Motor command error; outside of range.");
  }
  
  
}

