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

/*
    File Rx_Interrupt.c
    Function aimed to reset the timer counting when a byte from the serial port is received
    Author: Daniela Garofalo
*/
    
#include <Rx_Interrupt.h>

int flag_received;

CY_ISR(custom_UART_ISR)
{
    if (UART_1_ReadRxStatus() == UART_1_RX_STS_FIFO_NOTEMPTY) // Byte received
    {
        flag_received = 1; // Flag variable that indicates the receiving of the byte
    }
}
    
/* [] END OF FILE */
