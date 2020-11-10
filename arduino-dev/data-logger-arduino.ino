
/*
   Description:
   This is the first program used to test the bathymetry mapping of "Gould's Cove" on Pushaw Lake, Orono ME.

   Notes:
   - The latitude and longitude values written to the SD card are in the form 'ddmm.mmmm', known as degree and decimal
     minute (DMM) format. These were converted to decimal degree (DD) format in excel after data-logging to be successfully
     mapped in QGIS. This format conversion should be done in here.
   - The incoming sonar data is in millimeters, but is converted in here to feet using the '.toInt()' function and dividing
     by 305. This rounds the result to the nearest integer, which can be fixed to get a more accurate reading in feet for QGIS.

   IDE: Arduino
*/

#include <Adafruit_GPS.h>
#include <SD.h>
#include <SPI.h>
#include "ping1d.h"

Adafruit_GPS GPS(&Serial1);
static Ping1D ping { Serial };

// Variables for first two GPS NMEA sentences
String NMEA1;
String NMEA2;
// Variable for depthPoint reading
String depthPoint;
// To read characters coming from the GPS
char c;

int chipSelect = 4;
File sensorData;


void setup() {

  // Set baud rate of Ping sonar and GPS (default rate for both)
  Serial.begin(9600);
  GPS.begin(9600);

  // Set update rate to 1 Hz
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  // Only grab RMC and GGA NMEA sentences
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

  // Request updates on antenna status, comment out to keep quiet
  // GPS.sendCommand(PGCMD_ANTENNA);

  if (!SD.begin(chipSelect)) {
    //Serial.println("initialization failed!");
    while (1);
  }

  // Check to see if files are on SD card, if so, delete them
  if (SD.exists("NMEA.txt")) {
    SD.remove("NMEA.txt");
  }
  if (SD.exists("GPSData.txt")) {
    SD.remove("GPSData.txt");
  }

  // Initialize ping sensor to update every 1000ms
  // TODO: error checking
  ping.initialize(1000);

  // Hard-code header for "GPSData.txt"
  // TODO: dynamically update header, or place hemispherical letters appropiately
  sensorData = SD.open("GPSData.txt", FILE_WRITE);
  sensorData.println("N,W,depthPoint");
  sensorData.close();

  delay(1000);
}


void loop() {

  // Reads two NMEA sentences from GPS
  readGPS();

  // Only log data once have fix
  if (GPS.fix == 1) {
    // This opens AND creates the file
    sensorData = SD.open("NMEA.txt", FILE_WRITE);
    sensorData.print(NMEA1);
    sensorData.println(NMEA2);
    sensorData.close();

    sensorData = SD.open("GPSData.txt", FILE_WRITE);
    // Write latitude and longitude to 4 decimal places
    sensorData.print(GPS.latitude, 4);
    // N or S hemisphere
    //sensorData.print(GPS.lat);
    sensorData.print(",");
    sensorData.print(GPS.longitude, 4);
    // E or W hemisphere
    //sensorData.print(GPS.lon);
    sensorData.print(",");
    // Convert depth in mm to feet
    // TODO: fix rounding
    sensorData.println(depthPoint.toInt() / 305);
    sensorData.close();
  }

}

void readGPS() {

  while (!GPS.newNMEAreceived() && !ping.update()) { // Loop until you have a good NMEA sentence
    c = GPS.read();
  }
  depthPoint = ping.distance();
  GPS.parse(GPS.lastNMEA()); // Parse that last good NMEA sentence
  NMEA1 = GPS.lastNMEA();

  while (!GPS.newNMEAreceived()) { // Loop until you have a good NMEA sentence
    c = GPS.read();
  }
  GPS.parse(GPS.lastNMEA()); // Parse that last good NMEA sentence
  NMEA2 = GPS.lastNMEA();

  //Serial.print(NMEA1);
  //Serial.println(NMEA2);
}
