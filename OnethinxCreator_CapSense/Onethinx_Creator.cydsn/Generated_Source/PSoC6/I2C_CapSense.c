/***************************************************************************//**
* \file I2C_CapSense.c
* \version 2.0
*
*  This file provides the source code to the API for the EZI2C Component.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "I2C_CapSense.h"
#include "sysint/cy_sysint.h"
#include "cyfitter_sysint.h"
#include "cyfitter_sysint_cfg.h"

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************
*     Global variables
***************************************/

/** I2C_CapSense_initVar indicates whether the I2C_CapSense
*  component has been initialized. The variable is initialized to 0
*  and set to 1 the first time I2C_CapSense_Start() is called.
*  This allows  the component to restart without reinitialization
*  after the first call to the I2C_CapSense_Start() routine.
*
*  If re-initialization of the component is required, then the
*  I2C_CapSense_Init() function can be called before the
*  I2C_CapSense_Start() or I2C_CapSense_Enable() function.
*/
uint8_t I2C_CapSense_initVar = 0U;

/** The instance-specific configuration structure.
* The pointer to this structure should be passed to Cy_SCB_EZI2C_Init function
* to initialize component with GUI selected settings.
*/
cy_stc_scb_ezi2c_config_t const I2C_CapSense_config =
{
    .numberOfAddresses = CY_SCB_EZI2C_ONE_ADDRESS,
    .slaveAddress1     = 0x8U,
    .slaveAddress2     = 0U,
    .subAddressSize    = CY_SCB_EZI2C_SUB_ADDR16_BITS,

    .enableWakeFromSleep = false
};

/** The instance-specific context structure.
* It is used while the driver operation for internal configuration and
* data keeping for the EZI2C. The user should not modify anything in this 
* structure.
*/
cy_stc_scb_ezi2c_context_t I2C_CapSense_context;


/*******************************************************************************
* Function Name: I2C_CapSense_Start
****************************************************************************//**
*
* Invokes I2C_CapSense_Init() and I2C_CapSense_Enable().
* Also configures interrupt and low and high oversampling phases.
* After this function call the component is enabled and ready for operation.
* This is the preferred method to begin component operation.
*
* \globalvars
* \ref I2C_CapSense_initVar - used to check initial configuration,
* modified  on first function call.
*
*******************************************************************************/
void I2C_CapSense_Start(void)
{
    if (0U == I2C_CapSense_initVar)
    {
        /* Configure component */
        (void) Cy_SCB_EZI2C_Init(I2C_CapSense_HW, &I2C_CapSense_config, &I2C_CapSense_context);

        /* Hook interrupt service routine */
        #if defined(I2C_CapSense_SCB_IRQ__INTC_ASSIGNED)
            (void) Cy_SysInt_Init(&I2C_CapSense_SCB_IRQ_cfg, &I2C_CapSense_Interrupt);
        #endif /* (I2C_CapSense_SCB_IRQ__INTC_ASSIGNED) */

        I2C_CapSense_initVar = 1U;
    }

    /* Enable interrupt in NVIC */
    #if defined(I2C_CapSense_SCB_IRQ__INTC_ASSIGNED)
        NVIC_EnableIRQ((IRQn_Type) I2C_CapSense_SCB_IRQ_cfg.intrSrc);
    #endif /* (I2C_CapSense_SCB_IRQ__INTC_ASSIGNED) */
    
    Cy_SCB_EZI2C_Enable(I2C_CapSense_HW);
}

#if defined(__cplusplus)
}
#endif


/* [] END OF FILE */
