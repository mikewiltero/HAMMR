
// changes for r1: adding abillity to use potentiometer for input. this
// will help with control system modeling later.

#define motorPinA 7
#define motorPinB 8
#define motorSpeedPin 6
#define commandPin A0

void setup() {
  // serial setup
  // 9600 is so last year
  Serial.begin(115200);
  

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
  
  int commandValue = analogRead(commandPin);
  // Serial.print(commandValue);
  commandValue = map(commandValue, 0, 1023, -255, 255);
  runMotor(commandValue);
  delay(10);

}

void runMotor(int cmd)
{
  // cmd will be a value between -255 and 255

  // get speed
  int motorSpeed = abs(cmd);
  analogWrite(motorSpeedPin, motorSpeed);
  Serial.print(" Motor Speed Command: ");
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

