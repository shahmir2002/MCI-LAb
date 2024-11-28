#include <Servo.h>
Servo myservo;  // Creating a Servo Object
const int servoPin = 26; // Pin at PD3

// Define the RGB pins on TivaC
const int red = 30;     // PF1
const int green = 40;   // PF2
const int blue = 39;    // PF3

char colorDetect = 0;   // For different color detection

void setup() {
  // Initialize Baud Rate at 9600
  Serial.begin(9600);

  // Specify pin modes for RGB pins
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  
  // Start with no color on RGB LEDs
  analogWrite(red, 0);
  analogWrite(green, 0);
  analogWrite(blue, 0);

  // Attach servo to specified pin
  myservo.attach(servoPin);
}
 
void loop() {
  // Check if data is available from Bluetooth terminal
  if (Serial.available() > 0) {
    // Read Bluetooth data and store it in colorDetect variable
    colorDetect = Serial.read();

    // Print received character for debugging
    if (colorDetect != '0') {
      Serial.print("Received command: ");
      Serial.println(colorDetect); 
    }  
  }

  // Call chooseColor function with colorDetect as argument
  chooseColor(colorDetect);
}

void chooseColor(char color) {
  // Set colors and servo position based on received character
  switch (color) {
    case 'A': // Red color, 45 degrees
      analogWrite(red, 255);
      analogWrite(green, 0);
      analogWrite(blue, 0);
      myservo.write(45);
      break;

    case 'B': // Green color, 90 degrees
      analogWrite(red, 0);
      analogWrite(green, 255);
      analogWrite(blue, 0);
      myservo.write(90);
      break;

    case 'C': // Blue color, 135 degrees
      analogWrite(red, 0);
      analogWrite(green, 0);
      analogWrite(blue, 255);
      myservo.write(135);
      Serial.print("We here");
      break;

    case 'D': // White color, 180 degrees
      analogWrite(red, 255);
      analogWrite(green, 255);
      analogWrite(blue, 255);
      myservo.write(180);
      
      break;

    default: // LEDs off, neutral servo position
      analogWrite(red, 0);
      analogWrite(green, 0);
      analogWrite(blue, 0);
      myservo.write(90); // Neutral position
      break;
  }
}
