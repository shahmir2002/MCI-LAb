#include "UART.h"
#include "TM4C123GH6PM.h"

char uart[100]; 

/* UART initialization */
void UART5_init(void)
{
	
    SYSCTL->RCGCUART |= 0x20; /* Enable clock to UART5 */
    SYSCTL->RCGCGPIO |= 0x10; /* Enable clock to PORTE for PE4/Rx and PE5/Tx */

    UART5->CTL = 0;         /* Disable UART5 */
    UART5->IBRD = 104;      /* 9600 baud rate integer part */
    UART5->FBRD = 11;       /* 9600 baud rate fractional part */
    UART5->LCRH = 0x60;     /* 8-bit, no parity, 1-stop bit */
    UART5->CTL = 0x301;     /* Enable UART5, Tx, and Rx */

    GPIOE->DEN = 0x30;      /* Enable digital PE4, PE5 */
    GPIOE->AFSEL |= 0x30;   /* Alternate function for PE4, PE5 */
    GPIOE->PCTL |= 0x00110000; /* Configure PE4, PE5 for UART */

}

void UART5_Transmitter(unsigned char data)
{
    while ((UART5->FR & (1 << 5)) != 0)
        ; /* Wait until Tx buffer is not full */
    UART5->DR = data;        /* Transmit data */

}

void printstring(char *str)
{
    while (*str)
    {
        UART5_Transmitter(*(str++));
    }
}

/* Microsecond delay using Timer1 */
void Delay_MicroSecond(int time)
{
    SYSCTL->RCGCTIMER |= 2; /* Enable clock to Timer Block 1 */
    TIMER1->CTL = 0;        /* Disable Timer1A during setup */
    TIMER1->CFG = 0x04;     /* 16-bit timer mode */
    TIMER1->TAMR = 0x02;    /* Periodic mode, down-counter */
    TIMER1->TAILR = 16 - 1; /* 1us at 16MHz clock */
    TIMER1->ICR = 0x01;     /* Clear TimerA timeout flag */
    TIMER1->CTL |= 0x01;    /* Enable Timer1A */

    for (int i = 0; i < time; i++)
    {
        while ((TIMER1->RIS & 0x01) == 0)
            ; /* Wait for timeout */
        TIMER1->ICR = 0x01; /* Clear timeout flag */
    }
}
