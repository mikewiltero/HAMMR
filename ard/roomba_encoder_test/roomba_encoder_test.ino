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
#define motorSpeedPin 6 // this is PWM to the H-bridge
#define commandPin A0 // potentiometer

// define encoder inputs
#define encoderPin 2 // this is an interrupt pin
volatile unsigned int encoderPos = 0;

// the frequency at which the speed will be calculated. 1000 = every second
const int calcFreq = 200;
const int ppr = 830; // the points per wheel revolution, found experimentially
float RPM = 0;
unsigned long previousMillis = 0;
float encoderDelta = 0; // used to track difference in encoder readings
long lastEncoderPos = 0;
float encoderRawSpeed = 0;


void setup() {
  // serial setup
  // 9600 is so last year
  Serial.begin(115200);

  // encoder setups
  pinMode(encoderPin, INPUT);

  attachInterrupt(0, doEncoder, RISING); // attach the encoder to interrupt and trigger when rising.

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

  // get the commanded speed from the pot
  int commandValue = analogRead(commandPin);
  // Serial.print(commandValue);
  commandValue = map(commandValue, 0, 1023, -255, 255);

  runMotor(commandValue);

  unsigned long currentMillis = millis();

  // calculate the speed on a specified frequency, but only at a specified frequency.
  if (currentMillis - previousMillis >= calcFreq) {
    previousMillis = currentMillis;

    // find the difference in encoder ticks between last read and now
    encoderDelta = encoderPos - lastEncoderPos;
    lastEncoderPos = encoderPos;

    // this is the rotational speed of the ENCODER wheel
    encoderRawSpeed = ((encoderDelta / calcFreq)*1000); // points per second (p/s)

    RPM = ((encoderRawSpeed / ppr) * 60); // divide by the ppr conversion and seconds to get RPM

    Serial.print("\nCommand: ");
    Serial.print(commandValue);
    Serial.print("\tEncoder Pos: ");
    Serial.print(encoderPos);
    Serial.print("\tWheel Speed: ");
    Serial.println(RPM);
    Serial.print("Last Enc: ");
    Serial.print(lastEncoderPos);
    Serial.print("\tDelta: ");
    Serial.print(encoderDelta);
    Serial.print("\tRawSpd: ");
    Serial.println(encoderRawSpeed);
  }

}

// interrupt service routine
void doEncoder() {

  // add a count every time an encoder tick triggers the ISR
  encoderPos++;
  //Serial.println("TRIGGERED");

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
    //Serial.println(" Rotating CCW");
  }

  else if (cmd == 0) {
    digitalWrite(motorPinA, LOW);
    digitalWrite(motorPinB, LOW);
    //Serial.println(" stopped, idle");
  }

  else {
    digitalWrite(motorPinA, LOW);
    digitalWrite(motorPinB, LOW);
    //Serial.println(" Motor command error; outside of range.");
  }


}

