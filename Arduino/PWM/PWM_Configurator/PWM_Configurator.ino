#include <Servo.h>

Servo motorR;
Servo motorL;

void setup()
{
  Serial.begin(115200);
  motorR.attach(5);
  motorL.attach(6);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop()
{
motorController(70,70);
}

void motorController(int speedLeft, int speedRight) {
  int  minPWM = 1000; //100% reverse
  int  maxPWM = 2000; //100% forward

  speedRight = constrain(speedRight, -100, 100);  // -100 full speed reverse, 100 full speed forward
  speedLeft  = constrain(speedLeft, -100, 100);

  int MCspeedRight = map(speedRight, -100, 100, minPWM, maxPWM);
  int MCspeedLeft  = map(speedLeft , -100, 100, maxPWM, minPWM);
  Serial.print("      left motor: ");
  Serial.print(speedRight);
  Serial.print("      right motor: ");
  Serial.println(speedLeft);

  motorR.writeMicroseconds(MCspeedRight);
  motorL.writeMicroseconds(MCspeedLeft);
}
