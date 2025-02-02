#include <Servo.h>

// Declare servo objects
Servo shoulderServo;
Servo elbowServo;
Servo wristServo;
Servo baseServo;

// Pin assignments
const int shoulderPin = 9;
const int elbowPin = 10;
const int wristPin = 11;
const int basePin = 6;

// Custom pulse width ranges for each individual servo
// Shoulder Servo (270°)
const int minPulse_shoulder = 400;
const int maxPulse_shoulder = 2600;

// Elbow Servo (270°)
const int minPulse_elbow = 350;
const int maxPulse_elbow = 2700;

// Wrist Servo (Custom pulse range for 180°)
const int minPulse_wrist = 800;
const int maxPulse_wrist = 2100;

// Base Servo (270°)
const int minPulse_base = 400;
const int maxPulse_base = 2600;

// Delay between each incremental movement (in milliseconds)
const int moveDelay = 3;

void setup() {
  // Attach the servo objects to their respective pins
  shoulderServo.attach(shoulderPin);
  elbowServo.attach(elbowPin);
  wristServo.attach(wristPin);
  baseServo.attach(basePin);
  
  Serial.begin(9600);
  Serial.println("Servo Calibration Mode:");
  Serial.println("Commands: 's' (shoulder), 'e' (elbow), 'w' (wrist), 'b' (base)");
  Serial.println("Enter angles in degrees (base rotation: 0-90, shoulder/elbow/wrist: 0-270).");
  
  // Set initial positions to match Python script
  // Initial base rotation: 45 degrees
  // Initial joint angles: 90 degrees
  int initialBasePulse = map(45, 0, 90, minPulse_base, maxPulse_base);
  int initialShoulderPulse = map(90, 0, 180, minPulse_shoulder, maxPulse_shoulder);
  int initialElbowPulse = map(90, 0, 270, minPulse_elbow, maxPulse_elbow);
  int initialWristPulse = map(90, 0, 270, minPulse_wrist, maxPulse_wrist);
  
  // Move servos to initial positions
  baseServo.writeMicroseconds(initialBasePulse);
  shoulderServo.writeMicroseconds(initialShoulderPulse);
  elbowServo.writeMicroseconds(initialElbowPulse);
  wristServo.writeMicroseconds(initialWristPulse);
}

void loop() {
  if (Serial.available() > 0) {
    // Read the command and joint angles from Python
    String command = Serial.readStringUntil(',');
    float baseRotation = Serial.parseFloat();
    float theta1 = Serial.parseFloat();
    float theta2 = Serial.parseFloat();
    float theta3 = Serial.parseFloat();
    Serial.read(); // Clear newline or extra characters
    
    // Map the angles to pulse width values
    int basePulse = map(baseRotation, 0, 180, minPulse_base, maxPulse_base);
    int shoulderPulse = map(theta1, 0, 180, minPulse_shoulder, maxPulse_shoulder);
    int elbowPulse = map(theta2, 0, 270, minPulse_elbow, maxPulse_elbow);
    int wristPulse = map(theta3, 0, 270, minPulse_wrist, maxPulse_wrist);
    
    // Send the appropriate commands to each servo
    gradualMove(baseServo, basePulse);
    gradualMove(shoulderServo, shoulderPulse);
    gradualMove(elbowServo, elbowPulse);
    gradualMove(wristServo, wristPulse);
    
    Serial.println("Movement completed.");
  }
}

// Function to move the servo gradually
void gradualMove(Servo &servo, int targetPulse) {
  int currentPulse = servo.readMicroseconds();
  
  // Move the servo towards the target pulse width gradually
  if (currentPulse < targetPulse) {
    for (int i = currentPulse; i <= targetPulse; i++) {
      servo.writeMicroseconds(i);
      delay(moveDelay);
    }
  } else {
    for (int i = currentPulse; i >= targetPulse; i--) {
      servo.writeMicroseconds(i);
      delay(moveDelay);
    }
  }
}
