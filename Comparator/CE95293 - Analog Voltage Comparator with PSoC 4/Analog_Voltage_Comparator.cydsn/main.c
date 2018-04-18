/******************************************************************************
* File Name: main.c
*
* Version 1.20
*
* Description: 
*
* Related Document: 
*  CE195293_Analog_Voltage_Comparator_PSoC4.pdf
*
* Hardware Dependency: 
*  See CE195293_Analog_Voltage_Comparator_PSoC4.pdf
*
*******************************************************************************
* Copyright (2017), Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress’s integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, 
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress 
* reserves the right to make changes to the Software without notice. Cypress 
* does not assume any liability arising out of the application or use of the 
* Software or any product or circuit described in the Software. Cypress does 
* not authorize its products for use in any products where a malfunction or 
* failure of the Cypress product may reasonably be expected to result in 
* significant property damage, injury or death ("High Risk Product"). By 
* including Cypress’s product in a High Risk Product, the manufacturer of such 
* system or application assumes all risk of such use and in doing so agrees to 
* indemnify Cypress against all liability. 
*******************************************************************************/

#include <project.h>

#define AMUX_CHANNEL_VMINUS (1u)
#define AMUX_CHANNEL_VPLUS  (0u)


/*******************************************************************************
* Function Name: main
********************************************************************************
* 
* Summary:
*  This function:
*   1. Starts the AMux and Comparator Components
*   2. Switches AMux to prepare Comparator for calibration
*   3. Performs Comparator's calibration
*   4. Switches the AMux back to normal operation of the Comparator
*
*******************************************************************************/
int main()
{
    uint32 trimVal = 0u;
    
	/* Start the AMux and Comparator Components */
    Comp_Start();
    AMux_Start();
    
	
    /* Switch AMux to connect Positive and Negative inputs of the Comparator 
    * with the voltagre reference
    */
    AMux_Select(AMUX_CHANNEL_VMINUS);
    
    
    /* Perform custom calibration of the input offset to minimize the error for a 
    * set of conditions: the comparator reference voltage, supply voltage, 
    * and operating temperature.
    */
    trimVal = Comp_ZeroCal();
    
	
    /* Set the Comparator trim value. It is already done as part of the
    * ZeroCal() function. This demonstrates the method to set a trim register
    * with a specific value.
    */
    Comp_LoadTrim(trimVal);
    
	
    /* Returns to normal operation */
    AMux_Select(AMUX_CHANNEL_VPLUS);
    
	
    /* Set the Comparator Speed as High. It is already done by the
    * component Start() function which sets the Speed/Power value selected in 
    * the customizer. This API is used to demonstrate that the same can be done to
    * a function call.
    */
    Comp_SetSpeed(Comp_HIGH_SPEED);
   
    for(;;)
    {
         
    }
}


/* [] END OF FILE */
