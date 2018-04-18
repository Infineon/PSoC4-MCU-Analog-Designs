/*******************************************************************************
* File Name: main.c
*
* Version: 1.00
*
* Description:
*  This example project uses Analog Hardware Mux, IDAC, Comparator, Multiplexer
*  and D flip-flops. Input signals are compared with the corresponding threshold
*  signals generated using IDAC and resistors. The output is latched using
*  digital De-multiplexer and latch logic.
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

/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  Performs the following tasks:
*  - Start the components
*  - Set the IDAC current value
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
int main()
{
    /* Start the Components */
    Comparator_Start();
    IDAC_Start();
    
    /* Set the IDAC current value to 100 uA */
    IDAC_SetValue(100u);
    
    for(;;)
    {
    
    }
}

/* [] END OF FILE */
