/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include <Timer_1.h>
#include <UART_1.h>
#include <Timer_Interrupt.h>
#include <Rx_Interrupt.h>

// Definition of different status values
#define IDLE 0
#define HEADERBYTE_RECEIVED 1
#define REDBYTE_RECEIVED 2
#define GREENBYTE_RECEIVED 3
#define BLUEBYTE_RECEIVED 4
#define TAILBYTE_RECEIVED 5

#define HEADER 0xA0 // Value representing the beginning of the message
#define TAIL 0xC0 // Value representing the end of the message

extern int flag_timer;
extern int flag_received;
// Definition of the flags as extern (global) variables

int stato;
int i;
uint16 message[5] = {'\0'}; // Initialisation of the variable which will contain the transmitted information

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_1_Start();
    Timer_1_Init(); // Initialisation of the timer but not of the beginning of the counting

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    stato = IDLE; // Status in which the device is waiting for the receiving of the first byte
    flag_timer = 0;
    flag_received = 0; // Initialisation of both flags to zero at the beginning of the program
    i = 0; // Initialisation of the index
    for(;;)
    {
        /* Place your application code here. */
        if (flag_received == 1) // Receiving of the one byte
        {
            switch(stato)
            {
                case IDLE:
                {
                    message[IDLE] = UART_1_ReadRxData();
                    flag_received = 0;
                    if (message[IDLE] == HEADER) // The first byte received is actually the header byte: correct transmission
                    {
                        Timer_1_Start(); // Starting of the counting
                        stato = HEADERBYTE_RECEIVED;
                        break;
                    }
                }
                case HEADERBYTE_RECEIVED:
                {
                    Timer_1_Start();
                    message[HEADERBYTE_RECEIVED] = UART_1_ReadRxData();
                    if (message[HEADERBYTE_RECEIVED] >= 0 && message[HEADERBYTE_RECEIVED] < 256)
                    // Controlling the received value with respect to the expected input range
                    {       
                        stato = REDBYTE_RECEIVED;
                    } else {
                        UART_1_PutString("Invalid inserted value");
                        stato = IDLE;
                    }
                    flag_received = 0;
                    break;
                }
                case REDBYTE_RECEIVED:
                {
                    Timer_1_Start();
                    message[REDBYTE_RECEIVED] = UART_1_ReadRxData();
                    if (message[REDBYTE_RECEIVED] >= 0 && message[REDBYTE_RECEIVED] < 256)
                    // Controlling the received value with respect to the expected input range
                    {       
                        stato = GREENBYTE_RECEIVED;
                    } else {
                        UART_1_PutString("Invalid inserted value");
                        stato = IDLE;
                    }
                    flag_received = 0;
                    break;
                }
                case GREENBYTE_RECEIVED:
                {
                    Timer_1_Start();
                    message[GREENBYTE_RECEIVED] = UART_1_ReadRxData();
                    if (message[GREENBYTE_RECEIVED] >= 0 && message[GREENBYTE_RECEIVED] < 256)
                    // Controlling the received value with respect to the expected input range
                    {       
                        stato = BLUEBYTE_RECEIVED;
                    } else {
                        UART_1_PutString("Invalid inserted value");
                        stato = IDLE;
                    }
                    flag_received = 0;
                    break;
                }
                case BLUEBYTE_RECEIVED:
                {
                    Timer_1_Stop(); // Ending the counting of the timer because the last byte is received
                    message[BLUEBYTE_RECEIVED] = UART_1_ReadRxData();
                    if (message[BLUEBYTE_RECEIVED] == TAIL) // The last byte received is actually the tail byte: correct transmission
                    {       
                        stato = IDLE;
                    } else {
                        UART_1_PutString("Invalid inserted value");
                        stato = IDLE;
                    }
                    flag_received = 0;
                    break;
                }
            }
        }
        if (flag_timer == 1) // Timer has generated an interrupt: resetting the device to the initial state
        {
            stato = IDLE;
            flag_received = 0;
            flag_timer = 0;
        }
    }
}

/* [] END OF FILE */
