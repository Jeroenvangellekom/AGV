#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include <TinyGPS++.h>
#include <Servo.h>

#define LSM9DS1_M  0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW

static const int RX3Pin = 15;          //GPS
static const int RXPin = 0;            //Laptop
static const int TXPin = 1;
static const uint32_t GPSBaud = 9600;


/* parkeerplaats
static const double way_LAT1 = 51.6472990, way_LON1 = 5.5449639;
static const double way_LAT2 = 51.6473595, way_LON2 = 5.5448237;
static const double way_LAT3 = 51.6471793, way_LON3 = 5.5447936;
static const double way_LAT4 = 51.6471321, way_LON4 = 5.5449736; */

static const double way_LAT1 = 51.647037, way_LON1 = 5.545925;
static const double way_LAT2 = 51.646819, way_LON2 = 5.545886;
static const double way_LAT3 = 51.646836, way_LON3 = 5.545570;
static const double way_LAT4 = 51.647079, way_LON4 = 5.545582;


LSM9DS1 imu;
TinyGPSPlus gps;
Servo motorR;
Servo motorL;

int magxOffset = -650;
int magyOffset = 875;

int state;
int waypointRadius = 1;

void setup()
{

  ////sets I2c ports for compass////
  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;

  ////sets serial for GPS////
  pinMode(RX3Pin, INPUT);
  Serial3.begin(GPSBaud); //sets GPS software serial

  ////sets serial for GPS////
  pinMode(RXPin, INPUT);
  pinMode(TXPin, INPUT);
  Serial.begin(115200); //sets GPS software serial

  ////configure motors////
  motorR.attach(5);
  motorL.attach(6);


  imu.begin();
}

/////////////////////////////////////////////////////////////////////
///////////////////////BEGIN COMPASS FUNCTIONS///////////////////////
/////////////////////////////////////////////////////////////////////
int CompassHeading() {
  imu.readMag();

  int x =  imu.mx + magxOffset;
  int y = imu.my + magyOffset;
  int h1 = (atan2(x, y)) * 180 / PI;
  if (h1 < 0) {
    int h = map(h1, -180, 0, 180, 360);
    return h;
    Serial.print("current Angle  ");
    Serial.print(h);
    Serial.println();
  }
  else {
    int h = h1;
    return h;
    Serial.print("current Angle  ");
    Serial.print(h);
    Serial.println();
  }

}


/////////////////////////////////////////////////////////////////////
///////////////////////////GPS FUNCTIONS/////////////////////////////
/////////////////////////////////////////////////////////////////////
unsigned long distanceToWaypoint(float way_LAT, float way_LON) {
  unsigned long distanceToWaypoint =
    (unsigned long)TinyGPSPlus::distanceBetween(
      gps.location.lat(),
      gps.location.lng(),
      way_LAT,
      way_LON);
  Serial.print(" distance to waypoint   ");
  Serial.println(distanceToWaypoint);
  return (distanceToWaypoint);
}

double courseToWaypoint(float way_LAT, float way_LON) {
  double courseToWaypoint =
    TinyGPSPlus::courseTo(
      gps.location.lat(),
      gps.location.lng(),
      way_LAT,
      way_LON);
  return (courseToWaypoint);
}

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (Serial3.available())
      gps.encode(Serial3.read());
  } while (millis() - start < ms);
}

/////////////////////////////////////////////////////////////////////
/////////////////////NAVIGATIE FUNCTIONS/////////////////////////////
/////////////////////////////////////////////////////////////////////
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
  return (steer_angle);
}
/////////////////////////////////////////////////////////////////////
/////////////////////DRIVE FUNCTIONS/////////////////////////////////
/////////////////////////////////////////////////////////////////////
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

void driveToWaypoint(double way_LAT, double way_LON) {
  int turnAngle = TurnToAngle(courseToWaypoint(way_LAT, way_LON)); //waarde tussen -180 en 180
  double Pgain = 1.5;

  if (turnAngle < 5 && turnAngle > -5) {
    motorController(70, 70);
  }
  else {
    int speedR = 70 - (turnAngle * Pgain);
    int speedL = 70 + (turnAngle * Pgain);
    motorController(speedL, speedR);
  }



  Serial.print("huidige locatie:  ");
  Serial.print(gps.location.lat(), 7);
  Serial.print(", ");
  Serial.println(gps.location.lng(), 7);

  smartDelay(10);
  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
}



void loop()
{


  switch (state)
  {
    case 0:
      Serial.print("State 0, waiting for satalites .... ");
      Serial.print(gps.satellites.value());
      Serial.println();
      smartDelay(1000);
      if (gps.satellites.value() > 3) {
        state = 1;
      }
      break;


    case 1:
      Serial.print("State 1, driving to waypoint 1");
      Serial.println();
      if (distanceToWaypoint(way_LAT1, way_LON1) <= waypointRadius) {
        state = 2;
      }
      else {
        driveToWaypoint(way_LAT1, way_LON1);
      }
      break;

    case 2:
      Serial.print("State 2, driving to waypoint 2");
      Serial.println();
      if (distanceToWaypoint(way_LAT2, way_LON2) <= waypointRadius) {
        state = 3;
      }
      else {
        driveToWaypoint(way_LAT2, way_LON2);
      }
      break;

    case 3:
      Serial.print("State 3, driving to waypoint 3");
      Serial.println();
      if (distanceToWaypoint(way_LAT3, way_LON3) <= waypointRadius) {
        state = 4;
      }
      else {
        driveToWaypoint(way_LAT3, way_LON3);
      }
      break;

    case 4:
      Serial.print("State 4, driving to waypoint 4");
      Serial.println();
      if (distanceToWaypoint(way_LAT4, way_LON4) <= waypointRadius) {
        state = 5;
      }
      else {
        driveToWaypoint(way_LAT4, way_LON4);
      }
      break;

    case 5:
      Serial.print("State 4, stopping at waypoint 3");
      Serial.println();
      motorController(0, 0);
  }
}
