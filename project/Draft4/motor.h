// motor.h

#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include "TM4C123GH6PM.h"

// Duty cycle variables for Motor A and Motor B
extern int duty_cycle_A;
extern int duty_cycle_B;

// Function prototypes
void Delay_ms(int time_ms);
void PWM_init(void);
void Turn_Clockwise_A(void);
void Turn_AntiClockwise_A(void);
void Turn_Clockwise_B(void);
void Turn_AntiClockwise_B(void);

#endif // MOTOR_CONTROL_H
