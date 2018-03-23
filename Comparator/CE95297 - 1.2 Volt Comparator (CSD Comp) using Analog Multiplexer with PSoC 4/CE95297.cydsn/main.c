/*******************************************************************************
* File Name: main.c
*
* Version: 1.0
*
* Description:
*  This example project demonstrates how the comparator's input can be multiplexed 
*  using the Amux component.
*
********************************************************************************
* Copyright 2014, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include <project.h>

#define OFF (1u)
#define ON  (0u)

int main()
{
    uint8 channel;
    
    /* Disable WDT */
    CySysWdtDisable();  
    
    Comp_Start();
    AMux_Start();
    
    /* Sets Comparator Speed as Fast. Actually it is already done by 
    * component Start() function, which set the Speed/Power value selected in 
    * the customizer. This API is to demonstrate that the same can be done with
    * a function call.
    */
    Comp_SetSpeed(Comp_FAST_SPEED);

    for(;;)
    {
        for(channel=0;channel<AMux_CHANNELS; channel++)
        {
            AMux_FastSelect(channel);
            if (0 != Comp_GetCompare())
            {
                GLed_Write(ON);
                RLed_Write(OFF); 
            }
            else
            {
                RLed_Write(ON);
                GLed_Write(OFF);
            }
            CyDelay(500u);
        }
    }
}

/* [] END OF FILE */
