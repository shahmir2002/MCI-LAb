#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <stdint.h>

// Ultrasonic sensor pins
extern const int echoPin;
extern const int trigPin;

// Desired distance from wall in centimeters
#define DESIRED_DISTANCE 5

void Ultrasonic_Init(void);                        // Initialize ultrasonic sensor pins
long measure_distance(int trigPin, int echoPin);    // Measure distance in cm
long microsecondsToCentimeters(long microseconds);  // Convert microseconds to cm


volatile uint32_t* get_distance(void);

void Trigger_Sensors(void);


// Declaration for pulseIn
long pulseIn(int pin, int value); // Adjust 'value' type based on your requirements (e.g., HIGH/LOW)

// Declaration for Motor_Init
void Motor_Init(void);

// Declaration for Ultrasonic_measure_Distance
uint32_t Measure_distance(void);

void TIMER0B_Handler(void);
void TIMER0A_Handler(void);
#endif // ULTRASONIC_H
