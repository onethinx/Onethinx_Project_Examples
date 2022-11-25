/*******************************************************************************
* File Name: `$INSTANCE_NAME`.h
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  Contains the function prototypes and constants available to the UART
*  user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_UART_`$INSTANCE_NAME`_H)
#define CY_UART_`$INSTANCE_NAME`_H

#include "cyfitter.h"
//#include "project.h"
//#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */
#include "syslib/cy_syslib.h"  /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

/***************************************
* Conditional Compilation Parameters
***************************************/

#define `$INSTANCE_NAME`_RX_ENABLED                     (`$Enable_RX`u)
#define `$INSTANCE_NAME`_TX_ENABLED                     (`$Enable_TX`u)
#define `$INSTANCE_NAME`_HD_ENABLED                     (`$HalfDuplexEn`u)
#define `$INSTANCE_NAME`_RX_INTERRUPT_ENABLED           (`$Enable_RXIntInterrupt`u)
#define `$INSTANCE_NAME`_TX_INTERRUPT_ENABLED           (`$Enable_TXIntInterrupt`u)
#define `$INSTANCE_NAME`_INTERNAL_CLOCK_USED            (`$InternalClockUsed`u)
#define `$INSTANCE_NAME`_RXHW_ADDRESS_ENABLED           (`$EnableHWAddress`u)
#define `$INSTANCE_NAME`_OVER_SAMPLE_COUNT              (`$OverSamplingRate`u)
#define `$INSTANCE_NAME`_PARITY_TYPE                    (`$ParityType`u)
#define `$INSTANCE_NAME`_PARITY_TYPE_SW                 (`$ParityTypeSw`u)
#define `$INSTANCE_NAME`_BREAK_DETECT                   (`$BreakDetect`u)
#define `$INSTANCE_NAME`_BREAK_BITS_TX                  (`$BreakBitsTX`u)
#define `$INSTANCE_NAME`_BREAK_BITS_RX                  (`$BreakBitsRX`u)
#define `$INSTANCE_NAME`_TXCLKGEN_DP                    (`$TXBitClkGenDP`u)
#define `$INSTANCE_NAME`_USE23POLLING                   (`$Use23Polling`u)
#define `$INSTANCE_NAME`_FLOW_CONTROL                   (`$FlowControl`u)
#define `$INSTANCE_NAME`_CLK_FREQ                       (`$FlowControl`u)
#define `$INSTANCE_NAME`_TX_BUFFER_SIZE                 (`$TXBufferSize`u)
#define `$INSTANCE_NAME`_RX_BUFFER_SIZE                 (`$RXBufferSize`u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
//#if !defined (CY_PSOC5LP)
//    #error Component `$CY_COMPONENT_NAME` requires cy_boot v3.0 or later
//#endif /* (CY_PSOC5LP) */

#if defined(`$INSTANCE_NAME`_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define `$INSTANCE_NAME`_CONTROL_REG_REMOVED            (0u)
#else
    #define `$INSTANCE_NAME`_CONTROL_REG_REMOVED            (1u)
#endif /* End `$INSTANCE_NAME`_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct `$INSTANCE_NAME`_backupStruct_
{
    uint8_t enableState;

    #if(`$INSTANCE_NAME`_CONTROL_REG_REMOVED == 0u)
        uint8_t cr;
    #endif /* End `$INSTANCE_NAME`_CONTROL_REG_REMOVED */

} `$INSTANCE_NAME`_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void `$INSTANCE_NAME`_Start(void) `=ReentrantKeil($INSTANCE_NAME . "_Start")`;
void `$INSTANCE_NAME`_Stop(void) `=ReentrantKeil($INSTANCE_NAME . "_Stop")`;
uint8_t `$INSTANCE_NAME`_ReadControlRegister(void) `=ReentrantKeil($INSTANCE_NAME . "_ReadControlRegister")`;
void `$INSTANCE_NAME`_WriteControlRegister(uint8_t control) `=ReentrantKeil($INSTANCE_NAME . "_WriteControlRegister")`;

void `$INSTANCE_NAME`_Init(void) `=ReentrantKeil($INSTANCE_NAME . "_Init")`;
void `$INSTANCE_NAME`_Enable(void) `=ReentrantKeil($INSTANCE_NAME . "_Enable")`;
void `$INSTANCE_NAME`_SaveConfig(void) `=ReentrantKeil($INSTANCE_NAME . "_SaveConfig")`;
void `$INSTANCE_NAME`_RestoreConfig(void) `=ReentrantKeil($INSTANCE_NAME . "_RestoreConfig")`;
void `$INSTANCE_NAME`_Sleep(void) `=ReentrantKeil($INSTANCE_NAME . "_Sleep")`;
void `$INSTANCE_NAME`_Wakeup(void) `=ReentrantKeil($INSTANCE_NAME . "_Wakeup")`;

/* Only if RX is enabled */
#if( (`$INSTANCE_NAME`_RX_ENABLED) || (`$INSTANCE_NAME`_HD_ENABLED) )

    #if (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED)
        #define `$INSTANCE_NAME`_EnableRxInt()  CyIntEnable (`$INSTANCE_NAME`_RX_VECT_NUM)
        #define `$INSTANCE_NAME`_DisableRxInt() CyIntDisable(`$INSTANCE_NAME`_RX_VECT_NUM)
        CY_ISR_PROTO(`$INSTANCE_NAME`_RXISR);
    #endif /* `$INSTANCE_NAME`_RX_INTERRUPT_ENABLED */

    void `$INSTANCE_NAME`_SetRxAddressMode(uint8_t addressMode)
                                                           `=ReentrantKeil($INSTANCE_NAME . "_SetRxAddressMode")`;
    void `$INSTANCE_NAME`_SetRxAddress1(uint8_t address) `=ReentrantKeil($INSTANCE_NAME . "_SetRxAddress1")`;
    void `$INSTANCE_NAME`_SetRxAddress2(uint8_t address) `=ReentrantKeil($INSTANCE_NAME . "_SetRxAddress2")`;

    void  `$INSTANCE_NAME`_SetRxInterruptMode(uint8_t intSrc) `=ReentrantKeil($INSTANCE_NAME . "_SetRxInterruptMode")`;
    uint8_t `$INSTANCE_NAME`_ReadRxData(void) `=ReentrantKeil($INSTANCE_NAME . "_ReadRxData")`;
    uint8_t `$INSTANCE_NAME`_ReadRxStatus(void) `=ReentrantKeil($INSTANCE_NAME . "_ReadRxStatus")`;
    uint8_t `$INSTANCE_NAME`_GetChar(void) `=ReentrantKeil($INSTANCE_NAME . "_GetChar")`;
    uint16 `$INSTANCE_NAME`_GetByte(void) `=ReentrantKeil($INSTANCE_NAME . "_GetByte")`;
    `$RxBuffRegSizeReplacementString` `$INSTANCE_NAME`_GetRxBufferSize(void)
                                                            `=ReentrantKeil($INSTANCE_NAME . "_GetRxBufferSize")`;
    void `$INSTANCE_NAME`_ClearRxBuffer(void) `=ReentrantKeil($INSTANCE_NAME . "_ClearRxBuffer")`;

    /* Obsolete functions, defines for backward compatible */
    #define `$INSTANCE_NAME`_GetRxInterruptSource   `$INSTANCE_NAME`_ReadRxStatus

#endif /* End (`$INSTANCE_NAME`_RX_ENABLED) || (`$INSTANCE_NAME`_HD_ENABLED) */

/* Only if TX is enabled */
#if(`$INSTANCE_NAME`_TX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED)

    #if(`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED)
        #define `$INSTANCE_NAME`_EnableTxInt()  CyIntEnable (`$INSTANCE_NAME`_TX_VECT_NUM)
        #define `$INSTANCE_NAME`_DisableTxInt() CyIntDisable(`$INSTANCE_NAME`_TX_VECT_NUM)
        #define `$INSTANCE_NAME`_SetPendingTxInt() CyIntSetPending(`$INSTANCE_NAME`_TX_VECT_NUM)
        #define `$INSTANCE_NAME`_ClearPendingTxInt() CyIntClearPending(`$INSTANCE_NAME`_TX_VECT_NUM)
        CY_ISR_PROTO(`$INSTANCE_NAME`_TXISR);
    #endif /* `$INSTANCE_NAME`_TX_INTERRUPT_ENABLED */

    void `$INSTANCE_NAME`_SetTxInterruptMode(uint8_t intSrc) `=ReentrantKeil($INSTANCE_NAME . "_SetTxInterruptMode")`;
    void `$INSTANCE_NAME`_WriteTxData(uint8_t txDataByte) `=ReentrantKeil($INSTANCE_NAME . "_WriteTxData")`;
    uint8_t `$INSTANCE_NAME`_ReadTxStatus(void) `=ReentrantKeil($INSTANCE_NAME . "_ReadTxStatus")`;
    void `$INSTANCE_NAME`_PutChar(uint8_t txDataByte) `=ReentrantKeil($INSTANCE_NAME . "_PutChar")`;
    void `$INSTANCE_NAME`_PutString(const char8 string[]) `=ReentrantKeil($INSTANCE_NAME . "_PutString")`;
    void `$INSTANCE_NAME`_PutArray(const uint8_t string[], `$TxBuffRegSizeReplacementString` byteCount)
                                                            `=ReentrantKeil($INSTANCE_NAME . "_PutArray")`;
    void `$INSTANCE_NAME`_PutCRLF(uint8_t txDataByte) `=ReentrantKeil($INSTANCE_NAME . "_PutCRLF")`;
    void `$INSTANCE_NAME`_ClearTxBuffer(void) `=ReentrantKeil($INSTANCE_NAME . "_ClearTxBuffer")`;
    void `$INSTANCE_NAME`_SetTxAddressMode(uint8_t addressMode) `=ReentrantKeil($INSTANCE_NAME . "_SetTxAddressMode")`;
    void `$INSTANCE_NAME`_SendBreak(uint8_t retMode) `=ReentrantKeil($INSTANCE_NAME . "_SendBreak")`;
    `$TxBuffRegSizeReplacementString` `$INSTANCE_NAME`_GetTxBufferSize(void)
                                                            `=ReentrantKeil($INSTANCE_NAME . "_GetTxBufferSize")`;
    /* Obsolete functions, defines for backward compatible */
    #define `$INSTANCE_NAME`_PutStringConst         `$INSTANCE_NAME`_PutString
    #define `$INSTANCE_NAME`_PutArrayConst          `$INSTANCE_NAME`_PutArray
    #define `$INSTANCE_NAME`_GetTxInterruptSource   `$INSTANCE_NAME`_ReadTxStatus

#endif /* End `$INSTANCE_NAME`_TX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED */

#if(`$INSTANCE_NAME`_HD_ENABLED)
    void `$INSTANCE_NAME`_LoadRxConfig(void) `=ReentrantKeil($INSTANCE_NAME . "_LoadRxConfig")`;
    void `$INSTANCE_NAME`_LoadTxConfig(void) `=ReentrantKeil($INSTANCE_NAME . "_LoadTxConfig")`;
#endif /* End `$INSTANCE_NAME`_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_`$INSTANCE_NAME`) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    `$INSTANCE_NAME`_CyBtldrCommStart(void) CYSMALL `=ReentrantKeil($INSTANCE_NAME . "_CyBtldrCommStart")`;
    void    `$INSTANCE_NAME`_CyBtldrCommStop(void) CYSMALL `=ReentrantKeil($INSTANCE_NAME . "_CyBtldrCommStop")`;
    void    `$INSTANCE_NAME`_CyBtldrCommReset(void) CYSMALL `=ReentrantKeil($INSTANCE_NAME . "_CyBtldrCommReset")`;
    cystatus `$INSTANCE_NAME`_CyBtldrCommWrite(const uint8_t pData[], uint16 size, uint16 * count, uint8_t timeOut) CYSMALL
             `=ReentrantKeil($INSTANCE_NAME . "_CyBtldrCommWrite")`;
    cystatus `$INSTANCE_NAME`_CyBtldrCommRead(uint8_t pData[], uint16 size, uint16 * count, uint8_t timeOut) CYSMALL
             `=ReentrantKeil($INSTANCE_NAME . "_CyBtldrCommRead")`;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_`$INSTANCE_NAME`)
        #define CyBtldrCommStart    `$INSTANCE_NAME`_CyBtldrCommStart
        #define CyBtldrCommStop     `$INSTANCE_NAME`_CyBtldrCommStop
        #define CyBtldrCommReset    `$INSTANCE_NAME`_CyBtldrCommReset
        #define CyBtldrCommWrite    `$INSTANCE_NAME`_CyBtldrCommWrite
        #define CyBtldrCommRead     `$INSTANCE_NAME`_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_`$INSTANCE_NAME`) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define `$INSTANCE_NAME`_BYTE2BYTE_TIME_OUT (25u)
    #define `$INSTANCE_NAME`_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define `$INSTANCE_NAME`_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define `$INSTANCE_NAME`_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define `$INSTANCE_NAME`_SET_SPACE      (0x00u)
#define `$INSTANCE_NAME`_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (`$INSTANCE_NAME`_TX_ENABLED) || (`$INSTANCE_NAME`_HD_ENABLED) )
    #if(`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED)
        #define `$INSTANCE_NAME`_TX_VECT_NUM            (uint8_t)`$INSTANCE_NAME`_TXInternalInterrupt__INTC_NUMBER
        #define `$INSTANCE_NAME`_TX_PRIOR_NUM           (uint8_t)`$INSTANCE_NAME`_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* `$INSTANCE_NAME`_TX_INTERRUPT_ENABLED */

    #define `$INSTANCE_NAME`_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define `$INSTANCE_NAME`_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define `$INSTANCE_NAME`_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(`$INSTANCE_NAME`_TX_ENABLED)
        #define `$INSTANCE_NAME`_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (`$INSTANCE_NAME`_HD_ENABLED) */
        #define `$INSTANCE_NAME`_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (`$INSTANCE_NAME`_TX_ENABLED) */

    #define `$INSTANCE_NAME`_TX_STS_COMPLETE            (uint8_t)(0x01u << `$INSTANCE_NAME`_TX_STS_COMPLETE_SHIFT)
    #define `$INSTANCE_NAME`_TX_STS_FIFO_EMPTY          (uint8_t)(0x01u << `$INSTANCE_NAME`_TX_STS_FIFO_EMPTY_SHIFT)
    #define `$INSTANCE_NAME`_TX_STS_FIFO_FULL           (uint8_t)(0x01u << `$INSTANCE_NAME`_TX_STS_FIFO_FULL_SHIFT)
    #define `$INSTANCE_NAME`_TX_STS_FIFO_NOT_FULL       (uint8_t)(0x01u << `$INSTANCE_NAME`_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (`$INSTANCE_NAME`_TX_ENABLED) || (`$INSTANCE_NAME`_HD_ENABLED)*/

#if( (`$INSTANCE_NAME`_RX_ENABLED) || (`$INSTANCE_NAME`_HD_ENABLED) )
    #if(`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED)
        #define `$INSTANCE_NAME`_RX_VECT_NUM            (uint8_t)`$INSTANCE_NAME`_RXInternalInterrupt__INTC_NUMBER
        #define `$INSTANCE_NAME`_RX_PRIOR_NUM           (uint8_t)`$INSTANCE_NAME`_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* `$INSTANCE_NAME`_RX_INTERRUPT_ENABLED */
    #define `$INSTANCE_NAME`_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define `$INSTANCE_NAME`_RX_STS_BREAK_SHIFT             (0x01u)
    #define `$INSTANCE_NAME`_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define `$INSTANCE_NAME`_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define `$INSTANCE_NAME`_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define `$INSTANCE_NAME`_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define `$INSTANCE_NAME`_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define `$INSTANCE_NAME`_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define `$INSTANCE_NAME`_RX_STS_MRKSPC           (uint8_t)(0x01u << `$INSTANCE_NAME`_RX_STS_MRKSPC_SHIFT)
    #define `$INSTANCE_NAME`_RX_STS_BREAK            (uint8_t)(0x01u << `$INSTANCE_NAME`_RX_STS_BREAK_SHIFT)
    #define `$INSTANCE_NAME`_RX_STS_PAR_ERROR        (uint8_t)(0x01u << `$INSTANCE_NAME`_RX_STS_PAR_ERROR_SHIFT)
    #define `$INSTANCE_NAME`_RX_STS_STOP_ERROR       (uint8_t)(0x01u << `$INSTANCE_NAME`_RX_STS_STOP_ERROR_SHIFT)
    #define `$INSTANCE_NAME`_RX_STS_OVERRUN          (uint8_t)(0x01u << `$INSTANCE_NAME`_RX_STS_OVERRUN_SHIFT)
    #define `$INSTANCE_NAME`_RX_STS_FIFO_NOTEMPTY    (uint8_t)(0x01u << `$INSTANCE_NAME`_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define `$INSTANCE_NAME`_RX_STS_ADDR_MATCH       (uint8_t)(0x01u << `$INSTANCE_NAME`_RX_STS_ADDR_MATCH_SHIFT)
    #define `$INSTANCE_NAME`_RX_STS_SOFT_BUFF_OVER   (uint8_t)(0x01u << `$INSTANCE_NAME`_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define `$INSTANCE_NAME`_RX_HW_MASK                     (0x7Fu)
#endif /* End (`$INSTANCE_NAME`_RX_ENABLED) || (`$INSTANCE_NAME`_HD_ENABLED) */

/* Control Register definitions */
#define `$INSTANCE_NAME`_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define `$INSTANCE_NAME`_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define `$INSTANCE_NAME`_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define `$INSTANCE_NAME`_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define `$INSTANCE_NAME`_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define `$INSTANCE_NAME`_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define `$INSTANCE_NAME`_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define `$INSTANCE_NAME`_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define `$INSTANCE_NAME`_CTRL_HD_SEND               (uint8_t)(0x01u << `$INSTANCE_NAME`_CTRL_HD_SEND_SHIFT)
#define `$INSTANCE_NAME`_CTRL_HD_SEND_BREAK         (uint8_t)(0x01u << `$INSTANCE_NAME`_CTRL_HD_SEND_BREAK_SHIFT)
#define `$INSTANCE_NAME`_CTRL_MARK                  (uint8_t)(0x01u << `$INSTANCE_NAME`_CTRL_MARK_SHIFT)
#define `$INSTANCE_NAME`_CTRL_PARITY_TYPE_MASK      (uint8_t)(0x03u << `$INSTANCE_NAME`_CTRL_PARITY_TYPE0_SHIFT)
#define `$INSTANCE_NAME`_CTRL_RXADDR_MODE_MASK      (uint8_t)(0x07u << `$INSTANCE_NAME`_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define `$INSTANCE_NAME`_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define `$INSTANCE_NAME`_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define `$INSTANCE_NAME`_SEND_BREAK                         (0x00u)
#define `$INSTANCE_NAME`_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define `$INSTANCE_NAME`_REINIT                             (0x02u)
#define `$INSTANCE_NAME`_SEND_WAIT_REINIT                   (0x03u)

#define `$INSTANCE_NAME`_OVER_SAMPLE_8                      (8u)
#define `$INSTANCE_NAME`_OVER_SAMPLE_16                     (16u)

#define `$INSTANCE_NAME`_BIT_CENTER                         (`$INSTANCE_NAME`_OVER_SAMPLE_COUNT - 2u)

#define `$INSTANCE_NAME`_FIFO_LENGTH                        (4u)
#define `$INSTANCE_NAME`_NUMBER_OF_START_BIT                (1u)
#define `$INSTANCE_NAME`_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define `$INSTANCE_NAME`_TXBITCTR_BREAKBITS8X   ((`$INSTANCE_NAME`_BREAK_BITS_TX * `$INSTANCE_NAME`_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define `$INSTANCE_NAME`_TXBITCTR_BREAKBITS ((`$INSTANCE_NAME`_BREAK_BITS_TX * `$INSTANCE_NAME`_OVER_SAMPLE_COUNT) - 1u)

#define `$INSTANCE_NAME`_HALF_BIT_COUNT   \
                            (((`$INSTANCE_NAME`_OVER_SAMPLE_COUNT / 2u) + (`$INSTANCE_NAME`_USE23POLLING * 1u)) - 2u)
#if (`$INSTANCE_NAME`_OVER_SAMPLE_COUNT == `$INSTANCE_NAME`_OVER_SAMPLE_8)
    #define `$INSTANCE_NAME`_HD_TXBITCTR_INIT   (((`$INSTANCE_NAME`_BREAK_BITS_TX + \
                            `$INSTANCE_NAME`_NUMBER_OF_START_BIT) * `$INSTANCE_NAME`_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define `$INSTANCE_NAME`_RXBITCTR_INIT  ((((`$INSTANCE_NAME`_BREAK_BITS_RX + `$INSTANCE_NAME`_NUMBER_OF_START_BIT) \
                            * `$INSTANCE_NAME`_OVER_SAMPLE_COUNT) + `$INSTANCE_NAME`_HALF_BIT_COUNT) - 1u)

#else /* `$INSTANCE_NAME`_OVER_SAMPLE_COUNT == `$INSTANCE_NAME`_OVER_SAMPLE_16 */
    #define `$INSTANCE_NAME`_HD_TXBITCTR_INIT   ((8u * `$INSTANCE_NAME`_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define `$INSTANCE_NAME`_RXBITCTR_INIT      (((7u * `$INSTANCE_NAME`_OVER_SAMPLE_COUNT) - 1u) + \
                                                      `$INSTANCE_NAME`_HALF_BIT_COUNT)
#endif /* End `$INSTANCE_NAME`_OVER_SAMPLE_COUNT */

#define `$INSTANCE_NAME`_HD_RXBITCTR_INIT                   `$INSTANCE_NAME`_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8_t `$INSTANCE_NAME`_initVar;
#if (`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED && `$INSTANCE_NAME`_TX_ENABLED)
    extern volatile uint8_t `$INSTANCE_NAME`_txBuffer[`$INSTANCE_NAME`_TX_BUFFER_SIZE];
    extern volatile `$TxBuffRegSizeReplacementString` `$INSTANCE_NAME`_txBufferRead;
    extern `$TxBuffRegSizeReplacementString` `$INSTANCE_NAME`_txBufferWrite;
#endif /* (`$INSTANCE_NAME`_TX_INTERRUPT_ENABLED && `$INSTANCE_NAME`_TX_ENABLED) */
#if (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED && (`$INSTANCE_NAME`_RX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED))
    extern uint8_t `$INSTANCE_NAME`_errorStatus;
    extern volatile uint8_t `$INSTANCE_NAME`_rxBuffer[`$INSTANCE_NAME`_RX_BUFFER_SIZE];
    extern volatile `$RxBuffRegSizeReplacementString` `$INSTANCE_NAME`_rxBufferRead;
    extern volatile `$RxBuffRegSizeReplacementString` `$INSTANCE_NAME`_rxBufferWrite;
    extern volatile uint8_t `$INSTANCE_NAME`_rxBufferLoopDetect;
    extern volatile uint8_t `$INSTANCE_NAME`_rxBufferOverflow;
    #if (`$INSTANCE_NAME`_RXHW_ADDRESS_ENABLED)
        extern volatile uint8_t `$INSTANCE_NAME`_rxAddressMode;
        extern volatile uint8_t `$INSTANCE_NAME`_rxAddressDetected;
    #endif /* (`$INSTANCE_NAME`_RXHW_ADDRESS_ENABLED) */
#endif /* (`$INSTANCE_NAME`_RX_INTERRUPT_ENABLED && (`$INSTANCE_NAME`_RX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

`#cy_declare_enum B_UART__AddressModes`
`#cy_declare_enum B_UART__Parity_Type_revB`


/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define `$INSTANCE_NAME`_NUMBER_OF_DATA_BITS    ((`$NumDataBits`u > 8u) ? 8u : `$NumDataBits`u)
#define `$INSTANCE_NAME`_NUMBER_OF_STOP_BITS    (`$NumStopBits`u)

#if (`$INSTANCE_NAME`_RXHW_ADDRESS_ENABLED)
    #define `$INSTANCE_NAME`_RX_ADDRESS_MODE    (`$RXAddressMode`u)
    #define `$INSTANCE_NAME`_RX_HW_ADDRESS1     (`$Address1`u)
    #define `$INSTANCE_NAME`_RX_HW_ADDRESS2     (`$Address2`u)
#endif /* (`$INSTANCE_NAME`_RXHW_ADDRESS_ENABLED) */

#define `$INSTANCE_NAME`_INIT_RX_INTERRUPTS_MASK \
                                  (uint8_t)((`$InterruptOnByteRcvd` << `$INSTANCE_NAME`_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (`$InterruptOnAddDetect` << `$INSTANCE_NAME`_RX_STS_MRKSPC_SHIFT) \
                                        | (`$InterruptOnAddressMatch` << `$INSTANCE_NAME`_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (`$InterruptOnParityError` << `$INSTANCE_NAME`_RX_STS_PAR_ERROR_SHIFT) \
                                        | (`$InterruptOnStopError` << `$INSTANCE_NAME`_RX_STS_STOP_ERROR_SHIFT) \
                                        | (`$InterruptOnBreak` << `$INSTANCE_NAME`_RX_STS_BREAK_SHIFT) \
                                        | (`$InterruptOnOverrunError` << `$INSTANCE_NAME`_RX_STS_OVERRUN_SHIFT))

#define `$INSTANCE_NAME`_INIT_TX_INTERRUPTS_MASK \
                                  (uint8_t)((`$InterruptOnTXComplete` << `$INSTANCE_NAME`_TX_STS_COMPLETE_SHIFT) \
                                        | (`$InterruptOnTXFifoEmpty` << `$INSTANCE_NAME`_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (`$InterruptOnTXFifoFull` << `$INSTANCE_NAME`_TX_STS_FIFO_FULL_SHIFT) \
                                        | (`$InterruptOnTXFifoNotFull` << `$INSTANCE_NAME`_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef `$INSTANCE_NAME`_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define `$INSTANCE_NAME`_CONTROL_REG \
                            (* (reg8 *) `$INSTANCE_NAME`_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define `$INSTANCE_NAME`_CONTROL_PTR \
                            (  (reg8 *) `$INSTANCE_NAME`_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End `$INSTANCE_NAME`_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(`$INSTANCE_NAME`_TX_ENABLED)
    #define `$INSTANCE_NAME`_TXDATA_REG          (* (reg8 *) `$INSTANCE_NAME`_BUART_sTX_TxShifter_u0__F0_REG)
    #define `$INSTANCE_NAME`_TXDATA_PTR          (  (reg8 *) `$INSTANCE_NAME`_BUART_sTX_TxShifter_u0__F0_REG)
    #define `$INSTANCE_NAME`_TXDATA_AUX_CTL_REG  (* (reg8 *) `$INSTANCE_NAME`_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define `$INSTANCE_NAME`_TXDATA_AUX_CTL_PTR  (  (reg8 *) `$INSTANCE_NAME`_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define `$INSTANCE_NAME`_TXSTATUS_REG        (* (reg8 *) `$INSTANCE_NAME`_BUART_sTX_TxSts__STATUS_REG)
    #define `$INSTANCE_NAME`_TXSTATUS_PTR        (  (reg8 *) `$INSTANCE_NAME`_BUART_sTX_TxSts__STATUS_REG)
    #define `$INSTANCE_NAME`_TXSTATUS_MASK_REG   (* (reg8 *) `$INSTANCE_NAME`_BUART_sTX_TxSts__MASK_REG)
    #define `$INSTANCE_NAME`_TXSTATUS_MASK_PTR   (  (reg8 *) `$INSTANCE_NAME`_BUART_sTX_TxSts__MASK_REG)
    #define `$INSTANCE_NAME`_TXSTATUS_ACTL_REG   (* (reg8 *) `$INSTANCE_NAME`_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define `$INSTANCE_NAME`_TXSTATUS_ACTL_PTR   (  (reg8 *) `$INSTANCE_NAME`_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(`$INSTANCE_NAME`_TXCLKGEN_DP)
        #define `$INSTANCE_NAME`_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) `$INSTANCE_NAME`_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define `$INSTANCE_NAME`_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) `$INSTANCE_NAME`_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define `$INSTANCE_NAME`_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) `$INSTANCE_NAME`_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define `$INSTANCE_NAME`_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) `$INSTANCE_NAME`_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define `$INSTANCE_NAME`_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) `$INSTANCE_NAME`_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define `$INSTANCE_NAME`_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) `$INSTANCE_NAME`_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define `$INSTANCE_NAME`_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) `$INSTANCE_NAME`_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define `$INSTANCE_NAME`_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) `$INSTANCE_NAME`_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define `$INSTANCE_NAME`_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) `$INSTANCE_NAME`_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define `$INSTANCE_NAME`_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) `$INSTANCE_NAME`_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* `$INSTANCE_NAME`_TXCLKGEN_DP */

#endif /* End `$INSTANCE_NAME`_TX_ENABLED */

#if(`$INSTANCE_NAME`_HD_ENABLED)

    #define `$INSTANCE_NAME`_TXDATA_REG             (* (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxShifter_u0__F1_REG )
    #define `$INSTANCE_NAME`_TXDATA_PTR             (  (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxShifter_u0__F1_REG )
    #define `$INSTANCE_NAME`_TXDATA_AUX_CTL_REG     (* (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define `$INSTANCE_NAME`_TXDATA_AUX_CTL_PTR     (  (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define `$INSTANCE_NAME`_TXSTATUS_REG           (* (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxSts__STATUS_REG )
    #define `$INSTANCE_NAME`_TXSTATUS_PTR           (  (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxSts__STATUS_REG )
    #define `$INSTANCE_NAME`_TXSTATUS_MASK_REG      (* (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxSts__MASK_REG )
    #define `$INSTANCE_NAME`_TXSTATUS_MASK_PTR      (  (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxSts__MASK_REG )
    #define `$INSTANCE_NAME`_TXSTATUS_ACTL_REG      (* (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define `$INSTANCE_NAME`_TXSTATUS_ACTL_PTR      (  (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End `$INSTANCE_NAME`_HD_ENABLED */

#if( (`$INSTANCE_NAME`_RX_ENABLED) || (`$INSTANCE_NAME`_HD_ENABLED) )
    #define `$INSTANCE_NAME`_RXDATA_REG             (* (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxShifter_u0__F0_REG )
    #define `$INSTANCE_NAME`_RXDATA_PTR             (  (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxShifter_u0__F0_REG )
    #define `$INSTANCE_NAME`_RXADDRESS1_REG         (* (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxShifter_u0__D0_REG )
    #define `$INSTANCE_NAME`_RXADDRESS1_PTR         (  (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxShifter_u0__D0_REG )
    #define `$INSTANCE_NAME`_RXADDRESS2_REG         (* (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxShifter_u0__D1_REG )
    #define `$INSTANCE_NAME`_RXADDRESS2_PTR         (  (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxShifter_u0__D1_REG )
    #define `$INSTANCE_NAME`_RXDATA_AUX_CTL_REG     (* (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define `$INSTANCE_NAME`_RXBITCTR_PERIOD_REG    (* (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define `$INSTANCE_NAME`_RXBITCTR_PERIOD_PTR    (  (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define `$INSTANCE_NAME`_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define `$INSTANCE_NAME`_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define `$INSTANCE_NAME`_RXBITCTR_COUNTER_REG   (* (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxBitCounter__COUNT_REG )
    #define `$INSTANCE_NAME`_RXBITCTR_COUNTER_PTR   (  (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxBitCounter__COUNT_REG )

    #define `$INSTANCE_NAME`_RXSTATUS_REG           (* (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxSts__STATUS_REG )
    #define `$INSTANCE_NAME`_RXSTATUS_PTR           (  (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxSts__STATUS_REG )
    #define `$INSTANCE_NAME`_RXSTATUS_MASK_REG      (* (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxSts__MASK_REG )
    #define `$INSTANCE_NAME`_RXSTATUS_MASK_PTR      (  (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxSts__MASK_REG )
    #define `$INSTANCE_NAME`_RXSTATUS_ACTL_REG      (* (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define `$INSTANCE_NAME`_RXSTATUS_ACTL_PTR      (  (reg8 *) `$INSTANCE_NAME`_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (`$INSTANCE_NAME`_RX_ENABLED) || (`$INSTANCE_NAME`_HD_ENABLED) */

#if(`$INSTANCE_NAME`_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define `$INSTANCE_NAME`_INTCLOCK_CLKEN_REG     (* (reg8 *) `$INSTANCE_NAME`_IntClock__PM_ACT_CFG)
    #define `$INSTANCE_NAME`_INTCLOCK_CLKEN_PTR     (  (reg8 *) `$INSTANCE_NAME`_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define `$INSTANCE_NAME`_INTCLOCK_CLKEN_MASK    `$INSTANCE_NAME`_IntClock__PM_ACT_MSK
#endif /* End `$INSTANCE_NAME`_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(`$INSTANCE_NAME`_TX_ENABLED)
    #define `$INSTANCE_NAME`_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End `$INSTANCE_NAME`_TX_ENABLED */

#if(`$INSTANCE_NAME`_HD_ENABLED)
    #define `$INSTANCE_NAME`_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End `$INSTANCE_NAME`_HD_ENABLED */

#if( (`$INSTANCE_NAME`_RX_ENABLED) || (`$INSTANCE_NAME`_HD_ENABLED) )
    #define `$INSTANCE_NAME`_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (`$INSTANCE_NAME`_RX_ENABLED) || (`$INSTANCE_NAME`_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define `$INSTANCE_NAME`_WAIT_1_MS      `$INSTANCE_NAME`_BL_CHK_DELAY_MS   

#define `$INSTANCE_NAME`_TXBUFFERSIZE   `$INSTANCE_NAME`_TX_BUFFER_SIZE
#define `$INSTANCE_NAME`_RXBUFFERSIZE   `$INSTANCE_NAME`_RX_BUFFER_SIZE

#if (`$INSTANCE_NAME`_RXHW_ADDRESS_ENABLED)
    #define `$INSTANCE_NAME`_RXADDRESSMODE  `$INSTANCE_NAME`_RX_ADDRESS_MODE
    #define `$INSTANCE_NAME`_RXHWADDRESS1   `$INSTANCE_NAME`_RX_HW_ADDRESS1
    #define `$INSTANCE_NAME`_RXHWADDRESS2   `$INSTANCE_NAME`_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define `$INSTANCE_NAME`_RXAddressMode  `$INSTANCE_NAME`_RXADDRESSMODE
#endif /* (`$INSTANCE_NAME`_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define `$INSTANCE_NAME`_initvar                    `$INSTANCE_NAME`_initVar

#define `$INSTANCE_NAME`_RX_Enabled                 `$INSTANCE_NAME`_RX_ENABLED
#define `$INSTANCE_NAME`_TX_Enabled                 `$INSTANCE_NAME`_TX_ENABLED
#define `$INSTANCE_NAME`_HD_Enabled                 `$INSTANCE_NAME`_HD_ENABLED
#define `$INSTANCE_NAME`_RX_IntInterruptEnabled     `$INSTANCE_NAME`_RX_INTERRUPT_ENABLED
#define `$INSTANCE_NAME`_TX_IntInterruptEnabled     `$INSTANCE_NAME`_TX_INTERRUPT_ENABLED
#define `$INSTANCE_NAME`_InternalClockUsed          `$INSTANCE_NAME`_INTERNAL_CLOCK_USED
#define `$INSTANCE_NAME`_RXHW_Address_Enabled       `$INSTANCE_NAME`_RXHW_ADDRESS_ENABLED
#define `$INSTANCE_NAME`_OverSampleCount            `$INSTANCE_NAME`_OVER_SAMPLE_COUNT
#define `$INSTANCE_NAME`_ParityType                 `$INSTANCE_NAME`_PARITY_TYPE

#if( `$INSTANCE_NAME`_TX_ENABLED && (`$INSTANCE_NAME`_TXBUFFERSIZE > `$INSTANCE_NAME`_FIFO_LENGTH))
    #define `$INSTANCE_NAME`_TXBUFFER               `$INSTANCE_NAME`_txBuffer
    #define `$INSTANCE_NAME`_TXBUFFERREAD           `$INSTANCE_NAME`_txBufferRead
    #define `$INSTANCE_NAME`_TXBUFFERWRITE          `$INSTANCE_NAME`_txBufferWrite
#endif /* End `$INSTANCE_NAME`_TX_ENABLED */
#if( ( `$INSTANCE_NAME`_RX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED ) && \
     (`$INSTANCE_NAME`_RXBUFFERSIZE > `$INSTANCE_NAME`_FIFO_LENGTH) )
    #define `$INSTANCE_NAME`_RXBUFFER               `$INSTANCE_NAME`_rxBuffer
    #define `$INSTANCE_NAME`_RXBUFFERREAD           `$INSTANCE_NAME`_rxBufferRead
    #define `$INSTANCE_NAME`_RXBUFFERWRITE          `$INSTANCE_NAME`_rxBufferWrite
    #define `$INSTANCE_NAME`_RXBUFFERLOOPDETECT     `$INSTANCE_NAME`_rxBufferLoopDetect
    #define `$INSTANCE_NAME`_RXBUFFER_OVERFLOW      `$INSTANCE_NAME`_rxBufferOverflow
#endif /* End `$INSTANCE_NAME`_RX_ENABLED */

#ifdef `$INSTANCE_NAME`_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define `$INSTANCE_NAME`_CONTROL                `$INSTANCE_NAME`_CONTROL_REG
#endif /* End `$INSTANCE_NAME`_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(`$INSTANCE_NAME`_TX_ENABLED)
    #define `$INSTANCE_NAME`_TXDATA                 `$INSTANCE_NAME`_TXDATA_REG
    #define `$INSTANCE_NAME`_TXSTATUS               `$INSTANCE_NAME`_TXSTATUS_REG
    #define `$INSTANCE_NAME`_TXSTATUS_MASK          `$INSTANCE_NAME`_TXSTATUS_MASK_REG
    #define `$INSTANCE_NAME`_TXSTATUS_ACTL          `$INSTANCE_NAME`_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(`$INSTANCE_NAME`_TXCLKGEN_DP)
        #define `$INSTANCE_NAME`_TXBITCLKGEN_CTR        `$INSTANCE_NAME`_TXBITCLKGEN_CTR_REG
        #define `$INSTANCE_NAME`_TXBITCLKTX_COMPLETE    `$INSTANCE_NAME`_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define `$INSTANCE_NAME`_TXBITCTR_PERIOD        `$INSTANCE_NAME`_TXBITCTR_PERIOD_REG
        #define `$INSTANCE_NAME`_TXBITCTR_CONTROL       `$INSTANCE_NAME`_TXBITCTR_CONTROL_REG
        #define `$INSTANCE_NAME`_TXBITCTR_COUNTER       `$INSTANCE_NAME`_TXBITCTR_COUNTER_REG
    #endif /* `$INSTANCE_NAME`_TXCLKGEN_DP */
#endif /* End `$INSTANCE_NAME`_TX_ENABLED */

#if(`$INSTANCE_NAME`_HD_ENABLED)
    #define `$INSTANCE_NAME`_TXDATA                 `$INSTANCE_NAME`_TXDATA_REG
    #define `$INSTANCE_NAME`_TXSTATUS               `$INSTANCE_NAME`_TXSTATUS_REG
    #define `$INSTANCE_NAME`_TXSTATUS_MASK          `$INSTANCE_NAME`_TXSTATUS_MASK_REG
    #define `$INSTANCE_NAME`_TXSTATUS_ACTL          `$INSTANCE_NAME`_TXSTATUS_ACTL_REG
#endif /* End `$INSTANCE_NAME`_HD_ENABLED */

#if( (`$INSTANCE_NAME`_RX_ENABLED) || (`$INSTANCE_NAME`_HD_ENABLED) )
    #define `$INSTANCE_NAME`_RXDATA                 `$INSTANCE_NAME`_RXDATA_REG
    #define `$INSTANCE_NAME`_RXADDRESS1             `$INSTANCE_NAME`_RXADDRESS1_REG
    #define `$INSTANCE_NAME`_RXADDRESS2             `$INSTANCE_NAME`_RXADDRESS2_REG
    #define `$INSTANCE_NAME`_RXBITCTR_PERIOD        `$INSTANCE_NAME`_RXBITCTR_PERIOD_REG
    #define `$INSTANCE_NAME`_RXBITCTR_CONTROL       `$INSTANCE_NAME`_RXBITCTR_CONTROL_REG
    #define `$INSTANCE_NAME`_RXBITCTR_COUNTER       `$INSTANCE_NAME`_RXBITCTR_COUNTER_REG
    #define `$INSTANCE_NAME`_RXSTATUS               `$INSTANCE_NAME`_RXSTATUS_REG
    #define `$INSTANCE_NAME`_RXSTATUS_MASK          `$INSTANCE_NAME`_RXSTATUS_MASK_REG
    #define `$INSTANCE_NAME`_RXSTATUS_ACTL          `$INSTANCE_NAME`_RXSTATUS_ACTL_REG
#endif /* End  (`$INSTANCE_NAME`_RX_ENABLED) || (`$INSTANCE_NAME`_HD_ENABLED) */

#if(`$INSTANCE_NAME`_INTERNAL_CLOCK_USED)
    #define `$INSTANCE_NAME`_INTCLOCK_CLKEN         `$INSTANCE_NAME`_INTCLOCK_CLKEN_REG
#endif /* End `$INSTANCE_NAME`_INTERNAL_CLOCK_USED */

#define `$INSTANCE_NAME`_WAIT_FOR_COMLETE_REINIT    `$INSTANCE_NAME`_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_`$INSTANCE_NAME`_H */


/* [] END OF FILE */
