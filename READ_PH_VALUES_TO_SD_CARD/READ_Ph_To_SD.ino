#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include <TinyGPS++.h>

//Pin Defenition PH Meter
int CS_PIN = 10; //Chip Select pin for PH sensor
int PH_PIN = A0; //Analog input pin for PH sensor

//Pin Defenition GPS
int GPS_RX = 2; //RX pin for GPS
int GPS_TX = 3; //TX pin for GPS

//Global Varibles
float PH_RAW = 0;
float PH_CALC = 0;

File SD_File;
TinyGPSPlus gps;
SoftwareSerial GPS_SERIAL(GPS_RX, GPS_TX);

void setup()
{
  //PinModes
  pinMode(PH_PIN, INPUT);
  pinMode(GPS_RX, INPUT);
  pinMode(GPS_TX, INPUT);

  //Start Serial Communication between laptop and arduino
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Serial communication for laptop is active");

  //Start Serial Communication between GPS and arduino
  GPS_SERIAL.begin(9600);
  while (!GPS_SERIAL)
  {
    ; //wait for GPS_SERIAL to connect
  }
  Serial.println("Serial communication for GPS is active");

  //Inititlize SD Card
  Initialize_SD_Card(CS_PIN);
  Write_String_To_TXT("DATA.txt", "========== REBOOT VAN ARDUINO ==========");

  delay(2000);
  Serial.println("Longitude, Latitude, Date, Time, Available Sattellites, Hdop, PH Raw, PH Calculated");
}

void loop()
{
  //Measure and calculate PH
  PH_RAW = analogRead(PH_PIN);
  PH_CALC = (-0.03 * PH_RAW) + 31; //values are calclated to calibrate outcome
  
  //Clear Data_string
  String Data_string = "";

  //Add GPS data to Data_string
  Data_string += String(gps.location.lng(), 7);
  Data_string += F(", ");
  Data_string += String(gps.location.lat(), 7);
  Data_string += F(", ");
  Data_string += String(gps.date.day());
  Data_string += F("-");
  Data_string += String(gps.date.month());
  Data_string += F("-");
  Data_string += String(gps.date.year());
  Data_string += F(", ");
  Data_string += String(gps.time.hour() + 1);
  Data_string += F(":");
  Data_string += String(gps.time.minute());
  Data_string += F(":");
  Data_string += String(gps.time.second());
  Data_string += F(", ");
  Data_string += String(gps.satellites.value());
  Data_string += F(", ");
  Data_string += String(gps.hdop.value());
  Data_string += F(", ");

  //Put PH data in Data_sting
  Data_string += String(PH_RAW);
  Data_string += F(", ");
  Data_string += String(PH_CALC);

  //Print and write Data_string to serial monitor and txt file
  Serial.println(Data_string);
  if (gps.satellites.value() >= 3) //Only print when 3 or more satellites are available
  {
    Write_String_To_TXT("DATA.txt", Data_string);
  }

  //Update GPS and Delay Code
  smartDelay(1);
  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
}









////////////////////////////////////////////////////////////////////////////
/////////////////////////////////FUNCTIONS//////////////////////////////////
////////////////////////////////////////////////////////////////////////////

//Start SD card communication. Call this function once in Setup
void Initialize_SD_Card(int CS_PIN)
{
  if (!SD.begin(CS_PIN))
  {
    Serial.println("SD initialization failed!");
    return;
  }
  Serial.println("SD initialization sucesfull.\r\n");
}

//Writes string to new row in file
void Write_String_To_TXT(String File_Name, String string)
{
  SD_File = SD.open(File_Name, FILE_WRITE); //Opens .TXT file
  if (SD_File)
  {
    SD_File.println(string);               //Write sting into new line of .TXT file
    SD_File.close();
  }
  else
  {
    Serial.println("error writing to txt file");
  }
}

//Writes string to now row in file
void Write_Float_To_TXT(String File_Name, float x)
{
  SD_File = SD.open(File_Name, FILE_WRITE); //Opens .TXT file
  if (SD_File)
  {
    SD_File.println(x);               //Write sting into new line of .TXT file
    SD_File.close();
  }
  else
  {
    Serial.println("error writing to txt file");
  }
}

//Reads out complete file to serial monitor
void Readout_TXT_File(String File_Name)
{
  SD_File = SD.open(File_Name);
  if (SD_File) {
    Serial.println("========================");
    Serial.print("Opening file: ");
    Serial.println(File_Name);
    Serial.print("========================");
    Serial.println();
    while (SD_File.available())
    {
      Serial.write(SD_File.read());
    }
    Serial.println("========================");
    Serial.print("Close file: ");
    Serial.println(File_Name);
    Serial.print("========================");
    Serial.println();
    SD_File.close();
  }
  else
  {
    Serial.println("error reading txt file");
  }
}

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (GPS_SERIAL.available())
      gps.encode(GPS_SERIAL.read());
  } while (millis() - start < ms);
}

