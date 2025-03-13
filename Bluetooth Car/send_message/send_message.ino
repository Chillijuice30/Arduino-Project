#include <Arduino.h>
#include <SoftwareSerial.h> // Include the SoftwareSerial library for software serial communication with NeoGPS
#include <stdlib.h> // for exit()

// Motor Driver Pins
#define IN1 3
#define IN2 5
#define IN3 6
#define IN4 11

// Force sensor pin
#define FORCE_SENSOR A0

// Ultrasonic sensor pins
#define TRIG_PIN 2
#define ECHO_PIN 4

// NeoGPS module pins (RX, TX)
#define NEOGPS_RX 9
#define NEOGPS_TX 10

// SIM800L module pins (RX, TX)
#define SIM800L_RX 7
#define SIM800L_TX 8

// Speed range: 0 - 255
int Speed = 204;

// Obstacle detection variables
long duration;
int distance = 0;
int forceValue = 0;
bool obstacleDetected = false;
bool crashDetected = false;

// Variables for non-blocking motor control
unsigned long motorStartTime = 0;
unsigned long motorDuration = 1000; // Set the motor run time in milliseconds

// Create software serial objects for NeoGPS and SIM800L
SoftwareSerial neoGPS(NEOGPS_RX, NEOGPS_TX);
SoftwareSerial sim800l(SIM800L_RX, SIM800L_TX);

void setup() {
  // Pin mode setup
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(FORCE_SENSOR, INPUT);
  
  // Begin serial communication
  Serial.begin(9600); // Set baud rate for Serial communication
  neoGPS.begin(9600); // Initialize NeoGPS software serial
  sim800l.begin(9600); // Initialize SIM800L software serial
}

void loop() {
  // Read Bluetooth input
  if (Serial.available() > 0) {
    char command = Serial.read();

    // Perform action based on command received
    switch (command) {
      case 'F':
        forward();
        break;
      case 'B':
        back();
        break;
      case 'L':
        left();
        break;
      case 'R':
        right();
        break;
      case 'S':
        stop();
        break;
    }
  }

  // Read force sensor value
  forceValue = analogRead(FORCE_SENSOR);

  // Check for obstacle using ultrasonic sensor
  detectObstacle();

  // If obstacle detected or crash detected, stop the vehicle
  if (obstacleDetected || crashDetected) {
    stop();
  }

  // If force sensor threshold crossed, send message through SIM800L
  if (forceValue > 300) {
    sendSMS("Force sensor threshold crossed. Car crashed!");
    delay(5000); // Wait for 5 seconds before sending another message
  }

  // Print distance and vehicle state
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm, Vehicle state: ");
  Serial.println(obstacleDetected || crashDetected ? "Stopped" : "Running");
}

// Motor control functions
void forward() {
  analogWrite(IN3, Speed);
  analogWrite(IN2, Speed);
  motorStartTime = millis(); // Record the start time for motor execution
}

void back() {
  analogWrite(IN4, Speed);
  analogWrite(IN1, Speed);
  motorStartTime = millis(); // Record the start time for motor execution
}

void left() {
  analogWrite(IN1, Speed);
  analogWrite(IN3, Speed);
  motorStartTime = millis(); // Record the start time for motor execution
}

void right() {
  analogWrite(IN2, Speed);
  analogWrite(IN4, Speed);
  motorStartTime = millis(); // Record the start time for motor execution
}

void stop() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}

// Function to detect obstacles using ultrasonic sensor and force sensor
void detectObstacle() {
  // Ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;
  obstacleDetected = (distance < 15);

  // Force sensor
  forceValue = analogRead(FORCE_SENSOR);
  crashDetected = (forceValue > 300);
}

// Function to send SMS using SIM800L module
void sendSMS(String message) {
  sim800l.println("AT+CMGF=1"); // Set SMS mode to text
  delay(1000);
  sim800l.print("AT+CMGS=\"+7757017761\"\r"); // Replace +1234567890 with your phone number
  delay(1000);
  sim800l.println(message); // Send the message
  delay(1000);
  sim800l.println((char)26); // End the message with CTRL+Z
  delay(1000);
}
