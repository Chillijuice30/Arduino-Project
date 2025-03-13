#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// Define the GPS serial connection pins
#define GPS_RX_PIN 9
#define GPS_TX_PIN 10

// Create a SoftwareSerial object for the GPS module
SoftwareSerial gpsSerial(GPS_RX_PIN, GPS_TX_PIN);

// Create a TinyGPS++ object
TinyGPSPlus gps;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  gpsSerial.begin(9600);

  Serial.println("Neo 6M GPS Module Test");
}

void loop() {
  // Read GPS data
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      // If new GPS data is parsed successfully, print the data
      printGPSData();
    }
  }
}

void printGPSData() {
  // Check if there is valid GPS data available
  if (gps.location.isValid()) {
    // Print latitude and longitude
    Serial.print("Latitude: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(", Longitude: ");
    Serial.println(gps.location.lng(), 6);

    // Print altitude
    Serial.print("Altitude (meters): ");
    Serial.println(gps.altitude.meters());

    // Print speed over ground (km/h)
    Serial.print("Speed (km/h): ");
    Serial.println(gps.speed.kmph());

    // Print date and time
    if (gps.date.isValid() && gps.time.isValid()) {
      Serial.print("Date: ");
      Serial.print(gps.date.month());
      Serial.print("/");
      Serial.print(gps.date.day());
      Serial.print("/");
      Serial.print(gps.date.year());
      Serial.print(" Time: ");
      Serial.print(gps.time.hour());
      Serial.print(":");
      Serial.print(gps.time.minute());
      Serial.print(":");
      Serial.print(gps.time.second());
      Serial.println(" UTC");
    }
  } else {
    // Print "No GPS data" if valid data is not available
    Serial.println("No GPS data");
  }
}
