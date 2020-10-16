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
    File Timer_Interrupt.c
    Function aimed in the change of device status when the end of the counting is reached by the timer or
    the restarting of the timer when a byte is received from the serial port
    Author: Daniela Garofalo
*/

#include <Timer_Interrupt.h>

int flag_timer;

CY_ISR(custom_Timer_ISR)
{
    flag_timer = 1; // Flag variable that indicates the end of the counting
}

/* [] END OF FILE */
