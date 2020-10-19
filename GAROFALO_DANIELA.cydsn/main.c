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

#define HEADER_VALUE 0xA0 // Value representing the beginning of the message: 160
#define TAIL_VALUE 0xC0 // Value representing the end of the message: 192

extern int flag_timer;
extern int flag_received;
// Definition of the flags as extern (global) variables

int stato;
int i;
uint8_t message[5] = {'\0'};
int flag_message; // Definition of the flag variable that indicates the complete receiving of the message

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    PWM_RedGreen_Start();
    PWM_Blue_Start(); // Enabling of the PWMs
    UART_1_Start(); // Initialisation of the UART transmission
    Timer_1_Init(); // Initialisation of the timer but not of the beginning of the counting
    Timer_ISR_StartEx(custom_Timer_ISR);
    Rx_ISR_StartEx(custom_UART_ISR); // Enabling of the ISRs
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    stato = IDLE; // Status in which the device is waiting for the receiving of the first byte
    flag_timer = 0; // Initialisation of the flag to zero because the timer has not generated any interrupt
    flag_received = 0; // Initialisation of the flag to zero because there is no received byte
    flag_message = 0; // Initialisation of the flag to zero because there are no complete messages received 
    
    UART_1_PutString("Please, insert first the header (0xA0 = 160), then 3 values corresponding to RGB values (numbers from 0 to 255) and finally the tail (0xCO = 192)\r\n");
    // Starting message on the terminal of the serial port
    for(;;)
    {
        /* Place your application code here. */
        if (flag_received == 1) // Received one byte
        {
            switch(stato)
            {
                case IDLE: // Expected that the byte is equal to the header package
                {
                    Timer_1_Stop(); // Ending the counting of the timer because the device is in the "waiting" mode
                    message[HEADER] = UART_1_ReadRxData();
                    if (message[HEADER] == HEADER_VALUE) // The first byte received is actually the header byte: correct transmission
                    {
                        // UART_1_PutString("HEADER byte received correctly\r\n");
                        Timer_1_Start(); // Starting of the counting when a correct byte is received
                        stato = HEADERBYTE_RECEIVED;
                    } else if (message[HEADER] == 'v') {
                        UART_1_PutString("RGB LED Program $$$\r\n");
                    } else {
                        UART_1_PutString("Invalid inserted value\r\n");
                    }
                    flag_received = 0;
                    break;
                }
                case HEADERBYTE_RECEIVED: // Expected that the byte contains information about the RED component
                {
                    message[RED] = UART_1_ReadRxData();
                    Timer_1_Start(); // Restarting of the counting when a correct byte is received before the generation of an interrupt by the timer
                    // UART_1_PutString("RED byte received correctly\r\n");
                    stato = REDBYTE_RECEIVED;
                    flag_received = 0;
                    break;
                }
                case REDBYTE_RECEIVED: // Expected that the byte contains information about the GREEN component
                {
                    message[GREEN] =  UART_1_ReadRxData();
                    Timer_1_Start(); // Restarting of the counting when a correct byte is received before the generation of an interrupt by the timer
                    // UART_1_PutString("GREEN byte received correctly\r\n");
                    stato = GREENBYTE_RECEIVED;
                    flag_received = 0;
                    break;
                }
                case GREENBYTE_RECEIVED: // Expected that the byte contains information about the BLUE component
                {
                    message[BLUE] = UART_1_ReadRxData();
                    Timer_1_Start(); // Restarting of the counting when a correct byte is received before the generation of an interrupt by the timer
                    // UART_1_PutString("BLUE byte received correctly\r\n");
                    stato = BLUEBYTE_RECEIVED;
                    flag_received = 0;
                    break;
                }
                case BLUEBYTE_RECEIVED: // Expected that the byte is equal to the tail package
                {
                    message[TAIL] = UART_1_ReadRxData();
                    if (message[TAIL] == TAIL_VALUE) // The last byte received is actually the tail byte: correct transmission
                    {       
                        flag_message = 1; /* Receiving the entire message correctly, so it is possible to switch on the LED
                        with the colour encoded in the data */
                        // -UART_1_PutString("TAIL byte received correctly\r\n");
                    } else {
                        UART_1_PutString("Invalid inserted value\r\n");
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
            flag_message = 0;
            UART_1_PutString("Time is up: restart the communication, please\r\n");
            flag_timer = 0;
        }
        if (flag_message == 1) // The entire message has been correctly received
        {
            PWM_RedGreen_WriteCompare1(message[RED]);
            PWM_RedGreen_WriteCompare2(message[GREEN]);
            PWM_Blue_WriteCompare(message[BLUE]);
            // Setting the compare values of the PWM components in order to obtain the desider colour
            stato = IDLE;
            flag_received = 0;
            UART_1_PutString("LED switched on\r\n");
            flag_message = 0;
        }
    }
}

/* [] END OF FILE */
