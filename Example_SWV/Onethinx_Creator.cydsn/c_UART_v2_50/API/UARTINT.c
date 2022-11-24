/*******************************************************************************
* File Name: `$INSTANCE_NAME`INT.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides all Interrupt Service functionality of the UART component
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "`$INSTANCE_NAME`.h"
`$CY_API_CALLBACK_HEADER_INCLUDE`


/***************************************
* Custom Declarations
***************************************/
/* `#START CUSTOM_DECLARATIONS` Place your declaration here */

/* `#END` */

#if (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED && (`$INSTANCE_NAME`_RX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED))
    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_RXISR
    ********************************************************************************
    *
    * Summary:
    *  Interrupt Service Routine for RX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  `$INSTANCE_NAME`_rxBuffer - RAM buffer pointer for save received data.
    *  `$INSTANCE_NAME`_rxBufferWrite - cyclic index for write to rxBuffer,
    *     increments after each byte saved to buffer.
    *  `$INSTANCE_NAME`_rxBufferRead - cyclic index for read from rxBuffer,
    *     checked to detect overflow condition.
    *  `$INSTANCE_NAME`_rxBufferOverflow - software overflow flag. Set to one
    *     when `$INSTANCE_NAME`_rxBufferWrite index overtakes
    *     `$INSTANCE_NAME`_rxBufferRead index.
    *  `$INSTANCE_NAME`_rxBufferLoopDetect - additional variable to detect overflow.
    *     Set to one when `$INSTANCE_NAME`_rxBufferWrite is equal to
    *    `$INSTANCE_NAME`_rxBufferRead
    *  `$INSTANCE_NAME`_rxAddressMode - this variable contains the Address mode,
    *     selected in customizer or set by UART_SetRxAddressMode() API.
    *  `$INSTANCE_NAME`_rxAddressDetected - set to 1 when correct address received,
    *     and analysed to store following addressed data bytes to the buffer.
    *     When not correct address received, set to 0 to skip following data bytes.
    *
    *******************************************************************************/
    CY_ISR(`$INSTANCE_NAME`_RXISR)
    {
        uint8_t readData;
        uint8_t readStatus;
        uint8_t increment_pointer = 0u;

    #if(CY_PSOC3)
        uint8_t int_en;
    #endif /* (CY_PSOC3) */

    #ifdef `$INSTANCE_NAME`_RXISR_ENTRY_CALLBACK
        `$INSTANCE_NAME`_RXISR_EntryCallback();
    #endif /* `$INSTANCE_NAME`_RXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START `$INSTANCE_NAME`_RXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        do
        {
            /* Read receiver status register */
            readStatus = `$INSTANCE_NAME`_RXSTATUS_REG;
            /* Copy the same status to readData variable for backward compatibility support 
            *  of the user code in `$INSTANCE_NAME`_RXISR_ERROR` section. 
            */
            readData = readStatus;

            if((readStatus & (`$INSTANCE_NAME`_RX_STS_BREAK | 
                            `$INSTANCE_NAME`_RX_STS_PAR_ERROR |
                            `$INSTANCE_NAME`_RX_STS_STOP_ERROR | 
                            `$INSTANCE_NAME`_RX_STS_OVERRUN)) != 0u)
            {
                /* ERROR handling. */
                `$INSTANCE_NAME`_errorStatus |= readStatus & ( `$INSTANCE_NAME`_RX_STS_BREAK | 
                                                            `$INSTANCE_NAME`_RX_STS_PAR_ERROR | 
                                                            `$INSTANCE_NAME`_RX_STS_STOP_ERROR | 
                                                            `$INSTANCE_NAME`_RX_STS_OVERRUN);
                /* `#START `$INSTANCE_NAME`_RXISR_ERROR` */

                /* `#END` */
                
            #ifdef `$INSTANCE_NAME`_RXISR_ERROR_CALLBACK
                `$INSTANCE_NAME`_RXISR_ERROR_Callback();
            #endif /* `$INSTANCE_NAME`_RXISR_ERROR_CALLBACK */
            }
            
            if((readStatus & `$INSTANCE_NAME`_RX_STS_FIFO_NOTEMPTY) != 0u)
            {
                /* Read data from the RX data register */
                readData = `$INSTANCE_NAME`_RXDATA_REG;
            #if (`$INSTANCE_NAME`_RXHW_ADDRESS_ENABLED)
                if(`$INSTANCE_NAME`_rxAddressMode == (uint8_t)`$INSTANCE_NAME`__B_UART__AM_SW_DETECT_TO_BUFFER)
                {
                    if((readStatus & `$INSTANCE_NAME`_RX_STS_MRKSPC) != 0u)
                    {
                        if ((readStatus & `$INSTANCE_NAME`_RX_STS_ADDR_MATCH) != 0u)
                        {
                            `$INSTANCE_NAME`_rxAddressDetected = 1u;
                        }
                        else
                        {
                            `$INSTANCE_NAME`_rxAddressDetected = 0u;
                        }
                    }
                    if(`$INSTANCE_NAME`_rxAddressDetected != 0u)
                    {   /* Store only addressed data */
                        `$INSTANCE_NAME`_rxBuffer[`$INSTANCE_NAME`_rxBufferWrite] = readData;
                        increment_pointer = 1u;
                    }
                }
                else /* Without software addressing */
                {
                    `$INSTANCE_NAME`_rxBuffer[`$INSTANCE_NAME`_rxBufferWrite] = readData;
                    increment_pointer = 1u;
                }
            #else  /* Without addressing */
                `$INSTANCE_NAME`_rxBuffer[`$INSTANCE_NAME`_rxBufferWrite] = readData;
                increment_pointer = 1u;
            #endif /* (`$INSTANCE_NAME`_RXHW_ADDRESS_ENABLED) */

                /* Do not increment buffer pointer when skip not addressed data */
                if(increment_pointer != 0u)
                {
                    if(`$INSTANCE_NAME`_rxBufferLoopDetect != 0u)
                    {   /* Set Software Buffer status Overflow */
                        `$INSTANCE_NAME`_rxBufferOverflow = 1u;
                    }
                    /* Set next pointer. */
                    `$INSTANCE_NAME`_rxBufferWrite++;

                    /* Check pointer for a loop condition */
                    if(`$INSTANCE_NAME`_rxBufferWrite >= `$INSTANCE_NAME`_RX_BUFFER_SIZE)
                    {
                        `$INSTANCE_NAME`_rxBufferWrite = 0u;
                    }

                    /* Detect pre-overload condition and set flag */
                    if(`$INSTANCE_NAME`_rxBufferWrite == `$INSTANCE_NAME`_rxBufferRead)
                    {
                        `$INSTANCE_NAME`_rxBufferLoopDetect = 1u;
                        /* When Hardware Flow Control selected */
                        #if (`$INSTANCE_NAME`_FLOW_CONTROL != 0u)
                            /* Disable RX interrupt mask, it is enabled when user read data from the buffer using APIs */
                            `$INSTANCE_NAME`_RXSTATUS_MASK_REG  &= (uint8_t)~`$INSTANCE_NAME`_RX_STS_FIFO_NOTEMPTY;
                            CyIntClearPending(`$INSTANCE_NAME`_RX_VECT_NUM);
                            break; /* Break the reading of the FIFO loop, leave the data there for generating RTS signal */
                        #endif /* (`$INSTANCE_NAME`_FLOW_CONTROL != 0u) */
                    }
                }
            }
        }while((readStatus & `$INSTANCE_NAME`_RX_STS_FIFO_NOTEMPTY) != 0u);

        /* User code required at end of ISR (Optional) */
        /* `#START `$INSTANCE_NAME`_RXISR_END` */

        /* `#END` */

    #ifdef `$INSTANCE_NAME`_RXISR_EXIT_CALLBACK
        `$INSTANCE_NAME`_RXISR_ExitCallback();
    #endif /* `$INSTANCE_NAME`_RXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
    }
    
#endif /* (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED && (`$INSTANCE_NAME`_RX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED)) */


#if (`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED && `$INSTANCE_NAME`_TX_ENABLED)
    /*******************************************************************************
    * Function Name: `$INSTANCE_NAME`_TXISR
    ********************************************************************************
    *
    * Summary:
    * Interrupt Service Routine for the TX portion of the UART
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    * Global Variables:
    *  `$INSTANCE_NAME`_txBuffer - RAM buffer pointer for transmit data from.
    *  `$INSTANCE_NAME`_txBufferRead - cyclic index for read and transmit data
    *     from txBuffer, increments after each transmitted byte.
    *  `$INSTANCE_NAME`_rxBufferWrite - cyclic index for write to txBuffer,
    *     checked to detect available for transmission bytes.
    *
    *******************************************************************************/
    CY_ISR(`$INSTANCE_NAME`_TXISR)
    {
    #if(CY_PSOC3)
        uint8_t int_en;
    #endif /* (CY_PSOC3) */

    #ifdef `$INSTANCE_NAME`_TXISR_ENTRY_CALLBACK
        `$INSTANCE_NAME`_TXISR_EntryCallback();
    #endif /* `$INSTANCE_NAME`_TXISR_ENTRY_CALLBACK */

        /* User code required at start of ISR */
        /* `#START `$INSTANCE_NAME`_TXISR_START` */

        /* `#END` */

    #if(CY_PSOC3)   /* Make sure nested interrupt is enabled */
        int_en = EA;
        CyGlobalIntEnable;
    #endif /* (CY_PSOC3) */

        while((`$INSTANCE_NAME`_txBufferRead != `$INSTANCE_NAME`_txBufferWrite) &&
             ((`$INSTANCE_NAME`_TXSTATUS_REG & `$INSTANCE_NAME`_TX_STS_FIFO_FULL) == 0u))
        {
            /* Check pointer wrap around */
            if(`$INSTANCE_NAME`_txBufferRead >= `$INSTANCE_NAME`_TX_BUFFER_SIZE)
            {
                `$INSTANCE_NAME`_txBufferRead = 0u;
            }

            `$INSTANCE_NAME`_TXDATA_REG = `$INSTANCE_NAME`_txBuffer[`$INSTANCE_NAME`_txBufferRead];

            /* Set next pointer */
            `$INSTANCE_NAME`_txBufferRead++;
        }

        /* User code required at end of ISR (Optional) */
        /* `#START `$INSTANCE_NAME`_TXISR_END` */

        /* `#END` */

    #ifdef `$INSTANCE_NAME`_TXISR_EXIT_CALLBACK
        `$INSTANCE_NAME`_TXISR_ExitCallback();
    #endif /* `$INSTANCE_NAME`_TXISR_EXIT_CALLBACK */

    #if(CY_PSOC3)
        EA = int_en;
    #endif /* (CY_PSOC3) */
   }
#endif /* (`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED && `$INSTANCE_NAME`_TX_ENABLED) */


/* [] END OF FILE */
