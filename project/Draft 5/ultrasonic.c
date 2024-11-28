#include "TM4C123GH6PM.h"
#include <stdio.h>
#include "UART.h"
#include  "ultrasonic.h"

/* Function prototypes */


/* Global variables */
volatile uint32_t distance1; /* Distance from Sensor 1 */
volatile uint32_t distance2; /* Distance from Sensor 2 */
static volatile uint32_t distances[2]; // Distance array to store measurements
char mesg1[50];              /* String format of distance1 */
char mesg2[50];              /* String format of distance2 */

/* Function to compute and return distances */
volatile uint32_t* get_distance(void)
{
    /* Trigger sensors and calculate distances */
    Trigger_Sensors();
    Delay_MicroSecond(50000); /* Wait for the echo signals to settle */

    /* Populate distances array */
    distances[0] = distance1; // Sensor 1
    distances[1] = distance2; // Sensor 2

    return distances; /* Return pointer to the array */
}

/* Trigger both ultrasonic sensors simultaneously */
void Trigger_Sensors(void)
{
    GPIOA->DATA &= ~(1 << 4); /* Trigger pin for Sensor 1 low */
    GPIOA->DATA &= ~(1 << 7); /* Trigger pin for Sensor 2 low */
    Delay_MicroSecond(2);     /* Short delay before sending the pulse */

    GPIOA->DATA |= (1 << 4);  /* Trigger pin for Sensor 1 high */
    GPIOA->DATA |= (1 << 7);  /* Trigger pin for Sensor 2 high */
    Delay_MicroSecond(10);    /* Maintain 10 µs high pulse */

    GPIOA->DATA &= ~(1 << 4); /* Trigger pin for Sensor 1 low */
    GPIOA->DATA &= ~(1 << 7); /* Trigger pin for Sensor 2 low */
}

/* Timer0A initialization for Sensor 1 (Echo) */
void Timer0ACapture_init(void)
{
    SYSCTL->RCGCTIMER |= 1;  /* Enable clock to Timer Block 0 */
    SYSCTL->RCGCGPIO |= 2;   /* Enable clock to PORTB */

    GPIOB->DIR &= ~0x40;     /* Make PB6 an input pin for Sensor 1 Echo */
    GPIOB->DEN |= 0x40;      /* Enable digital functionality for PB6 */
    GPIOB->AFSEL |= 0x40;    /* Use PB6 alternate function */
    GPIOB->PCTL &= ~0x0F000000;
    GPIOB->PCTL |= 0x07000000; /* Configure PB6 for T0CCP0 */

    TIMER0->CTL &= ~1;       /* Disable Timer0A during setup */
    TIMER0->CFG = 4;         /* 16-bit timer mode */
    TIMER0->TAMR = 0x17;     /* Up-count, edge-time, capture mode */
    TIMER0->CTL |= 0x0C;     /* Capture both edges */
    TIMER0->IMR |= 0x04;     /* Enable capture event interrupt */
    NVIC->ISER[0] |= 1 << 19; /* Enable IRQ 19 for Timer0A */

    TIMER0->CTL |= 1;        /* Enable Timer0A */
}

/* Timer0B initialization for Sensor 2 (Echo) */
void Timer0BCapture_init(void)
{
    SYSCTL->RCGCTIMER |= 1;  /* Enable clock to Timer Block 0 */
    SYSCTL->RCGCGPIO |= 2;   /* Enable clock to PORTB */

    GPIOB->DIR &= ~0x80;     /* Make PB7 an input pin for Sensor 2 Echo */
    GPIOB->DEN |= 0x80;      /* Enable digital functionality for PB7 */
    GPIOB->AFSEL |= 0x80;    /* Use PB7 alternate function */
    GPIOB->PCTL &= ~0xF0000000;
    GPIOB->PCTL |= 0x70000000; /* Configure PB7 for T0CCP1 */

    TIMER0->CTL &= ~0x100;   /* Disable Timer0B during setup */
    TIMER0->TBMR = 0x17;     /* Up-count, edge-time, capture mode */
    TIMER0->CTL |= 0xC00;    /* Capture both edges */
    TIMER0->IMR |= 0x400;    /* Enable capture event interrupt for Timer0B */
    NVIC->ISER[0] |= 1 << 20; /* Enable IRQ 20 for Timer0B */

    TIMER0->CTL |= 0x100;    /* Enable Timer0B */
}

/* Initialize PA4 as Trigger Pin for Sensor 1 */
void Sensor1_Trigger_Init(void)
{
    SYSCTL->RCGCGPIO |= (1 << 0); /* Enable clock to GPIOA */

    GPIOA->DIR |= (1 << 4);  /* Set PA4 as output */
    GPIOA->DEN |= (1 << 4);  /* Enable digital functionality for PA4 */
    GPIOA->DATA &= ~(1 << 4); /* Initialize PA4 to low */
}

/* Initialize PA7 as Trigger Pin for Sensor 2 */
void Sensor2_Trigger_Init(void)
{
    SYSCTL->RCGCGPIO |= (1 << 0); /* Enable clock to GPIOA */

    GPIOA->DIR |= (1 << 7);  /* Set PA7 as output */
    GPIOA->DEN |= (1 << 7);  /* Enable digital functionality for PA7 */
    GPIOA->DATA &= ~(1 << 7); /* Initialize PA7 to low */
}

/* Timer0A interrupt handler for Sensor 1 */
void TIMER0A_Handler(void)
{
    static int lastEdge;
    int thisEdge;

    if (TIMER0->MIS & 0x04) /* Capture event */
    {
        TIMER0->ICR = 0x04; /* Clear capture interrupt flag */
        if (GPIOB->DATA & (1 << 6)) /* Rising edge */
        {
            lastEdge = TIMER0->TAR; /* Store the timestamp */
        }
        else /* Falling edge */
        {
            thisEdge = TIMER0->TAR; /* Store the timestamp */
            distance1 = (thisEdge - lastEdge) * 10625 / 10000000; /* Convert to cm */
        }
    }
}

/* Timer0B interrupt handler for Sensor 2 */
void TIMER0B_Handler(void)
{
    static unsigned int lastEdge = 0;
    unsigned int thisEdge;

    if (TIMER0->MIS & 0x400) /* Capture event for Timer0B */
    {
        TIMER0->ICR = 0x400; /* Clear Timer0B capture interrupt flag */
        
        if (GPIOB->DATA & (1 << 7)) /* Rising edge */
        {
            lastEdge = TIMER0->TBR; /* Store the timestamp */
        }
        else /* Falling edge */
        {
            thisEdge = TIMER0->TBR; /* Store the timestamp */
            
            /* Calculate distance, ensuring no negative values by using unsigned subtraction */
            if (thisEdge >= lastEdge)
            {
                distance2 = (thisEdge - lastEdge) * 10625 / 10000000; /* Convert to cm */
            }
            else
            {
                /* Handle timer wraparound case */
                distance2 = ((0xFFFF - lastEdge + thisEdge) + 1) * 10625 / 10000000; /* Convert to cm */
            }
        }
    }
}



