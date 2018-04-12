/*******************************************************************************
* File Name: main.c
*
* Version: 1.00
*
* Description:
*  This is source code for the datasheet example of the ScanComp component.
*
********************************************************************************
* Copyright 2013, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include <project.h>

uint8 isEndOfConversion = 0u;

CY_ISR(eocIsr)
{
    EosIsr_ClearPending();
    isEndOfConversion = 1u;
}

int main()
{
	uint8 statusVal;
	
	/* Start ISR for EOF component */
	EosIsr_StartEx(eocIsr);
    EosIsr_ClearPending();
	
	/* Start LCD component */
    LCD_Start();
    	
	/* Start ScanComp components */
    ScanComp_Start();
	
	/* Enable global interrupts to the CPU core */
    CyGlobalIntEnable;
    
    for(;;)
    {
        if(isEndOfConversion)
		{
			isEndOfConversion = 0u;
			
            /* Get ScanComp outputs status */
			statusVal = CompStatus_Read();
			
            /* Display comparison result for each channels */
		    LCD_Position(0u,0u);
		    LCD_PrintString("Ch2="); 
		    LCD_PrintNumber((statusVal & 2u) >> 1u); 
		    LCD_Position(0u,6u);
		    LCD_PrintString("Ch1=");
		    LCD_PrintNumber(statusVal & 1u); 
			
            /* Display state of interrupt output */
			LCD_Position(1u,0u);
		    LCD_PrintString("Intr="); 
		    LCD_PrintNumber((statusVal & 4u) >> 2u); 
			
            /* Get and clear interrupt source for selected block */
		    if (((statusVal & 4u) >> 2u) != 0u )
		    {
		    	CyDelay(200u);
				ScanComp_GetInterruptSource(0u);
			}
			
            /* Get and display the current comparison result for all channels */
			LCD_Position(1u,8u);
		    LCD_PrintString("Src=");
			statusVal = ScanComp_GetCompare(0u) | ScanComp_GetCompare(1u); 
		    
            LCD_PrintInt8(statusVal); 
		}
    }
}


/* [] END OF FILE */
