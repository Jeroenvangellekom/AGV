#include <Servo.h>

Servo Motor_R;
Servo Motor_L;

int MoveSpeed = 0;
int WaitTime = 20;
void setup()
{
  Serial.begin(9600);
  Motor_R.attach(11);
  Motor_L.attach(10);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop()
{
  for (int i = 0; i <= 1; i++)
  {
    for (MoveSpeed = 0; MoveSpeed <= 255; MoveSpeed++)
    {
      Serial.println(MoveSpeed);
      Motor_R.write(MoveSpeed);
      Motor_L.write(MoveSpeed);
      delay(WaitTime);
    }
    for (MoveSpeed = 255; MoveSpeed >= 0; MoveSpeed--)
    {
      Serial.println(MoveSpeed);
      Motor_R.write(MoveSpeed);
      Motor_L.write(MoveSpeed);
      delay(WaitTime);
    }
  }
  for (int x = 0; x <= 400; x++)
  {
    digitalWrite(13, HIGH);
    Serial.println(127);
    Motor_R.write(127);
    Motor_L.write(127);
    delay(WaitTime);
    digitalWrite(13, LOW);
  }
}
