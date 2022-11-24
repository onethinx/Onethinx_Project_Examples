/*******************************************************************************
* File Name: `$INSTANCE_NAME`.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides all API functionality of the UART component
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "`$INSTANCE_NAME`.h"
#if (`$INSTANCE_NAME`_INTERNAL_CLOCK_USED)
    #include "`$INSTANCE_NAME`_IntClock.h"
#endif /* End `$INSTANCE_NAME`_INTERNAL_CLOCK_USED */


/***************************************
* Global data allocation
***************************************/

uint8_t `$INSTANCE_NAME`_initVar = 0u;

#if (`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED && `$INSTANCE_NAME`_TX_ENABLED)
    volatile uint8_t `$INSTANCE_NAME`_txBuffer[`$INSTANCE_NAME`_TX_BUFFER_SIZE];
    volatile `$TxBuffRegSizeReplacementString` `$INSTANCE_NAME`_txBufferRead = 0u;
    `$TxBuffRegSizeReplacementString` `$INSTANCE_NAME`_txBufferWrite = 0u;
#endif /* (`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED && `$INSTANCE_NAME`_TX_ENABLED) */

#if (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED && (`$INSTANCE_NAME`_RX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED))
    uint8_t `$INSTANCE_NAME`_errorStatus = 0u;
    volatile uint8_t `$INSTANCE_NAME`_rxBuffer[`$INSTANCE_NAME`_RX_BUFFER_SIZE];
    volatile `$RxBuffRegSizeReplacementString` `$INSTANCE_NAME`_rxBufferRead  = 0u;
    volatile `$RxBuffRegSizeReplacementString` `$INSTANCE_NAME`_rxBufferWrite = 0u;
    volatile uint8_t `$INSTANCE_NAME`_rxBufferLoopDetect = 0u;
    volatile uint8_t `$INSTANCE_NAME`_rxBufferOverflow   = 0u;
    #if (`$INSTANCE_NAME`_RXHW_ADDRESS_ENABLED)
        volatile uint8_t `$INSTANCE_NAME`_rxAddressMode = `$INSTANCE_NAME`_RX_ADDRESS_MODE;
        volatile uint8_t `$INSTANCE_NAME`_rxAddressDetected = 0u;
    #endif /* (`$INSTANCE_NAME`_RXHW_ADDRESS_ENABLED) */
#endif /* (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED && (`$INSTANCE_NAME`_RX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED)) */


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Start
********************************************************************************
*
* Summary:
*  This is the preferred method to begin component operation.
*  `$INSTANCE_NAME`_Start() sets the initVar variable, calls the
*  `$INSTANCE_NAME`_Init() function, and then calls the
*  `$INSTANCE_NAME`_Enable() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  The `$INSTANCE_NAME`_intiVar variable is used to indicate initial
*  configuration of this component. The variable is initialized to zero (0u)
*  and set to one (1u) the first time `$INSTANCE_NAME`_Start() is called. This
*  allows for component initialization without re-initialization in all
*  subsequent calls to the `$INSTANCE_NAME`_Start() routine.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void `$INSTANCE_NAME`_Start(void) `=ReentrantKeil($INSTANCE_NAME . "_Start")`
{
    /* If not initialized then initialize all required hardware and software */
    if(`$INSTANCE_NAME`_initVar == 0u)
    {
        `$INSTANCE_NAME`_Init();
        `$INSTANCE_NAME`_initVar = 1u;
    }

    `$INSTANCE_NAME`_Enable();
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Init
********************************************************************************
*
* Summary:
*  Initializes or restores the component according to the customizer Configure
*  dialog settings. It is not necessary to call `$INSTANCE_NAME`_Init() because
*  the `$INSTANCE_NAME`_Start() API calls this function and is the preferred
*  method to begin component operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void `$INSTANCE_NAME`_Init(void) `=ReentrantKeil($INSTANCE_NAME . "_Init")`
{
    #if(`$INSTANCE_NAME`_RX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED)

        #if (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED)
            /* Set RX interrupt vector and priority */
            (void) CyIntSetVector(`$INSTANCE_NAME`_RX_VECT_NUM, &`$INSTANCE_NAME`_RXISR);
            CyIntSetPriority(`$INSTANCE_NAME`_RX_VECT_NUM, `$INSTANCE_NAME`_RX_PRIOR_NUM);
            `$INSTANCE_NAME`_errorStatus = 0u;
        #endif /* (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED) */

        #if (`$INSTANCE_NAME`_RXHW_ADDRESS_ENABLED)
            `$INSTANCE_NAME`_SetRxAddressMode(`$INSTANCE_NAME`_RX_ADDRESS_MODE);
            `$INSTANCE_NAME`_SetRxAddress1(`$INSTANCE_NAME`_RX_HW_ADDRESS1);
            `$INSTANCE_NAME`_SetRxAddress2(`$INSTANCE_NAME`_RX_HW_ADDRESS2);
        #endif /* End `$INSTANCE_NAME`_RXHW_ADDRESS_ENABLED */

        /* Init Count7 period */
        `$INSTANCE_NAME`_RXBITCTR_PERIOD_REG = `$INSTANCE_NAME`_RXBITCTR_INIT;
        /* Configure the Initial RX interrupt mask */
        `$INSTANCE_NAME`_RXSTATUS_MASK_REG  = `$INSTANCE_NAME`_INIT_RX_INTERRUPTS_MASK;
    #endif /* End `$INSTANCE_NAME`_RX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED*/

    #if(`$INSTANCE_NAME`_TX_ENABLED)
        #if (`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED)
            /* Set TX interrupt vector and priority */
            (void) CyIntSetVector(`$INSTANCE_NAME`_TX_VECT_NUM, &`$INSTANCE_NAME`_TXISR);
            CyIntSetPriority(`$INSTANCE_NAME`_TX_VECT_NUM, `$INSTANCE_NAME`_TX_PRIOR_NUM);
        #endif /* (`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED) */

        /* Write Counter Value for TX Bit Clk Generator*/
        #if (`$INSTANCE_NAME`_TXCLKGEN_DP)
            `$INSTANCE_NAME`_TXBITCLKGEN_CTR_REG = `$INSTANCE_NAME`_BIT_CENTER;
            `$INSTANCE_NAME`_TXBITCLKTX_COMPLETE_REG = ((`$INSTANCE_NAME`_NUMBER_OF_DATA_BITS +
                        `$INSTANCE_NAME`_NUMBER_OF_START_BIT) * `$INSTANCE_NAME`_OVER_SAMPLE_COUNT) - 1u;
        #else
            `$INSTANCE_NAME`_TXBITCTR_PERIOD_REG = ((`$INSTANCE_NAME`_NUMBER_OF_DATA_BITS +
                        `$INSTANCE_NAME`_NUMBER_OF_START_BIT) * `$INSTANCE_NAME`_OVER_SAMPLE_8) - 1u;
        #endif /* End `$INSTANCE_NAME`_TXCLKGEN_DP */

        /* Configure the Initial TX interrupt mask */
        #if (`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED)
            `$INSTANCE_NAME`_TXSTATUS_MASK_REG = `$INSTANCE_NAME`_TX_STS_FIFO_EMPTY;
        #else
            `$INSTANCE_NAME`_TXSTATUS_MASK_REG = `$INSTANCE_NAME`_INIT_TX_INTERRUPTS_MASK;
        #endif /*End `$INSTANCE_NAME`_TX_INTERRUPT_ENABLED*/

    #endif /* End `$INSTANCE_NAME`_TX_ENABLED */

    #if(`$INSTANCE_NAME`_PARITY_TYPE_SW)  /* Write Parity to Control Register */
        `$INSTANCE_NAME`_WriteControlRegister( \
            (`$INSTANCE_NAME`_ReadControlRegister() & (uint8_t)~`$INSTANCE_NAME`_CTRL_PARITY_TYPE_MASK) | \
            (uint8_t)(`$INSTANCE_NAME`_PARITY_TYPE << `$INSTANCE_NAME`_CTRL_PARITY_TYPE0_SHIFT) );
    #endif /* End `$INSTANCE_NAME`_PARITY_TYPE_SW */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Enable
********************************************************************************
*
* Summary:
*  Activates the hardware and begins component operation. It is not necessary
*  to call `$INSTANCE_NAME`_Enable() because the `$INSTANCE_NAME`_Start() API
*  calls this function, which is the preferred method to begin component
*  operation.

* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  `$INSTANCE_NAME`_rxAddressDetected - set to initial state (0).
*
*******************************************************************************/
void `$INSTANCE_NAME`_Enable(void) `=ReentrantKeil($INSTANCE_NAME . "_Enable")`
{
    uint8_t enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    #if (`$INSTANCE_NAME`_RX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED)
        /* RX Counter (Count7) Enable */
        `$INSTANCE_NAME`_RXBITCTR_CONTROL_REG |= `$INSTANCE_NAME`_CNTR_ENABLE;

        /* Enable the RX Interrupt */
        `$INSTANCE_NAME`_RXSTATUS_ACTL_REG  |= `$INSTANCE_NAME`_INT_ENABLE;

        #if (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED)
            `$INSTANCE_NAME`_EnableRxInt();

            #if (`$INSTANCE_NAME`_RXHW_ADDRESS_ENABLED)
                `$INSTANCE_NAME`_rxAddressDetected = 0u;
            #endif /* (`$INSTANCE_NAME`_RXHW_ADDRESS_ENABLED) */
        #endif /* (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED) */
    #endif /* (`$INSTANCE_NAME`_RX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED) */

    #if(`$INSTANCE_NAME`_TX_ENABLED)
        /* TX Counter (DP/Count7) Enable */
        #if(!`$INSTANCE_NAME`_TXCLKGEN_DP)
            `$INSTANCE_NAME`_TXBITCTR_CONTROL_REG |= `$INSTANCE_NAME`_CNTR_ENABLE;
        #endif /* End `$INSTANCE_NAME`_TXCLKGEN_DP */

        /* Enable the TX Interrupt */
        `$INSTANCE_NAME`_TXSTATUS_ACTL_REG |= `$INSTANCE_NAME`_INT_ENABLE;
        #if (`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED)
            `$INSTANCE_NAME`_ClearPendingTxInt(); /* Clear history of TX_NOT_EMPTY */
            `$INSTANCE_NAME`_EnableTxInt();
        #endif /* (`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED) */
     #endif /* (`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED) */

    #if (`$INSTANCE_NAME`_INTERNAL_CLOCK_USED)
        `$INSTANCE_NAME`_IntClock_Start();  /* Enable the clock */
    #endif /* (`$INSTANCE_NAME`_INTERNAL_CLOCK_USED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Stop
********************************************************************************
*
* Summary:
*  Disables the UART operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void `$INSTANCE_NAME`_Stop(void) `=ReentrantKeil($INSTANCE_NAME . "_Stop")`
{
    uint8_t enableInterrupts;
    enableInterrupts = CyEnterCriticalSection();

    /* Write Bit Counter Disable */
    #if (`$INSTANCE_NAME`_RX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED)
        `$INSTANCE_NAME`_RXBITCTR_CONTROL_REG &= (uint8_t) ~`$INSTANCE_NAME`_CNTR_ENABLE;
    #endif /* (`$INSTANCE_NAME`_RX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED) */

    #if (`$INSTANCE_NAME`_TX_ENABLED)
        #if(!`$INSTANCE_NAME`_TXCLKGEN_DP)
            `$INSTANCE_NAME`_TXBITCTR_CONTROL_REG &= (uint8_t) ~`$INSTANCE_NAME`_CNTR_ENABLE;
        #endif /* (!`$INSTANCE_NAME`_TXCLKGEN_DP) */
    #endif /* (`$INSTANCE_NAME`_TX_ENABLED) */

    #if (`$INSTANCE_NAME`_INTERNAL_CLOCK_USED)
        `$INSTANCE_NAME`_IntClock_Stop();   /* Disable the clock */
    #endif /* (`$INSTANCE_NAME`_INTERNAL_CLOCK_USED) */

    /* Disable internal interrupt component */
    #if (`$INSTANCE_NAME`_RX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED)
        `$INSTANCE_NAME`_RXSTATUS_ACTL_REG  &= (uint8_t) ~`$INSTANCE_NAME`_INT_ENABLE;

        #if (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED)
            `$INSTANCE_NAME`_DisableRxInt();
        #endif /* (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED) */
    #endif /* (`$INSTANCE_NAME`_RX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED) */

    #if (`$INSTANCE_NAME`_TX_ENABLED)
        `$INSTANCE_NAME`_TXSTATUS_ACTL_REG &= (uint8_t) ~`$INSTANCE_NAME`_INT_ENABLE;

        #if (`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED)
            `$INSTANCE_NAME`_DisableTxInt();
        #endif /* (`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED) */
    #endif /* (`$INSTANCE_NAME`_TX_ENABLED) */

    CyExitCriticalSection(enableInterrupts);
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_ReadControlRegister
********************************************************************************
*
* Summary:
*  Returns the current value of the control register.
*
* Parameters:
*  None.
*
* Return:
*  Contents of the control register.
*
*******************************************************************************/
uint8_t `$INSTANCE_NAME`_ReadControlRegister(void) `=ReentrantKeil($INSTANCE_NAME . "_ReadControlRegister")`
{
    #if (`$INSTANCE_NAME`_CONTROL_REG_REMOVED)
        return(0u);
    #else
        return(`$INSTANCE_NAME`_CONTROL_REG);
    #endif /* (`$INSTANCE_NAME`_CONTROL_REG_REMOVED) */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_WriteControlRegister
********************************************************************************
*
* Summary:
*  Writes an 8-bit value into the control register
*
* Parameters:
*  control:  control register value
*
* Return:
*  None.
*
*******************************************************************************/
void  `$INSTANCE_NAME`_WriteControlRegister(uint8_t control) `=ReentrantKeil($INSTANCE_NAME . "_WriteControlRegister")`
{
    #if (`$INSTANCE_NAME`_CONTROL_REG_REMOVED)
        if(0u != control)
        {
            /* Suppress compiler warning */
        }
    #else
       `$INSTANCE_NAME`_CONTROL_REG = control;
    #endif /* (`$INSTANCE_NAME`_CONTROL_REG_REMOVED) */
}


#if(`$INSTANCE_NAME`_RX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED)
    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_SetRxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the RX interrupt sources enabled.
    *
    * Parameters:
    *  IntSrc:  Bit field containing the RX interrupts to enable. Based on the 
    *  bit-field arrangement of the status register. This value must be a 
    *  combination of status register bit-masks shown below:
    *      `$INSTANCE_NAME`_RX_STS_FIFO_NOTEMPTY    Interrupt on byte received.
    *      `$INSTANCE_NAME`_RX_STS_PAR_ERROR        Interrupt on parity error.
    *      `$INSTANCE_NAME`_RX_STS_STOP_ERROR       Interrupt on stop error.
    *      `$INSTANCE_NAME`_RX_STS_BREAK            Interrupt on break.
    *      `$INSTANCE_NAME`_RX_STS_OVERRUN          Interrupt on overrun error.
    *      `$INSTANCE_NAME`_RX_STS_ADDR_MATCH       Interrupt on address match.
    *      `$INSTANCE_NAME`_RX_STS_MRKSPC           Interrupt on address detect.
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_SetRxInterruptMode(uint8_t intSrc) `=ReentrantKeil($INSTANCE_NAME . "_SetRxInterruptMode")`
    {
        `$INSTANCE_NAME`_RXSTATUS_MASK_REG  = intSrc;
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_ReadRxData
    ********************************************************************************
    *
    * Summary:
    *  Returns the next byte of received data. This function returns data without
    *  checking the status. You must check the status separately.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Received data from RX register
    *
    * Global Variables:
    *  `$INSTANCE_NAME`_rxBuffer - RAM buffer pointer for save received data.
    *  `$INSTANCE_NAME`_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  `$INSTANCE_NAME`_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  `$INSTANCE_NAME`_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8_t `$INSTANCE_NAME`_ReadRxData(void) `=ReentrantKeil($INSTANCE_NAME . "_ReadRxData")`
    {
        uint8_t rxData;

    #if (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED)

        `$RxBuffRegSizeReplacementString` locRxBufferRead;
        `$RxBuffRegSizeReplacementString` locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        `$INSTANCE_NAME`_DisableRxInt();

        locRxBufferRead  = `$INSTANCE_NAME`_rxBufferRead;
        locRxBufferWrite = `$INSTANCE_NAME`_rxBufferWrite;

        if( (`$INSTANCE_NAME`_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = `$INSTANCE_NAME`_rxBuffer[locRxBufferRead];
            locRxBufferRead++;

            if(locRxBufferRead >= `$INSTANCE_NAME`_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            `$INSTANCE_NAME`_rxBufferRead = locRxBufferRead;

            if(`$INSTANCE_NAME`_rxBufferLoopDetect != 0u)
            {
                `$INSTANCE_NAME`_rxBufferLoopDetect = 0u;
                #if ((`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED) && (`$INSTANCE_NAME`_FLOW_CONTROL != 0u))
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( `$INSTANCE_NAME`_HD_ENABLED )
                        if((`$INSTANCE_NAME`_CONTROL_REG & `$INSTANCE_NAME`_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only in RX
                            *  configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            `$INSTANCE_NAME`_RXSTATUS_MASK_REG  |= `$INSTANCE_NAME`_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        `$INSTANCE_NAME`_RXSTATUS_MASK_REG  |= `$INSTANCE_NAME`_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end `$INSTANCE_NAME`_HD_ENABLED */
                #endif /* ((`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED) && (`$INSTANCE_NAME`_FLOW_CONTROL != 0u)) */
            }
        }
        else
        {   /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
            rxData = `$INSTANCE_NAME`_RXDATA_REG;
        }

        `$INSTANCE_NAME`_EnableRxInt();

    #else

        /* Needs to check status for RX_STS_FIFO_NOTEMPTY bit */
        rxData = `$INSTANCE_NAME`_RXDATA_REG;

    #endif /* (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_ReadRxStatus
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the receiver status register and the software
    *  buffer overflow status.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Current state of the status register.
    *
    * Side Effect:
    *  All status register bits are clear-on-read except
    *  `$INSTANCE_NAME`_RX_STS_FIFO_NOTEMPTY.
    *  `$INSTANCE_NAME`_RX_STS_FIFO_NOTEMPTY clears immediately after RX data
    *  register read.
    *
    * Global Variables:
    *  `$INSTANCE_NAME`_rxBufferOverflow - used to indicate overload condition.
    *   It set to one in RX interrupt when there isn't free space in
    *   `$INSTANCE_NAME`_rxBufferRead to write new data. This condition returned
    *   and cleared to zero by this API as an
    *   `$INSTANCE_NAME`_RX_STS_SOFT_BUFF_OVER bit along with RX Status register
    *   bits.
    *
    *******************************************************************************/
    uint8_t `$INSTANCE_NAME`_ReadRxStatus(void) `=ReentrantKeil($INSTANCE_NAME . "_ReadRxStatus")`
    {
        uint8_t status;

        status = `$INSTANCE_NAME`_RXSTATUS_REG & `$INSTANCE_NAME`_RX_HW_MASK;

    #if (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED)
        if(`$INSTANCE_NAME`_rxBufferOverflow != 0u)
        {
            status |= `$INSTANCE_NAME`_RX_STS_SOFT_BUFF_OVER;
            `$INSTANCE_NAME`_rxBufferOverflow = 0u;
        }
    #endif /* (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED) */

        return(status);
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_GetChar
    ********************************************************************************
    *
    * Summary:
    *  Returns the last received byte of data. `$INSTANCE_NAME`_GetChar() is
    *  designed for ASCII characters and returns a uint8_t where 1 to 255 are values
    *  for valid characters and 0 indicates an error occurred or no data is present.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Character read from UART RX buffer. ASCII characters from 1 to 255 are valid.
    *  A returned zero signifies an error condition or no data available.
    *
    * Global Variables:
    *  `$INSTANCE_NAME`_rxBuffer - RAM buffer pointer for save received data.
    *  `$INSTANCE_NAME`_rxBufferWrite - cyclic index for write to rxBuffer,
    *     checked to identify new data.
    *  `$INSTANCE_NAME`_rxBufferRead - cyclic index for read from rxBuffer,
    *     incremented after each byte has been read from buffer.
    *  `$INSTANCE_NAME`_rxBufferLoopDetect - cleared if loop condition was detected
    *     in RX ISR.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint8_t `$INSTANCE_NAME`_GetChar(void) `=ReentrantKeil($INSTANCE_NAME . "_GetChar")`
    {
        uint8_t rxData = 0u;
        uint8_t rxStatus;

    #if (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED)
        `$RxBuffRegSizeReplacementString` locRxBufferRead;
        `$RxBuffRegSizeReplacementString` locRxBufferWrite;

        /* Protect variables that could change on interrupt */
        `$INSTANCE_NAME`_DisableRxInt();

        locRxBufferRead  = `$INSTANCE_NAME`_rxBufferRead;
        locRxBufferWrite = `$INSTANCE_NAME`_rxBufferWrite;

        if( (`$INSTANCE_NAME`_rxBufferLoopDetect != 0u) || (locRxBufferRead != locRxBufferWrite) )
        {
            rxData = `$INSTANCE_NAME`_rxBuffer[locRxBufferRead];
            locRxBufferRead++;
            if(locRxBufferRead >= `$INSTANCE_NAME`_RX_BUFFER_SIZE)
            {
                locRxBufferRead = 0u;
            }
            /* Update the real pointer */
            `$INSTANCE_NAME`_rxBufferRead = locRxBufferRead;

            if(`$INSTANCE_NAME`_rxBufferLoopDetect != 0u)
            {
                `$INSTANCE_NAME`_rxBufferLoopDetect = 0u;
                #if( (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED) && (`$INSTANCE_NAME`_FLOW_CONTROL != 0u) )
                    /* When Hardware Flow Control selected - return RX mask */
                    #if( `$INSTANCE_NAME`_HD_ENABLED )
                        if((`$INSTANCE_NAME`_CONTROL_REG & `$INSTANCE_NAME`_CTRL_HD_SEND) == 0u)
                        {   /* In Half duplex mode return RX mask only if
                            *  RX configuration set, otherwise
                            *  mask will be returned in LoadRxConfig() API.
                            */
                            `$INSTANCE_NAME`_RXSTATUS_MASK_REG |= `$INSTANCE_NAME`_RX_STS_FIFO_NOTEMPTY;
                        }
                    #else
                        `$INSTANCE_NAME`_RXSTATUS_MASK_REG |= `$INSTANCE_NAME`_RX_STS_FIFO_NOTEMPTY;
                    #endif /* end `$INSTANCE_NAME`_HD_ENABLED */
                #endif /* `$INSTANCE_NAME`_RX_INTERRUPT_ENABLED and Hardware flow control*/
            }

        }
        else
        {   rxStatus = `$INSTANCE_NAME`_RXSTATUS_REG;
            if((rxStatus & `$INSTANCE_NAME`_RX_STS_FIFO_NOTEMPTY) != 0u)
            {   /* Read received data from FIFO */
                rxData = `$INSTANCE_NAME`_RXDATA_REG;
                /*Check status on error*/
                if((rxStatus & (`$INSTANCE_NAME`_RX_STS_BREAK | `$INSTANCE_NAME`_RX_STS_PAR_ERROR |
                                `$INSTANCE_NAME`_RX_STS_STOP_ERROR | `$INSTANCE_NAME`_RX_STS_OVERRUN)) != 0u)
                {
                    rxData = 0u;
                }
            }
        }

        `$INSTANCE_NAME`_EnableRxInt();

    #else

        rxStatus =`$INSTANCE_NAME`_RXSTATUS_REG;
        if((rxStatus & `$INSTANCE_NAME`_RX_STS_FIFO_NOTEMPTY) != 0u)
        {
            /* Read received data from FIFO */
            rxData = `$INSTANCE_NAME`_RXDATA_REG;

            /*Check status on error*/
            if((rxStatus & (`$INSTANCE_NAME`_RX_STS_BREAK | `$INSTANCE_NAME`_RX_STS_PAR_ERROR |
                            `$INSTANCE_NAME`_RX_STS_STOP_ERROR | `$INSTANCE_NAME`_RX_STS_OVERRUN)) != 0u)
            {
                rxData = 0u;
            }
        }
    #endif /* (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED) */

        return(rxData);
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_GetByte
    ********************************************************************************
    *
    * Summary:
    *  Reads UART RX buffer immediately, returns received character and error
    *  condition.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  MSB contains status and LSB contains UART RX data. If the MSB is nonzero,
    *  an error has occurred.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    uint16 `$INSTANCE_NAME`_GetByte(void) `=ReentrantKeil($INSTANCE_NAME . "_GetByte")`
    {
        
    #if (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED)
        uint16 locErrorStatus;
        /* Protect variables that could change on interrupt */
        `$INSTANCE_NAME`_DisableRxInt();
        locErrorStatus = (uint16)`$INSTANCE_NAME`_errorStatus;
        `$INSTANCE_NAME`_errorStatus = 0u;
        `$INSTANCE_NAME`_EnableRxInt();
        return ( (uint16)(locErrorStatus << 8u) | `$INSTANCE_NAME`_ReadRxData() );
    #else
        return ( ((uint16)`$INSTANCE_NAME`_ReadRxStatus() << 8u) | `$INSTANCE_NAME`_ReadRxData() );
    #endif /* `$INSTANCE_NAME`_RX_INTERRUPT_ENABLED */
        
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_GetRxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of received bytes available in the RX buffer.
    *  * RX software buffer is disabled (RX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty RX FIFO or 1 for not empty RX FIFO.
    *  * RX software buffer is enabled: returns the number of bytes available in 
    *    the RX software buffer. Bytes available in the RX FIFO do not take to 
    *    account.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  `$RxBuffRegSizeReplacementString`: Number of bytes in the RX buffer. 
    *    Return value type depends on RX Buffer Size parameter.
    *
    * Global Variables:
    *  `$INSTANCE_NAME`_rxBufferWrite - used to calculate left bytes.
    *  `$INSTANCE_NAME`_rxBufferRead - used to calculate left bytes.
    *  `$INSTANCE_NAME`_rxBufferLoopDetect - checked to decide left bytes amount.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the RX Buffer is.
    *
    *******************************************************************************/
    `$RxBuffRegSizeReplacementString` `$INSTANCE_NAME`_GetRxBufferSize(void)
                                                            `=ReentrantKeil($INSTANCE_NAME . "_GetRxBufferSize")`
    {
        `$RxBuffRegSizeReplacementString` size;

    #if (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt */
        `$INSTANCE_NAME`_DisableRxInt();

        if(`$INSTANCE_NAME`_rxBufferRead == `$INSTANCE_NAME`_rxBufferWrite)
        {
            if(`$INSTANCE_NAME`_rxBufferLoopDetect != 0u)
            {
                size = `$INSTANCE_NAME`_RX_BUFFER_SIZE;
            }
            else
            {
                size = 0u;
            }
        }
        else if(`$INSTANCE_NAME`_rxBufferRead < `$INSTANCE_NAME`_rxBufferWrite)
        {
            size = (`$INSTANCE_NAME`_rxBufferWrite - `$INSTANCE_NAME`_rxBufferRead);
        }
        else
        {
            size = (`$INSTANCE_NAME`_RX_BUFFER_SIZE - `$INSTANCE_NAME`_rxBufferRead) + `$INSTANCE_NAME`_rxBufferWrite;
        }

        `$INSTANCE_NAME`_EnableRxInt();

    #else

        /* We can only know if there is data in the fifo. */
        size = ((`$INSTANCE_NAME`_RXSTATUS_REG & `$INSTANCE_NAME`_RX_STS_FIFO_NOTEMPTY) != 0u) ? 1u : 0u;

    #endif /* (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED) */

        return(size);
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_ClearRxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears the receiver memory buffer and hardware RX FIFO of all received data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  `$INSTANCE_NAME`_rxBufferWrite - cleared to zero.
    *  `$INSTANCE_NAME`_rxBufferRead - cleared to zero.
    *  `$INSTANCE_NAME`_rxBufferLoopDetect - cleared to zero.
    *  `$INSTANCE_NAME`_rxBufferOverflow - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may
    *  have remained in the RAM.
    *
    * Side Effects:
    *  Any received data not read from the RAM or FIFO buffer will be lost.
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_ClearRxBuffer(void) `=ReentrantKeil($INSTANCE_NAME . "_ClearRxBuffer")`
    {
        uint8_t enableInterrupts;

        /* Clear the HW FIFO */
        enableInterrupts = CyEnterCriticalSection();
        `$INSTANCE_NAME`_RXDATA_AUX_CTL_REG |= (uint8_t)  `$INSTANCE_NAME`_RX_FIFO_CLR;
        `$INSTANCE_NAME`_RXDATA_AUX_CTL_REG &= (uint8_t) ~`$INSTANCE_NAME`_RX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        `$INSTANCE_NAME`_DisableRxInt();

        `$INSTANCE_NAME`_rxBufferRead = 0u;
        `$INSTANCE_NAME`_rxBufferWrite = 0u;
        `$INSTANCE_NAME`_rxBufferLoopDetect = 0u;
        `$INSTANCE_NAME`_rxBufferOverflow = 0u;

        `$INSTANCE_NAME`_EnableRxInt();

    #endif /* (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED) */

    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_SetRxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Sets the software controlled Addressing mode used by the RX portion of the
    *  UART.
    *
    * Parameters:
    *  addressMode: Enumerated value indicating the mode of RX addressing
    *  `$INSTANCE_NAME`__B_UART__AM_SW_BYTE_BYTE -  Software Byte-by-Byte address
    *                                               detection
    *  `$INSTANCE_NAME`__B_UART__AM_SW_DETECT_TO_BUFFER - Software Detect to Buffer
    *                                               address detection
    *  `$INSTANCE_NAME`__B_UART__AM_HW_BYTE_BY_BYTE - Hardware Byte-by-Byte address
    *                                               detection
    *  `$INSTANCE_NAME`__B_UART__AM_HW_DETECT_TO_BUFFER - Hardware Detect to Buffer
    *                                               address detection
    *  `$INSTANCE_NAME`__B_UART__AM_NONE - No address detection
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  `$INSTANCE_NAME`_rxAddressMode - the parameter stored in this variable for
    *   the farther usage in RX ISR.
    *  `$INSTANCE_NAME`_rxAddressDetected - set to initial state (0).
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_SetRxAddressMode(uint8_t addressMode)
                                                        `=ReentrantKeil($INSTANCE_NAME . "_SetRxAddressMode")`
    {
        #if(`$INSTANCE_NAME`_RXHW_ADDRESS_ENABLED)
            #if(`$INSTANCE_NAME`_CONTROL_REG_REMOVED)
                if(0u != addressMode)
                {
                    /* Suppress compiler warning */
                }
            #else /* `$INSTANCE_NAME`_CONTROL_REG_REMOVED */
                uint8_t tmpCtrl;
                tmpCtrl = `$INSTANCE_NAME`_CONTROL_REG & (uint8_t)~`$INSTANCE_NAME`_CTRL_RXADDR_MODE_MASK;
                tmpCtrl |= (uint8_t)(addressMode << `$INSTANCE_NAME`_CTRL_RXADDR_MODE0_SHIFT);
                `$INSTANCE_NAME`_CONTROL_REG = tmpCtrl;

                #if(`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED && \
                   (`$INSTANCE_NAME`_RXBUFFERSIZE > `$INSTANCE_NAME`_FIFO_LENGTH) )
                    `$INSTANCE_NAME`_rxAddressMode = addressMode;
                    `$INSTANCE_NAME`_rxAddressDetected = 0u;
                #endif /* End `$INSTANCE_NAME`_RXBUFFERSIZE > `$INSTANCE_NAME`_FIFO_LENGTH*/
            #endif /* End `$INSTANCE_NAME`_CONTROL_REG_REMOVED */
        #else /* `$INSTANCE_NAME`_RXHW_ADDRESS_ENABLED */
            if(0u != addressMode)
            {
                /* Suppress compiler warning */
            }
        #endif /* End `$INSTANCE_NAME`_RXHW_ADDRESS_ENABLED */
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_SetRxAddress1
    ********************************************************************************
    *
    * Summary:
    *  Sets the first of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #1 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_SetRxAddress1(uint8_t address) `=ReentrantKeil($INSTANCE_NAME . "_SetRxAddress1")`
    {
        `$INSTANCE_NAME`_RXADDRESS1_REG = address;
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_SetRxAddress2
    ********************************************************************************
    *
    * Summary:
    *  Sets the second of two hardware-detectable receiver addresses.
    *
    * Parameters:
    *  address: Address #2 for hardware address detection.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_SetRxAddress2(uint8_t address) `=ReentrantKeil($INSTANCE_NAME . "_SetRxAddress2")`
    {
        `$INSTANCE_NAME`_RXADDRESS2_REG = address;
    }

#endif  /* `$INSTANCE_NAME`_RX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED*/


#if( (`$INSTANCE_NAME`_TX_ENABLED) || (`$INSTANCE_NAME`_HD_ENABLED) )
    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_SetTxInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the TX interrupt sources to be enabled, but does not enable the
    *  interrupt.
    *
    * Parameters:
    *  intSrc: Bit field containing the TX interrupt sources to enable
    *   `$INSTANCE_NAME`_TX_STS_COMPLETE        Interrupt on TX byte complete
    *   `$INSTANCE_NAME`_TX_STS_FIFO_EMPTY      Interrupt when TX FIFO is empty
    *   `$INSTANCE_NAME`_TX_STS_FIFO_FULL       Interrupt when TX FIFO is full
    *   `$INSTANCE_NAME`_TX_STS_FIFO_NOT_FULL   Interrupt when TX FIFO is not full
    *
    * Return:
    *  None.
    *
    * Theory:
    *  Enables the output of specific status bits to the interrupt controller
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_SetTxInterruptMode(uint8_t intSrc) `=ReentrantKeil($INSTANCE_NAME . "_SetTxInterruptMode")`
    {
        `$INSTANCE_NAME`_TXSTATUS_MASK_REG = intSrc;
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_WriteTxData
    ********************************************************************************
    *
    * Summary:
    *  Places a byte of data into the transmit buffer to be sent when the bus is
    *  available without checking the TX status register. You must check status
    *  separately.
    *
    * Parameters:
    *  txDataByte: data byte
    *
    * Return:
    * None.
    *
    * Global Variables:
    *  `$INSTANCE_NAME`_txBuffer - RAM buffer pointer for save data for transmission
    *  `$INSTANCE_NAME`_txBufferWrite - cyclic index for write to txBuffer,
    *    incremented after each byte saved to buffer.
    *  `$INSTANCE_NAME`_txBufferRead - cyclic index for read from txBuffer,
    *    checked to identify the condition to write to FIFO directly or to TX buffer
    *  `$INSTANCE_NAME`_initVar - checked to identify that the component has been
    *    initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_WriteTxData(uint8_t txDataByte) `=ReentrantKeil($INSTANCE_NAME . "_WriteTxData")`
    {
        /* If not Initialized then skip this function*/
        if(`$INSTANCE_NAME`_initVar != 0u)
        {
        #if (`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED)

            /* Protect variables that could change on interrupt. */
            `$INSTANCE_NAME`_DisableTxInt();

            if( (`$INSTANCE_NAME`_txBufferRead == `$INSTANCE_NAME`_txBufferWrite) &&
                ((`$INSTANCE_NAME`_TXSTATUS_REG & `$INSTANCE_NAME`_TX_STS_FIFO_FULL) == 0u) )
            {
                /* Add directly to the FIFO. */
                `$INSTANCE_NAME`_TXDATA_REG = txDataByte;
            }
            else
            {
                if(`$INSTANCE_NAME`_txBufferWrite >= `$INSTANCE_NAME`_TX_BUFFER_SIZE)
                {
                    `$INSTANCE_NAME`_txBufferWrite = 0u;
                }

                `$INSTANCE_NAME`_txBuffer[`$INSTANCE_NAME`_txBufferWrite] = txDataByte;

                /* Add to the software buffer. */
                `$INSTANCE_NAME`_txBufferWrite++;
            }

            `$INSTANCE_NAME`_EnableTxInt();

        #else

            /* Add directly to the FIFO. */
            `$INSTANCE_NAME`_TXDATA_REG = txDataByte;

        #endif /*(`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED) */
        }
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_ReadTxStatus
    ********************************************************************************
    *
    * Summary:
    *  Reads the status register for the TX portion of the UART.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Contents of the status register
    *
    * Theory:
    *  This function reads the TX status register, which is cleared on read.
    *  It is up to the user to handle all bits in this return value accordingly,
    *  even if the bit was not enabled as an interrupt source the event happened
    *  and must be handled accordingly.
    *
    *******************************************************************************/
    uint8_t `$INSTANCE_NAME`_ReadTxStatus(void) `=ReentrantKeil($INSTANCE_NAME . "_ReadTxStatus")`
    {
        return(`$INSTANCE_NAME`_TXSTATUS_REG);
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_PutChar
    ********************************************************************************
    *
    * Summary:
    *  Puts a byte of data into the transmit buffer to be sent when the bus is
    *  available. This is a blocking API that waits until the TX buffer has room to
    *  hold the data.
    *
    * Parameters:
    *  txDataByte: Byte containing the data to transmit
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  `$INSTANCE_NAME`_txBuffer - RAM buffer pointer for save data for transmission
    *  `$INSTANCE_NAME`_txBufferWrite - cyclic index for write to txBuffer,
    *     checked to identify free space in txBuffer and incremented after each byte
    *     saved to buffer.
    *  `$INSTANCE_NAME`_txBufferRead - cyclic index for read from txBuffer,
    *     checked to identify free space in txBuffer.
    *  `$INSTANCE_NAME`_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to transmit any byte of data in a single transfer
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_PutChar(uint8_t txDataByte) `=ReentrantKeil($INSTANCE_NAME . "_PutChar")`
    {
    #if (`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED)
        /* The temporary output pointer is used since it takes two instructions
        *  to increment with a wrap, and we can't risk doing that with the real
        *  pointer and getting an interrupt in between instructions.
        */
        `$TxBuffRegSizeReplacementString` locTxBufferWrite;
        `$TxBuffRegSizeReplacementString` locTxBufferRead;

        do
        { /* Block if software buffer is full, so we don't overwrite. */

        #if ((`$INSTANCE_NAME`_TX_BUFFER_SIZE > `$INSTANCE_NAME`_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Disable TX interrupt to protect variables from modification */
            `$INSTANCE_NAME`_DisableTxInt();
        #endif /* (`$INSTANCE_NAME`_TX_BUFFER_SIZE > `$INSTANCE_NAME`_MAX_BYTE_VALUE) && (CY_PSOC3) */

            locTxBufferWrite = `$INSTANCE_NAME`_txBufferWrite;
            locTxBufferRead  = `$INSTANCE_NAME`_txBufferRead;

        #if ((`$INSTANCE_NAME`_TX_BUFFER_SIZE > `$INSTANCE_NAME`_MAX_BYTE_VALUE) && (CY_PSOC3))
            /* Enable interrupt to continue transmission */
            `$INSTANCE_NAME`_EnableTxInt();
        #endif /* (`$INSTANCE_NAME`_TX_BUFFER_SIZE > `$INSTANCE_NAME`_MAX_BYTE_VALUE) && (CY_PSOC3) */
        }
        while( (locTxBufferWrite < locTxBufferRead) ? (locTxBufferWrite == (locTxBufferRead - 1u)) :
                                ((locTxBufferWrite - locTxBufferRead) ==
                                (`$TxBuffRegSizeReplacementString`)(`$INSTANCE_NAME`_TX_BUFFER_SIZE - 1u)) );

        if( (locTxBufferRead == locTxBufferWrite) &&
            ((`$INSTANCE_NAME`_TXSTATUS_REG & `$INSTANCE_NAME`_TX_STS_FIFO_FULL) == 0u) )
        {
            /* Add directly to the FIFO */
            `$INSTANCE_NAME`_TXDATA_REG = txDataByte;
        }
        else
        {
            if(locTxBufferWrite >= `$INSTANCE_NAME`_TX_BUFFER_SIZE)
            {
                locTxBufferWrite = 0u;
            }
            /* Add to the software buffer. */
            `$INSTANCE_NAME`_txBuffer[locTxBufferWrite] = txDataByte;
            locTxBufferWrite++;

            /* Finally, update the real output pointer */
        #if ((`$INSTANCE_NAME`_TX_BUFFER_SIZE > `$INSTANCE_NAME`_MAX_BYTE_VALUE) && (CY_PSOC3))
            `$INSTANCE_NAME`_DisableTxInt();
        #endif /* (`$INSTANCE_NAME`_TX_BUFFER_SIZE > `$INSTANCE_NAME`_MAX_BYTE_VALUE) && (CY_PSOC3) */

            `$INSTANCE_NAME`_txBufferWrite = locTxBufferWrite;

        #if ((`$INSTANCE_NAME`_TX_BUFFER_SIZE > `$INSTANCE_NAME`_MAX_BYTE_VALUE) && (CY_PSOC3))
            `$INSTANCE_NAME`_EnableTxInt();
        #endif /* (`$INSTANCE_NAME`_TX_BUFFER_SIZE > `$INSTANCE_NAME`_MAX_BYTE_VALUE) && (CY_PSOC3) */

            if(0u != (`$INSTANCE_NAME`_TXSTATUS_REG & `$INSTANCE_NAME`_TX_STS_FIFO_EMPTY))
            {
                /* Trigger TX interrupt to send software buffer */
                `$INSTANCE_NAME`_SetPendingTxInt();
            }
        }

    #else

        while((`$INSTANCE_NAME`_TXSTATUS_REG & `$INSTANCE_NAME`_TX_STS_FIFO_FULL) != 0u)
        {
            /* Wait for room in the FIFO */
        }

        /* Add directly to the FIFO */
        `$INSTANCE_NAME`_TXDATA_REG = txDataByte;

    #endif /* `$INSTANCE_NAME`_TX_INTERRUPT_ENABLED */
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_PutString
    ********************************************************************************
    *
    * Summary:
    *  Sends a NULL terminated string to the TX buffer for transmission.
    *
    * Parameters:
    *  string[]: Pointer to the null terminated string array residing in RAM or ROM
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  `$INSTANCE_NAME`_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_PutString(const char8 string[]) `=ReentrantKeil($INSTANCE_NAME . "_PutString")`
    {
        uint16 bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(`$INSTANCE_NAME`_initVar != 0u)
        {
            /* This is a blocking function, it will not exit until all data is sent */
            while(string[bufIndex] != (char8) 0)
            {
                `$INSTANCE_NAME`_PutChar((uint8_t)string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_PutArray
    ********************************************************************************
    *
    * Summary:
    *  Places N bytes of data from a memory array into the TX buffer for
    *  transmission.
    *
    * Parameters:
    *  string[]: Address of the memory array residing in RAM or ROM.
    *  byteCount: Number of bytes to be transmitted. The type depends on TX Buffer
    *             Size parameter.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  `$INSTANCE_NAME`_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  If there is not enough memory in the TX buffer for the entire string, this
    *  function blocks until the last character of the string is loaded into the
    *  TX buffer.
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_PutArray(const uint8_t string[], `$TxBuffRegSizeReplacementString` byteCount)
                                                                    `=ReentrantKeil($INSTANCE_NAME . "_PutArray")`
    {
        `$TxBuffRegSizeReplacementString` bufIndex = 0u;

        /* If not Initialized then skip this function */
        if(`$INSTANCE_NAME`_initVar != 0u)
        {
            while(bufIndex < byteCount)
            {
                `$INSTANCE_NAME`_PutChar(string[bufIndex]);
                bufIndex++;
            }
        }
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_PutCRLF
    ********************************************************************************
    *
    * Summary:
    *  Writes a byte of data followed by a carriage return (0x0D) and line feed
    *  (0x0A) to the transmit buffer.
    *
    * Parameters:
    *  txDataByte: Data byte to transmit before the carriage return and line feed.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  `$INSTANCE_NAME`_initVar - checked to identify that the component has been
    *     initialized.
    *
    * Reentrant:
    *  No.
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_PutCRLF(uint8_t txDataByte) `=ReentrantKeil($INSTANCE_NAME . "_PutCRLF")`
    {
        /* If not Initialized then skip this function */
        if(`$INSTANCE_NAME`_initVar != 0u)
        {
            `$INSTANCE_NAME`_PutChar(txDataByte);
            `$INSTANCE_NAME`_PutChar(0x0Du);
            `$INSTANCE_NAME`_PutChar(0x0Au);
        }
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_GetTxBufferSize
    ********************************************************************************
    *
    * Summary:
    *  Returns the number of bytes in the TX buffer which are waiting to be 
    *  transmitted.
    *  * TX software buffer is disabled (TX Buffer Size parameter is equal to 4): 
    *    returns 0 for empty TX FIFO, 1 for not full TX FIFO or 4 for full TX FIFO.
    *  * TX software buffer is enabled: returns the number of bytes in the TX 
    *    software buffer which are waiting to be transmitted. Bytes available in the
    *    TX FIFO do not count.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  Number of bytes used in the TX buffer. Return value type depends on the TX 
    *  Buffer Size parameter.
    *
    * Global Variables:
    *  `$INSTANCE_NAME`_txBufferWrite - used to calculate left space.
    *  `$INSTANCE_NAME`_txBufferRead - used to calculate left space.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Allows the user to find out how full the TX Buffer is.
    *
    *******************************************************************************/
    `$TxBuffRegSizeReplacementString` `$INSTANCE_NAME`_GetTxBufferSize(void)
                                                            `=ReentrantKeil($INSTANCE_NAME . "_GetTxBufferSize")`
    {
        `$TxBuffRegSizeReplacementString` size;

    #if (`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        `$INSTANCE_NAME`_DisableTxInt();

        if(`$INSTANCE_NAME`_txBufferRead == `$INSTANCE_NAME`_txBufferWrite)
        {
            size = 0u;
        }
        else if(`$INSTANCE_NAME`_txBufferRead < `$INSTANCE_NAME`_txBufferWrite)
        {
            size = (`$INSTANCE_NAME`_txBufferWrite - `$INSTANCE_NAME`_txBufferRead);
        }
        else
        {
            size = (`$INSTANCE_NAME`_TX_BUFFER_SIZE - `$INSTANCE_NAME`_txBufferRead) +
                    `$INSTANCE_NAME`_txBufferWrite;
        }

        `$INSTANCE_NAME`_EnableTxInt();

    #else

        size = `$INSTANCE_NAME`_TXSTATUS_REG;

        /* Is the fifo is full. */
        if((size & `$INSTANCE_NAME`_TX_STS_FIFO_FULL) != 0u)
        {
            size = `$INSTANCE_NAME`_FIFO_LENGTH;
        }
        else if((size & `$INSTANCE_NAME`_TX_STS_FIFO_EMPTY) != 0u)
        {
            size = 0u;
        }
        else
        {
            /* We only know there is data in the fifo. */
            size = 1u;
        }

    #endif /* (`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED) */

    return(size);
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_ClearTxBuffer
    ********************************************************************************
    *
    * Summary:
    *  Clears all data from the TX buffer and hardware TX FIFO.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  `$INSTANCE_NAME`_txBufferWrite - cleared to zero.
    *  `$INSTANCE_NAME`_txBufferRead - cleared to zero.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  Setting the pointers to zero makes the system believe there is no data to
    *  read and writing will resume at address 0 overwriting any data that may have
    *  remained in the RAM.
    *
    * Side Effects:
    *  Data waiting in the transmit buffer is not sent; a byte that is currently
    *  transmitting finishes transmitting.
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_ClearTxBuffer(void) `=ReentrantKeil($INSTANCE_NAME . "_ClearTxBuffer")`
    {
        uint8_t enableInterrupts;

        enableInterrupts = CyEnterCriticalSection();
        /* Clear the HW FIFO */
        `$INSTANCE_NAME`_TXDATA_AUX_CTL_REG |= (uint8_t)  `$INSTANCE_NAME`_TX_FIFO_CLR;
        `$INSTANCE_NAME`_TXDATA_AUX_CTL_REG &= (uint8_t) ~`$INSTANCE_NAME`_TX_FIFO_CLR;
        CyExitCriticalSection(enableInterrupts);

    #if (`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED)

        /* Protect variables that could change on interrupt. */
        `$INSTANCE_NAME`_DisableTxInt();

        `$INSTANCE_NAME`_txBufferRead = 0u;
        `$INSTANCE_NAME`_txBufferWrite = 0u;

        /* Enable Tx interrupt. */
        `$INSTANCE_NAME`_EnableTxInt();

    #endif /* (`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_SendBreak
    ********************************************************************************
    *
    * Summary:
    *  Transmits a break signal on the bus.
    *
    * Parameters:
    *  uint8_t retMode:  Send Break return mode. See the following table for options.
    *   `$INSTANCE_NAME`_SEND_BREAK - Initialize registers for break, send the Break
    *       signal and return immediately.
    *   `$INSTANCE_NAME`_WAIT_FOR_COMPLETE_REINIT - Wait until break transmission is
    *       complete, reinitialize registers to normal transmission mode then return
    *   `$INSTANCE_NAME`_REINIT - Reinitialize registers to normal transmission mode
    *       then return.
    *   `$INSTANCE_NAME`_SEND_WAIT_REINIT - Performs both options: 
    *      `$INSTANCE_NAME`_SEND_BREAK and `$INSTANCE_NAME`_WAIT_FOR_COMPLETE_REINIT.
    *      This option is recommended for most cases.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  `$INSTANCE_NAME`_initVar - checked to identify that the component has been
    *     initialized.
    *  txPeriod - static variable, used for keeping TX period configuration.
    *
    * Reentrant:
    *  No.
    *
    * Theory:
    *  SendBreak function initializes registers to send 13-bit break signal. It is
    *  important to return the registers configuration to normal for continue 8-bit
    *  operation.
    *  There are 3 variants for this API usage:
    *  1) SendBreak(3) - function will send the Break signal and take care on the
    *     configuration returning. Function will block CPU until transmission
    *     complete.
    *  2) User may want to use blocking time if UART configured to the low speed
    *     operation
    *     Example for this case:
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     SendBreak(1);     - complete Break operation
    *  3) Same to 2) but user may want to initialize and use the interrupt to
    *     complete break operation.
    *     Example for this case:
    *     Initialize TX interrupt with "TX - On TX Complete" parameter
    *     SendBreak(0);     - initialize Break signal transmission
    *         Add your code here to use CPU time
    *     When interrupt appear with `$INSTANCE_NAME`_TX_STS_COMPLETE status:
    *     SendBreak(2);     - complete Break operation
    *
    * Side Effects:
    *  The `$INSTANCE_NAME`_SendBreak() function initializes registers to send a
    *  break signal.
    *  Break signal length depends on the break signal bits configuration.
    *  The register configuration should be reinitialized before normal 8-bit
    *  communication can continue.
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_SendBreak(uint8_t retMode) `=ReentrantKeil($INSTANCE_NAME . "_SendBreak")`
    {

        /* If not Initialized then skip this function*/
        if(`$INSTANCE_NAME`_initVar != 0u)
        {
            /* Set the Counter to 13-bits and transmit a 00 byte */
            /* When that is done then reset the counter value back */
            uint8_t tmpStat;

        #if(`$INSTANCE_NAME`_HD_ENABLED) /* Half Duplex mode*/

            if( (retMode == `$INSTANCE_NAME`_SEND_BREAK) ||
                (retMode == `$INSTANCE_NAME`_SEND_WAIT_REINIT ) )
            {
                /* CTRL_HD_SEND_BREAK - sends break bits in HD mode */
                `$INSTANCE_NAME`_WriteControlRegister(`$INSTANCE_NAME`_ReadControlRegister() |
                                                      `$INSTANCE_NAME`_CTRL_HD_SEND_BREAK);
                /* Send zeros */
                `$INSTANCE_NAME`_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = `$INSTANCE_NAME`_TXSTATUS_REG;
                }
                while((tmpStat & `$INSTANCE_NAME`_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == `$INSTANCE_NAME`_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == `$INSTANCE_NAME`_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = `$INSTANCE_NAME`_TXSTATUS_REG;
                }
                while(((uint8_t)~tmpStat & `$INSTANCE_NAME`_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == `$INSTANCE_NAME`_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == `$INSTANCE_NAME`_REINIT) ||
                (retMode == `$INSTANCE_NAME`_SEND_WAIT_REINIT) )
            {
                `$INSTANCE_NAME`_WriteControlRegister(`$INSTANCE_NAME`_ReadControlRegister() &
                                              (uint8_t)~`$INSTANCE_NAME`_CTRL_HD_SEND_BREAK);
            }

        #else /* `$INSTANCE_NAME`_HD_ENABLED Full Duplex mode */

            static uint8_t txPeriod;

            if( (retMode == `$INSTANCE_NAME`_SEND_BREAK) ||
                (retMode == `$INSTANCE_NAME`_SEND_WAIT_REINIT) )
            {
                /* CTRL_HD_SEND_BREAK - skip to send parity bit at Break signal in Full Duplex mode */
                #if( (`$INSTANCE_NAME`_PARITY_TYPE != `$INSTANCE_NAME`__B_UART__NONE_REVB) || \
                                    (`$INSTANCE_NAME`_PARITY_TYPE_SW != 0u) )
                    `$INSTANCE_NAME`_WriteControlRegister(`$INSTANCE_NAME`_ReadControlRegister() |
                                                          `$INSTANCE_NAME`_CTRL_HD_SEND_BREAK);
                #endif /* End `$INSTANCE_NAME`_PARITY_TYPE != `$INSTANCE_NAME`__B_UART__NONE_REVB  */

                #if(`$INSTANCE_NAME`_TXCLKGEN_DP)
                    txPeriod = `$INSTANCE_NAME`_TXBITCLKTX_COMPLETE_REG;
                    `$INSTANCE_NAME`_TXBITCLKTX_COMPLETE_REG = `$INSTANCE_NAME`_TXBITCTR_BREAKBITS;
                #else
                    txPeriod = `$INSTANCE_NAME`_TXBITCTR_PERIOD_REG;
                    `$INSTANCE_NAME`_TXBITCTR_PERIOD_REG = `$INSTANCE_NAME`_TXBITCTR_BREAKBITS8X;
                #endif /* End `$INSTANCE_NAME`_TXCLKGEN_DP */

                /* Send zeros */
                `$INSTANCE_NAME`_TXDATA_REG = 0u;

                do /* Wait until transmit starts */
                {
                    tmpStat = `$INSTANCE_NAME`_TXSTATUS_REG;
                }
                while((tmpStat & `$INSTANCE_NAME`_TX_STS_FIFO_EMPTY) != 0u);
            }

            if( (retMode == `$INSTANCE_NAME`_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == `$INSTANCE_NAME`_SEND_WAIT_REINIT) )
            {
                do /* Wait until transmit complete */
                {
                    tmpStat = `$INSTANCE_NAME`_TXSTATUS_REG;
                }
                while(((uint8_t)~tmpStat & `$INSTANCE_NAME`_TX_STS_COMPLETE) != 0u);
            }

            if( (retMode == `$INSTANCE_NAME`_WAIT_FOR_COMPLETE_REINIT) ||
                (retMode == `$INSTANCE_NAME`_REINIT) ||
                (retMode == `$INSTANCE_NAME`_SEND_WAIT_REINIT) )
            {

            #if(`$INSTANCE_NAME`_TXCLKGEN_DP)
                `$INSTANCE_NAME`_TXBITCLKTX_COMPLETE_REG = txPeriod;
            #else
                `$INSTANCE_NAME`_TXBITCTR_PERIOD_REG = txPeriod;
            #endif /* End `$INSTANCE_NAME`_TXCLKGEN_DP */

            #if( (`$INSTANCE_NAME`_PARITY_TYPE != `$INSTANCE_NAME`__B_UART__NONE_REVB) || \
                 (`$INSTANCE_NAME`_PARITY_TYPE_SW != 0u) )
                `$INSTANCE_NAME`_WriteControlRegister(`$INSTANCE_NAME`_ReadControlRegister() &
                                                      (uint8_t) ~`$INSTANCE_NAME`_CTRL_HD_SEND_BREAK);
            #endif /* End `$INSTANCE_NAME`_PARITY_TYPE != NONE */
            }
        #endif    /* End `$INSTANCE_NAME`_HD_ENABLED */
        }
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_SetTxAddressMode
    ********************************************************************************
    *
    * Summary:
    *  Configures the transmitter to signal the next bytes is address or data.
    *
    * Parameters:
    *  addressMode: 
    *       `$INSTANCE_NAME`_SET_SPACE - Configure the transmitter to send the next
    *                                    byte as a data.
    *       `$INSTANCE_NAME`_SET_MARK  - Configure the transmitter to send the next
    *                                    byte as an address.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  This function sets and clears `$INSTANCE_NAME`_CTRL_MARK bit in the Control
    *  register.
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_SetTxAddressMode(uint8_t addressMode) `=ReentrantKeil($INSTANCE_NAME . "_SetTxAddressMode")`
    {
        /* Mark/Space sending enable */
        if(addressMode != 0u)
        {
        #if( `$INSTANCE_NAME`_CONTROL_REG_REMOVED == 0u )
            `$INSTANCE_NAME`_WriteControlRegister(`$INSTANCE_NAME`_ReadControlRegister() |
                                                  `$INSTANCE_NAME`_CTRL_MARK);
        #endif /* End `$INSTANCE_NAME`_CONTROL_REG_REMOVED == 0u */
        }
        else
        {
        #if( `$INSTANCE_NAME`_CONTROL_REG_REMOVED == 0u )
            `$INSTANCE_NAME`_WriteControlRegister(`$INSTANCE_NAME`_ReadControlRegister() &
                                                  (uint8_t) ~`$INSTANCE_NAME`_CTRL_MARK);
        #endif /* End `$INSTANCE_NAME`_CONTROL_REG_REMOVED == 0u */
        }
    }

#endif  /* End`$INSTANCE_NAME`_TX_ENABLED */

#if(`$INSTANCE_NAME`_HD_ENABLED)


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_LoadRxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the receiver configuration in half duplex mode. After calling this
    *  function, the UART is ready to receive data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the transmitter
    *  configuration.
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_LoadRxConfig(void) `=ReentrantKeil($INSTANCE_NAME . "_LoadRxConfig")`
    {
        `$INSTANCE_NAME`_WriteControlRegister(`$INSTANCE_NAME`_ReadControlRegister() &
                                                (uint8_t)~`$INSTANCE_NAME`_CTRL_HD_SEND);
        `$INSTANCE_NAME`_RXBITCTR_PERIOD_REG = `$INSTANCE_NAME`_HD_RXBITCTR_INIT;

    #if (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED)
        /* Enable RX interrupt after set RX configuration */
        `$INSTANCE_NAME`_SetRxInterruptMode(`$INSTANCE_NAME`_INIT_RX_INTERRUPTS_MASK);
    #endif /* (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED) */
    }


    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_LoadTxConfig
    ********************************************************************************
    *
    * Summary:
    *  Loads the transmitter configuration in half duplex mode. After calling this
    *  function, the UART is ready to transmit data.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Side Effects:
    *  Valid only in half duplex mode. You must make sure that the previous
    *  transaction is complete and it is safe to unload the receiver configuration.
    *
    *******************************************************************************/
    void `$INSTANCE_NAME`_LoadTxConfig(void) `=ReentrantKeil($INSTANCE_NAME . "_LoadTxConfig")`
    {
    #if (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED)
        /* Disable RX interrupts before set TX configuration */
        `$INSTANCE_NAME`_SetRxInterruptMode(0u);
    #endif /* (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED) */

        `$INSTANCE_NAME`_WriteControlRegister(`$INSTANCE_NAME`_ReadControlRegister() | `$INSTANCE_NAME`_CTRL_HD_SEND);
        `$INSTANCE_NAME`_RXBITCTR_PERIOD_REG = `$INSTANCE_NAME`_HD_TXBITCTR_INIT;
    }

#endif  /* `$INSTANCE_NAME`_HD_ENABLED */


/* [] END OF FILE */
