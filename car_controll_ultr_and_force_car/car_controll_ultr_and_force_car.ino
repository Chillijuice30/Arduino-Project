#include <Arduino.h>

// Motor Driver Pins
#define IN1 3
#define IN2 5
#define IN3 6
#define IN4 11
#define force A0
// Bluetooth control pins
#define bluetoothTx 0
#define bluetoothRx 1

// Ultrasonic sensor pins
#define TRIG_PIN 2
#define ECHO_PIN 4


// Speed range: 0 - 255
int Speed = 204;

// Obstacle detection variables
long duration;
int distance = 0;
boolean obstacleDetected = false;
boolean crash = false;
int force_1 =0;
// Variables for non-blocking motor control
unsigned long motorStartTime = 0;
unsigned long motorDuration = 1000; // Set the motor run time in milliseconds

void setup() {
  // Pin mode setup
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
pinMode(force, INPUT);
  Serial.begin(9600); // Set baud rate for Serial communication
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
        Stop();
        break;
    }
  }

  // Check for obstacle using ultrasonic sensor
  detectObstacle();

  // If obstacle detected, stop the vehicle
  if (obstacleDetected) {
    Stop();
  }
  if (crash)
  {
    Stop();
    Stop();
    Stop();
    }
  // Check if motor duration has elapsed and stop motors
  if (millis() - motorStartTime >= motorDuration) {
    Stop();
  }

  // Print distance and vehicle state
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm, Vehicle state: ");
  Serial.println(obstacleDetected ? "Stopped" : "Running");
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

void Stop() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}

// Function to detect obstacles using ultrasonic sensor
void detectObstacle() {
  // Send ultrasonic sensor pulse
  force_1=analogRead(force);
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Read the echoPin, return the sound wave travel time in microseconds
  duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance
  distance = duration * 0.034 / 2;

  // Check if an obstacle is detected within a specific range
  obstacleDetected = (distance < 15);
  crash=(force_1 >300);
}
