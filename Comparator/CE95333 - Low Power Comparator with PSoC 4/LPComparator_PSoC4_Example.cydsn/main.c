/*******************************************************************************
* File: main.c
*
* Version : 1.2
*
* Description: 
*  This is source code for the example project of the PSoC 4 Low Power Comparator.
*
********************************************************************************
* Copyright 2013-2016, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include <project.h>

void GSR_ISR_Interrupt_InterruptCallback(void)
{
    /* Set the LED value accordingly to the PLComp output */
    Led_Write((uint8) ~LPComp_GetCompare());
    
    /* Clear pending interrupts */
    LPComp_ClearInterrupt(LPComp_INTR);
    GSR_ISR_ClearPending();
}


int main()
{
    uint32 trimVal;
    
    /* Initialize the ISR before starting it's interrupt source - LPComp */
    GSR_ISR_Start();
    
    LPComp_Start();
       
    /* Performs custom calibration of the input offset to minimize the error for a 
    * specific set of conditions: the comparator reference voltage, supply voltage, 
    * and operating temperature. A reference voltage in the range at which the 
    * comparator will be used must be applied to the Vminus input of the 
    * comparator. This can be done using an external resistive divider for example.
    */
    trimVal = LPComp_ZeroCal();
    
    /* Sets the Comparator trim value. Actually it is already done as a part of 
    * ZeroCal() function. This demonstrates method to set trim register with specific  
    * value.
    */
    LPComp_LoadTrim(trimVal);
       
    /* Sets LP Comparator Speed as Low. Actually it is already done by 
    * the component Init() function, which sets the Speed/Power value selected in 
    * the customizer. This API is to demonstrate that the same can be done with
    * a function call.
    */
    LPComp_SetSpeed(LPComp_LOW_SPEED);
    
    /* Enable global interrupts. */
    CyGlobalIntEnable;
    
    for(;;)
    {        
    #if(CY_IP_SRSSV2)
        /* Enter Hibernate Mode (for all PSoC 4 devices except PSoC 4000S,
			PSoC 4100S and PSoC Analog Coprocessor) */
        CySysPmHibernate();
    #else
        /* Enter Deep Sleep Mode (for PSoC 4000S,
			PSoC 4100S and PSoC Analog Coprocessor devices) */
        CySysPmDeepSleep();
    #endif
    }
}

/* [] END OF FILE */
