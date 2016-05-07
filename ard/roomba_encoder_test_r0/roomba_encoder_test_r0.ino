
#define motorPinA 7
#define motorPinB 8
#define motorSpeedPin 6

void setup() {
  // serial setup
  Serial.begin(115200);
  

  pinMode(motorPinA, OUTPUT);
  pinMode(motorPinB, OUTPUT);
  pinMode(motorSpeedPin, OUTPUT);
  Serial.println("Starting...");

}

void loop() {
  // manually enter the speed here. value between -255 and 255
  // NOTE: minium useful speed is about 90. anything less than that
  // and nothing will really happen. (roomba motor). i.e the deadband
  // of the motors is +/-90.
  int test = 0;
  runMotor(test);
  delay(500);

}

void runMotor(int cmd)
{
  // cmd will be a value between -255 and 255

  // get speed
  int motorSpeed = abs(cmd);
  analogWrite(motorSpeedPin, motorSpeed);
  Serial.print("Motor Speed Command: ");
  Serial.print(motorSpeed);

  if (cmd > 0 && cmd <= 255) {
    digitalWrite(motorPinA, HIGH);
    digitalWrite(motorPinB, LOW);
    Serial.println(" Rotating CC");
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

