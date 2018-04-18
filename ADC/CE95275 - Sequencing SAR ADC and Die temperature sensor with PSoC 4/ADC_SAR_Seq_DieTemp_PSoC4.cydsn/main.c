/*******************************************************************************
* File Name: main.c
*
* Version: 2.10
*
* Description:
*  Sequencing SAR ADC and DieTemp datasheet example project.
*
* Note:
*  Pin_Vin pin (P0[4]) must be connected to the GND.
*  UART_tx pin (P0[5]) must be connected to the pin (P12[6]) of J8 header.
*  Verify that J9 is connected to 5V.
*
* Hardware Dependency:
*  PSoC4 Pioneer kit
*
********************************************************************************
* Copyright 2013-2017, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include <project.h>
#include <stdio.h>

/* ADC SAR sequencer component header to access Vref value */
#include <ADC_SAR_SEQ.h>

#define CH0_N               (0x00u)
#define TEMP_CH             (0x01u)
#define DELAY_1SEC          (1000u)

/* Get actual Vref. value from ADC SAR sequencer */
#define ADC_VREF_VALUE_V    ((float)ADC_SAR_SEQ_DEFAULT_VREF_MV_VALUE/1000.0)

volatile uint32 dataReady = 0u;
volatile int16 result[ADC_SAR_SEQ_TOTAL_CHANNELS_NUM];
volatile uint32 timer_delay = 0u;


/******************************************************************************
* Function Name: ISR_TIMER_LOC
*******************************************************************************
*
* Summary:
*  Handle Interrupt Service Routine. Source - TIMER.
*
******************************************************************************/
CY_ISR(ISR_TIMER_LOC)
{
    timer_delay++;
    
    /* Measures temperature once per second */
    if(timer_delay > DELAY_1SEC)
    {
        /* Enables injection channel for next scan */
        ADC_SAR_SEQ_EnableInjection();
        timer_delay = 0u;
    }    
    
    /* Clears interrupt request from terminal count */
    PWM_ClearInterrupt(PWM_INTR_MASK_TC);
}


/******************************************************************************
* Function Name: ADC_SAR_SEQ_ISR_LOC
*******************************************************************************
*
* Summary:
*  Handle Interrupt Service Routine. Source - ADC SAR Seq.
*
******************************************************************************/
CY_ISR(ADC_SAR_SEQ_ISR_LOC)
{
    uint32 intr_status;
    uint32 range_status;

    /* Read interrupt status registers */
    intr_status = ADC_SAR_SEQ_SAR_INTR_MASKED_REG;
    /* Check for End of Scan interrupt */
    if((intr_status & ADC_SAR_SEQ_EOS_MASK) != 0u)
    {
        /* Read range detect status */
        range_status = ADC_SAR_SEQ_SAR_RANGE_INTR_MASKED_REG;
        /* Verify that the conversion result met the condition Low_Limit <= Result < High_Limit  */
        if((range_status & (uint32)(1ul << CH0_N)) != 0u) 
        {
            /* Read conversion result */
            result[CH0_N] = ADC_SAR_SEQ_GetResult16(CH0_N);
            /* Set PWM compare from channel0 */
            PWM_WriteCompare(result[CH0_N]);
        }    

        /* Clear range detect status */
        ADC_SAR_SEQ_SAR_RANGE_INTR_REG = range_status;
        dataReady |= ADC_SAR_SEQ_EOS_MASK;
    }    

    /* Check for Injection End of Conversion */
    if((intr_status & ADC_SAR_SEQ_INJ_EOC_MASK) != 0u)
    {
        result[TEMP_CH] = ADC_SAR_SEQ_GetResult16(TEMP_CH);
        dataReady |= ADC_SAR_SEQ_INJ_EOC_MASK;
    }    

    /* Clear handled interrupt */
    ADC_SAR_SEQ_SAR_INTR_REG = intr_status;
}


/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
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
    int16 res = 0;
    int32 temperature;
    char  uartLine[250];
    int16 ADCCountsCorrected;

    /* Initialize the UART */
    UART_Start();
    UART_PutCRLF();
    UART_PutCRLF();
    UART_PutString("Starting temperature measurement...");
    UART_PutCRLF();
    
    PWM_Start();
    PWM_TriggerCommand(PWM_MASK, PWM_CMD_START);

    /* Init and start sequencing SAR ADC */
    ADC_SAR_SEQ_Start();
    ADC_SAR_SEQ_StartConvert();
    /* Enable interrupt and set interrupt handler to local routine */
    ADC_SAR_SEQ_IRQ_StartEx(ADC_SAR_SEQ_ISR_LOC);
    
    /* Init interrupt from timer to measure the temperature rarely */
    ISR_TIMER_StartEx(ISR_TIMER_LOC);

    CyGlobalIntEnable;

    for(;;)
    {
        /* When conversion of sequencing channels has completed */
        if((dataReady & ADC_SAR_SEQ_EOS_MASK) != 0u)
        {
            /* Get voltage, measured by ADC */
            dataReady &= ~ADC_SAR_SEQ_EOS_MASK;
            res = ADC_SAR_SEQ_CountsTo_mVolts(CH0_N, result[CH0_N]);
        }    
        
        /* When conversion of the injection channel has completed */
        if((dataReady & ADC_SAR_SEQ_INJ_EOC_MASK) != 0u)
        {
            dataReady &= ~ADC_SAR_SEQ_INJ_EOC_MASK;

            /******************************************************************************
            * Adjust data from ADC with respect to Vref value.
            * This adjustment is to be done if Vref is set to any other than
            * internal 1.024V.
            * For more detailed description see Functional Description section
            * of DieTemp P4 datasheet.
            *******************************************************************************/
            ADCCountsCorrected = (int16)(result[TEMP_CH]*(((float)ADC_VREF_VALUE_V/1.024)));
            
            temperature = DieTemp_CountsTo_Celsius(ADCCountsCorrected);

            /* Print temperature value to UART */
            sprintf(
                    uartLine, "Temperature value: %dC",
                    (int16) temperature
                    );
            
            UART_PutString(uartLine);
            UART_PutCRLF();
            
            /* Print voltage value to UART */
            sprintf(
                uartLine, "ADC measured voltage: %dmV",
                (uint16) res
                );
            
            UART_PutString(uartLine);
            UART_PutCRLF();
        }
    }
}


/* [] END OF FILE */
