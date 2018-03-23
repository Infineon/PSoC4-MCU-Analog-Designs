/*******************************************************************************
* File: main.c
*
* Version: 1.0
*
* Description:
*  This is a source code for example project that demonstartes basic
*  functionality of PSoC4 IDAC8.
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

#define    IDAC_VAL_UNPRESSED     (100)   // every 1 bit is 1.2uA
#define    IDAC_VAL_PRESSED (200)

/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  Main function performs following functions:
*   1: Initializes the LCD and clears the display
*   2: Start the IDAC
*   3: IDAC range is set to 0-306uA and default value is set to 120uA
*   4: Prints test name on LCD
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

    /* Start the IDAC component */
    IDAC8_Start();

    /* Sets the IDAC value to 100 (120uA) */
    IDAC8_SetValue(IDAC_VAL_UNPRESSED);
    
    for(;;)
    {
        if(SW2_Read() == 0u)
        {
            CyDelay(5u);    /* Basic debounce delay */
            if(SW2_Read() == 0u)
            {
                IDAC8_SetValue(IDAC_VAL_PRESSED);
            }
            while(SW2_Read() == 0u)
            {
                ; /* Wait till button released */
            }
        }
        else
        {
            IDAC8_SetValue(IDAC_VAL_UNPRESSED);
        }
    }
}

/* [] END OF FILE */
