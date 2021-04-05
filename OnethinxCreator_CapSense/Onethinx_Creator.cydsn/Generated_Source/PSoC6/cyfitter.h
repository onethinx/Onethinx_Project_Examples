/*******************************************************************************
* File Name: cyfitter.h
* 
* PSoC Creator  4.3
*
* Description:
* 
* This file is automatically generated by PSoC Creator.
*
********************************************************************************
* Copyright (c) 2007-2019 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#ifndef INCLUDED_CYFITTER_H
#define INCLUDED_CYFITTER_H
#include "cy_device_headers.h"
#include "cyfitter_gpio.h"
#include "cyfitter_sysint.h"

/* CapSense_CSD */
#define CapSense_CSD__CMOD_PAD 1u
#define CapSense_CSD__CSH_TANK_PAD 2u
#define CapSense_CSD__CSHIELD_PAD 4u
#define CapSense_CSD__DEDICATED_IO0 CapSense_CSD__CSHIELD_PAD
#define CapSense_CSD__HW CSD0
#define CapSense_CSD__IDX 0
#define CapSense_CSD__VREF_EXT_PAD 8u

/* CapSense_IDACComp */
#define CapSense_IDACComp__HW CSD0
#define CapSense_IDACComp__IDX 1

/* Miscellaneous */
#define CapSense_IDACMod__HW CSD0
#define CapSense_IDACMod__IDX 0
#define CapSense_ModClk__DIV_IDX 0
#define CapSense_ModClk__DIV_NUM 0
#define CapSense_ModClk__DIV_TYPE CY_SYSCLK_DIV_8_BIT
#define CY_PROJECT_NAME "Onethinx_Creator"
#define CY_SRSS_WCO_IN_0_DRIVEMODE CY_GPIO_DM_ANALOG
#define CY_SRSS_WCO_IN_0_INBUF_ENABLED 0u
#define CY_SRSS_WCO_IN_0_INIT_DRIVESTATE 0u
#define CY_SRSS_WCO_IN_0_INIT_MUXSEL 0u
#define CY_SRSS_WCO_IN_0_INPUT_SYNC 2u
#define CY_SRSS_WCO_IN_0_INTERRUPT_MODE CY_GPIO_INTR_DISABLE
#define CY_SRSS_WCO_IN_0_NUM 0u
#define CY_SRSS_WCO_IN_0_PORT GPIO_PRT0
#define CY_SRSS_WCO_IN_0_SLEWRATE CY_GPIO_SLEW_FAST
#define CY_SRSS_WCO_IN_0_THRESHOLD_LEVEL CY_GPIO_VTRIP_CMOS
#define CY_SRSS_WCO_IN_DRIVEMODE CY_GPIO_DM_ANALOG
#define CY_SRSS_WCO_IN_INBUF_ENABLED 0u
#define CY_SRSS_WCO_IN_INIT_DRIVESTATE 0u
#define CY_SRSS_WCO_IN_INIT_MUXSEL 0u
#define CY_SRSS_WCO_IN_INPUT_SYNC 2u
#define CY_SRSS_WCO_IN_INTERRUPT_MODE CY_GPIO_INTR_DISABLE
#define CY_SRSS_WCO_IN_NUM 0u
#define CY_SRSS_WCO_IN_PORT GPIO_PRT0
#define CY_SRSS_WCO_IN_SLEWRATE CY_GPIO_SLEW_FAST
#define CY_SRSS_WCO_IN_THRESHOLD_LEVEL CY_GPIO_VTRIP_CMOS
#define CY_SRSS_WCO_OUT_0_DRIVEMODE CY_GPIO_DM_ANALOG
#define CY_SRSS_WCO_OUT_0_INBUF_ENABLED 0u
#define CY_SRSS_WCO_OUT_0_INIT_DRIVESTATE 0u
#define CY_SRSS_WCO_OUT_0_INIT_MUXSEL 0u
#define CY_SRSS_WCO_OUT_0_INPUT_SYNC 2u
#define CY_SRSS_WCO_OUT_0_INTERRUPT_MODE CY_GPIO_INTR_DISABLE
#define CY_SRSS_WCO_OUT_0_NUM 1u
#define CY_SRSS_WCO_OUT_0_PORT GPIO_PRT0
#define CY_SRSS_WCO_OUT_0_SLEWRATE CY_GPIO_SLEW_FAST
#define CY_SRSS_WCO_OUT_0_THRESHOLD_LEVEL CY_GPIO_VTRIP_CMOS
#define CY_SRSS_WCO_OUT_DRIVEMODE CY_GPIO_DM_ANALOG
#define CY_SRSS_WCO_OUT_INBUF_ENABLED 0u
#define CY_SRSS_WCO_OUT_INIT_DRIVESTATE 0u
#define CY_SRSS_WCO_OUT_INIT_MUXSEL 0u
#define CY_SRSS_WCO_OUT_INPUT_SYNC 2u
#define CY_SRSS_WCO_OUT_INTERRUPT_MODE CY_GPIO_INTR_DISABLE
#define CY_SRSS_WCO_OUT_NUM 1u
#define CY_SRSS_WCO_OUT_PORT GPIO_PRT0
#define CY_SRSS_WCO_OUT_SLEWRATE CY_GPIO_SLEW_FAST
#define CY_SRSS_WCO_OUT_THRESHOLD_LEVEL CY_GPIO_VTRIP_CMOS
#define CY_VERSION "PSoC Creator  4.3"
#define CYDEV_CHIP_DIE_LEOPARD 1u
#define CYDEV_CHIP_DIE_PSOC4A 24u
#define CYDEV_CHIP_DIE_PSOC5LP 2u
#define CYDEV_CHIP_DIE_PSOC5TM 3u
#define CYDEV_CHIP_DIE_TMA4 4u
#define CYDEV_CHIP_DIE_UNKNOWN 0u
#define CYDEV_CHIP_FAMILY_FM0P 5u
#define CYDEV_CHIP_FAMILY_FM3 6u
#define CYDEV_CHIP_FAMILY_FM4 7u
#define CYDEV_CHIP_FAMILY_PSOC3 1u
#define CYDEV_CHIP_FAMILY_PSOC4 2u
#define CYDEV_CHIP_FAMILY_PSOC5 3u
#define CYDEV_CHIP_FAMILY_PSOC6 4u
#define CYDEV_CHIP_FAMILY_UNKNOWN 0u
#define CYDEV_CHIP_FAMILY_USED CYDEV_CHIP_FAMILY_PSOC6
#define CYDEV_CHIP_JTAG_ID 0xE2072100u
#define CYDEV_CHIP_MEMBER_3A 1u
#define CYDEV_CHIP_MEMBER_4A 24u
#define CYDEV_CHIP_MEMBER_4AA 23u
#define CYDEV_CHIP_MEMBER_4AB 28u
#define CYDEV_CHIP_MEMBER_4AC 14u
#define CYDEV_CHIP_MEMBER_4D 18u
#define CYDEV_CHIP_MEMBER_4E 6u
#define CYDEV_CHIP_MEMBER_4F 25u
#define CYDEV_CHIP_MEMBER_4G 4u
#define CYDEV_CHIP_MEMBER_4H 22u
#define CYDEV_CHIP_MEMBER_4I 30u
#define CYDEV_CHIP_MEMBER_4J 19u
#define CYDEV_CHIP_MEMBER_4K 20u
#define CYDEV_CHIP_MEMBER_4L 29u
#define CYDEV_CHIP_MEMBER_4M 27u
#define CYDEV_CHIP_MEMBER_4N 11u
#define CYDEV_CHIP_MEMBER_4O 8u
#define CYDEV_CHIP_MEMBER_4P 26u
#define CYDEV_CHIP_MEMBER_4Q 15u
#define CYDEV_CHIP_MEMBER_4R 9u
#define CYDEV_CHIP_MEMBER_4S 12u
#define CYDEV_CHIP_MEMBER_4T 10u
#define CYDEV_CHIP_MEMBER_4U 5u
#define CYDEV_CHIP_MEMBER_4V 21u
#define CYDEV_CHIP_MEMBER_4W 13u
#define CYDEV_CHIP_MEMBER_4X 7u
#define CYDEV_CHIP_MEMBER_4Y 16u
#define CYDEV_CHIP_MEMBER_4Z 17u
#define CYDEV_CHIP_MEMBER_5A 3u
#define CYDEV_CHIP_MEMBER_5B 2u
#define CYDEV_CHIP_MEMBER_6A 31u
#define CYDEV_CHIP_MEMBER_FM3 35u
#define CYDEV_CHIP_MEMBER_FM4 36u
#define CYDEV_CHIP_MEMBER_PDL_FM0P_TYPE1 32u
#define CYDEV_CHIP_MEMBER_PDL_FM0P_TYPE2 33u
#define CYDEV_CHIP_MEMBER_PDL_FM0P_TYPE3 34u
#define CYDEV_CHIP_MEMBER_UNKNOWN 0u
#define CYDEV_CHIP_MEMBER_USED CYDEV_CHIP_MEMBER_6A
#define CYDEV_CHIP_DIE_EXPECT CYDEV_CHIP_MEMBER_USED
#define CYDEV_CHIP_DIE_ACTUAL CYDEV_CHIP_DIE_EXPECT
#define CYDEV_CHIP_REV_LEOPARD_ES1 0u
#define CYDEV_CHIP_REV_LEOPARD_ES2 1u
#define CYDEV_CHIP_REV_LEOPARD_ES3 3u
#define CYDEV_CHIP_REV_LEOPARD_PRODUCTION 3u
#define CYDEV_CHIP_REV_PSOC4A_ES0 17u
#define CYDEV_CHIP_REV_PSOC4A_PRODUCTION 17u
#define CYDEV_CHIP_REV_PSOC5LP_ES0 0u
#define CYDEV_CHIP_REV_PSOC5LP_PRODUCTION 0u
#define CYDEV_CHIP_REV_PSOC5TM_ES0 0u
#define CYDEV_CHIP_REV_PSOC5TM_ES1 1u
#define CYDEV_CHIP_REV_PSOC5TM_PRODUCTION 1u
#define CYDEV_CHIP_REV_TMA4_ES 17u
#define CYDEV_CHIP_REV_TMA4_ES2 33u
#define CYDEV_CHIP_REV_TMA4_PRODUCTION 17u
#define CYDEV_CHIP_REVISION_3A_ES1 0u
#define CYDEV_CHIP_REVISION_3A_ES2 1u
#define CYDEV_CHIP_REVISION_3A_ES3 3u
#define CYDEV_CHIP_REVISION_3A_PRODUCTION 3u
#define CYDEV_CHIP_REVISION_4A_ES0 17u
#define CYDEV_CHIP_REVISION_4A_PRODUCTION 17u
#define CYDEV_CHIP_REVISION_4AA_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4AB_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4AC_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4D_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4E_CCG2_NO_USBPD 0u
#define CYDEV_CHIP_REVISION_4E_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4F_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4F_PRODUCTION_256DMA 0u
#define CYDEV_CHIP_REVISION_4F_PRODUCTION_256K 0u
#define CYDEV_CHIP_REVISION_4G_ES 17u
#define CYDEV_CHIP_REVISION_4G_ES2 33u
#define CYDEV_CHIP_REVISION_4G_PRODUCTION 17u
#define CYDEV_CHIP_REVISION_4H_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4I_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4J_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4K_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4L_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4M_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4N_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4O_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4P_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4Q_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4R_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4S_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4T_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4U_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4V_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4W_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4X_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4Y_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_4Z_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_5A_ES0 0u
#define CYDEV_CHIP_REVISION_5A_ES1 1u
#define CYDEV_CHIP_REVISION_5A_PRODUCTION 1u
#define CYDEV_CHIP_REVISION_5B_ES0 0u
#define CYDEV_CHIP_REVISION_5B_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_6A_ES 17u
#define CYDEV_CHIP_REVISION_6A_NO_UDB 33u
#define CYDEV_CHIP_REVISION_6A_PRODUCTION 33u
#define CYDEV_CHIP_REVISION_FM3_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_FM4_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_PDL_FM0P_TYPE1_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_PDL_FM0P_TYPE2_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_PDL_FM0P_TYPE3_PRODUCTION 0u
#define CYDEV_CHIP_REVISION_USED CYDEV_CHIP_REVISION_6A_PRODUCTION
#define CYDEV_CHIP_REV_EXPECT CYDEV_CHIP_REVISION_USED
#define CYDEV_CLK_ALTSYSTICKCLK__HZ 32768U
#define CYDEV_CLK_ALTSYSTICKCLK__KHZ 32U
#define CYDEV_CLK_ALTSYSTICKCLK__MHZ 0U
#define CYDEV_CLK_BAKCLK__HZ 32768U
#define CYDEV_CLK_BAKCLK__KHZ 32U
#define CYDEV_CLK_BAKCLK__MHZ 0U
#define CYDEV_CLK_FASTCLK__HZ 8000000U
#define CYDEV_CLK_FASTCLK__KHZ 8000U
#define CYDEV_CLK_FASTCLK__MHZ 8U
#define CYDEV_CLK_HFCLK0__HZ 8000000U
#define CYDEV_CLK_HFCLK0__KHZ 8000U
#define CYDEV_CLK_HFCLK0__MHZ 8U
#define CYDEV_CLK_IMO__HZ 8000000U
#define CYDEV_CLK_IMO__KHZ 8000U
#define CYDEV_CLK_IMO__MHZ 8U
#define CYDEV_CLK_LFCLK__HZ 32768U
#define CYDEV_CLK_LFCLK__KHZ 32U
#define CYDEV_CLK_LFCLK__MHZ 0U
#define CYDEV_CLK_PATHMUX0__HZ 8000000U
#define CYDEV_CLK_PATHMUX0__KHZ 8000U
#define CYDEV_CLK_PATHMUX0__MHZ 8U
#define CYDEV_CLK_PATHMUX1__HZ 8000000U
#define CYDEV_CLK_PATHMUX1__KHZ 8000U
#define CYDEV_CLK_PATHMUX1__MHZ 8U
#define CYDEV_CLK_PATHMUX2__HZ 8000000U
#define CYDEV_CLK_PATHMUX2__KHZ 8000U
#define CYDEV_CLK_PATHMUX2__MHZ 8U
#define CYDEV_CLK_PATHMUX3__HZ 8000000U
#define CYDEV_CLK_PATHMUX3__KHZ 8000U
#define CYDEV_CLK_PATHMUX3__MHZ 8U
#define CYDEV_CLK_PATHMUX4__HZ 8000000U
#define CYDEV_CLK_PATHMUX4__KHZ 8000U
#define CYDEV_CLK_PATHMUX4__MHZ 8U
#define CYDEV_CLK_PERICLK__HZ 8000000U
#define CYDEV_CLK_PERICLK__KHZ 8000U
#define CYDEV_CLK_PERICLK__MHZ 8U
#define CYDEV_CLK_PUMPCLK__HZ 8000000U
#define CYDEV_CLK_PUMPCLK__KHZ 8000U
#define CYDEV_CLK_PUMPCLK__MHZ 8U
#define CYDEV_CLK_SLOWCLK__HZ 8000000U
#define CYDEV_CLK_SLOWCLK__KHZ 8000U
#define CYDEV_CLK_SLOWCLK__MHZ 8U
#define CYDEV_CLK_TIMERCLK__HZ 8000000U
#define CYDEV_CLK_TIMERCLK__KHZ 8000U
#define CYDEV_CLK_TIMERCLK__MHZ 8U
#define CYDEV_CLK_WCO__HZ 32768U
#define CYDEV_CLK_WCO__KHZ 32U
#define CYDEV_CLK_WCO__MHZ 0U
#define CYDEV_CONFIG_READ_ACCELERATOR 1
#define CYDEV_CONFIG_UNUSED_IO_AllowButWarn 0
#define CYDEV_CONFIG_UNUSED_IO CYDEV_CONFIG_UNUSED_IO_AllowButWarn
#define CYDEV_CONFIG_UNUSED_IO_AllowWithInfo 1
#define CYDEV_CONFIG_UNUSED_IO_Disallowed 2
#define CYDEV_CONFIGURATION_COMPRESSED 1
#define CYDEV_CONFIGURATION_MODE_COMPRESSED 0
#define CYDEV_CONFIGURATION_MODE CYDEV_CONFIGURATION_MODE_COMPRESSED
#define CYDEV_CONFIGURATION_MODE_DMA 2
#define CYDEV_CONFIGURATION_MODE_UNCOMPRESSED 1
#define CYDEV_CORE_VOLTAGE CY_SYSPM_LDO_VOLTAGE_1_1V
#define CYDEV_DEBUGGING_DPS_Disable 3
#define CYDEV_DEBUGGING_DPS CYDEV_DEBUGGING_DPS_Disable
#define CYDEV_DEBUGGING_DPS_JTAG_4 1
#define CYDEV_DEBUGGING_DPS_JTAG_5 0
#define CYDEV_DEBUGGING_DPS_SWD 2
#define CYDEV_DEBUGGING_DPS_SWD_SWV 6
#define CYDEV_DEBUGGING_ENABLE 0
#define CYDEV_ENABLE_SIMO_BUCK false
#define CYDEV_IS_EXPORTING_CODE 0
#define CYDEV_IS_IMPORTING_CODE 0
#define CYDEV_PMIC_ENABLED 0
#define CYDEV_PMIC_MODE CYDEV_PMIC_MODE_DISABLED
#define CYDEV_POWER_MODE CYDEV_POWER_MODE_LDO_LINEAR_REG_1_1V
#define CYDEV_PROJ_TYPE 0
#define CYDEV_PROJ_TYPE_BOOTLOADER 1
#define CYDEV_PROJ_TYPE_LAUNCHER 5
#define CYDEV_PROJ_TYPE_LOADABLE 2
#define CYDEV_PROJ_TYPE_LOADABLEANDBOOTLOADER 4
#define CYDEV_PROJ_TYPE_MULTIAPPBOOTLOADER 3
#define CYDEV_PROJ_TYPE_STANDARD 0
#define CYDEV_SYSTEM_AREF_CURRENT_HIGH 1
#define CYDEV_SYSTEM_AREF_CURRENT CYDEV_SYSTEM_AREF_CURRENT_HIGH
#define CYDEV_SYSTEM_AREF_CURRENT_LOW 0
#define CYDEV_SYSTEM_AREF_DEEPSLEEP 0
#define CYDEV_SYSTEM_AREF_SOURCE CY_SYSANALOG_VREF_SOURCE_LOCAL_1_2V
#define CYDEV_VARIABLE_VDDA 0
#define CYDEV_VBAC_SUPPLY CYDEV_VBAC_SUPPLY_VDDD
#define CYDEV_VBACKUP 3.3
#define CYDEV_VBACKUP_MV 3300
#define CYDEV_VDD_NS 3.3
#define CYDEV_VDD_NS_MV 3300
#define CYDEV_VDDA 3.3
#define CYDEV_VDDA_MV 3300
#define CYDEV_VDDD 3.3
#define CYDEV_VDDD_MV 3300
#define CYDEV_VDDIO0 3.3
#define CYDEV_VDDIO0_MV 3300
#define CYDEV_VDDIO1 3.3
#define CYDEV_VDDIO1_MV 3300
#define CYDEV_VDDR_HVL 3.3
#define CYDEV_VDDR_HVL_MV 3300
#define SRSS__HW SRSS0
#define SRSS__IDX 0
#define CYDEV_BOOTLOADER_ENABLE 0

#endif /* INCLUDED_CYFITTER_H */
