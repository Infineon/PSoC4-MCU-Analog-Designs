/*******************************************************************************
* File Name: main.c
*
* Version: 1.0
*
* Description:
*  This is source code for the example project of Debouncer component.
*
*  Parameters used:
*   Signal width:  1 
*   Edge Type:         
*                - Positive edge
*
* The project demonstrates how to eliminate the oscillations that occur on 
* transitions of an input digital signal from a switch using Debouncer 
* component.
* 
********************************************************************************
* Copyright 2012, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include <device.h>

uint8 count = 0u;       /* # of raw (unfiltered)transitions of input pin 'SW' */
uint8 filteredCount = 0u;      /* # of filtered transitions of input pin 'SW' */

/*******************************************************************************
* Function Name: SwInt_ISR
********************************************************************************
* Summary:
*  The Interrupt Service Routine. Increments a global 'count' variable.
*   
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
CY_ISR(SwInt_ISR)
{
    count++; 
}


/*******************************************************************************
* Function Name: FiltSwInt_ISR
********************************************************************************
* Summary:
*  The Interrupt Service Routine. Increments a global 'filteredCount' variable.
*  
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
CY_ISR(FiltSwInt_ISR)
{
    filteredCount++;
}


/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*  Main function performs the following:
*   1. Initializes LCD and Interrupt components.
*   2. Continually displays raw and filtered counts on the LCD display.
*   
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void main()
{
    uint8 temp1;  /* Local copy of count variable */
    uint8 temp2;  /* Local copy of filtered count variable */
    
    /* Initialization code */
    LCD_Start();
    LCD_Position(0,0); /* row, column */
    LCD_PrintString("Raw Count   = ");
    LCD_Position(1,0); /* row, column */
    LCD_PrintString("Filt. Count = ");    
    SwInt_StartEx(SwInt_ISR);
    FiltSwInt_StartEx(FiltSwInt_ISR);

    for(;;)
    {
        /* Grab a copy of the shared count variables, and display the copies.
         * This ensures the interrupt handler will not change the count
         * variables while them are being displayed.
         */
        CyGlobalIntDisable;
        temp1 = count;
        temp2 = filteredCount;
        CyGlobalIntEnable;
        LCD_Position(0,14); /* row, column */
        LCD_PrintDecUint16(temp1);
        LCD_Position(1,14); /* row, column */
        LCD_PrintDecUint16(temp2);
    }
}


/* [] END OF FILE */
