3const int forceSensorPin = A0; // Analog pin connected to the force sensor

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Read the analog input from the force sensor
  int forceValue = analogRead(forceSensorPin);
  
  // Print the force sensor value to the serial monitor
  Serial.print("Force Sensor Value: ");
  Serial.println(forceValue);
  
  delay(1000); // Wait for 1 second before taking the next reading
}
