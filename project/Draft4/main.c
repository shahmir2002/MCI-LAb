#include <stdint.h>
#include "TM4C123GH6PM.h"
#include "ultrasonic.h"      // Include ultrasonic sensor header
#include "motor.h"           // Include motor control header
#include "infrared.h"
#include "UART.h"
#define SAFE_DISTANCE 5      // Safe distance in cm

/* Function prototypes */
void Switch_Init(void);
int Switch1_Pressed(void);
int Switch2_Pressed(void);

/* Global variables */
volatile uint32_t distances_main[2]; /* Stores measured distance values */
char mesgg[100];                     /* String buffer for UART debugging */

volatile uint32_t distance_sensor1;  /* Distance from Sensor 1 */
volatile uint32_t distance_sensor2;  /* Distance from Sensor 2 */

int main(void)
{
    UART5_init();             // Initialize UART5 for debugging

    /* Initialization */
    PWM_init();               // Initialize PWM for both motors
    Turn_Clockwise_A();       // Set Motor A to spin clockwise
    Turn_Clockwise_B();       // Set Motor B to spin clockwise
    Timer0ACapture_init();    // Initialize Timer0A for Sensor 1
    Timer0BCapture_init();    // Initialize Timer1A for Sensor 2

    Sensor2_Trigger_Init();   // Initialize PA7 as Trigger for Sensor 2
    Sensor1_Trigger_Init();   // Initialize PA4 as Trigger for Sensor 1

    IR_Sensor_Init();

    Switch_Init();            // Initialize switches

    sprintf(mesgg, "I main");
    printstring(mesgg);

    while (1)
    {
        // Wait until Switch 1 is pressed to start
        if (Switch1_Pressed()) 
        {
            // Robot starts moving
            sprintf(mesgg, "Robot started!");
            printstring(mesgg);

            while (!Switch2_Pressed()) // Continue moving until Switch 2 is pressed
            {
                /* Get distances measured by ultrasonic module */
                volatile uint32_t* distances_main = get_distance(); // Retrieve the distances array

                /* Extract individual sensor values */
                distance_sensor1 = distances_main[0];
                distance_sensor2 = distances_main[1];

                /* Debugging: Print distances */
                sprintf(mesgg, "\r\nDistance 1 = %d cm, Distance 2 = %d cm", distance_sensor1, distance_sensor2);
                printstring(mesgg);

                if (~IR_Detect()) {
                    PWM1->_3_CMPA = 0;
                    PWM1->_3_CMPB = 0;
                    sprintf(mesgg, "Obstacle detected, stopping motors.");
                    printstring(mesgg);
                } else {
                    // Motor control logic based on sensor readings
                    if ((distance_sensor1 < SAFE_DISTANCE) && ~IR_Detect()) {
                        PWM1->_3_CMPA = 4000; // Set Motor A duty cycle (e.g., 50% speed)
                        sprintf(mesgg, "\r\nMotor A: Speeding up, CMPA = %d", PWM1->_3_CMPA);
                        printstring(mesgg);
                    } else {
                        PWM1->_3_CMPA = 500; // Set Motor A to a lower speed
                        sprintf(mesgg, "\r\nMotor A: Default speed, CMPA = %d", PWM1->_3_CMPA);
                        printstring(mesgg);
                    }

                    if ((distance_sensor2 < SAFE_DISTANCE) && ~IR_Detect()) {
                        PWM1->_3_CMPB = 4000; // Set Motor B duty cycle (e.g., 50% speed)
                        sprintf(mesgg, "\r\nMotor B: Speeding up, CMPB = %d", PWM1->_3_CMPB);
                        printstring(mesgg);
                    } else {
                        PWM1->_3_CMPB = 500; // Set Motor B to a lower speed
                        sprintf(mesgg, "\r\nMotor B: Default speed, CMPB = %d", PWM1->_3_CMPB);
                        printstring(mesgg);
                    }
                }
                Delay_MicroSecond(100); // Short delay before the next measurement cycle
            }

            // Stop motors when Switch 2 is pressed
            PWM1->_3_CMPA = 0;
            PWM1->_3_CMPB = 0;
            sprintf(mesgg, "Robot stopped.");
            printstring(mesgg);
        }

        Delay_MicroSecond(100); // Short delay before checking switch again
    }
}

/* Initialize switches (PF0 and PF1) */
void Switch_Init(void) {
    SYSCTL->RCGCGPIO |= 0x20;   // Enable clock for GPIOF (PF0 and PF1)
    GPIOF->DIR &= ~(0x03);      // Set PF0 and PF1 as inputs
    GPIOF->DEN |= 0x03;         // Enable digital function on PF0 and PF1
    GPIOF->PUR |= 0x03;         // Enable pull-up resistors on PF0 and PF1
}

/* Check if Switch 1 (PF0) is pressed */
int Switch1_Pressed(void) {
    return !(GPIOF->DATA & 0x01);  // Return true if PF0 is low (pressed)
}

/* Check if Switch 2 (PF1) is pressed */
int Switch2_Pressed(void) {
    return !(GPIOF->DATA & 0x02);  // Return true if PF1 is low (pressed)
}
