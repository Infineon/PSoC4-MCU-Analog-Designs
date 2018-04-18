 /*******************************************************************************
* File: main.c
*
* Version : 1.1
*
* Description: 
*   This is source code for the PSoC4 Opamp example project. 
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include <project.h>

int main()
{
    /* Start Opamp component */
    Opamp_Start();

    /* Sets Opamp power mode to High power */
    Opamp_SetPower(Opamp_HIGH_POWER);

    for(;;)
    {
    }
}


/* [] END OF FILE */
