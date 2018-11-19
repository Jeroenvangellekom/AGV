double GPS_LON() 
{
  // print the string when a newline arrives:
  if (stringComplete) 
  {
    String GPSID = inputString.substring(0, 6);
    if (GPSID == "$GPGGA") 
    {
      //gets the "Before Decimal" part of the string that is the degrees in the coordinate
      String LON_bd_str = inputString.substring(17, 19);
      float LON_bd = LON_bd_str.toInt();

      //gets the "After Decimal" part of the string and converts it to a decimal degree
      String LON_ad_str = inputString.substring(19, 27);
      LON_ad_str.remove(2, 1);
      float LON_ad_fl = LON_ad_str.toInt();
      double LON_ad = LON_ad_fl / 6000000.0;

      //adds the degree to the decimal part to get a correct decimal coordinate
      double LON = LON_bd + LON_ad;

      return (LON);
    }
  }
}

double GPS_LAT() 
{
  if (stringComplete) 
  {
    String GPSID = inputString.substring(0, 6);
    if (GPSID == "$GPGGA") 
    {
      //gets the "Before Decimal" part of the string that is the degrees in the coordinate
      String LAT_bd_str = inputString.substring(31, 33);
      int LAT_bd = LAT_bd_str.toInt();

      //gets the "After Decimal" part of the string and converts it to a decimal degree
      String LAT_ad_str = inputString.substring(33, 41);
      LAT_ad_str.remove(2, 1);
      float LAT_ad_fl = LAT_ad_str.toInt();
      double LAT_ad = LAT_ad_fl / 6000000.0;

      //adds the degree to the decimal part to get a correct decimal coordinate
      float LAT = LAT_bd + LAT_ad;

      return (LAT);
    }
  }
}

String GPS_SAT() // nog naar integer
{

  if (stringComplete) 
  {
    String GPSID = inputString.substring(0, 6);
    if (GPSID == "$GPGGA") 
    {
      return (inputString.substring(46, 48);
    }
  }
}

String GPS_Precision() //nog naar integer
{

  if (stringComplete)
  {
    String GPSID = inputString.substring(0, 6);
    if (GPSID == "$GPGGA")
    {
      return (inputString.substring(49, 53);
    }
  }
}

