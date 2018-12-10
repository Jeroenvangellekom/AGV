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
  Serial.print("current angle is ");
  Serial.println(CompassHeading());
}

int TurnToAngle(int angle_to_target) {
  int error = angle_to_target - CompassHeading();
  Serial.print("error to target angle is ");
  Serial.println(error);
}


void loop()
{

  imu.readMag();
  Serial.print(imu.mx);
  Serial.print(", ");
  Serial.print(imu.my);
  Serial.print(", ");
  Serial.println(CompassHeading());

}



