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
char lastTriggeredGender = ' '; // Tracks which gender last triggered a rotation

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

      // Start a new rotation only if the detected gender is different
      // from the one that last triggered a rotation
      if (!rotating && incomingByte != lastTriggeredGender) {
        rotating = true;
        lastTriggeredGender = incomingByte;
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
        rotating = false;     // Stop — wait for opposite gender
      } else if (currentAngle <= minAngle) {
        currentAngle = minAngle;
        sweepDirection = 1;   // Next rotation goes the other way
        rotating = false;     // Stop — wait for opposite gender
      }

      myServo.write(currentAngle);
    }
  }
  // When not rotating, servo stays at its current position
}