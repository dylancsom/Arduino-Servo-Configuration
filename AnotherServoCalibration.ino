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

// Starting angles for the servos
int shoulderStart = 90; // Starting angle for the shoulder (0-270)
int elbowStart = 90;    // Starting angle for the elbow (0-270)
int wristStart = 90;     // Starting angle for the wrist (0-180)
int baseStart = 0;     // Starting angle for the base (0-270)

// Delay between each incremental movement (in milliseconds)
const int moveDelay = 2;

void setup() {
  // Attach the servo objects to their respective pins
  shoulderServo.attach(shoulderPin);
  elbowServo.attach(elbowPin);
  wristServo.attach(wristPin);
  baseServo.attach(basePin);

  Serial.begin(9600);
  Serial.println("Servo Calibration Mode:");
  Serial.println("Commands: 's' (shoulder), 'e' (elbow), 'w' (wrist), 'b' (base)");
  Serial.println("Enter an angle (0-270 for 270° servos, 0-180 for wrist servo).");
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();  
    Serial.read(); // Clear newline

    Serial.println("Enter angle:");
    while (Serial.available() == 0);

    int angle = Serial.parseInt();
    Serial.read(); // Clear newline

    int pulseWidth;

    switch (command) {
      case 's':  // Shoulder (270° servo)
        if (angle >= 0 && angle <= 270) {
          pulseWidth = map(angle, 0, 270, minPulse_shoulder, maxPulse_shoulder);
          gradualMove(shoulderServo, pulseWidth);
          Serial.print("Shoulder set to: ");
        } else Serial.println("Invalid angle! 0-270 only.");
        break;

      case 'e':  // Elbow (270° servo)
        if (angle >= 0 && angle <= 270) {
          pulseWidth = map(angle, 0, 270, minPulse_elbow, maxPulse_elbow);
          gradualMove(elbowServo, pulseWidth);
          Serial.print("Elbow set to: ");
        } else Serial.println("Invalid angle! 0-270 only.");
        break;

      case 'w':  // Wrist (180° servo)
        if (angle >= 0 && angle <= 270) {
          pulseWidth = map(angle, 0, 270, minPulse_wrist, maxPulse_wrist);
          gradualMove(wristServo, pulseWidth);
          Serial.print("Wrist set to: ");
        } else Serial.println("Invalid angle! 0-270 only.");
        break;

      case 'b':  // Base (270° servo)
        if (angle >= 0 && angle <= 180) {
          pulseWidth = map(angle, 0, 180, minPulse_base, maxPulse_base);
          gradualMove(baseServo, pulseWidth);
          Serial.print("Base set to: ");
        } else Serial.println("Invalid angle! 0-180 only.");
        break;

      default:
        Serial.println("Invalid command! Use 's', 'e', 'w', or 'b'.");
        return;
    }

    Serial.println(angle);
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
