#include "TM4C123GH6PM.h"
#include "UART.h"
#include "motor.h"
char motor_mesg[100];


// Motor A - Clockwise
void Turn_Clockwise_A(void)
{
    SYSCTL->RCGCGPIO |= (1 << 0);          // Enable clock to PORTA
    GPIOA->DIR |= (1 << 2) | (1 << 3);     // Set PA2 and PA3 as outputs
    GPIOA->DEN |= (1 << 2) | (1 << 3);     // Enable digital functionality for PA2, PA3
    GPIOA->DATA |= (1 << 2);               // Set PA2 (IN1) HIGH
    GPIOA->DATA &= ~(1 << 3);              // Set PA3 (IN2) LOW
}

// Motor A - Counterclockwise
void Turn_AntiClockwise_A(void)
{
    SYSCTL->RCGCGPIO |= (1 << 0);          // Enable clock to PORTA
    GPIOA->DIR |= (1 << 2) | (1 << 3);     // Set PA2 and PA3 as outputs
    GPIOA->DEN |= (1 << 2) | (1 << 3);     // Enable digital functionality for PA2, PA3
    GPIOA->DATA &= ~(1 << 2);              // Set PA2 (IN1) LOW
    GPIOA->DATA |= (1 << 3);               // Set PA3 (IN2) HIGH
}

// Motor B - Clockwise
void Turn_Clockwise_B(void)
{
    SYSCTL->RCGCGPIO |= (1 << 0);          // Enable clock to PORTA
    GPIOA->DIR |= (1 << 5) | (1 << 6);     // Set PA5 and PA6 as outputs
    GPIOA->DEN |= (1 << 5) | (1 << 6);     // Enable digital functio	nality for PA5, PA6
    GPIOA->DATA |= (1 << 5);               // Set PA5 (IN3) HIGH
    GPIOA->DATA &= ~(1 << 6);              // Set PA6 (IN4) LOW
	  sprintf(motor_mesg, "Motor B clockwise");
		printstring(motor_mesg);
}

// Motor B - Counterclockwise
void Turn_AntiClockwise_B(void)
{
    SYSCTL->RCGCGPIO |= (1 << 0);          // Enable clock to PORTA
    GPIOA->DIR |= (1 << 5) | (1 << 6);     // Set PA5 and PA6 as outputs
    GPIOA->DEN |= (1 << 5) | (1 << 6);     // Enable digital functionality for PA5, PA6
    GPIOA->DATA &= ~(1 << 5);              // Set PA5 (IN3) LOW
    GPIOA->DATA |= (1 << 6);               // Set PA6 (IN4) HIGH
		sprintf(motor_mesg, "Motor B clockwise");
		printstring(motor_mesg);
}

void PWM_init(void)
{
    SYSCTL->RCGCPWM |= (1 << 1);           // Enable clock to PWM1 module
    SYSCTL->RCGCGPIO |= (1 << 5);          // Enable clocks to PORTF
    Delay_ms(1);                           // Short delay for clock stabilization

    GPIOF->AFSEL |= (1 << 2) | (1 << 3);   // Enable alternate functions for PF2 and PF3
    GPIOF->PCTL &= ~0x0000FF00;            // Clear PCTL bits for PF2 and PF3
    GPIOF->PCTL |= 0x00005500;             // Set PF2 as M1PWM6 and PF3 as M1PWM7
    GPIOF->DEN |= (1 << 2) | (1 << 3);     // Enable digital functionality for PF2, PF3

    SYSCTL->RCC &= ~0x000E0000;            // Clear PWM clock divisor bits
    SYSCTL->RCC |= (1 << 20) | 0x000A0000; // Enable System Clock Divisor, set pre-divider to 64

    PWM1->_3_CTL = 0;                      // Disable Generator 3 counter
    PWM1->_3_GENA = 0x0000008C;            // Set PWM high on reload, low on match (Motor A)
    PWM1->_3_GENB = 0x0000080C;            // Set PWM high on reload, low on match (Motor B)
    PWM1->_3_LOAD = 5000;                  // Set load value for 50Hz (250kHz/5000)
    PWM1->_3_CMPA = 4000;                  // Initial duty cycle for Motor A
    PWM1->_3_CMPB = 4000;                  // Initial duty cycle for Motor B
    PWM1->_3_CTL |= 1;                     // Enable Generator 3 counter
    PWM1->ENABLE = 0xC0;                   // Enable PWM1 channel 6 (PF2) and channel 7 (PF3)
}


/* Delay function */
void Delay_ms(int time_ms)
{
    int i, j;
    for(i = 0; i < time_ms; i++)
        for(j = 0; j < 3180; j++) {}  // NOP for 1ms delay
}
