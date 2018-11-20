#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 0, TXPin = 1;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

float way_lat = 51.646918;
float way_lon = 5.545337;
float lat;
float lon;
int sat;
int HDOP;

void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);
}

void getGPS() {
  while (ss.available() > 0) {
    gps.encode(ss.read());

    if (gps.location.isUpdated()) {
      lat = gps.location.lat();
      lon = gps.location.lng();
      sat = gps.satellites.value();
      HDOP = gps.hdop.value();
    }
  }
}

float bearing() {

  float teta1 = radians(lat);
  float teta2 = radians(way_lat);
  float delta1 = radians(way_lat - lat);
  float delta2 = radians(way_lon - lon);

  float y = sin(delta2) * cos(teta2);
  float x = cos(teta1) * sin(teta2) - sin(teta1) * cos(teta2) * cos(delta2);
  float brng = atan2(y, x);
  brng = degrees(brng);// radians to degrees
  brng = ( ((int)brng + 360) % 360 );

  while (gps.satellites.value() >= 4) {
    Serial.print("Current position: ");
    Serial.print(lat, 6);
    Serial.print(",");
    Serial.println(lon, 6);

    Serial.print("Target position:  ");
    Serial.print(way_lat, 6);
    Serial.print(",");
    Serial.println(way_lon, 6);

    Serial.print("Heading GPS:      ");
    Serial.print(brng);
    Serial.println(" degrees");

    Serial.print("Satellite locks:  ");
    Serial.println(gps.satellites.value());
    Serial.println("=================================================");
    delay(2000);

  }
  Serial.print("Searching for satallites (");
  Serial.print(gps.satellites.value());
  Serial.println("/4)");
  delay(500);
  Serial.print(".");
  delay(500);
  Serial.print(".");
  delay(500);
  Serial.println(".");
  delay(500);
}


void loop() {

  getGPS();
  bearing();
  //Serial.println(gps.satellites.value());


}
