#include <SoftwareSerial.h>

SoftwareSerial SIM800L(2, 3);  // RX, TX

void setup() {
  Serial.begin(9600);
  SIM800L.begin(9600);

  // SIM800L module setup commands
  SIM800L.println("AT");
  delay(1000);
  SIM800L.println("ATE1");
  delay(1000);
  SIM800L.println("AT+CPIN?");
  delay(1000);
  SIM800L.println("AT+CMGF=1");
  delay(1000);
  SIM800L.println("AT+CNMI=1,1,0,0,0");
  delay(1000);
}

void loop() {
  // SIM800L module communication
  while (SIM800L.available()) {
    Serial.println(SIM800L.readString());
  }

  while (Serial.available()) {
    SIM800L.println(Serial.readString());
  }
}
