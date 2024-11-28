#include "TM4C123GH6PM.h"
#include "PID_control.h"
#include "motor.h"
#include "UART.h"

// PID controller
PID_t pid;

// Motor control constants
#define BASE_SPEED 2000
#define MAX_SPEED 4000
#define DESIRED_SPEED 50.0f // Target speed in RPM or equivalent units

// Encoder pins
#define ENCODER_A PB6
#define ENCODER_B PB7

// Function prototypes
int measure_encoder_speed(void);
void initialize_encoder(void);

int main(void) {
    // Initialize peripherals
    UART5_init();
    PWM_init();
    Turn_Clockwise_A();
    Turn_Clockwise_B();
    initialize_encoder();

    // Initialize PID controller
    PID_Init(&pid, 1.0f, 0.1f, 0.05f, DESIRED_SPEED);

    while (1) {
        // Measure speed from encoder
        int speed = measure_encoder_speed();

        // Compute PID output
        float pid_output = PID_Compute(&pid, speed);

        // Adjust motor speed
        int motor_speed = BASE_SPEED + (int)pid_output;
        if (motor_speed > MAX_SPEED) motor_speed = MAX_SPEED;
        if (motor_speed < 0) motor_speed = 0;

        // Set PWM duty cycle
        PWM1->_3_CMPA = motor_speed;

        // Debugging via UART
        char mesg[100];
        sprintf(mesg, "Speed: %d, PWM: %d\n", speed, motor_speed);
        printstring(mesg);

        Delay_MicroSecond(1000); // Loop delay
    }
}

