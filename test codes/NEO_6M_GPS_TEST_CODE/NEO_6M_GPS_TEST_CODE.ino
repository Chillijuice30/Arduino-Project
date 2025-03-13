#include <SoftwareSerial.h>

// Create a SoftwareSerial object to communicate with the NEO-6M GPS module
SoftwareSerial gpsSerial(9, 7); // RX, TX pins

void setup() {
  // Start serial communication
  Serial.begin(9600);
  gpsSerial.begin(9600);
}

void loop() {
  // Read data from the GPS module
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    Serial.write(c);
  }
  
  // Check if any data is available from the GPS module and print it to the serial monitor
  if (gpsSerial.find("$GPGGA")) { // Check for the GPGGA sentence which contains essential GPS data
    String GPGGA = gpsSerial.readStringUntil('\n');
    Serial.println(GPGGA);
  }
  
  delay(1000); // Wait for a second before reading again
}
