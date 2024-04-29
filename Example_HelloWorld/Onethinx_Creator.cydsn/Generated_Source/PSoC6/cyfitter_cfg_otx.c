
/*******************************************************************************
* File Name: cyfitter_cfg.c
* 
* PSoC Creator  4.4
*
* Description:
* This file contains device initialization code.
* Except for the user defined sections in CyClockStartupError(), this file should not be modified.
* This file was automatically generated by PSoC Creator and is updated
* for the Onethinx Core module by Onethinx Creator PostBuild 2.12
*
********************************************************************************
* Copyright (c) 2007-2020 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include <string.h>
#include "cydevice_trm.h"
#include "cyfitter.h"
#include "trigmux/cy_trigmux.h"
#include "gpio/cy_gpio.h"
#include "syslib/cy_syslib.h"
#include "cyfitter_cfg.h"
#include "cyapicallbacks.h"
#include "sysclk/cy_sysclk.h"
#include "systick/cy_systick.h"

#define CY_NEED_CYCLOCKSTARTUPERROR 1
#include "syspm/cy_syspm.h"


#if defined(__GNUC__) || defined(__ARMCC_VERSION)
    #define CYPACKED 
    #define CYPACKED_ATTR __attribute__ ((packed))
    #define CYALIGNED __attribute__ ((aligned))
    #define CY_CFG_UNUSED __attribute__ ((unused))
    #ifndef CY_CFG_SECTION
        #define CY_CFG_SECTION __attribute__ ((section(".psocinit")))
    #endif
    
    #if defined(__ARMCC_VERSION)
        #define CY_CFG_MEMORY_BARRIER() __memory_changed()
    #else
        #define CY_CFG_MEMORY_BARRIER() __sync_synchronize()
    #endif
    
#elif defined(__ICCARM__)
    #include <intrinsics.h>

    #define CYPACKED __packed
    #define CYPACKED_ATTR 
    #define CYALIGNED _Pragma("data_alignment=4")
    #define CY_CFG_UNUSED _Pragma("diag_suppress=Pe177")
    #define CY_CFG_SECTION _Pragma("location=\".psocinit\"")
    
    #define CY_CFG_MEMORY_BARRIER() __DMB()
    
#else
    #error Unsupported toolchain
#endif

#ifndef CYCODE
    #define CYCODE
#endif
#ifndef CYDATA
    #define CYDATA
#endif
#ifndef CYFAR
    #define CYFAR
#endif
#ifndef CYXDATA
    #define CYXDATA
#endif


CY_CFG_UNUSED
static void CYMEMZERO(void *s, size_t n);
CY_CFG_UNUSED
static void CYMEMZERO(void *s, size_t n)
{
	(void)memset(s, 0, n);
}
CY_CFG_UNUSED
static void CYCONFIGCPY(void *dest, const void *src, size_t n);
CY_CFG_UNUSED
static void CYCONFIGCPY(void *dest, const void *src, size_t n)
{
	(void)memcpy(dest, src, n);
}
CY_CFG_UNUSED
static void CYCONFIGCPYCODE(void *dest, const void *src, size_t n);
CY_CFG_UNUSED
static void CYCONFIGCPYCODE(void *dest, const void *src, size_t n)
{
	(void)memcpy(dest, src, n);
}




/* Clock startup error codes                                                   */
#define CYCLOCKSTART_NO_ERROR    0u
#define CYCLOCKSTART_XTAL_ERROR  1u
#define CYCLOCKSTART_32KHZ_ERROR 2u
#define CYCLOCKSTART_PLL_ERROR   3u
#define CYCLOCKSTART_FLL_ERROR   4u
#define CYCLOCKSTART_WCO_ERROR   5u


#ifdef CY_NEED_CYCLOCKSTARTUPERROR
/*******************************************************************************
* Function Name: CyClockStartupError
********************************************************************************
* Summary:
*  If an error is encountered during clock configuration (crystal startup error,
*  PLL lock error, etc.), the system will end up here.  Unless reimplemented by
*  the customer, this function will stop in an infinite loop.
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/
CY_CFG_UNUSED
static void CyClockStartupError(uint8 errorCode);
CY_CFG_UNUSED
static void CyClockStartupError(uint8 errorCode)
{
    /* To remove the compiler warning if errorCode not used.                */
    errorCode = errorCode;

    /* If we have a clock startup error (bad MHz crystal, PLL lock, etc.),  */
    /* we will end up here to allow the customer to implement something to  */
    /* deal with the clock condition.                                       */

#ifdef CY_CFG_CLOCK_STARTUP_ERROR_CALLBACK
    CY_CFG_Clock_Startup_ErrorCallback();
#else
    while(1) {}
#endif /* CY_CFG_CLOCK_STARTUP_ERROR_CALLBACK */
}
#endif

#define CY_CFG_BASE_ADDR_COUNT 3u
CYPACKED typedef struct
{
	uint8 offset;
	uint8 value;
} CYPACKED_ATTR cy_cfg_addrvalue_t;



/*******************************************************************************
* Function Name: cfg_write_bytes32
********************************************************************************
* Summary:
*  This function is used for setting up the chip configuration areas that
*  contain relatively sparse data.
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/
static void cfg_write_bytes32(const uint32 addr_table[], const cy_cfg_addrvalue_t data_table[]);
static void cfg_write_bytes32(const uint32 addr_table[], const cy_cfg_addrvalue_t data_table[])
{
	/* For 32-bit little-endian architectures */
	uint32 i, j = 0u;
	for (i = 0u; i < CY_CFG_BASE_ADDR_COUNT; i++)
	{
		uint32 baseAddr = addr_table[i];
		uint8 count = (uint8)baseAddr;
		baseAddr &= 0xFFFFFF00u;
		while (count != 0u)
		{
			CY_SET_REG8((void *)(baseAddr + data_table[j].offset), data_table[j].value);
			j++;
			count--;
		}
	}
}
static void ClockInit(void);
static void ClockInit(void)
{
	uint32_t status;

	/* Enable all source clocks */
	status = CY_RET_SUCCESS;          // Removed enabling of WCO as it is already enabled by the Onethinx Core: status = Cy_SysClk_WcoEnable(500000u);
	if (CY_RET_SUCCESS != status)
	{
		CyClockStartupError(CYCLOCKSTART_WCO_ERROR);
	}
	Cy_SysClk_ClkLfSetSource(CY_SYSCLK_CLKLF_IN_WCO);

	/* Configure CPU clock dividers */
	Cy_SysClk_ClkFastSetDivider(0u);
	Cy_SysClk_ClkPeriSetDivider(0u);
	Cy_SysClk_ClkSlowSetDivider(0u);

	/* Configure LF & HF clocks */
	Cy_SysClk_ClkHfSetSource(0u, CY_SYSCLK_CLKHF_IN_CLKPATH0);
	Cy_SysClk_ClkHfSetDivider(0u, CY_SYSCLK_CLKHF_NO_DIVIDE);
	Cy_SysClk_ClkHfEnable(0u);

	/* Configure Path Clocks */
	Cy_SysClk_ClkPathSetSource(1, CY_SYSCLK_CLKPATH_IN_IMO);
	Cy_SysClk_ClkPathSetSource(2, CY_SYSCLK_CLKPATH_IN_IMO);
	Cy_SysClk_ClkPathSetSource(3, CY_SYSCLK_CLKPATH_IN_IMO);
	Cy_SysClk_ClkPathSetSource(4, CY_SYSCLK_CLKPATH_IN_IMO);
	Cy_SysClk_ClkPathSetSource(0, CY_SYSCLK_CLKPATH_IN_IMO);

	/* Configure miscellaneous clocks */
	Cy_SysClk_ClkTimerSetSource(CY_SYSCLK_CLKTIMER_IN_HF0_NODIV);
	Cy_SysClk_ClkTimerSetDivider(0);
	Cy_SysClk_ClkTimerEnable();
	Cy_SysClk_ClkPumpSetSource(CY_SYSCLK_PUMP_IN_CLKPATH0);
	Cy_SysClk_ClkPumpSetDivider(CY_SYSCLK_PUMP_NO_DIV);
	Cy_SysClk_ClkPumpEnable();
	Cy_SysClk_ClkBakSetSource(CY_SYSCLK_BAK_IN_WCO);
	Cy_SysTick_SetClockSource(CY_SYSTICK_CLOCK_SOURCE_CLK_LF);

	/* Disable clocks started by default */
	Cy_SysClk_IloDisable();

	/* Set memory wait states based on 8 MHz HFClk[0] */
	Cy_SysLib_SetWaitStates(false, 8);

	/* Configure peripheral clock dividers */
	Cy_SysClk_PeriphAssignDivider(PCLK_UDB_CLOCKS0, CY_SYSCLK_DIV_24_5_BIT, 0u);
	Cy_SysClk_PeriphSetFracDivider(CY_SYSCLK_DIV_24_5_BIT, 0u, 1599999u, 0u);
	Cy_SysClk_PeriphEnableDivider(CY_SYSCLK_DIV_24_5_BIT, 0u);
}


/* Analog API Functions */


/*******************************************************************************
* Function Name: AnalogSetDefault
********************************************************************************
*
* Summary:
*  Sets up the analog portions of the chip to default values based on chip
*  configuration options from the project.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void AnalogSetDefault(void);
static void AnalogSetDefault(void)
{
	CY_SET_REG32(CYREG_PASS_AREF_AREF_CTRL, 0x80110001u);
}




/*******************************************************************************
* Function Name: Cy_SystemInit
********************************************************************************
* Summary:
*  This function is called by the start-up code for the selected device. It
*  performs all of the necessary device configuration based on the design
*  settings.  This includes settings from the Design Wide Resources (DWR) such
*  as Clocks and Pins as well as any component configuration that is necessary.
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/

void Cy_SystemInit(void)
{
	CyDelay(1500); /* Failsafe guard: wrong clocksettings may brick the Onethinx module. Remove this delay in the release version. */

	/* Removed Onethinx Core conflicting code by Onethinx Creator PostBuild */

	//	/* Set worst case memory wait states (150 MHz), ClockInit() will update */
	Cy_SysLib_SetWaitStates(false, 150);
	//	Cy_SysLib_SetWaitStates(false, 150);
	//
	//	if(0u == Cy_SysLib_GetResetReason()) /* POR, XRES, or BOD */
	//	{
	//		Cy_SysLib_ResetBackupDomain();
	//		Cy_SysClk_IloDisable();
	//		Cy_SysClk_IloEnable();
	//	}
	//
	//	/* Power Mode */
	//	Cy_SysPm_LdoSetVoltage(CY_SYSPM_LDO_VOLTAGE_1_1V);
	//
	//	/* PMIC Control */
	//	Cy_SysPm_UnlockPmic();
	//	Cy_SysPm_DisablePmicOutput();
	/* Resume non-conflicting code by Onethinx Creator PostBuild */

	/* Perform Trigger Mux configuration */
	(void)Cy_TrigMux_Connect(TRIG14_IN_UDB_TR_UDB4, TRIG14_OUT_TR_GROUP2_INPUT42, CY_TR_MUX_TR_INV_DISABLE, TRIGGER_TYPE_LEVEL);
	(void)Cy_TrigMux_Connect(TRIG7_IN_TR_GROUP14_OUTPUT15, TRIG7_OUT_UDB_TR_IN1, CY_TR_MUX_TR_INV_DISABLE, TRIGGER_TYPE_UDB_TR_UDB__LEVEL);

	/* Pin0_0 configuration */
	{
	    const cy_stc_gpio_pin_config_t pin0_0_cfg =
	    {
	        .outVal    = 0x00u,
	        .driveMode = 0x00u,
	        .hsiom     = P0_0_GPIO,
	    };
	    (void)Cy_GPIO_Pin_Init(GPIO_PRT0, 0, &pin0_0_cfg);
	}

	/* Pin0_1 configuration */
	{
	    const cy_stc_gpio_pin_config_t pin0_1_cfg =
	    {
	        .outVal    = 0x00u,
	        .driveMode = 0x00u,
	        .hsiom     = P0_1_GPIO,
	    };
	    (void)Cy_GPIO_Pin_Init(GPIO_PRT0, 1, &pin0_1_cfg);
	}

	/* Clock */
	ClockInit();
	{
		static const uint32 CYCODE cy_cfg_addr_table[] = {
			0x40342203u, /* Base address: 0x40342200 Count: 3 */
			0x40347101u, /* Base address: 0x40347100 Count: 1 */
			0x40347802u, /* Base address: 0x40347800 Count: 2 */
		};

		static const cy_cfg_addrvalue_t CYCODE cy_cfg_data_table[] = {
			{0xBAu, 0xC0u},
			{0xDCu, 0x04u},
			{0xE3u, 0x01u},
			{0xECu, 0x0Cu},
			{0x00u, 0x01u},
			{0x10u, 0x01u},
		};



		CYPACKED typedef struct {
			void *address;
			uint16 size;
		} CYPACKED_ATTR cfg_memset_t;


		CYPACKED typedef struct {
			void *dest;
			const void *src;
			size_t size;
		} CYPACKED_ATTR cfg_memcpy_t;

		static const cfg_memset_t CYCODE cfg_memset_list[] = {
			/* address, size */
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR0_UDBSNG0_BASE), 116u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR0_UDBSNG1_BASE), 116u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR1_UDBSNG0_BASE), 116u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR1_UDBSNG1_BASE), 116u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR2_UDBSNG0_BASE), 116u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR2_UDBSNG1_BASE), 116u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR3_UDBSNG0_BASE), 116u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR3_UDBSNG1_BASE), 116u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR4_UDBSNG0_BASE), 116u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR4_UDBSNG1_BASE), 116u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR5_UDBSNG0_BASE), 116u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR5_UDBSNG1_BASE), 116u},
		};

		/* UDB_UDBPAIR0_ROUTE Address: CYDEV_UDB_UDBPAIR0_ROUTE_BASE Size (bytes): 144 */
		static const uint8 CYCODE BS_UDB_UDBPAIR0_ROUTE_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x04u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x07u, 0x06u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x60u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFBu, 0xFFu, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x10u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x01u, 0x11u};

		/* UDB_UDBPAIR1_ROUTE Address: CYDEV_UDB_UDBPAIR1_ROUTE_BASE Size (bytes): 144 */
		static const uint8 CYCODE BS_UDB_UDBPAIR1_ROUTE_VAL[] = {
			0x00u, 0x00u, 0x04u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xF3u, 0x4Fu, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xF7u, 0xFFu, 0xFFu, 0xFFu, 
			0xF6u, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x1Fu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x01u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x13u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x18u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x91u, 0x11u, 0x10u, 0x11u, 0x10u, 0x11u, 0x11u, 0x11u, 0x11u};

		/* UDB_UDBPAIR2_ROUTE Address: CYDEV_UDB_UDBPAIR2_ROUTE_BASE Size (bytes): 144 */
		static const uint8 CYCODE BS_UDB_UDBPAIR2_ROUTE_VAL[] = {
			0x00u, 0x00u, 0x02u, 0x60u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x02u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xF2u, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x00u, 0x00u, 0x00u, 0x0Eu, 0x1Fu, 0x07u, 0x00u, 
			0x06u, 0x1Fu, 0x00u, 0x1Fu, 0x0Du, 0x00u, 0x00u, 0x1Fu, 0x00u, 0x1Fu, 0x1Fu, 0x00u, 0x00u, 0x00u, 0xF0u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xDFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xB3u, 0xFFu, 
			0x11u, 0x11u, 0x11u, 0xC1u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x1Du, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x01u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		/* UDB_UDBPAIR3_ROUTE Address: CYDEV_UDB_UDBPAIR3_ROUTE_BASE Size (bytes): 144 */
		static const uint8 CYCODE BS_UDB_UDBPAIR3_ROUTE_VAL[] = {
			0x00u, 0x80u, 0x80u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x04u, 0xFFu, 0xFFu, 0xFFu, 0x60u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x07u, 0x06u, 
			0x00u, 0x1Fu, 0x00u, 0x1Fu, 0x00u, 0x06u, 0x00u, 0x1Fu, 0x08u, 0x00u, 0x1Fu, 0x00u, 0x00u, 0x00u, 0x0Fu, 0x0Fu, 
			0x00u, 0x00u, 0x00u, 0xF0u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFBu, 0xFFu, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x01u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x10u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x01u, 0x11u, 0x11u, 0x11u, 0x10u, 0x11u};

		/* UDB_UDBPAIR4_ROUTE Address: CYDEV_UDB_UDBPAIR4_ROUTE_BASE Size (bytes): 144 */
		static const uint8 CYCODE BS_UDB_UDBPAIR4_ROUTE_VAL[] = {
			0x20u, 0x04u, 0x98u, 0x92u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x00u, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x00u, 0x1Fu, 0x07u, 0x1Fu, 0x00u, 0x00u, 0x1Fu, 0x1Fu, 0x0Cu, 0x1Fu, 0x00u, 0x00u, 0xF0u, 0x00u, 
			0x00u, 0x0Fu, 0x00u, 0x00u, 0xFFu, 0xDFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFEu, 0xDFu, 0x6Fu, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		/* UDB_UDBPAIR5_ROUTE Address: CYDEV_UDB_UDBPAIR5_ROUTE_BASE Size (bytes): 144 */
		static const uint8 CYCODE BS_UDB_UDBPAIR5_ROUTE_VAL[] = {
			0x00u, 0x00u, 0x02u, 0x90u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0x7Fu, 0xF7u, 
			0x67u, 0x66u, 0xFFu, 0xFFu, 0xF6u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xF7u, 0x77u, 0x1Fu, 0xFFu, 0xFFu, 0xFFu, 
			0x7Fu, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x00u, 0x00u, 0x00u, 0x06u, 0x1Fu, 0x00u, 0x1Fu, 
			0x00u, 0x1Fu, 0x00u, 0x1Fu, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x07u, 0x00u, 0x1Fu, 0x00u, 0x06u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 
			0x11u, 0x11u, 0x1Bu, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x13u, 0x11u, 0x18u, 0x11u, 0xE1u, 0x12u, 0x91u, 0xE1u, 0x12u, 0x97u, 0xE1u, 0x62u};

		/* UDB_DSI0 Address: CYDEV_UDB_DSI0_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI0_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		/* UDB_DSI1 Address: CYDEV_UDB_DSI1_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI1_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x00u, 0x00u, 0x1Fu, 0x00u, 0x00u, 0x00u, 0x00u, 0x0Fu, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0xBFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		/* UDB_DSI2 Address: CYDEV_UDB_DSI2_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI2_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x20u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x0Cu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x17u, 0x1Fu, 0x1Fu, 0x14u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x00u, 0x1Fu, 0x00u, 0x00u, 0x1Fu, 0x0Fu, 0x00u, 0x0Fu, 
			0x1Fu, 0x00u, 0x00u, 0x06u, 0x0Cu, 0x00u, 0x00u, 0x00u, 0x05u, 0x00u, 0x00u, 0x00u, 0x00u, 0x70u, 0x00u, 0x00u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xBFu, 0xFFu, 0x11u, 0x11u, 0x11u, 0x10u, 
			0x17u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0xD1u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0xE1u, 0x11u, 0x18u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0xD1u, 0x11u, 0x11u, 0xEEu, 0xB1u, 0x11u, 0x11u};

		/* UDB_DSI3 Address: CYDEV_UDB_DSI3_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI3_VAL[] = {
			0x00u, 0x00u, 0x08u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x0Du, 0x1Fu, 0x1Fu, 0x00u, 0x0Eu, 0x00u, 0x00u, 0x00u, 0x0Fu, 0x00u, 0x1Fu, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x0Eu, 0x00u, 0x00u, 0x00u, 0x07u, 0x07u, 0x00u, 0x70u, 0x00u, 0xF0u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x11u, 0x11u, 0x11u, 0x10u, 
			0xD1u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0xD1u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x1Du, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0xF0u, 0xE1u, 0xC1u, 0x11u, 0xF1u};

		/* UDB_DSI4 Address: CYDEV_UDB_DSI4_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI4_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x0Eu, 0x00u, 0x0Eu, 0x1Fu, 0x1Fu, 0x0Fu, 0x00u, 0x1Fu, 
			0x1Fu, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x0Fu, 0x1Fu, 0x00u, 0x00u, 0x00u, 0xF0u, 0x00u, 0x00u, 0x0Fu, 0x00u, 0x00u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFEu, 0xFDu, 0xF7u, 0xFFu, 0xBFu, 0xFFu, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0xF1u, 0x11u, 0x11u, 0x1Eu, 0x11u, 0x11u, 0x11u, 0xD1u, 0x11u, 0x1Du, 0x11u, 0xE1u};

		/* UDB_DSI5 Address: CYDEV_UDB_DSI5_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI5_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x04u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x0Cu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x0Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x00u, 0x1Fu, 0x00u, 0x00u, 0x00u, 0x07u, 0x00u, 0x07u, 
			0x00u, 0x00u, 0x06u, 0x07u, 0x1Fu, 0x00u, 0x07u, 0x1Fu, 0x00u, 0x5Fu, 0x00u, 0x40u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xEFu, 0xFFu, 0xFEu, 0x11u, 0x11u, 0x11u, 0xD1u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x10u, 0x11u, 0x11u, 0x11u, 0x01u, 0x11u, 0x11u, 0x11u, 0x11u, 0x01u, 0x11u, 0x11u, 
			0xE6u, 0x11u, 0x11u, 0x11u, 0x11u, 0x1Eu, 0xD1u, 0xE0u, 0xD1u, 0x19u, 0x11u, 0x1Eu};

		/* UDB_DSI6 Address: CYDEV_UDB_DSI6_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI6_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0xF0u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x1Du, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0xE1u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		/* UDB_DSI7 Address: CYDEV_UDB_DSI7_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI7_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x07u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x0Du, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x17u, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x00u, 0x00u, 0x0Fu, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x17u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x1Fu, 0x11u, 0x11u, 0x11u, 0x11u, 0xE1u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x01u, 0x01u};

		/* UDB_DSI8 Address: CYDEV_UDB_DSI8_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI8_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x40u, 0x06u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x17u, 0x0Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x05u, 0x1Fu, 0x06u, 0x00u, 0x00u, 0x00u, 0x1Fu, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x0Au, 0x06u, 0x04u, 0x0Du, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xDFu, 0xFFu, 0xBFu, 0xDFu, 0xFBu, 0x5Fu, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x71u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x17u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x01u, 0x11u, 0x11u, 0x10u, 0x11u, 0x11u, 0x11u};

		/* UDB_DSI9 Address: CYDEV_UDB_DSI9_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI9_VAL[] = {
			0x00u, 0x00u, 0x20u, 0x00u, 0x00u, 0x00u, 0x00u, 0x10u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x00u, 0x1Fu, 0x00u, 0x1Fu, 0x00u, 0x1Fu, 0x00u, 0x0Cu, 
			0x00u, 0x1Fu, 0x0Du, 0x1Fu, 0x1Fu, 0x00u, 0x1Fu, 0x00u, 0x00u, 0x00u, 0x5Fu, 0x0Fu, 0x00u, 0x00u, 0x00u, 0x60u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xF7u, 0xFEu, 0x11u, 0x11u, 0x11u, 0x11u, 
			0xE1u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0xD1u, 0x11u, 0x11u, 0x11u, 
			0x10u, 0xD1u, 0x11u, 0x11u, 0x11u, 0x11u, 0x1Eu, 0xE1u, 0x11u, 0x1Du, 0xE1u, 0xEEu};

		/* UDB_DSI10 Address: CYDEV_UDB_DSI10_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI10_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x00u, 0x00u, 0x0Fu, 0x08u, 0x00u, 0x00u, 0x0Fu, 
			0x00u, 0x0Eu, 0x00u, 0x00u, 0x0Fu, 0x00u, 0x00u, 0x0Fu, 0x00u, 0x00u, 0x70u, 0x00u, 0x00u, 0x07u, 0x00u, 0x00u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFEu, 0xFFu, 0xFFu, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x15u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0xF1u, 0x11u, 0x1Fu, 0x11u, 0x11u};

		/* UDB_DSI11 Address: CYDEV_UDB_DSI11_BASE Size (bytes): 124 */
		static const uint8 CYCODE BS_UDB_DSI11_VAL[] = {
			0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x50u, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 
			0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x1Fu, 0x07u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0x00u, 0x00u, 0x00u, 0x00u, 0x06u, 0x1Fu, 0x0Cu, 0x07u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 
			0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFBu, 0xBEu, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 
			0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u, 0x11u};

		static const cfg_memcpy_t CYCODE cfg_memcpy_list [] = {
			/* dest, src, size */
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR0_ROUTE_BASE), BS_UDB_UDBPAIR0_ROUTE_VAL, 144u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR1_ROUTE_BASE), BS_UDB_UDBPAIR1_ROUTE_VAL, 144u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR2_ROUTE_BASE), BS_UDB_UDBPAIR2_ROUTE_VAL, 144u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR3_ROUTE_BASE), BS_UDB_UDBPAIR3_ROUTE_VAL, 144u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR4_ROUTE_BASE), BS_UDB_UDBPAIR4_ROUTE_VAL, 144u},
			{(void CYFAR *)(CYDEV_UDB_UDBPAIR5_ROUTE_BASE), BS_UDB_UDBPAIR5_ROUTE_VAL, 144u},
			{(void CYFAR *)(CYDEV_UDB_DSI0_BASE), BS_UDB_DSI0_VAL, 124u},
			{(void CYFAR *)(CYDEV_UDB_DSI1_BASE), BS_UDB_DSI1_VAL, 124u},
			{(void CYFAR *)(CYDEV_UDB_DSI2_BASE), BS_UDB_DSI2_VAL, 124u},
			{(void CYFAR *)(CYDEV_UDB_DSI3_BASE), BS_UDB_DSI3_VAL, 124u},
			{(void CYFAR *)(CYDEV_UDB_DSI4_BASE), BS_UDB_DSI4_VAL, 124u},
			{(void CYFAR *)(CYDEV_UDB_DSI5_BASE), BS_UDB_DSI5_VAL, 124u},
			{(void CYFAR *)(CYDEV_UDB_DSI6_BASE), BS_UDB_DSI6_VAL, 124u},
			{(void CYFAR *)(CYDEV_UDB_DSI7_BASE), BS_UDB_DSI7_VAL, 124u},
			{(void CYFAR *)(CYDEV_UDB_DSI8_BASE), BS_UDB_DSI8_VAL, 124u},
			{(void CYFAR *)(CYDEV_UDB_DSI9_BASE), BS_UDB_DSI9_VAL, 124u},
			{(void CYFAR *)(CYDEV_UDB_DSI10_BASE), BS_UDB_DSI10_VAL, 124u},
			{(void CYFAR *)(CYDEV_UDB_DSI11_BASE), BS_UDB_DSI11_VAL, 124u},
		};

		size_t i;

		/* Power on the UDB array */
		CY_SET_REG32(0x402101F0u, 0x05FA0003u);

		/* Zero out critical memory blocks before beginning configuration */
		for (i = 0u; i < (sizeof(cfg_memset_list)/sizeof(cfg_memset_list[0])); i++)
		{
			const cfg_memset_t *ms = &cfg_memset_list[i];
			CYMEMZERO(ms->address, ms->size);
		}

		/* Copy device configuration data into registers */
		for (i = 0u; i < (sizeof(cfg_memcpy_list)/sizeof(cfg_memcpy_list[0])); i++)
		{
			const cfg_memcpy_t *mc = &cfg_memcpy_list[i];
			CYCONFIGCPYCODE(mc->dest, mc->src, mc->size);
		}

		cfg_write_bytes32(cy_cfg_addr_table, cy_cfg_data_table);

		/* Enable UDB array and digital routing */
		CY_SET_REG32((void *)0x40347900u, CY_GET_REG32((void *)0x40347900u) | 0x06u);
	}

	/* Port0 configuration */
	{
	    const cy_stc_gpio_prt_config_t port0_cfg =
	    {
	        .out        = 0x00000000u,
	        .intrMask   = 0x00000000u,
	        .intrCfg    = 0x00000000u,
	        .cfg        = 0x00000000u,
	        .cfgIn      = 0x00000000u,
	        .cfgOut     = 0x00000000u,
	        .cfgSIO     = 0x00000000u,
	        .sel0Active = 0x00000000u,
	        .sel1Active = 0x00000000u,
	    };
	    (void)Cy_GPIO_Port_Init(GPIO_PRT0, &port0_cfg);
	}

	/* Port12 configuration */
	{
	    const cy_stc_gpio_prt_config_t port12_cfg =
	    {
	        .out        = 0x00000020u,
	        .intrMask   = 0x00000000u,
	        .intrCfg    = 0x00000000u,
	        .cfg        = 0x00660000u,
	        .cfgIn      = 0x00000000u,
	        .cfgOut     = 0x00000000u,
	        .cfgSIO     = 0x00000000u,
	        .sel0Active = 0x00000000u,
	        .sel1Active = 0x00000300u,
	    };
	    (void)Cy_GPIO_Port_Init(GPIO_PRT12, &port12_cfg);
	}


	/* Perform basic analog initialization to defaults */
	AnalogSetDefault();

}
