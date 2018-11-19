#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 0, TXPin = 3;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);
float lat2 = 51.646918;
float lon2 = 5.545337;
 
float lat;
float lon;
int sat;
int HDOP;

void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);
}

float bearing(float lat, float lon, float lat2, float lon2) {

  float teta1 = radians(lat);
  float teta2 = radians(lat2);
  float delta1 = radians(lat2 - lat);
  float delta2 = radians(lon2 - lon);

  float y = sin(delta2) * cos(teta2);
  float x = cos(teta1) * sin(teta2) - sin(teta1) * cos(teta2) * cos(delta2);
  float brng = atan2(y, x);
  brng = degrees(brng);// radians to degrees
  brng = ( ((int)brng + 360) % 360 );
  Serial.print("Heading GPS: ");
  Serial.println(brng);

  return brng;


}
void loop() {
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0) {
    gps.encode(ss.read());
    if (gps.location.isUpdated()) {
      lat = gps.location.lat();
      lon = gps.location.lng();
      sat = gps.satellites.value();
      HDOP = gps.hdop.value();
    }
  }
  bearing(lat, lon, lat2, lon2);
  Serial.println(gps.satellites.value());
delay(500);
}
