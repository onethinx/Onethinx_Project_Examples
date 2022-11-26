
/********************************************************************************
 *    ___             _   _     _			
 *   / _ \ _ __   ___| |_| |__ (_)_ __ __  __
 *  | | | | '_ \ / _ \ __| '_ \| | '_ \\ \/ /
 *  | |_| | | | |  __/ |_| | | | | | | |>  < 
 *   \___/|_| |_|\___|\__|_| |_|_|_| |_/_/\_\
 *
 ********************************************************************************
 *
 * Copyright (c) 2019-2022 Onethinx BV <info@onethinx.com>
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
	.System.Idle.DebugON  = true,
	.RX.Boost			  = false,
};

sleepConfig_t sleepConfig =
{
	.sleepMode = modeDeepSleep,
	.BleEcoON = false,
	.DebugON = true,
	.sleepCores = coresBoth,
	.wakeUpPin = wakeUpPinHigh(true),
	.wakeUpTime = wakeUpDelay(0, 0, 0, 20), // day, hour, minute, second
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

/* OnethinxCore uses the following structures and variables, which can be defined globally */
coreStatus_t 	coreStatus;
coreInfo_t 		coreInfo;

uint8_t RXbuffer[64];
uint8_t TXbuffer[64];

#include <cyfitter_cfg.h>

int main(void)
{
	uint8_t j=0;


	/* enable global interrupts */
	__enable_irq();

	/* Blue LED ON while joining*/
	Cy_GPIO_Write(LED_B_PORT, LED_B_NUM, 1);

	/* initialize radio with parameters in coreConfig */
	coreStatus = LoRaWAN_Init(&coreConfig);

	/* Check Onethinx Core info */
	LoRaWAN_GetInfo(&coreInfo);

	/* send join using parameters in coreConfig, blocks until either success or MAXtries */
	coreStatus = LoRaWAN_Join(M4_WaitDeepSleep);

	/* check for successful join, flash Red LED if not joined */
	if (!coreStatus.mac.isJoined){
		
		while(1) {
			Cy_GPIO_Write(LED_B_PORT, LED_B_NUM, 0);
			CyDelay(100);
			Cy_GPIO_Write(LED_B_PORT, LED_B_NUM, 1);
			CyDelay(100);
		}
	}

	/* main loop */
	for(;;)
	{
		/* Blue LED on while sending*/
		Cy_GPIO_Write(LED_B_PORT, LED_B_NUM, 1);

		/* Compose a message to send */
        j=0;
        TXbuffer[j++] = 0x48; /* H */
		TXbuffer[j++] = 0x45; /* E */
		TXbuffer[j++] = 0x4c; /* L */
		TXbuffer[j++] = 0x4c; /* L */
		TXbuffer[j++] = 0x4f; /* O */
		TXbuffer[j++] = 0x20; /*   */
		TXbuffer[j++] = 0x57; /* W */
		TXbuffer[j++] = 0x4f; /* O */
		TXbuffer[j++] = 0x52; /* R */
		TXbuffer[j++] = 0x4c; /* L */
		TXbuffer[j++] = 0x44; /* D */

		/* Send message over LoRaWAN */
        coreStatus = LoRaWAN_Send(TXbuffer, j, M4_WaitDeepSleep);

		/* Turn led off before sleep */
		Cy_GPIO_Write(LED_B_PORT, LED_B_NUM, 0);

		/* Sleep before sending next message, wake up with a button as well */
		LoRaWAN_Sleep(&sleepConfig);
	}
}


/* [] END OF FILE */
