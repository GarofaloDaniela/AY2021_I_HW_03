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
#include <PWM_RedGreen.h>
#include <PWM_Blue.h>

// Definition of different status values
#define IDLE 0
#define HEADERBYTE_RECEIVED 1
#define REDBYTE_RECEIVED 2
#define GREENBYTE_RECEIVED 3
#define BLUEBYTE_RECEIVED 4
#define TAILBYTE_RECEIVED 5

#define HEADER 0
#define RED 1
#define GREEN 2
#define BLUE 3
#define TAIL 4

#define HEADER_VALUE 0xA0 // Value representing the beginning of the message
#define TAIL_VALUE 0xC0 // Value representing the end of the message

extern int flag_timer;
extern int flag_received;
// Definition of the flags as extern (global) variables

int stato;
int i;
uint16 message[5] = {'\0'}; // Initialisation of the variable which will contain the transmitted information
int flag_message; // Definition of the flag variable that indicates the complete receiving of the message

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_1_Start();
    Timer_1_Init(); // Initialisation of the timer but not of the beginning of the counting
    PWM_RedGreen_Start();
    PWM_Blue_Start();
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    stato = IDLE; // Status in which the device is waiting for the receiving of the first byte
    flag_timer = 0;
    flag_received = 0; // Initialisation of both flags to zero at the beginning of the program
    flag_message = 0; // Initialisation of the flag to zero because there are no complete messages received 
    for(;;)
    {
        /* Place your application code here. */
        if (flag_received == 1) // Receiving of the one byte
        {
            switch(stato)
            {
                case IDLE:
                {
                    Timer_1_Stop(); // Ending the counting of the timer because the device is in the "waiting" mode
                    UART_1_PutString("Please, insert first the header (0xA0), then 3 values corresponding to RGB values (numbers from 0 to 255) and finally the tail (0xCO)");
                    message[HEADER] = UART_1_ReadRxData();
                    flag_received = 0;
                    if (message[HEADER] == HEADER_VALUE) // The first byte received is actually the header byte: correct transmission
                    {
                        stato = HEADERBYTE_RECEIVED;
                        Timer_1_Start(); // Starting of the counting of the delay between the header and the first colour value 
                        break;
                    }
                }
                case HEADERBYTE_RECEIVED:
                {
                    message[RED] = UART_1_ReadRxData();
                    if (message[RED] >= 0 && message[RED] < 256) // Controlling the received value with respect to the expected input range
                    {       
                        stato = REDBYTE_RECEIVED;
                        Timer_1_Start(); // Restarting of the counting when a correct byte is received before the generation of an interrupt by the timer
                    } else {
                        UART_1_PutString("Invalid inserted value");
                        stato = IDLE;
                    }
                    flag_received = 0;
                    break;
                }
                case REDBYTE_RECEIVED:
                {
                    message[GREEN] = UART_1_ReadRxData();
                    if (message[GREEN] >= 0 && message[GREEN] < 256)
                    // Controlling the received value with respect to the expected input range
                    {       
                        stato = GREENBYTE_RECEIVED;
                        Timer_1_Start(); // Restarting of the counting when a correct byte is received before the generation of an interrupt by the timer
                    } else {
                        UART_1_PutString("Invalid inserted value");
                        stato = IDLE;
                    }
                    flag_received = 0;
                    break;
                }
                case GREENBYTE_RECEIVED:
                {
                    message[BLUE] = UART_1_ReadRxData();
                    if (message[BLUE] >= 0 && message[BLUE] < 256)
                    // Controlling the received value with respect to the expected input range
                    {       
                        stato = BLUEBYTE_RECEIVED;
                        Timer_1_Start(); // Restarting of the counting when a correct byte is received before the generation of an interrupt by the timer
                    } else {
                        UART_1_PutString("Invalid inserted value");
                        stato = IDLE;
                    }
                    flag_received = 0;
                    break;
                }
                case BLUEBYTE_RECEIVED:
                {
                    message[TAIL] = UART_1_ReadRxData();
                    if (message[TAIL] == TAIL_VALUE) // The last byte received is actually the tail byte: correct transmission
                    {       
                        flag_message = 1; /* Receiving the entire message correctly, so it is possible to switch on the LED
                        with the colour encoded in the data */
                    } else {
                        UART_1_PutString("Invalid inserted value");
                    }
                    stato = IDLE;
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
            flag_message = 0;
        }
        if (flag_message == 1) // The entire message has been correctly received
        {
            PWM_RedGreen_WriteCompare1(message[RED]);
            PWM_RedGreen_WriteCompare2(message[GREEN]);
            PWM_Blue_WriteCompare(message[BLUE]);
            // Setting the compare values of the PWM components in order to obtain the desider colour
            flag_message = 0;
            stato = IDLE;
        }
    }
}

/* [] END OF FILE */
