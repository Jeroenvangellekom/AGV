#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

static const double way1_LAT = 51.646749, way1_LON = 5.545316; //schommel
static const double way2_LAT = 51.646684, way2_LON = 5.544922; //huis
static const double way3_LAT = 51.647349, way3_LON = 5.545416; //grote loods

TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(115200);
  ss.begin(GPSBaud);
}



void loop()
{

  /*printInt(gps.satellites.value(), gps.satellites.isValid(), 5);
  printFloat(gps.hdop.hdop(), gps.hdop.isValid(), 6, 1);
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
  printInt(gps.location.age(), gps.location.isValid(), 5);
  printDateTime(gps.date, gps.time);
  printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
  printFloat(gps.course.deg(), gps.course.isValid(), 7, 2);
  printFloat(gps.speed.kmph(), gps.speed.isValid(), 6, 2);
  printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.deg()) : "*** ", 6);*/

  Serial.println("Schommel:");
  Serial.print("   Distance to waypoint1: ");
  Serial.println(distanceToWaypoint(way1_LAT, way1_LON));
  Serial.print("   course to waypoint1: ");
  Serial.println(courseToWaypoint(way1_LAT, way1_LON));
  Serial.println();

  /* Serial.println("Huis:");
   Serial.print("   Distance to waypoint2: ");
   Serial.println(distanceToWaypoint(way2_LAT, way2_LON));
   Serial.print("   course to waypoint2: ");
   Serial.println(courseToWaypoint(way2_LAT, way2_LON));
   Serial.println();

   Serial.println("Loods:");
   Serial.print("   Distance to waypoint3: ");
   Serial.println(distanceToWaypoint(way3_LAT, way3_LON));
   Serial.print("   course to waypoint3: ");
   Serial.println(courseToWaypoint(way3_LAT, way3_LON));
   Serial.println();

   Serial.println("huidige locatie:");
   Serial.println(gps.location.lat(),7);
   Serial.print(" , ");
   Serial.println(gps.location.lng(),7);
   Serial.println();*/

  smartDelay(1000);

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));

}













/* GPS functies om te zorgen voor koers en afstand tot waypoint
 */
unsigned long distanceToWaypoint(float way_LAT, float way_LON) {
  unsigned long distanceToWaypoint =
    (unsigned long)TinyGPSPlus::distanceBetween(
      gps.location.lat(),
      gps.location.lng(),
      way_LAT,
      way_LON);
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
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}


