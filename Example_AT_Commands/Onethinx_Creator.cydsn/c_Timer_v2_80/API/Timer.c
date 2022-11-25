/*******************************************************************************
* File Name: `$INSTANCE_NAME`.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  The Timer component consists of a 8, 16, 24 or 32-bit timer with
*  a selectable period between 2 and 2^Width - 1.  The timer may free run
*  or be used as a capture timer as well.  The capture can be initiated
*  by a positive or negative edge signal as well as via software.
*  A trigger input can be programmed to enable the timer on rising edge
*  falling edge, either edge or continous run.
*  Interrupts may be generated due to a terminal count condition
*  or a capture event.
*
* Note:
*
********************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "`$INSTANCE_NAME`.h"

uint8 `$INSTANCE_NAME`_initVar = 0u;


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Init
********************************************************************************
*
* Summary:
*  Initialize to the schematic state
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void `$INSTANCE_NAME`_Init(void) `=ReentrantKeil($INSTANCE_NAME . "_Init")`
{
    #if(!`$INSTANCE_NAME`_UsingFixedFunction)
            /* Interrupt State Backup for Critical Region*/
            uint8 `$INSTANCE_NAME`_interruptState;
    #endif /* Interrupt state back up for Fixed Function only */

    #if (`$INSTANCE_NAME`_UsingFixedFunction)
        /* Clear all bits but the enable bit (if it's already set) for Timer operation */
        `$INSTANCE_NAME`_CONTROL &= `$INSTANCE_NAME`_CTRL_ENABLE;

        /* Clear the mode bits for continuous run mode */
        #if (CY_PSOC5A)
            `$INSTANCE_NAME`_CONTROL2 &= ((uint8)(~`$INSTANCE_NAME`_CTRL_MODE_MASK));
        #endif /* Clear bits in CONTROL2 only in PSOC5A */

        #if (CY_PSOC3 || CY_PSOC5LP)
            `$INSTANCE_NAME`_CONTROL3 &= ((uint8)(~`$INSTANCE_NAME`_CTRL_MODE_MASK));
        #endif /* CONTROL3 register exists only in PSoC3 OR PSoC5LP */

        /* Check if One Shot mode is enabled i.e. RunMode !=0*/
        #if (`$INSTANCE_NAME`_RunModeUsed != 0x0u)
            /* Set 3rd bit of Control register to enable one shot mode */
            `$INSTANCE_NAME`_CONTROL |= 0x04u;
        #endif /* One Shot enabled only when RunModeUsed is not Continuous*/

        #if (`$INSTANCE_NAME`_RunModeUsed == 2)
            #if (CY_PSOC5A)
                /* Set last 2 bits of control2 register if one shot(halt on
                interrupt) is enabled*/
                `$INSTANCE_NAME`_CONTROL2 |= 0x03u;
            #endif /* Set One-Shot Halt on Interrupt bit in CONTROL2 for PSoC5A */

            #if (CY_PSOC3 || CY_PSOC5LP)
                /* Set last 2 bits of control3 register if one shot(halt on
                interrupt) is enabled*/
                `$INSTANCE_NAME`_CONTROL3 |= 0x03u;
            #endif /* Set One-Shot Halt on Interrupt bit in CONTROL3 for PSoC3 or PSoC5LP */

        #endif /* Remove section if One Shot Halt on Interrupt is not enabled */

        #if (`$INSTANCE_NAME`_UsingHWEnable != 0)
            #if (CY_PSOC5A)
                /* Set the default Run Mode of the Timer to Continuous */
                `$INSTANCE_NAME`_CONTROL2 |= `$INSTANCE_NAME`_CTRL_MODE_PULSEWIDTH;
            #endif /* Set Continuous Run Mode in CONTROL2 for PSoC5A */

            #if (CY_PSOC3 || CY_PSOC5LP)
                /* Clear and Set ROD and COD bits of CFG2 register */
                `$INSTANCE_NAME`_CONTROL3 &= ((uint8)(~`$INSTANCE_NAME`_CTRL_RCOD_MASK));
                `$INSTANCE_NAME`_CONTROL3 |= `$INSTANCE_NAME`_CTRL_RCOD;

                /* Clear and Enable the HW enable bit in CFG2 register */
                `$INSTANCE_NAME`_CONTROL3 &= ((uint8)(~`$INSTANCE_NAME`_CTRL_ENBL_MASK));
                `$INSTANCE_NAME`_CONTROL3 |= `$INSTANCE_NAME`_CTRL_ENBL;

                /* Set the default Run Mode of the Timer to Continuous */
                `$INSTANCE_NAME`_CONTROL3 |= `$INSTANCE_NAME`_CTRL_MODE_CONTINUOUS;
            #endif /* Set Continuous Run Mode in CONTROL3 for PSoC3ES3 or PSoC5A */

        #endif /* Configure Run Mode with hardware enable */

        /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        `$INSTANCE_NAME`_RT1 &= ((uint8)(~`$INSTANCE_NAME`_RT1_MASK));
        `$INSTANCE_NAME`_RT1 |= `$INSTANCE_NAME`_SYNC;

        /*Enable DSI Sync all all inputs of the Timer*/
        `$INSTANCE_NAME`_RT1 &= ((uint8)(~`$INSTANCE_NAME`_SYNCDSI_MASK));
        `$INSTANCE_NAME`_RT1 |= `$INSTANCE_NAME`_SYNCDSI_EN;

        /* Set the IRQ to use the status register interrupts */
        `$INSTANCE_NAME`_CONTROL2 |= `$INSTANCE_NAME`_CTRL2_IRQ_SEL;
    #endif /* Configuring registers of fixed function implementation */

    /* Set Initial values from Configuration */
    `$INSTANCE_NAME`_WritePeriod(`$INSTANCE_NAME`_INIT_PERIOD);
    `$INSTANCE_NAME`_WriteCounter(`$INSTANCE_NAME`_INIT_PERIOD);

    #if (`$INSTANCE_NAME`_UsingHWCaptureCounter)/* Capture counter is enabled */
        `$INSTANCE_NAME`_CAPTURE_COUNT_CTRL |= `$INSTANCE_NAME`_CNTR_ENABLE;
        `$INSTANCE_NAME`_SetCaptureCount(`$INSTANCE_NAME`_INIT_CAPTURE_COUNT);
    #endif /* Configure capture counter value */

    #if (!`$INSTANCE_NAME`_UsingFixedFunction)
        #if (`$INSTANCE_NAME`_SoftwareCaptureMode)
            `$INSTANCE_NAME`_SetCaptureMode(`$INSTANCE_NAME`_INIT_CAPTURE_MODE);
        #endif /* Set Capture Mode for UDB implementation if capture mode is software controlled */

        #if (`$INSTANCE_NAME`_SoftwareTriggerMode)
            #if (!`$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED)
                if (0u == (`$INSTANCE_NAME`_CONTROL & `$INSTANCE_NAME`__B_TIMER__TM_SOFTWARE))
                {
                    `$INSTANCE_NAME`_SetTriggerMode(`$INSTANCE_NAME`_INIT_TRIGGER_MODE);
                }
            #endif /* (!`$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED) */
        #endif /* Set trigger mode for UDB Implementation if trigger mode is software controlled */

        /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
        /* Enter Critical Region*/
        `$INSTANCE_NAME`_interruptState = CyEnterCriticalSection();

        /* Use the interrupt output of the status register for IRQ output */
        `$INSTANCE_NAME`_STATUS_AUX_CTRL |= `$INSTANCE_NAME`_STATUS_ACTL_INT_EN_MASK;

        /* Exit Critical Region*/
        CyExitCriticalSection(`$INSTANCE_NAME`_interruptState);

        #if (`$INSTANCE_NAME`_EnableTriggerMode)
            `$INSTANCE_NAME`_EnableTrigger();
        #endif /* Set Trigger enable bit for UDB implementation in the control register*/
		
		
        #if (`$INSTANCE_NAME`_InterruptOnCaptureCount && !`$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED)
            `$INSTANCE_NAME`_SetInterruptCount(`$INSTANCE_NAME`_INIT_INT_CAPTURE_COUNT);
        #endif /* Set interrupt count in UDB implementation if interrupt count feature is checked.*/

        `$INSTANCE_NAME`_ClearFIFO();
    #endif /* Configure additional features of UDB implementation */

    `$INSTANCE_NAME`_SetInterruptMode(`$INSTANCE_NAME`_INIT_INTERRUPT_MODE);
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Enable
********************************************************************************
*
* Summary:
*  Enable the Timer
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void `$INSTANCE_NAME`_Enable(void) `=ReentrantKeil($INSTANCE_NAME . "_Enable")`
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (`$INSTANCE_NAME`_UsingFixedFunction)
        `$INSTANCE_NAME`_GLOBAL_ENABLE |= `$INSTANCE_NAME`_BLOCK_EN_MASK;
        `$INSTANCE_NAME`_GLOBAL_STBY_ENABLE |= `$INSTANCE_NAME`_BLOCK_STBY_EN_MASK;
    #endif /* Set Enable bit for enabling Fixed function timer*/

    /* Remove assignment if control register is removed */
    #if (!`$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED || `$INSTANCE_NAME`_UsingFixedFunction)
        `$INSTANCE_NAME`_CONTROL |= `$INSTANCE_NAME`_CTRL_ENABLE;
    #endif /* Remove assignment if control register is removed */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Start
********************************************************************************
*
* Summary:
*  The start function initializes the timer with the default values, the
*  enables the timerto begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  `$INSTANCE_NAME`_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void `$INSTANCE_NAME`_Start(void) `=ReentrantKeil($INSTANCE_NAME . "_Start")`
{
    if(`$INSTANCE_NAME`_initVar == 0u)
    {
        `$INSTANCE_NAME`_Init();

        `$INSTANCE_NAME`_initVar = 1u;   /* Clear this bit for Initialization */
    }

    /* Enable the Timer */
    `$INSTANCE_NAME`_Enable();
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the timer, but does not change any modes or disable
*  interrupts.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Side Effects: If the Enable mode is set to Hardware only then this function
*               has no effect on the operation of the timer.
*
*******************************************************************************/
void `$INSTANCE_NAME`_Stop(void) `=ReentrantKeil($INSTANCE_NAME . "_Stop")`
{
    /* Disable Timer */
    #if(!`$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED || `$INSTANCE_NAME`_UsingFixedFunction)
        `$INSTANCE_NAME`_CONTROL &= ((uint8)(~`$INSTANCE_NAME`_CTRL_ENABLE));
    #endif /* Remove assignment if control register is removed */

    /* Globally disable the Fixed Function Block chosen */
    #if (`$INSTANCE_NAME`_UsingFixedFunction)
        `$INSTANCE_NAME`_GLOBAL_ENABLE &= ((uint8)(~`$INSTANCE_NAME`_BLOCK_EN_MASK));
        `$INSTANCE_NAME`_GLOBAL_STBY_ENABLE &= ((uint8)(~`$INSTANCE_NAME`_BLOCK_STBY_EN_MASK));
    #endif /* Disable global enable for the Timer Fixed function block to stop the Timer*/
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SetInterruptMode
********************************************************************************
*
* Summary:
*  This function selects which of the interrupt inputs may cause an interrupt.
*  The twosources are caputure and terminal.  One, both or neither may
*  be selected.
*
* Parameters:
*  interruptMode:   This parameter is used to enable interrups on either/or
*                   terminal count or capture.
*
* Return:
*  void
*
*******************************************************************************/
void `$INSTANCE_NAME`_SetInterruptMode(uint8 interruptMode) `=ReentrantKeil($INSTANCE_NAME . "_SetInterruptMode")`
{
    `$INSTANCE_NAME`_STATUS_MASK = interruptMode;
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SoftwareCapture
********************************************************************************
*
* Summary:
*  This function forces a capture independent of the capture signal.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Side Effects:
*  An existing hardware capture could be overwritten.
*
*******************************************************************************/
void `$INSTANCE_NAME`_SoftwareCapture(void) `=ReentrantKeil($INSTANCE_NAME . "_SoftwareCapture")`
{
    /* Generate a software capture by reading the counter register */
    #if(`$INSTANCE_NAME`_UsingFixedFunction)
        (void)CY_GET_REG16(`$INSTANCE_NAME`_COUNTER_LSB_PTR);
    #else
        (void)CY_GET_REG8(`$INSTANCE_NAME`_COUNTER_LSB_PTR_8BIT);
    #endif/* (`$INSTANCE_NAME`_UsingFixedFunction) */
    /* Capture Data is now in the FIFO */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_ReadStatusRegister
********************************************************************************
*
* Summary:
*  Reads the status register and returns it's state. This function should use
*  defined types for the bit-field information as the bits in this register may
*  be permuteable.
*
* Parameters:
*  void
*
* Return:
*  The contents of the status register
*
* Side Effects:
*  Status register bits may be clear on read.
*
*******************************************************************************/
uint8   `$INSTANCE_NAME`_ReadStatusRegister(void) `=ReentrantKeil($INSTANCE_NAME . "_ReadStatusRegister")`
{
    return (`$INSTANCE_NAME`_STATUS);
}


#if (!`$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED) /* Remove API if control register is unused */


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_ReadControlRegister
********************************************************************************
*
* Summary:
*  Reads the control register and returns it's value.
*
* Parameters:
*  void
*
* Return:
*  The contents of the control register
*
*******************************************************************************/
uint8 `$INSTANCE_NAME`_ReadControlRegister(void) `=ReentrantKeil($INSTANCE_NAME . "_ReadControlRegister")`
{
    #if (!`$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED) 
        return ((uint8)`$INSTANCE_NAME`_CONTROL);
    #else
        return (0);
    #endif /* (!`$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED) */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_WriteControlRegister
********************************************************************************
*
* Summary:
*  Sets the bit-field of the control register.
*
* Parameters:
*  control: The contents of the control register
*
* Return:
*
*******************************************************************************/
void `$INSTANCE_NAME`_WriteControlRegister(uint8 control) `=ReentrantKeil($INSTANCE_NAME . "_WriteControlRegister")`
{
    #if (!`$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED) 
        `$INSTANCE_NAME`_CONTROL = control;
    #else
        control = 0u;
    #endif /* (!`$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED) */
}

#endif /* Remove API if control register is unused */


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_ReadPeriod
********************************************************************************
*
* Summary:
*  This function returns the current value of the Period.
*
* Parameters:
*  void
*
* Return:
*  The present value of the counter.
*
*******************************************************************************/
`$RegSizeReplacementString` `$INSTANCE_NAME`_ReadPeriod(void) `=ReentrantKeil($INSTANCE_NAME . "_ReadPeriod")`
{
   #if(`$INSTANCE_NAME`_UsingFixedFunction)
       return ((`$RegSizeReplacementString`)CY_GET_REG16(`$INSTANCE_NAME`_PERIOD_LSB_PTR));
   #else
       return (`$CyGetRegReplacementString`(`$INSTANCE_NAME`_PERIOD_LSB_PTR));
   #endif /* (`$INSTANCE_NAME`_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_WritePeriod
********************************************************************************
*
* Summary:
*  This function is used to change the period of the counter.  The new period
*  will be loaded the next time terminal count is detected.
*
* Parameters:
*  period: This value may be between 1 and (2^Resolution)-1.  A value of 0 will
*          result in the counter remaining at zero.
*
* Return:
*  void
*
*******************************************************************************/
void `$INSTANCE_NAME`_WritePeriod(`$RegSizeReplacementString` period) `=ReentrantKeil($INSTANCE_NAME . "_WritePeriod")`
{
    #if(`$INSTANCE_NAME`_UsingFixedFunction)
        uint16 period_temp = (uint16)period;
        CY_SET_REG16(`$INSTANCE_NAME`_PERIOD_LSB_PTR, period_temp);
    #else
        `$CySetRegReplacementString`(`$INSTANCE_NAME`_PERIOD_LSB_PTR, period);
    #endif /*Write Period value with appropriate resolution suffix depending on UDB or fixed function implementation */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_ReadCapture
********************************************************************************
*
* Summary:
*  This function returns the last value captured.
*
* Parameters:
*  void
*
* Return:
*  Present Capture value.
*
*******************************************************************************/
`$RegSizeReplacementString` `$INSTANCE_NAME`_ReadCapture(void) `=ReentrantKeil($INSTANCE_NAME . "_ReadCapture")`
{
   #if(`$INSTANCE_NAME`_UsingFixedFunction)
       return ((`$RegSizeReplacementString`)CY_GET_REG16(`$INSTANCE_NAME`_CAPTURE_LSB_PTR));
   #else
       return (`$CyGetRegReplacementString`(`$INSTANCE_NAME`_CAPTURE_LSB_PTR));
   #endif /* (`$INSTANCE_NAME`_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_WriteCounter
********************************************************************************
*
* Summary:
*  This funtion is used to set the counter to a specific value
*
* Parameters:
*  counter:  New counter value.
*
* Return:
*  void
*
*******************************************************************************/
void `$INSTANCE_NAME`_WriteCounter(`$RegSizeReplacementString` counter) `=ReentrantKeil($INSTANCE_NAME . "_WriteCounter")`
{
   #if(`$INSTANCE_NAME`_UsingFixedFunction)
        /* This functionality is removed until a FixedFunction HW update to
         * allow this register to be written
         */
        CY_SET_REG16(`$INSTANCE_NAME`_COUNTER_LSB_PTR, (uint16)counter);
        
    #else
        `$CySetRegReplacementString`(`$INSTANCE_NAME`_COUNTER_LSB_PTR, counter);
    #endif /* Set Write Counter only for the UDB implementation (Write Counter not available in fixed function Timer */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_ReadCounter
********************************************************************************
*
* Summary:
*  This function returns the current counter value.
*
* Parameters:
*  void
*
* Return:
*  Present compare value.
*
*******************************************************************************/
`$RegSizeReplacementString` `$INSTANCE_NAME`_ReadCounter(void) `=ReentrantKeil($INSTANCE_NAME . "_ReadCounter")`
{
    /* Force capture by reading Accumulator */
    /* Must first do a software capture to be able to read the counter */
    /* It is up to the user code to make sure there isn't already captured data in the FIFO */
    #if(`$INSTANCE_NAME`_UsingFixedFunction)
        (void)CY_GET_REG16(`$INSTANCE_NAME`_COUNTER_LSB_PTR);
    #else
        (void)CY_GET_REG8(`$INSTANCE_NAME`_COUNTER_LSB_PTR_8BIT);
    #endif/* (`$INSTANCE_NAME`_UsingFixedFunction) */

    /* Read the data from the FIFO (or capture register for Fixed Function)*/
    #if(`$INSTANCE_NAME`_UsingFixedFunction)
        return ((`$RegSizeReplacementString`)CY_GET_REG16(`$INSTANCE_NAME`_CAPTURE_LSB_PTR));
    #else
        return (`$CyGetRegReplacementString`(`$INSTANCE_NAME`_CAPTURE_LSB_PTR));
    #endif /* (`$INSTANCE_NAME`_UsingFixedFunction) */
}


#if(!`$INSTANCE_NAME`_UsingFixedFunction) /* UDB Specific Functions */

    
/*******************************************************************************
 * The functions below this point are only available using the UDB
 * implementation.  If a feature is selected, then the API is enabled.
 ******************************************************************************/


#if (`$INSTANCE_NAME`_SoftwareCaptureMode)


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SetCaptureMode
********************************************************************************
*
* Summary:
*  This function sets the capture mode to either rising or falling edge.
*
* Parameters:
*  captureMode: This parameter sets the capture mode of the UDB capture feature
*  The parameter values are defined using the
*  `#cy_declare_enum B_Timer__CaptureModes` identifiers
*  The following are the possible values of the parameter
*  `$INSTANCE_NAME`__B_TIMER__CM_NONE        - Set Capture mode to None
*  `$INSTANCE_NAME`__B_TIMER__CM_RISINGEDGE  - Rising edge of Capture input
*  `$INSTANCE_NAME`__B_TIMER__CM_FALLINGEDGE - Falling edge of Capture input
*  `$INSTANCE_NAME`__B_TIMER__CM_EITHEREDGE  - Either edge of Capture input
*
* Return:
*  void
*
*******************************************************************************/
void `$INSTANCE_NAME`_SetCaptureMode(uint8 captureMode) `=ReentrantKeil($INSTANCE_NAME . "_SetCaptureMode")`
{
    /* This must only set to two bits of the control register associated */
    captureMode = ((uint8)((uint8)captureMode << `$INSTANCE_NAME`_CTRL_CAP_MODE_SHIFT));
    captureMode &= (`$INSTANCE_NAME`_CTRL_CAP_MODE_MASK);

    #if (!`$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED)
        /* Clear the Current Setting */
        `$INSTANCE_NAME`_CONTROL &= ((uint8)(~`$INSTANCE_NAME`_CTRL_CAP_MODE_MASK));

        /* Write The New Setting */
        `$INSTANCE_NAME`_CONTROL |= captureMode;
    #endif /* (!`$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED) */
}
#endif /* Remove API if Capture Mode is not Software Controlled */


#if (`$INSTANCE_NAME`_SoftwareTriggerMode)


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SetTriggerMode
********************************************************************************
*
* Summary:
*  This function sets the trigger input mode
*
* Parameters:
*  triggerMode: Pass one of the pre-defined Trigger Modes (except Software)
    #define `$INSTANCE_NAME`__B_TIMER__TM_NONE 0x00u
    #define `$INSTANCE_NAME`__B_TIMER__TM_RISINGEDGE 0x04u
    #define `$INSTANCE_NAME`__B_TIMER__TM_FALLINGEDGE 0x08u
    #define `$INSTANCE_NAME`__B_TIMER__TM_EITHEREDGE 0x0Cu
    #define `$INSTANCE_NAME`__B_TIMER__TM_SOFTWARE 0x10u
*
* Return:
*  void
*
*******************************************************************************/
void `$INSTANCE_NAME`_SetTriggerMode(uint8 triggerMode) `=ReentrantKeil($INSTANCE_NAME . "_SetTriggerMode")`
{
    /* This must only set to two bits of the control register associated */
    triggerMode &= `$INSTANCE_NAME`_CTRL_TRIG_MODE_MASK;

    #if (!`$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED)   /* Remove assignment if control register is removed */
    
        /* Clear the Current Setting */
        `$INSTANCE_NAME`_CONTROL &= ((uint8)(~`$INSTANCE_NAME`_CTRL_TRIG_MODE_MASK));

        /* Write The New Setting */
        `$INSTANCE_NAME`_CONTROL |= (triggerMode | `$INSTANCE_NAME`__B_TIMER__TM_SOFTWARE);
    #endif /* Remove code section if control register is not used */
}
#endif /* Remove API if Trigger Mode is not Software Controlled */

#if (`$INSTANCE_NAME`_EnableTriggerMode)


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_EnableTrigger
********************************************************************************
*
* Summary:
*  Sets the control bit enabling Hardware Trigger mode
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void `$INSTANCE_NAME`_EnableTrigger(void) `=ReentrantKeil($INSTANCE_NAME . "_EnableTrigger")`
{
    #if (!`$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED)   /* Remove assignment if control register is removed */
        `$INSTANCE_NAME`_CONTROL |= `$INSTANCE_NAME`_CTRL_TRIG_EN;
    #endif /* Remove code section if control register is not used */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_DisableTrigger
********************************************************************************
*
* Summary:
*  Clears the control bit enabling Hardware Trigger mode
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void `$INSTANCE_NAME`_DisableTrigger(void) `=ReentrantKeil($INSTANCE_NAME . "_DisableTrigger")`
{
    #if (!`$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED )   /* Remove assignment if control register is removed */
        `$INSTANCE_NAME`_CONTROL &= ((uint8)(~`$INSTANCE_NAME`_CTRL_TRIG_EN));
    #endif /* Remove code section if control register is not used */
}
#endif /* Remove API is Trigger Mode is set to None */

#if(`$INSTANCE_NAME`_InterruptOnCaptureCount)


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SetInterruptCount
********************************************************************************
*
* Summary:
*  This function sets the capture count before an interrupt is triggered.
*
* Parameters:
*  interruptCount:  A value between 0 and 3 is valid.  If the value is 0, then
*                   an interrupt will occur each time a capture occurs.
*                   A value of 1 to 3 will cause the interrupt
*                   to delay by the same number of captures.
*
* Return:
*  void
*
*******************************************************************************/
void `$INSTANCE_NAME`_SetInterruptCount(uint8 interruptCount) `=ReentrantKeil($INSTANCE_NAME . "_SetInterruptCount")`
{
    /* This must only set to two bits of the control register associated */
    interruptCount &= `$INSTANCE_NAME`_CTRL_INTCNT_MASK;

    #if (!`$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED)
        /* Clear the Current Setting */
        `$INSTANCE_NAME`_CONTROL &= ((uint8)(~`$INSTANCE_NAME`_CTRL_INTCNT_MASK));
        /* Write The New Setting */
        `$INSTANCE_NAME`_CONTROL |= interruptCount;
    #endif /* (!`$INSTANCE_NAME`_UDB_CONTROL_REG_REMOVED) */
}
#endif /* `$INSTANCE_NAME`_InterruptOnCaptureCount */


#if (`$INSTANCE_NAME`_UsingHWCaptureCounter)


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SetCaptureCount
********************************************************************************
*
* Summary:
*  This function sets the capture count
*
* Parameters:
*  captureCount: A value between 2 and 127 inclusive is valid.  A value of 1
*                to 127 will cause the interrupt to delay by the same number of
*                captures.
*
* Return:
*  void
*
*******************************************************************************/
void `$INSTANCE_NAME`_SetCaptureCount(uint8 captureCount) `=ReentrantKeil($INSTANCE_NAME . "_SetCaptureCount")`
{
    `$INSTANCE_NAME`_CAP_COUNT = captureCount;
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_ReadCaptureCount
********************************************************************************
*
* Summary:
*  This function reads the capture count setting
*
* Parameters:
*  void
*
* Return:
*  Returns the Capture Count Setting
*
*******************************************************************************/
uint8 `$INSTANCE_NAME`_ReadCaptureCount(void) `=ReentrantKeil($INSTANCE_NAME . "_ReadCaptureCount")`
{
    return ((uint8)`$INSTANCE_NAME`_CAP_COUNT);
}
#endif /* `$INSTANCE_NAME`_UsingHWCaptureCounter */


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_ClearFIFO
********************************************************************************
*
* Summary:
*  This function clears all capture data from the capture FIFO
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void `$INSTANCE_NAME`_ClearFIFO(void) `=ReentrantKeil($INSTANCE_NAME . "_ClearFIFO")`
{
    while(0u != (`$INSTANCE_NAME`_ReadStatusRegister() & `$INSTANCE_NAME`_STATUS_FIFONEMP))
    {
        (void)`$INSTANCE_NAME`_ReadCapture();
    }
}

#endif /* UDB Specific Functions */


/* [] END OF FILE */
