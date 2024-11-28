#include <stdio.h>
#include "UART.h"
#include "infrared.h"

#define IR_SENSOR_PIN (1 << 4)  // Define PF4 as IR sensor pin

// Initialize PF4 for IR sensor input
void IR_Sensor_Init(void) {
    SYSCTL->RCGCGPIO |= 0x20;  // Enable clock for PORTF (bit 5)
    GPIOF->DIR &= ~IR_SENSOR_PIN;  // Set PF4 as input
    GPIOF->DEN |= IR_SENSOR_PIN;   // Enable PF4 as a digital pin
}

// Read PF4 to check for obstacle presence
int IR_Detect(void) {
    return (GPIOF->DATA & IR_SENSOR_PIN) ? 1 : 0;
}
