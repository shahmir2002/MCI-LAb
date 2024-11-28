#ifndef UART5_H
#define UART5_H

#include <stdint.h>
#include "TM4C123GH6PM.h" // Include the microcontroller-specific header

void UART5_init(void);            // Function to initialize UART5
void UART5_Transmitter(uint8_t data); // Function to transmit a byte of data
void printstring(char *str);      // Function to send a string
void Delay_MicroSecond(int time);

#endif // UART5_H
