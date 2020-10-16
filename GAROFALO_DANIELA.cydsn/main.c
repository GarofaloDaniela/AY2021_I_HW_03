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

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
    }
}

/* [] END OF FILE */
