/***************************************************************************//**
* \file I2C_CapSense.c
* \version 2.0
*
*  This file provides constants and parameter values for the EZI2C component.
*
********************************************************************************
* \copyright
* Copyright 2016-2017, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(I2C_CapSense_CY_SCB_EZI2C_PDL_H)
#define I2C_CapSense_CY_SCB_EZI2C_PDL_H

#include "cyfitter.h"
#include "scb/cy_scb_ezi2c.h"

#if defined(__cplusplus)
extern "C" {
#endif

/***************************************
*   Initial Parameter Constants
****************************************/

#define I2C_CapSense_ENABLE_SECOND_ADDR  (0U)


/***************************************
*        Function Prototypes
***************************************/
/**
* \addtogroup group_general
* @{
*/
/* Component only APIs. */
void I2C_CapSense_Start(void);

/* Basic functions. */
__STATIC_INLINE cy_en_scb_ezi2c_status_t I2C_CapSense_Init(cy_stc_scb_ezi2c_config_t const *config);
__STATIC_INLINE void I2C_CapSense_DeInit (void);
__STATIC_INLINE void I2C_CapSense_Enable (void);
__STATIC_INLINE void I2C_CapSense_Disable(void);

__STATIC_INLINE uint32_t I2C_CapSense_GetActivity(void);

/* Configuration functions for address 1 */
__STATIC_INLINE void     I2C_CapSense_SetAddress1(uint8_t addr);
__STATIC_INLINE uint32_t I2C_CapSense_GetAddress1(void);
__STATIC_INLINE void     I2C_CapSense_SetBuffer1(uint8_t* buffer, uint32_t size, uint32_t rwBoundary);

#if (I2C_CapSense_ENABLE_SECOND_ADDR)
/* Configuration functions for address 2 */
__STATIC_INLINE void     I2C_CapSense_SetAddress2(uint8_t addr);
__STATIC_INLINE uint32_t I2C_CapSense_GetAddress2(void);
__STATIC_INLINE void     I2C_CapSense_SetBuffer2(uint8_t* buffer, uint32_t size, uint32_t rwBoundary);
#endif /* (I2C_CapSense_ENABLE_SECOND_ADDR) */

__STATIC_INLINE void I2C_CapSense_Interrupt(void);
/** @} group_general */


/***************************************
*    Variables with External Linkage
***************************************/
/**
* \addtogroup group_globals
* @{
*/
extern uint8_t I2C_CapSense_initVar;
extern cy_stc_scb_ezi2c_config_t const I2C_CapSense_config;
extern cy_stc_scb_ezi2c_context_t I2C_CapSense_context;
/** @} group_globals */


/***************************************
*         Preprocessor Macros
***************************************/
/**
* \addtogroup group_macros
* @{
*/
/** The pointer to the base address of the SCB instance */
#define I2C_CapSense_HW     ((CySCB_Type *) I2C_CapSense_SCB__HW)
/** @} group_macros */


/***************************************
*    In-line Function Implementation
***************************************/

/*******************************************************************************
* Function Name: I2C_CapSense_Init
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_Init() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE cy_en_scb_ezi2c_status_t I2C_CapSense_Init(cy_stc_scb_ezi2c_config_t const *config)
{
    return Cy_SCB_EZI2C_Init(I2C_CapSense_HW, config, &I2C_CapSense_context);
}


/*******************************************************************************
*  Function Name: I2C_CapSense_DeInit
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_DeInit() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_CapSense_DeInit(void)
{
    Cy_SCB_EZI2C_DeInit(I2C_CapSense_HW);
}


/*******************************************************************************
* Function Name: I2C_CapSense_Enable
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_Enable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_CapSense_Enable(void)
{
    Cy_SCB_EZI2C_Enable(I2C_CapSense_HW);
}


/*******************************************************************************
* Function Name: I2C_CapSense_Disable
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_Disable() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_CapSense_Disable(void)
{
    Cy_SCB_EZI2C_Disable(I2C_CapSense_HW, &I2C_CapSense_context);
}


/*******************************************************************************
* Function Name: I2C_CapSense_GetActivity
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_GetActivity() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2C_CapSense_GetActivity(void)
{
    return Cy_SCB_EZI2C_GetActivity(I2C_CapSense_HW, &I2C_CapSense_context);
}


/*******************************************************************************
* Function Name: I2C_CapSense_SetAddress1
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_SetAddress1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_CapSense_SetAddress1(uint8_t addr)
{
    Cy_SCB_EZI2C_SetAddress1(I2C_CapSense_HW, addr, &I2C_CapSense_context);
}


/*******************************************************************************
* Function Name: I2C_CapSense_GetAddress1
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_GetAddress1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2C_CapSense_GetAddress1(void)
{
    return Cy_SCB_EZI2C_GetAddress1(I2C_CapSense_HW, &I2C_CapSense_context);
}


/*******************************************************************************
* Function Name: I2C_CapSense_SetBuffer1
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_SetBuffer1() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_CapSense_SetBuffer1(uint8_t* buffer, uint32_t size, uint32_t rwBoundary)
{
    Cy_SCB_EZI2C_SetBuffer1(I2C_CapSense_HW, buffer, size, rwBoundary, &I2C_CapSense_context);
}


#if (I2C_CapSense_ENABLE_SECOND_ADDR)
/*******************************************************************************
* Function Name: I2C_CapSense_SetAddress2
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_SlaveGetAddress2() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_CapSense_SetAddress2(uint8_t addr)
{
    Cy_SCB_EZI2C_SetAddress2(I2C_CapSense_HW, addr, &I2C_CapSense_context);
}


/*******************************************************************************
* Function Name: I2C_CapSense_GetAddress2
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_SlaveGetAddress2() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE uint32_t I2C_CapSense_GetAddress2(void)
{
    return Cy_SCB_EZI2C_GetAddress2(I2C_CapSense_HW, &I2C_CapSense_context);
}


/*******************************************************************************
* Function Name: I2C_CapSense_SetBuffer2
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_SetBuffer2() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_CapSense_SetBuffer2(uint8_t* buffer, uint32_t size, uint32_t rwBoundary)
{
    Cy_SCB_EZI2C_SetBuffer2(I2C_CapSense_HW, buffer, size, rwBoundary, &I2C_CapSense_context);
}
#endif /* (I2C_CapSense_ENABLE_SECOND_ADDR) */


/*******************************************************************************
* Function Name: I2C_CapSense_Interrupt
****************************************************************************//**
*
* Invokes the Cy_SCB_EZI2C_Interrupt() PDL driver function.
*
*******************************************************************************/
__STATIC_INLINE void I2C_CapSense_Interrupt(void)
{
    Cy_SCB_EZI2C_Interrupt(I2C_CapSense_HW, &I2C_CapSense_context);
}

#if defined(__cplusplus)
}
#endif

#endif /* I2C_CapSense_CY_SCB_EZI2C_PDL_H */


/* [] END OF FILE */
