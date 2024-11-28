#include "TM4C123.h"
#include <stdint.h>
#include <stdlib.h>

void Delay(unsigned long counter);             // Used to add delay
void HC05_init(void);                          // Initialize UART5 module for HC-05
char Bluetooth_Read(void);                     // Read data from Rx5 pin of TM4C123
void Bluetooth_Write(unsigned char data);      // Transmit a character to HC-05 over Tx5
void Bluetooth_Write_String(char *str);        // Transmit a string to HC-05 over Tx5

int main(void) {
  HC05_init(); // Initialize UART5 of TM4C123GH6PM for HC-05

  // Enable clock to GPIOF and set PF1, PF2, PF3 as digital output pins
  SYSCTL->RCGCGPIO |= 0x20;         // Enable clock to GPIOF
  GPIOF->DIR |= 0x0E;               // Set PF1, PF2, and PF3 as output
  GPIOF->DEN |= 0x0E;               // Enable digital function for PF1, PF2, and PF3
	GPIOF->DATA &= 0;
	Delay(5);

  while(1) {
    char c = Bluetooth_Read(); // Get a character from UART5

    // Check the received character and control the RGB LED
    if (c == 'A') { // Green LED ON
      GPIOF->DATA |= (1 << 3);           // Turn on PF3 (green LED)
      Bluetooth_Write_String("Green LED ON\n");
    } 
    else if (c == 'B') { // Green LED OFF
      GPIOF->DATA &= ~(1 << 3);          // Turn off PF3 (green LED)
      Bluetooth_Write_String("Green LED OFF\n");
    } 
    else if (c == 'C') { // Blue LED ON
      GPIOF->DATA |= (1 << 2);           // Turn on PF2 (blue LED)
      Bluetooth_Write_String("Blue LED ON\n");
    } 
    else if (c == 'D') { // Blue LED OFF
      GPIOF->DATA &= ~(1 << 2);          // Turn off PF2 (blue LED)
      Bluetooth_Write_String("Blue LED OFF\n");
    } 
    else if (c == 'E') { // Red LED ON
      GPIOF->DATA |= (1 << 1);           // Turn on PF1 (red LED)
      Bluetooth_Write_String("Red LED ON\n");
    } 
    else if (c == 'F') { // Red LED OFF
      GPIOF->DATA &= ~(1 << 1);          // Turn off PF1 (red LED)
      Bluetooth_Write_String("Red LED OFF\n");
    }
  }
}

// Initialization of UART5 for HC-05 Bluetooth communication
void HC05_init(void) {
  SYSCTL->RCGCUART |= 0x20;        // Enable clock to UART5
  SYSCTL->RCGCGPIO |= 0x10;        // Enable clock to PORTE for PE4/Rx and PE5/Tx
  Delay(1);

  UART5->CTL = 0;                  // Disable UART5 module
  UART5->IBRD = 104;               // Integer portion of baud rate divisor
  UART5->FBRD = 11;                // Fractional portion of baud rate divisor
  UART5->CC = 0;                   // Select system clock
  UART5->LCRH = 0x60;              // Set 8-bit data length, no parity, no FIFO
  UART5->CTL = 0x301;              // Enable UART5, Rx, and Tx

  GPIOE->DEN = 0x30;               // Set PE4 and PE5 as digital pins
  GPIOE->AFSEL = 0x30;             // Enable alternate function for PE4, PE5
  GPIOE->AMSEL = 0;                // Disable analog function
  GPIOE->PCTL = 0x00110000;        // Configure PE4 and PE5 for UART
}

// Function to read data from HC-05 Bluetooth module
char Bluetooth_Read(void) {
  while ((UART5->FR & (1 << 4)) != 0); // Wait until Rx buffer is not empty
  return (unsigned char)(UART5->DR);   // Return the received data
}

// Function to write a character to HC-05 Bluetooth module
void Bluetooth_Write(unsigned char data) {
  while ((UART5->FR & (1 << 5)) != 0); // Wait until Tx buffer is not full
  UART5->DR = data;                    // Send the data
}

// Function to write a string to HC-05 Bluetooth module
void Bluetooth_Write_String(char *str) {
  while (*str) {
    Bluetooth_Write(*(str++));
  }
}

// Delay function
void Delay(unsigned long counter) {
  unsigned long i = 0;
  for (i = 0; i < counter; i++);
}
