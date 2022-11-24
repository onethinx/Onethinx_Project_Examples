#include "BLE_EcoTrim.h"


#define CY_BLE_RF_DCXO_CFG_REG                               (0x1e08U)
#define CY_BLE_RF_DCXO_CFG_REG_DCXO_CAP_SHIFT                (1U)
#define CY_BLE_RF_DCXO_CFG_REG_VALUE                         (0x1001U)
#define CY_BLE_RF_DCXO_CFG2_REG                              (0x1e0fU)
#define CY_BLE_RF_DCXO_CFG2_REG_VALUE                        (0x6837U)
#define CY_BLE_ECO_SET_TRIM_DELAY_COEF                       (32U)
#define CY_BLE_RCB_FIFO_WR_BIT_MASK                          (0x8000UL)
#define CY_BLE_RCB_FIFO_WR_BIT_SHIFT                         (16U)

#define CY_BLE_DELAY_TIME                                    (1U)                          /* in us */
#define CY_BLE_RCB_TIMEOUT                                   (1000U / CY_BLE_DELAY_TIME)   /* 1ms   */

#define CY_BLE_RCB_RETRIES                                   (10U)
#define CY_BLE_PMU_MODE_TRANSITION_REG                       (0x1e02U)
#define CY_BLE_PMU_MODE_TRANSITION_REG_CLK_ANA_DIG_EN_BIT    (uint16_t)(1UL << 12U)
#define CY_BLE_PMU_MODE_TRANSITION_REG_RST_ACT_N_BIT         (uint16_t)(1UL << 11U)
#define CY_BLE_PMU_PMU_CTRL_REG                              (0x1e03u)
#define CY_BLE_PMU_PMU_CTRL_REG_CLK_CMOS_SEL_BIT             (uint16_t)(1UL << 10U)
#define CY_BLE_MXD_RADIO_CLK_BUF_EN_VAL                      (CY_BLE_PMU_MODE_TRANSITION_REG_CLK_ANA_DIG_EN_BIT | \
                                                               CY_BLE_PMU_MODE_TRANSITION_REG_RST_ACT_N_BIT)

#define CY_BLE_MXD_RADIO_DIG_CLK_OUT_EN_VAL                  (CY_BLE_PMU_PMU_CTRL_REG_CLK_CMOS_SEL_BIT)
#define CY_BLE_PMU_PMU_CTRL_REG                              (0x1e03u)

#define CY_BLE_RF_DCXO_BUF_CFG_REG                           (0x1e09U)
#define CY_BLE_RF_DCXO_BUF_CFG_REG_XTAL_32M_SEL_BIT          (uint16_t)(1UL << 6U)
#define CY_BLE_RF_DCXO_BUF_CFG_REG_BUF_AMP_SEL_SHIFT         (4U)
#define CY_BLE_RF_DCXO_BUF_CFG_REG_BUF_AMP_SEL_MASK          (0x03UL)
#define CY_BLE_RF_DCXO_BUF_CFG_REG_CLK_DIV_SHIFT             (0U)
#define CY_BLE_RF_DCXO_BUF_CFG_REG_CLK_DIV_MASK              (0x0fUL)
#define CY_BLE_DEFAULT_ECO_CLK_FREQ_32MHZ                    (32000000UL)
#define CY_BLE_DEFAULT_ECO_CLK_FREQ_16MHZ                    (16000000UL)

static cy_en_ble_eco_status_t BLE_HAL_RcbRegRead(uint16_t addr, uint16_t *data)
{
    cy_en_ble_eco_status_t status = CY_BLE_ECO_HARDWARE_ERROR;
    uint32_t timeout = CY_BLE_RCB_TIMEOUT;
    uint32_t temp = (uint32_t)((CY_BLE_RCB_FIFO_WR_BIT_MASK | addr) << CY_BLE_RCB_FIFO_WR_BIT_SHIFT);

    /* Write Address to the RCB */
    BLE->RCB.TX_FIFO_WR = temp;

    /* Wait for Data. */
    while(((BLE->RCB.INTR & BLE_RCB_INTR_RCB_DONE_Msk) == 0U) && (timeout > 0U))
    {
        timeout--;
        Cy_SysLib_DelayUs(CY_BLE_DELAY_TIME);
    }

    if(timeout > 0U)
    {
        BLE->RCB.INTR |= BLE_RCB_INTR_RCB_DONE_Msk;
        *data = (uint16_t)BLE->RCB.RX_FIFO_RD;
        status = CY_BLE_ECO_SUCCESS;
    }

    return(status);
}
static cy_en_ble_eco_status_t BLE_HAL_RcbRegWrite(uint16_t addr, uint16_t data)
{
    cy_en_ble_eco_status_t status = CY_BLE_ECO_HARDWARE_ERROR;
    uint32_t timeout = CY_BLE_RCB_TIMEOUT;
    uint32_t temp = ((uint32_t)((~CY_BLE_RCB_FIFO_WR_BIT_MASK & addr) << CY_BLE_RCB_FIFO_WR_BIT_SHIFT)) | data;

    BLE->RCB.TX_FIFO_WR = temp;

    /* Wait for RCB done. */
    while(((BLE->RCB.INTR & BLE_RCB_INTR_RCB_DONE_Msk) == 0U) && (timeout > 0U))
    {
        timeout--;
        Cy_SysLib_DelayUs(CY_BLE_DELAY_TIME);
    }

    if(timeout > 0U)
    {
        BLE->RCB.INTR |= BLE_RCB_INTR_RCB_DONE_Msk;
        status = CY_BLE_ECO_SUCCESS;
    }
    
    return(status);
}

/*******************************************************************************
* Function Name: BLE_HAL_EcoSetFrequency
****************************************************************************//**
*
*  Enables and configures radio clock.
*  
*  \param ecoConfig: Clock configuration of type 'cy_stc_ble_bless_eco_cfg_params_t'.
*
*  \return
*  \ref cy_en_ble_eco_status_t : Return value indicates if the function succeeded or
*  failed. The following are possible error codes:
*
*  Error Codes                 | Description
*  --------------------        | -----------
*  CY_BLE_ECO_SUCCESS          | On successful operation.
*  CY_BLE_ECO_HARDWARE_ERROR   | If RCB operation failed
*
*******************************************************************************/
cy_en_ble_eco_status_t BLE_HAL_EcoSetFrequency(cy_en_ble_bless_eco_freq_t ecoFreq, cy_en_ble_bless_sys_eco_clk_div_t ecoSysDiv)
{
    uint16_t temp;
    uint32_t retries = CY_BLE_RCB_RETRIES;
    cy_en_ble_eco_status_t status;

    /* De-assert active domain reset and enable clock buffer in MXD Radio */
    do
    {
        status = BLE_HAL_RcbRegWrite(CY_BLE_PMU_MODE_TRANSITION_REG, CY_BLE_MXD_RADIO_CLK_BUF_EN_VAL);
        if(status == CY_BLE_ECO_SUCCESS)
        {
            status = BLE_HAL_RcbRegRead(CY_BLE_PMU_MODE_TRANSITION_REG, &temp);
        }
        if(retries > 0U)
        {
            retries--;
        }
        else
        {
            status = CY_BLE_ECO_HARDWARE_ERROR;
        }
    }
    while((status == CY_BLE_ECO_SUCCESS) && (temp != CY_BLE_MXD_RADIO_CLK_BUF_EN_VAL));

    /* Check if ECO clock output is enabled
     * In MXD_VER2, ECO clock is stopped on overwriting this bit */
    if(status == CY_BLE_ECO_SUCCESS)
    {
        retries = CY_BLE_RCB_RETRIES;
        status = BLE_HAL_RcbRegRead(CY_BLE_PMU_PMU_CTRL_REG, &temp);
        if((status == CY_BLE_ECO_SUCCESS) && ((temp & CY_BLE_MXD_RADIO_DIG_CLK_OUT_EN_VAL) == 0U)) 
        {
            /* Enable digital ECO clock output from MXD Radio to BLESS */
            do
            {
                status = BLE_HAL_RcbRegWrite(CY_BLE_PMU_PMU_CTRL_REG, CY_BLE_MXD_RADIO_DIG_CLK_OUT_EN_VAL);
                if(status == CY_BLE_ECO_SUCCESS)
                {
                    status = BLE_HAL_RcbRegRead(CY_BLE_PMU_PMU_CTRL_REG, &temp);
                }
                if(retries > 0U)
                {
                    retries--;
                }
                else
                {
                    status = CY_BLE_ECO_HARDWARE_ERROR;
                }
            }
            while((status == CY_BLE_ECO_SUCCESS) && (temp != CY_BLE_MXD_RADIO_DIG_CLK_OUT_EN_VAL));
        }
    }

    /* Configure ECO clock frequency and clock divider in MXD Radio */
    if(status == CY_BLE_ECO_SUCCESS)
    {
        /* Read the MXD Radio register */
        status = BLE_HAL_RcbRegRead(CY_BLE_RF_DCXO_BUF_CFG_REG, &temp);
    }
    if(status == CY_BLE_ECO_SUCCESS)
    {
        uint16_t blerdDivider;
        uint16_t ecoSysDivider;
        uint16_t blellDivider = 0U;

        /* Clear clock divider and select amp buffer output bits */
        temp &= CY_LO16(~((CY_BLE_RF_DCXO_BUF_CFG_REG_CLK_DIV_MASK << CY_BLE_RF_DCXO_BUF_CFG_REG_CLK_DIV_SHIFT) |
                          (CY_BLE_RF_DCXO_BUF_CFG_REG_BUF_AMP_SEL_MASK << CY_BLE_RF_DCXO_BUF_CFG_REG_BUF_AMP_SEL_SHIFT)));

        /* Total ECO divider consist of divider located on BLERD and BLESS divider
         * Set BLERD divider to maximum value taking in to account that 8 MHz is required for BLELL
         * BLELL clock frequency is set to 8 MHz irrespective of the crystal value.
         */
        if(ecoFreq == CY_BLE_BLESS_ECO_FREQ_32MHZ)
        {
            if(ecoSysDiv >= CY_BLE_SYS_ECO_CLK_DIV_4)
            {
                blerdDivider = (uint16_t)CY_BLE_MXD_RADIO_CLK_DIV_4;
            }
            else
            {
                blerdDivider = (uint16_t)ecoSysDiv;
                blellDivider = (uint16_t)CY_BLE_MXD_RADIO_CLK_DIV_4 - blerdDivider;
            }
            temp |= CY_BLE_RF_DCXO_BUF_CFG_REG_XTAL_32M_SEL_BIT;
            temp |= (uint16_t)(CY_BLE_MXD_RADIO_CLK_BUF_AMP_32M_LARGE << CY_BLE_RF_DCXO_BUF_CFG_REG_BUF_AMP_SEL_SHIFT);

            /* Update cy_BleEcoClockFreqHz for the proper Cy_SysLib_Delay functionality */
            cy_BleEcoClockFreqHz = CY_BLE_DEFAULT_ECO_CLK_FREQ_32MHZ / (1UL << (uint16_t)ecoSysDiv);
        }
        else
        {
            if(ecoSysDiv >= CY_BLE_SYS_ECO_CLK_DIV_2)
            {
                blerdDivider = (uint16_t)CY_BLE_MXD_RADIO_CLK_DIV_2;
            }
            else
            {
                blerdDivider = (uint16_t)ecoSysDiv;
                blellDivider = (uint16_t)CY_BLE_MXD_RADIO_CLK_DIV_2 - blerdDivider;
            }
            temp &= (uint16_t) ~CY_BLE_RF_DCXO_BUF_CFG_REG_XTAL_32M_SEL_BIT;
            temp |= (uint16_t)(CY_BLE_MXD_RADIO_CLK_BUF_AMP_16M_LARGE << CY_BLE_RF_DCXO_BUF_CFG_REG_BUF_AMP_SEL_SHIFT);

            /* Update cy_BleEcoClockFreqHz for the proper Cy_SysLib_Delay functionality */
            cy_BleEcoClockFreqHz = CY_BLE_DEFAULT_ECO_CLK_FREQ_16MHZ / (1UL << (uint16_t)ecoSysDiv);
        }

        temp |= (uint16_t)(blerdDivider << CY_BLE_RF_DCXO_BUF_CFG_REG_CLK_DIV_SHIFT);
        
        /* Write the MXD Radio register */
        status = BLE_HAL_RcbRegWrite(CY_BLE_RF_DCXO_BUF_CFG_REG, temp);
        
        /* Reduce BLESS divider by BLERD divider value */
        ecoSysDivider = (uint16_t)ecoSysDiv - blerdDivider;
        temp = (uint16_t)(ecoSysDivider & BLE_BLESS_XTAL_CLK_DIV_CONFIG_SYSCLK_DIV_Msk);
        temp |= (uint16_t)(blellDivider << BLE_BLESS_XTAL_CLK_DIV_CONFIG_LLCLK_DIV_Pos);

        /* Set clock divider */
        BLE->BLESS.XTAL_CLK_DIV_CONFIG = temp;
    }
    
    return(status);
}

/*******************************************************************************
* Function Name: Cy_BLE_HAL_EcoSetTrim
****************************************************************************//**
*
*  Sets ECO trim value.
*
*  \param trim       : Trim value = ((load_capacitance_in_pF - 7.5) / 0.075)
*         startUpTime: Start up time delay = (startuptime_in_us / 31.25)
*
*  \return
*  \ref cy_en_ble_eco_status_t : Return value indicates if the function succeeded or
*                                failed. The following are possible error codes:
*  Error Codes                 | Description
*  --------------------        | -----------
*  CY_BLE_ECO_SUCCESS          | On successful operation.
*  CY_BLE_ECO_HARDWARE_ERROR   | If RCB operation failed
*
*******************************************************************************/
cy_en_ble_eco_status_t BLE_HAL_EcoSetTrim(uint8_t trim, uint8_t startUpTime)
{
    uint16_t reg = CY_BLE_RF_DCXO_CFG_REG_VALUE;
    cy_en_ble_eco_status_t status;
    
    /* Load the new CAP TRIM value */
    reg |= (uint16_t)((uint16_t)trim << CY_BLE_RF_DCXO_CFG_REG_DCXO_CAP_SHIFT);

    /* Write the new value to the register */
    status = BLE_HAL_RcbRegWrite(CY_BLE_RF_DCXO_CFG_REG, reg);
        
    if(status == CY_BLE_ECO_SUCCESS)
    {
        /* Write the new value to the CFG2 register */
        status = BLE_HAL_RcbRegWrite(CY_BLE_RF_DCXO_CFG2_REG, CY_BLE_RF_DCXO_CFG2_REG_VALUE);
    }
    
    Cy_SysLib_DelayUs(startUpTime * CY_BLE_ECO_SET_TRIM_DELAY_COEF);
    
    return(status);
}