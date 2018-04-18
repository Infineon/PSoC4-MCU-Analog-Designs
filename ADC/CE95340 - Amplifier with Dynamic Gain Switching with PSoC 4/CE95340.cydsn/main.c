/*******************************************************************************
* File Name: main.c
*
* Version: 1.00
*
* Description:
*  This example project shows how to multiplex three different channels with
*  ADC using Analog Mux. The ADC input and gain can be changed on the fly by
*  pressing a switch.
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
#include <device.h>

/* Macro definitions */
#define LOW                 (0u)
#define HIGH                (1u)
#define CHANNEL_1           (0u)
#define CHANNEL_2           (1u)
#define CHANNEL_3           (2u)
#define NO_OF_CHANNELS      (3u)
#define CLEAR_SCREEN        (0x0C)
#define CONVERT_TO_ASCII    (0x30u)

/* Resistor Values */
#define R1                    ( 1000u)
#define R2                    ( 2200u)
#define R3                    ( 4700u)
#define RF1                    ( 1000u)
#define RF2                    (10000u)
#define RF3                    (47000u)
#define GAIN1               (1 + (float)RF1 / R1)
#define GAIN2               (1 + (float)RF2 / R2)
#define GAIN3               (1 + (float)RF3 / R3)

volatile uint32 windowFlag = 0u;
volatile uint8  dataReady  = 0u;
volatile uint8  switchFlag = 0u;

/* Send the channel number and voltage to UART */
static void SendChannelVoltage(int8 channel, int16 mVolts);

/* Interrupt prototypes */
CY_ISR_PROTO(ADC_ISR_Handler);
CY_ISR_PROTO(SW_ISR_Handler);


/*******************************************************************************
* Function Name: main
********************************************************************************
*
* Summary:
*  Performs the following tasks:
*  - Start the components.
*  - Starts ADC conversion.
*  - Selects the desired channel and gain.
*  - Sends the result to HyperTerminal (PC) using UART.
*  - Turns ON the LED when ADC input is outside the voltage
*      window of 250mV to 750mV.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void main()
{
    int8  channel = 0;
    int16 mVolts  = 0; 
    int16 previousValue = 0;
    float chGain[3u] = {GAIN1, GAIN2, GAIN3};

    /* Start the Components */
    Opamp_Start();
    ADC_Start();
    Input_AMux_Start();
    Gain_AMux_Start();
    UART_Start();
    
    /* Start ISRs */
    ADC_IRQ_StartEx(ADC_ISR_Handler);
    Isr_SW2_StartEx(SW_ISR_Handler);
    
    /* Enable global interrupts */
    CyGlobalIntEnable;
    
    /* Initialize both the analog mux (AMux) to default channel 0 */
    Gain_AMux_Next();
    Input_AMux_Next();
    
    /* Start ADC conversion */
    ADC_StartConvert();

    for(;;)
    {
        while(dataReady == 0u)
        {
            ; /* Wait for ADC conversion */
        }
        
        /* Convert the ADC counts to mVolts. ADC is configured to be single channel */
        mVolts = ADC_CountsTo_mVolts(CHANNEL_1, ADC_GetResult16(CHANNEL_1));
        
        /* Divide the result by gain of current channel */    
        mVolts = mVolts / chGain[Gain_AMux_GetChannel()];
                    
        /* If switch SW2 is pressed, change the input channel */
        if(switchFlag)
        {
            /* Select the next input channel and its corresponding gain */
            Input_AMux_Next();
            Gain_AMux_Next();
            
            /* Get the active input channel */
            channel = Input_AMux_GetChannel();
            
            /* Clear the switch flag */
            switchFlag = 0u;
        }
        
        /* Check ADC limit detection interrupt */
        if(windowFlag)
        {
            /* Turn ON the LED when input is outside the voltage window (250mV - 750mV) */
            LED_Write(LOW);    
            
            /* Note: If LED is active HIGH, then replace "LOW" with "HIGH" */
        }
        else
        {
            /* Turn ON the LED when input is outside the voltage window (250mV - 750mV) */
            LED_Write(HIGH);
            
            /* Note:If LED is active HIGH, then replace "HIGH" with "LOW" */
        }
        
        /* If the result changes, send it to UART */
        if(previousValue != mVolts)
        {    
            SendChannelVoltage(channel, mVolts);
            previousValue = mVolts;
        }
    }
}

/*******************************************************************************
* Function Name: SendChannelVoltage
********************************************************************************
*
* Summary:
*  Performs following functions:
*  - Convert the channel number to ASCII character
*  - Clear terminal screen
*  - Display channel number and voltage on HyperTerminal using UART
*
* Parameters:
*  int8  channel : Channel Number
*  int16 mVolts  : ADC result to send to UART    
*
* Return:
*  None.
*
************************************************************************************/
static void SendChannelVoltage(int8 channel, int16 mVolts)
{
    /* Clear Screen */ 
    UART_UartPutChar(CLEAR_SCREEN);
    /* Print Channel */
    UART_UartPutString(" Channel ");
    /* Display the channel number starting from 1 */
    channel++;
    /* Convert Channel number to ASCII and send */
    channel += CONVERT_TO_ASCII;
    UART_UartPutChar(channel);
    
    UART_UartPutString(" = ");
    
    /* Send Volts to UART */
    UART_UartPutChar((mVolts/1000) + CONVERT_TO_ASCII);
    mVolts %= 1000;
    UART_UartPutChar((mVolts/100) + CONVERT_TO_ASCII);
    mVolts %= 100;
    UART_UartPutChar((mVolts/10) + CONVERT_TO_ASCII);
    mVolts %= 10;
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


/******************************************************************************
* Function Name: SW_ISR_Handler
*******************************************************************************
*
* Summary:
*  Interrupt Service Routine. Sets the flag if SW2 is pressed.
*
******************************************************************************/
CY_ISR(SW_ISR_Handler)
{
    switchFlag = 1u;
}

/* [] END OF FILE */
