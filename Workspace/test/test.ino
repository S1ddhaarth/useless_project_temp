#include <ESP32Servo.h>

const int servoPin = 13; 
Servo myServo;

// Movement variables for sweeping (full 180° rotation: 0 <-> 180)
int currentAngle = 90;
int sweepDirection = 1;         // 1 for increasing angle, -1 for decreasing
unsigned long lastSweepTime = 0;
const int sweepInterval = 10;   // Speed of the sweep in milliseconds (lower = faster)
const int maxAngle = 180;       // End of a full rotation
const int minAngle = 0;         // Start of a full rotation

// State and timeout management
unsigned long lastCommandTime = 0;
const unsigned long timeoutMillis = 500; // Time (ms) to wait before assuming out of frame
char currentState = '2';        // '1' = Man, '0' = Woman, '2' = No one/Timeout

// Rotation management
bool rotating = false;          // True while a 180° rotation is in progress

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

      // Start a new rotation if one isn't already in progress
      if (!rotating) {
        rotating = true;
      }
    }
  }

  // 2. Check if the face went out of frame (Timeout)
  //    Only update state — do NOT stop a rotation in progress
  if (millis() - lastCommandTime > timeoutMillis) {
    currentState = '2'; // Reset to neutral state
  }

  // 3. Execute servo movement based on the current state
  unsigned long currentMillis = millis();

  if (rotating) {
    // A rotation is in progress — always complete it regardless of currentState
    if (currentMillis - lastSweepTime >= sweepInterval) {
      lastSweepTime = currentMillis;
      currentAngle += sweepDirection;

      // Clamp and mark rotation complete when an endpoint is reached
      if (currentAngle >= maxAngle) {
        currentAngle = maxAngle;
        sweepDirection = -1;  // Next rotation goes the other way
        rotating = false;     // This rotation is complete

        // If a face is still detected, immediately start the return rotation
        if (currentState == '1' || currentState == '0') {
          rotating = true;
        }
      } else if (currentAngle <= minAngle) {
        currentAngle = minAngle;
        sweepDirection = 1;   // Next rotation goes the other way
        rotating = false;     // This rotation is complete

        // If a face is still detected, immediately start the return rotation
        if (currentState == '1' || currentState == '0') {
          rotating = true;
        }
      }

      myServo.write(currentAngle);
    }
  }
  else if (currentState == '2') {
    // No rotation in progress and no one detected: return to center
    if (currentAngle != 90) {
      if (currentMillis - lastSweepTime >= sweepInterval) {
        lastSweepTime = currentMillis;
        if (currentAngle < 90) {
          currentAngle++;
        } else {
          currentAngle--;
        }
        myServo.write(currentAngle);
      }
    }
  }
}