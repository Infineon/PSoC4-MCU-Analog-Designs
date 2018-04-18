/*******************************************************************************
* File Name: main.c
*
* Version: 2.00
*
* Description:
*  This example project shows how to sample four different channels using
*  SAR MUX and ADC, and send channel output to HyperTerminal (PC) using UART.
*
********************************************************************************
* Copyright 2013-2018, Cypress Semiconductor Corporation. All rights reserved.
* This software is owned by Cypress Semiconductor Corporation and is protected
* by and subject to worldwide patent and copyright laws and treaties.
* Therefore, you may use this software only as provided in the license agreement
* accompanying the software package from which you obtained this software.
* CYPRESS AND ITS SUPPLIERS MAKE NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* WITH REGARD TO THIS SOFTWARE, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT,
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

#include <project.h>

/* Macro definitions */
#define LOW                 (0u)
#define HIGH                (1u)
#define CHANNEL_1           (0u)
#define CHANNEL_2           (1u)
#define CHANNEL_3           (2u)
#define CHANNEL_4           (3u)
#define NO_OF_CHANNELS      (4u)
#define CLEAR_SCREEN        (0x0C)
#define CONVERT_TO_ASCII    (0x30u)

/* Resistor Values and Gain */
#define RG                  (2200u)
#define R1                  (10000u)
#define GAIN                (1 + 2 * (float)R1 / RG)

/* Send the channel number and voltage to UART */
static void SendChannelVoltage(uint8 channel, int16 mVolts);

/* Interrupt prototypes */
CY_ISR_PROTO(ADC_ISR_Handler);

/* Global variables */
volatile uint32 windowFlag    = 0u;
volatile uint8  dataReady     = 0u;
volatile uint8  channelFlag   = 0u;


/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  Performs the following tasks:
*  - Start the components.
*  - Starts ADC conversion.
*  - Buffer ADC results.
*  - Sends the result to HyperTerminal (PC) using UART.
*  - Turns ON an LED when ADC input is outside the voltage 
*     window of 250mV to 750mV.
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
    uint8 channel = CHANNEL_1;
    int16 adcVal[4u];
    int16 mVolts;
    int16 previousValue = 0;
    int32 adcGain;
    
    /* Start the Components */
    Opamp_1_Start();
    Opamp_2_Start();
    UART_Start();
    ADC_Start();
    
    /* Start ISRs */
    ADC_IRQ_StartEx(ADC_ISR_Handler);
    
    /* Enable global interrupts */
    CyGlobalIntEnable;

    /* Start ADC conversion */
    ADC_StartConvert();
    /* Calculate gain for the first channel */
    adcGain = (((int32)ADC_SAR_WRK_MAX_12BIT * ADC_10MV_COUNTS) +
              ADC_DEFAULT_VREF_MV_VALUE) / (ADC_DEFAULT_VREF_MV_VALUE * 2);
    adcGain *= GAIN;
    ADC_SetGain(CHANNEL_1, adcGain);
    
    for(;;)
    {
        while(dataReady == 0u)
        {
            ; /* Wait for ADC conversion */
        }
        /* Buffer the results */
        adcVal[CHANNEL_1] = ADC_GetResult16(CHANNEL_1);
        adcVal[CHANNEL_2] = ADC_GetResult16(CHANNEL_2);
        adcVal[CHANNEL_3] = ADC_GetResult16(CHANNEL_3);
        adcVal[CHANNEL_4] = ADC_GetResult16(CHANNEL_4);
        
        /* Check for ADC window limit interrupt */
        if(windowFlag != 0u)
        {
            /* Turn ON the LED when input is outside the voltage window (250mV - 750mV) */
            LED_Write(LOW);
            
            /* Note: If LED is active HIGH, then replace "LOW" with "HIGH" */
        }
        else
        {
            /* Turn OFF the LED when input is within the voltage window (250mV - 750mV) */
            LED_Write(HIGH);
            
            /* Note:If LED is active HIGH, then replace "HIGH" with "LOW" */
        }
        
        /* Switch processing */
        if(SW2_Read() == 0u)
        {
            CyDelay(5u);    /* Basic debounce delay */
            if(SW2_Read() == 0u)
            {
                channelFlag = 1u;
                /* Update the channel number */
                channel++;
                channel %= NO_OF_CHANNELS;
            }
            while(SW2_Read() == 0u)
            {
                ; /* Wait till button released */
            }
        }

        /* Convert the ADC counts of active channel to mVolts */
        mVolts = ADC_CountsTo_mVolts(channel, adcVal[channel]);

        /* If ADC result or channel has been changed, send the data to UART */
        if((previousValue != mVolts) || (channelFlag != 0u))
        {    
            SendChannelVoltage(channel, mVolts);
            previousValue = mVolts;
            
            /* Clear the flag */
            channelFlag = 0u;
        }
        dataReady = 0u;
    }
}

/*******************************************************************************
* Function Name: SendChannelVoltage
********************************************************************************
*
* Summary:
*  Performs the following tasks:
*   - Converts the channel number to ASCII character
*   - Clears terminal screen
*   - Sends the channel number and voltage to UART
*
* Parameters:
*  uint8 channel : Channel Number
*  int16 mVolts  : ADC counts    
*
* Return:
*  None.
*
********************************************************************************/
static void SendChannelVoltage(uint8 channel, int16 mVolts)
{
    /* Clear screen */ 
    UART_UartPutChar(CLEAR_SCREEN);
    UART_UartPutString("Channel ");
    
    /* Display the channel number starting from 1 */
    channel++;
    channel += CONVERT_TO_ASCII;
    UART_UartPutChar(channel);
    UART_UartPutString(" = ");
    
    /* Find the sign of the result */
    if(mVolts < 0)
    {
        UART_UartPutString("-");
        mVolts = -mVolts;
    }
        
    /* Send voltage to UART */
    UART_UartPutChar((mVolts/1000u) + CONVERT_TO_ASCII);
    mVolts %= 1000u;
    UART_UartPutChar((mVolts/100u) + CONVERT_TO_ASCII);
    mVolts %= 100u;
    UART_UartPutChar((mVolts/10u) + CONVERT_TO_ASCII);
    mVolts %= 10u;
    UART_UartPutChar(mVolts + CONVERT_TO_ASCII);
    UART_UartPutString(" mV");
    UART_UartPutCRLF(0u);
}


/******************************************************************************
* Function Name: ADC_ISR_Handler
*******************************************************************************
*
* Summary:
*  Interrupt Service Routine. Check the ADC status and sets window and data
*  ready flags.
*
******************************************************************************/
CY_ISR(ADC_ISR_Handler)
{
    uint32 intr_status;

    /* Read interrupt status registers */
    intr_status = ADC_SAR_INTR_MASKED_REG;
    /* Check for End of Scan interrupt */
    if((intr_status & ADC_EOS_MASK) != 0u)
    {
        /* Read range interrupt status and raise the flag */
        windowFlag = ADC_SAR_RANGE_INTR_MASKED_REG;
        /* Clear range detect status */
        ADC_SAR_RANGE_INTR_REG = windowFlag;
        dataReady = 1u;
    }
    /* Clear handled interrupt */
    ADC_SAR_INTR_REG = intr_status;
}

/* [] END OF FILE */
