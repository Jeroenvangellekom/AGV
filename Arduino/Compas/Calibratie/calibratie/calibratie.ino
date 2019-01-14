#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
LSM9DS1 imu;
#define LSM9DS1_M	0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG	0x6B // Would be 0x6A if SDO_AG is LOW


void setup()
{
  Serial.begin(115200);

  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;

  if (!imu.begin())
  {
    Serial.println("Failed to communicate with LSM9DS1.");
    Serial.println("Double-check wiring.");
    Serial.println("Default settings in this sketch will " \
                   "work for an out of the box LSM9DS1 " \
                   "Breakout, but may need to be modified " \
                   "if the board jumpers are.");
    while (1)
      ;
  }
}

int CompassHeading() {
  imu.readMag();

  int x =  imu.mx - 500;
  int y = imu.my + 0;
  int h1 = (atan2(x, y)) * 180 / PI;
  if (h1 < 0) {
    int h = map(h1, -180, 0, 180, 360);
    return h;
  }
  else {
    int h = h1;
    return h;
  }

}

double TurnToAngle(double angle_to_target) {

  double steer_angle1 = angle_to_target - CompassHeading();
  double steer_angle;

  if (steer_angle1 > 180) {
    steer_angle = (steer_angle1 - 360);
  }
  else if (steer_angle1 < -180) {
    steer_angle = (steer_angle1 + 360);
  }
  else {
    steer_angle = steer_angle1;
  }


  Serial.print("current angle is         ");
  Serial.println(CompassHeading());
  Serial.print("error to target angle is ");
  Serial.println(steer_angle);
  delay(500);
}


void loop()
{

  //TurnToAngle(10);
  imu.readMag();
  Serial.print(imu.mx - 650);
  Serial.print(", ");
  Serial.print(imu.my + 875);
  Serial.println();
delay(100);
}



