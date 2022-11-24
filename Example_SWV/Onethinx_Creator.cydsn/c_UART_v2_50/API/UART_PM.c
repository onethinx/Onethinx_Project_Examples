/*******************************************************************************
* File Name: `$INSTANCE_NAME`_PM.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
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


/***************************************
* Local data allocation
***************************************/

static `$INSTANCE_NAME`_BACKUP_STRUCT  `$INSTANCE_NAME`_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component nonretention control register.
*  Does not save the FIFO which is a set of nonretention registers.
*  This function is called by the `$INSTANCE_NAME`_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  `$INSTANCE_NAME`_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void `$INSTANCE_NAME`_SaveConfig(void)
{
    #if(`$INSTANCE_NAME`_CONTROL_REG_REMOVED == 0u)
        `$INSTANCE_NAME`_backup.cr = `$INSTANCE_NAME`_CONTROL_REG;
    #endif /* End `$INSTANCE_NAME`_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the nonretention control register except FIFO.
*  Does not restore the FIFO which is a set of nonretention registers.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  `$INSTANCE_NAME`_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Notes:
*  If this function is called without calling `$INSTANCE_NAME`_SaveConfig() 
*  first, the data loaded may be incorrect.
*
*******************************************************************************/
void `$INSTANCE_NAME`_RestoreConfig(void)
{
    #if(`$INSTANCE_NAME`_CONTROL_REG_REMOVED == 0u)
        `$INSTANCE_NAME`_CONTROL_REG = `$INSTANCE_NAME`_backup.cr;
    #endif /* End `$INSTANCE_NAME`_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The `$INSTANCE_NAME`_Sleep() API saves the current component state. Then it
*  calls the `$INSTANCE_NAME`_Stop() function and calls 
*  `$INSTANCE_NAME`_SaveConfig() to save the hardware configuration.
*  Call the `$INSTANCE_NAME`_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  `$INSTANCE_NAME`_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void `$INSTANCE_NAME`_Sleep(void)
{
    #if(`$INSTANCE_NAME`_RX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED)
        if((`$INSTANCE_NAME`_RXSTATUS_ACTL_REG  & `$INSTANCE_NAME`_INT_ENABLE) != 0u)
        {
            `$INSTANCE_NAME`_backup.enableState = 1u;
        }
        else
        {
            `$INSTANCE_NAME`_backup.enableState = 0u;
        }
    #else
        if((`$INSTANCE_NAME`_TXSTATUS_ACTL_REG  & `$INSTANCE_NAME`_INT_ENABLE) !=0u)
        {
            `$INSTANCE_NAME`_backup.enableState = 1u;
        }
        else
        {
            `$INSTANCE_NAME`_backup.enableState = 0u;
        }
    #endif /* End `$INSTANCE_NAME`_RX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED*/

    `$INSTANCE_NAME`_Stop();
    `$INSTANCE_NAME`_SaveConfig();
}


/*******************************************************************************
* Function Name: `$INSTANCE_NAME`_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  `$INSTANCE_NAME`_Sleep() was called. The `$INSTANCE_NAME`_Wakeup() function
*  calls the `$INSTANCE_NAME`_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  `$INSTANCE_NAME`_Sleep() function was called, the `$INSTANCE_NAME`_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  `$INSTANCE_NAME`_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void `$INSTANCE_NAME`_Wakeup(void)
{
    `$INSTANCE_NAME`_RestoreConfig();
    #if( (`$INSTANCE_NAME`_RX_ENABLED) || (`$INSTANCE_NAME`_HD_ENABLED) )
        `$INSTANCE_NAME`_ClearRxBuffer();
    #endif /* End (`$INSTANCE_NAME`_RX_ENABLED) || (`$INSTANCE_NAME`_HD_ENABLED) */
    #if(`$INSTANCE_NAME`_TX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED)
        `$INSTANCE_NAME`_ClearTxBuffer();
    #endif /* End `$INSTANCE_NAME`_TX_ENABLED || `$INSTANCE_NAME`_HD_ENABLED */

    if(`$INSTANCE_NAME`_backup.enableState != 0u)
    {
        `$INSTANCE_NAME`_Enable();
    }
}


/* [] END OF FILE */
