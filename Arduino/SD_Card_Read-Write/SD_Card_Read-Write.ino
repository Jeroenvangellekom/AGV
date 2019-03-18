#include <SPI.h>
#include <SD.h>
File SD_File;

int CS_PIN = 10;

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Initialize_SD_Card(CS_PIN);
  Write_Single_Line_To_TXT("DATA.txt", "SAMPLE TEXT");
  Readout_TXT_File("DATA.txt");
}

void loop()
{

}

//Start SD card communication
//Call this function once in Setup
void Initialize_SD_Card(int CS_PIN)
{
  if (!SD.begin(CS_PIN))
  {
    Serial.println("SD initialization failed!");
    return;
  }
  Serial.println("SD initialization done.\r\n");
}

//Writes string to now row in file
void Write_Single_Line_To_TXT(String File_Name, String string)
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
    SD_File.close();
  }
  else
  {
    Serial.println("error reading txt file");
  }
}

