#ifndef INFRARED_H
#define INFRARED_H

#include "TM4C123GH6PM.h"

// Define pin for IR sensor input (PF4)
#define IR_SENSOR_PIN (6 << 4)  // PF4

// Function prototypes
void IR_Sensor_Init(void);      // Initialize the IR sensor
int IR_Detect(void);            // Detect obstacle using IR sensor

#endif // INFRARED_H
