#include <ESP32Servo.h>

const int servoPin = 13; 
Servo myServo;

// Movement variables for sweeping
int currentAngle = 90;
int sweepDirection = 1;         // 1 for increasing angle, -1 for decreasing
unsigned long lastSweepTime = 0;
const int sweepInterval = 10;   // Speed of the sweep in milliseconds (lower = faster)
const int maxAngle = 135;       // Right limit of the sweep
const int minAngle = 45;        // Left limit of the sweep

// State and timeout management
unsigned long lastCommandTime = 0;
const unsigned long timeoutMillis = 500; // Time (ms) to wait before assuming out of frame
char currentState = '2';        // '1' = Man, '0' = Woman, '2' = No one/Timeout

void setup() {
  Serial.begin(115200);
  myServo.attach(servoPin);
  myServo.write(90); // Start at center
  Serial.println("ESP32 Ready. Waiting for serial data...");
}

void loop() {
  // 1. Read incoming serial data
  if (Serial.available() > 0) {
    char incomingByte = Serial.read();
    
    // Update state and reset the timer if a valid command is received
    if (incomingByte == '1' || incomingByte == '0') {
      currentState = incomingByte;
      lastCommandTime = millis(); 
    }
  }

  // 2. Check if the face went out of frame (Timeout)
  if (millis() - lastCommandTime > timeoutMillis) {
    currentState = '2'; // Reset to neutral state
  }

  // 3. Execute servo movement based on the current state
  unsigned long currentMillis = millis();
  
  if (currentState == '1') {
    // Man detected: Sweep back and forth continuously
    if (currentMillis - lastSweepTime >= sweepInterval) {
      lastSweepTime = currentMillis;
      currentAngle += sweepDirection;
      
      // Reverse direction at the limits
      if (currentAngle >= maxAngle) {
        sweepDirection = -1;
      } else if (currentAngle <= minAngle) {
        sweepDirection = 1;
      }
      
      myServo.write(currentAngle);
    }
  } 
  else if (currentState == '0') {
    // Woman detected: Move to a fixed position (0 degrees)
    currentAngle = 0;
    myServo.write(currentAngle);
  } 
  else if (currentState == '2') {
    // No one detected (Timeout): Return to center and stop
    currentAngle = 90;
    myServo.write(currentAngle);
  }
}