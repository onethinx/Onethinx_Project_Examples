
/********************************************************************************
 *    ___             _   _     _			
 *   / _ \ _ __   ___| |_| |__ (_)_ __ __  __
 *  | | | | '_ \ / _ \ __| '_ \| | '_ \\ \/ /
 *  | |_| | | | |  __/ |_| | | | | | | |>  < 
 *   \___/|_| |_|\___|\__|_| |_|_|_| |_/_/\_\
 *
 ********************************************************************************
 *
 * Copyright (c) 2019 Onethinx BV <info@onethinx.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ********************************************************************************
 *
 * Created by: Rolf Nooteboom on 2020-10-09
 *
 * Sample project to demostrate the integration of a PSoC Creator project
 * into Visual Studio Code
 * 
 * For a description please see:
 * https://github.com/onethinx/Onethinx_Creator
 *
 ********************************************************************************/

#include "project.h"
#include "OnethinxCore01.h"
#include "OnethinxExt01.h"
#include "LoRaWAN_keys.h"

/* Go to ../OnethinxCore/LoRaWAN_keys.h and fill in the fields of the TTN_OTAAkeys structure */

coreConfiguration_t	coreConfig = {
	.Join.KeysPtr = 		&TTN_OTAAkeys,
	.Join.DataRate =		DR_AUTO,
	.Join.Power =			PWR_MAX,
	.Join.MAXTries = 		100,
    .Join.SubBand_1st =     EU_SUB_BANDS_DEFAULT,
	.Join.SubBand_2nd =     EU_SUB_BANDS_DEFAULT,
	.TX.Confirmed = 		false,
	.TX.DataRate = 			DR_ADR,		// Adaptive Data Rate
	.TX.Power = 			PWR_ADR,	// Adaptive Data Rate
	.TX.FPort = 			1,
	.System.Idle.Mode = 	M0_DeepSleep,
	.System.Idle.BleEcoON = false,
	.System.Idle.DebugON =  true,
};

sleepConfig_t sleepConfig =
{
	.sleepMode = modeDeepSleep,
	.BleEcoON = false,
	.DebugON = true,
	.sleepCores = coresBoth,
	.wakeUpPin = wakeUpPinHigh(true),
	.wakeUpTime = wakeUpDelay(0, 0, 0, 2), // day, hour, minute, second
};

/*******************************************************************************arm-none-eabi-gcc
* Function Name: main()
********************************************************************************
* Summary:
*  Sends a "Hello World" message using LoRaWAN.
* Details:
*  Go to ../OnethinxCore/LoRaWAN_keys.h and fill in the fields of the TTN_OTAAkeys structure
*  Read the quickstart guide on how to create keys.
*******************************************************************************/

void Test_If_This_DevKit_Supports_CapSense();

/* OnethinxCore uses the following structures and variables, which can be defined globally */
coreStatus_t 	coreStatus;
coreInfo_t 		coreInfo;

#define TXbuffer "Onethinx Capsense"

uint8_t wakeUpCounter = 0;

#include <cyfitter_cfg.h>

int main(void)
 {
	CyDelay(2000); // Needs to be here
	//Cy_SystemInit();

	/* enable global interrupts */
	__enable_irq();

	// /* Blue LED ON while joining*/
	Cy_GPIO_Write(LED_B_PORT, LED_B_NUM, 1);

	/* initialize radio with parameters in coreConfig */
	coreStatus = LoRaWAN_Init(&coreConfig);

	/* Ulock port 7 which is used for CapSense. */
	LoRaWAN_Unlock();

	/* You can use this fuction to see if your DevKit supports CapSense. P10.2 and P7.7 are shorted (IO_3). Older modules do not support CapSense*/
	Test_If_This_DevKit_Supports_CapSense();

	Cy_GPIO_Pin_FastInit(P10_2_PORT, P10_2_NUM, CY_GPIO_DM_HIGHZ, 0, CY_GPIO_DM_ANALOG);

	/* Check Onethinx Core info */
	LoRaWAN_GetInfo(&coreInfo);

	/* send join using parameters in coreConfig, blocks until either success or MAXtries */
	coreStatus = LoRaWAN_Join(M4_WaitDeepSleep);

	/* Blue LED OFF after joining*/
	Cy_GPIO_Write(LED_B_PORT, LED_B_NUM, 0);

	/* check for successful join, flash Blue LED if not joined */
	if (!coreStatus.mac.isJoined){
		
		while(1) {
			Cy_GPIO_Write(LED_B_PORT, LED_B_NUM, 0);
			CyDelay(100);
			Cy_GPIO_Write(LED_B_PORT, LED_B_NUM, 1);
			CyDelay(100);
		}
	}

	/* Starts and configures EZI2C Block for CapSense tuning */
	// I2C_CapSense_Start();
	// I2C_CapSense_SetBuffer1((uint8_t *)&CapSense_dsRam,
    //                     	sizeof(CapSense_dsRam),
    //                     	sizeof(CapSense_dsRam));


	CapSense_Start();
	CapSense_InitializeAllBaselines();
	CapSense_ScanAllWidgets();
	volatile int status = 0;
	/* main loop */
	for(;;)
	{
		if (!CapSense_IsBusy())
		{
			CapSense_ProcessAllWidgets();
			// CapSense_RunTuner();
			status = CapSense_IsSensorActive(CapSense_BUTTON0_WDGT_ID, CapSense_BUTTON0_SNS0_ID);
			Cy_GPIO_Write(LED_B_PORT, LED_B_NUM, status);
			if (status) wakeUpCounter++;
			else wakeUpCounter = 0;
			CapSense_UpdateAllBaselines();
			CapSense_ScanAllWidgets();
		}

		if (wakeUpCounter > 5)
		{
			/* Reset wakeUpCounter */
			wakeUpCounter = 0;

			/* Blue LED on while sending*/
			Cy_GPIO_Write(LED_B_PORT, LED_B_NUM, 1);

			/* Send message over LoRaWAN */
			coreStatus = LoRaWAN_Send((uint8_t *) TXbuffer, sizeof (TXbuffer), M4_WaitDeepSleep);

			/* Turn led off before sleep */
			Cy_GPIO_Write(LED_B_PORT, LED_B_NUM, 0);
		}

		/* Sleep before sending next message, wake up with a button as well */
		LoRaWAN_Sleep(&sleepConfig);
	}
}

void Test_If_This_DevKit_Supports_CapSense()
{
	bool error = false;
	Cy_GPIO_Write(LED_B_PORT, LED_B_NUM, 1);
	Cy_GPIO_Pin_FastInit(P7_7_PORT, P7_7_NUM, CY_GPIO_DM_PULLUP, 1UL, HSIOM_SEL_GPIO);
	Cy_GPIO_Pin_FastInit(P10_2_PORT, P10_2_NUM, CY_GPIO_DM_STRONG, 0UL, HSIOM_SEL_GPIO);
	if (Cy_GPIO_Read(P7_7_PORT, P7_7_NUM) != 0) 
		error = true;
	Cy_GPIO_Pin_FastInit(P7_7_PORT, P7_7_NUM, CY_GPIO_DM_PULLDOWN, 0UL, HSIOM_SEL_GPIO);
	Cy_GPIO_Write(P10_2_PORT, P10_2_NUM, 1);
	if (Cy_GPIO_Read(P7_7_PORT, P7_7_NUM) == 0) 
		error = true;
	Cy_GPIO_Write(LED_B_PORT, LED_B_NUM, 0);
	if (error) while (1);
}
/* [] END OF FILE */
