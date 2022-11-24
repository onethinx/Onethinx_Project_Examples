/*******************************************************************************
* File Name: `$INSTANCE_NAME`.h
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
*  Description:
*     Contains the function prototypes and constants available to the timer
*     user module.
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_TIMER_`$INSTANCE_NAME`_H)
#define CY_TIMER_`$INSTANCE_NAME`_H

#include "cyfitter.h"
//#include "cytypes.h"
//#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */
#include "syslib/cy_syslib.h"  /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 `$INSTANCE_NAME`_initVar;


/**************************************
*           Parameter Defaults
**************************************/

#define `$INSTANCE_NAME`_Resolution                 `$Resolution`u
#define `$INSTANCE_NAME`_UsingFixedFunction         `$FixedFunctionUsed`u
#define `$INSTANCE_NAME`_UsingHWCaptureCounter      `$HWCaptureCounterEnabled`u
#define `$INSTANCE_NAME`_SoftwareCaptureMode        `$SoftwareCaptureModeEnabled`u
#define `$INSTANCE_NAME`_SoftwareTriggerMode        `$SoftwareTriggerModeEnabled`u
#define `$INSTANCE_NAME`_UsingHWEnable              `$UsesHWEnable`u
#define `$INSTANCE_NAME`_EnableTriggerMode          `$TriggerInputEnabled`u
#define `$INSTANCE_NAME`_InterruptOnCaptureCount    `$InterruptOnCapture`u
#define `$INSTANCE_NAME`_RunModeUsed                `$RunMode`u
#define `$INSTANCE_NAME`_ControlRegRemoved          `$ControlRegRemoved`u

#if defined(`$INSTANCE_NAME`_TimerUDB_sCTRLReg_`$CtlModeReplacementString`_ctrlreg__CONTROL_REG)
    #define `$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED            (0u)
#elif  (`$INSTANCE_NAME`_UsingFixedFunction)
    #define `$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED            (0u)
#else 
    #define `$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED            (1u)
#endif /* End `$INSTANCE_NAME`_TimerUDB_sCTRLReg_`$CtlModeReplacementString`_ctrlreg__CONTROL_REG */


/***************************************
*       Type defines
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for Timer Component
 *************************************************************************/
typedef struct
{
    uint8 TimerEnableState;
    #if(!`$INSTANCE_NAME`_UsingFixedFunction)

        `$RegSizeReplacementString` TimerUdb;
        uint8 InterruptMaskValue;
        #if (`$INSTANCE_NAME`_UsingHWCaptureCounter)
            uint8 TimerCaptureCounter;
        #endif /* variable declarations for backing up non retention registers in CY_UDB_V1 */

        #if (!`$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED)
            uint8 TimerControlRegister;
        #endif /* variable declaration for backing up enable state of the Timer */
    #endif /* define backup variables only for UDB implementation. Fixed function registers are all retention */

}`$INSTANCE_NAME`_backupStruct;


/***************************************
*       Function Prototypes
***************************************/

void    `$INSTANCE_NAME`_Start(void) `=ReentrantKeil($INSTANCE_NAME . "_Start")`;
void    `$INSTANCE_NAME`_Stop(void) `=ReentrantKeil($INSTANCE_NAME . "_Stop")`;

void    `$INSTANCE_NAME`_SetInterruptMode(uint8 interruptMode) `=ReentrantKeil($INSTANCE_NAME . "_SetInterruptMode")`;
uint8   `$INSTANCE_NAME`_ReadStatusRegister(void) `=ReentrantKeil($INSTANCE_NAME . "_ReadStatusRegister")`;
/* Deprecated function. Do not use this in future. Retained for backward compatibility */
#define `$INSTANCE_NAME`_GetInterruptSource() `$INSTANCE_NAME`_ReadStatusRegister()

#if(!`$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED)
    uint8   `$INSTANCE_NAME`_ReadControlRegister(void) `=ReentrantKeil($INSTANCE_NAME . "_ReadControlRegister")`;
    void    `$INSTANCE_NAME`_WriteControlRegister(uint8 control) `=ReentrantKeil($INSTANCE_NAME . "_WriteControlRegister")`;
#endif /* (!`$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED) */

`$RegSizeReplacementString`  `$INSTANCE_NAME`_ReadPeriod(void) `=ReentrantKeil($INSTANCE_NAME . "_ReadPeriod")`;
void    `$INSTANCE_NAME`_WritePeriod(`$RegSizeReplacementString` period) `=ReentrantKeil($INSTANCE_NAME . "_WritePeriod")`;
`$RegSizeReplacementString`  `$INSTANCE_NAME`_ReadCounter(void) `=ReentrantKeil($INSTANCE_NAME . "_ReadCounter")`;
void    `$INSTANCE_NAME`_WriteCounter(`$RegSizeReplacementString` counter) `=ReentrantKeil($INSTANCE_NAME . "_WriteCounter")`;
`$RegSizeReplacementString`  `$INSTANCE_NAME`_ReadCapture(void) `=ReentrantKeil($INSTANCE_NAME . "_ReadCapture")`;
void    `$INSTANCE_NAME`_SoftwareCapture(void) `=ReentrantKeil($INSTANCE_NAME . "_SoftwareCapture")`;

#if(!`$INSTANCE_NAME`_UsingFixedFunction) /* UDB Prototypes */
    #if (`$INSTANCE_NAME`_SoftwareCaptureMode)
        void    `$INSTANCE_NAME`_SetCaptureMode(uint8 captureMode) `=ReentrantKeil($INSTANCE_NAME . "_SetCaptureMode")`;
    #endif /* (!`$INSTANCE_NAME`_UsingFixedFunction) */

    #if (`$INSTANCE_NAME`_SoftwareTriggerMode)
        void    `$INSTANCE_NAME`_SetTriggerMode(uint8 triggerMode) `=ReentrantKeil($INSTANCE_NAME . "_SetTriggerMode")`;
    #endif /* (`$INSTANCE_NAME`_SoftwareTriggerMode) */

    #if (`$INSTANCE_NAME`_EnableTriggerMode)
        void    `$INSTANCE_NAME`_EnableTrigger(void) `=ReentrantKeil($INSTANCE_NAME . "_EnableTrigger")`;
        void    `$INSTANCE_NAME`_DisableTrigger(void) `=ReentrantKeil($INSTANCE_NAME . "_DisableTrigger")`;
    #endif /* (`$INSTANCE_NAME`_EnableTriggerMode) */


    #if(`$INSTANCE_NAME`_InterruptOnCaptureCount)
        void    `$INSTANCE_NAME`_SetInterruptCount(uint8 interruptCount) `=ReentrantKeil($INSTANCE_NAME . "_SetInterruptCount")`;
    #endif /* (`$INSTANCE_NAME`_InterruptOnCaptureCount) */

    #if (`$INSTANCE_NAME`_UsingHWCaptureCounter)
        void    `$INSTANCE_NAME`_SetCaptureCount(uint8 captureCount) `=ReentrantKeil($INSTANCE_NAME . "_SetCaptureCount")`;
        uint8   `$INSTANCE_NAME`_ReadCaptureCount(void) `=ReentrantKeil($INSTANCE_NAME . "_ReadCaptureCount")`;
    #endif /* (`$INSTANCE_NAME`_UsingHWCaptureCounter) */

    void `$INSTANCE_NAME`_ClearFIFO(void) `=ReentrantKeil($INSTANCE_NAME . "_ClearFIFO")`;
#endif /* UDB Prototypes */

/* Sleep Retention APIs */
void `$INSTANCE_NAME`_Init(void)          `=ReentrantKeil($INSTANCE_NAME . "_Init")`;
void `$INSTANCE_NAME`_Enable(void)        `=ReentrantKeil($INSTANCE_NAME . "_Enable")`;
void `$INSTANCE_NAME`_SaveConfig(void)    `=ReentrantKeil($INSTANCE_NAME . "_SaveConfig")`;
void `$INSTANCE_NAME`_RestoreConfig(void) `=ReentrantKeil($INSTANCE_NAME . "_RestoreConfig")`;
void `$INSTANCE_NAME`_Sleep(void)         `=ReentrantKeil($INSTANCE_NAME . "_Sleep")`;
void `$INSTANCE_NAME`_Wakeup(void)        `=ReentrantKeil($INSTANCE_NAME . "_Wakeup")`;


/***************************************
*   Enumerated Types and Parameters
***************************************/

/* Enumerated Type B_Timer__CaptureModes, Used in Capture Mode */
`#cy_declare_enum B_Timer__CaptureModes`


/* Enumerated Type B_Timer__TriggerModes, Used in Trigger Mode */
#define `$INSTANCE_NAME`__B_TIMER__TM_NONE 0x00u
#define `$INSTANCE_NAME`__B_TIMER__TM_RISINGEDGE 0x04u
#define `$INSTANCE_NAME`__B_TIMER__TM_FALLINGEDGE 0x08u
#define `$INSTANCE_NAME`__B_TIMER__TM_EITHEREDGE 0x0Cu
#define `$INSTANCE_NAME`__B_TIMER__TM_SOFTWARE 0x10u


/***************************************
*    Initialial Parameter Constants
***************************************/

#define `$INSTANCE_NAME`_INIT_PERIOD             `$Period`u
#define `$INSTANCE_NAME`_INIT_CAPTURE_MODE       ((uint8)((uint8)`$CaptureMode`u << `$INSTANCE_NAME`_CTRL_CAP_MODE_SHIFT))
#define `$INSTANCE_NAME`_INIT_TRIGGER_MODE       ((uint8)((uint8)`$TriggerMode`u << `$INSTANCE_NAME`_CTRL_TRIG_MODE_SHIFT))
#if (`$INSTANCE_NAME`_UsingFixedFunction)
    #define `$INSTANCE_NAME`_INIT_INTERRUPT_MODE (((uint8)((uint8)`$IntOnTC`u << `$INSTANCE_NAME`_STATUS_TC_INT_MASK_SHIFT)) | \
                                                  ((uint8)((uint8)`$IntOnCapture` << `$INSTANCE_NAME`_STATUS_CAPTURE_INT_MASK_SHIFT)))
#else
    #define `$INSTANCE_NAME`_INIT_INTERRUPT_MODE (((uint8)((uint8)`$IntOnTC`u << `$INSTANCE_NAME`_STATUS_TC_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)`$IntOnCapture` << `$INSTANCE_NAME`_STATUS_CAPTURE_INT_MASK_SHIFT)) | \
                                                 ((uint8)((uint8)`$IntOnFIFOFull` << `$INSTANCE_NAME`_STATUS_FIFOFULL_INT_MASK_SHIFT)))
#endif /* (`$INSTANCE_NAME`_UsingFixedFunction) */
#define `$INSTANCE_NAME`_INIT_CAPTURE_COUNT      (`$CaptureCount`u)
#define `$INSTANCE_NAME`_INIT_INT_CAPTURE_COUNT  ((uint8)((uint8)(`$NumberOfCaptures`u - 1u) << `$INSTANCE_NAME`_CTRL_INTCNT_SHIFT))


/***************************************
*           Registers
***************************************/

#if (`$INSTANCE_NAME`_UsingFixedFunction) /* Implementation Specific Registers and Register Constants */


    /***************************************
    *    Fixed Function Registers
    ***************************************/

    #define `$INSTANCE_NAME`_STATUS         (*(reg8 *) `$INSTANCE_NAME`_TimerHW__SR0 )
    /* In Fixed Function Block Status and Mask are the same register */
    #define `$INSTANCE_NAME`_STATUS_MASK    (*(reg8 *) `$INSTANCE_NAME`_TimerHW__SR0 )
    #define `$INSTANCE_NAME`_CONTROL        (*(reg8 *) `$INSTANCE_NAME`_TimerHW__CFG0)
    #define `$INSTANCE_NAME`_CONTROL2       (*(reg8 *) `$INSTANCE_NAME`_TimerHW__CFG1)
    #define `$INSTANCE_NAME`_CONTROL2_PTR   ( (reg8 *) `$INSTANCE_NAME`_TimerHW__CFG1)
    #define `$INSTANCE_NAME`_RT1            (*(reg8 *) `$INSTANCE_NAME`_TimerHW__RT1)
    #define `$INSTANCE_NAME`_RT1_PTR        ( (reg8 *) `$INSTANCE_NAME`_TimerHW__RT1)

    #if (CY_PSOC3 || CY_PSOC5LP)
        #define `$INSTANCE_NAME`_CONTROL3       (*(reg8 *) `$INSTANCE_NAME`_TimerHW__CFG2)
        #define `$INSTANCE_NAME`_CONTROL3_PTR   ( (reg8 *) `$INSTANCE_NAME`_TimerHW__CFG2)
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */
    #define `$INSTANCE_NAME`_GLOBAL_ENABLE  (*(reg8 *) `$INSTANCE_NAME`_TimerHW__PM_ACT_CFG)
    #define `$INSTANCE_NAME`_GLOBAL_STBY_ENABLE  (*(reg8 *) `$INSTANCE_NAME`_TimerHW__PM_STBY_CFG)

    #define `$INSTANCE_NAME`_CAPTURE_LSB         (* (reg16 *) `$INSTANCE_NAME`_TimerHW__CAP0 )
    #define `$INSTANCE_NAME`_CAPTURE_LSB_PTR       ((reg16 *) `$INSTANCE_NAME`_TimerHW__CAP0 )
    #define `$INSTANCE_NAME`_PERIOD_LSB          (* (reg16 *) `$INSTANCE_NAME`_TimerHW__PER0 )
    #define `$INSTANCE_NAME`_PERIOD_LSB_PTR        ((reg16 *) `$INSTANCE_NAME`_TimerHW__PER0 )
    #define `$INSTANCE_NAME`_COUNTER_LSB         (* (reg16 *) `$INSTANCE_NAME`_TimerHW__CNT_CMP0 )
    #define `$INSTANCE_NAME`_COUNTER_LSB_PTR       ((reg16 *) `$INSTANCE_NAME`_TimerHW__CNT_CMP0 )


    /***************************************
    *    Register Constants
    ***************************************/

    /* Fixed Function Block Chosen */
    #define `$INSTANCE_NAME`_BLOCK_EN_MASK                     `$INSTANCE_NAME`_TimerHW__PM_ACT_MSK
    #define `$INSTANCE_NAME`_BLOCK_STBY_EN_MASK                `$INSTANCE_NAME`_TimerHW__PM_STBY_MSK

    /* Control Register Bit Locations */
    /* Interrupt Count - Not valid for Fixed Function Block */
    #define `$INSTANCE_NAME`_CTRL_INTCNT_SHIFT                  0x00u
    /* Trigger Polarity - Not valid for Fixed Function Block */
    #define `$INSTANCE_NAME`_CTRL_TRIG_MODE_SHIFT               0x00u
    /* Trigger Enable - Not valid for Fixed Function Block */
    #define `$INSTANCE_NAME`_CTRL_TRIG_EN_SHIFT                 0x00u
    /* Capture Polarity - Not valid for Fixed Function Block */
    #define `$INSTANCE_NAME`_CTRL_CAP_MODE_SHIFT                0x00u
    /* Timer Enable - As defined in Register Map, part of TMRX_CFG0 register */
    #define `$INSTANCE_NAME`_CTRL_ENABLE_SHIFT                  0x00u

    /* Control Register Bit Masks */
    #define `$INSTANCE_NAME`_CTRL_ENABLE                        ((uint8)((uint8)0x01u << `$INSTANCE_NAME`_CTRL_ENABLE_SHIFT))

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define `$INSTANCE_NAME`_CTRL2_IRQ_SEL_SHIFT                 0x00u
    #define `$INSTANCE_NAME`_CTRL2_IRQ_SEL                      ((uint8)((uint8)0x01u << `$INSTANCE_NAME`_CTRL2_IRQ_SEL_SHIFT))

    #if (CY_PSOC5A)
        /* Use CFG1 Mode bits to set run mode */
        /* As defined by Verilog Implementation */
        #define `$INSTANCE_NAME`_CTRL_MODE_SHIFT                 0x01u
        #define `$INSTANCE_NAME`_CTRL_MODE_MASK                 ((uint8)((uint8)0x07u << `$INSTANCE_NAME`_CTRL_MODE_SHIFT))
    #endif /* (CY_PSOC5A) */
    #if (CY_PSOC3 || CY_PSOC5LP)
        /* Control3 Register Bit Locations */
        #define `$INSTANCE_NAME`_CTRL_RCOD_SHIFT        0x02u
        #define `$INSTANCE_NAME`_CTRL_ENBL_SHIFT        0x00u
        #define `$INSTANCE_NAME`_CTRL_MODE_SHIFT        0x00u

        /* Control3 Register Bit Masks */
        #define `$INSTANCE_NAME`_CTRL_RCOD_MASK  ((uint8)((uint8)0x03u << `$INSTANCE_NAME`_CTRL_RCOD_SHIFT)) /* ROD and COD bit masks */
        #define `$INSTANCE_NAME`_CTRL_ENBL_MASK  ((uint8)((uint8)0x80u << `$INSTANCE_NAME`_CTRL_ENBL_SHIFT)) /* HW_EN bit mask */
        #define `$INSTANCE_NAME`_CTRL_MODE_MASK  ((uint8)((uint8)0x03u << `$INSTANCE_NAME`_CTRL_MODE_SHIFT)) /* Run mode bit mask */

        #define `$INSTANCE_NAME`_CTRL_RCOD       ((uint8)((uint8)0x03u << `$INSTANCE_NAME`_CTRL_RCOD_SHIFT))
        #define `$INSTANCE_NAME`_CTRL_ENBL       ((uint8)((uint8)0x80u << `$INSTANCE_NAME`_CTRL_ENBL_SHIFT))
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /*RT1 Synch Constants: Applicable for PSoC3 and PSoC5LP */
    #define `$INSTANCE_NAME`_RT1_SHIFT                       0x04u
    /* Sync TC and CMP bit masks */
    #define `$INSTANCE_NAME`_RT1_MASK                        ((uint8)((uint8)0x03u << `$INSTANCE_NAME`_RT1_SHIFT))
    #define `$INSTANCE_NAME`_SYNC                            ((uint8)((uint8)0x03u << `$INSTANCE_NAME`_RT1_SHIFT))
    #define `$INSTANCE_NAME`_SYNCDSI_SHIFT                   0x00u
    /* Sync all DSI inputs with Mask  */
    #define `$INSTANCE_NAME`_SYNCDSI_MASK                    ((uint8)((uint8)0x0Fu << `$INSTANCE_NAME`_SYNCDSI_SHIFT))
    /* Sync all DSI inputs */
    #define `$INSTANCE_NAME`_SYNCDSI_EN                      ((uint8)((uint8)0x0Fu << `$INSTANCE_NAME`_SYNCDSI_SHIFT))

    #define `$INSTANCE_NAME`_CTRL_MODE_PULSEWIDTH            ((uint8)((uint8)0x01u << `$INSTANCE_NAME`_CTRL_MODE_SHIFT))
    #define `$INSTANCE_NAME`_CTRL_MODE_PERIOD                ((uint8)((uint8)0x02u << `$INSTANCE_NAME`_CTRL_MODE_SHIFT))
    #define `$INSTANCE_NAME`_CTRL_MODE_CONTINUOUS            ((uint8)((uint8)0x00u << `$INSTANCE_NAME`_CTRL_MODE_SHIFT))

    /* Status Register Bit Locations */
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define `$INSTANCE_NAME`_STATUS_TC_SHIFT                 0x07u
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define `$INSTANCE_NAME`_STATUS_CAPTURE_SHIFT            0x06u
    /* As defined in Register Map, part of TMRX_SR0 register */
    #define `$INSTANCE_NAME`_STATUS_TC_INT_MASK_SHIFT        (`$INSTANCE_NAME`_STATUS_TC_SHIFT - 0x04u)
    /* As defined in Register Map, part of TMRX_SR0 register, Shared with Compare Status */
    #define `$INSTANCE_NAME`_STATUS_CAPTURE_INT_MASK_SHIFT   (`$INSTANCE_NAME`_STATUS_CAPTURE_SHIFT - 0x04u)

    /* Status Register Bit Masks */
    #define `$INSTANCE_NAME`_STATUS_TC                       ((uint8)((uint8)0x01u << `$INSTANCE_NAME`_STATUS_TC_SHIFT))
    #define `$INSTANCE_NAME`_STATUS_CAPTURE                  ((uint8)((uint8)0x01u << `$INSTANCE_NAME`_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on TC */
    #define `$INSTANCE_NAME`_STATUS_TC_INT_MASK              ((uint8)((uint8)0x01u << `$INSTANCE_NAME`_STATUS_TC_INT_MASK_SHIFT))
    /* Interrupt Enable Bit-Mask for interrupt on Capture */
    #define `$INSTANCE_NAME`_STATUS_CAPTURE_INT_MASK         ((uint8)((uint8)0x01u << `$INSTANCE_NAME`_STATUS_CAPTURE_INT_MASK_SHIFT))

#else   /* UDB Registers and Register Constants */


    /***************************************
    *           UDB Registers
    ***************************************/

    #define `$INSTANCE_NAME`_STATUS              (* (reg8 *) `$INSTANCE_NAME`_TimerUDB_`$RstStatusReplacementString`_stsreg__STATUS_REG )
    #define `$INSTANCE_NAME`_STATUS_MASK         (* (reg8 *) `$INSTANCE_NAME`_TimerUDB_`$RstStatusReplacementString`_stsreg__MASK_REG)
    #define `$INSTANCE_NAME`_STATUS_AUX_CTRL     (* (reg8 *) `$INSTANCE_NAME`_TimerUDB_`$RstStatusReplacementString`_stsreg__STATUS_AUX_CTL_REG)
    #define `$INSTANCE_NAME`_CONTROL             (* (reg8 *) `$INSTANCE_NAME`_TimerUDB_sCTRLReg_`$CtlModeReplacementString`_ctrlreg__CONTROL_REG )
    
    #if(`$INSTANCE_NAME`_Resolution <= 8u) /* 8-bit Timer */
        #define `$INSTANCE_NAME`_CAPTURE_LSB         (* (reg8 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__F0_REG )
        #define `$INSTANCE_NAME`_CAPTURE_LSB_PTR       ((reg8 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__F0_REG )
        #define `$INSTANCE_NAME`_PERIOD_LSB          (* (reg8 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__D0_REG )
        #define `$INSTANCE_NAME`_PERIOD_LSB_PTR        ((reg8 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__D0_REG )
        #define `$INSTANCE_NAME`_COUNTER_LSB         (* (reg8 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__A0_REG )
        #define `$INSTANCE_NAME`_COUNTER_LSB_PTR       ((reg8 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__A0_REG )
    #elif(`$INSTANCE_NAME`_Resolution <= 16u) /* 8-bit Timer */
        #if(CY_PSOC3) /* 8-bit addres space */
            #define `$INSTANCE_NAME`_CAPTURE_LSB         (* (reg16 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__F0_REG )
            #define `$INSTANCE_NAME`_CAPTURE_LSB_PTR       ((reg16 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__F0_REG )
            #define `$INSTANCE_NAME`_PERIOD_LSB          (* (reg16 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__D0_REG )
            #define `$INSTANCE_NAME`_PERIOD_LSB_PTR        ((reg16 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__D0_REG )
            #define `$INSTANCE_NAME`_COUNTER_LSB         (* (reg16 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__A0_REG )
            #define `$INSTANCE_NAME`_COUNTER_LSB_PTR       ((reg16 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__A0_REG )
        #else /* 16-bit address space */
            #define `$INSTANCE_NAME`_CAPTURE_LSB         (* (reg16 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__16BIT_F0_REG )
            #define `$INSTANCE_NAME`_CAPTURE_LSB_PTR       ((reg16 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__16BIT_F0_REG )
            #define `$INSTANCE_NAME`_PERIOD_LSB          (* (reg16 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__16BIT_D0_REG )
            #define `$INSTANCE_NAME`_PERIOD_LSB_PTR        ((reg16 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__16BIT_D0_REG )
            #define `$INSTANCE_NAME`_COUNTER_LSB         (* (reg16 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__16BIT_A0_REG )
            #define `$INSTANCE_NAME`_COUNTER_LSB_PTR       ((reg16 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__16BIT_A0_REG )
        #endif /* CY_PSOC3 */
    #elif(`$INSTANCE_NAME`_Resolution <= 24u)/* 24-bit Timer */
        #define `$INSTANCE_NAME`_CAPTURE_LSB         (* (reg32 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__F0_REG )
        #define `$INSTANCE_NAME`_CAPTURE_LSB_PTR       ((reg32 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__F0_REG )
        #define `$INSTANCE_NAME`_PERIOD_LSB          (* (reg32 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__D0_REG )
        #define `$INSTANCE_NAME`_PERIOD_LSB_PTR        ((reg32 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__D0_REG )
        #define `$INSTANCE_NAME`_COUNTER_LSB         (* (reg32 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__A0_REG )
        #define `$INSTANCE_NAME`_COUNTER_LSB_PTR       ((reg32 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__A0_REG )
    #else /* 32-bit Timer */
        #if(CY_PSOC3 || CY_PSOC5) /* 8-bit address space */
            #define `$INSTANCE_NAME`_CAPTURE_LSB         (* (reg32 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__F0_REG )
            #define `$INSTANCE_NAME`_CAPTURE_LSB_PTR       ((reg32 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__F0_REG )
            #define `$INSTANCE_NAME`_PERIOD_LSB          (* (reg32 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__D0_REG )
            #define `$INSTANCE_NAME`_PERIOD_LSB_PTR        ((reg32 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__D0_REG )
            #define `$INSTANCE_NAME`_COUNTER_LSB         (* (reg32 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__A0_REG )
            #define `$INSTANCE_NAME`_COUNTER_LSB_PTR       ((reg32 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__A0_REG )
        #else /* 32-bit address space */
            #define `$INSTANCE_NAME`_CAPTURE_LSB         (* (reg32 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__32BIT_F0_REG )
            #define `$INSTANCE_NAME`_CAPTURE_LSB_PTR       ((reg32 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__32BIT_F0_REG )
            #define `$INSTANCE_NAME`_PERIOD_LSB          (* (reg32 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__32BIT_D0_REG )
            #define `$INSTANCE_NAME`_PERIOD_LSB_PTR        ((reg32 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__32BIT_D0_REG )
            #define `$INSTANCE_NAME`_COUNTER_LSB         (* (reg32 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__32BIT_A0_REG )
            #define `$INSTANCE_NAME`_COUNTER_LSB_PTR       ((reg32 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__32BIT_A0_REG )
        #endif /* CY_PSOC3 || CY_PSOC5 */ 
    #endif

    #define `$INSTANCE_NAME`_COUNTER_LSB_PTR_8BIT       ((reg8 *) `$INSTANCE_NAME`_TimerUDB_`$VerilogSectionReplacementString`_timerdp_u0__A0_REG )
    
    #if (`$INSTANCE_NAME`_UsingHWCaptureCounter)
        #define `$INSTANCE_NAME`_CAP_COUNT              (*(reg8 *) `$INSTANCE_NAME`_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define `$INSTANCE_NAME`_CAP_COUNT_PTR          ( (reg8 *) `$INSTANCE_NAME`_TimerUDB_sCapCount_counter__PERIOD_REG )
        #define `$INSTANCE_NAME`_CAPTURE_COUNT_CTRL     (*(reg8 *) `$INSTANCE_NAME`_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
        #define `$INSTANCE_NAME`_CAPTURE_COUNT_CTRL_PTR ( (reg8 *) `$INSTANCE_NAME`_TimerUDB_sCapCount_counter__CONTROL_AUX_CTL_REG )
    #endif /* (`$INSTANCE_NAME`_UsingHWCaptureCounter) */


    /***************************************
    *       Register Constants
    ***************************************/

    /* Control Register Bit Locations */
    #define `$INSTANCE_NAME`_CTRL_INTCNT_SHIFT              0x00u       /* As defined by Verilog Implementation */
    #define `$INSTANCE_NAME`_CTRL_TRIG_MODE_SHIFT           0x02u       /* As defined by Verilog Implementation */
    #define `$INSTANCE_NAME`_CTRL_TRIG_EN_SHIFT             0x04u       /* As defined by Verilog Implementation */
    #define `$INSTANCE_NAME`_CTRL_CAP_MODE_SHIFT            0x05u       /* As defined by Verilog Implementation */
    #define `$INSTANCE_NAME`_CTRL_ENABLE_SHIFT              0x07u       /* As defined by Verilog Implementation */

    /* Control Register Bit Masks */
    #define `$INSTANCE_NAME`_CTRL_INTCNT_MASK               ((uint8)((uint8)0x03u << `$INSTANCE_NAME`_CTRL_INTCNT_SHIFT))
    #define `$INSTANCE_NAME`_CTRL_TRIG_MODE_MASK            ((uint8)((uint8)0x03u << `$INSTANCE_NAME`_CTRL_TRIG_MODE_SHIFT))
    #define `$INSTANCE_NAME`_CTRL_TRIG_EN                   ((uint8)((uint8)0x01u << `$INSTANCE_NAME`_CTRL_TRIG_EN_SHIFT))
    #define `$INSTANCE_NAME`_CTRL_CAP_MODE_MASK             ((uint8)((uint8)0x03u << `$INSTANCE_NAME`_CTRL_CAP_MODE_SHIFT))
    #define `$INSTANCE_NAME`_CTRL_ENABLE                    ((uint8)((uint8)0x01u << `$INSTANCE_NAME`_CTRL_ENABLE_SHIFT))

    /* Bit Counter (7-bit) Control Register Bit Definitions */
    /* As defined by the Register map for the AUX Control Register */
    #define `$INSTANCE_NAME`_CNTR_ENABLE                    0x20u

    /* Status Register Bit Locations */
    #define `$INSTANCE_NAME`_STATUS_TC_SHIFT                0x00u  /* As defined by Verilog Implementation */
    #define `$INSTANCE_NAME`_STATUS_CAPTURE_SHIFT           0x01u  /* As defined by Verilog Implementation */
    #define `$INSTANCE_NAME`_STATUS_TC_INT_MASK_SHIFT       `$INSTANCE_NAME`_STATUS_TC_SHIFT
    #define `$INSTANCE_NAME`_STATUS_CAPTURE_INT_MASK_SHIFT  `$INSTANCE_NAME`_STATUS_CAPTURE_SHIFT
    #define `$INSTANCE_NAME`_STATUS_FIFOFULL_SHIFT          0x02u  /* As defined by Verilog Implementation */
    #define `$INSTANCE_NAME`_STATUS_FIFONEMP_SHIFT          0x03u  /* As defined by Verilog Implementation */
    #define `$INSTANCE_NAME`_STATUS_FIFOFULL_INT_MASK_SHIFT `$INSTANCE_NAME`_STATUS_FIFOFULL_SHIFT

    /* Status Register Bit Masks */
    /* Sticky TC Event Bit-Mask */
    #define `$INSTANCE_NAME`_STATUS_TC                      ((uint8)((uint8)0x01u << `$INSTANCE_NAME`_STATUS_TC_SHIFT))
    /* Sticky Capture Event Bit-Mask */
    #define `$INSTANCE_NAME`_STATUS_CAPTURE                 ((uint8)((uint8)0x01u << `$INSTANCE_NAME`_STATUS_CAPTURE_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define `$INSTANCE_NAME`_STATUS_TC_INT_MASK             ((uint8)((uint8)0x01u << `$INSTANCE_NAME`_STATUS_TC_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define `$INSTANCE_NAME`_STATUS_CAPTURE_INT_MASK        ((uint8)((uint8)0x01u << `$INSTANCE_NAME`_STATUS_CAPTURE_SHIFT))
    /* NOT-Sticky FIFO Full Bit-Mask */
    #define `$INSTANCE_NAME`_STATUS_FIFOFULL                ((uint8)((uint8)0x01u << `$INSTANCE_NAME`_STATUS_FIFOFULL_SHIFT))
    /* NOT-Sticky FIFO Not Empty Bit-Mask */
    #define `$INSTANCE_NAME`_STATUS_FIFONEMP                ((uint8)((uint8)0x01u << `$INSTANCE_NAME`_STATUS_FIFONEMP_SHIFT))
    /* Interrupt Enable Bit-Mask */
    #define `$INSTANCE_NAME`_STATUS_FIFOFULL_INT_MASK       ((uint8)((uint8)0x01u << `$INSTANCE_NAME`_STATUS_FIFOFULL_SHIFT))

    #define `$INSTANCE_NAME`_STATUS_ACTL_INT_EN             0x10u   /* As defined for the ACTL Register */

    /* Datapath Auxillary Control Register definitions */
    #define `$INSTANCE_NAME`_AUX_CTRL_FIFO0_CLR             0x01u   /* As defined by Register map */
    #define `$INSTANCE_NAME`_AUX_CTRL_FIFO1_CLR             0x02u   /* As defined by Register map */
    #define `$INSTANCE_NAME`_AUX_CTRL_FIFO0_LVL             0x04u   /* As defined by Register map */
    #define `$INSTANCE_NAME`_AUX_CTRL_FIFO1_LVL             0x08u   /* As defined by Register map */
    #define `$INSTANCE_NAME`_STATUS_ACTL_INT_EN_MASK        0x10u   /* As defined for the ACTL Register */

#endif /* Implementation Specific Registers and Register Constants */

#endif  /* CY_TIMER_`$INSTANCE_NAME`_H */


/* [] END OF FILE */
