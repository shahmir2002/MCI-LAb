#include "TM4C123GH6PM.h"
void Delay_ms(int time_ms);
int duty_cycle_A = 4999;
int duty_cycle_B = 4999;
void PWM_init(void);
void Turn_Clockwise_A(void);
void Turn_AntiClockwise_A(void);
void Turn_Clockwise_B(void);
void Turn_AntiClockwise_B(void);

int main(void)
{
    PWM_init();               // Initialize PWM for both motors
    Turn_Clockwise_A();        // Set Motor A to spin clockwise
    Turn_Clockwise_B();        // Set Motor B to spin clockwise

    for(;;)
    {
        duty_cycle_A -= 10;   // Decrement duty cycle for Motor A
        duty_cycle_B -= 10;   // Decrement duty cycle for Motor B

        if (duty_cycle_A <= 0) duty_cycle_A = 5000;
        if (duty_cycle_B <= 0) duty_cycle_B = 5000;

        PWM1->_3_CMPA = duty_cycle_A; // Update Motor A duty cycle
        PWM1->_3_CMPB = duty_cycle_B; // Update Motor B duty cycle
        Delay_ms(100);
    }
}

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
    GPIOA->DIR |= (1 << 5) | (1 << 6);     // Set PA4 and PA5 as outputs
    GPIOA->DEN |= (1 << 5) | (1 << 6);     // Enable digital functionality for PA4, PA5
    GPIOA->DATA |= (1 << 5);               // Set PA4 (IN3) HIGH
    GPIOA->DATA &= ~(1 << 6);              // Set PA5 (IN4) LOW
}

// Motor B - Counterclockwise
void Turn_AntiClockwise_B(void)
{
    SYSCTL->RCGCGPIO |= (1 << 0);          // Enable clock to PORTA
    GPIOA->DIR |= (1 << 5) | (1 <<6);     // Set PA4 and PA5 as outputs
    GPIOA->DEN |= (1 << 5) | (1 << 6);     // Enable digital functionality for PA4, PA5
    GPIOA->DATA &= ~(1 << 5);              // Set PA4 (IN3) LOW
    GPIOA->DATA |= (1 << 6);               // Set PA5 (IN4) HIGH
}

void PWM_init(void)
{
    SYSCTL->RCGCPWM |= (1 << 1);           // Enable clock to PWM1 module
    SYSCTL->RCGCGPIO |= (1 << 5);          // Enable system clock to PORTF
    SYSCTL->RCC |= (1 << 20);              // Enable System Clock Divisor
    SYSCTL->RCC |= 0x000E0000;             // Set pre-divider to 64 for PWM clock

    // Setting of PF2 and PF3 for M1PWM6 (Motor A) and M1PWM7 (Motor B) output pins
    GPIOF->AFSEL |= (1 << 2) | (1 << 3);   // Enable alternate functions for PF2 and PF3
    GPIOF->PCTL &= ~0x0000FF00;            // Clear PCTL bits for PF2 and PF3
    GPIOF->PCTL |= 0x00005500;             // Set PF2 as M1PWM6 and PF3 as M1PWM7
    GPIOF->DEN |= (1 << 2) | (1 << 3);     // Enable digital functionality for PF2, PF3

    PWM1->_3_CTL &= ~(1 << 0);             // Disable Generator 3 counter
    PWM1->_3_CTL &= ~(1 << 1);             // Select down-count mode
    PWM1->_3_GENA = 0x0000008C;            // Set PWM high on reload, low on match (Motor A)
    PWM1->_3_GENB = 0x0000080C;            // Set PWM high on reload, low on match (Motor B)
    PWM1->_3_LOAD = 5000;                  // Set load value for 50Hz (250kHz/5000)
    PWM1->_3_CMPA = 4999;                  // Set duty cycle for Motor A
    PWM1->_3_CMPB = 4999;                  // Set duty cycle for Motor B
    PWM1->_3_CTL |= (1 << 0);              // Enable Generator 3 counter
    PWM1->ENABLE = 0xC0;                   // Enable PWM1 channel 6 (PF2) and channel 7 (PF3)
}

/* Delay function */
void Delay_ms(int time_ms)
{
    int i, j;
    for(i = 0; i < time_ms; i++)
        for(j = 0; j < 3180; j++) {}  // NOP for 1ms delay
}
