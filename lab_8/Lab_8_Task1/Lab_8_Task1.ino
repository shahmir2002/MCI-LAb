// Motor A (Left Motor) connections
int enA = 26;   // ENABLE pin (PD3) for Left Motor to provide PWM
int in1A = 11;  // IN1 pin (PA2) for Left Motor direction
int in2A = 12;  // IN2 pin (PA3) for Left Motor direction

// Motor B (Right Motor) connections
int enB = 25;   // ENABLE pin (PD2) for Right Motor to provide PWM
int in1B = 13;  // IN1 pin (PA4) for Right Motor direction
int in2B = 14;  // IN2 pin (PA5) for Right Motor direction

void setup() {
  Serial.begin(115200); // Serial initializing for Debugging

  // Setup pinModes for Left Motor
  pinMode(in1A, OUTPUT);
  pinMode(in2A, OUTPUT);
  pinMode(enA, OUTPUT);

  // Setup pinModes for Right Motor
  pinMode(in1B, OUTPUT);
  pinMode(in2B, OUTPUT);
  pinMode(enB, OUTPUT);

  // Initialize motors OFF
  stopMotors();
}

void loop() {
  // Move the robot in a straight line for 5 seconds
  moveStraight(255); // Move forward with maximum speed
  delay(5000);       // Wait for 5 seconds

  // Stop the motors
  stopMotors();
  delay(2000);       // Wait for 2 seconds

//  // Move backward in a straight line for 5 seconds
//  moveStraight(-255); // Move backward with maximum speed
//  delay(5000);
//
//  // Stop the motors
//  stopMotors();
//  delay(2000);
}

void moveStraight(int speed) {
  if (speed > 0) {
    // Forward direction
    digitalWrite(in1A, HIGH);
    digitalWrite(in2A, LOW);  // Left motor forward
    digitalWrite(in1B, HIGH);
    digitalWrite(in2B, LOW);  // Right motor forward
  } else {
    // Backward direction
    digitalWrite(in1A, LOW);
    digitalWrite(in2A, HIGH);  // Left motor backward
    digitalWrite(in1B, LOW);
    digitalWrite(in2B, HIGH);  // Right motor backward
    speed = -speed;            // Convert speed to positive for analogWrite
  }

  // Set the speed of both motors (same speed for straight-line motion)
  analogWrite(enA, speed);  // Set speed for Left Motor
  analogWrite(enB, speed);  // Set speed for Right Motor
}

void stopMotors() {
  // Stop both motors
  analogWrite(enA, 0);      // Left Motor stop
  analogWrite(enB, 0);      // Right Motor stop
  digitalWrite(in1A, LOW);
  digitalWrite(in2A, LOW);  // Left motor stop
  digitalWrite(in1B, LOW);
  digitalWrite(in2B, LOW);  // Right motor stop
}
