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
    File Rx_Interrupt.h
    Function aimed to the definition of the ISR function generated when a byte is received from the serial port
    Author: Daniela Garofalo
*/

#ifndef __RX_INTERRUPT_H__
    #define __RX_INTERRUPT_H__
   
    #include <cytypes.h>
    #include <UART_1.h>
    
    CY_ISR_PROTO(custom_UART_ISR);
    
#endif

/* [] END OF FILE */
